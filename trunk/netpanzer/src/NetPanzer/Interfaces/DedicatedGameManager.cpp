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

#include "Bot/BotManager.hpp"
#include "DedicatedGameManager.hpp"

#include <stdio.h>
#include <fcntl.h>
#include <string>
#include <iomanip>
#include "SDL.h"

#include "Interfaces/ChatInterface.hpp"
#include "Interfaces/ConsoleInterface.hpp"
#include "Interfaces/Console.hpp"
#include "Interfaces/GameConfig.hpp"
#include "Interfaces/GameControlRulesDaemon.hpp"
#include "Interfaces/GameManager.hpp"
#include "Interfaces/MapInterface.hpp"
#include "Interfaces/MapsManager.hpp"
#include "Interfaces/Heartbeat.hpp"
#include "Interfaces/InfoSocket.hpp"
#include "Objectives/ObjectiveInterface.hpp"
#include "Interfaces/PlayerInterface.hpp"

#include "Classes/Network/NetworkState.hpp"
#include "Classes/Network/NetworkServer.hpp"

#include "Particles/Particle2D.hpp"
#include "Particles/ParticleInterface.hpp"
#include "Particles/Physics.hpp"

#include "Util/Log.hpp"
#include "Network/MessageRouter.hpp"

DedicatedGameManager::DedicatedGameManager()
    : commandqueue_mutex(0), console(0), heartbeat(0), infosocket(0)
{
    commandqueue_mutex = SDL_CreateMutex();
    Console::initialize();
}

DedicatedGameManager::~DedicatedGameManager()
{
    delete console;
    if ( heartbeat )
        delete heartbeat;
    if ( infosocket )
        delete infosocket;
    Console::shutdown();

    SDL_DestroyMutex(commandqueue_mutex);
    commandqueue_mutex = 0;                  
}

void DedicatedGameManager::initializeVideoSubSystem()
{
    //LoadingView::setLoadingView(new ConsoleLoadingView());
}

void DedicatedGameManager::shutdownVideoSubSystem()
{
    //LoadingView::deleteLoadingView();
    delete console;
    console = 0;
}

//-----------------------------------------------------------------
void DedicatedGameManager::initializeGameConfig(const std::string& configfile)
{
    if(configfile == "")
        gameconfig = new GameConfig("/config/netpanzer-dedicated.ini");
    else
        gameconfig = new GameConfig(configfile, false);
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
                ChatInterface::setMessageScopeServer();
                ChatInterface::sendCurrentMessage(command.argument.c_str());
                break;
            }
            case ServerCommand::STATUS:
            {
                //*Console::server
                std::cout
                    << "Server " << gameconfig->playername
                    << " version " << PACKAGE_VERSION << " port "
                    << gameconfig->serverport << "\n"
                    << "Map: " << gameconfig-> map << "\n"
                    << std::setw(3) << "ID" << " "
                    << std::setw(30) << "Name" << " "
                    << std::setw(4) << "Kill" << " "
                    << std::setw(4) << "Lost" << " "
                    << std::setw(5) << "Score" << " "
                    << std::setw(21) << "IP\n";
                for(size_t i = 0; i<PlayerInterface::getMaxPlayers(); ++i) {
                    PlayerState* playerstate =
                        PlayerInterface::getPlayer(i);
                    if(playerstate->getStatus() != _player_state_active)
                        continue;
                    //*Console::server
                    std::cout
                        << std::setw(3) << playerstate->getID() << " "
                        << std::setw(30) << playerstate->getName() << " "
                        << std::setw(4) << playerstate->getKills() << " "
                        << std::setw(4) << playerstate->getLosses() << " "
                        << std::setw(5) << playerstate->getTotal() << " "
                        << std::setw(21) 
                        << NetworkServer::getIP(playerstate->getID())
                        << "\n";
                }
                //*Console::server << std::flush;
                std::cout << std::flush;
                break;
            }
            case ServerCommand::MAPCHANGE:
                if(!MapsManager::existsMap(command.argument)) {
                    std::cout << "map '" << command.argument
                        << "' doesn't exist." << std::endl;
                    break;
                }
            
                GameControlRulesDaemon::forceMapChange(command.argument);
                std::cout << "Preparing mapchange..." << std::endl;
                break;
            case ServerCommand::KICK:
            {
                std::stringstream idstream(command.argument);
                Uint16 id = 0xffff;
                idstream >> id;
                if(id >= PlayerInterface::getMaxPlayers()) {
                    std::cout << "Unknown player." << std::endl;
                    break;
                }
                NetworkServer::dropClient(id);
                break;
            }
            case ServerCommand::ADDBOT:
            {
                Uint16 botid = BotManager::addBot();
                if ( botid != 0xffff )
                {
                    std::cout << "Added bot with player id " << botid << std::endl;
                }
                break;
            }
            case ServerCommand::REMOVEBOTS:
                BotManager::removeAllBots();
                break;
        }
        commandqueue.pop();
    }
    SDL_mutexV(commandqueue_mutex);
}

bool
DedicatedGameManager::mainLoop()
{
    if ( heartbeat )
        heartbeat->checkHeartbeat();
    return BaseGameManager::mainLoop();
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
    *Console::server << "starting dedicated netPanzer server\n";

    gameconfig->map = MapsManager::getNextMap("");

    GameManager::dedicatedLoadGameMap(gameconfig->map.c_str());

    GameManager::reinitializeGameLogic();

    MessageRouter::initialize(true);
    NetworkServer::openSession();
    NetworkServer::hostSession();

    GameControlRulesDaemon::setStateServerInProgress();
    GameControlRulesDaemon::setDedicatedServer();
    NetworkState::setNetworkStatus( _network_state_server );

    GameManager::setNetPanzerGameOptions();

    gameconfig->hostorjoin=_game_session_host;

    Particle2D::setCreateParticles(false);

    *Console::server << "contacting masterserver." << std::endl;
    if((bool) gameconfig->publicServer &&
        (const std::string&) gameconfig->masterservers != "") {
        try {
            if ( infosocket ) {
                delete infosocket;
                infosocket = 0;
            }
            infosocket = new InfoSocket(gameconfig->serverport);
            if ( heartbeat ) {
                delete heartbeat;
                heartbeat = 0;
            }
            heartbeat = new Heartbeat();
        } catch(std::exception& e) {
            LOGGER.warning("heartbeats disabled: %s", e.what());
            if ( infosocket ) {
                delete infosocket;
                infosocket = 0;
            }
            if ( heartbeat ) {
                delete heartbeat;
                heartbeat = 0;
            }
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
    if ( infosocket ) {
        delete infosocket;
        infosocket = 0;
    }

    if ( heartbeat ) {
        delete heartbeat;
        heartbeat=0;
    }
    BaseGameManager::shutdownNetworkSubSystem();
}
