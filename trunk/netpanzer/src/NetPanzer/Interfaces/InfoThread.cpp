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
#include "Network/Address.hpp"

InfoThread::InfoThread(int port)
    : socket(0)
{
    socket = new network::UDPSocket(port, false);

    // start the thread
    thread = SDL_CreateThread(threadMain, this);
}

InfoThread::~InfoThread()
{
    running = false;
    SDL_WaitThread(thread, 0);

    delete socket;
}

int InfoThread::threadMain(void* data)
{
    InfoThread* _this = reinterpret_cast<InfoThread*> (data);

    _this->running = true;
    while(_this->running) {
        try {
            _this->handleStatusRequests();
        } catch(std::exception& e) {
            LOGGER.warning("Error while handling status request: %s",
                    e.what());
        } catch(...) {
            LOGGER.warning("Unexpected exception in InfoThread.\n");
        }
    }

    return 0;
}

void InfoThread::handleStatusRequests()
{
    network::Address addr;
    char buffer[4096];
   
    size_t size = 0;
    while(size == 0) {
        if(!running)
            return;         
        size = socket->recv(addr, buffer, sizeof(buffer));
        SDL_Delay(20);
    }

    std::string packetstr(buffer, size);
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

            socket->send(addr, responsestr.c_str(), responsestr.size());
            break; // enough, next client
        } else if(query == "echo") {
            std::string word = tokenizer.getNextToken();

            socket->send(addr, word.c_str(), word.size());
        } else {
            // unknown query skip it
        }
    } while(query != "");
}

void InfoThread::sendInfo(std::stringstream& out)
{
    // This should be some game-specific logic...
    out << "gamename\\netpanzer\\"
        << "protocol\\" << NETPANZER_PROTOCOL_VERSION << "\\"
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
    out << "gamestyle\\" << gameconfig->getGameTypeString() << "\\"
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
            << "deaths_" << i << "\\" << playerState->getLosses() << "\\"
            << "score_" << i << "\\" 
                << playerState->getObjectivesHeld() << "\\";
    }
    // TODO add team/alliance info
}

