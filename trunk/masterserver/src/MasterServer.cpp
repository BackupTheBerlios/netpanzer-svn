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

#include "MasterServer.hpp"

#include <stdexcept>
#include <sstream>
#include <fstream>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "Tokenizer.hpp"
#include "SocketStream.hpp"
#include "Log.hpp"
#include "Config.hpp"
#include "HeartbeatThread.hpp"
#include "iniparser/Section.hpp"

namespace masterserver
{

MasterServer::MasterServer()
    : serverconfig(config->getSection("server")), heartbeatThread(0)
{   
    sock = -1;
    try {
        // bind to socket
        sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
        if(sock < 0) {
            std::stringstream msg;
            msg << "Couldn't create socket: " << strerror(errno);
            throw std::runtime_error(msg.str());
        }

        // resolved bind/listen address
        struct hostent* hentry;
        hentry =
            gethostbyname(serverconfig.getValue("listen").c_str());
        if(!hentry) {
            std::stringstream msg;
            msg << "Couldn't resolve listen-address '" << 
                serverconfig.getValue("listen") << "'.";
            throw std::runtime_error(msg.str());
        }
        int bindaddress = ((struct in_addr*) hentry->h_addr)->s_addr;
        if(bindaddress == 0) {
            throw std::runtime_error("Must specify an explicit bind address");
        }
       
        memset(&serveraddr, 0, sizeof(struct sockaddr_in));
        serveraddr.sin_family = AF_INET;
        serveraddr.sin_addr.s_addr = bindaddress;
        serveraddr.sin_port = htons(serverconfig.getIntValue("port"));
        int res = bind(sock, (struct sockaddr*) &serveraddr,
                sizeof(struct sockaddr_in));
        if(res < 0) {
            std::stringstream msg;
            msg << "Couldn't bind socket to port " <<
                serverconfig.getIntValue("port") << " : " << strerror(errno);
            throw std::runtime_error(msg.str());
        }

        res = listen(sock, 20);
        if(res < 0) {
            std::stringstream msg;
            msg << "Couldn't listen on socket: " << strerror(errno);
            throw std::runtime_error(msg.str());
        }
        
        pthread_mutex_init(&serverlist_mutex, 0);

        // start heartbeatthread
        heartbeatThread = new HeartbeatThread(this);
    } catch(...) {
        if(sock >= 0)
            close(sock);
        throw;
    }
}

MasterServer::~MasterServer()
{
    writeNeighborCache();
    delete heartbeatThread;

    for(std::list<RequestThread*>::iterator i = threads.begin();
            i != threads.end(); ++i) {
        delete *i;
    }
    
    close(sock);
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
            if(currenttime - (*i)->getStartTime() >
                    serverconfig.getIntValue("client-request-timeout")) {
                delete *i;
                i = threads.erase(i);
            } else {
                ++i;
            }               
        }
        if(threads.size() > serverconfig.getIntValue("connection-limit")) {
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
            *log << "Heartbeat from " << inet_ntoa(addr->sin_addr) << std::endl;
            bool result = updateServerInfo(*i, tokenizer);
            // TODO do UDP check
            if(!result) {
                strstream << "\\error\\Couldn't update server\\final\\" <<
                    std::flush;
            } else {
                stream << "\\ok\\server updated\\final\\" << std::flush;
            }
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
    *log << "New server at " << inet_ntoa(addr->sin_addr) << std::endl;
    if(updateServerInfo(info, tokenizer)) {
        stream << "\\ok\\server added\\final\\" << std::flush;
        serverlist.push_back(info);
    } else {
        stream << "\\error\\Couldn't add server.\\final\\" << std::flush;
    }

    pthread_mutex_unlock(&serverlist_mutex);
}

bool MasterServer::updateServerInfo(ServerInfo& info, Tokenizer& tokenizer)
{    
    if(tokenizer.getNextToken() != "gamename") {
        *log << "Invalid heartbeat query." << std::endl;
        return false;
    }
    std::string gamename = tokenizer.getNextToken();
    // need passwort for masterserver
    if(gamename == "master") {
        if(tokenizer.getNextToken() != "passwort") {
            *log << "Missing passwort for masterserver heartbeat." << std::endl;
            return false;
        }
        if(tokenizer.getNextToken() !=
                serverconfig.getValue("masterserver-password")) {
            *log << "Wrong passwort in masterserver heartbeat." << std::endl;
            return false;
        }
    }
    
    info.gamename = tokenizer.getNextToken();
    info.lastheartbeat = time(0);
    return true;
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
        if(currenttime - i->lastheartbeat <=
                serverconfig.getIntValue("server-alive-timeout")) {
            if(i->gamename == gamename) {
                stream << "\\ip\\" << inet_ntoa(i->address.sin_addr)
                    << "\\port\\" << ntohs(i->address.sin_port);
            }
            ++i;
        } else {
            // remove server from list
            *log << "server timeout at " << inet_ntoa(addr->sin_addr)
                 << std::endl;
            i = serverlist.erase(i);
        }
    }
    pthread_mutex_unlock(&serverlist_mutex);

    // we're a masterserver ourself
    if(gamename == "master") {
        stream << "\\ip\\" << inet_ntoa(serveraddr.sin_addr) << "\\";
    }
    
    stream << "\\final\\";
}

void
MasterServer::parseQuit(std::iostream& stream, struct sockaddr_in* addr,
        Tokenizer& tokenizer)
{
    std::string token = tokenizer.getNextToken();
    std::stringstream strstream(token);
    
    int queryport;
    strstream >> queryport;

    pthread_mutex_lock(&serverlist_mutex);
    for(std::vector<ServerInfo>::iterator i = serverlist.begin();
            i != serverlist.end(); ++i) {
        if(i->address.sin_addr.s_addr == addr->sin_addr.s_addr
            && i->address.sin_port == htons(queryport)) {
            *log << "Server '" << i->gamename << "' at " <<
                inet_ntoa(i->address.sin_addr) << ":" << 
                ntohs(i->address.sin_port) << "\n";
            serverlist.erase(i);
            break;
        }
    }
    pthread_mutex_unlock(&serverlist_mutex);
}

void
MasterServer::writeNeighborCache()
{
    iniparser::Store neighborini;
    iniparser::Section& neighbors = neighborini.getSection("neighbors");

    pthread_mutex_lock(&serverlist_mutex);
    int idx = 0;
    for(std::vector<ServerInfo>::iterator i = serverlist.begin();
            i != serverlist.end(); ++i) {
        if(i->gamename == "master") {
            std::stringstream key;
            key << "ip" << idx++;
            neighbors.setValue(key.str(), inet_ntoa(i->address.sin_addr));
        }
    }
    pthread_mutex_unlock(&serverlist_mutex);

    const std::string neighborcachefile = 
        config->getSection("server").getValue("neighborcachefile");
    std::ofstream out(neighborcachefile.c_str());
    if(!out.good()) {
        *log << "Couldn't write neighborcachefile '" << neighborcachefile
            << "'.\n";
        return;
    }
    neighborini.save(out);
}

}
