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

#include "FileSystem.hpp"
#include "Log.hpp"
#include "Exception.hpp"
#include "GameConfig.hpp"
#include "XmlConfig.hpp"
#include "XmlStore.hpp"

std::string GameConfig::configfile;
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

bool      GameConfig::map_cycling_on_off = false;
bool      GameConfig::powerups_on_off = false;

float        GameConfig::objective_occupation_percentage = 100.0;
bool      GameConfig::allow_allies_on_off = true;
int          GameConfig::cloud_coverage;
float        GameConfig::wind_speed;
unsigned int GameConfig::respawn_type = _game_config_respawn_type_round_robin;

char    GameConfig::game_map_name[256];

// ** Visuals Configuration **
unsigned int GameConfig::screen_resolution = 0;
bool GameConfig::screen_fullscreen = true;

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

PlayerUnitConfig GameConfig::unit_spawn_config;

int     GameConfig::attackNotificationTime = 5;
bool GameConfig::blendSmoke             = true;


void GameConfig::initialize(const char* newconfigfile)
{
    configfile = newconfigfile;
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

    try {
        loadConfig();
    } catch(Exception e) {
        LOG(("couldn't read game configuration: %s", e.getMessage()));
    }
}

void GameConfig::shutdown()
{
    try {
        saveConfig();
    } catch(Exception e) {
        LOG(("couldn't save game configuration: %s", e.getMessage()));
    }
}

void GameConfig::loadConfig()
{
    const char *xmlfile = FileSystem::getRealName(configfile.c_str()).c_str();
    XmlConfig config(xmlfile);

    int configversion = config.readInt("version");
    if (configversion != CONFIG_VERSION) {
        throw Exception("wrong config file version, %d!=%d",
                configversion, CONFIG_VERSION);
    }

    XmlConfig game = config.getChild("game");
    GameMode = game.readInt("mode");
    GameType = game.readInt("type");
    NumberPlayers = game.readInt("players");
    NumberUnits = game.readInt("units");
    NumberInitialUnits = game.readInt("init_units");

    XmlConfig visuals = config.getChild("visuals");
    screen_resolution = visuals.readInt("resolution");
    screen_fullscreen = visuals.readInt("fullscreen");
    display_shadows_flag = visuals.readInt("shadows_flag");
    display_unit_flags = visuals.readInt("unit_flags");
    UnitColor = visuals.readInt("unit_color");
}

void GameConfig::saveConfig()
{
    XmlStore xmlStore("netpanzer");
    xmlStore.writeInt("version", CONFIG_VERSION);

    XmlStore game = xmlStore.createChild("game");
    game.writeInt("mode", GameMode);
    game.writeInt("type", GameType);
    game.writeInt("players", NumberPlayers);
    game.writeInt("units", NumberUnits);
    game.writeInt("init_units", NumberInitialUnits);

    XmlStore visuals = xmlStore.createChild("visuals");
    visuals.writeInt("resolution", screen_resolution);
    visuals.writeInt("fullscreen", screen_fullscreen);
    visuals.writeInt("shadows_flag", display_shadows_flag);
    visuals.writeInt("unit_flags", display_unit_flags);
    visuals.writeInt("unit_color", UnitColor);

    xmlStore.save(configfile.c_str());
}

void GameConfig::setGameMapName( char *map_name )
{
    strcpy( game_map_name, map_name );
}

char * GameConfig::getGameMapName( void )
{
    return( game_map_name );
}
