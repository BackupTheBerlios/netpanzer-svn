/*
Copyright (C) 2003 Ivo Danihelka <ivo@danihelka.net>
 
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
#include "BotGameManager.hpp"

#include "Bot.hpp"
#include "BotPlayer.hpp"

#include "GameManager.hpp"
#include "GameConfig.hpp"
#include "NetworkState.hpp"
#include "Client.hpp"
#include "ClientConnectDaemon.hpp"
#include "Exception.hpp"
#include "XmlParser.hpp"

//-----------------------------------------------------------------
BotGameManager::BotGameManager(const std::string &serverHost)
    : m_serverHost(serverHost)
{
    /* empty */
}
//-----------------------------------------------------------------
/**
 * Bot uses name = playername + "-" + rand() % 1000
 */
void BotGameManager::initializeGameConfig()
{
    gameconfig = new GameConfig("config/netpanzer-bot.xml");
    std::string::size_type pos =
        ((std::string)gameconfig->playername).rfind("-");
    std::string botname(gameconfig->playername, 0, pos);
    gameconfig->playername = botname + "-"
        + XmlParser::toString(rand() % 1000);
}
//-----------------------------------------------------------------
void BotGameManager::initializeInputDevices()
{
    Bot::initialize(new BotPlayer());
}
//-----------------------------------------------------------------
void BotGameManager::shutdownInputDevices()
{
    Bot::shutdown();
}
//-----------------------------------------------------------------
void BotGameManager::inputLoop()
{
    Bot::bot()->processEvents();
}

//-----------------------------------------------------------------
bool BotGameManager::launchNetPanzerGame()
{
    GameManager::setNetPanzerGameOptions();
    NetworkState::setNetworkStatus( _network_state_client );

    if (!CLIENT->joinSession(m_serverHost.c_str())) {
        return false;
    }

    ClientConnectDaemon::startConnectionProcess();
    return true;
}


