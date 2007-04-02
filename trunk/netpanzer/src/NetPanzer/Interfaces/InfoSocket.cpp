/*
Copyright (C) 2007 by Aaron Perez <aaronps@gmail.com>

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

#include "InfoSocket.hpp"
#include "Util/StringTokenizer.hpp"

#include "NetworkGlobals.hpp"
#include "GameConfig.hpp"
#include "PlayerInterface.hpp"
#include "ObjectiveInterface.hpp"
#include "GameManager.hpp"

#include "Util/Log.hpp"

#include <string>
#include <sstream>

using namespace std;

InfoSocket::InfoSocket(int p) : socket(0)
{
    Address addr = Address::resolve( gameconfig->bindaddress, p);
    socket = new network::UDPSocket(addr,this);
    
    // This parameters are fixed always
    // others I plan to be modificable while game is running.
    stringstream s;
    s << "gamename\\netpanzer\\protocol\\" << NETPANZER_PROTOCOL_VERSION
      << "\\hostname\\" << gameconfig->playername;
    statusHead = s.str();
}

InfoSocket::~InfoSocket()
{
    if (socket)
        socket->destroy();
    socket=0;
}

void
InfoSocket::onDataReceived(UDPSocket *s, const Address &from, const char *data, const int len)
{
    string querypacket(data,len);
    StringTokenizer qtokenizer(querypacket, '\\');
    
    string query;
    while ( !(query = qtokenizer.getNextToken()).empty()) {
        LOGGER.debug("InfoSocket:: Received query '%s'", query.c_str());
        if ( query == "status" ) {
            string answer = prepareStatusPacket();

            LOGGER.debug("InfoSocket:: sending answer [%s][%d]", answer.c_str(), (int)answer.size());
            socket->send(from, answer.c_str(), answer.size());

            break;
        } else if(query == "echo") {
            string echotoken = qtokenizer.getNextToken();
            socket->send(from, echotoken.c_str(), echotoken.size());
        }
    }
}

string
InfoSocket::prepareStatusPacket()
{
    stringstream s;
    int playingPlayers = PlayerInterface::countPlayers();
    int maxPlayers = PlayerInterface::getMaxPlayers();
    
    s << statusHead
      << "\\mapname\\"    << gameconfig->map
      << "\\mapcycle\\"   << gameconfig->mapcycle
      << "\\numplayers\\" << playingPlayers
      << "\\maxplayers\\" << maxPlayers;
    
    if ( !playingPlayers )
        s << "\\empty\\1";
    else if ( playingPlayers==maxPlayers )
        s << "\\full\\1";

    s << "\\gamestyle\\" << gameconfig->getGameTypeString()
      << "\\units_per_player\\" << gameconfig->GetUnitsPerPlayer()
      << "\\time\\" << GameManager::getGameTime()/60
      << "\\timelimit\\" << gameconfig->timelimit
      << "\\fraglimit\\" << gameconfig->fraglimit
      << "\\objectivelimit\\" << ObjectiveInterface::getObjectiveLimit();

    ObjectiveInterface::updatePlayerObjectiveCounts();
    int n = 0;
    for(int i = 0; i < maxPlayers; ++i) {
        PlayerState* playerState = PlayerInterface::getPlayerState(i);
        if(playerState->getStatus() != _player_state_active)
            continue;
        s << "\\player_" << n << "\\" << playerState->getName()
          << "\\kills_"  << n << "\\" << playerState->getKills()
          << "\\deaths_" << n << "\\" << playerState->getLosses()
          << "\\score_"  << n << "\\" << playerState->getObjectivesHeld()
          << "\\flag_"   << n << "\\" << (int) playerState->getFlag();
        n++;
    }
    
    s << "\\final\\";

    return s.str();
}
