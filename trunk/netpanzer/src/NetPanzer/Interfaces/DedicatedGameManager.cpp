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
#include "Server.hpp"

//-----------------------------------------------------------------
void DedicatedGameManager::initializeGameConfig()
{
    gameconfig = new GameConfig("config/netpanzer-dedicated.cfg");
}
//-----------------------------------------------------------------
void DedicatedGameManager::initializeInputDevices()
{
    ConsoleInterface::setStdoutPipe(true);
}
//-----------------------------------------------------------------
void DedicatedGameManager::inputLoop()
{
    //TODO: slow down for happy CPU

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
void DedicatedGameManager::dedicatedLoadGameMap(const char *map_name)
{
    std::string map_path("maps/");
    map_path.append(map_name);

    MapInterface::startMapLoad(map_path.c_str(), false, 0);

    map_path.append(".opt");
    ObjectiveInterface::loadObjectiveList(map_path.c_str());

    ParticleInterface::initParticleSystems();
    Particle2D::setCreateParticles(false);

    ParticleInterface::addCloudParticle(gameconfig->cloudcoverage);
    Physics::wind.setVelocity(gameconfig->windspeed, 107);
}
//-----------------------------------------------------------------
// custom version of readString that doesn't return the trailing \n
static inline void readString(char* buffer, size_t buffersize, FILE* file)
{
    fgets(buffer, buffersize, file);
    buffer[strlen(buffer)-1] = '\0';
}
//-----------------------------------------------------------------
bool DedicatedGameManager::launchNetPanzerGame()
{
    ConsoleInterface::postMessage( "netPanzer Dedicated Server");

    char input_str[256];

    MapsManager::getCurrentMap( input_str );
    gameconfig->map = input_str;

    const char* mapname = ((const std::string&)(gameconfig->map)).c_str();
    printf( "Map Name <%s> : ", mapname);
    fflush(stdout);
    readString(input_str, 256, stdin);
    if ( strlen(input_str) > 0 ) {
        gameconfig->map = input_str;
    }

    printf( "Players <%d> : ", (int) gameconfig->maxplayers );
    fflush(stdout);
    readString(input_str, 256, stdin);
    if ( strlen(input_str) > 0 ) {
        int players;
        sscanf( input_str, "%d", &players );
        gameconfig->maxplayers = players;
    }

    printf( "Units <%d> : ", (int) gameconfig->maxunits );
    fflush(stdout);
    readString(input_str, 256, stdin);
    if ( strlen(input_str) > 0 ) {
        int units;
        sscanf( input_str, "%d", &units );
        gameconfig->maxunits = units;
    }

    int game_type = 1;

    do {
        printf( "Game Type\n" );
        printf( "(1) Objective \n");
        printf( "(2) Frag Limit \n" );
        printf( "(3) Time Limit \n" );
        printf( "Choose <1>: " );
        fflush(stdout);
        readString(input_str, 256, stdin);
        sscanf( input_str, "%d", &game_type );
    } while( (game_type < 1) || (game_type > 3) );

    switch( game_type ) {
    case 1 : {
            gameconfig->gametype = _gametype_objective;
            printf( "Outpost Occupation <%d %%> : ",
                    (int) gameconfig->objectiveoccupationpercentage );
            fflush(stdout);
            readString(input_str, 256, stdin);
            if ( strlen(input_str) > 0 ) {
                int percent;
                sscanf( input_str, "%d", &percent );
                gameconfig->objectiveoccupationpercentage = percent;
            }
        }
        break;

    case 2 : {
            gameconfig->gametype = _gametype_fraglimit;
            printf( "Frag Limit <%d> frags : ", (int) gameconfig->fraglimit );
            fflush(stdout);
            readString(input_str, 256, stdin);
            if ( strlen(input_str) > 0 ) {
                int frags;
                sscanf( input_str, "%d", &frags);
                gameconfig->fraglimit = frags;
            }
        }
        break;

    case 3 : {
            gameconfig->gametype = _gametype_timelimit;
            printf( "Time Limit <%d> minutes: ", (int) gameconfig->timelimit );
            fflush(stdout);
            readString(input_str, 256, stdin);
            if ( strlen(input_str) > 0 ) {
                int time_limit;
                sscanf( input_str, "%d", &time_limit );
                gameconfig->timelimit = time_limit;
            }
        }
        break;

    } // ** switch

    printf( "PowerUps <NO> (Y/N) : " );
    fflush(stdout);
    readString(input_str, 256, stdin);
    if ( strcasecmp( "y", input_str ) == 0 ) {
        gameconfig->powerups = true;
    } else {
        gameconfig->powerups = false;
    }

    printf( "Server Name <Dedicated Server> :" );
    fflush(stdout);
    readString(input_str, 256, stdin);
    if ( strlen(input_str) > 0 ) {
        gameconfig->playername = input_str;
    } else {
        gameconfig->playername = "Dedicated Server";
    }

    mapname = ((const std::string&)(gameconfig->map)).c_str();
    MapsManager::setCycleStartMap(mapname);
    dedicatedLoadGameMap(mapname);

    GameManager::reinitializeGameLogic();

    SERVER->openSession();
    SERVER->hostSession();

    GameControlRulesDaemon::setStateServerInProgress();
    NetworkState::setNetworkStatus( _network_state_server );

    GameManager::setNetPanzerGameOptions();

    Particle2D::setCreateParticles(false);

    ConsoleInterface::postMessage( "Game Launched, Server Ready...");

    GameManager::startGameTimer();
    return true;
}


