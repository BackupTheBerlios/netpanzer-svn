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
#ifndef __GAMECONFIG_HPP
#define __GAMECONFIG_HPP

#include "string.h"
#include "memory.h"
#include "PObject.hpp"
#include "Point.hpp"
#include "Surface.hpp"
#include "PlayerUnitConfig.hpp"

enum { _mini_map_objective_draw_mode_solid_rect,
       _mini_map_objective_draw_mode_outline_rect,
	   _mini_map_objective_draw_mode_count
       //_mini_map_objective_draw_mode_player_flag,
     };

enum { _mini_map_unit_size_small,
       _mini_map_unit_size_large,
       _mini_map_unit_size_count
     };

enum { _unit_selection_box_draw_mode_rect,
       _unit_selection_box_draw_mode_rect_edges,
       _unit_selection_box_draw_mode_count
     };

enum { _gamemode_skirmish, 
       _gamemode_multiplayer 
     };

enum { _game_session_host,
       _game_session_join
     };

enum { _gametype_objective, 
       _gametype_fraglimit, 
       _gametype_timelimit 
     };

enum { _connection_tcpip, 
       _connection_direct_connect, 
       _connection_modem
     };

enum { _game_config_respawn_type_round_robin,
       _game_config_respawn_type_random,
	   _game_config_respawn_type_max
	 };

enum { _game_config_standard_res_640x480,
       _game_config_standard_res_800x600,
	   _game_config_standard_res_1024x768,
	   _game_config_standard_res_max
	 };

enum { _color_aqua,
       _color_blue,
       _color_dark_blue,
       _color_blue_gray,
       _color_red,
       _color_dark_red,
       _color_light_green,
       _color_green,
       _color_dark_green,
       _color_yellow,
       _color_light_orange,
       _color_orange,
       _color_black,
       _color_white,
       _color_gray,
       _color_dark_gray,
     };


#define _GAME_CONFIG_PLAYER_LIMIT_UPPER 25
#define _GAME_CONFIG_PLAYER_LIMIT_LOWER  1

#define _GAME_CONFIG_UNIT_LIMIT_UPPER   2000
#define _GAME_CONFIG_UNIT_LIMIT_LOWER      1        

#define _GAME_CONFIG_INITIAL_UNIT_LIMIT_UPPER   10
#define _GAME_CONFIG_INITIAL_UNIT_LIMIT_LOWER    1 

#define _GAME_CONFIG_FRAGLIMIT_UPPER 10000
#define _GAME_CONFIG_FRAGLIMIT_LOWER     3

#define _GAME_CONFIG_TIMELIMIT_UPPER  1440
#define _GAME_CONFIG_TIMELIMIT_LOWER     5

#define _GAME_CONFIG_SCROLL_RATE_LIMIT_UPPER 2000
#define _GAME_CONFIG_SCROLL_RATE_LIMIT_LOWER  100

#define _GAME_CONFIG_MINI_MAP_SCROLL_LIMIT_UPPER	 1000
#define _GAME_CONFIG_MINI_MAP_SCROLL_LIMIT_LOWER	  100

// XXX: CHanged the CONFIG_CONSOLE values from doubles to ints (removed .0)
#define _GAME_CONFIG_CONSOLE_TEXT_DELAY_LIMIT_UPPER 30
#define _GAME_CONFIG_CONSOLE_TEXT_DELAY_LIMIT_LOWER  1

#define _GAME_CONFIG_CONSOLE_TEXT_USEAGE_LIMIT_UPPER 25
#define _GAME_CONFIG_CONSOLE_TEXT_USEAGE_LIMIT_LOWER  5

#define _GAME_CONFIG_SCREEN_GAMMA_LIMIT_UPPER  100
#define _GAME_CONFIG_SCREEN_GAMMA_LIMIT_LOWER	 0

const float _GAME_CONFIG_SCREEN_BRIGHTNESS_LIMIT_UPPER = +3.0f;
const float _GAME_CONFIG_SCREEN_BRIGHTNESS_LIMIT_LOWER =  0.0f;

#define _GAME_CONFIG_SOUND_VOLUME_LIMIT_UPPER  100
#define _GAME_CONFIG_SOUND_VOLUME_LIMIT_LOWER    0 

#define _GAME_CONFIG_ATTACK_NOTIFICATION_TIME_LIMIT_UPPER 30
#define _GAME_CONFIG_ATTACK_NOTIFICATION_TIME_LIMIT_LOWER  2


class GameConfig : public PObject
{

  protected:
   static char    UnitColor;
   static char    GameMode;               //Skirmish or Multiplayer
   static char    GameType;               //Objectives, FragLimit, TimeLimit
   static char    HostOrJoin;             //1=host, 2=join 
   static char    VehicleGeneration;
   static short   NumberPlayers;          //max = 25;
   static short   NumberUnits;            //max = 50;
   static short   NumberInitialUnits;     //max = 50; 

   static char    PlayerName[64];

   static short   PlayerFlag;
				  
   static int     NetworkConnectionType;
   static int     TimeLimit;               //current limit = 120
   static int     FragLimit;               //current limit = 1000;

   static bool      map_cycling_on_off;
   static bool      powerups_on_off;
   static float        objective_occupation_percentage;
   static bool      allow_allies_on_off;
   static int          cloud_coverage;
   static unsigned int respawn_type;
   static float        wind_speed;
   static int          attackNotificationTime;
   static bool      blendSmoke;

   static char         game_map_name[256];

   // ** Visuals Configuration **
   static unsigned int  screen_resolution_enum;

   static bool display_shadows_flag;
   static bool display_unit_flags;
   
   static bool radar_display_clouds_flag;
   static int     radar_player_unit_color;
   static int     radar_allied_unit_color;
   static int     radar_player_outpost_color;
   static int     radar_allied_outpost_color;

   static int     radar_enemy_outpost_color;
   static int     vehicle_selection_box_color;
   static int     console_text_color;

   static int     mini_map_unit_size;
   static int     unit_selection_box_draw_mode;
   static bool draw_unit_damage;
   static bool draw_unit_reload;
   static int     mini_map_objective_draw_mode;
   static int     unitInfoDrawLayer;

   static float   console_test_delay;       // in seconds
   static int	  console_text_usage;   	// in lines

   static int     screen_gamma;

   static float   scroll_rate;            // in pixels/s   
   static int     mini_map_resize_rate;   // in pixels/s

   static float screen_brightness; // [-1.0 ... 1.0]
  
   
   // ** Input Configuration **
   static bool input_joystick_state; 
   
   // ** Sound Configuration **
   static bool sound_on_off_flag;
   static int	  sound_volume;
  
  protected:
   
    static inline PIX colorEnumToPix( int color_enum )
     {
      switch( color_enum )
       {
        case _color_aqua         : return( Color::unitAqua );
        case _color_yellow       : return( Color::unitYellow );
        case _color_red          : return( Color::unitRed );
        case _color_blue         : return( Color::unitBlue );
        case _color_dark_blue    : return( Color::unitDarkBlue );
        case _color_light_green  : return( Color::unitLightGreen );
        case _color_green        : return( Color::unitGreen );
        case _color_blue_gray    : return( Color::unitBlueGray );
        case _color_dark_red     : return( Color::unitDarkRed );
        case _color_black        : return( Color::unitBlack );
        case _color_dark_green   : return( Color::unitDarkGreen );
        case _color_white        : return( Color::unitWhite );
        case _color_light_orange : return( Color::unitLightOrange );
        case _color_orange       : return( Color::unitOrange );
        case _color_gray         : return( Color::unitGray );
        case _color_dark_gray    : return( Color::unitDarkGray );
       } // ** switch 
      
      assert(false);
	  return( Color::white );
     }

  public:
	static PlayerUnitConfig unit_spawn_config;

    static void initialize( void );

	static inline void SetUnitColor(char unit_color)
	{	UnitColor = unit_color;}

	static inline char GetUnitColor()
	{	return(UnitColor);}
	//////////////////////////////


	static inline void SetGameMode(char game_mode)
	{	GameMode = game_mode;}

	static inline char GetGameMode()
	{	return(GameMode);}
	//////////////////////////////


	static inline void SetGameType(char game_type)
	{	GameType = game_type;}

	static inline char GetGameType()
	{	return(GameType);}
	//////////////////////////////
    
    static char * getGameTypeString()
     {
      switch(GameType)
       {
        case _gametype_objective :
         return( "Objective" );
        case _gametype_fraglimit :
         return( "Frag Limit" ); 
        case _gametype_timelimit :
         return( "Time Limit" );
       }
      return( "Unknown" );
     }

	static inline void SetHostOrJoin(char host_or_join)
	{	
     HostOrJoin = host_or_join; 
    }

	static inline char GetHostOrJoin()
	{	return(HostOrJoin);}
	//////////////////////////////


	static inline void SetGeneration(char generation)
	{	VehicleGeneration = generation;}

	static inline char GetGeneration()
	{	return(VehicleGeneration);}
	//////////////////////////////


	//this will be more complex later--
	static inline void SetNumberPlayers(short num_players)
	{	if(num_players < _GAME_CONFIG_PLAYER_LIMIT_LOWER) 
	     NumberPlayers = _GAME_CONFIG_PLAYER_LIMIT_LOWER;
		else 
         if(num_players > _GAME_CONFIG_PLAYER_LIMIT_UPPER ) 
          NumberPlayers = _GAME_CONFIG_PLAYER_LIMIT_UPPER;
		 else NumberPlayers = num_players;
    }

	static inline short GetNumberPlayers()
	{	return(NumberPlayers);}

    static inline int GetNumberPlayersBoundsLower( void  )
     {
      return( _GAME_CONFIG_PLAYER_LIMIT_LOWER );
     }
 
    static inline int GetNumberPlayersBoundsUpper( void )
     {
      return( _GAME_CONFIG_PLAYER_LIMIT_UPPER );
     }

	//this will be more complex later--
	static inline void SetNumberUnits(short num_units)
	{	if(num_units < _GAME_CONFIG_UNIT_LIMIT_LOWER ) 
         NumberUnits = _GAME_CONFIG_UNIT_LIMIT_LOWER;
		else 
         if(num_units > _GAME_CONFIG_UNIT_LIMIT_UPPER ) 
          NumberUnits = _GAME_CONFIG_UNIT_LIMIT_UPPER ;
		else 
         NumberUnits = num_units;
    }
	
	static inline short GetNumberUnits()
	 {	
      return( NumberUnits ); 
     }
	
	static inline int getNumberUnitsBoundsUpper( void )
	 { return( _GAME_CONFIG_UNIT_LIMIT_UPPER); }

	static inline int getNumberUnitsBoundsLower( void )
	 { return(_GAME_CONFIG_UNIT_LIMIT_LOWER); }

    static inline short GetUnitsPerPlayer()
     {
      return( NumberUnits / ((short) NumberPlayers) );
     }
     
    //////////////////////////////

    static inline void SetNumberInitialUnits( short num_units )
     {
      if ( num_units < _GAME_CONFIG_INITIAL_UNIT_LIMIT_LOWER )
       {
        NumberInitialUnits = _GAME_CONFIG_INITIAL_UNIT_LIMIT_LOWER;
       }
      else
       if ( num_units > NumberUnits )
        {
         num_units = NumberUnits;        
         if ( num_units > _GAME_CONFIG_INITIAL_UNIT_LIMIT_UPPER  )
          { NumberInitialUnits = _GAME_CONFIG_INITIAL_UNIT_LIMIT_UPPER ; }         
         else
          { NumberInitialUnits = num_units; }     
        }
       else
        if ( num_units > _GAME_CONFIG_INITIAL_UNIT_LIMIT_UPPER  )
         { NumberInitialUnits = _GAME_CONFIG_INITIAL_UNIT_LIMIT_UPPER ; }         
        else
         { NumberInitialUnits = num_units; }     
     }

    static inline short GetNumberInitialUnits( void )
     {
      return( NumberInitialUnits );
     }

	static inline int getNumberInitialUnitsBoundUpper( void )
	 { return (_GAME_CONFIG_INITIAL_UNIT_LIMIT_UPPER ); }

	static inline int getNumberInitialUnitsBoundLower( void )
	 { return (_GAME_CONFIG_INITIAL_UNIT_LIMIT_LOWER ); }

	static inline void SetPlayerName(const char *player_name)
	{	memmove(PlayerName, player_name, 64);}

	static inline char *GetPlayerName()
	{	return(PlayerName);}
	//////////////////////////////


	static inline void SetPlayerFlag(short player_flag)
	{	PlayerFlag = player_flag;}

	static inline short GetPlayerFlag()
	{	return(PlayerFlag);}
	//////////////////////////////


	static inline void SetNetworkConnectType(int network_connect)
	{	NetworkConnectionType = network_connect;}

	static inline int GetNetworkConnectType()
	{	return(NetworkConnectionType);}
	//////////////////////////////


	static inline void SetTimeLimit(int time_limit)
	 {	
      if(time_limit < _GAME_CONFIG_TIMELIMIT_LOWER) 
        TimeLimit = _GAME_CONFIG_TIMELIMIT_LOWER;
	   else 
        if(time_limit > _GAME_CONFIG_TIMELIMIT_UPPER ) 
          TimeLimit = _GAME_CONFIG_TIMELIMIT_UPPER;
		 else 
          TimeLimit = time_limit;
     }

	static inline int GetTimeLimit()
	{	return( TimeLimit );}
	//////////////////////////////

	static inline int GetTimeLimitSeconds()
	 { return( TimeLimit * 60 ); }
    
    static inline int GetTimeLimitBoundsUpper( void  )
     {
      return( _GAME_CONFIG_TIMELIMIT_UPPER );
     }
   
    static inline int GetTimeLimitBoundsLower( void )
     {
      return( _GAME_CONFIG_TIMELIMIT_LOWER );
     }

	static inline void SetFragLimit(int frag_limit)
	 {	
       if(frag_limit < _GAME_CONFIG_TIMELIMIT_LOWER) 
         FragLimit = _GAME_CONFIG_TIMELIMIT_LOWER;
		else 
         if(frag_limit > _GAME_CONFIG_FRAGLIMIT_UPPER) 
          FragLimit = _GAME_CONFIG_FRAGLIMIT_UPPER;
		 else 
          FragLimit = frag_limit;
     }

	static inline int GetFragLimit()
	{	return(FragLimit);}
	//////////////////////////////

    static inline int GetFragLimitBoundsUpper( void )
     {
      return( _GAME_CONFIG_FRAGLIMIT_UPPER );
     }

    static inline int GetFragLimitBoundsLower( void )
     {
      return( _GAME_CONFIG_FRAGLIMIT_LOWER );
     }

	static inline void increaseObjectiveOccuapationPercentage( void )
	 {
	  if ( (objective_occupation_percentage + 10) > 100 )
	   { objective_occupation_percentage = 100; }
	  else
	   { objective_occupation_percentage  = (objective_occupation_percentage + 10); }	    
	 }
	
	static inline void decreaseObjectiveOccuapationPercentage( void )
	 {
	  if ( (objective_occupation_percentage - 10) < 20 )
	   { objective_occupation_percentage = 20; }
	  else
	   { objective_occupation_percentage  = (objective_occupation_percentage - 10); }	    
	 }
	
	static inline float getObjectiveOccuapationPercentage()
	 {
	  return( objective_occupation_percentage );
	 } 

	static inline float getObjectiveOccuapationPercentageDecimal()
	 {
	  return( objective_occupation_percentage / 100 );
	 } 

	static inline void setObjectiveOccuapationPercentage( float percent )
	 {
	  if( percent < 20 )
       { objective_occupation_percentage = 20; }
      else 
       if( percent > 100 )
        { objective_occupation_percentage = 100; }
       else
        {
         objective_occupation_percentage = percent;
        }
	 } 
    
    static inline void setMapCycleState( bool on_off )
	 {
	  map_cycling_on_off = on_off;
	 }

	static inline bool getMapCycleState( void )
	 {
	  return( map_cycling_on_off );
	 }

    static inline char * getMapCycleString( void )
	 {
	  if ( 	map_cycling_on_off == true )
	   { return( "Yes" ); }
	  else
	   { return( "No" ); }
	 }

    static inline void setPowerUpState( bool on_off )
	 {
	  powerups_on_off= on_off;
	 }

	static inline bool getPowerUpState( void )
	 {
	  return( powerups_on_off );
	 }

    static inline char * getPowerUpString( void )
	 {
	  if ( 	powerups_on_off == true )
	   { return( "Yes" ); }
	  else
	   { return( "No" ); }
	 }

	static inline void setAllieState( bool on_off )
	 {
	  allow_allies_on_off = on_off;
	 }

	static inline bool getAllieState( void )
	 {
	  return( allow_allies_on_off );
	 }

    static inline char * getAllieStateString( void )
	 {
	  if ( 	allow_allies_on_off == true )
	   { return( "Yes" ); }
	  else
	   { return( "No" ); }
	 }

	static inline void setNextRespawnType(	void )
	 {
	  respawn_type = (respawn_type + 1) % _game_config_respawn_type_max;
	 }

    static inline void setPreviousRespawnType(	void )
	 {
	  respawn_type = (respawn_type - 1) % _game_config_respawn_type_max;
	 }

	static inline char * getRespawnTypeString( void )
	 {
	  switch( respawn_type )
	   {
	    case _game_config_respawn_type_round_robin :
		 return( "Round Robin" );
		break;
		
		case _game_config_respawn_type_random :
		 return( "Random" );
		break;	   

		case _game_config_respawn_type_max :
		 return( "Max" );
		break;	   
	   } // ** switch
	  
	  assert(false);
	  return( "Unknown" );
	 }

   static inline void setCloudCoverage( int cloud_coverage )
    {
	 GameConfig::cloud_coverage = cloud_coverage;
	}

   static inline int getCloudCoverage( void )
   	{
	 return( cloud_coverage );
	}
   
   static inline void setWindSpeed( float FUCK_FUCK_FUCK_FUCK_FUCK_FUCK_FUCK_FUCK_FUCK )
    {
	 GameConfig::wind_speed = FUCK_FUCK_FUCK_FUCK_FUCK_FUCK_FUCK_FUCK_FUCK;
	}

   static inline float getWindSpeed( void )
    {
	 return( wind_speed );
	}
   
   static void setGameMapName( char *map_name );   
   static char * getGameMapName( void );

   // ** Visuals Configuration Methods **
	static inline float getScrollRate( void )
	 {
	  return( scroll_rate );
	 } 
   
    static inline void setScrollRate( float rate )
	 {
	  if ( rate < _GAME_CONFIG_SCROLL_RATE_LIMIT_LOWER )
	   { scroll_rate = _GAME_CONFIG_SCROLL_RATE_LIMIT_LOWER; }
	  else
	   if ( rate > _GAME_CONFIG_SCROLL_RATE_LIMIT_UPPER )
	    { scroll_rate = _GAME_CONFIG_SCROLL_RATE_LIMIT_UPPER; }	 
	   else
	    { scroll_rate = rate; }
	 }

	static inline int getScrollRateBoundsUpper( void )
	 { return( _GAME_CONFIG_SCROLL_RATE_LIMIT_UPPER); }

	static inline int getScrollRateBoundsLower( void )
	 { return(_GAME_CONFIG_SCROLL_RATE_LIMIT_LOWER); }

   //***********************************************************
	static inline float getMiniMapResizeRate( void )
	 {
	  return( mini_map_resize_rate );
	 } 

    static inline void setMiniMapResizeRate( float rate )
	 {
	  if ( rate < _GAME_CONFIG_SCROLL_RATE_LIMIT_LOWER )
	   { mini_map_resize_rate = _GAME_CONFIG_MINI_MAP_SCROLL_LIMIT_LOWER; }
	  else
	   if ( rate > _GAME_CONFIG_MINI_MAP_SCROLL_LIMIT_UPPER )
	    { mini_map_resize_rate = _GAME_CONFIG_MINI_MAP_SCROLL_LIMIT_UPPER; }	 
	   else
	    { mini_map_resize_rate = (int) rate; }
	 }

	static inline int getMiniMapResizeRateBoundsUpper( void )
	 { return( _GAME_CONFIG_MINI_MAP_SCROLL_LIMIT_UPPER); }

	static inline int getMiniMapResizeRateBoundsLower( void )
	 { return(_GAME_CONFIG_MINI_MAP_SCROLL_LIMIT_LOWER); }

   //***********************************************************
	static inline int getScreenGamma( void )
	 {
	  return( screen_gamma );
	 } 

    static inline void setScreenGamma( int gamma )
	 {
	  if ( gamma < _GAME_CONFIG_SCREEN_GAMMA_LIMIT_LOWER )
	   { screen_gamma = _GAME_CONFIG_SCREEN_GAMMA_LIMIT_LOWER; }
	  else
	   if ( gamma > _GAME_CONFIG_SCREEN_GAMMA_LIMIT_UPPER )
	    { screen_gamma = _GAME_CONFIG_SCREEN_GAMMA_LIMIT_UPPER; }	 
	   else
	    { screen_gamma = gamma; }
	 }

	static inline float getScreenGammaBoundsUpper( void )
	 { return( _GAME_CONFIG_SCREEN_GAMMA_LIMIT_UPPER); }

	static inline float getScreenGammaBoundsLower( void )
	 { return(_GAME_CONFIG_SCREEN_GAMMA_LIMIT_LOWER); }

 //*********************************************************** 
	static inline float getScreenBrightness( void )
	 {
	  return( screen_brightness );
	 } 

    static inline void setScreenBrightness( float brightness )
	 {
	  if ( brightness < _GAME_CONFIG_SCREEN_BRIGHTNESS_LIMIT_LOWER )
	   { screen_brightness = _GAME_CONFIG_SCREEN_BRIGHTNESS_LIMIT_LOWER; }
	  else
	   if ( brightness > _GAME_CONFIG_SCREEN_BRIGHTNESS_LIMIT_UPPER )
	    { screen_brightness = _GAME_CONFIG_SCREEN_BRIGHTNESS_LIMIT_UPPER; }	 
	   else
	    { screen_brightness = brightness; }
	 }

	static inline float getScreenBrightnessBoundsUpper( void )
	 { return( _GAME_CONFIG_SCREEN_BRIGHTNESS_LIMIT_UPPER); }

	static inline float getScreenBrightnessBoundsLower( void )
	 { return(_GAME_CONFIG_SCREEN_BRIGHTNESS_LIMIT_LOWER); }

 //*********************************************************** 
	static inline int getConsoleTextUsage( void )
	 {
	  return( console_text_usage );
	 } 

    static inline void setConsoleTextUsage( int lines )
	 {
	  if ( lines < _GAME_CONFIG_CONSOLE_TEXT_USEAGE_LIMIT_LOWER )
	   { console_text_usage = _GAME_CONFIG_CONSOLE_TEXT_USEAGE_LIMIT_LOWER; }
	  else
	   if ( lines > _GAME_CONFIG_CONSOLE_TEXT_USEAGE_LIMIT_UPPER )
	    { console_text_usage = _GAME_CONFIG_CONSOLE_TEXT_USEAGE_LIMIT_UPPER; }	 
	   else
	    { console_text_usage = lines; }
	 }

	static inline int getConsoleTextUsageBoundsUpper( void )
	 { return( _GAME_CONFIG_CONSOLE_TEXT_USEAGE_LIMIT_UPPER); }

	static inline int getConsoleTextUsageBoundsLower( void )
	 { return(_GAME_CONFIG_CONSOLE_TEXT_USEAGE_LIMIT_LOWER); }


 //*********************************************************** 
	static inline float getConsoleTextDelay( void )
	 {
	  return( console_test_delay );
	 } 
   
    static inline void setConsoleTextDelay( float delay )
	 {
	  if ( delay < _GAME_CONFIG_CONSOLE_TEXT_DELAY_LIMIT_LOWER )
	   { console_test_delay = _GAME_CONFIG_CONSOLE_TEXT_DELAY_LIMIT_LOWER; }
	  else
	   if ( delay > _GAME_CONFIG_CONSOLE_TEXT_DELAY_LIMIT_UPPER )
	    { console_test_delay = _GAME_CONFIG_CONSOLE_TEXT_DELAY_LIMIT_UPPER; }	 
	   else
	    { console_test_delay = delay; }
	 }

	static inline int getConsoleTextDelayBoundsUpper( void )
	 { return( _GAME_CONFIG_CONSOLE_TEXT_DELAY_LIMIT_UPPER); }

	static inline int getConsoleTextDelayBoundsLower( void )
	 { return(_GAME_CONFIG_CONSOLE_TEXT_DELAY_LIMIT_LOWER); }

  //*********************************************************** 

	static inline void setPlayerRadarUnitColor( int color )
	 { radar_player_unit_color = color;	 }

	static inline PIX getPlayerRadarUnitColor( void )
	 { return( colorEnumToPix( radar_player_unit_color ) ); }

	static inline PIX getPlayerRadarUnitColorEnum( void )
	 { return( radar_player_unit_color ); }

   //*********************************************************** 

	static inline void setAlliedRadarUnitColor( int color )
	 { radar_allied_unit_color = color;	 }

	static inline PIX getAlliedRadarUnitColor( void )
	 { return( colorEnumToPix( radar_allied_unit_color ) ); }

	static inline PIX getAlliedRadarUnitColorEnum( void )
	 { return( radar_allied_unit_color ); }

   //*********************************************************** 

	static inline void setPlayerOutpostRadarColor( int color )
	 { radar_player_outpost_color = color;	 }

	static inline PIX getPlayerOutpostRadarColor( void )
	 { return( colorEnumToPix( radar_player_outpost_color ) ); }

	static inline PIX getPlayerOutpostRadarColorEnum( void )
	 { return( radar_player_outpost_color ); }

   //*********************************************************** 

	static inline void setAlliedOutpostRadarColor( int color )
	 { radar_allied_outpost_color = color;	 }

	static inline PIX getAlliedOutpostRadarColor( void )
	 { return( colorEnumToPix( radar_allied_outpost_color ) ); }

	static inline PIX getAlliedOutpostRadarColorEnum( void )
	 { return( radar_allied_outpost_color ); }

   //*********************************************************** 

	static inline void setEnemyOutpostRadarColor( int color )
	 { radar_enemy_outpost_color = color;	 }

	static inline PIX getEnemyOutpostRadarColor( void )
	 { return( colorEnumToPix( radar_enemy_outpost_color ) ); }

	static inline PIX getEnemyOutpostRadarColorEnum( void )
	 { return( radar_enemy_outpost_color ); }

   //*********************************************************** 

	static inline void setVehicleSelectionBoxColor( int color )
	 { vehicle_selection_box_color = color;	 }

	static inline PIX getVehicleSelectionBoxColor( void )
	 { return( colorEnumToPix( vehicle_selection_box_color ) ); }

	static inline PIX getVehicleSelectionBoxColorEnum( void )
	 { return( vehicle_selection_box_color ); }

   //*********************************************************** 

	static inline void setConsoleTextColor( int color )
	 { console_text_color = color;	 }

	static inline PIX getConsoleTextColor( void )
	 { return( colorEnumToPix( console_text_color ) ); }

	static inline PIX getConsoleTextColorEnum( void )
	 { return( console_text_color ); }

   //*********************************************************** 

    static inline void setDisplayCloudsOnRadarFlag( bool on_off )
	 { radar_display_clouds_flag = on_off; }

	static inline bool getDisplayCloudsOnRadarFlag( void )
	 { return( radar_display_clouds_flag ); }

	static inline char * getDisplayCloudsOnRadarFlagString( void )
	 { 
	   if ( radar_display_clouds_flag == true )
	    { return( "On" );  }
	   else
	    { return( "Off" ); }	
	 }

   //*********************************************************** 

    static inline void setDisplayShadowsFlag( bool on_off )
	 { display_shadows_flag = on_off; }

	static inline bool getDisplayShadowsFlag( void )
	 { return( display_shadows_flag ); }

	static inline char * getDisplayShadowsFlagString( void )
	 { 
	   if ( display_shadows_flag == true )
	    { return( "On" );  }
	   else
	    { return( "Off" ); }	
	 }

   //*********************************************************** 

    static inline void setDisplayUnitFlags( bool on_off )
	 { display_unit_flags = on_off; }

	static inline void toggleDisplayUnitFlags( void )
	 { display_unit_flags = !display_unit_flags; }

	static inline bool getDisplayUnitFlags( void )
	 { return( display_unit_flags ); }

   // ** Input Configuration Methods **

    static inline void setJoystickState( bool on_off )
	 { input_joystick_state = on_off; }

	static inline bool getJoystickState( void )
	 { return( input_joystick_state ); }

	static inline char * getJoystickStateString( void )
	 { 
	   if ( input_joystick_state == true )
	    { return( "On" );  }
	   else
	    { return( "Off" ); }	
	 }
	

   // ** Sound Configuration Methods **

    static inline void setSoundState( bool on_off )
	 { sound_on_off_flag = on_off; }

	static inline bool getSoundState( void )
	 { return( sound_on_off_flag ); }

	static inline char * getSoundStateString( void )
	 { 
	   if ( sound_on_off_flag == true )
	    { return( "On" );  }
	   else
	    { return( "Off" ); }	
	 }

  //*********************************************************** 
	static inline int getSoundVolume( void )
	 {
	  return( sound_volume );
	 } 

    static inline void setSoundVolume( int volume )
	 {
	  if ( volume < _GAME_CONFIG_SOUND_VOLUME_LIMIT_LOWER )
	   { sound_volume = _GAME_CONFIG_SOUND_VOLUME_LIMIT_LOWER; }
	  else
	   if ( volume > _GAME_CONFIG_SOUND_VOLUME_LIMIT_UPPER )
	    { sound_volume = _GAME_CONFIG_SOUND_VOLUME_LIMIT_UPPER; }	 
	   else
	    { sound_volume = volume; }
	 }

	static inline int getSoundVolumeBoundsUpper( void )
	 { return( _GAME_CONFIG_SOUND_VOLUME_LIMIT_UPPER); }

	static inline int getSoundVolumeBoundsLower( void )
	 { return(_GAME_CONFIG_SOUND_VOLUME_LIMIT_UPPER); }


	static inline void setGameScreenResolution( unsigned int game_res_enum )
	 {
	  if ( (game_res_enum >= 0) && (game_res_enum <= _game_config_standard_res_max) )
	   {
		screen_resolution_enum = game_res_enum;
	   }
	 }

    static inline void setNextGameScreenResolution(	void )
	 {
	  screen_resolution_enum = (screen_resolution_enum + 1) % _game_config_standard_res_max;
	 }

    static inline void setPreviousGameScreenResolution(	void )
	 {
	  screen_resolution_enum = (screen_resolution_enum - 1) % _game_config_standard_res_max;
	 }

	static inline char * getGameScreenResolutionString( void )
	 {
	  switch(screen_resolution_enum)
	   {
	   	case _game_config_standard_res_640x480 :
		 return( "640x480" );
		break;

		case _game_config_standard_res_800x600 :
		 return( "800x600" );
		break;

		case _game_config_standard_res_1024x768 :
		 return( "1024x768" );		
		break;
	   } 
	 
	  return( "Invalid Mode" );
	 }

	static inline PointXYi getGameScreenResolutionSize( void )
	 {
	  switch(screen_resolution_enum)
	   {
	   	case _game_config_standard_res_640x480 :
		 return( PointXYi( 640, 480) );
		break;

		case _game_config_standard_res_800x600 :
		 return( PointXYi( 800, 600) );
		break;

		case _game_config_standard_res_1024x768 :
		 return( PointXYi(1024, 768) );		
		break;
	   } 
	 
      return( PointXYi( 640, 480) );
	 }

   //*********************************************************** 
	 static inline void increaseAttackNotificationTime()
	 {
		if (++attackNotificationTime > _GAME_CONFIG_ATTACK_NOTIFICATION_TIME_LIMIT_UPPER)
		{
			attackNotificationTime = _GAME_CONFIG_ATTACK_NOTIFICATION_TIME_LIMIT_UPPER;
		}

	 }
	 static inline void decreaseAttackNotificationTime()
	 {
		if (--attackNotificationTime < _GAME_CONFIG_ATTACK_NOTIFICATION_TIME_LIMIT_LOWER)
		{
			attackNotificationTime = _GAME_CONFIG_ATTACK_NOTIFICATION_TIME_LIMIT_LOWER;
		}

	 }

	 static inline int getAttackNotificationTime()
	 {
		return attackNotificationTime;
	 }

   //*********************************************************** 

	 static inline void setBlendSmokeTrue()
	 { blendSmoke = true; }
	 
	 static inline void setBlendSmokeFalse()
	 { blendSmoke = false; }
	 
	 static inline bool getBlendSmoke()
	 { return( blendSmoke ); }
	 
	 static inline const char *getBlendSmokeString()
	 {
		if (blendSmoke == true)
		{
			return "On";
		}

		return "Off";
	 }

   //*********************************************************** 

	static inline void setNextMiniMapUnitSize( void )
	 {
	   if (++mini_map_unit_size >= _mini_map_unit_size_count)
	   {
	     mini_map_unit_size = 0;
	   }
	 }

	static inline void setPreviousMiniMapUnitSize( void )
	 {
	   if (--mini_map_unit_size < 0)
	   {
	     mini_map_unit_size = _mini_map_unit_size_count - 1;
	   }
	 }

	static inline void setMiniMapUnitSize( int enumValue )
	 {
	   assert(enumValue < _mini_map_unit_size_count);

	   mini_map_unit_size = enumValue;
	 }

	static inline int getMiniMapUnitSize( void )
	 { return( mini_map_unit_size ); }

	static inline const char *getMiniMapUnitSizeString( void )
	 {
	   if (mini_map_unit_size == _mini_map_unit_size_small) { return( "Small" );}
	   else if (mini_map_unit_size == _mini_map_unit_size_large) { return( "Large" );}
	   else assert(false);
	   return( "Undefined value" );
	 }

   //*********************************************************** 

	static inline void setNextUnitSelectionBoxDrawMode( void )
	 {
	   if (++unit_selection_box_draw_mode >= _unit_selection_box_draw_mode_count)
	   {
	     unit_selection_box_draw_mode = 0;
	   }
	 }

	static inline void setPreviousUnitSelectionBoxDrawMode( void )
	 {
	   if (--unit_selection_box_draw_mode < 0)
	   {
	     unit_selection_box_draw_mode = _unit_selection_box_draw_mode_count - 1;
	   }
	 }

	static inline void setUnitSelectionBoxDrawMode( int enumValue )
	 {
	   assert(enumValue < _unit_selection_box_draw_mode_count);

	   unit_selection_box_draw_mode = enumValue;
	 }

	static inline int getUnitSelectionBoxDrawMode( void )
	 { return( unit_selection_box_draw_mode ); }

	static inline const char *getUnitSelectionBoxDrawModeString( void )
	 {
	   if (unit_selection_box_draw_mode == _unit_selection_box_draw_mode_rect) { return( "Rectangle" );}
	   else if (unit_selection_box_draw_mode == _unit_selection_box_draw_mode_rect_edges) { return( "Rectangle Edges" );}
	   else assert(false);
	   return( "Undefined value" );
	 }

   //*********************************************************** 

	static inline void setDrawUnitDamage( int value )
	 { draw_unit_damage = value;	 }

	static inline void toggleDrawUnitDamage( void )
	 { draw_unit_damage = !draw_unit_damage;	 }

	static inline bool getDrawUnitDamage( void )
	 { return( draw_unit_damage ); }

   //*********************************************************** 

	static inline void setDrawUnitReload( int value )
	 { draw_unit_reload = value;	 }

	static inline void toggleDrawUnitReload( void )
	 { draw_unit_reload = !draw_unit_reload;	 }

	static inline bool getDrawUnitReload( void )
	 { return( draw_unit_reload ); }

   //*********************************************************** 

	static inline void setNextMiniMapObjectiveDrawMode( void )
	 {
	   if (++mini_map_objective_draw_mode >= _mini_map_objective_draw_mode_count)
	   {
	     mini_map_objective_draw_mode = 0;
	   }
	 }

	static inline void setMiniMapObjectiveDrawMode( int enumValue )
	 {
	   assert(enumValue < _mini_map_objective_draw_mode_count);

	   mini_map_objective_draw_mode = enumValue;
	 }

	static inline void setPreviousMiniMapObjectiveDrawMode( void )
	 {
	   if (--mini_map_objective_draw_mode < 0)
	   {
	     mini_map_objective_draw_mode = _mini_map_objective_draw_mode_count - 1;
	   }
	 }

	static inline int getMiniMapObjectiveDrawMode( void )
	 { return( mini_map_objective_draw_mode ); }

	static inline const char *getMiniMapObjectiveDrawModeString( void )
	 {
	   if (mini_map_objective_draw_mode == _mini_map_objective_draw_mode_solid_rect) { return( "Solid Rectangle" );}
	   else if (mini_map_objective_draw_mode == _mini_map_objective_draw_mode_outline_rect) { return( "Outlined Rectangle" );}
	   //else if (mini_map_objective_draw_mode == _mini_map_objective_draw_mode_player_flag) { return( "Player Flag" );}
	   else assert(false);
	   return( "Undefined value" );
	 }

   //*********************************************************** 
	 static inline void setUnitInfoDrawLayerTop()
	 { unitInfoDrawLayer = 7; }

	 static inline void setUnitInfoDrawLayerBottom()
	 { unitInfoDrawLayer = 0; }

	 static inline void toggleUnitInfoDrawLayer()
	 { unitInfoDrawLayer = (( unitInfoDrawLayer == 7 ) ? 0 : 7); }

	 static inline int getUnitInfoDrawLayer()
	 { return( unitInfoDrawLayer ); }

	 static inline const char *getUnitInfoDrawLayerString()
	 {
	   if ( unitInfoDrawLayer == 0 ) { return( "Bottom" ); }
	   else if ( unitInfoDrawLayer == 7 ) { return( "Top" ); }
	   return "Undefined value";
	 }
  
  static void loadConfigScript( void );
};

#endif // ** __GAMECONFIG_HPP
