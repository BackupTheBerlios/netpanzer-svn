#include <config.h>

#include "MasterServer.hpp"

#include <stdexcept>
#include <sstream>
#include <iostream>

#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "Tokenizer.hpp"
#include "SocketStream.hpp"
#include "Log.hpp"

#define PORT 28900

#define SERVERTIMEOUT       12*60
#define REQUESTTIMEOUT      30
#define CONNECTIONLIMIT     15

namespace masterserver
{

MasterServer::MasterServer()
{   
    initializeLog();
    
    sock = -1;
    try {
        // bind to socket
        sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
        if(sock < 0) {
            std::stringstream msg;
            msg << "Couldn't create socket: " << strerror(errno);
            throw std::runtime_error(msg.str());
        }
       
        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(struct sockaddr_in));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(PORT);
        int res = bind(sock, (struct sockaddr*) &addr,
                sizeof(struct sockaddr_in));
        if(res < 0) {
            std::stringstream msg;
            msg << "Couldn't bind socket to port " << PORT << " : "
                << strerror(errno);
            throw std::runtime_error(msg.str());
        }

        res = listen(sock, 20);
        if(res < 0) {
            std::stringstream msg;
            msg << "Couldn't listen on socket: " << strerror(errno);
            throw std::runtime_error(msg.str());
        }
        
        pthread_mutex_init(&serverlist_mutex, 0);
        
    } catch(...) {
        if(sock >= 0)
            close(sock);
        throw;
    }
}

MasterServer::~MasterServer()
{
    close(sock);
    closeLog();
}

void MasterServer::run()
{
    while(1) {
        // wait for clients...
        struct sockaddr_in clientaddr;
        socklen_t socklen = sizeof(struct sockaddr_in);
        int clientsock = accept(sock, (struct sockaddr*) &clientaddr,
                &socklen);
        if(clientsock < 0) {
            std::stringstream msg;
            msg << "Accept error: " << strerror(errno);
            throw std::runtime_error(msg.str());
        }

        // remove too old threads
        std::list<RequestThread*>::iterator i;
        time_t currenttime = time(0);
        for(i = threads.begin(); i != threads.end(); ) {
            if(currenttime - (*i)->getStartTime() > REQUESTTIMEOUT) {
                delete *i;
                i = threads.erase(i);
            } else {
                ++i;
            }               
        }
        if(threads.size() > REQUESTTIMEOUT) {
            // drop the client, we're too busy
            close(clientsock);
            continue;
        }

        SocketStream* stream = new SocketStream(clientsock);
        RequestThread* thread = new RequestThread(this, stream, clientaddr);
        threads.push_back(thread);
    }
}

void MasterServer::parseHeartbeat(std::iostream& stream,
        struct sockaddr_in* addr, Tokenizer& tokenizer)
{
    std::string token = tokenizer.getNextToken();
    std::stringstream strstream(token);
    
    int queryport;
    strstream >> queryport;

    pthread_mutex_lock(&serverlist_mutex);

    std::vector<ServerInfo>::iterator i;
    for(i = serverlist.begin(); i != serverlist.end(); ++i) {
        if(i->address.sin_addr.s_addr == addr->sin_addr.s_addr
            && i->address.sin_port == htons(queryport)) {
            updateServerInfo(*i, tokenizer);
            *log << "Heartbeat from " << inet_ntoa(addr->sin_addr) << std::endl;
            stream << "\\ok\\server updated\\final\\" << std::flush;
            pthread_mutex_unlock(&serverlist_mutex);
            return;
        }
    }

    // create a new list entry
    ServerInfo info;
    memset(&info.address, 0, sizeof(info.address));
    info.address.sin_family = AF_INET;
    info.address.sin_addr.s_addr = addr->sin_addr.s_addr;
    info.address.sin_port = htons(queryport);
    updateServerInfo(info, tokenizer);
    serverlist.push_back(info);
    *log << "New server at " << inet_ntoa(addr->sin_addr) << std::endl;
    stream << "\\ok\\server added\\final\\" << std::flush;

    pthread_mutex_unlock(&serverlist_mutex);
}

void MasterServer::updateServerInfo(ServerInfo& info, Tokenizer& tokenizer)
{    
    if(tokenizer.getNextToken() != "gamename") {
        *log << "Invalid heartbeat query." << std::endl;
        return;
    }
    info.gamename = tokenizer.getNextToken();
    info.lastheartbeat = time(0);
}

void MasterServer::parseList(std::iostream& stream,
        struct sockaddr_in* addr, Tokenizer& tokenizer)
{
    time_t currenttime = time(0);
    std::vector<ServerInfo>::iterator i;

    // get the name of the game
    std::string token = tokenizer.getNextToken();
    if(token != "gamename") {
        *log << "Malformed list query: gamename missing (" <<
            inet_ntoa(addr->sin_addr) << std::endl;
        return;
    }
    std::string gamename = tokenizer.getNextToken();
    *log << "List query '" << gamename << "' from "
         << inet_ntoa(addr->sin_addr) << std::endl;

    pthread_mutex_lock(&serverlist_mutex);
    for(i=serverlist.begin(); i != serverlist.end(); /* nothing */) {
        if(currenttime - i->lastheartbeat <= SERVERTIMEOUT) {
            if(i->gamename == gamename) {
                stream << "\\ip\\" << inet_ntoa(i->address.sin_addr)
                    << ":" << ntohs(i->address.sin_port);
            }
            ++i;
        } else {
            // remove server from list
            *log << "timeout from " << inet_ntoa(addr->sin_addr) << std::endl;
            i = serverlist.erase(i);
        }
    }
    pthread_mutex_unlock(&serverlist_mutex);
    
    stream << "\\final\\";
}

}
