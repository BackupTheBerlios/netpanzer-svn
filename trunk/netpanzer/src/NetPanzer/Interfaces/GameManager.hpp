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
#ifndef _GAMEMANAGER_HPP
#define _GAMEMANAGER_HPP

#include "Aliasdef.h"
#include "PlayerState.hpp"
#include "NetPacket.hpp"
#include "Point.hpp"
#include "ClientConnectDaemon.hpp"
#include "ServerConnectDaemon.hpp"
#include "GameControlRulesDaemon.hpp"

enum { _execution_mode_loop_back_server,
       _execution_mode_dedicated_server
     };

enum { _game_state_idle,
       _game_state_in_progress,
       _game_state_completed
     };

enum { _mapload_result_success,
       _mapload_result_no_map_file,
       _mapload_result_no_wad_file
     };


class GameManager 
 {
  private:
   static char map_path[256];

  protected:
   friend class ClientConnectDaemon;
   friend class ServerConnectDaemon;
   friend class GameControlRulesDaemon;

  protected:
   static time_t game_start_time;
   static time_t game_elapsed_time_offset;

   static boolean display_frame_rate_flag;
   static boolean display_network_info_flag;
   
   static int execution_mode;
   
   static PointXYi previous_video_mode_res;
   static PointXYi current_video_mode_res;
   static BYTE     current_mode_flags;

  protected:
   static unsigned char game_state;
   
   static boolean initializeDirectXSubSystem( void );
   static void    shutdownDirectXSubSystem( void );
  
   static boolean initializeWin32SubSystem( void );
   static void    shutdownWin32SubSystem( void );
   
   static boolean initializeNetworkSubSystem( void );
   static void    shutdownNetworkSubSystem( void );

   static boolean initializeInputDevices( void );
   
   static boolean initializeVideoSubSystem( void );

   static boolean initializeSoundSubSystem( void );
   static void    shutdownSoundSubSystem( void );

   static boolean initializeWindowSubSystem( void );
   
   // initialize all static objects / interfaces;
   static boolean initializeGameObjects( void );
   static boolean shutdownGameObjects( void );

   static boolean initializeDedicatedConsole( void );
   static boolean shutdownDedicatedConsole( void );

   // boots up netPanzer; initializes all subsystems, game objects etc. 
   static boolean bootStrap( void );
   static boolean dedicatedBootStrap( void );

   static void shutdown( unsigned long failure_index );
   static void dedicatedShutdown( unsigned long failure_index );

   // cyclic executive for loop back server / client 
   static void gameLoop( void );

   static void simLoop( void );
   static void inputLoop( void );
   static void graphicsLoop( void );
       
   // cyclic executive for dedicated server
   static void dedicatedGameLoop( void );

   static void dedicatedSimLoop( void );
   static void dedicatedInputLoop( void );
   
   static void setupKeyboardBindings( void );

   static boolean startGameMapLoad( char *map_file_path, unsigned long partitions, int *result_code );
   static boolean gameMapLoad( int *percent_complete );
   static void finishGameMapLoad( void );

   static void dedicatedLoadGameMap( char *map_file_path );

   static boolean loadGameData( void );     
   static boolean dedicatedLoadGameData( void );

   static boolean initializeGameLogic( void );
   static boolean reinitializeGameLogic( void );
   static boolean resetGameLogic( void );
   static void    shutdownGameLogic( void );
   
   static void shutdownParticleSystems( void );

   static void processSystemKeys( void );

   // ** Game Rules Methods   
   static void spawnPlayer( PlayerState *player_state );
   static void respawnAllPlayers( void );     
   static void fraglimitGameCompleted( PlayerState *winner_player_state ); 
   static void objectiveGameCompleted( PlayerState *winner_player_state );
   static void evaluateGameRules( void );
 
   // ** Network Message Handlers
   static void netMessageSetView( NetMessage *message );
   static void netMessageViewControl( NetMessage *message );
   static void netMessageClientGameSetup( NetMessage *message );
   static void netMessagePingRequest( NetMessage *message );
   static void netMessagePingAcknowledge( NetMessage *message ); 
   static void netMessageConnectAlert( NetMessage *message );
   static void netMessageResetGameLogic( NetMessage *message );
   static void getServerGameSetup( NetMessage *message );
   
   static boolean startClientGameSetup( NetMessage *message, int *result_code );
   static boolean clientGameSetup( int *percent_complete );

   // ** Game Launching Methods
   static void hostMultiPlayerGame( void );
   static void joinMultiPlayerGame( void );
   static void launchMultiPlayerGame( void );
   static void launchDedicatedServer( void );

   
  
  public:

   static boolean initialize( const char *command_line );
   
   static void shutdown( void );
      
   static void mainLoop( void );
 
   static void launchNetPanzerGame( void );

   static void exitNetPanzer( void );

   static void quitNetPanzerGame( void );

   static void setNetPanzerGameOptions( void );

   static boolean initializeConnectionType( void );

   static void spawnPlayer( PlayerID &player );     

   static void requestNetworkPing( void );

   static void processSystemMessage( NetMessage *message );

   static boolean setVideoMode( PointXYi mode_res );
   static void	restorePreviousVideoMode( void );
   
   static void increaseDisplayResolution( void );
   static void decreaseDisplayResolution( void );

   static void loadPalette( char *palette_path );

   static void drawTextCenteredOnScreen(const char *string, unsigned char color);
   
   static void   startGameTimer( void );
   static time_t getGameTime( void ); 
 };

#endif
