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

#include <exception>

#include "PlayerGameManager.hpp"

#include "Server.hpp"
#include "NetworkServer.hpp"
#include "Client.hpp"
#include "NetworkClient.hpp"
#include "ClientMessageRouter.hpp"
#include "ClientConnectDaemon.hpp"
#include "ServerConnectDaemon.hpp"
#include "ServerMessageRouter.hpp"
#include "NetworkState.hpp"
#include "SystemNetMessage.hpp"
#include "ConnectNetMessage.hpp"

#include "Util/Log.hpp"
#include "MouseInterface.hpp"
#include "KeyboardInterface.hpp"
#include "GameConfig.hpp"
#include "TileInterface.hpp"
#include "TileEngine.hpp"
#include "MapInterface.hpp"
#include "MiniMapInterface.hpp"
#include "PlayerInterface.hpp"
#include "UnitInterface.hpp"
#include "UnitBlackBoard.hpp"
#include "ProjectileInterface.hpp"
#include "PathScheduler.hpp"
#include "WorldViewInterface.hpp"
#include "WorldInputCmdProcessor.hpp"
#include "SpriteSorter.hpp"
#include "Vehicle.hpp"
#include "UnitProfileInterface.hpp"
#include "ObjectiveInterface.hpp"
#include "ConsoleInterface.hpp"
#include "UnitGlobals.hpp"
#include "SelectionBoxSprite.hpp"
#include "MapsManager.hpp"
#include "PowerUpInterface.hpp"
#include "ChatInterface.hpp"
#include "Util/Exception.hpp"
#include "Util/FileSystem.hpp"

#include "cMouse.hpp"
#include "2D/Palette.hpp"
#include "Desktop.hpp"
#include "GameView.hpp"
#include "MainMenuView.hpp"
#include "JoinView.hpp"
#include "HostView.hpp"
#include "GetSessionView.hpp"
#include "GetSessionHostView.hpp"
#include "RankView.hpp"
#include "VehicleSelectionView.hpp"
#include "MiniMapView.hpp"
#include "OptionsTemplateView.hpp"
#include "OrderingView.hpp"
#include "SkirmishView.hpp"
#include "HelpView.hpp"
#include "SoundView.hpp"
#include "ControlsView.hpp"
#include "InterfaceView.hpp"
#include "VisualsView.hpp"
#include "LobbyView.hpp"
#include "ProgressView.hpp"
#include "CodeStatsView.hpp"
#include "ViewGlobals.hpp"
#include "LibView.hpp"
#include "HelpScrollView.hpp"
#include "ResignView.hpp"
#include "AreYouSureResignView.hpp"
#include "AreYouSureExitView.hpp"
#include "UnitSelectionView.hpp"
#include "FlagSelectionView.hpp"
#include "UnitColorView.hpp"
#include "HostOptionsView.hpp"
#include "MapSelectionView.hpp"
#include "PlayerNameView.hpp"
#include "GameInfoView.hpp"
#include "ServerListView.hpp"

#include "IPAddressView.hpp"

//** Physics/Particle Stuff
#include "Particle2D.hpp"
#include "ParticleSystem2D.hpp"
#include "ParticleInterface.hpp"
#include "Physics.hpp"
#include "Util/TimerInterface.hpp"
#include "Util/Math.hpp"

#include "System/Sound.hpp"
#include "System/SDLSound.hpp"
#include "System/SDLVideo.hpp"
#include "System/DummySound.hpp"
#include "System/SDLEvents.hpp"
#include "GameManager.hpp"
#include "GameControlRulesDaemon.hpp"

#include "InfoThread.hpp"
#include "HeartbeatThread.hpp"

//** User interface
//#include "UI/Painter.hpp"


PlayerGameManager::PlayerGameManager()
    : sdlVideo(0), heartbeatthread(0), infothread(0)
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
    initFont();
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
        if(gameconfig->enablesound)
            sound = new SDLSound();
    } catch(std::exception& e) {
        LOGGER.warning("Couldn't initialize sound: %s", e.what());
    }

    if(sound == 0)
        sound = new DummySound();

    sound->setSoundVolume(gameconfig->effectsvolume);

    // start some music
    if(gameconfig->enablemusic) {
        sound->playMusic("sound/music/");
        sound->setMusicVolume(gameconfig->musicvolume);
    }
}
//-----------------------------------------------------------------
void PlayerGameManager::initializeInputDevices()
{
    MouseInterface::initialize();
}
//-----------------------------------------------------------------
void PlayerGameManager::initializeWindowSubSystem()
{
    GameManager::loadPalette("netp");

    gameView.init();
    Desktop::add( &gameView );
    Desktop::add(new RankView());
    Desktop::add(new VehicleSelectionView());
    Desktop::add( &miniMapView );
    Desktop::add(new CodeStatsView());
    Desktop::add(new LibView());
    Desktop::add(new HelpScrollView());
    Desktop::add(new GameInfoView());

    lobbyView = new LobbyView();
    lobbyView->init();
    Desktop::add(lobbyView);

    progressView = new ProgressView();    
    progressView->init();
    Desktop::add(progressView);

    GameManager::loadPalette("netpmenu");

    Desktop::add(new MapSelectionView());
    Desktop::add(new MainMenuView());
    Desktop::add(new JoinView());
    Desktop::add(new HostView());
    Desktop::add(new GetSessionView());
    Desktop::add(new GetSessionHostView());
    Desktop::add(new OptionsTemplateView());
    Desktop::add(new OrderingView());
    Desktop::add(new HelpView());
    Desktop::add(new SoundView());
    Desktop::add(new ControlsView());
    Desktop::add(new VisualsView());
    Desktop::add(new InterfaceView());
    Desktop::add(new FlagSelectionView());
    Desktop::add(new HostOptionsView());
    PlayerNameView *playernameview=new PlayerNameView();
    Desktop::add(playernameview);
    Desktop::add(new ResignView());
    Desktop::add(new AreYouSureResignView());
    Desktop::add(new AreYouSureExitView());

    Desktop::add(new IPAddressView());
    Desktop::add(new ServerListView());

    Desktop::setVisibilityAllWindows(false);
    Desktop::setVisibility("MainView", true);

    Desktop::checkResolution(iXY(640,480), iXY(screen->getPix()));
    Desktop::checkViewPositions(screen->getPix());


    //Test for new UI
    //testpanel = new Panels::TestPanel(iXY(30, 60), &fontManager);
}
//-----------------------------------------------------------------
void PlayerGameManager::inputLoop()
{
    if(infothread)
        infothread->lastFrame = now();

    processSystemKeys();

#if 0
    if(showNewPanel && Desktop::getVisible("GameView") && testpanel->contains(mouse.getScreenPos()))
    {
        //Game started, draw interface
        testpanel->processEvents(mouse.getScreenPos(), mouse.getButtonMask(), KMOD_NONE);
    }else
#endif
       Desktop::manage(mouse.getScreenPos().x,
               mouse.getScreenPos().y, mouse.getButtonMask());

    COMMAND_PROCESSOR.updateScrollStatus( mouse.getScreenPos() );
}
//-----------------------------------------------------------------
void PlayerGameManager::graphicsLoop()
{
    screen->lock();

    Desktop::draw(*screen);

#if 0
    //TODO : clean this ugly test :)
    if(showNewPanel && Desktop::getVisible("GameView"))
    {
        UI::Painter painter(Screen->getSurface(), &fontManager);

        //Game started, draw interface
        testpanel->draw(painter);
    }
#endif

    if (Desktop::getVisible("GameView")) {
        ConsoleInterface::update(*screen);
    }

    mouse.draw(*screen);
    MouseInterface::updateCursor();     

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
    delete infothread;
    infothread = 0;
    delete heartbeatthread;
    heartbeatthread = 0;
    BaseGameManager::shutdownNetworkSubSystem();
}

//-----------------------------------------------------------------
void PlayerGameManager::hostMultiPlayerGame()
{
    PlayerID player;
    PlayerState *player_state;
    Timer wait;

    progressView->open();
    progressView->scrollAndUpdateDirect( "Launching Server ..." );
    try {
        SERVER->hostSession();

        if((bool) gameconfig->publicServer &&
                (const std::string&) gameconfig->masterservers != "") {
            try {
                delete infothread;
                delete heartbeatthread;
                infothread = new InfoThread(gameconfig->serverport);
                heartbeatthread = new HeartbeatThread();
            } catch(std::exception& e) {
                LOGGER.warning("heartbeats disabled: %s", e.what());
                delete infothread;
                infothread = 0;
                delete heartbeatthread;
                heartbeatthread = 0;
            }
        }
    } catch(std::exception& e) {
        progressView->scrollAndUpdateDirect( "SERVER LAUNCH FAILED" );
        progressView->scrollAndUpdateDirect(e.what());
        wait.changePeriod( 4 );
        while( !wait.count() );

        progressView->toggleMainMenu();
        return;
    }

    progressView->updateDirect( "Launching Server ... (100%) " );

    GameControlRulesDaemon::setStateServerInProgress();
    NetworkState::setNetworkStatus( _network_state_server );

    progressView->scrollAndUpdateDirect( "Loading Game Data ..." );

    gameconfig->map = MapsManager::getNextMap("");
    const char* mapname = gameconfig->map.c_str();

    try {
        GameManager::startGameMapLoad(mapname, 20);
    } catch(std::exception& e) {
        LOGGER.warning("Error while loading map '%s':", mapname);
        LOGGER.warning(e.what());
        progressView->toggleMainMenu();
        return;
    }

    int percent_complete;
    char strbuf[256];

    ObjectiveInterface::resetLogic();

    while( GameManager::gameMapLoad( &percent_complete ) == true ) {
        sprintf( strbuf, "Loading Game Data ... (%d%%)", percent_complete);
        progressView->updateDirect( strbuf );
    }

    sprintf( strbuf, "Loading Game Data ... (%d%%)", percent_complete);
    progressView->updateDirect( strbuf );


    progressView->scrollAndUpdateDirect( "Initializing Game Logic ..." );
    GameManager::reinitializeGameLogic();
    progressView->updateDirect( "Initializing Game Logic ... (100%) " );


    progressView->scrollAndUpdateDirect( "Spawning Player ..." );
    player_state = PlayerInterface::allocateLoopBackPlayer();
    const char* playername = gameconfig->playername.c_str();
    player_state->setName(playername);
    player_state->setFlag( (unsigned char) gameconfig->playerflag );
    player = PlayerInterface::getLocalPlayerID();
    GameManager::spawnPlayer( player );
    progressView->updateDirect( "Spawning Player ... (100%)" );

    wait.changePeriod( 3 );
    while( !wait.count() );

    GameManager::startGameTimer();

    progressView->close();

    // Set the palette to the game palette.
    GameManager::loadPalette( "netp");

    GameManager::setNetPanzerGameOptions();

    // Need to open at beginning of game until we are saving status of things.
    // when last played.
    Desktop::setVisibility("GameInfoView", true);
    Desktop::setVisibility("MiniMapView", true);
    Desktop::setVisibility("GameView", true);
    Desktop::setActiveView("GameView");
}

void PlayerGameManager::quitGame()
{
    delete infothread;
    infothread = 0;
    delete heartbeatthread;    
    heartbeatthread = 0;
}

//-----------------------------------------------------------------
void PlayerGameManager::joinMultiPlayerGame()
{
    GameManager::setNetPanzerGameOptions();
    //reinitializeGameLogic();
    NetworkState::setNetworkStatus( _network_state_client );

    CLIENT->joinServer(gameconfig->serverConnect);

    ClientConnectDaemon::startConnectionProcess();
    sound->playTankIdle();
}

bool PlayerGameManager::mainLoop()
{
    // this is a bit ugly...
    static bool firstrun = true;
    if(firstrun) {
        firstrun = false;
        if(gameconfig->quickConnect == true) {
            Desktop::setVisibilityAllWindows(false);
            Desktop::setVisibility("LobbyView", true);
            launchMultiPlayerGame();
        }
    }

    // handle SDL Events
    if(handleSDLEvents())
        return false;

    return BaseGameManager::mainLoop();
}

//-----------------------------------------------------------------
void PlayerGameManager::processSystemKeys()
{
    if (Desktop::getVisible("GameView")) {
        if (KeyboardInterface::getKeyPressed( SDLK_F12 )){
            showNewPanel = !showNewPanel;
        }
        
        if (KeyboardInterface::getKeyPressed( SDLK_F5 )) {
            //  DEBUG VIEW
            Desktop::toggleVisibility( "LibView" );
        }
    }

    if (KeyboardInterface::getKeyState( SDLK_LALT ) ||
            KeyboardInterface::getKeyState( SDLK_RALT )) {
        if (KeyboardInterface::getKeyPressed(SDLK_RETURN)) {
            gameconfig->fullscreen.toggle();
            GameManager::setVideoMode();
        }
    } // ** LFT_ALT or RGT_ALT pressed

    if (Desktop::getView("GameView")->getVisible()) {
        if (KeyboardInterface::getKeyPressed(SDLK_m)) {
            Desktop::toggleVisibility( "MiniMapView" );
        }
        if (KeyboardInterface::getKeyPressed(SDLK_TAB) ) {
            Desktop::toggleVisibility( "RankView" );
        }
        if (KeyboardInterface::getKeyPressed(SDLK_F3)) {
            Desktop::toggleVisibility( "DesktopView" );
        }
        if (KeyboardInterface::getKeyPressed(SDLK_F4)) {
            Desktop::toggleVisibility( "CodeStatsView" );
        }
        if (KeyboardInterface::getKeyPressed(SDLK_F1)) {
            Desktop::toggleVisibility( "HelpScrollView" );
        }

        if (KeyboardInterface::getKeyPressed(SDLK_ESCAPE)) {
            if (Desktop::getView("GameView")->getVisible()) {
                if (!Desktop::getView("OptionsView")->getVisible() &&
                        !Desktop::getView("SoundView")->getVisible() &&
                        !Desktop::getView("ControlsView")->getVisible() &&
                        !Desktop::getView("InterfaceView")->getVisible() &&
                        !Desktop::getView("VisualsView")->getVisible()) {
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
                } else {
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
