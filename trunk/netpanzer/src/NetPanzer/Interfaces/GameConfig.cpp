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
#include "GameConfig.hpp"

//#include "lua.h"
//#include "lualib.h"

  char  GameConfig::UnitColor;
  char  GameConfig::GameMode;                 //Skirmish or Multiplayer
  char  GameConfig::GameType;                 //Objectives, FragLimit, TimeLimit
  char  GameConfig::HostOrJoin;               // 1=host, 2=join 
  char  GameConfig::VehicleGeneration;
  short GameConfig::NumberPlayers      = 8;   //max = 25;
  short GameConfig::NumberUnits        = 500; //max = 50;
  short GameConfig::NumberInitialUnits = 5;
  
  char  GameConfig::PlayerName[64];

  short GameConfig::PlayerFlag;

  int   GameConfig::NetworkConnectionType = _connection_tcpip;
  int   GameConfig::TimeLimit = 50;          //in minutes
  int   GameConfig::FragLimit = 1000;        //in frags;

  bool      GameConfig::map_cycling_on_off = true;
  bool      GameConfig::powerups_on_off = false;

  float        GameConfig::objective_occupation_percentage = 100.0;
  bool      GameConfig::allow_allies_on_off = true;
  int          GameConfig::cloud_coverage;
  float        GameConfig::wind_speed;
  unsigned int GameConfig::respawn_type = _game_config_respawn_type_round_robin;

  char    GameConfig::game_map_name[256];

   // ** Visuals Configuration **
   unsigned int GameConfig::screen_resolution_enum = _game_config_standard_res_640x480;
   
   bool GameConfig::display_shadows_flag = true;
   bool GameConfig::display_unit_flags = false;
  
   bool GameConfig::radar_display_clouds_flag = false;

   int     GameConfig::radar_player_unit_color = _color_aqua;
   int     GameConfig::radar_allied_unit_color = _color_orange;
   int     GameConfig::radar_player_outpost_color = _color_blue;
   int     GameConfig::radar_allied_outpost_color = _color_orange;
   int     GameConfig::radar_enemy_outpost_color = _color_red;
   int     GameConfig::vehicle_selection_box_color = _color_blue;
   int     GameConfig::console_text_color = _color_white;
   
   int     GameConfig::mini_map_unit_size = _mini_map_unit_size_small;
   int     GameConfig::unit_selection_box_draw_mode = _unit_selection_box_draw_mode_rect_edges;
   bool GameConfig::draw_unit_damage = false;
   bool GameConfig::draw_unit_reload = true;
   int     GameConfig::mini_map_objective_draw_mode = _mini_map_objective_draw_mode_outline_rect;
   int     GameConfig::unitInfoDrawLayer = 0;

   float   GameConfig::console_test_delay = 10.0;   // in seconds
   int	   GameConfig::console_text_usage = 25;   	// in lines

   int     GameConfig::screen_gamma = 50;       // 0..100
   float   GameConfig::screen_brightness = 1.0f;  // 0..100

   float   GameConfig::scroll_rate = 1000;            // in pixels/s   
   int     GameConfig::mini_map_resize_rate = 400;   // in pixels/s
     
   // ** Input Configuration **
   bool GameConfig::input_joystick_state = false; 
   
   // ** Sound Configuration **
   bool GameConfig::sound_on_off_flag = true;
   int	   GameConfig::sound_volume = 50;

   PlayerUnitConfig GameConfig::unit_spawn_config;

   int     GameConfig::attackNotificationTime = 5;
   bool GameConfig::blendSmoke             = true;


void GameConfig::initialize( void )
 {
  //UnitColor;
  GameMode = _gamemode_multiplayer; //Skirmish or Multiplayer
  GameType = _gametype_objective;   //Objectives, FragLimit, TimeLimit
  HostOrJoin = _game_session_join;  // 1=host, 2=join 
  VehicleGeneration = true;
  NumberPlayers      = 8;           //max = 25;
  NumberUnits        = 500;         //max = 50;
  NumberInitialUnits = 5;
  
  strcpy( PlayerName, "Player" );

  PlayerFlag = 0;

  NetworkConnectionType = _connection_tcpip;
  TimeLimit = 50;                   //current limit = 120
  FragLimit = 1000;                   //current limit = 1000;

  objective_occupation_percentage = 100.0f;
  allow_allies_on_off = true;
  /* XXX fix warnings...
  cloud_coverage;
  wind_speed;
  */
  respawn_type = _game_config_respawn_type_round_robin;

  // ** Visuals Configuration **
  screen_resolution_enum = _game_config_standard_res_640x480;
   
  display_shadows_flag = true;
   
  radar_display_clouds_flag = false;

  radar_player_unit_color = _color_aqua;
  radar_allied_unit_color = _color_orange;
  radar_player_outpost_color = _color_blue;
  radar_allied_outpost_color = _color_orange;
  radar_enemy_outpost_color = _color_red;
   
  console_test_delay = 3.0;       // in seconds
  console_text_usage = 25;   	// in lines

  screen_gamma = 50;       // 0..100
  screen_brightness = 1.0f;  // 0..100

  scroll_rate = 1000;            // in pixels/s   
  mini_map_resize_rate = 400;   // in pixels/s
     
  // ** Input Configuration **
  input_joystick_state = false; 
   
  // ** Sound Configuration **
  sound_on_off_flag = true;
  sound_volume = 50;

 }

void GameConfig::setGameMapName( char *map_name )
 {
  strcpy( game_map_name, map_name );
 }

char * GameConfig::getGameMapName( void )
 {
  return( game_map_name );
 }

void GameConfig::loadConfigScript( void )
 {
  return;

  //int lua_return;
  //lua_Object envValue;


  // The LUA library is probably not compatible with
  // GPL so I'm commenting it out for now 
  /*
  lua_return = lua_dofile( "Scripts\\config.lua" ); 
  assert( lua_return == 0 );
  
  envValue = lua_getglobal( "envGameType" );            
  SetGameType( (int) lua_getnumber( envValue ) );

  envValue = lua_getglobal( "envMaxPlayers" );            
  SetNumberPlayers( (int) lua_getnumber( envValue ) );

  envValue = lua_getglobal( "envMaxUnits" );            
  SetNumberUnits( (int) lua_getnumber( envValue ) );
  
  envValue = lua_getglobal( "envPlayerOrServerName" );            
  SetPlayerName( lua_getstring( envValue ) );

  envValue = lua_getglobal( "envTimeLimit" );            
  SetTimeLimit( (int) lua_getnumber( envValue ) );

  envValue = lua_getglobal( "envFragLimit" );            
  SetFragLimit( (int) lua_getnumber( envValue ) );

  envValue = lua_getglobal( "envMapCycling" );            
  setMapCycleState( (int) lua_getnumber( envValue ) );

  envValue = lua_getglobal( "envPowerUps" );            
  setPowerUpState( (int) lua_getnumber( envValue ) ); 
  */
 }
