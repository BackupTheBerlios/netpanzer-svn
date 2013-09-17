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


#include <exception>

#include "Interfaces/PlayerGameManager.hpp"
#include "Interfaces/BaseGameManager.hpp"
#include "Interfaces/GameManager.hpp"
#include "Interfaces/ChatInterface.hpp"
#include "Interfaces/Console.hpp"
#include "Interfaces/ConsoleInterface.hpp"
#include "Interfaces/MouseInterface.hpp"
#include "Interfaces/KeyboardInterface.hpp"
#include "Interfaces/GameConfig.hpp"
#include "Interfaces/MapInterface.hpp"
#include "Objectives/ObjectiveInterface.hpp"
#include "Interfaces/PathScheduler.hpp"
#include "Interfaces/PlayerGameManager.hpp"
#include "Interfaces/PlayerInterface.hpp"
#include "Interfaces/StrManager.hpp"
#include "PowerUps/PowerUpInterface.hpp"
#include "Weapons/ProjectileInterface.hpp"
#include "Interfaces/TileInterface.hpp"
#include "Units/UnitInterface.hpp"
#include "Units/UnitProfileInterface.hpp"
#include "Interfaces/WorldViewInterface.hpp"

#include "Classes/ScreenSurface.hpp"
#include "Units/UnitBlackBoard.hpp"
#include "Classes/WorldInputCmdProcessor.hpp"
#include "Classes/SpriteSorter.hpp"
#include "Classes/Network/ClientMessageRouter.hpp"
#include "Classes/Network/ClientConnectDaemon.hpp"
#include "Classes/Network/ServerMessageRouter.hpp"
#include "Classes/Network/NetworkServer.hpp"
#include "Classes/Network/NetworkClient.hpp"
#include "Classes/Network/NetworkState.hpp"
#include "Classes/Network/SystemNetMessage.hpp"
#include "Classes/Network/ConnectNetMessage.hpp"
#include "Units/UnitGlobals.hpp"

#include "2D/Palette.hpp"
#include "Views/Components/ViewGlobals.hpp"
#include "Views/Game/LoadingView.hpp"

#include "Particles/Particle2D.hpp"
#include "Particles/ParticleSystem2D.hpp"
#include "Particles/ParticleInterface.hpp"
#include "Particles/Physics.hpp"

#include "System/Sound.hpp"
#include "System/SDLSound.hpp"
#include "System/SDLVideo.hpp"
#include "System/DummySound.hpp"

#include "Util/TimerInterface.hpp"
#include "Util/Math.hpp"
#include "Util/Log.hpp"
#include "Util/Exception.hpp"
#include "Util/FileSystem.hpp"
#include "Util/NTimer.hpp"

#include "Bot/Bot.hpp"

#include "Actions/ActionManager.hpp"
#include "2D/TextRenderingSystem.hpp"

PlayerGameManager::PlayerGameManager()
    : sdlVideo(0), heartbeat(0), infosocket(0)
{
    //fontManager.loadFont("fixed10", "fonts/fixed10.pcf", 10);

    showNewPanel = false;
}

//-----------------------------------------------------------------
void PlayerGameManager::initializeVideoSubSystem()
{
    LOGGER.info("Initializing video mode");
    sdlVideo = new SDLVideo();
    Screen = sdlVideo;
    TextRenderingSystem::initialize();
    GameManager::setVideoMode();
}

//-----------------------------------------------------------------
void PlayerGameManager::shutdownVideoSubSystem()
{
    delete sdlVideo;
    sdlVideo = 0;
    Screen = 0;
}
//-----------------------------------------------------------------
void PlayerGameManager::initializeSoundSubSystem()
{
    delete sound;
    sound = 0;

    LOGGER.info("Initializing sound system.");
    try {
        if ( gameconfig->sound.useSound() )
            sound = new SDLSound();
    } catch(std::exception& e) {
        LOGGER.warning("Couldn't initialize sound: %s", e.what());
    }

    if(sound == 0)
        sound = new DummySound();

    sound->setSoundVolume(gameconfig->sound.getEffectsVol());

    // start some music
    if ( gameconfig->sound.useMusic() )
    {
        sound->playMusic("sound/music/");
        sound->setMusicVolume(gameconfig->sound.getMusicVol());
    }
}
//-----------------------------------------------------------------
void PlayerGameManager::initializeInputDevices()
{
}
//-----------------------------------------------------------------
void PlayerGameManager::initializeWindowSubSystem()
{

    if ( gameconfig->quickConnect && gameconfig->needPassword )
    {
//        lv->setNeedPassword(true);
    }

}
//-----------------------------------------------------------------
void PlayerGameManager::inputLoop()
{
    processSystemKeys();

    BaseGameManager::inputLoop();
}
//-----------------------------------------------------------------
void PlayerGameManager::graphicsLoop()
{
   // empty
}
//-----------------------------------------------------------------
bool PlayerGameManager::launchNetPanzerGame()
{
    ActionManager::addGUIActions();
    initializeWindowSubSystem();
    return true;
}
//-----------------------------------------------------------------
void PlayerGameManager::launchMultiPlayerGame()
{
    if( gameconfig->hostorjoin == _game_session_host ) {
        hostMultiPlayerGame();
    } else {
        joinMultiPlayerGame();
    }
}

//-----------------------------------------------------------------
void PlayerGameManager::shutdownNetworkSubSystem()
{
    if ( infosocket ) {
        delete infosocket;
        infosocket = 0;
    }
    if ( heartbeat ) {
        delete heartbeat;
        heartbeat = 0;
    }
    BaseGameManager::shutdownNetworkSubSystem();
}

//-----------------------------------------------------------------
void PlayerGameManager::hostMultiPlayerGame()
{
    PlayerState *player_state;
    Timer wait;

    GameConfig::game_teammode = false;
    LoadingView::show();
    // refresh the view in each append
    LoadingView::append( _("Launching Server...") );

    try
    {
    	if (CLIENT)
        {
            delete CLIENT;
            CLIENT=0;
        }
        CLIENT = new NetworkClient();
        
        SERVER->hostSession();

        if ( gameconfig->host.isPublic()
             && GameConfig::server_masterservers->size() != 0 )
        {
            try
            {
                if ( infosocket )
                {
                    delete infosocket;
                    infosocket = 0;
                }
                infosocket = new InfoSocket(gameconfig->host.getPort());
                
                if ( heartbeat )
                {
                    delete heartbeat;
                    heartbeat = 0;
                }
                heartbeat = new Heartbeat();
                
            }
            catch(std::exception& e)
            {
                LOGGER.warning("heartbeats disabled: %s", e.what());
                if ( infosocket )
                {
                    delete infosocket;
                    infosocket = 0;
                }
                
                if ( heartbeat )
                {
                    delete heartbeat;
                    heartbeat = 0;
                }
            }
        }
    }
    catch(std::exception& e)
    {
        LoadingView::append( _("SERVER LAUNCH FAILED") );
        LoadingView::append(e.what());
        wait.changePeriod( 4 );
        while( !wait.count() );

        ActionManager::runAction("loaderror");
        return;
    }

    // refresh views
    LoadingView::update( NPString(_("Launching Server..."))+"(100%)" );
    graphicsLoop();
    GameControlRulesDaemon::setStateServerInProgress();
    NetworkState::setNetworkStatus( _network_state_server );

    LoadingView::append( _("Loading Game Data") );
    graphicsLoop();

    GameConfig::game_map->assign( GameManager::getNextMapName("") );
    const char* mapname = GameConfig::game_map->c_str();

    try
    {
        GameManager::loadMap(mapname);
    }
    catch(std::exception& e)
    {
        LOGGER.warning("Error while loading map '%s': %s", mapname, e.what());
        ActionManager::runAction("loaderror");
        return;
    }

    int percent_complete = 0;
    char strbuf[256];

    ObjectiveInterface::resetLogic();

    UnitProfileInterface::loadUnitProfiles();
    if ( UnitProfileInterface::getNumUnitTypes() == 0 )
    {
        LoadingView::append( _("ERROR unit profiles") );
        LoadingView::append( _("game.unit_profiles")+ *GameConfig::game_unit_profiles );
        graphicsLoop();
        SDL_Delay(5000);
        
        if ( infosocket )
        {
            delete infosocket;
            infosocket = 0;
        }
        
        if ( heartbeat )
        {
            delete heartbeat;
            heartbeat = 0;
        }
        
        if (CLIENT)
        {
            delete CLIENT;
            CLIENT=0;
        }

        SERVER->closeSession();

        GameControlRulesDaemon::setStateServerIdle();
        ActionManager::runAction("loaderror");
        return;
    }

    sprintf( strbuf, "%s (%d%%)", _("Loading Game Data"), percent_complete);
    LoadingView::update( strbuf );
    graphicsLoop();


    LoadingView::append( _("Initializing Game Logic") );
    graphicsLoop();
    GameManager::reinitializeGameLogic();

    LoadingView::update( NPString(_("Initializing Game Logic"))+" (100%)" );
    graphicsLoop();

    LoadingView::append( _("Spawning Player") );
    graphicsLoop();

    player_state = PlayerInterface::allocateLoopBackPlayer();
    const char* playername = GameConfig::player_name->c_str();
    player_state->setName(playername);

    LoadingView::update( NPString(_("Spawning Player"))+" (100%)" );

    graphicsLoop();

    wait.changePeriod( 3 );
    //while( !wait.count() );

    GameManager::startGameTimer();

    LoadingView::loadFinish();
}

void PlayerGameManager::quitGame()
{
    if ( infosocket )
    {
        delete infosocket;
        infosocket = 0;
    }
    
    if ( heartbeat )
    {
        delete heartbeat;
        heartbeat = 0;
    }
}

//-----------------------------------------------------------------
void PlayerGameManager::joinMultiPlayerGame()
{
    GameManager::setNetPanzerGameOptions();
    //reinitializeGameLogic();
    NetworkState::setNetworkStatus( _network_state_client );

    LoadingView::show();
//    LoadingView *lv = static_cast<LoadingView*>(Desktop::getView("LoadingView"));
//    if ( ! lv->doesNeedPassword() )
//    {
//        CLIENT->joinServer(gameconfig->serverConnect, "");
//        ClientConnectDaemon::startConnectionProcess();
//        sound->playTankIdle();
//    }

}

bool PlayerGameManager::mainLoop()
{
    // this is a bit ugly...
    static bool firstrun = true;
    if(firstrun) {
        firstrun = false;
        if(gameconfig->quickConnect == true) {
            LoadingView::show();
            launchMultiPlayerGame();
        }
    }

    if ( heartbeat )
        heartbeat->checkHeartbeat();

//    handleSDLEvents();

    if ( NetworkState::getNetworkStatus() == _network_state_server )
    {
        static NTimer aktimer(10000); //all 10 sec only
        if (aktimer.isTimeOut())
        {
            aktimer.reset();
            PlayerState * player = 0;
            unsigned long max_players;
            max_players = PlayerInterface::getMaxPlayers();
            for (unsigned long i = 0; i < max_players; i++)
            {
                if ( i != PlayerInterface::getLocalPlayerIndex() )
                {
                    player = PlayerInterface::getPlayer((unsigned short) i);
                    if ( player->isActive() )
                    {
                        if ( player->checkAutokick() )
                        {
                            char chat_string[256];
                            sprintf(chat_string, _("Server kicked '%s' due to inactivity"),player->getName().c_str());
                            LOGGER.info("PSE: %s", chat_string);
                            ChatInterface::serversay(chat_string);
                            SERVER->kickClient((PlayerID)i);
                        }
                    }
                }
            }
        }
    }

    return BaseGameManager::mainLoop();
}

//-----------------------------------------------------------------
void PlayerGameManager::processSystemKeys()
{
//    if (Desktop::getVisible("GameView"))
//    {
        if (   KeyboardInterface::getKeyState( SDLK_LALT )
            || KeyboardInterface::getKeyState( SDLK_RALT ) )
        {
            if (KeyboardInterface::getKeyPressed(SDLK_RETURN))
            {
                gameconfig->video.setFullScreen(!gameconfig->video.useFullScreen());
                GameManager::setVideoMode();
            }
        }

        if ( KeyboardInterface::getKeyState(SDLK_F9) )
        {
            Screen->doScreenshoot();
        }

//        if (KeyboardInterface::getKeyPressed(SDLK_ESCAPE))
//        {
//            if ( ! Desktop::getVisible("OptionsView") )
//            {
//                if ( Desktop::getVisible("HelpScrollView") )
//                {
//                    Desktop::toggleVisibility("HelpScrollView");
//                }
//                else if ( Desktop::getVisible("GFlagSelectionView") )
//                {
//                    Desktop::toggleVisibility("GFlagSelectionView");
//                }
//                else
//                {
//                    View *v = Desktop::getView("MenuTemplateView");
//                    assert(v != 0);
//                    ((MenuTemplateView *)v)->init();
//                    ((MenuTemplateView *)v)->setAlwaysOnBottom(false);
//                    
//                    v = Desktop::getView("OptionsView");
//                    assert(v != 0);
//                    ((OptionsTemplateView *)v)->initButtons();
//                    ((OptionsTemplateView *)v)->setAlwaysOnBottom(false);
//
//                    Desktop::setVisibility("MenuTemplateView", true);
//                    Desktop::setVisibility("OptionsView", true);
//                    Desktop::setActiveView("OptionsView");
//                }
//            }
//            else
//            {
//                Desktop::setVisibility("MenuTemplateView", false);
//                Desktop::setVisibility("OptionsView", false);
//            }
//        }
//    }
}
