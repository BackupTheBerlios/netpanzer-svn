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

// send a heartbeat packet every 5 minutes
static const int UPDATEINTERVAL = 5*60;

HeartbeatThread::HeartbeatThread()
    : running(false)
{
    int masterport = 28900;
    std::string masterhost = gameconfig->masterservers;
    
    // lookup server addresses
    const char* str = gameconfig->masterservers.c_str();
    char c;
    size_t i = 0;
    std::string currentstr;
    while( (c = str[i++]) != 0) {
        if(c == ',') {
            IPaddress serveraddr;
            int res = SDLNet_ResolveHost(&serveraddr,
                    const_cast<char*> (masterhost.c_str()), masterport);
            if(res < 0) {
                std::cerr << "Couldn't resolve address of masterserver '"
                          << masterhost << "': " << SDLNet_GetError() << "\n";
            } else {
                serveraddrs.push_back(serveraddr);
            }
        } else if(isspace(c)) {
            // do nothing
        } else {
            currentstr += c;
        }
    }
    if(currentstr != "") {
        IPaddress serveraddr;
        int res = SDLNet_ResolveHost(&serveraddr,
                const_cast<char*> (masterhost.c_str()), masterport);            
        if(res < 0) {
            std::cerr << "Couldn't resolve address of masterserver '"
                      << masterhost << "': " << SDLNet_GetError() << "\n";
        } else {
            serveraddrs.push_back(serveraddr);
        }
    }

    // send initial heartbeat
    for(std::vector<IPaddress>::iterator i = serveraddrs.begin();
            i != serveraddrs.end(); ++i) {
        sendHeartbeat(*i);
    }

    // start thread
    thread = SDL_CreateThread(threadMain, this);
}

HeartbeatThread::~HeartbeatThread()
{
    // signal thread to stop
    SDL_KillThread(thread);
    thread = 0;
}

int HeartbeatThread::threadMain(void* data)
{
    HeartbeatThread* _this = reinterpret_cast<HeartbeatThread*> (data);
    
    _this->running = true;
    while(_this->running) {
        // use nanosleep to have a thread cancelation point
        SDL_Delay(UPDATEINTERVAL * 1000);
        
        try {
            for(std::vector<IPaddress>::iterator i = _this->serveraddrs.begin();
                    i != _this->serveraddrs.end(); ++i) {
                _this->sendHeartbeat(*i);
            }
        } catch(std::exception& e) {
            std::cerr << "Couldn't send heartbeat packet: " << e.what() << "\n"
                << "retrying in " << UPDATEINTERVAL << "seconds." << std::endl;
        } catch(...) {
            std::cerr << "Unexpected exception while sending heartbeat.\n";
        }
    }

    return 0;
}

void HeartbeatThread::sendHeartbeat(IPaddress addr)
{
    TCPsocket sock = 0;
    try {
        sock = SDLNet_TCP_Open(&addr);
        if(sock == 0) {
            std::stringstream msg;
            msg << "Couldn't create socket: " << SDLNet_GetError();
            throw std::runtime_error(msg.str());
        }
        
        // send heartbeat packet
        std::stringstream packet;
        packet << "\\heartbeat\\" << "\\gamename\\netpanzer"
               << "\\port\\" << gameconfig->serverport
               << "\\protocol\\" << _NETPANZER_PROTOCOL_VERSION
               << "\\final\\" << std::flush;
        const void* data = packet.str().c_str();
        size_t datasize = packet.str().size();
        int res = SDLNet_TCP_Send(sock, const_cast<void*>(data), (int) datasize);
        if(res != (int) datasize) {
            std::cerr << 
                "Couldn't send heartbeat packet: " << SDLNet_GetError();
        }

        SDLNet_TCP_Close(sock);
    } catch(...) {
        if(sock != 0)
            SDLNet_TCP_Close(sock);
        throw;
    }
}

