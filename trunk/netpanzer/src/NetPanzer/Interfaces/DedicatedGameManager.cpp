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
#include <SDL.h>

#include "ChatInterface.hpp"
#include "ConsoleInterface.hpp"
#include "Console.hpp"
#include "GameConfig.hpp"
#include "GameControlRulesDaemon.hpp"
#include "GameManager.hpp"
#include "MapInterface.hpp"
#include "MapsManager.hpp"
#include "NetworkState.hpp"
#include "ObjectiveInterface.hpp"
#include "Particle2D.hpp"
#include "ParticleInterface.hpp"
#include "PlayerInterface.hpp"
#include "Physics.hpp"
#include "Util/Log.hpp"
#include "Server.hpp"
#include "LobbyView.hpp"
#include "ProgressView.hpp"
#include "ConsoleLoadingView.hpp"
#include "Console.hpp"
#include "HeartbeatThread.hpp"
#include "InfoThread.hpp"
#include "Util/Log.hpp"

DedicatedGameManager::DedicatedGameManager()
    : commandqueue_mutex(0), console(0), heartbeatthread(0), infothread(0)
{
    Console::initialize();
}

DedicatedGameManager::~DedicatedGameManager()
{
    delete console;
    delete heartbeatthread;
    delete infothread;
}

void DedicatedGameManager::initializeVideoSubSystem()
{
    lobbyView = new ConsoleLoadingView();
    progressView = new ConsoleLoadingView();
    commandqueue_mutex = SDL_CreateMutex();
}

void DedicatedGameManager::shutdownVideoSubSystem()
{
    delete lobbyView;
    lobbyView = 0;
    delete progressView;
    progressView = 0;
    delete console;
    console = 0;
    SDL_DestroyMutex(commandqueue_mutex);
    commandqueue_mutex = 0;
}

//-----------------------------------------------------------------
void DedicatedGameManager::initializeGameConfig(const std::string& configfile)
{
    if(configfile == "")
        gameconfig = new GameConfig("/config/netpanzer-dedicated.ini");
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
    // handle server commands
    SDL_mutexP(commandqueue_mutex);
    while(!commandqueue.empty()) {
        const ServerCommand& command = commandqueue.front();

        switch(command.type) {
            case ServerCommand::QUIT:
            {
                GameManager::exitNetPanzer();
                break;
            }
            case ServerCommand::CHAT:
            {
                ChatInterface::sendCurrentMessage(command.argument.c_str());
                break;
            }
            case ServerCommand::STATUS:
            {
                *Console::server << "Server " << gameconfig->playername
                    << " version " << PACKAGE_VERSION << " port "
                    << gameconfig->serverport << "\n"
                    << "Map: " << gameconfig-> map << "\n"
                    << "ID Player    Ki  Lo  P\n";
                size_t playercount = PlayerInterface::countPlayers();
                for(size_t i = 0; i<playercount; ++i) {
                    PlayerState* playerstate =
                        PlayerInterface::getPlayerState(i);
                    *Console::server
                        << i << "  " << playerstate->getName() << "    "
                        << playerstate->getKills() << " - " 
                        << playerstate->getLosses() << " - "
                        << playerstate->getTotal() << " - "
                        << "xxx.xxx.xxx.xxx" << "\n";
                }
                *Console::server << std::flush;
                break;
            }
            case ServerCommand::MAPCHANGE:
                // TODO
                break;
            case ServerCommand::KICK:
                // TODO
                break;
        }
        commandqueue.pop();
    }
    SDL_mutexV(commandqueue_mutex);
}

//-----------------------------------------------------------------
void DedicatedGameManager::pushCommand(const ServerCommand& command)
{
    SDL_mutexP(commandqueue_mutex);
    commandqueue.push(command);
    SDL_mutexV(commandqueue_mutex);
}

//-----------------------------------------------------------------
bool DedicatedGameManager::launchNetPanzerGame()
{
    *Console::server << "starting dedicated netPanzer server";

    gameconfig->map = MapsManager::getNextMap("");
    
    *Console::server << "Server Settings:\n"
        << "Map: " << gameconfig->map << "\n"
        << "MaxPlayers: " << gameconfig->maxplayers << "\n"
        << "MaxUnits: " << gameconfig->maxunits << "\n"
        << "Gametype: " << gameconfig->getGameTypeString() << "\n"
        << "ObjectivePercentage: " <<
        gameconfig->objectiveoccupationpercentage << "\n"
        << "TimeLimit: " << gameconfig->timelimit << "\n"
        << "FragLimit: " << gameconfig->fraglimit << "\n"
        << "RespawnType: " << gameconfig->getRespawnTypeString() << "\n"
        << "Mapcycle: " << gameconfig->mapcycle << "\n"
        << "Powerups: " << (gameconfig->powerups ? "yes" : "no") << "\n"
        << "AllowAllies: " << (gameconfig->allowallies ? "yes" : "no") << "\n"
        << "CloudCoverage: " << gameconfig->cloudcoverage << " (Windspeed "
           << gameconfig->windspeed << ")" << std::endl;
        
    GameManager::dedicatedLoadGameMap(gameconfig->map.c_str());

    GameManager::reinitializeGameLogic();

    SERVER->openSession();
    SERVER->hostSession();

    GameControlRulesDaemon::setStateServerInProgress();
    GameControlRulesDaemon::setDedicatedServer();
    NetworkState::setNetworkStatus( _network_state_server );

    GameManager::setNetPanzerGameOptions();

    gameconfig->hostorjoin=_game_session_host;

    Particle2D::setCreateParticles(false);

    if((const std::string&) gameconfig->masterservers != "") {
        try {
            infothread = new InfoThread(gameconfig->serverport);
            heartbeatthread = new HeartbeatThread();
        } catch(std::exception& e) {
            LOGGER.warning("failed contacting masterserver: %s", e.what());
            delete infothread; infothread = 0;
            delete heartbeatthread; heartbeatthread = 0;
        }
    }

    *Console::server << "game started." << std::endl;

    console = new ServerConsole(this);
    console->startThread();

    GameManager::startGameTimer();
    return true;
}

//---------------------------------------------------------------------------
void
DedicatedGameManager::initializeNetworkSubSystem()
{
    BaseGameManager::initializeNetworkSubSystem();
}

void
DedicatedGameManager::shutdownNetworkSubSystem()
{
    delete infothread;
    infothread = 0;
    delete heartbeatthread;
    heartbeatthread = 0;
    BaseGameManager::shutdownNetworkSubSystem();
}
