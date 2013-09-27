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

#include <fstream>
#include <sstream>

#include "Util/FileSystem.hpp"
#include "Util/FileStream.hpp"
#include "Util/Log.hpp"
#include "Util/Exception.hpp"
#include "Interfaces/GameConfig.hpp"
#include "Core/NetworkGlobals.hpp"
#include "Views/Game/MiniMapView.hpp"
#include "Views/GameViewGlobals.hpp"

#include "json/json.h"

int       GameConfig::game_autokicktime = 20; // minutes;
bool      GameConfig::game_allowmultiip = true;
NPString* GameConfig::game_unit_profiles = 0;
NPString* GameConfig::game_unit_spawnlist = 0;
NPString* GameConfig::game_adminpass = 0;
NPString* GameConfig::game_gamepass = 0;
int       GameConfig::game_changeflagtime = 15; // minutes
int       GameConfig::game_gametype = 0;
int       GameConfig::game_maxplayers = 8;
int       GameConfig::game_maxunits = 8*40;
int       GameConfig::game_timelimit = 30;
int       GameConfig::game_fraglimit = 300;
bool      GameConfig::game_powerups = true;
bool      GameConfig::game_teammode = false;
NPString* GameConfig::game_team_names = 0;
int       GameConfig::game_occupationpercentage = 75;
bool      GameConfig::game_allowallies = true;
int       GameConfig::game_cloudcoverage = 0;
int       GameConfig::game_respawntype = 0;
int       GameConfig::game_windspeed = 30;
NPString* GameConfig::game_map = 0;
NPString* GameConfig::game_mapcycle = 0;

NPString* GameConfig::player_name = 0;

Uint8 GameConfig::player_flag_data[FLAG_WIDTH*FLAG_HEIGHT] = {0};


GameConfig::GameConfig(const std::string& configfile,bool usePhysFS)
    // VariableName("Name", value [, minimum, maximum])
    :
      hostorjoin(_game_session_join),
      quickConnect(false),
      needPassword(false),
      serverConnect("")
{
    this->configfile = configfile;
    this->usePhysFS = usePhysFS;
    masterservers.push_back("masterserver.netpanzer.org");
    masterservers.push_back("masterserver.netpanzer.info");

//    std::stringstream default_player;
//    default_player << "Player" << (rand()%1000);
//    if ( ! player_name )
//    {
//        player_name = new NPString();
//    }
//
//    player_name->assign(default_player.str());
    
    try
    {
        loadConfig();
    }
    catch(std::exception& e)
    {
        LOG(("couldn't read game configuration: %s", e.what()));
        LOG(("Using default config. (this is normal on first startup)"));
    }
}

GameConfig::~GameConfig()
{
    try
    {
        saveConfig();
    }
    catch(std::exception& e)
    {
        LOG(("couldn't save game configuration: %s", e.what()));
    }
}

void GameConfig::loadConfig()
{
    // @todo use "configfile"
    IFileStream ifile("config/client.json");
    if ( ifile.good() )
    {
        Json::Value root;
        Json::Reader reader;
        if ( ! reader.parse(ifile, root, true) )
        {
            LOGGER.warning("Error loading config file:\n%s", reader.getFormattedErrorMessages().c_str());
            return;
        }
        
        if ( root.isMember("video") ) video.load(root["video"]);
        if ( root.isMember("sound") ) sound.load(root["sound"]);
        if ( root.isMember("interface") ) gameinterface.load(root["interface"]);
    }
    

// these lines might be usefull infuture? 2012-01-18
//    if(usePhysFS)
//    {
//        IFileStream in(configfile);
//        inifile.load(in);
//    } else {
//        std::ifstream in(configfile.c_str());
//        inifile.load(in);
//    }

//    if ( player_name->length() == 0 )
//    {
//        std::stringstream default_player;
//        default_player << "Player" << (rand()%1000);
//        player_name->assign(default_player.str());
//    }
//
//    if ( game_mapcycle->length() == 0 )
//    {
//        game_mapcycle->assign("Two clans");
//    }
}

void GameConfig::saveConfig()
{
    // @todo use "configfile"
    Json::Value root(Json::objectValue);
    
    Json::Value videoNode(Json::objectValue);
    video.save(videoNode);
    
    Json::Value soundNode(Json::objectValue);
    sound.save(soundNode);

    Json::Value interfaceNode(Json::objectValue);
    gameinterface.save(interfaceNode);

    root["video"] = videoNode;
    root["sound"] = soundNode;
    root["interface"] = interfaceNode;

    OFileStream jsout("config/client.json");
    Json::StyledStreamWriter writer;
    writer.write(jsout, root);
}

GameConfig* gameconfig = 0;
