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

#include "InfoThread.hpp"

#include <sstream>
#include <stdexcept>

#include <string.h>

#include "Util/StringTokenizer.hpp"
#include "GameConfig.hpp"
#include "NetworkGlobals.hpp"
#include "PlayerInterface.hpp"
#include "Util/Log.hpp"

InfoThread::InfoThread(int port)
{
    // setup socket and bind it
    socket = 0; 
    try {
        socket = SDLNet_UDP_Open(port);
        if(socket == 0) {
            std::stringstream msg;
            msg << "Couldn't create socket: " << SDLNet_GetError();
            throw std::runtime_error(msg.str());
        }

        // start the thread
        thread = SDL_CreateThread(threadMain, this);
    } catch(...) {
        if(socket != 0)
            SDLNet_UDP_Close(socket);
        throw;
    }
}

InfoThread::~InfoThread()
{
    running = false;
    SDL_KillThread(thread);
    
    if(socket != 0)
        SDLNet_UDP_Close(socket);
}

int InfoThread::threadMain(void* data)
{
    InfoThread* _this = reinterpret_cast<InfoThread*> (data);

    _this->running = true;
    while(_this->running) {
        try {
            _this->handleStatusRequests();
        } catch(std::exception& e) {
            std::cerr << "Error while handling status request: " << e.what() <<
                "\n";
        } catch(...) {
            std::cerr << "Unexpected exception in InfoThread.\n";
        }
    }

    return 0;
}

void InfoThread::handleStatusRequests()
{
    UDPpacket* packet = SDLNet_AllocPacket(4096);
    int res = 0;
    while(res == 0) {
        res = SDLNet_UDP_Recv(socket, packet);
        // bleh... we need blocking calls...
        SDL_Delay(20);
    }
    if(res < 0) {
        SDLNet_FreePacket(packet);
        std::stringstream msg;
        msg << "Couldn't receive network data: " << SDLNet_GetError();
        throw std::runtime_error(msg.str());
    }

    std::string packetstr((char*) packet->data, packet->len);
    StringTokenizer tokenizer(packetstr, '\\');

    std::string query;
    do {
        query = tokenizer.getNextToken();

        LOGGER.debug("Received server status query: '%s'", query.c_str());
        if(query == "status") {
            std::stringstream response; 
            // you should make sure the response packet doesn't get > 1500bytes
            PlayerInterface::lock();
            sendInfo(response);
            sendRules(response);
            sendPlayers(response);
            PlayerInterface::unLock();
            response << "final\\";

            std::string responsestr = response.str();
            LOGGER.debug("Send back query response, size %d",
                    (int)responsestr.size());
            const void* data = responsestr.c_str();
            size_t datasize = responsestr.size();
            UDPpacket* sendpacket = SDLNet_AllocPacket(datasize);
            if(!sendpacket) {
                throw std::runtime_error("out of memory");
            }
            sendpacket->address = packet->address;
            sendpacket->len = datasize;
            memcpy(sendpacket->data, data, datasize);
            res = SDLNet_UDP_Send(socket, -1, sendpacket);
            SDLNet_FreePacket(sendpacket);
            if(res != 1) {
                SDLNet_FreePacket(packet);
                std::stringstream msg;
                msg << "Errro when sending back info: " << SDLNet_GetError();
                throw std::runtime_error(msg.str());
            }

            break; // enough, next client
        } else if(query == "echo") {
            std::string word = tokenizer.getNextToken();

            const void* data = word.c_str();
            size_t datasize = word.size();
            UDPpacket* sendpacket = SDLNet_AllocPacket(datasize);
            if(!sendpacket) {
                throw std::runtime_error("out of memory");
            }
            sendpacket->address = packet->address;
            sendpacket->len = datasize;
            memcpy(sendpacket->data, data, datasize);
            res = SDLNet_UDP_Send(socket, -1, sendpacket);
            SDLNet_FreePacket(sendpacket);
            if(res != 1) {
                SDLNet_FreePacket(packet);
                std::stringstream msg;
                msg << "Errro when sending back info: " << SDLNet_GetError();
                throw std::runtime_error(msg.str());
            }
        } else {
            // unknown query skip it
        }
    } while(query != "");
    SDLNet_FreePacket(packet);
}

void InfoThread::sendInfo(std::stringstream& out)
{
    // This should be some game-specific logic...
    out << "gamename\\netpanzer\\"
        << "protocol\\" << _NETPANZER_PROTOCOL_VERSION << "\\"
        << "hostname\\" << gameconfig->playername << "\\"
        << "mapname\\" << gameconfig->map << "\\"
        << "mapcycle\\" << gameconfig->mapcycle << "\\"
        << "numplayers\\" << PlayerInterface::countPlayers() << "\\"
        << "maxplayers\\" << gameconfig->maxplayers << "\\";
    if(PlayerInterface::countPlayers() == 0)
        out << "empty\\1\\";
    else if(PlayerInterface::countPlayers() >= gameconfig->maxplayers)
        out << "full\\1\\";
}

void InfoThread::sendRules(std::stringstream& out)
{
    out << "gamemode\\" << gameconfig->getGameTypeString() << "\\"
        << "units_per_player\\" << gameconfig->GetUnitsPerPlayer() << "\\"    
        << "timelimit\\" << gameconfig->timelimit << "\\"
        << "fraglimit\\" << gameconfig->fraglimit << "\\";
}

void InfoThread::sendPlayers(std::stringstream& out)
{
    for(int i = 0; i < PlayerInterface::countPlayers(); ++i) {
        PlayerState* playerState = PlayerInterface::getPlayerState(i);
        out << "player_" << i << "\\" << playerState->getName() << "\\"
            << "kills_" << i << "\\" << playerState->getKills() << "\\"
            << "losses_" << i << "\\" << playerState->getLosses() << "\\"
            << "objectives_" << i << "\\" 
                << playerState->getObjectivesHeld() << "\\";
    }
    // TODO add team/alliance info
}

