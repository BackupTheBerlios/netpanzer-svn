/*
Copyright (C) 2004 Matthias Braun <matze@braunis.de>
 
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.
 
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
 
You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
#include <config.h>

#include "HeartbeatThread.hpp"

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <sstream>

#include <time.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "Log.hpp"
#include "Config.hpp"
#include "SocketStream.hpp"
#include "MasterServer.hpp"
#include "Tokenizer.hpp"

namespace masterserver
{

HeartbeatThread::HeartbeatThread(MasterServer* newmasterserver)
    : masterserver(newmasterserver), running(false)
{
    // try to get list of masterservers
    requestMasterServerList();
   
    if(serveraddresses.size() == 0) {
        *log << "No additional masterservers found.\n";
    }
    
    // start thread
    pthread_create(&thread, 0, threadMain, this);
}

HeartbeatThread::~HeartbeatThread()
{
    // signal thread to stop
    running = false;
    pthread_cancel(thread);
    pthread_join(thread, 0);
}

void
HeartbeatThread::readNeighborCache(std::vector<std::string>& list)
{
    iniparser::Store neighborini;

    const std::string& neighborcachefile =
        config->getSection("server").getValue("neighborcachefile");
    std::ifstream in(neighborcachefile.c_str());
    if(!in.good()) {
        *log << "Couldn't open neighborcache file '" << neighborcachefile
            << "'.\n";
        return;
    }
    neighborini.load(in);

    iniparser::Section& neighbors = neighborini.getSection("neighbors");
    for(int i=0;  ; ++i) {
        std::stringstream keyname;
        keyname << "ip" << i;
        
        if(!neighbors.exists(keyname.str()))
            break;
        
        const std::string& ip = neighbors.getValue(keyname.str());
        list.push_back(ip);
    }
}

void HeartbeatThread::requestMasterServerList()
{
    std::vector<std::string> addresses;
    
    // parse config
    readNeighborCache(addresses);

    // query addresses for masterserverlist
    bool havelist = false;
    for(std::vector<std::string>::iterator addr = addresses.begin();
            addr != addresses.end(); ++addr) {
        // lookup server address
        struct hostent* hentry = gethostbyname(addr->c_str());
        if(!hentry) {
            *log << "Couldn't resolve address of masterserver '"
                << *addr << "'\n";
            continue;
        }

        struct sockaddr_in serveraddr;
        memset(&serveraddr, 0, sizeof(serveraddr));
        serveraddr.sin_family = AF_INET;
        serveraddr.sin_addr.s_addr = ((struct in_addr*) hentry->h_addr)->s_addr;
        serveraddr.sin_port =
            htons(config->getSection("server").getIntValue("port"));
        
        requestMasterServerList2(&serveraddr); 
    }
}

void HeartbeatThread::requestMasterServerList2(struct sockaddr_in* addr)
{
    int sock = -1;

    try {
        sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
        if(sock < 0) {
            std::stringstream msg;
            msg << "Couldn't create socket: " << strerror(errno);
            throw std::runtime_error(msg.str());
        }
        
        // connect to server
        int res = connect(sock, (struct sockaddr*) addr,
                sizeof(struct sockaddr_in));
        if(res < 0) {
            std::stringstream msg;
            msg << "Couldn't connect to masterserver: "
                << strerror(errno) << "\n";
            throw std::runtime_error(msg.str());
        }

        SocketStream stream(sock);
        sock = -1; // stream has control over the socket now
        
        stream << "\\list\\gamename\\master\\final\\" << std::flush;
        Tokenizer* tokenizer = new Tokenizer(stream);
        while(!stream.eof()) {
            std::string token = tokenizer->getNextToken();
            if(token == "ip") {
                addMasterServer(tokenizer->getNextToken());
            } else if(token == "port") {
                // ignore always assume 28900
                tokenizer->getNextToken();
            } else if(token == "final") {
                break;
            } else {
                *log << "Unknown token when querying for masterserverlist: " <<
                    token << "\n";
            }
        }

        delete tokenizer;
    } catch(std::exception& e) {
        if(sock>=0)
            close(sock);
        *log << "Couldn't request a list of other masterservers: " << e.what()
            << "\n";
    } catch(...) {
        if(sock>=0)
            close(sock);
        throw;
    }
}

void
HeartbeatThread::addMasterServer(const std::string& address)
{
    // lookup server address
    struct hostent* hentry = gethostbyname(address.c_str());
    if(!hentry) {
        *log << "Couldn't lookup address of masterserver '"
            << address << "'\n";
        return;
    }

    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(struct sockaddr_in));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = ((struct in_addr*) hentry->h_addr)->s_addr;
    serveraddr.sin_port =
        htons(config->getSection("server").getIntValue("port"));
    
    serveraddresses.push_back(serveraddr);

    masterserver->addServer("master", serveraddr);
}

void* HeartbeatThread::threadMain(void* data)
{
    HeartbeatThread* _this = reinterpret_cast<HeartbeatThread*> (data);
    
    _this->running = true;
    while(_this->running) {
        _this->sendHeartbeats();

        // use nanosleep to have a thread cancelation point
        timespec sleeptime = {
            config->getSection("server").
                getIntValue("masterserver-heartbeat-interval"), 0 };
        nanosleep(&sleeptime, 0);
    }

    return 0;
}

void
HeartbeatThread::sendHeartbeats()
{
    for(std::vector<struct sockaddr_in>::iterator i = serveraddresses.begin();
            i != serveraddresses.end(); ++i) {
        try {
            sendHeartbeat(& (*i));
        } catch(std::exception& e) {
            *log << "Couldn't send heartbeat packet: " << e.what() << "\n";
        } catch(...) {
            *log << "Unexpected exception while sending heartbeat.\n";
        }
    }
}

void
HeartbeatThread::sendHeartbeat(struct sockaddr_in* serveraddr)
{
    int sock = -1;
    int res;
    try {
        sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
        if(sock < 0) {
            std::stringstream msg;
            msg << "Couldn't create socket: " << strerror(errno);
            throw std::runtime_error(msg.str());
        }
        
        // connect to server
        res = connect(sock, (struct sockaddr*) serveraddr,
                sizeof(struct sockaddr_in));
        if(res < 0) {
            std::stringstream msg;
            msg << "Couldn't connect to masterserver: "
                << strerror(errno) << "\n";
            throw std::runtime_error(msg.str());
        }

        // send heartbeat packet
        SocketStream stream(sock);
        sock = -1; // stream has control of socket now
        
        stream << "\\heartbeat\\" <<
            config->getSection("server").getValue("port")
               << "\\gamename\\master\\passwort\\" << 
               config->getSection("server").getValue("masterserver-password")
               << "\\final\\" << std::flush;
    } catch(...) {
        if(sock>=0)
            close(sock);
        throw;
    }
}

} // end of namespace masterserver
