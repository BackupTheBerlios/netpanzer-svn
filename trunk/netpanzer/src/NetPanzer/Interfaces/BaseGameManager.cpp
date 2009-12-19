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

#include "BaseGameManager.hpp"
#include "Core/GlobalEngineState.hpp"
#include "SDL.h"

// ** PObject netPanzer Network Includes
#include "Classes/Network/ServerConnectDaemon.hpp"
#include "Classes/Network/NetworkState.hpp"
#include "Classes/Network/NetworkServer.hpp"
#include "Classes/Network/NetworkClient.hpp"

#include "Util/Log.hpp"
#include "Interfaces/GameConfig.hpp"
#include "Interfaces/GameManager.hpp"
#include "System/DummySound.hpp"
#include "Interfaces/PlayerInterface.hpp"
#include "Units/UnitInterface.hpp"
#include "Particles/Particle2D.hpp"
#include "Particles/ParticleSystem2D.hpp"
#include "Particles/ParticleInterface.hpp"
#include "PathScheduler.hpp"
#include "Weapons/ProjectileInterface.hpp"
#include "Units/UnitProfileInterface.hpp"
#include "Objectives/ObjectiveInterface.hpp"
#include "Interfaces/ConsoleInterface.hpp"
#include "MapsManager.hpp"
#include "PowerUps/PowerUpInterface.hpp"
#include "Interfaces/GameControlRulesDaemon.hpp"
#include "Util/Exception.hpp"
#include "Util/FileSystem.hpp"
#include "Resources/ResourceManager.hpp"
#include "2D/Palette.hpp"


//** Physics/Particle Stuff
#include "Particles/Physics.hpp"
#include "Util/TimerInterface.hpp"

#include "Network/SocketManager.hpp"
#include "Network/MessageRouter.hpp"
#include "Bot/BotManager.hpp"

//------------------------------------------------------------------
BaseGameManager::BaseGameManager()
    : running(true)
{
}

//------------------------------------------------------------------
BaseGameManager::~BaseGameManager()
{
}

//-----------------------------------------------------------------
Sound * BaseGameManager::initializeSoundSubSystem()
{
     return new DummySound();
}
//-----------------------------------------------------------------
void BaseGameManager::shutdownSoundSubSystem()
{
    if ( global_engine_state->sound_manager )
    {
        delete global_engine_state->sound_manager;
        global_engine_state->sound_manager = 0;
    }
}
//-----------------------------------------------------------------
void BaseGameManager::initializeGameConfig(const std::string& configfile)
{
    if(configfile == "")
        gameconfig = new GameConfig("config/netpanzer.ini");
    else
        gameconfig = new GameConfig(configfile, false);
}
//-----------------------------------------------------------------
void BaseGameManager::shutdownGameConfig()
{
    if (gameconfig) {
        delete gameconfig;
        gameconfig = 0;
    }
}
//-----------------------------------------------------------------
void BaseGameManager::initializeInputDevices()
{
}
//-----------------------------------------------------------------
void BaseGameManager::shutdownInputDevices()
{}
//-----------------------------------------------------------------
void BaseGameManager::initializeGameObjects()
{
    loadGameData(); // load unit profiles and start resource manager.
                   // unit profiles should be loaded when real game starts

    Physics::init(); // ok, only precalculates some values.
    Weapon::init(); // only load images, should be on the resourcemanager.
    PowerUpInterface::initialize(); // only load images

    ConsoleInterface::initialize(25); // aargh

//    GameManager::initializeGameLogic(); // should be initialized when real game starts.
}
//-----------------------------------------------------------------
void BaseGameManager::shutdownGameObjects()
{}
//-----------------------------------------------------------------
void BaseGameManager::loadGameData()
{
    global_game_state = new GlobalGameState();

    UnitProfileInterface * upi = new UnitProfileInterface();
    upi->loadUnitProfiles();
    if ( global_game_state->unit_profile_interface )
    {
        delete global_game_state->unit_profile_interface;
    }
    global_game_state->unit_profile_interface = upi;

    int numflags = ResourceManager::loadDefaultFlags();
    if ( numflags <= 0 )
    {
        throw Exception("Couldn't find any flag in pics/flags/.");
    }
}
//-----------------------------------------------------------------
void BaseGameManager::initializeNetworkSubSystem()
{
    //SERVER = new NetworkServer();
    //CLIENT = new NetworkClient();

    ServerConnectDaemon::initialize( gameconfig->maxplayers ); // only if server

    NetworkState::setNetworkStatus( _network_state_server );
    NetworkState::resetNetworkStats();
}
//-----------------------------------------------------------------
void BaseGameManager::shutdownNetworkSubSystem()
{
    NetworkServer::closeSession();
    NetworkClient::partServer();
}
//-----------------------------------------------------------------
// boots up netPanzer; initializes all subsystems, game objects etc.
void BaseGameManager::initialize(const std::string& configfile)
{
    try
    {
        if( !filesystem::exists("config") )
        {
            filesystem::mkdir("config");
        }

        initializeGameConfig(configfile);
        global_engine_state->sound_manager = initializeSoundSubSystem();
        initializeVideoSubSystem();
        initializeGameObjects(); // aargh
        initializeNetworkSubSystem();
        initializeInputDevices();
    }
    catch(std::exception& e)
    {
        LOGGER.warning("Initialisation failed:\n%s", e.what());
        shutdown();
        throw Exception("bootstrap failed.");
    }
}
//-----------------------------------------------------------------
void BaseGameManager::shutdown()
{
    shutdownSubSystems();
}
//-----------------------------------------------------------------
void BaseGameManager::shutdownSubSystems()
{
    shutdownGameLogic();

    shutdownNetworkSubSystem();
    shutdownSoundSubSystem();
    shutdownVideoSubSystem();
    shutdownInputDevices();
    shutdownGameConfig();
}
//-----------------------------------------------------------------
bool BaseGameManager::mainLoop()
{
    TimerInterface::start();

    inputLoop();
    graphicsLoop();
    simLoop();

    sleeping();
    TimerInterface::update();

    return running;
}
//-----------------------------------------------------------------
/**
 * Sleep to make stable FPS and happy CPU.
 */
void
BaseGameManager::sleeping()
{
    static Uint32 nextTime = 0;

    Uint32 now = SDL_GetTicks();
    if (now < nextTime) {
        SDL_Delay(nextTime - now);
    }
    nextTime += TIMEINTERVAL;
}

//-----------------------------------------------------------------
void BaseGameManager::simLoop()
{
    NetworkServer::cleanUpClientList();
    network::SocketManager::handleEvents();

    // GameControlRulesDaemon might take some packets before they are routed.
    GameControlRulesDaemon::updateGameControlFlow();
    MessageRouter::routePackets();

    if ( NetworkState::status == _network_state_server )
    {
        ServerConnectDaemon::connectProcess();
    }
//    else
//    {
//        ClientConnectDaemon::connectProcess();
//    }

    NetworkState::updateNetworkStats();

//    GameControlRulesDaemon::updateGameControlFlow(); // moved up

    NetworkServer::sendRemaining();
    NetworkClient::sendRemaining();
}
//-----------------------------------------------------------------
void BaseGameManager::inputLoop()
{
}
//-----------------------------------------------------------------
void BaseGameManager::graphicsLoop()
{
}

//---------------------------------------------------------------------
void
BaseGameManager::stopMainLoop()
{
    running = false;
}

void
BaseGameManager::initializeGameLogic()
{
    PlayerInterface::initialize(gameconfig->maxplayers); // can pas

    UnitInterface * uin = new UnitInterface();
    uin->initialize(gameconfig->maxunits); // uses map
    global_game_state->unit_manager = uin;

    PathScheduler::initialize(); // uses map
    PowerUpInterface::resetLogic(); // depends on map loaded

    startGameTimer();
}

void
BaseGameManager::reinitializeGameLogic()
{
    shutdownGameLogic();
    initializeGameLogic();
}

bool
BaseGameManager::resetGameLogic()
{
    PlayerInterface::reset();

    if ( global_game_state->unit_manager )
    {
        global_game_state->unit_manager->reset();
    }

    PathScheduler::initialize();
    PowerUpInterface::resetLogic();
    ProjectileInterface::resetLogic();

    startGameTimer();
    return true;
}

void
BaseGameManager::shutdownGameLogic()
{
    PlayerInterface::cleanUp();

    if ( global_game_state->unit_manager )
    {
        delete global_game_state->unit_manager;
        global_game_state->unit_manager = 0;
    }

//    UnitInterface::cleanUp();
    PathScheduler::cleanUp();
    //ObjectiveInterface::cleanUp();

    game_elapsed_time_offset = 0;
}

void
BaseGameManager::startGameTimer()
{
    time( &game_start_time );
}

time_t
BaseGameManager::getGameTime()
{
    time_t current_time;

    time( &current_time );

    return( (current_time - game_start_time) + game_elapsed_time_offset );
}
