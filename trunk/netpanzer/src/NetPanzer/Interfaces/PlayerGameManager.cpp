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
#include "PlayerGameManager.hpp"

// ** PObject netPanzer Network Includes
#include "Server.hpp"
#include "Client.hpp"
#include "ClientMessageRouter.hpp"
#include "ClientConnectDaemon.hpp"
#include "ServerConnectDaemon.hpp"
#include "ServerMessageRouter.hpp"
#include "NetworkState.hpp"
#include "SystemNetMessage.hpp"
#include "ConnectNetMessage.hpp"

#include "Log.hpp"
#include "MouseInterface.hpp"
#include "KeyboardInterface.hpp"
#include "ScreenSurface.hpp"
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
#include "Gdatstct.hpp"
#include "MapsManager.hpp"
#include "PowerUpInterface.hpp"
#include "ChatInterface.hpp"
#include "Exception.hpp"
#include "FileSystem.hpp"

// ** GVS Includes
#include "cMouse.hpp"
#include "Palette.hpp"
#include "Desktop.hpp"
#include "GameView.hpp"
#include "MainMenuView.hpp"
#include "JoinView.hpp"
#include "HostView.hpp"
#include "GetSessionView.hpp"
#include "GetSessionHostView.hpp"
#include "ChatView.hpp"
#include "WinnerMesgView.hpp"
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
#include "GameToolbarView.hpp"

//winsock hack
#include "IPAddressView.hpp"

//** Physics/Particle Stuff
#include "Particle2D.hpp"
#include "ParticleSystem2D.hpp"
#include "ParticleInterface.hpp"
#include "Physics.hpp"
#include "TimerInterface.hpp"
#include "Math.hpp"

#include "Sound.hpp"
#include "SDLSound.hpp"
#include "DummySound.hpp"
#include "UIDraw.hpp"
#include "SDLDraw.hpp"
#include "GameManager.hpp"
#include "GameControlRulesDaemon.hpp"

//-----------------------------------------------------------------
void PlayerGameManager::initializeVideoSubSystem()
{
    LOGGER.info("Initializing video mode");
    Screen = new SDLDraw();
    GameManager::setVideoMode();

    lobbyView = new LobbyView();
    progressView = new ProgressView();
}
//-----------------------------------------------------------------
void PlayerGameManager::shutdownVideoSubSystem()
{
    delete Screen;
    Screen = 0;
}
//-----------------------------------------------------------------
void PlayerGameManager::initializeSoundSubSystem()
{
    LOGGER.info("Initialize sound system.");
    try {
        sound = new SDLSound();
    } catch(Exception e) {
        LOGGER.warning("Couldn't initialize sound: %s", e.what());
        sound = new DummySound();
    }

    // start some music
    sound->playMusic("sound/music/");
}
//-----------------------------------------------------------------
void PlayerGameManager::initializeInputDevices()
{
    MouseInterface::initialize();
}
//-----------------------------------------------------------------
void PlayerGameManager::initializeWindowSubSystem()
{
    GameManager::loadPalette("wads/netp.act");

    initFont();

    gameView.init();
    Desktop::add( &gameView );
    Desktop::add(new RankView());
    Desktop::add(new VehicleSelectionView());
    Desktop::add(new WinnerMesgView());
    Desktop::add( &miniMapView );
    Desktop::add(new CodeStatsView());
    Desktop::add(new LibView());
    //Desktop::add(new DesktopView());
    //Desktop::add(new UnitColorView());
    Desktop::add(new HelpScrollView());
    Desktop::add(new GameToolbarView());
    Desktop::add(new GameInfoView());

    lobbyView->init();
    Desktop::add(lobbyView);
    progressView->init();
    Desktop::add(progressView);

    GameManager::loadPalette("wads/netpmenu.act");

    //chatView.init();
    //Desktop::add(&chatView);

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
    Desktop::add(new PlayerNameView());
    Desktop::add(new ResignView());
    Desktop::add(new AreYouSureResignView());
    Desktop::add(new AreYouSureExitView());

    //winsock hack
    Desktop::add(new IPAddressView());

    Desktop::setVisibilityAllWindows(false);
    Desktop::setVisibility("MainView", true);

    Desktop::checkResolution(iXY(640,480), iXY(screen->getPix()));
    Desktop::checkViewPositions(screen->getPix());
}
//-----------------------------------------------------------------
void PlayerGameManager::inputLoop()
{
    processSystemKeys();

    Desktop::manage(mouse.getScreenPos().x, mouse.getScreenPos().y, mouse.getButtonMask() );

    COMMAND_PROCESSOR.updateScrollStatus( mouse.getScreenPos() );
}
//-----------------------------------------------------------------
void PlayerGameManager::graphicsLoop()
{
    screen->lock();

    Desktop::draw(*screen);
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
void PlayerGameManager::hostMultiPlayerGame()
{
    PlayerID player;
    PlayerState *player_state;
    Timer wait;

    progressView->open();

    //InitStreamServer(gapp.hwndApp);

    progressView->scrollAndUpdateDirect( "Launching Server ..." );
    try {
        SERVER->hostSession();
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
    CLIENT->openSession();

    progressView->scrollAndUpdateDirect( "Loading Game Data ..." );

    const char* mapname = gameconfig->map.c_str();
    MapsManager::setCycleStartMap(mapname);

    int result_code;
    GameManager::startGameMapLoad(mapname, 20, &result_code);

    if( result_code == _mapload_result_no_wad_file ) {
        progressView->scrollAndUpdateDirect( "MAP TILE SET NOT FOUND!" );
        progressView->scrollAndUpdateDirect( "please download the appropriate tileset" );
        progressView->scrollAndUpdateDirect( "from www.pyrosoftgames.com" );
        wait.changePeriod( 12 );
        while( !wait.count() );

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
    GameManager::loadPalette( "wads/netp.act" );

    GameManager::setNetPanzerGameOptions();

    // Need to open at beginning of game until we are saving status of things.
    // when last played.
    Desktop::setVisibility("GameToolbarView", true);
    Desktop::setVisibility("GameInfoView", true);
    Desktop::setVisibility("MiniMapView", true);
    Desktop::setVisibility("GameView", true);
    Desktop::setActiveView("GameView");
}
//-----------------------------------------------------------------
void PlayerGameManager::joinMultiPlayerGame()
{
    GameManager::setNetPanzerGameOptions();
    //reinitializeGameLogic();
    NetworkState::setNetworkStatus( _network_state_client );

    CLIENT->joinSession(IPAddressView::szServer.getString());

    ClientConnectDaemon::startConnectionProcess();
    sound->playTankIdle();
}
//-----------------------------------------------------------------
void PlayerGameManager::processSystemKeys()
{
    if (Desktop::getVisible("GameView")) {
        if (KeyboardInterface::getKeyPressed( SDLK_F5 )) {
            //  DEBUG VIEW
            Desktop::toggleVisibility( "LibView" );
        }

        if (KeyboardInterface::getKeyPressed( SDLK_F3 )) {
            Desktop::toggleVisibility( "UnitColorView" );
        }

        // Remove all selection.
        if (KeyboardInterface::getKeyPressed(SDLK_ESCAPE)) {
            COMMAND_PROCESSOR.closeSelectionBox();
            MiniMapInterface::deselectUnits();
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
        if (KeyboardInterface::getKeyPressed(SDLK_F8)) {
            Desktop::toggleVisibility( "MiniMapView" );
        }
        if (KeyboardInterface::getKeyPressed(SDLK_F7)) {
            Desktop::toggleVisibility( "ChatView" );
        }
        if (KeyboardInterface::getKeyPressed(SDLK_F6)) {
            Desktop::toggleVisibility( "RankView" );
        }
        if (KeyboardInterface::getKeyPressed(SDLK_F3)) {
            Desktop::toggleVisibility( "DesktopView" );
        }
        if (KeyboardInterface::getKeyPressed(SDLK_TAB)) {
            Desktop::toggleVisibility( "GameToolbarView" );
        }
        if (KeyboardInterface::getKeyPressed(SDLK_F4)) {
            Desktop::toggleVisibility( "CodeStatsView" );
        }
        if (KeyboardInterface::getKeyPressed(SDLK_F1)) {
            Desktop::toggleVisibility( "HelpScrollView" );
        }

        if (KeyboardInterface::getKeyPressed(SDLK_F2)) {
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
