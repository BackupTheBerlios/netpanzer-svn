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

#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <string>

#include "Surface.hpp"
#include "Color.hpp"
#include "NoCopy.hpp"
#include "ConfigVariable.hpp"
#include "PlayerUnitConfig.hpp"

enum { _mini_map_objective_draw_mode_solid_rect,
       _mini_map_objective_draw_mode_outline_rect,
       _mini_map_objective_draw_mode_last
     };

enum { _mini_map_unit_size_small,
       _mini_map_unit_size_large,
       _mini_map_unit_size_last
     };

enum { _unit_selection_box_draw_mode_rect,
       _unit_selection_box_draw_mode_rect_edges,
       _unit_selection_box_draw_mode_last
     };

enum { _gamemode_skirmish,
       _gamemode_multiplayer
     };

enum { _game_session_host,
       _game_session_join,
       _game_session_last
     };

enum { _gametype_objective,
       _gametype_fraglimit,
       _gametype_timelimit,
       _gametype_last
     };

enum { _connection_tcpip,
       _connection_direct_connect,
       _connection_modem
     };

enum { _game_config_respawn_type_round_robin,
       _game_config_respawn_type_random,
       _game_config_respawn_type_last
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
       _color_last
     };

class XmlConfig;
class XmlStore;

class GameConfig : public NoCopy
{
public:
    GameConfig(const std::string& configfile);
    ~GameConfig();

    void loadConfig();
    void saveConfig();

    // game Settings
    ConfigInt hostorjoin;           //1=host, 2=join

    // player settings
    ConfigString playername;

    // server settings
    ConfigInt   gametype;             //Objectives, FragLimit, TimeLimit
    ConfigInt   maxplayers;
    ConfigInt   maxunits;
    ConfigInt   initialunits;
    ConfigInt   timelimit;
    ConfigInt   fraglimit;
    ConfigBool  mapcycling;
    ConfigBool  powerups;
    ConfigInt   objectiveoccupationpercentage;
    ConfigBool  allowallies;
    ConfigInt   cloudcoverage;
    ConfigInt   respawntype;
    ConfigInt   windspeed;
    ConfigString map;

    // Visuals Settings
    ConfigInt   screenresolution;
    ConfigBool  fullscreen;
    ConfigBool  displayshadows;
    ConfigBool  blendsmoke;
    ConfigInt   screengamma;
   
    // interface settings
    ConfigInt   unitcolor;
    ConfigInt   playerflag;
    ConfigInt   attacknotificationtime;
    ConfigInt   vehicleselectioncolor;
    ConfigInt   consoletextcolor;
    ConfigInt   unitselectionmode;
    ConfigInt   unitinfodrawlayer;
    ConfigBool  drawunitdamage;
    ConfigBool  drawunitreload;
    ConfigBool  drawunitflags;
    ConfigInt   consoletextdelay;
    ConfigInt   consoletextusage;
    ConfigInt   scrollrate;

    // radar settings
    ConfigBool  radar_displayclouds;
    ConfigInt   radar_playerunitcolor;
    ConfigInt   radar_selectedunitcolor;
    ConfigInt   radar_alliedunitcolor;
    ConfigInt   radar_playeroutpostcolor;
    ConfigInt   radar_alliedoutpostcolor;
    ConfigInt   radar_enemyoutpostcolor;
    ConfigInt   radar_unitsize;
    ConfigInt   radar_objectivedrawmode;
    ConfigInt   radar_resizerate;

    PlayerUnitConfig unit_spawn_config;
    
public:
    const char* getGameTypeString() const
    {
        switch(gametype) {
        case _gametype_objective :
            return( "Objective" );
        case _gametype_fraglimit :
            return( "Frag Limit" );
        case _gametype_timelimit :
            return( "Time Limit" );
        }
        return( "Unknown" );
    }

    int GetUnitsPerPlayer() const
    {
        return maxunits / maxplayers;
    }

    int GetTimeLimitSeconds() const
    {
        return timelimit * 60;
    }

    const char * getRespawnTypeString() const
    {
        switch( respawntype ) {
        case _game_config_respawn_type_round_robin :
            return( "Round Robin" );
            break;

        case _game_config_respawn_type_random :
            return( "Random" );
            break;
        } // ** switch

        assert(false);
        return( "Unknown" );
    }

    PIX getUnitColor() const
    {
        return colorEnumToPix(unitcolor);
    }

    PIX getPlayerRadarUnitColor() const
    {
        return( colorEnumToPix( radar_playerunitcolor ) );
    }

    PIX getSelectedRadarUnitColor() const
    {
        return( colorEnumToPix( radar_selectedunitcolor ) );
    }
    
    PIX getAlliedRadarUnitColor() const
    {
        return( colorEnumToPix( radar_alliedunitcolor ) );
    }

    PIX getPlayerOutpostRadarColor() const
    {
        return( colorEnumToPix( radar_playeroutpostcolor ) );
    }

    PIX getAlliedOutpostRadarColor() const
    {
        return( colorEnumToPix( radar_alliedoutpostcolor ) );
    }
    
    PIX getEnemyOutpostRadarColor() const
    {
        return( colorEnumToPix( radar_enemyoutpostcolor ) );
    }
    
    PIX getVehicleSelectionBoxColor() const
    {
        return( colorEnumToPix( vehicleselectioncolor ) );
    }

    PIX getConsoleTextColor() const
    {
        return( colorEnumToPix( consoletextcolor ) );
    }

    const char *getMiniMapUnitSizeString() const
    {
        if (radar_unitsize == _mini_map_unit_size_small) {
            return( "Small" );
        } else if (radar_unitsize == _mini_map_unit_size_large) {
            return( "Large" );
        } else assert(false);
        return( "Undefined value" );
    }

private:
    std::string configfile;

    PIX colorEnumToPix(int color_enum) const
    {
        switch( color_enum ) {
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

    void loadSettings(XmlConfig& config, const char* name,
            std::vector<ConfigVariable*>& settings);
    void saveSettings(XmlStore& config, const char* name,
            std::vector<ConfigVariable*>& settings);

    std::vector<ConfigVariable*> gamesettings;
    std::vector<ConfigVariable*> playersettings;
    std::vector<ConfigVariable*> serversettings;
    std::vector<ConfigVariable*> visualssettings;
    std::vector<ConfigVariable*> interfacesettings;
    std::vector<ConfigVariable*> radarsettings;    
};

extern GameConfig* gameconfig;

#endif // ** __GAMECONFIG_HPP
