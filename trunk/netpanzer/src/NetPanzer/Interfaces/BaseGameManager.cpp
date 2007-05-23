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
#include "BaseGameManager.hpp"

#include "SDL.h"
#include "NetworkServerUnix.hpp"
#include "NetworkClientUnix.hpp"

// ** PObject netPanzer Network Includes
#include "Server.hpp"
#include "Client.hpp"
#include "ClientMessageRouter.hpp"
#include "ServerConnectDaemon.hpp"
#include "ServerMessageRouter.hpp"
#include "NetworkState.hpp"

#include "Util/Log.hpp"
#include "GameConfig.hpp"
#include "GameManager.hpp"
#include "System/DummySound.hpp"
#include "PlayerInterface.hpp"
#include "UnitInterface.hpp"
#include "UnitBlackBoard.hpp"
#include "ProjectileInterface.hpp"
#include "Particle2D.hpp"
#include "ParticleSystem2D.hpp"
#include "ParticleInterface.hpp"
#include "PathScheduler.hpp"
#include "UnitProfileInterface.hpp"
#include "ObjectiveInterface.hpp"
#include "ConsoleInterface.hpp"
#include "MapsManager.hpp"
#include "PowerUpInterface.hpp"
#include "GameControlRulesDaemon.hpp"
#include "Util/Exception.hpp"
#include "Util/FileSystem.hpp"
#include "LobbyView.hpp"
#include "ProgressView.hpp"
#include "ConsoleLoadingView.hpp"
#include "SelectionBoxSprite.hpp"

#include "UnitGlobals.hpp"
#include "2D/ColorTable.hpp"
#include "2D/Palette.hpp"


//** Physics/Particle Stuff
#include "Physics.hpp"
#include "Util/TimerInterface.hpp"

#include "Network/SocketManager.hpp"

BaseGameManager* gamemanager = 0;

//------------------------------------------------------------------
BaseGameManager::BaseGameManager()
    : running(true)
{
    assert(gamemanager == 0);
    gamemanager = this;
}

//------------------------------------------------------------------
BaseGameManager::~BaseGameManager()
{
    gamemanager = 0;
}

//-----------------------------------------------------------------
void BaseGameManager::initializeSoundSubSystem()
{
    sound = new DummySound();
}
//-----------------------------------------------------------------
void BaseGameManager::shutdownSoundSubSystem()
{
    if(sound) {
        delete sound;
        sound = 0;
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
    loadGameData();

    Physics::init();
    Weapon::init();

    ConsoleInterface::initialize(25);
    PowerUpInterface::initialize();

    GameManager::initializeGameLogic();
}
//-----------------------------------------------------------------
void BaseGameManager::shutdownGameObjects()
{}
//-----------------------------------------------------------------
void BaseGameManager::loadGameData()
{
    UnitProfileInterface::loadUnitProfiles();
    LoadUnitSurfaces();
    UNIT_FLAGS_SURFACE.loadAllBMPInDirectory("pics/flags/");
    if(UNIT_FLAGS_SURFACE.getNumFrames() == 0) {
        throw Exception("Couldn't find any flag in pics/flags/.");
    }
    if(gameconfig->playerflag.isDefaultValue()) {
        gameconfig->playerflag=rand()%UNIT_FLAGS_SURFACE.getNumFrames();
    }
}
//-----------------------------------------------------------------
void BaseGameManager::initializeNetworkSubSystem()
{
    SERVER = new NetworkServerUnix();
    CLIENT = new NetworkClientUnix();

    ServerMessageRouter::initialize();
    ClientMessageRouter::initialize();

    ServerConnectDaemon::initialize( gameconfig->maxplayers );

    NetworkState::setNetworkStatus( _network_state_server );
    NetworkState::resetNetworkStats();
}
//-----------------------------------------------------------------
void BaseGameManager::shutdownNetworkSubSystem()
{
    if(SERVER) {
        SERVER->closeSession();
        ServerMessageRouter::cleanUp();
        delete SERVER;
        SERVER = 0;
    }
    if(CLIENT) {
        CLIENT->partServer();
        ClientMessageRouter::cleanUp();
        delete CLIENT;
        CLIENT = 0;
    }
}
//-----------------------------------------------------------------
// boots up netPanzer; initializes all subsystems, game objects etc.
void BaseGameManager::initialize(const std::string& configfile)
{
    try {
        if(!filesystem::exists("config"))
            filesystem::mkdir("config");
        initializeGameConfig(configfile);
        initializeSoundSubSystem();
        initializeVideoSubSystem();
        initializeGameObjects();
        initializeNetworkSubSystem();
        initializeInputDevices();
    } catch(std::exception& e) {
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
    GameManager::shutdownGameLogic();
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
#if 1
    static Uint32 nextTime = 0;

    Uint32 now = SDL_GetTicks();
    if (now < nextTime) {
        SDL_Delay(nextTime - now);
    }
    nextTime += TIMEINTERVAL;
#endif
}

//-----------------------------------------------------------------
void BaseGameManager::simLoop()
{
    network::SocketManager::handleEvents();
    
    if ( NetworkState::status == _network_state_server ) {
        ServerMessageRouter::routeMessages();
    } else {
        ClientMessageRouter::routeMessages();
    }

    NetworkState::updateNetworkStats();

    UnitInterface::updateUnitStatus();
    ProjectileInterface::updateStatus();
    ObjectiveInterface::updateObjectiveStatus();
    PowerUpInterface::updateState();
    PathScheduler::run();

    Physics::sim();

    ParticleSystem2D::simAll();
    Particle2D::simAll();

    GameControlRulesDaemon::updateGameControlFlow();
    if ( SERVER )
        SERVER->sendRemaining();
    if ( CLIENT )
        CLIENT->sendRemaining();
}
//-----------------------------------------------------------------
void BaseGameManager::inputLoop()
{
}
//-----------------------------------------------------------------
void BaseGameManager::graphicsLoop()
{
}
//-----------------------------------------------------------------
void BaseGameManager::LoadUnitSurfaces()
{
    Palette::init("netp");
    gAbramsTurret.loadPAK( "units/pics/pak/TitaTNSD.pak" );
    gAbramsBody.loadPAK( "units/pics/pak/TitaHNSD.pak" );

    gLeopardTurret.loadPAK( "units/pics/pak/PantTNSD.pak" );
    gLeopardBody.loadPAK( "units/pics/pak/PantHNSD.pak" );

    gValentineTurret.loadPAK( "units/pics/pak/MantTNSD.pak" );
    gValentineBody.loadPAK( "units/pics/pak/MantHNSD.pak" );

    gHammerheadTurret.loadPAK( "units/pics/pak/StinTNSD.pak" );
    gHammerheadBody.loadPAK( "units/pics/pak/StinHNSD.pak" );

    //gHumveeBody.load( "units/pics/pak/ScouHNSD.pak" );

    gScorpionTurret.loadPAK( "units/pics/pak/WolfTNSD.pak" );
    gScorpionBody.loadPAK( "units/pics/pak/WolfHNSD.pak" );

    gLynxTurret.loadPAK( "units/pics/pak/BobcTNSD.pak" );
    gLynxBody.loadPAK( "units/pics/pak/BobcHNSD.pak" );

    gM109Turret.loadPAK( "units/pics/pak/DrakTNSD.pak" );
    gM109Body.loadPAK( "units/pics/pak/DrakHNSD.pak" );

    gBearTurret.loadPAK( "units/pics/pak/BearTNSD.pak" );
    gBearBody.loadPAK( "units/pics/pak/BearHNSD.pak" );

    gSpahPanzerTurret.loadPAK( "units/pics/pak/SpahTNSD.pak" );
    gSpahPanzerBody.loadPAK( "units/pics/pak/SpahHNSD.pak" );

    gArcherTurret.loadPAK( "units/pics/pak/ArchTNSD.pak" );
    gArcherBody.loadPAK( "units/pics/pak/ArchHNSD.pak" );

//#ifdef _GRAY_MAPPED_UNITS
#if 0
    ColorTable gray256;

    float grayPercent = 1.25f;

    // 256 shades of gray.
    gray256.init(256);
    for (int num = 0; num < 256; num++) {
        int c = int(Palette::color[num].r+Palette::color[num].g+Palette::color[num].b)/3; //brightness
        int nearestColor = Palette::findNearestColor(
		SDL_Color(int(c * grayPercent), int(c * grayPercent),
			 int(c * grayPercent)));
        gray256.setColor(num, nearestColor);
    }

    gray256.setColor(255, 0);

    //********** DARK BLUE UNITS ************
    colorMapGray(gAbramsTurret, gAbramsTurretDarkBlue, gray256);
    colorMapGray(gAbramsBody, gAbramsBodyDarkBlue, gray256);

    colorMapGray(gLeopardTurret, gLeopardTurretDarkBlue, gray256);
    colorMapGray(gLeopardBody, gLeopardBodyDarkBlue, gray256);

    colorMapGray(gValentineTurret, gValentineTurretDarkBlue, gray256);
    colorMapGray(gValentineBody, gValentineBodyDarkBlue, gray256);

    colorMapGray(gHammerheadTurret, gHammerheadTurretDarkBlue, gray256);
    colorMapGray(gHammerheadBody, gHammerheadBodyDarkBlue, gray256);

    //colorMapGray(gHumveeBody, gHumveeBodyDarkBlue, gray256);

    colorMapGray(gScorpionTurret, gScorpionTurretDarkBlue, gray256);
    colorMapGray(gScorpionBody, gScorpionBodyDarkBlue, gray256);

    colorMapGray(gLynxTurret, gLynxTurretDarkBlue, gray256);
    colorMapGray(gLynxBody, gLynxBodyDarkBlue, gray256);

    colorMapGray(gM109Turret, gM109TurretDarkBlue, gray256);
    colorMapGray(gM109Body, gM109BodyDarkBlue, gray256);

    colorMapGray(gBearTurret, gBearTurretDarkBlue, gray256);
    colorMapGray(gBearBody, gBearBodyDarkBlue, gray256);

    colorMapGray(gSpahPanzerTurret, gSpahPanzerTurretDarkBlue, gray256);
    colorMapGray(gSpahPanzerBody, gSpahPanzerBodyDarkBlue, gray256);

    colorMapGray(gArcherTurret, gArcherTurretDarkBlue, gray256);
    colorMapGray(gArcherBody, gArcherBodyDarkBlue, gray256);

#else
    //********** DARK BLUE UNITS ************

    gAbramsTurretDarkBlue.loadPAK( "units/pics/pak/TitaTNSD.pak" );
    gAbramsBodyDarkBlue.loadPAK( "units/pics/pak/TitaHNSD.pak" );

    gLeopardTurretDarkBlue.loadPAK( "units/pics/pak/PantTNSD.pak" );
    gLeopardBodyDarkBlue.loadPAK( "units/pics/pak/PantHNSD.pak" );

    gValentineTurretDarkBlue.loadPAK( "units/pics/pak/MantTNSD.pak" );
    gValentineBodyDarkBlue.loadPAK( "units/pics/pak/MantHNSD.pak" );

    gHammerheadTurretDarkBlue.loadPAK( "units/pics/pak/StinTNSD.pak" );
    gHammerheadBodyDarkBlue.loadPAK( "units/pics/pak/StinHNSD.pak" );

    //gHumveeBodyDarkBlue.load( "units/pics/pak/ScouHNSD.pak" );

    gScorpionTurretDarkBlue.loadPAK( "units/pics/pak/WolfTNSD.pak" );
    gScorpionBodyDarkBlue.loadPAK( "units/pics/pak/WolfHNSD.pak" );

    gLynxTurretDarkBlue.loadPAK( "units/pics/pak/BobcTNSD.pak" );
    gLynxBodyDarkBlue.loadPAK( "units/pics/pak/BobcHNSD.pak" );

    gM109TurretDarkBlue.loadPAK( "units/pics/pak/DrakTNSD.pak" );
    gM109BodyDarkBlue.loadPAK( "units/pics/pak/DrakHNSD.pak" );

    gBearTurretDarkBlue.loadPAK( "units/pics/pak/BearTNSD.pak" );
    gBearBodyDarkBlue.loadPAK( "units/pics/pak/BearHNSD.pak" );

    gSpahPanzerTurretDarkBlue.loadPAK( "units/pics/pak/SpahTNSD.pak" );
    gSpahPanzerBodyDarkBlue.loadPAK( "units/pics/pak/SpahHNSD.pak" );

    gArcherTurretDarkBlue.loadPAK( "units/pics/pak/ArchTNSD.pak" );
    gArcherBodyDarkBlue.loadPAK( "units/pics/pak/ArchHNSD.pak" );

#endif
    //********** SHADOWS ************

    gAbramsTurretShadow.loadPAK( "units/pics/pak/TitaTSSD.pak" );
    gAbramsTurretShadow.setAllColorsBlack();
    gAbramsBodyShadow.loadPAK( "units/pics/pak/TitaHSSD.pak" );
    gAbramsBodyShadow.setAllColorsBlack();

    gLeopardTurretShadow.loadPAK( "units/pics/pak/PantTSSD.pak" );
    gLeopardTurretShadow.setAllColorsBlack();
    gLeopardBodyShadow.loadPAK( "units/pics/pak/PantHSSD.pak" );
    gLeopardBodyShadow.setAllColorsBlack();

    gValentineTurretShadow.loadPAK( "units/pics/pak/MantTSSD.pak" );
    gValentineTurretShadow.setAllColorsBlack();
    gValentineBodyShadow.loadPAK( "units/pics/pak/MantHSSD.pak" );
    gValentineBodyShadow.setAllColorsBlack();

    gHammerheadTurretShadow.loadPAK( "units/pics/pak/StinTSSD.pak" );
    gHammerheadTurretShadow.setAllColorsBlack();
    gHammerheadBodyShadow.loadPAK( "units/pics/pak/StinHSSD.pak" );
    gHammerheadBodyShadow.setAllColorsBlack();

    gLynxTurretShadow.loadPAK( "units/pics/pak/BobcTSSD.pak" );
    gLynxTurretShadow.setAllColorsBlack();
    gLynxBodyShadow.loadPAK( "units/pics/pak/BobcHSSD.pak" );
    gLynxBodyShadow.setAllColorsBlack();

    gM109TurretShadow.loadPAK( "units/pics/pak/DrakTSSD.pak" );
    gM109TurretShadow.setAllColorsBlack();
    gM109BodyShadow.loadPAK( "units/pics/pak/DrakHSSD.pak" );
    gM109BodyShadow.setAllColorsBlack();

    gSpahPanzerTurretShadow.loadPAK( "units/pics/pak/SpahTSSD.pak" );
    gSpahPanzerTurretShadow.setAllColorsBlack();
    gSpahPanzerBodyShadow.loadPAK( "units/pics/pak/SpahHSSD.pak" );
    gSpahPanzerBodyShadow.setAllColorsBlack();

    gBearTurretShadow.loadPAK( "units/pics/pak/BearTSSD.pak" );
    gBearTurretShadow.setAllColorsBlack();
    gBearBodyShadow.loadPAK( "units/pics/pak/BearHSSD.pak" );
    gBearBodyShadow.setAllColorsBlack();

    gScorpionTurretShadow.loadPAK( "units/pics/pak/WolfTSSD.pak" );
    gScorpionTurretShadow.setAllColorsBlack();
    gScorpionBodyShadow.loadPAK( "units/pics/pak/WolfHSSD.pak" );
    gScorpionBodyShadow.setAllColorsBlack();

    gArcherTurretShadow.loadPAK( "units/pics/pak/ArchTSSD.pak" );
    gArcherTurretShadow.setAllColorsBlack();
    gArcherBodyShadow.loadPAK( "units/pics/pak/ArchHSSD.pak" );
    gArcherBodyShadow.setAllColorsBlack();
}

//---------------------------------------------------------------------
void
BaseGameManager::stopMainLoop()
{
    running = false;
}
