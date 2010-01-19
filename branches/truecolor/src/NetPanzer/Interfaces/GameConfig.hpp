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
#include <vector>
#include <string>

#include "2D/Surface.hpp"
#include "2D/Color.hpp"
#include "Util/NoCopy.hpp"
#include "ConfigVariable.hpp"
#include "Classes/PlayerUnitConfig.hpp"

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
       _game_config_standard_res_1280x1024,
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

namespace INI {
    class Section;
}

class GameConfig : public NoCopy
{
public:
    GameConfig(const std::string& configfile, bool usePhysFS = true);
    ~GameConfig();

    void loadConfig();
    void saveConfig();

    static unsigned int video_width;
    static unsigned int video_height;
    static bool         video_fullscreen;
    static bool         video_hardwaresurface;
    static bool         video_doublebuffer;
    static bool         video_shadows;
    static bool         video_blendsmoke;
#ifdef _WIN32
    static bool         video_usedirectx;
#endif

    // game Settings (there are not saved to disk)
    ConfigInt       hostorjoin;         // 1=host, 2=join
    ConfigBool      quickConnect;
    ConfigString    serverConnect;      // server to connect to

    // player settings
    ConfigStringSpecialChars playername;
    ConfigString masterservers;

    // server settings
    ConfigInt    serverport;
    ConfigString proxyserver;
    ConfigString proxyserveruser;
    ConfigString proxyserverpass;
    ConfigString bindaddress;
    ConfigInt    gametype;             //Objectives, FragLimit, TimeLimit
    ConfigInt    maxplayers;
    ConfigInt    maxunits;
    ConfigInt    timelimit;
    ConfigInt    fraglimit;
    ConfigBool   powerups;
    ConfigInt    objectiveoccupationpercentage;
    ConfigBool   allowallies;
    ConfigInt    cloudcoverage;
    ConfigInt    windspeed;
    ConfigString map;
    ConfigString mapcycle;
    ConfigString motd;
    ConfigBool   logging;
    ConfigBool   publicServer;
    ConfigBool   capturebases;
    ConfigInt    respawntype; // this is round robin or random already in np
    ConfigInt    respawnmode; // 0- normal, 1- round, 2- timer
    ConfigInt    respawntime; // used in timer mode. in seconds

    // sound settings
    ConfigBool  enablesound;
    ConfigBool  enablemusic;
    ConfigInt   musicvolume;
    ConfigBool  enableeffects;
    ConfigInt   effectsvolume;
   
    // interface settings
    ConfigInt   unitcolor;
    ConfigString   playerflag;
    ConfigInt   attacknotificationtime;
    ConfigInt   vehicleselectioncolor;
    ConfigInt   unitselectionmode;
    ConfigInt   unitinfodrawlayer;
    ConfigBool  drawunitdamage;
    ConfigBool  drawunitreload;
    ConfigBool  drawunitflags;
    ConfigBool  drawunitowner;
    ConfigInt   consoletextdelay;
    ConfigInt   consoletextusage;
    ConfigInt   scrollrate;
    ConfigXY    minimapposition;
    ConfigXY    minimapsize;
    ConfigInt   minimapdrawtype;
    ConfigXY    gameinfoposition;
    ConfigXY    toolbarposition;
    ConfigXY    rankposition;
    ConfigInt   viewdrawbackgroundmode;

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
    
    // spawnconfig settings
    ConfigInt   titan;
    ConfigInt   manta;
    ConfigInt   panther1;
    ConfigInt   stinger;
    ConfigInt   spanzer;
    ConfigInt   bobcat;
    ConfigInt   wolf;
    ConfigInt   bear;
    ConfigInt   drake;
    ConfigInt   archer;

    // quick chat settings
    ConfigString quickchat_1;
    ConfigString quickchat_2;
    ConfigString quickchat_3;
    ConfigString quickchat_4;
    ConfigString quickchat_5;
    ConfigString quickchat_6;
    ConfigString quickchat_7;
    ConfigString quickchat_8;
    ConfigString quickchat_9;
    ConfigString quickchat_0;

    std::vector<ConfigVariable*> spawnsettings;
    std::vector<ConfigVariable*> quickchatsettings;
    
public:
    void clearSpawnSettings()
    {
        std::vector<ConfigVariable*>::iterator i = spawnsettings.begin();
        while ( i != spawnsettings.end() )
        {
            delete *i;
            i++;
        }
        spawnsettings.clear();
    }
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

    IntColor getUnitColor() const
    {
        return colorEnumToPix(unitcolor);
    }

    IntColor getPlayerRadarUnitColor() const
    {
        return( colorEnumToPix( radar_playerunitcolor ) );
    }

    IntColor getSelectedRadarUnitColor() const
    {
        return( colorEnumToPix( radar_selectedunitcolor ) );
    }
    
    IntColor getAlliedRadarUnitColor() const
    {
        return( colorEnumToPix( radar_alliedunitcolor ) );
    }

    IntColor getPlayerOutpostRadarColor() const
    {
        return( colorEnumToPix( radar_playeroutpostcolor ) );
    }

    IntColor getAlliedOutpostRadarColor() const
    {
        return( colorEnumToPix( radar_alliedoutpostcolor ) );
    }
    
    IntColor getEnemyOutpostRadarColor() const
    {
        return( colorEnumToPix( radar_enemyoutpostcolor ) );
    }
    
    IntColor getVehicleSelectionBoxColor() const
    {
        return( colorEnumToPix( vehicleselectioncolor ) );
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
    friend class ScriptManager;
    static void registerScript(const char * table_name);
    std::string configfile;
    bool usePhysFS;

    IntColor colorEnumToPix(int color_enum) const
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

    void loadSettings(const INI::Section&,
            std::vector<ConfigVariable*>& settings);
    void loadSpawnSettings(const INI::Section& section,
                           std::vector<ConfigVariable*>& settings);
    void saveSettings(INI::Section&, std::vector<ConfigVariable*>& settings);

    std::vector<ConfigVariable*> gamesettings;
    std::vector<ConfigVariable*> playersettings;
    std::vector<ConfigVariable*> serversettings;
    std::vector<ConfigVariable*> soundsettings;
    std::vector<ConfigVariable*> interfacesettings;
    std::vector<ConfigVariable*> radarsettings;
};

extern GameConfig* gameconfig;

#endif // ** __GAMECONFIG_HPP
