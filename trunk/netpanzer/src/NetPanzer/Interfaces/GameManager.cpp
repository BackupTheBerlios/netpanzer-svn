/*
Copyright (C) 1998 Pyrosoft Inc. (www.pyrosoftgames.com), Matthew Bogue

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
#include "GameManager.hpp"  

#include <stdio.h>
#include <fcntl.h>
#ifdef WIN32
#include <io.h>
#include <conio.h>
#include <windows.h>
#endif

// ** netPanzer Core Includes
#include "gapp.hpp"
#include "netPanzerGlobals.h"

// ** Direct X Includes
#ifdef WIN32
#include "DirectDrawGlobals.hpp"
#include "DirectPlay.h"
#include "DirectInput.hpp"
#include "DSound.hpp"
#endif
#ifdef UNIX
#include "UILib/SDL/SDLSound.hpp"
#endif

#ifdef WIN32
#include "NetworkServerWinSock.hpp"
#include "NetworkClientWinSock.hpp"
//#include "NetworkServerDPlay.hpp"
//#include "NetworkClientDPlay.hpp"
#endif
#ifdef UNIX
#include "NetworkServerUnix.hpp"
#include "NetworkClientUnix.hpp"
#endif

#include "UILib/UIDraw.hpp"
#include "UILib/Sound.hpp"

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
#include "KeyBinder.hpp"
#include "KeyActionEnum.hpp"
#include "KeyScanCodeDefs.h"
#include "DDHardSurface.hpp"
#include "GameConfig.hpp"
#include "TileInterface.hpp"
#include "TileEngine.hpp"
#include "MapInterface.hpp"
#include "MiniMapInterface.hpp"
#include "StaticsInterface.hpp"
#include "PlayerInterface.hpp"
#include "UnitInterface.hpp"
#include "UnitBlackBoard.hpp"
#include "ProjectileInterface.hpp"
#include "StaticsInterface.hpp"
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

// ** GVS Includes
#include "codewiz.hpp"
#include "cMouse.hpp"
#include "loadPics.hpp"
#include "Palette.hpp"
#include "Desktop.hpp"
#include "GameView.hpp"
#include "MainMenuView.hpp"
#include "JoinView.hpp"
#include "HostView.hpp"
#include "GetSessionView.hpp"
#include "GetConnectionTypeView.hpp"
#include "GetSessionHostView.hpp"
#include "ChatView.hpp"
#include "WinnerMesgView.hpp"
#include "RankView.hpp"
#include "VehicleSelectionView.hpp"
#include "ControlPaletteView.hpp"
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
#include "DirectPlayErrorExceptionView.hpp"
#include "UnitSelectionView.hpp"
#include "FlagSelectionView.hpp"
#include "MiniMapOptionsView.hpp"
#include "UnitColorView.hpp"
#include "HostOptionsView.hpp"
#include "MapSelectionView.hpp"
#include "PlayerNameView.hpp"
#include "GameInfoView.hpp"
#include "GameToolbarView.hpp"

//winsock hack
#include "IPAddressView.h"

//** Physics/Particle Stuff 
#include "Particle2D.hpp"
#include "ParticleSystem2D.hpp"
#include "ParticleInterface.hpp"
#include "Physics.hpp"
#include "TimerInterface.hpp"
#include "FontSystem2D.hpp"
#include "Math.hpp"


#define _MAX_INITIALIZE_PROCS (10)
#define _MAX_DEDICATED_INITIALIZE_PROCS (8)


char GameManager::map_path[256];

time_t GameManager::game_start_time = 0;
time_t GameManager::game_elapsed_time_offset = 0;

bool GameManager::display_frame_rate_flag = false;
bool GameManager::display_network_info_flag;

int GameManager::execution_mode;

unsigned char GameManager::game_state  = _game_state_idle;
PointXYi GameManager::previous_video_mode_res;
PointXYi GameManager::current_video_mode_res;
BYTE     GameManager::current_mode_flags;


static Surface hostLoadSurface;

// ******************************************************************
void GameManager::initializeVideoSubSystem()
{									     
   	LOG( ( "Initializing Direct Draw" ) );
	if( ( Screen->initialize() ) == false ) 
	   	throw Exception("DDraw.Intialize failed.");  

	LOG( ( "Setting Default Video Sub-system" ) );
  
	current_video_mode_res = PointXYi(640,480); 

#ifdef _DEBUG
	//current_mode_flags = VIDEO_MODE_WINDOWED;
	current_mode_flags = VIDEO_MODE_DOUBLE_BUFFER;
	//current_mode_flags = VIDEO_MODE_TRIPLE_BUFFER;
#else
	//current_mode_flags = VIDEO_MODE_WINDOWED;
	current_mode_flags = VIDEO_MODE_DOUBLE_BUFFER;
	//current_mode_flags = VIDEO_MODE_TRIPLE_BUFFER;
#endif

	setVideoMode(current_video_mode_res);
	loadPalette("wads/netp.act");
}

void GameManager::shutdownVideoSubSystem()
{
	Screen->shutdown();
}

// ******************************************************************

void GameManager::initializeSoundSubSystem()
{
#ifdef WIN32
    sound = new DirectSound();	
#endif
#ifdef USE_SDL
	LOG( ("Initialize sound system.") );
	sound = new SDLSound();
#endif
    if (!sound || !sound->initialize())
    {
		LOG( ( "Failed to initialize the Sound Sub-system" ) );
		delete sound;
		sound = 0;
    } 
}

// ******************************************************************

void GameManager::shutdownSoundSubSystem()
{
	if(sound) {
		LOG( ("shutdown sound system.") );
		sound->shutdown();
		delete sound;
		sound = 0;
	}
}

// ******************************************************************

void GameManager::initializeWindowSubSystem()
{
	LOG(("Initializing Game Viewing System"));

	// Use this when needing to create colorfilters.
	loadPalette("wads/netp.act"); 

	initFont();  
	loadPics();

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
	Desktop::add(new MiniMapOptionsView());
	Desktop::add(new GameToolbarView());
	Desktop::add(new GameInfoView());

	lobbyView.init();
	Desktop::add(&lobbyView);
	progressView.init();
	Desktop::add(&progressView);

	//chatView.init();
	//Desktop::add(&chatView);

	loadPalette( "wads/netpmenu.act" );

	Desktop::add(new MapSelectionView());
	Desktop::add(new MainMenuView());
	Desktop::add(new JoinView());
	Desktop::add(new HostView());
	Desktop::add(new GetSessionView());
	Desktop::add(new GetConnectionTypeView());
	Desktop::add(new GetSessionHostView());
	Desktop::add(new OptionsTemplateView());
	Desktop::add(new OrderingView());
	Desktop::add(new HelpView());
	Desktop::add(new SoundView());
	Desktop::add(new ControlsView());
	Desktop::add(new VisualsView());
	Desktop::add(new InterfaceView());
	Desktop::add(new DirectPlayErrorExceptionView());
	//Desktop::add(new UnitSelectionView());
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
}

// ******************************************************************
void GameManager::setVideoMode( PointXYi mode_res )
{
   	if(!Screen->isDisplayModeAvailable( mode_res.x, mode_res.y, 8 ))
		throw Exception("desired Video mode not available.");
		
	previous_video_mode_res = current_video_mode_res;
	current_video_mode_res = mode_res;

	if (!Screen->setVideoMode(current_video_mode_res.x, current_video_mode_res.y,
							8, current_mode_flags))
		throw Exception("failed to set video mode.");
		
	WorldViewInterface::setCameraSize( current_video_mode_res.x, current_video_mode_res.y );
	FRAME_BUFFER.create(current_video_mode_res.x, current_video_mode_res.y, current_video_mode_res.x, 1 );
	screen.createNoAlloc(current_video_mode_res);   
	gameView.setSize( current_video_mode_res );
}

// ******************************************************************

void GameManager::restorePreviousVideoMode()
{
  	setVideoMode( previous_video_mode_res );
}

void GameManager::drawTextCenteredOnScreen(const char *string, PIX color)
 {
  FRAME_BUFFER.lock();
  screen.lock(FRAME_BUFFER.mem);
  screen.fill(0);
  screen.bltStringCenter(string, color);
  FRAME_BUFFER.unlock();
  screen.unlock();
  Screen->copyDoubleBufferandFlip(); 
 }

// ******************************************************************

void GameManager::increaseDisplayResolution()
 {
  PointXYi new_mode;
  
  drawTextCenteredOnScreen("Changing Resolution", Color::white);

  GameConfig::setNextGameScreenResolution();
  
  drawTextCenteredOnScreen("Changing Resolution", Color::white);

  new_mode = GameConfig::getGameScreenResolutionSize();

  setVideoMode(new_mode);
    
  previous_video_mode_res = current_video_mode_res;
  current_video_mode_res = new_mode;

  WorldViewInterface::setCameraSize( current_video_mode_res.x, current_video_mode_res.y );
  FRAME_BUFFER.create(current_video_mode_res.x, current_video_mode_res.y, current_video_mode_res.x, 1 );
  screen.createNoAlloc(current_video_mode_res);   
  gameView.setSize( current_video_mode_res );
  Desktop::checkViewPositions();
  ConsoleInterface::setToSurfaceSize( current_video_mode_res );

  loadPalette( "wads/netp.act" );
	
  ConsoleInterface::postMessage( "Screen Resolution :  %d  x  %d", current_video_mode_res.x, current_video_mode_res.y );
}

// ******************************************************************

void GameManager::decreaseDisplayResolution()
 {
  PointXYi new_mode;
  
  drawTextCenteredOnScreen("Changing Resolution", Color::white);

  GameConfig::setPreviousGameScreenResolution();
  
  drawTextCenteredOnScreen("Changing Resolution", Color::white);

  new_mode = GameConfig::getGameScreenResolutionSize();

  setVideoMode(new_mode);
    
  previous_video_mode_res = current_video_mode_res;
  current_video_mode_res = new_mode;

  WorldViewInterface::setCameraSize( current_video_mode_res.x, current_video_mode_res.y );
  FRAME_BUFFER.create(current_video_mode_res.x, current_video_mode_res.y, current_video_mode_res.x, 1 );
  screen.createNoAlloc(current_video_mode_res);   
  gameView.setSize( current_video_mode_res );
  Desktop::checkViewPositions();
  ConsoleInterface::setToSurfaceSize( current_video_mode_res );    
  
  loadPalette("wads/netp.act"); 

  ConsoleInterface::postMessage( "Screen Resolution :  %d  x  %d", current_video_mode_res.x, current_video_mode_res.y );
}

// ******************************************************************

void GameManager::loadPalette(char *palette_path)
 {
	Palette::init(palette_path);
	
	Palette pal;

//	Screen->palette.loadLibPalette(pal, Screen->lpFrontBuffer);
	Screen->setPalette(pal.color);
 }

// ******************************************************************
void increaseBrightness(const char *filename)
{
	GameConfig::setScreenBrightness(GameConfig::getScreenBrightness() + 0.1f);

	Palette::setBrightnessAbsolute(GameConfig::getScreenBrightness());

	Palette pal;

//	Screen->palette.loadLibPalette(pal, Screen->lpFrontBuffer);
	Screen->setPalette(pal.color);
	ConsoleInterface::postMessage( "Increasing Brightness");

} // end increaseBrightness

// ******************************************************************
void decreaseBrightness(const char *filename)
{
	GameConfig::setScreenBrightness(GameConfig::getScreenBrightness() - 0.1f);

	Palette::setBrightnessAbsolute(GameConfig::getScreenBrightness());

	Palette pal;

//	Screen->palette.loadLibPalette(pal, Screen->lpFrontBuffer);
	Screen->setPalette(pal.color);
	ConsoleInterface::postMessage( "Decreasing Brightness");

} // end increaseBrightness


// ******************************************************************
void GameManager::initializeInputDevices()
{
#ifdef WIN32
   	LOG( ( "Initializing Direct Input\n" ) );
	if ( DirectInput::initialize() == false )
		throw Exception("DirectInput failed to intialize."); 
	 
	LOG( ("Initializing Direct Keyboard Input") );
  	// XXX commented out, since the main window isn't hidden as intended... I
	// dunno why. If a key is pressed in the main window, then we need a defined
	// DirectInput Handler.
  	if( execution_mode == _execution_mode_loop_back_server)
	{
		if ( DirectInput::initializeDirectKeyboard() == false )
			throw Exception("failed to initialize DirectKeyboard.");			
	}
#endif

	setupKeyboardBindings();
  
	MouseInterface::initialize();
	mouse.setPointer(&mouseArrow);
}

void GameManager::shutdownInputDevices()
{
#ifdef WIN32
  	DirectInput::shutdown();
#endif
}

// ******************************************************************
void GameManager::initializeGameObjects()
{
  	GameConfig::initialize();
	MapsManager::initialize();
	MapsManager::scanMaps();
  
	Physics::init();
	Math::init();
	Weapon::init();

	ConsoleInterface::initialize(25);
	PowerUpInterface::initialize();
}

// ******************************************************************

void GameManager::shutdownGameObjects()
{
}

// ******************************************************************
void GameManager::initializeDedicatedConsole()
{
#ifdef MSVC
	ShowWindow(gapp.hwndApp, SW_HIDE);
	if( AllocConsole() == 0 )
		throw Exception("couldn't allocate a console.");
   
	freopen( "CONOUT$", "a+t", stdout );
	freopen( "CONIN$", "a+t", stdin );
#endif
  
	ConsoleInterface::setStdoutPipe(true);
}

// ******************************************************************
void GameManager::shutdownDedicatedConsole()
{
#ifdef MSVC
  	FreeConsole();
#endif
}

// ******************************************************************
void GameManager::initializeNetworkSubSystem()
{
#ifdef WIN32
  	SERVER = new NetworkServerWinSock();
	CLIENT = new NetworkClientWinSock();
#endif
#ifdef UNIX
	SERVER = new NetworkServerUnix();
	CLIENT = new NetworkClientUnix();
#endif

	ServerMessageRouter::initialize();
	ClientMessageRouter::initialize();
  
	ServerConnectDaemon::initialize( GameConfig::GetNumberPlayers() );
	
	NetworkState::setNetworkStatus( _network_state_server );
	NetworkState::resetNetworkStats();
	
	SetPacketFunction( EnqueueIncomingPacket );
}

// ******************************************************************
void GameManager::shutdownNetworkSubSystem()
{
	if(SERVER) {
		SERVER->closeSession();
		ServerMessageRouter::cleanUp();
		delete SERVER;
		SERVER = 0;
	}
	if(CLIENT) {
		CLIENT->closeSession();
		ClientMessageRouter::cleanUp();
		delete CLIENT;
		CLIENT = 0;
	}
}

// ******************************************************************
void GameManager::initializeGameLogic()
{
  	PlayerInterface::initialize( GameConfig::GetNumberPlayers(), 
                               GameConfig::GetNumberInitialUnits() );
	UnitBlackBoard::initializeBlackBoard();
	UnitInterface::initialize( GameConfig::GetUnitsPerPlayer() );
	PathScheduler::initialize();
	PowerUpInterface::resetLogic(); 
}

// ******************************************************************
void GameManager::reinitializeGameLogic()
{
   	shutdownGameLogic();
	initializeGameLogic();
}

// ******************************************************************
bool GameManager::resetGameLogic()
{
  	PlayerInterface::reset();
	UnitInterface::reset();
	UnitBlackBoard::initializeBlackBoard();
	PathScheduler::initialize(); 
	PowerUpInterface::resetLogic();
	ProjectileInterface::resetLogic();
	startGameTimer();
	return true;
}

// ******************************************************************
void GameManager::shutdownGameLogic()
{
  	PlayerInterface::cleanUp();
	UnitInterface::cleanUp();
	PathScheduler::cleanUp();
}

void GameManager::shutdownParticleSystems()
{
  	ParticleSystem2D::removeAll();
	Particle2D::removeAll();
}

// ******************************************************************

void GameManager::setupKeyboardBindings()
{
  	KeyBinder::staticInitialize();
	KEY_BINDER.initialize( 256 );
  
	KEY_BINDER.bindAction( _action_mini_map, "MiniMapView", SDLK_F8 );
	KEY_BINDER.lockKey( SDLK_F8 );

	// UNSUPPORTED VIEW 
	KEY_BINDER.bindAction( _action_chat_view, "ChatView", SDLK_F7 );
	KEY_BINDER.lockKey( SDLK_F7 );

	KEY_BINDER.bindAction( _action_rank_view, "RankView", SDLK_F6 );
	KEY_BINDER.lockKey( SDLK_F6 );
}

void GameManager::processSystemKeys()
{
	unsigned char scan_code;

	if (Desktop::getVisible("GameView"))
	{
		
        if (KeyboardInterface::getKeyPressed( SDLK_F5 ))
		{
			//  DEBUG VIEW 
			Desktop::toggleVisibility( "LibView" );    
		} 
        
		if (KeyboardInterface::getKeyPressed( SDLK_F3 ))
		{
			Desktop::toggleVisibility( "UnitColorView" );    
		} 

#if 0 // XXX need another key here, TILDE is not good, because on some keybaords
      // (german ones) it's a composed char)
		// Toggle unit damage bars.
		if (KeyboardInterface::getKeyPressed(SDLK_TILDE))
		{
			GameConfig::toggleDrawUnitDamage();
		} 
#endif

		// Remove all selection.
		if (KeyboardInterface::getKeyPressed(SDLK_ESCAPE))
		{
	        COMMAND_PROCESSOR.closeSelectionBox();
			MiniMapInterface::deselectUnits();
		} 

	}

	if (	KeyboardInterface::getKeyState( SDLK_LALT ) ||
			KeyboardInterface::getKeyState( SDLK_RALT ))
	{
		if ( KeyboardInterface::getKeyPressed( SDLK_MINUS) )
		{
			if (!Desktop::getVisible("GameView"))
			{
				decreaseBrightness("wads/netpmenu.act");
			}
			else
			{
				decreaseBrightness("wads/netp.act");
			}
		} 

		if (KeyboardInterface::getKeyPressed(SDLK_EQUALS))
		{
			if (!Desktop::getVisible("GameView"))
			{
				increaseBrightness("wads/netpmenu.act");
			}
			else
			{
				increaseBrightness("wads/netp.act");
			}
		} 

       if( (KeyboardInterface::getKeyState( SDLK_LCTRL) ||
			KeyboardInterface::getKeyState( SDLK_RCTRL) )   )
        {
         
		  // DEBUG VIEW 
		  if ( KeyboardInterface::getKeyPressed( SDLK_f )  )
          { display_frame_rate_flag = !display_frame_rate_flag; }
          
          // DEBUG VIEW 
		  if ( KeyboardInterface::getKeyPressed( SDLK_n )  )
          { display_network_info_flag = !display_network_info_flag; }
         
        } 

	} // ** LFT_ALT or RGT_ALT pressed

	if (Desktop::getView("GameView")->getVisible())
	{
		scan_code = KEY_BINDER.getActionKey( _action_mini_map );

		if (KeyboardInterface::getKeyPressed( scan_code ))
		{
			Desktop::toggleVisibility( "MiniMapView" );
		}

		scan_code = KEY_BINDER.getActionKey( _action_chat_view );

		if (KeyboardInterface::getKeyPressed( scan_code ))
		{
			Desktop::toggleVisibility( "ChatView" );
		}

		scan_code = KEY_BINDER.getActionKey( _action_rank_view );

		if (KeyboardInterface::getKeyPressed( scan_code ))
		{
			Desktop::toggleVisibility( "RankView" );
		} 

        if (KeyboardInterface::getKeyPressed( SDLK_F3 ))
		{
			Desktop::toggleVisibility( "DesktopView" );    
		} 

		if (KeyboardInterface::getKeyPressed( SDLK_TAB ))
		{
			Desktop::toggleVisibility( "GameToolbarView" );    
		} 

		
        if (KeyboardInterface::getKeyPressed( SDLK_F4 ))
		{
			Desktop::toggleVisibility( "CodeStatsView" );    
		} 
        
        if (KeyboardInterface::getKeyPressed(SDLK_F1))
		{
			Desktop::toggleVisibility( "HelpScrollView" );    
		}

		if (	KeyboardInterface::getKeyState( SDLK_LALT ) ||
				KeyboardInterface::getKeyState( SDLK_RALT ))
		{

			if (KeyboardInterface::getKeyPressed( SDLK_KP_PLUS ) == true)
			{
				if (Desktop::getView("MainView")->getVisible() == false) 
				{
					increaseDisplayResolution();
				}
			}

			if (KeyboardInterface::getKeyPressed( SDLK_KP_MINUS ) == true)
			{
				if (Desktop::getView("MainView")->getVisible() == false) 
				{
					decreaseDisplayResolution();
				}  
			}
		}

		if (KeyboardInterface::getKeyPressed(SDLK_F2))
		{
			if (Desktop::getView("GameView")->getVisible())
			{
				if (!Desktop::getView("OptionsView")->getVisible() &&
					!Desktop::getView("SoundView")->getVisible() &&
					!Desktop::getView("ControlsView")->getVisible() &&
					!Desktop::getView("InterfaceView")->getVisible() &&
					!Desktop::getView("VisualsView")->getVisible())
				{
					View *v = Desktop::getView("OptionsView");
					
					if (v != 0)
					{
						((OptionsTemplateView *)v)->initButtons();
						((OptionsTemplateView *)v)->setAlwaysOnBottom(false);
					} else
					{
						assert(false);
					}

					v = Desktop::getView("SoundView");
					if (v != 0)
					{
						((SoundView *)v)->initButtons();
						((OptionsTemplateView *)v)->setAlwaysOnBottom(false);
					} else
					{
						assert(false);
					}

					v = Desktop::getView("ControlsView");
					if (v != 0)
					{
						((ControlsView *)v)->initButtons();
						((OptionsTemplateView *)v)->setAlwaysOnBottom(false);
					} else
					{
						assert(false);
					}

					v = Desktop::getView("VisualsView");
					if (v != 0)
					{
						((VisualsView *)v)->initButtons();
						((OptionsTemplateView *)v)->setAlwaysOnBottom(false);
					} else
					{
						assert(false);
					}

					v = Desktop::getView("InterfaceView");
					if (v != 0)
					{
						((InterfaceView *)v)->initButtons();
						((OptionsTemplateView *)v)->setAlwaysOnBottom(false);
					} else
					{
						assert(false);
					}

					Desktop::setVisibility("OptionsView", true);
					Desktop::setActiveView("OptionsView");
				} else
				{
					View *v = Desktop::getView("OptionsView");
					if (v != 0)
					{
						((OptionsTemplateView *)v)->setAlwaysOnBottom(true);
						((OptionsTemplateView *)v)->setVisible(false);
					}
						
					v = Desktop::getView("InterfaceView");
					if (v != 0)
					{
						((OptionsTemplateView *)v)->setAlwaysOnBottom(true);
						((OptionsTemplateView *)v)->setVisible(false);
					}
					
					v = Desktop::getView("VisualsView");
					if (v != 0)
					{
						((OptionsTemplateView *)v)->setAlwaysOnBottom(true);
						((OptionsTemplateView *)v)->setVisible(false);
					}
					
					v = Desktop::getView("SoundView");
					if (v != 0)
					{
						((OptionsTemplateView *)v)->setAlwaysOnBottom(true);
						((OptionsTemplateView *)v)->setVisible(false);
					}
					
					v = Desktop::getView("ControlsView");
					if (v != 0)
					{
						((OptionsTemplateView *)v)->setAlwaysOnBottom(true);
						((OptionsTemplateView *)v)->setVisible(false);
					}
				}
			}
		}
	} // ** if (Desktop::getView("GameView")->getVisible())
}	


// ******************************************************************
bool GameManager::loadGameData()
 {
  UnitProfileInterface::loadUnitProfiles();
  LoadUnitSurfaces();
  UNIT_FLAGS_SURFACE.loadAllBMPInDirectory("pics/flags/netp/");
  
  GameConfig::loadConfigScript();
  return true; 
 }

// ******************************************************************
void GameManager::dedicatedLoadGameData()
{
   	UnitProfileInterface::loadUnitProfiles();
	LoadUnitSurfaces();
	UNIT_FLAGS_SURFACE.loadAllBMPInDirectory("pics/flags/netp/");

	GameConfig::loadConfigScript();
}

// ******************************************************************
bool GameManager::startGameMapLoad( char *map_file_path, unsigned long partitions, int *result_code )
 {
  int check_return_code; 
  check_return_code = MapsManager::checkMapValidity( map_file_path );

  if( check_return_code == _mapfile_not_found )
   {
    *result_code = _mapload_result_no_map_file;
    return false;
   }
  else
   if( check_return_code == _wadfile_not_found )
    {
     *result_code = _mapload_result_no_wad_file;
     return false;
    }
   else
    { *result_code = _mapload_result_success; }
  
  strcpy( map_path, "./maps/" );
  strcat( map_path, map_file_path );
  
  if ( MapInterface::startMapLoad( map_path, true, partitions ) == false )
   { 
    finishGameMapLoad();
    return false;
   }
  
  return true;
 }

// ******************************************************************

bool GameManager::gameMapLoad( int *percent_complete )
 {
  if( MapInterface::loadMap( percent_complete ) == false )
   {
    finishGameMapLoad();
    return false;
   }
 
  return true;
 }

// ******************************************************************

void GameManager::finishGameMapLoad()
 {
  char temp_path[256];

  strcpy( temp_path, map_path );  
  strcat( temp_path, ".opt" );
  ObjectiveInterface::loadObjectiveList( temp_path );

  miniMapView.init();
  ParticleInterface::initParticleSystems();
  
  ParticleInterface::addCloudParticle(GameConfig::getCloudCoverage());
  Physics::wind.setVelocity(GameConfig::getWindSpeed(), 107);
 }
  
// ******************************************************************

void GameManager::dedicatedLoadGameMap( char *map_name )
{  
  strcpy( map_path, "./maps/" );
  strcat( map_path, map_name );
  
  MapInterface::startMapLoad( map_path, false, 0 ); 
  
  strcat( map_path, ".opt" );
  ObjectiveInterface::loadObjectiveList( map_path );
 
  ParticleInterface::initParticleSystems();
  Particle2D::setCreateParticles(false);

  ParticleInterface::addCloudParticle(GameConfig::getCloudCoverage());
  Physics::wind.setVelocity(GameConfig::getWindSpeed(), 107);
}

// ******************************************************************
bool GameManager::initialize( const char *command_line  )
{
  	char work_str[256];
	char *token;
	strcpy( work_str, command_line );

	token = strtok( work_str, " " );
  
	if ( token != 0 )
	{
		if ( strcasecmp( token, "-dedicated" ) == 0 )
		{
			execution_mode = _execution_mode_dedicated_server;
			return( dedicatedBootStrap() );
		}

		execution_mode = _execution_mode_loop_back_server;
		return( bootStrap() );
	}

	execution_mode = _execution_mode_loop_back_server;
	return( bootStrap() );
}

// ******************************************************************
bool GameManager::bootStrap()
{
	try {
		initializeSoundSubSystem();
		initializeVideoSubSystem();
		loadGameData();
		initializeWindowSubSystem();
		initializeGameObjects();
		initializeGameLogic();
		initializeNetworkSubSystem();
		initializeInputDevices();
	} catch(Exception e) {
#ifdef WIN32
		MessageBox(gapp.hwndApp, e.getMessage(), "Netpanzer", MB_OK);
#else
		fprintf(stderr, "Initialisation failed:\n%s\n", e.getMessage());
#endif
		shutdown();
		return false;
	}
    
	return true;
}

// ******************************************************************
bool GameManager::dedicatedBootStrap()
{
	try {
		dedicatedLoadGameData();
		initializeGameObjects();
		initializeGameLogic();
		initializeNetworkSubSystem();   
		initializeInputDevices();
		initializeDedicatedConsole();
	} catch(Exception e) {
		fprintf(stderr, "Initialisation failed:\n%s\n", e.getMessage());
		dedicatedShutdown();
		return false;
	}
   
	try {
		launchDedicatedServer();
	} catch(Exception e) {
		fprintf(stderr, "Serverlaunch failed :\n%s\n", e.getMessage());
		dedicatedShutdown();
		return false;
	}
	return true;
}

// ******************************************************************

void GameManager::shutdown()
{
	shutdownSubSystems();
}

void GameManager::shutdownSubSystems()
{
	shutdownGameLogic();
	shutdownNetworkSubSystem();
	shutdownSoundSubSystem();
	shutdownVideoSubSystem();
	shutdownInputDevices();
}

void GameManager::dedicatedShutdown()
{
	shutdownGameLogic();
	shutdownNetworkSubSystem();
	shutdownInputDevices();
	shutdownDedicatedConsole();
}

// ******************************************************************

void GameManager::spawnPlayer( const PlayerID &player )
{
  PointXYi spawn_point;

  sound->StopTankIdle(); 
  
  // ** Get a new spawn point and spawn the player ** 
  MapInterface::getFreeSpawnPoint( &spawn_point );
  PlayerInterface::spawnPlayer( player, spawn_point ); 
  
  //** Change the location of the view camera to the spawn point **
  PointXYi world_loc;  
  MapInterface::mapXYtoPointXY( spawn_point, &world_loc );  
  if ( PlayerInterface::getLocalPlayerIndex() == player.getIndex() )
   {
    WorldViewInterface::setCameraPosition( world_loc );
   }
  else
   {
    SystemSetPlayerView set_view;

    set_view.camera_loc = world_loc;
    SERVER->sendMessage( &set_view, sizeof( SystemSetPlayerView ), player, 0 );
   }
  
  sound->PlayTankIdle();
}     

void GameManager::respawnAllPlayers()
 {
  PlayerState    *player_state;
  unsigned short max_players;
  unsigned short player_index;

  max_players = PlayerInterface::getMaxPlayers();

  for( player_index = 0; player_index < max_players; player_index++ )
   {
    player_state = PlayerInterface::getPlayerState( player_index );
    if ( player_state->getStatus() == _player_state_active )
     {
      spawnPlayer( player_state->getPlayerID() );
     }
   }
 }     

// ******************************************************************
void GameManager::spawnPlayer( PlayerState *player_state )
 {
  spawnPlayer( player_state->getPlayerID() ); 
 }  

// ******************************************************************

void GameManager::fraglimitGameCompleted( PlayerState *winner_player_state )
 {
  SystemViewControl view_control;

  view_control.set( "WinnerMesgView", _view_control_flag_visible_on | _view_control_flag_close_all );

  Desktop::setVisibilityAllWindows(false);
  Desktop::setVisibility("GameView", true);
  Desktop::setVisibility( "WinnerMesgView", true );  

  SERVER->sendMessage( &view_control, sizeof( SystemViewControl ), 0 );
 
  game_state = _game_state_completed;
 } 

void GameManager::objectiveGameCompleted( PlayerState *winner_player_state )
 {
  SystemViewControl view_control;

  view_control.set( "WinnerMesgView", _view_control_flag_visible_on | _view_control_flag_close_all );

  Desktop::setVisibilityAllWindows(false);
  Desktop::setVisibility("GameView", true);
  Desktop::setVisibility( "WinnerMesgView", true );  

  SERVER->sendMessage( &view_control, sizeof( SystemViewControl ), 0 );

  game_state = _game_state_completed;
 } 

// ******************************************************************
void GameManager::evaluateGameRules()
 {
  PlayerState *player_state;
  
  if ( game_state == _game_state_in_progress )
   {
    if ( NetworkState::status == _network_state_server )
     {
      unsigned char game_type;
      game_type = GameConfig::GetGameType();
      
      switch( game_type )
       {
       
        case _gametype_fraglimit :
         {
          if ( PlayerInterface::testRuleScoreLimit( GameConfig::GetFragLimit(), &player_state ) == true )
           {
            fraglimitGameCompleted( player_state );
           }
         } break;
             
        case _gametype_objective :
         {
          if ( PlayerInterface::testRuleObjectiveRatio( GameConfig::getObjectiveOccuapationPercentageDecimal(), &player_state ) == true )
           {
            objectiveGameCompleted( player_state );
           }
         } break;
       
       } // ** switch     
      
      // ** Check for Player Respawns ** 
      bool respawn_rule_complete = false;
      while( respawn_rule_complete == false )
       {
        if ( PlayerInterface::testRulePlayerRespawn( &respawn_rule_complete, &player_state ) )
         {
          spawnPlayer( player_state );       
         } // ** if testRulePlayerRespawn
  
       } // ** while 
     
     
     } // ** if NetworkState::status == _network_state_server
   } 
 
 
 }

// ******************************************************************

bool GameManager::initializeConnectionType()
 {
  //bug#15
  int connection_type;
   
  connection_type = GameConfig::GetNetworkConnectType();

  switch( connection_type )
   {
    case _connection_tcpip :
     {
      //bug#15
      //winsock hack
      //ServProvReturn = SetServProv( gapp.hwndApp, TCPIP );
      //if(ServProvReturn == 0) return 0;
     }
    break;

   } // ** switch 

  return 1;
 }

// ******************************************************************

void GameManager::netMessageSetView( NetMessage *message )
 {
  SystemSetPlayerView *set_view;

  set_view = (SystemSetPlayerView *) message;
  
  WorldViewInterface::setCameraPosition( set_view->camera_loc );
 }

// ******************************************************************

void GameManager::netMessageViewControl( NetMessage *message )
 {
  SystemViewControl *view_control;
  
  view_control =  (SystemViewControl *) message;

  if ( view_control->action_flags & _view_control_flag_close_all )
   {
    Desktop::setVisibilityAllWindows(false);
    Desktop::setVisibility("GameView", true);    
   }

  if ( view_control->action_flags & _view_control_flag_visible_on )
   {
    Desktop::setVisibility( view_control->view_name, true );    
   }

  if ( view_control->action_flags & _view_control_flag_visible_off )
   {
    Desktop::setVisibility( view_control->view_name, false );    
   }


 }

// ******************************************************************
void GameManager::netMessageConnectAlert( NetMessage *message )
 {
  SystemConnectAlert *connect_alert = 0;
  PlayerState *player_state = 0;
  
  connect_alert = (SystemConnectAlert *) message;

  player_state = PlayerInterface::getPlayerState( connect_alert->player_id );

  switch( connect_alert->alert_enum )
   {
    case _connect_alert_mesg_connect : 
     {
      ConsoleInterface::postMessage( "%s has joined the game.", player_state->getName() );
     } break;

    case _connect_alert_mesg_disconnect : 
     {
      ConsoleInterface::postMessage( "%s has left the game.", player_state->getName() );
     } break;
    
    case _connect_alert_mesg_client_drop : 
     {
      ConsoleInterface::postMessage( "Connection to %s has been unexpectedly broken.", player_state->getName() );   
     } break;

    default :
     assert(0);
   } // ** switch 
    
 } 

// ******************************************************************

void GameManager::netMessageResetGameLogic( NetMessage *message )
 {
  resetGameLogic();
 }

// ******************************************************************

void GameManager::getServerGameSetup( NetMessage *message )
 {
  ConnectMesgServerGameSettings *game_setup;

  game_setup = (ConnectMesgServerGameSettings *) message;

  game_setup->max_players = GameConfig::GetNumberPlayers();
  game_setup->max_units = GameConfig::GetNumberUnits();
  MapsManager::getCurrentMap( game_setup->map_name );
  game_setup->cloud_coverage = GameConfig::getCloudCoverage();
  game_setup->wind_speed = GameConfig::getWindSpeed();
  game_setup->game_type = GameConfig::GetGameType();
  game_setup->map_cycle_state = GameConfig::getMapCycleState();
  game_setup->powerup_state = GameConfig::getPowerUpState();
  game_setup->frag_limit = GameConfig::GetFragLimit();
  game_setup->time_limit = GameConfig::GetTimeLimit(); 
  game_setup->elapsed_time = getGameTime();
 }

// ******************************************************************

void GameManager::netMessagePingRequest( NetMessage *message )
 {
  PlayerID player_id;
  SystemPingRequest *ping_request;
 
  ping_request = (SystemPingRequest *) message;
  
  player_id = PlayerInterface::getPlayerID( ping_request->client_player_index );
  
  SystemPingAcknowledge ping_ack;

  if ( (PlayerInterface::getPlayerStatus( player_id ) == _player_state_active) && 
       (ping_request->client_player_index != 0)
	 ) 
   {
    SERVER->sendMessage( &ping_ack, sizeof(SystemPingAcknowledge), player_id, 0 );
   }
 }

// ******************************************************************

void GameManager::netMessagePingAcknowledge( NetMessage *message )
 {
  NetworkState::ping_time = (now() - NetworkState::ping_time_stamp) * 1000;
 }

// ******************************************************************

bool GameManager::startClientGameSetup( NetMessage *message, int *result_code )
 {
  ConnectMesgServerGameSettings *game_setup;

  game_setup = (ConnectMesgServerGameSettings *) message;

  GameConfig::SetNumberPlayers( game_setup->max_players );
  GameConfig::SetNumberUnits( game_setup->max_units );
  GameConfig::setCloudCoverage( game_setup->cloud_coverage );
  GameConfig::setWindSpeed( game_setup->wind_speed );
  GameConfig::setMapCycleState( game_setup->map_cycle_state );
  GameConfig::setPowerUpState( game_setup->powerup_state );
  GameConfig::SetGameType( game_setup->game_type );
  GameConfig::SetFragLimit( game_setup->frag_limit );
  GameConfig::SetTimeLimit( game_setup->time_limit );

  startGameTimer();
  game_elapsed_time_offset = game_setup->elapsed_time;

  startGameMapLoad( game_setup->map_name, 20, result_code );  
  return true;
 }

// ******************************************************************

bool GameManager::clientGameSetup( int *percent_complete )
 {
  if( gameMapLoad( percent_complete ) == false )
   {
    reinitializeGameLogic();
    return false;
   }
  
  return true;
 }


// ******************************************************************
void GameManager::processSystemMessage( NetMessage *message )
 {
  switch( message->message_id )
   {
    case _net_message_id_system_set_view :
     netMessageSetView( message );
    break;
    
    case _net_message_id_system_view_control :    
     netMessageViewControl( message );
    break;

    case _net_message_id_system_reset_game_logic :
     netMessageResetGameLogic( message );
    break;

    case _net_message_id_system_ping_request :
     netMessagePingRequest( message );
    break;

    case _net_message_id_system_ping_ack :
     netMessagePingAcknowledge( message );
    break;
 
    case _net_message_id_system_connect_alert :
     netMessageConnectAlert( message );
    break;

  
   } // ** switch
 }

// ******************************************************************
void GameManager::requestNetworkPing()
 {
  SystemPingRequest ping_request;
  
  ping_request.client_player_index = PlayerInterface::getLocalPlayerIndex();
 
  NetworkState::ping_time_stamp = now();
  CLIENT->sendMessage( &ping_request, sizeof(SystemPingRequest), 0  );
    
 }

void GameManager::setNetPanzerGameOptions()
 {

  MiniMapInterface::setProperties( GameConfig::getPlayerRadarUnitColor(),
                                   GameConfig::getAlliedRadarUnitColor(),
                                   GameConfig::getPlayerOutpostRadarColor(),
                                   GameConfig::getAlliedOutpostRadarColor(),
                                   GameConfig::getEnemyOutpostRadarColor() );
 }

//--------------------------------------------------------------------------
void displayHostMultiPlayerGameProgress(const int &curNum)
{
	const int yOffset = 20;
	iXY pos(0, 140);

	{
	FRAME_BUFFER.lock();

	screen.lock(FRAME_BUFFER.mem);
	hostLoadSurface.blt(screen);
	char strBuf[256];
	
	sprintf(strBuf, "SPAWNING HOST");
	pos.x = 179;
	pos.y = 153;
	screen.bltString(pos, strBuf, Color::white);

	pos.y += yOffset;
	pos.y += yOffset;

	sprintf(strBuf, "Load Game Map...................%s", (curNum > 1) ? "DONE" : "");
	pos.y += yOffset;
	screen.bltString(pos, strBuf, Color::white);

	sprintf(strBuf, "Initialize Game Logic...........%s", (curNum > 2) ? "DONE" : "");
	pos.y += yOffset;
	screen.bltString(pos, strBuf, Color::white);

	sprintf(strBuf, "Initializing Connection Type....%s", (curNum > 3) ? "DONE" : "");
	pos.y += yOffset;
	screen.bltString(pos, strBuf, Color::white);

	sprintf(strBuf, "Allocating Server...............%s", (curNum > 4) ? "DONE" : "");
	pos.y += yOffset;
	screen.bltString(pos, strBuf, Color::white);

	sprintf(strBuf, "Spawning Player.................%s", (curNum > 5) ? "DONE" : "");
	pos.y += yOffset;
	screen.bltString(pos, strBuf, Color::white);

	FRAME_BUFFER.unlock();
	screen.unlock();

	Screen->copyDoubleBufferandFlip(); 
	}
}

// ******************************************************************
void GameManager::hostMultiPlayerGame()
 {
	PlayerID player;
	PlayerState *player_state;
	Timer wait;

    progressView.open();

    //winsock hack
    //fix dialup problem--
	//Screen->setGDIStatus(true);
	//minimize = MinimizeOrNot( gapp.hwndApp );    
	
    /*
    initializeConnectionType();
	HostSession( gapp.hwndApp );                 

	if (minimize == false)
	 { 
      Screen->setGDIStatus(false);
	 }  
    else
     {
      OpenIcon( gapp.hwndApp ); 
      Screen->restoreAll();
      Screen->setGDIStatus( FALSE ); 
     }
	*/
    
    //InitStreamServer(gapp.hwndApp);

    progressView.scrollAndUpdateDirect( "Launching Server ..." );
    if ( SERVER->hostSession() == false )
     {
      progressView.scrollAndUpdateDirect( "SERVER LAUNCH FAILED" );
      wait.changePeriod( 4 );    
      while( !wait.count() );
   	  
      progressView.toggleMainMenu();      
      return;
     }

    progressView.updateDirect( "Launching Server ... (100%) " );

    game_state = _game_state_in_progress;
	NetworkState::setNetworkStatus( _network_state_server );
	CLIENT->openSession( _connection_loop_back, 0 );


    progressView.scrollAndUpdateDirect( "Loading Game Data ..." );

    MapsManager::setCycleStartMap( GameConfig::getGameMapName() );
    
    int result_code;
    startGameMapLoad( GameConfig::getGameMapName(), 20, &result_code );  
      
    if( result_code == _mapload_result_no_wad_file )
     {
      progressView.scrollAndUpdateDirect( "MAP TILE SET NOT FOUND!" );
      progressView.scrollAndUpdateDirect( "please download the appropriate tileset" );      
      progressView.scrollAndUpdateDirect( "from www.pyrosoftgames.com" );      
      wait.changePeriod( 12 );    
      while( !wait.count() );
   	  
      progressView.toggleMainMenu();      
      return;     
     }        

    int percent_complete;
    char strbuf[256];

    ObjectiveInterface::resetLogic();

    while( gameMapLoad( &percent_complete ) == true )
     {
      sprintf( strbuf, "Loading Game Data ... (%d%%)", percent_complete);         
      progressView.updateDirect( strbuf );
     }
    
    sprintf( strbuf, "Loading Game Data ... (%d%%)", percent_complete);         
    progressView.updateDirect( strbuf );
	

    progressView.scrollAndUpdateDirect( "Initializing Game Logic ..." );
    reinitializeGameLogic();
    progressView.updateDirect( "Initializing Game Logic ... (100%) " );
	
    
    progressView.scrollAndUpdateDirect( "Spawning Player ..." );
	player_state = PlayerInterface::allocateLoopBackPlayer();
	player_state->setName( GameConfig::GetPlayerName() );
	player_state->setFlag( (unsigned char ) GameConfig::GetPlayerFlag() );
	player = PlayerInterface::getLocalPlayerID();
	spawnPlayer( player );
    progressView.updateDirect( "Spawning Player ... (100%)" );

    wait.changePeriod( 3 );    
    while( !wait.count() );

    startGameTimer();    
    
    progressView.close();

    // Set the palette to the game palette.
    loadPalette( "wads/netp.act" ); 

	setNetPanzerGameOptions();

	// Need to open at beginning of game until we are saving status of things.
	// when last played.
	Desktop::setVisibility("GameToolbarView", true);
	Desktop::setVisibility("GameInfoView", true);
	Desktop::setVisibility("MiniMapView", true);
	Desktop::setVisibility("GameView", true);
	Desktop::setActiveView("GameView");
}

// ******************************************************************

void GameManager::joinMultiPlayerGame()
 {
  setNetPanzerGameOptions();
  //reinitializeGameLogic();
  NetworkState::setNetworkStatus( _network_state_client );
  
  //winsock hack
  //JoinSession( gapp.hwndApp );
  //InitStreamClient(gapp.hwndApp);
  Timer wait;
  
  if ( CLIENT->joinSession() == false )
   {
    lobbyView.scrollAndUpdate( "FAILED TO JOIN NETPANZER SESSION" );
    wait.changePeriod( 4 );    
    while( !wait.count() );
   	  
    lobbyView.toggleMainMenu();      
    return;
   }

  ClientConnectDaemon::startConnectionProcess();
  sound->PlayTankIdle();
 }

// ******************************************************************

void GameManager::launchMultiPlayerGame()
 {
  if( GameConfig::GetHostOrJoin() == _game_session_host )  
   {
    hostMultiPlayerGame();
   }
  else
  if( GameConfig::GetHostOrJoin() == _game_session_join )  
   {
    joinMultiPlayerGame();
   }
 }
// ******************************************************************
void GameManager::launchNetPanzerGame()
 {
  if ( GameConfig::GetGameMode() == _gamemode_multiplayer )
   {
    launchMultiPlayerGame();
   } 
 }

// ******************************************************************
void GameManager::launchDedicatedServer()
 {
  ConsoleInterface::postMessage( "netPanzer Dedicated Server");

  char input_str[256];

  MapsManager::getCurrentMap( input_str );
  GameConfig::setGameMapName( input_str );
  
  printf( "Map Name <%s> : ", GameConfig::getGameMapName() );
  fflush(stdout);
  gets(input_str);
  if ( strlen(input_str) > 0 )
  {  GameConfig::setGameMapName( input_str ); }

  printf( "Players <%d> : ", GameConfig::GetNumberPlayers() );
  fflush(stdout);
  gets(input_str);
  if ( strlen(input_str) > 0 )
   { 
    short players; 
    sscanf( input_str, "%d", &players );
    GameConfig::SetNumberPlayers( players ); 
   } 

  printf( "Units <%d> : ", GameConfig::GetNumberUnits() );
  fflush(stdout);
  gets(input_str);
  if ( strlen(input_str) > 0 )
   { 
    short units; 
    sscanf( input_str, "%d", &units );
    GameConfig::SetNumberUnits( units ); 
   } 
 
  int game_type;
   
  do
  {
   printf( "Game Type\n" );
   printf( "(1) Objective \n");
   printf( "(2) Frag Limit \n" );
   printf( "(3) Time Limit \n" );
   printf( "Choose : " );
   fflush(stdout);
   gets(input_str);
   sscanf( input_str, "%d", &game_type ); 
  } while( (game_type < 1) && (game_type > 3) );
 
  switch( game_type )
   {
    case 1 :
     {
      GameConfig::SetGameType(_gametype_objective);
      printf( "Outpost Occupation <%.0f %%> : ", GameConfig::getObjectiveOccuapationPercentage() );
      fflush(stdout);
      gets(input_str);
      if ( strlen(input_str) > 0 )
       { 
        float percent; 
        sscanf( input_str, "%f", &percent );
        GameConfig::setObjectiveOccuapationPercentage( percent ); 
       } 
     } break;

    case 2 :
     {
      GameConfig::SetGameType(_gametype_fraglimit);
      printf( "Frag Limit <%d> frags : ", GameConfig::GetFragLimit() );
      fflush(stdout);
      gets(input_str);
      if ( strlen(input_str) > 0 )
       { 
        int frags; 
        sscanf( input_str, "%d", &frags);
        GameConfig::SetFragLimit( frags ); 
       } 
     } break;

    case 3 :
     {
      GameConfig::SetGameType(_gametype_timelimit);
      printf( "Time Limit <%d> minutes: ", GameConfig::GetTimeLimit() );
      fflush(stdout);
      gets(input_str);
      if ( strlen(input_str) > 0 )
       { 
        int time_limit; 
        sscanf( input_str, "%d", &time_limit );
        GameConfig::SetTimeLimit( time_limit ); 
       } 
     } break;

   } // ** switch 
    
  printf( "PowerUps <NO> (Y/N) : " );
  fflush(stdout);
  gets(input_str);
  if ( strcasecmp( "y", input_str ) == 0 )
   { GameConfig::setPowerUpState(true); }

  printf( "Server Name <Dedicated Server> :" );
  fflush(stdout);
  gets(input_str);
  if ( strlen(input_str) > 0 )
   { GameConfig::SetPlayerName( input_str ); }
  else
   { GameConfig::SetPlayerName( "Dedicated Server" ); }


  MapsManager::setCycleStartMap( GameConfig::getGameMapName() );
  dedicatedLoadGameMap( GameConfig::getGameMapName() );

  reinitializeGameLogic();

  /*
  MinimizeOrNot( gapp.hwndApp );    
  
  InitializeDirectPlay( gapp.hwndApp);

  initializeConnectionType();
 
  HostSession( gapp.hwndApp );                 
  */

  //winsock hack
  //InitStreamServer(gapp.hwndApp);
  
  SERVER->openSession( 0, 0 );
  SERVER->hostSession( );
  	
  game_state = _game_state_in_progress;

  NetworkState::setNetworkStatus( _network_state_server );

  setNetPanzerGameOptions();  

  Particle2D::setCreateParticles(false); 
  
  ConsoleInterface::postMessage( "Game Launched, Server Ready...");
 
  startGameTimer();
 }
// ******************************************************************
void GameManager::exitNetPanzer()
{
#if 0
  // XXX 
  // NOTE: Hack
  sound->StopTankIdle(); 
#endif

  quitNetPanzerGame();

#ifdef WIN32
  PostMessage(gapp.hwndApp, WM_CLOSE, 0, 0);
#endif
#ifdef USE_SDL
 	SDL_Event event;
	event.type = SDL_QUIT;
	SDL_PushEvent(&event);
#endif
}


void GameManager::quitNetPanzerGame()
 {
  if ( NetworkState::status == _network_state_client )
   {
    ClientConnectDaemon::shutdownConnectDaemon();
    CLIENT->closeSession();
   }
  else
   {
    ServerConnectDaemon::shutdownConnectDaemon();
    SERVER->closeSession();
   }

 
  //***workaround for directplay fuckup on bad client disconnects
  //until we find out why close doesn't work anymore on the server
  //object after the crap out. actually, this call seems unnecessary,
  //since the dplay object is destroyed subsequently anyway. sr
  //QuitSession();
 }

// ******************************************************************
void GameManager::mainLoop()
 {
  if ( execution_mode == _execution_mode_loop_back_server )
   { gameLoop(); }
  else
   { dedicatedGameLoop(); }
 }

// ******************************************************************
void GameManager::gameLoop()
 {
  static bool once = true;

  if (once)
   {
  	once = false;
	gMainLoopBegin = true;
    Desktop::setVisibility("MainView", true);
   }

  TimerInterface::start();

  inputLoop();
  graphicsLoop();
  simLoop();
 
  TimerInterface::update();
 }

// ******************************************************************
void GameManager::simLoop()
 {
  if ( NetworkState::status == _network_state_server )
   { ServerMessageRouter::routeMessages();  }
  else
   { ClientMessageRouter::routeMessages();  } 

  NetworkState::updateNetworkStats();

  UnitInterface::updateUnitStatus();
  ProjectileInterface::updateStatus();
  ObjectiveInterface::updateObjectiveStatus();  
  PowerUpInterface::updateState();
  PathScheduler::run();

  Physics::sim();

  ParticleSystem2D::simAll();
  Particle2D::simAll();
  FontSystem2D::simAll();

  //evaluateGameRules();
  GameControlRulesDaemon::updateGameControlFlow();
 }

// ******************************************************************
void GameManager::inputLoop()
 {
  KeyboardInterface::sampleKeyboard();

  // XXX Joystick code disabled
#if 0
  if (JoystickInterface::exists())
   {
    JoystickInterface::poll();
    gInputOffset = JoystickInterface::getOffset();
   }
#endif
  
  processSystemKeys();

  Desktop::manage(mouse.getScreenPos().x, mouse.getScreenPos().y, mouse.getButtonMask() );

  COMMAND_PROCESSOR.updateScrollStatus( mouse.getScreenPos() );
 }

// ******************************************************************
void GameManager::graphicsLoop()
 {
  FRAME_BUFFER.lock();
  screen.lock(FRAME_BUFFER.mem);
  
  Desktop::draw();
  
  FontSystem2D::drawAll();

  if ( KeyboardInterface::getKeyPressed( SDLK_F9 ) )
   {
	Palette pal;
    if (!screen.saveBMP(pal) )
	 { ConsoleInterface::postMessage("Error taking screen shot."); }
    else
	 { /* ConsoleInterface::postMessage("Screen shot taken."); */ }
   }
  
  char strBuf[256];

  if (display_frame_rate_flag == true)
  {
	sprintf(strBuf, "%3.1f : %3.1f" , TimerInterface::getFPS(), TimerInterface::getFPSAvg());
	screen.bltString5x5(iXY(2, 2), strBuf, Color::white);
  }
 
  if (display_network_info_flag == true)
  {
   sprintf(strBuf, "|| %.4f : %.4f || %.4f : %.4f ||" , NetworkState::packets_sent_per_sec, NetworkState::bytes_sent_per_sec,
                                                        NetworkState::packets_received_per_sec, NetworkState::bytes_received_per_sec  );
   screen.bltString5x5(iXY(60, 2), strBuf, Color::white);
  }

  if (Desktop::getVisible("GameView"))
   { ConsoleInterface::update( FRAME_BUFFER ); }

  mouse.draw(screen);

  //char strBuf[256];
  //sprintf(strBuf, "%d, %d", mouse.getScreenPos().x, mouse.getScreenPos().y);
  //screen.bltString(40, 2, strBuf, Color::white);

  MouseInterface::updateCursor(); 
     
  FRAME_BUFFER.unlock();
  screen.unlock();

  Screen->copyDoubleBufferandFlip(); 

 }

// ******************************************************************
void GameManager::dedicatedGameLoop()
 {
  TimerInterface::start();

  dedicatedInputLoop();
  dedicatedSimLoop();
 
  TimerInterface::update();
 }

// ******************************************************************
void GameManager::dedicatedSimLoop()
 {
  if ( NetworkState::status == _network_state_server )
   { ServerMessageRouter::routeMessages();  }
  else
   { ClientMessageRouter::routeMessages();  } 

  NetworkState::updateNetworkStats();

  UnitInterface::updateUnitStatus();
  ProjectileInterface::updateStatus();
  ObjectiveInterface::updateObjectiveStatus();  
  PowerUpInterface::updateState();
  PathScheduler::run();

  Physics::sim();

  //evaluateGameRules();
  GameControlRulesDaemon::updateGameControlFlow();
 }

// ******************************************************************
void GameManager::dedicatedInputLoop()
 {
  KeyboardInterface::sampleKeyboard();

  // XXX we need SDL code here
#ifdef WIN32
  if( kbhit() )
   {
    int key;
    key = getch();
    if ( key == 0 )
     {
      key = getch();
     }
    else
     {
      switch( key )
       {
        case 27 :
         {
          exitNetPanzer();
         } break;

        case 'Q' :
        case 'q' :
         {
          exitNetPanzer();
         } break;         
        
        case 'M' :
        case 'm' :
         {
          GameControlRulesDaemon::forceMapCycle();
         } break;
         
        case 'I' :
        case 'i' :
         {
          ConsoleInterface::postMessage( "Map: %s", GameConfig::getGameMapName() );
          ConsoleInterface::postMessage( "Players: %d/%d", PlayerInterface::getActivePlayerCount(),
                                                           GameConfig::GetNumberPlayers() );
         
          ConsoleInterface::postMessage( "Units: %d/%d", UnitInterface::getTotalUnitCount(),
                                                         GameConfig::GetNumberUnits() );
         } break;
       
        case '1' :
         {
          ChatInterface::setMessageScopeServer();
          ChatInterface::sendCurrentMessage( "Server will restart in 5 minutes" );
         } break;

        case '2' :
         {
          ChatInterface::setMessageScopeServer();
          ChatInterface::sendCurrentMessage( "Server is restarting" );
         } break;

        case '3' :
         {
          ChatInterface::setMessageScopeServer();
          ChatInterface::sendCurrentMessage( "Server is rotating map" );
         } break;

       } // ** switch
     }
   }
#endif
 
 }

void   GameManager::startGameTimer()
 {
  game_elapsed_time_offset = 0;
  time( &game_start_time );
 }

time_t GameManager::getGameTime() 
 {
  time_t current_time;
  
  time( &current_time );
 
  return( (current_time - game_start_time) + game_elapsed_time_offset );
 }

