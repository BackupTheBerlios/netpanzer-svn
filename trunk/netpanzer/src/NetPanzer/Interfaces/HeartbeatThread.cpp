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
#include <stdexcept>
#include <sstream>

#include <time.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

// send a heartbeat packet every 5 minutes
static const int UPDATEINTERVAL = 5*60;

HeartbeatThread::HeartbeatThread(const std::string& masteraddress,
        const std::string& newgamename, int gamestatusport)
    : running(false), gamename(newgamename), gameport(gamestatusport)
{
    int masterport = 28900;
    std::string masterhost;

    std::string::size_type colon = masteraddress.find(':', 0);
    if(colon == std::string::npos) {
        masterhost = masteraddress;
    } else {
        masterhost = masteraddress.substr(0, colon);
        colon++;
        masterport = atoi(masteraddress.c_str()+colon);
    }
    
    // lookup server address
    int res = SDLNet_ResolveHost(&serveraddr,
            const_cast<char*> (masterhost.c_str()), masterport);
    if(res < 0) {
        std::stringstream msg;
        msg << "Couldn't resolve address of masterserver '"
            << masteraddress << "': " << SDLNet_GetError();
        throw std::runtime_error(msg.str());
    }

    // send initial heartbeat
    sendHeartbeat();

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
            _this->sendHeartbeat();
        } catch(std::exception& e) {
            std::cerr << "Couldn't send heartbeat packet: " << e.what() << "\n"
                << "retrying in " << UPDATEINTERVAL << "seconds." << std::endl;
        } catch(...) {
            std::cerr << "Unexpected exception while sending heartbeat.\n";
        }
    }

    return 0;
}

void HeartbeatThread::sendHeartbeat()
{
    TCPsocket sock = 0;
    try {
        sock = SDLNet_TCP_Open(&serveraddr);
        if(sock == 0) {
            std::stringstream msg;
            msg << "Couldn't create socket: " << SDLNet_GetError();
            throw std::runtime_error(msg.str());
        }
        
        // send heartbeat packet
        std::stringstream packet;
        packet << "\\heartbeat\\" << gameport
               << "\\gamename\\" << gamename << "\\final\\";
        const void* data = packet.str().c_str();
        size_t datasize = packet.str().size();
        int res = SDLNet_TCP_Send(sock, const_cast<void*>(data), (int) datasize);
        if(res != (int) datasize) {
            std::stringstream msg;
            msg << "Couldn't send heartbeat packet: " << SDLNet_GetError();
            throw std::runtime_error(msg.str());
        }

        SDLNet_TCP_Close(sock);
    } catch(...) {
        if(sock != 0)
            SDLNet_TCP_Close(sock);
        throw;
    }
}

