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
#include "DedicatedGameManager.hpp"

#include <stdio.h>
#include <fcntl.h>
#include <string>

#include "ChatInterface.hpp"
#include "ConsoleInterface.hpp"
#include "GameConfig.hpp"
#include "GameControlRulesDaemon.hpp"
#include "GameManager.hpp"
#include "MapInterface.hpp"
#include "MapsManager.hpp"
#include "NetworkState.hpp"
#include "ObjectiveInterface.hpp"
#include "Particle2D.hpp"
#include "ParticleInterface.hpp"
#include "Physics.hpp"
#include "Log.hpp"
#include "Server.hpp"
#include "IRCLobbyView.hpp"

//-----------------------------------------------------------------
void DedicatedGameManager::initializeGameConfig(const std::string& configfile)
{
    if(configfile == "")
        gameconfig = new GameConfig("/config/netpanzer-dedicated.xml");
    else
        gameconfig = new GameConfig(configfile.c_str());
}
//-----------------------------------------------------------------
void DedicatedGameManager::initializeInputDevices()
{
    ConsoleInterface::setStdoutPipe(true);
}
//-----------------------------------------------------------------
void DedicatedGameManager::inputLoop()
{
    // XXX we need new code here (someone wanna write a readline version of this
    // stuff?
#ifdef WIN32
    if( kbhit() )
    {
        int key;
        key = getch();
        if ( key == 0 ) {
            key = getch();
        } else {
            switch( key ) {
            case 27 : {
                    GameManager::exitNetPanzer();
                }
                break;

            case 'Q' :
            case 'q' : {
                    GameManager::exitNetPanzer();
                }
                break;

            case 'M' :
            case 'm' : {
                    GameControlRulesDaemon::forceMapCycle();
                }
                break;

            case 'I' :
            case 'i' : {
                    ConsoleInterface::postMessage( "Map: %s", gameconfig->getGameMapName() );
                    ConsoleInterface::postMessage( "Players: %d/%d", PlayerInterface::getActivePlayerCount(),
                                                   gameconfig->maxplayers );

                    ConsoleInterface::postMessage( "Units: %d/%d", UnitInterface::getTotalUnitCount(),
                                                   gameconfig->maxunits);
                }
                break;

            case '1' : {
                    ChatInterface::setMessageScopeServer();
                    ChatInterface::sendCurrentMessage( "Server will restart in 5 minutes" );
                }
                break;

            case '2' : {
                    ChatInterface::setMessageScopeServer();
                    ChatInterface::sendCurrentMessage( "Server is restarting" );
                }
                break;

            case '3' : {
                    ChatInterface::setMessageScopeServer();
                    ChatInterface::sendCurrentMessage( "Server is rotating map" );
                }
                break;

            } // ** switch
        }
    }
#endif
}

//-----------------------------------------------------------------
bool DedicatedGameManager::launchNetPanzerGame()
{
    ConsoleInterface::postMessage( "netPanzer Dedicated Server");

    LOGGER.info("Server Settings:"); 
    LOGGER.info("Map: %s", gameconfig->map.c_str());
    LOGGER.info("MaxPlayers: %d", (int) gameconfig->maxplayers);
    LOGGER.info("MaxUnits: %d", (int) gameconfig->maxunits);
    LOGGER.info("Gametype: %s", gameconfig->getGameTypeString());
    LOGGER.info("ObjectivePercentage: %d",
            (int) gameconfig->objectiveoccupationpercentage);
    LOGGER.info("TimeLimit: %d", (int) gameconfig->timelimit);   
    LOGGER.info("FragLimit: %d", (int) gameconfig->fraglimit);
    LOGGER.info("RespawnType: %s", gameconfig->getRespawnTypeString());
    LOGGER.info("MapCycling: %s", gameconfig->mapcycling ? "yes" : "no");
    LOGGER.info("Powerups: %s", gameconfig->powerups ? "yes" : "no");
    LOGGER.info("AllowAllies: %s", gameconfig->allowallies ? "yes" : "no");
    LOGGER.info("CloudCoverage: %d (WindSpeed %d)", 
            (int) gameconfig->cloudcoverage, (int) gameconfig->windspeed);

    const char* mapname = gameconfig->map.c_str();
    MapsManager::setCycleStartMap(mapname);
    GameManager::dedicatedLoadGameMap(mapname);

    GameManager::reinitializeGameLogic();

    SERVER->openSession();
    SERVER->hostSession();

    GameControlRulesDaemon::setStateServerInProgress();
    NetworkState::setNetworkStatus( _network_state_server );

    GameManager::setNetPanzerGameOptions();

    gameconfig->hostorjoin=_game_session_host;

    Particle2D::setCreateParticles(false);
    if(!lobby_view)
        lobby_view = new IRCLobbyView;
    lobby_view->startIRC();

    ConsoleInterface::postMessage( "Game Launched, Server Ready...");

    GameManager::startGameTimer();
    return true;
}
