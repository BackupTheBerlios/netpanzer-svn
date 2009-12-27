/*
Copyright (C) 2008 by Aaron Perez <aaronps@gmail.com>

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
 *
 * Created on December 14, 2008, 3:42 PM
 */

#include <list>
#include "Core/GlobalGameState.hpp"

#include "BotManager.hpp"
#include "Bot.hpp"
#include "BotPlayer.hpp"
#include "Interfaces/PlayerInterface.hpp"
#include "Classes/Network/NetworkServer.hpp"
#include "Classes/Network/PlayerNetMessage.hpp"
#include "Classes/Network/SystemNetMessage.hpp"
#include "Objectives/ObjectiveInterface.hpp"
#include "Units/UnitInterface.hpp"
#include "Resources/ResourceManager.hpp"
#include "Resources/ResourceManagerMessages.hpp"

using namespace std;

typedef list<Bot *> BotList;
static BotList bot_list;

Uint16
BotManager::addBot()
{
    PlayerState *p = PlayerInterface::allocateNewPlayer();
    if ( p )
    {
        Bot * b = new BotPlayer(p->getID());
        bot_list.push_back(b);

        ResourceManagerSyncFlagMessage flagmsg;
        ResourceManager::getFlagData("bot.bmp",(Uint8*)&flagmsg.flagdata);
        flagmsg.setFlagID(ResourceManager::registerFlagFromData((Uint8*)&flagmsg.flagdata));

        p->setName("Bot");
        p->setFlag(flagmsg.getFlagID());
        p->setStatus(_player_state_active);

        NetworkServer::broadcastMessage(&flagmsg, sizeof(flagmsg));

        PlayerStateSync updatemsg(p->getNetworkPlayerState());
        NetworkServer::broadcastMessage( &updatemsg, sizeof(updatemsg));

        SystemConnectAlert connect_alert;
        connect_alert.set( p->getID(), _connect_alert_mesg_connect );
        NetworkServer::broadcastMessage( &connect_alert, sizeof(SystemConnectAlert));
        
        return p->getID();
    }
    return 0xffff;
}

void
BotManager::removeBot(Uint16 playerid)
{
    for (BotList::iterator i = bot_list.begin(); i != bot_list.end(); ++i)
    {
        if ( (*i)->botPlayerId == playerid )
        {
            delete (*i);
            bot_list.erase(i);
            break;
        }
    }
}

bool
BotManager::isBot(Uint16 playerid)
{
    for (BotList::iterator i = bot_list.begin(); i != bot_list.end(); i++)
    {
        if ( (*i)->botPlayerId == playerid )
        {
            return true;
        }
    }
    return false;
}

void
BotManager::simBots()
{
    for (BotList::iterator i = bot_list.begin(); i != bot_list.end(); i++)
    {
        (*i)->processEvents();
    }
}
