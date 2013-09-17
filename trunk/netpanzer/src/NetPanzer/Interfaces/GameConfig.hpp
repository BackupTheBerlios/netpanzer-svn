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

#include "Core/CoreTypes.hpp"
#include "Config/VideoConfig.hpp"
#include "Config/HostConfig.hpp"
#include "Config/InterfaceConfig.hpp"
#include "Config/SoundConfig.hpp"

#include "2D/Surface.hpp"
#include "2D/Color.hpp"
#include "Util/NoCopy.hpp"
#include "Classes/PlayerUnitConfig.hpp"

#define DEFAULT_UNIT_PROFILES "Manta, Panther1, Titan, Stinger, Bobcat, Bear, Archer, Wolf, Drake, Spanzer"

enum { _game_session_host,
       _game_session_join,
       _game_session_last
     };

enum { _gametype_objective,
       _gametype_fraglimit,
       _gametype_timelimit,
       _gametype_last
     };

enum { _game_config_respawn_type_round_robin,
       _game_config_respawn_type_random,
       _game_config_respawn_type_last
     };

class GameConfig : public NoCopy
{
public:
    GameConfig(const std::string& luaconfigfile, bool usePhysFS = true);
    ~GameConfig();

    void loadConfig();
    void saveConfig();

    // client only config
    VideoConfig video;
    InterfaceConfig gameinterface;
    SoundConfig sound;
    
    static bool      game_enable_bases;
    static int       game_base_capture_mode; // 0=no capture, 1=normal, 2=...
    static int       game_base_limit;  // 0=no limit, other number max bases per player
    static int       game_autokicktime; // in minutes
    static bool      game_allowmultiip; // allow multi connect from same ip
    static NPString* game_unit_profiles; // "," or space separated list of profiles to load
    static NPString* game_unit_spawnlist; // "," or space separated list with the numbers of each unit to spawn with
    static NPString* game_adminpass;     // the secret password for admins
    static NPString* game_gamepass;      // the secret password for entering game
    static int       game_changeflagtime; // in minutes
    static int       game_gametype;
    static int       game_maxplayers;
    static int       game_maxunits;
    static int       game_timelimit;
    static int       game_fraglimit;
    static bool      game_powerups;
    static bool      game_teammode;
    static NPString* game_team_names;
    static int       game_occupationpercentage;
    static bool      game_allowallies;
    static int       game_cloudcoverage;
    static int       game_respawntype;
    static int       game_windspeed;
    static NPString* game_map;
    static NPString* game_mapcycle;

    static Uint8 player_flag_data[FLAG_WIDTH*FLAG_HEIGHT];

    // game Settings (there are not saved to disk)
    int      hostorjoin;         // 1=host, 2=join
    bool     quickConnect;
    bool     needPassword;
    NPString serverConnect;      // server to connect to

    static NPString* player_name;

    HostConfig host;

    static NPString* server_masterservers;

public:
    const char* getGameTypeString() const
    {
        switch ( game_gametype )
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


    int GetUnitsPerPlayer() const
    {
        return game_maxunits / game_maxplayers;
    }

    const char * getRespawnTypeString() const
    {
        switch( game_respawntype )
        {
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

private:
    NPString configfile;
    bool usePhysFS;

};

extern GameConfig* gameconfig;

#endif // ** __GAMECONFIG_HPP
