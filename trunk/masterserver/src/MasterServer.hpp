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
#ifndef __MASTERSERVER_HPP__
#define __MASTERSERVER_HPP__

#include <vector>
#include <list>
#include <map>
#include <string>
#include <iostream>
#include <arpa/inet.h>
#include "ServerInfo.hpp"
#include "Tokenizer.hpp"
#include "RequestThread.hpp"
#include "iniparser/Section.hpp"

namespace masterserver
{

class HeartbeatThread;

class MasterServer
{
public:
    MasterServer();
    ~MasterServer();

    void run();

    void addServer(const std::string& gamename, struct sockaddr_in address);
    
private:
    friend class RequestThread;
    
    /** this function is threadsafe */
    void parseHeartbeat(std::iostream& stream,
            struct sockaddr_in* addr, Tokenizer& tokenizer);
    /** this function is threadsafe */
    void parseMasterHeartbeat(std::iostream& stream,
            struct sockaddr_in* addr, Tokenizer& tokenizer);
    /** this function is threadsafe */
    void parseListMasters(std::iostream& stream,
            struct sockaddr_in* addr, Tokenizer& tokenizer);
    /** this function is threadsafe */
    void parseList(std::iostream& stream,
            struct sockaddr_in* addr, Tokenizer& tokenizer);
    /** this function is threadsafe */
    void parseQuit(std::iostream& stream,
            struct sockaddr_in* addr, Tokenizer& tokenizer);

    void parseKeyValues(std::map<std::string, std::string>& keyvalues,
            std::iostream& stream, Tokenizer& tokenizer);

    /** this function is threadsafe */
    void writeNeighborCache();
    
    int sock;
    struct sockaddr_in serveraddr;
    pthread_mutex_t serverlist_mutex;
    std::vector<ServerInfo> serverlist;
    std::list<RequestThread*> threads;

    iniparser::Section& serverconfig;

    HeartbeatThread* heartbeatThread;
};

}

#endif

