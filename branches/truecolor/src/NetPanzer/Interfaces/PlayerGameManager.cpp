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
#include "Core/GlobalEngineState.hpp"

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
#include "Interfaces/MapsManager.hpp"
#include "Objectives/ObjectiveInterface.hpp"
#include "Interfaces/PathScheduler.hpp"
#include "Interfaces/PlayerGameManager.hpp"
#include "Interfaces/PlayerInterface.hpp"
#include "PowerUps/PowerUpInterface.hpp"
#include "Weapons/ProjectileInterface.hpp"
#include "Units/UnitInterface.hpp"
#include "Units/UnitProfileInterface.hpp"
#include "Interfaces/WorldViewInterface.hpp"

#include "Classes/ScreenSurface.hpp"
#include "Classes/WorldInputCmdProcessor.hpp"
#include "Classes/SpriteSorter.hpp"
#include "Classes/Network/ClientConnectDaemon.hpp"
#include "Classes/Network/ServerConnectDaemon.hpp"
#include "Classes/Network/NetworkServer.hpp"
#include "Classes/Network/NetworkClient.hpp"
#include "Classes/Network/NetworkState.hpp"
#include "Classes/Network/SystemNetMessage.hpp"
#include "Classes/Network/ConnectNetMessage.hpp"
#include "Units/Unit.hpp"

#include "2D/Palette.hpp"
#include "Views/Components/Desktop.hpp"
#include "Views/Components/ViewGlobals.hpp"
#include "Views/MainMenu/MainMenuView.hpp"
#include "Views/MainMenu/OptionsTemplateView.hpp"
#include "Views/MainMenu/HelpView.hpp"
#include "Views/MainMenu/Multi/JoinView.hpp"
#include "Views/MainMenu/Multi/HostView.hpp"
#include "Views/MainMenu/Multi/GetSessionView.hpp"
#include "Views/MainMenu/Multi/FlagSelectionView.hpp"
#include "Views/MainMenu/Multi/HostOptionsView.hpp"
#include "Views/MainMenu/Multi/MapSelectionView.hpp"
#include "Views/MainMenu/Multi/PlayerNameView.hpp"
#include "Views/MainMenu/Multi/IPAddressView.hpp"
#include "Views/MainMenu/Multi/ServerListView.hpp"
#include "Views/MainMenu/Options/SoundView.hpp"
#include "Views/MainMenu/Options/ControlsView.hpp"
#include "Views/MainMenu/Options/InterfaceView.hpp"
#include "Views/MainMenu/Options/VisualsView.hpp"

#include "Views/Game/RankView.hpp"
#include "Views/Game/VehicleSelectionView.hpp"
#include "Views/Game/CodeStatsView.hpp"
#include "Views/Game/LibView.hpp"
#include "Views/Game/HelpScrollView.hpp"
#include "Views/Game/AreYouSureResignView.hpp"
#include "Views/Game/AreYouSureExitView.hpp"
#include "Views/Game/GameView.hpp"
#include "Views/Game/MiniMapView.hpp"
#include "Views/Game/DisconectedView.hpp"
#include "Views/Game/LoadingView.hpp"

#include "Particles/Particle2D.hpp"
#include "Particles/ParticleSystem2D.hpp"
#include "Particles/ParticleInterface.hpp"
#include "Particles/Physics.hpp"

#include "System/Sound.hpp"
#include "System/SDLSound.hpp"
#include "System/SDLVideo.hpp"
#include "System/DummySound.hpp"
#include "System/SDLEvents.hpp"

#include "Util/TimerInterface.hpp"
#include "Util/Math.hpp"
#include "Util/Log.hpp"
#include "Util/Exception.hpp"
#include "Util/FileSystem.hpp"

#include "Bot/Bot.hpp"
#include "Network/MessageRouter.hpp"

PlayerGameManager::PlayerGameManager()
    : sdlVideo(0), heartbeat(0), infosocket(0)
{
    // nothing
}

//-----------------------------------------------------------------
void PlayerGameManager::initializeVideoSubSystem()
{
    LOGGER.info("Initializing video mode");
    sdlVideo = new SDLVideo();
    Screen = sdlVideo;
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
Sound * PlayerGameManager::initializeSoundSubSystem()
{
    Sound * s = 0;
    
    LOGGER.info("Initializing sound system.");
    try
    {
        if ( gameconfig->enablesound )
        {
            s = new SDLSound();
        }
    }
    catch(std::exception& e)
    {
        LOGGER.warning("Couldn't initialize sound: %s", e.what());
    }

    if ( s == 0 )
    {
        s = new DummySound();
    }

    s->setSoundVolume(gameconfig->effectsvolume);

    // start some music
    if ( gameconfig->enablemusic )
    {
        s->setMusicVolume(gameconfig->musicvolume);
        s->playMusic("sound/music/");
    }
    return s;
}
//-----------------------------------------------------------------
void PlayerGameManager::initializeInputDevices()
{
    MouseInterface::initialize();
}
//-----------------------------------------------------------------
void PlayerGameManager::initializeWindowSubSystem()
{
    Desktop::add(new GameView(), false); // all of this should be loaded when game starts
    Desktop::add(new RankView(), false);
    Desktop::add(new VehicleSelectionView(), false); // depends on unit profiles loaded
    Desktop::add(new MiniMapView(), false); // depends on map manager
    Desktop::add(new CodeStatsView(), false);
    Desktop::add(new LibView(), false);

    Desktop::add(new HelpScrollView());

    Desktop::add(new LoadingView());

    Desktop::add(new MapSelectionView());
    Desktop::add(new MainMenuView());
    Desktop::add(new JoinView());
    Desktop::add(new HostView());
    Desktop::add(new GetSessionView());
    Desktop::add(new OptionsTemplateView());
    Desktop::add(new HelpView());
    Desktop::add(new SoundView());
    Desktop::add(new ControlsView());
    Desktop::add(new VisualsView());
    Desktop::add(new InterfaceView());
    Desktop::add(new FlagSelectionView());
    Desktop::add(new HostOptionsView());
    Desktop::add(new PlayerNameView());
    Desktop::add(new AreYouSureResignView());
    Desktop::add(new AreYouSureExitView());
    Desktop::add(new DisconectedView());

    Desktop::add(new IPAddressView());
    Desktop::add(new ServerListView());
    Desktop::add(new DesktopView());

    Desktop::setVisibilityAllWindows(false);
    Desktop::setVisibility("MainView", true);

    Desktop::checkResolution(iXY(640,480), iXY(screen->getWidth(),screen->getHeight()));
    Desktop::checkViewPositions(iXY(screen->getWidth(),screen->getHeight()));
}
//-----------------------------------------------------------------
void PlayerGameManager::inputLoop()
{
    processSystemKeys();
    MouseInterface::manageClickTimer();
    Desktop::manage(MouseInterface::getMouseX(),
               MouseInterface::getMouseY(), MouseInterface::getButtonMask());
}
//-----------------------------------------------------------------
void PlayerGameManager::graphicsLoop()
{
    screen->lock();

    Desktop::draw();

    MouseInterface::draw(*screen);

    screen->unlock();
    screen->copyToVideoFlip();
}
//-----------------------------------------------------------------
bool PlayerGameManager::launchNetPanzerGame()
{
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
    Timer wait;

    LoadingView::show();
    // refresh the view in each append
    LoadingView::append( "Launching Server ..." );
    try
    {
        MessageRouter::initialize(true);
        
        NetworkServer::hostSession();

        if(    (bool) gameconfig->publicServer
            && (const std::string&) gameconfig->masterservers != "")
        {
            try
            {
                if ( infosocket )
                {
                    delete infosocket;
                    infosocket = 0;
                }
                infosocket = new InfoSocket(gameconfig->serverport);

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
        LoadingView::append( "SERVER LAUNCH FAILED" );
        LoadingView::append(e.what());
        wait.changePeriod( 4 );
        while( !wait.count() );

        LoadingView::loadError();
        return;
    }
    
    // refresh views
    LoadingView::update( "Launching Server ... (100%) " );
    graphicsLoop();

//    GameControlRulesDaemon::setStateServerInProgress();
    GameControlRulesDaemon::setStateServerLoadingMap();
    NetworkState::setNetworkStatus( _network_state_server );
}

void PlayerGameManager::quitGame()
{
    if ( infosocket ) {
        delete infosocket;
        infosocket = 0;
    }
    if ( heartbeat ) {
        delete heartbeat;
        heartbeat = 0;
    }
}

//-----------------------------------------------------------------
void PlayerGameManager::joinMultiPlayerGame()
{
    GameManager::setNetPanzerGameOptions();
    //global_engine_state->game_manager->reinitializeGameLogic();
    NetworkState::setNetworkStatus( _network_state_client );

//    MessageRouter::initialize(false);
//    NetworkClient::joinServer(gameconfig->serverConnect);
//    LoadingView::show();

//    ClientConnectDaemon::startConnectionProcess();
    GameControlRulesDaemon::setStateClientConnectToServer(gameconfig->serverConnect);
    global_engine_state->sound_manager->playTankIdle();
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

    // handle SDL Events
    if(handleSDLEvents())
    {
        // We got SDL_Quit, lets disconnect nicely.
        GameManager::exitNetPanzer();
        return false;
    }

    return BaseGameManager::mainLoop();
}

//-----------------------------------------------------------------
void PlayerGameManager::processSystemKeys()
{
    if (KeyboardInterface::getKeyState( SDLK_LALT ) ||
            KeyboardInterface::getKeyState( SDLK_RALT ))
    {
        if (KeyboardInterface::getKeyPressed(SDLK_RETURN))
        {
            GameConfig::video_fullscreen = !GameConfig::video_fullscreen;
            GameManager::setVideoMode();
        }
    } // ** LFT_ALT or RGT_ALT pressed

    if (KeyboardInterface::getKeyPressed(SDLK_F3))
    {
        Desktop::toggleVisibility( "DesktopView" );
    }

    if ( Desktop::getVisible("GameView") )
    {
        if (KeyboardInterface::getKeyPressed(SDLK_m))
        {
            Desktop::toggleVisibility( "MiniMapView" );
        }

        if (KeyboardInterface::getKeyPressed(SDLK_TAB) )
        {
            Desktop::toggleVisibility( "RankView" );
        }

        if (KeyboardInterface::getKeyPressed(SDLK_F4))
        {
            Desktop::toggleVisibility( "CodeStatsView" );
        }

        if (KeyboardInterface::getKeyPressed( SDLK_F5 ))
        {
            //  DEBUG VIEW
            Desktop::toggleVisibility( "LibView" );
        }

        if (KeyboardInterface::getKeyPressed(SDLK_F1))
        {
            Desktop::toggleVisibility( "HelpScrollView" );
        }

        if (KeyboardInterface::getKeyPressed(SDLK_ESCAPE))
        {
            if ( Desktop::getVisible("GameView") )
            {
                if (   !Desktop::getVisible("OptionsView")
                    && !Desktop::getVisible("SoundView")
                    && !Desktop::getVisible("ControlsView")
                    && !Desktop::getVisible("InterfaceView")
                    && !Desktop::getVisible("VisualsView") )
                {

                    View *v = Desktop::getView("OptionsView");
                    assert(v != 0);
                    ((OptionsTemplateView *)v)->initButtons();
                    ((OptionsTemplateView *)v)->setAlwaysOnBottom(false);

                    v = Desktop::getView("SoundView");
                    assert(v != 0);
                    ((SoundView *)v)->initButtons();
                    ((OptionsTemplateView *)v)->setAlwaysOnBottom(false);

                    v = Desktop::getView("ControlsView");
                    assert(v != 0);
                    ((ControlsView *)v)->initButtons();
                    ((OptionsTemplateView *)v)->setAlwaysOnBottom(false);

                    v = Desktop::getView("VisualsView");
                    assert(v != 0);
                    ((VisualsView *)v)->initButtons();
                    ((OptionsTemplateView *)v)->setAlwaysOnBottom(false);

                    v = Desktop::getView("InterfaceView");
                    assert(v != 0);
                    ((InterfaceView *)v)->initButtons();
                    ((OptionsTemplateView *)v)->setAlwaysOnBottom(false);

                    Desktop::setVisibility("OptionsView", true);
                    Desktop::setActiveView("OptionsView");
                }
                else
                {
                    View *v = Desktop::getView("OptionsView");
                    assert(v != 0);
                    ((OptionsTemplateView *)v)->setAlwaysOnBottom(true);
                    ((OptionsTemplateView *)v)->setVisible(false);

                    v = Desktop::getView("InterfaceView");
                    assert(v != 0);
                    ((OptionsTemplateView *)v)->setAlwaysOnBottom(true);
                    ((OptionsTemplateView *)v)->setVisible(false);

                    v = Desktop::getView("VisualsView");
                    assert(v != 0);
                    ((OptionsTemplateView *)v)->setAlwaysOnBottom(true);
                    ((OptionsTemplateView *)v)->setVisible(false);

                    v = Desktop::getView("SoundView");
                    assert(v != 0);
                    ((OptionsTemplateView *)v)->setAlwaysOnBottom(true);
                    ((OptionsTemplateView *)v)->setVisible(false);

                    v = Desktop::getView("ControlsView");
                    assert(v != 0);
                    ((OptionsTemplateView *)v)->setAlwaysOnBottom(true);
                    ((OptionsTemplateView *)v)->setVisible(false);
                }
            }
        }
    }
}
