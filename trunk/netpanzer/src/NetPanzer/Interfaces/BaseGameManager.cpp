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

#include <SDL.h>
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

//-----------------------------------------------------------------
void BaseGameManager::initializeVideoSubSystem()
{
    lobbyView = new ConsoleLoadingView();
    progressView = new ConsoleLoadingView();
}
//-----------------------------------------------------------------
void BaseGameManager::shutdownVideoSubSystem()
{
    if (lobbyView) {
        delete lobbyView;
    }
    if (progressView) {
        delete progressView;
    }
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
        gameconfig = new GameConfig("config/netpanzer.xml");
    else
        gameconfig = new GameConfig(configfile);
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

    MapsManager::initialize();
    MapsManager::scanMaps();

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
    if(UNIT_FLAGS_SURFACE.getFrameCount() == 0) {
        throw Exception("Couldn't find any flag in pics/flags/.");
    }
    if(gameconfig->playerflag.isDefaultValue()) {
        gameconfig->playerflag=rand()%UNIT_FLAGS_SURFACE.getFrameCount();
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
        if(!FileSystem::exists("config"))
            FileSystem::mkdir("config");
        initializeGameConfig(configfile);
        initializeSoundSubSystem();
        initializeVideoSubSystem();
        initializeGameObjects();
        initializeNetworkSubSystem();
        initializeInputDevices();
    } catch(Exception &e) {
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
void BaseGameManager::mainLoop()
{
    TimerInterface::start();

    inputLoop();
    graphicsLoop();
    simLoop();

    sleeping();
    TimerInterface::update();
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

    // to protect from time slide
    //nextTime = SDL_GetTicks() + TIMEINTERVAL;
}

//-----------------------------------------------------------------
void BaseGameManager::simLoop()
{
    CLIENT->checkIncoming();
    SERVER->checkIncoming();
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
    gAbramsTurret.load( "units/pics/pak/TitaTNSD.pak" );
    gAbramsBody.load( "units/pics/pak/TitaHNSD.pak" );

    gLeopardTurret.load( "units/pics/pak/PantTNSD.pak" );
    gLeopardBody.load( "units/pics/pak/PantHNSD.pak" );

    gValentineTurret.load( "units/pics/pak/MantTNSD.pak" );
    gValentineBody.load( "units/pics/pak/MantHNSD.pak" );

    gHammerheadTurret.load( "units/pics/pak/StinTNSD.pak" );
    gHammerheadBody.load( "units/pics/pak/StinHNSD.pak" );

    //gHumveeBody.load( "units/pics/pak/ScouHNSD.pak" );

    gScorpionTurret.load( "units/pics/pak/WolfTNSD.pak" );
    gScorpionBody.load( "units/pics/pak/WolfHNSD.pak" );

    gLynxTurret.load( "units/pics/pak/BobcTNSD.pak" );
    gLynxBody.load( "units/pics/pak/BobcHNSD.pak" );

    gM109Turret.load( "units/pics/pak/DrakTNSD.pak" );
    gM109Body.load( "units/pics/pak/DrakHNSD.pak" );

    gBearTurret.load( "units/pics/pak/BearTNSD.pak" );
    gBearBody.load( "units/pics/pak/BearHNSD.pak" );

    gSpahPanzerTurret.load( "units/pics/pak/SpahTNSD.pak" );
    gSpahPanzerBody.load( "units/pics/pak/SpahHNSD.pak" );

    gArcherTurret.load( "units/pics/pak/ArchTNSD.pak" );
    gArcherBody.load( "units/pics/pak/ArchHNSD.pak" );

#ifdef _GRAY_MAPPED_UNITS
    Palette::init("wads/netp.act");
    ColorTable gray256;

    float grayPercent = 1.25f;

    // 256 shades of gray.
    gray256.init(256);
    for (int num = 0; num < 256; num++) {
        int c            = Palette::color[num].getBrightnessInt();
        int nearestColor = Palette::findNearestColor(
		RGBColor(int(c * grayPercent), int(c * grayPercent),
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

    gAbramsTurretDarkBlue.load( "units/pics/pak/TitaTNSD.pak" );
    gAbramsBodyDarkBlue.load( "units/pics/pak/TitaHNSD.pak" );

    gLeopardTurretDarkBlue.load( "units/pics/pak/PantTNSD.pak" );
    gLeopardBodyDarkBlue.load( "units/pics/pak/PantHNSD.pak" );

    gValentineTurretDarkBlue.load( "units/pics/pak/MantTNSD.pak" );
    gValentineBodyDarkBlue.load( "units/pics/pak/MantHNSD.pak" );

    gHammerheadTurretDarkBlue.load( "units/pics/pak/StinTNSD.pak" );
    gHammerheadBodyDarkBlue.load( "units/pics/pak/StinHNSD.pak" );

    //gHumveeBodyDarkBlue.load( "units/pics/pak/ScouHNSD.pak" );

    gScorpionTurretDarkBlue.load( "units/pics/pak/WolfTNSD.pak" );
    gScorpionBodyDarkBlue.load( "units/pics/pak/WolfHNSD.pak" );

    gLynxTurretDarkBlue.load( "units/pics/pak/BobcTNSD.pak" );
    gLynxBodyDarkBlue.load( "units/pics/pak/BobcHNSD.pak" );

    gM109TurretDarkBlue.load( "units/pics/pak/DrakTNSD.pak" );
    gM109BodyDarkBlue.load( "units/pics/pak/DrakHNSD.pak" );

    gBearTurretDarkBlue.load( "units/pics/pak/BearTNSD.pak" );
    gBearBodyDarkBlue.load( "units/pics/pak/BearHNSD.pak" );

    gSpahPanzerTurretDarkBlue.load( "units/pics/pak/SpahTNSD.pak" );
    gSpahPanzerBodyDarkBlue.load( "units/pics/pak/SpahHNSD.pak" );

    gArcherTurretDarkBlue.load( "units/pics/pak/ArchTNSD.pak" );
    gArcherBodyDarkBlue.load( "units/pics/pak/ArchHNSD.pak" );

#endif
    //********** SHADOWS ************

    gAbramsTurretShadow.load( "units/pics/pak/TitaTSSD.pak" );
    gAbramsBodyShadow.load( "units/pics/pak/TitaHSSD.pak" );

    gLeopardTurretShadow.load( "units/pics/pak/PantTSSD.pak" );
    gLeopardBodyShadow.load( "units/pics/pak/PantHSSD.pak" );

    gValentineTurretShadow.load( "units/pics/pak/MantTSSD.pak" );
    gValentineBodyShadow.load( "units/pics/pak/MantHSSD.pak" );

    gHammerheadTurretShadow.load( "units/pics/pak/StinTSSD.pak" );
    gHammerheadBodyShadow.load( "units/pics/pak/StinHSSD.pak" );

    //gHumveeBodyShadow.load( "units/pics/pak/ScouHSSD.pak" );

    gLynxTurretShadow.load( "units/pics/pak/BobcTSSD.pak" );
    gLynxBodyShadow.load( "units/pics/pak/BobcHSSD.pak" );

    gM109TurretShadow.load( "units/pics/pak/DrakTSSD.pak" );
    gM109BodyShadow.load( "units/pics/pak/DrakHSSD.pak" );

    gSpahPanzerTurretShadow.load( "units/pics/pak/SpahTSSD.pak" );
    gSpahPanzerBodyShadow.load( "units/pics/pak/SpahHSSD.pak" );

    gBearTurretShadow.load( "units/pics/pak/BearTSSD.pak" );
    gBearBodyShadow.load( "units/pics/pak/BearHSSD.pak" );

    gScorpionTurretShadow.load( "units/pics/pak/WolfTSSD.pak" );
    gScorpionBodyShadow.load( "units/pics/pak/WolfHSSD.pak" );

    gArcherTurretShadow.load( "units/pics/pak/ArchTSSD.pak" );
    gArcherBodyShadow.load( "units/pics/pak/ArchHSSD.pak" );
}
