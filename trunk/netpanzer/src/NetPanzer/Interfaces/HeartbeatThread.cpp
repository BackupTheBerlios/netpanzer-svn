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

#include "GameConfig.hpp"
#include "HeartbeatThread.hpp"
#include "NetworkGlobals.hpp"

#include <iostream>
#include <stdexcept>
#include <sstream>

#include <time.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <SDL.h>

#include "Network/Address.hpp"
#include "Network/TCPSocket.hpp"
#include "Network/SocketStream.hpp"
#include "Util/Log.hpp"
#include "Util/StringTokenizer.hpp"
#include "Util/StreamTokenizer.hpp"

// send a heartbeat packet every 5 minutes
static const int UPDATEINTERVAL = 5*60;
// after how many heartbeats query for updated masterserverlist
static const int MASTERQUERYCOUNT = 24;

HeartbeatThread::HeartbeatThread()
    : running(false), masterquery(0)
{
    static const int masterport = 28900;
    std::string masterhost = gameconfig->masterservers;
    
    // lookup server addresses
    StringTokenizer tokenizer(gameconfig->masterservers, ',');
    std::string host;
    while( (host = tokenizer.getNextToken()) != "") {
        try {
            network::Address addr = network::Address::resolve(host, masterport);
            serveraddrs.push_back(addr);
        } catch(std::exception& e) {
            LOGGER.warning("Bad Masterserver: %s\n", e.what());
        }
    }
    
    // send initial heartbeat
    if(!sendHeartbeat()) {
        throw std::runtime_error("Sending initial heartbeat failed.");
    }

    // start thread
    thread = SDL_CreateThread(threadMain, this);
}

HeartbeatThread::~HeartbeatThread()
{
    // signal thread to stop
    SDL_KillThread(thread);
    thread = 0;

    std::stringstream packet;
    packet << "\\quit\\" << gameconfig->serverport << "\\final\\";
    masterquery = 1; // do not query for masterserverlist now
    sendPacket(packet.str());

    // write back masterserverlist
    std::string gameservers;
    for(Addresses::iterator i = serveraddrs.begin();
            i != serveraddrs.end(); ++i) {
        if(i != serveraddrs.begin())
            gameservers += ",";
        gameservers += i->getIP();
    }
    gameconfig->masterservers = gameservers;
}

int
HeartbeatThread::threadMain(void* data)
{
    HeartbeatThread* _this = reinterpret_cast<HeartbeatThread*> (data);
    
    _this->running = true;
    while(_this->running) {
        // use nanosleep to have a thread cancelation point
        SDL_Delay(UPDATEINTERVAL * 1000);
        
        try {
            if(!_this->sendHeartbeat()) {
                LOGGER.warning("all hearbteats failed. retrying in %d seconds.",
                        UPDATEINTERVAL);
            }
        } catch(...) {
            LOGGER.warning("Unexpected exception while sending heartbeat.");
            _this->running = false;
            break;
        }
    }

    return 0;
}

bool
HeartbeatThread::sendHeartbeat()
{
    std::stringstream packet;
    packet << "\\heartbeat\\gamename\\netpanzer"
           << "\\port\\" << gameconfig->serverport                     
           << "\\protocol\\" << NETPANZER_PROTOCOL_VERSION
           << "\\final\\" << std::flush;
    return sendPacket(packet.str());
}

bool
HeartbeatThread::sendPacket(const std::string& str)
{
    int failures = 0;
    
    for(size_t i = 0; i < serveraddrs.size(); ++i) {
        try {
            const network::Address& address = serveraddrs[i];
        
            // we use blocking mode here and hope that no masterserver quietly
            // leaves our connection open without sending data
            network::TCPSocket socket(address);
            network::SocketStream stream(socket);
            
            bool doMasterQuery = false;
            if(masterquery <= 0) {
                doMasterQuery = true;
                stream << "\\list\\gamename\\master\\final";
                masterquery = MASTERQUERYCOUNT;
            }
            
            // send heartbeat packet
            stream << str << std::flush;

            parseResult(stream, doMasterQuery);
        } catch(std::exception& e) {
            LOGGER.warning("failed contacting masterserver: %s", e.what());
            failures++;
        }
    }

    if(failures == (int) serveraddrs.size())
        return false;
    
    masterquery--;
    return true;
}

void
HeartbeatThread::parseResult(std::iostream& stream, bool doMasterQuery)
{
    StreamTokenizer tokenizer(stream, '\\');

    // deal with response to master query
    if(doMasterQuery) {
        std::string token;
        std::string ip;
        bool firstip = true;
        while( (token = tokenizer.getNextToken()) != "") {
            if(token == "error") {
                std::stringstream msg;
                msg << "Masterserver sent back error: "
                    << tokenizer.getNextToken();
                throw std::runtime_error(msg.str());
            } else if(token == "ip") {
                ip = tokenizer.getNextToken();
            } else if(token == "port") {
                int port;
                std::stringstream portstr(tokenizer.getNextToken());
                portstr >> port;

                try {
                    network::Address addr = network::Address::resolve(ip, port);
                                    
                    if(firstip) {
                        serveraddrs.clear();
                        firstip = false;
                    }
                    serveraddrs.push_back(addr);
                } catch(std::exception& e) {
                    LOGGER.warning(
                            "Couldn't resolve additional masterserver: %s",
                            e.what());
                }
            } else if(token == "final") {
                break;
            } else {
                std::stringstream msg;
                msg << "Unknown token in masterserver response: " << token;
                throw std::runtime_error(msg.str());
            }
        }
    }

    // deal with response to heartbeat packet
    std::string token;
    while( (token = tokenizer.getNextToken()) != "") {
        if(token == "error") {
            std::stringstream msg;
            msg << "Masterserver sent back error: " << tokenizer.getNextToken();
            throw std::runtime_error(msg.str());
        } else if(token == "final") {
            break;
        } else {
            std::stringstream msg;
            msg << "Unknown token in masterserver response: " << token;
            throw std::runtime_error(msg.str());
        }
    }
}

