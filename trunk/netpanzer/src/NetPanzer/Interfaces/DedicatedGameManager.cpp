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
#include "Util/Log.hpp"
#include "Server.hpp"
#include "IRCLobbyView.hpp"
#include "LobbyView.hpp"
#include "ProgressView.hpp"
#include "ConsoleLoadingView.hpp"

void DedicatedGameManager::initializeVideoSubSystem()
{
    lobbyView = new ConsoleLoadingView();
    progressView = new ConsoleLoadingView();
    commandqueue_mutex = SDL_CreateMutex();
}

void DedicatedGameManager::shutdownVideoSubSystem()
{
    delete lobbyView;
    delete progressView;
    SDL_DestroyMutex(commandqueue_mutex);
}

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
    // handle server commands
    SDL_mutexP(commandqueue_mutex);
    while(!commandqueue.empty()) {



    }
    SDL_mutexV(commandqueue_mutex);
}

//-----------------------------------------------------------------
bool DedicatedGameManager::launchNetPanzerGame()
{
    ConsoleInterface::postMessage("starting dedicated netPanzer server");

    gameconfig->map = MapsManager::getNextMap("");
    
    ConsoleInterface::postMessage("Server Settings:"); 
    ConsoleInterface::postMessage("Map: %s", gameconfig->map.c_str());
    ConsoleInterface::postMessage("MaxPlayers: %d",
            (int) gameconfig->maxplayers);
    ConsoleInterface::postMessage("MaxUnits: %d", (int) gameconfig->maxunits);
    ConsoleInterface::postMessage("Gametype: %s",
            gameconfig->getGameTypeString());
    ConsoleInterface::postMessage("ObjectivePercentage: %d",
            (int) gameconfig->objectiveoccupationpercentage);
    ConsoleInterface::postMessage("TimeLimit: %d",
            (int) gameconfig->timelimit);   
    ConsoleInterface::postMessage("FragLimit: %d", (int) gameconfig->fraglimit);
    ConsoleInterface::postMessage("RespawnType: %s",
            gameconfig->getRespawnTypeString());
    ConsoleInterface::postMessage("Mapcycle: %s", gameconfig->mapcycle.c_str());
    ConsoleInterface::postMessage("Powerups: %s",
            gameconfig->powerups ? "yes" : "no");
    ConsoleInterface::postMessage("AllowAllies: %s",
            gameconfig->allowallies ? "yes" : "no");
    ConsoleInterface::postMessage("CloudCoverage: %d (WindSpeed %d)", 
            (int) gameconfig->cloudcoverage, (int) gameconfig->windspeed);

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
    if(!lobby_view)
        lobby_view = new IRCLobbyView;
    lobby_view->startIRC();

    ConsoleInterface::postMessage("game started.");

    GameManager::startGameTimer();
    return true;
}
