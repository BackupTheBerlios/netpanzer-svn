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

GameConfig::GameConfig(const std::string& newconfigfile)
    // VariableName("Name", value [, minimum, maximum])
    : hostorjoin("hostorjoin", _game_session_join, 0, _game_session_last-1),
      playername("name", "Player"),

      gametype("gametype", _gametype_objective, 0, _gametype_last-1),
      maxplayers("maxplayers", 8, 1, 25),
      maxunits("maxunits", 500, 25, 10000),
      initialunits("initialunits", 5, 0, 100),
      timelimit("timelimit", 50, 1),
      fraglimit("fraglimit", 500, 50),
      mapcycling("mapcycling", false),
      powerups("powerups", true),
      objectiveoccupationpercentage("objectivepercentage", 100, 1, 100),
      allowallies("allowallies", true),
      cloudcoverage("cloudcoverage", 0),
      respawntype("respawntype", _game_config_respawn_type_random, 0,
                _game_config_respawn_type_last-1),
      windspeed("windspeed", 30),
      map("map", "Bad Neuburg"),
      
      screenresolution("resolution", 0, 0, 2),
      fullscreen("fullscreen", true),
      displayshadows("displayshadows", true),
      blendsmoke("blendsmoke", true),
      screengamma("gamma", 50, 0, 100),

      unitcolor("unitcolor", 0, 0, _color_last-1),
      playerflag("playerflag", 0, 0, 100),
      attacknotificationtime("attacknotificationtime", 5, 0, 100),
      vehicleselectioncolor("vehicleselectioncolor", _color_blue, 0, _color_last-1),
      consoletextcolor("consoletextcolor", _color_white, 0, _color_last-1),
      unitselectionmode("unitselectionmode", _unit_selection_box_draw_mode_rect_edges, 0, _unit_selection_box_draw_mode_last-1),
      unitinfodrawlayer("unitinfodrawlayer", 0, 0, 1),
      drawunitdamage("drawunitdamage", true),
      drawunitreload("drawunitreload", false),
      drawunitflags("drawunitflags", true),
      consoletextdelay("consoletextdelay", 3, 1, 20),
      consoletextusage("consoletextusage", 25, 1, 100),
      scrollrate("scrollrate", 1000, 100, 10000),
                  
      radar_displayclouds("displayclouds", false),
      radar_playerunitcolor("playerunitcolor", _color_aqua, 0, _color_last-1),
      radar_selectedunitcolor("selectedunitcolor", _color_white, 0, _color_last-1),
      radar_alliedunitcolor("alliedunitcolor", _color_orange, 0, _color_last-1),
      radar_playeroutpostcolor("playeroutpostcolor", _color_blue, 0, _color_last-1),
      radar_alliedoutpostcolor("alliedoutpostcolor", _color_orange, 0, _color_last),
      radar_enemyoutpostcolor("enemyoutpostcolor", _color_red, 0, _color_last-1),
      radar_unitsize("unitsize", _mini_map_unit_size_small, 0, _mini_map_unit_size_last-1),
      radar_objectivedrawmode("objectivedrawmode", _mini_map_objective_draw_mode_outline_rect, 0, _mini_map_objective_draw_mode_last-1),
      radar_resizerate("resizerate", 400, 10, 1000)      
{
    configfile = newconfigfile;

    //gamesettings.push_back(&hostorjoin);

    playersettings.push_back(&playername);

    serversettings.push_back(&gametype);
    serversettings.push_back(&maxplayers);
    serversettings.push_back(&maxunits);
    serversettings.push_back(&initialunits);
    serversettings.push_back(&timelimit);
    serversettings.push_back(&fraglimit);
    serversettings.push_back(&mapcycling);
    serversettings.push_back(&powerups);
    serversettings.push_back(&objectiveoccupationpercentage);
    serversettings.push_back(&allowallies);
    serversettings.push_back(&cloudcoverage);
    serversettings.push_back(&respawntype);
    serversettings.push_back(&windspeed);
    serversettings.push_back(&map);
   
    visualssettings.push_back(&screenresolution);
    visualssettings.push_back(&fullscreen);
    visualssettings.push_back(&displayshadows);
    visualssettings.push_back(&blendsmoke);
    visualssettings.push_back(&screengamma);

    interfacesettings.push_back(&unitcolor);
    interfacesettings.push_back(&playerflag);
    interfacesettings.push_back(&attacknotificationtime);
    interfacesettings.push_back(&vehicleselectioncolor);
    interfacesettings.push_back(&consoletextcolor);
    interfacesettings.push_back(&unitselectionmode);
    interfacesettings.push_back(&unitinfodrawlayer);
    interfacesettings.push_back(&drawunitdamage);
    interfacesettings.push_back(&drawunitreload);
    interfacesettings.push_back(&drawunitflags);
    interfacesettings.push_back(&consoletextdelay);
    interfacesettings.push_back(&consoletextusage);
    interfacesettings.push_back(&scrollrate);

    radarsettings.push_back(&radar_displayclouds);
    radarsettings.push_back(&radar_playerunitcolor);
    radarsettings.push_back(&radar_alliedunitcolor);
    radarsettings.push_back(&radar_playeroutpostcolor);
    radarsettings.push_back(&radar_alliedoutpostcolor);
    radarsettings.push_back(&radar_enemyoutpostcolor);
    radarsettings.push_back(&radar_unitsize);
    radarsettings.push_back(&radar_objectivedrawmode);
    radarsettings.push_back(&radar_resizerate);
    
    try {
        loadConfig();
    } catch(Exception e) {
        LOG(("couldn't read game configuration: %s", e.what()));
    }
}

GameConfig::~GameConfig()
{
    try {
        saveConfig();
    } catch(Exception e) {
        LOG(("couldn't save game configuration: %s", e.what()));
    }
}

void GameConfig::loadConfig()
{
    const char *xmlfile = FileSystem::getRealName(configfile.c_str()).c_str();
    XmlConfig config(xmlfile);

    loadSettings(config, "game", gamesettings);
    loadSettings(config, "player", playersettings);
    loadSettings(config, "visuals", visualssettings);
    loadSettings(config, "interface", interfacesettings);
    loadSettings(config, "radar", radarsettings);
    loadSettings(config, "server", serversettings);
}

void GameConfig::loadSettings(XmlConfig& config, const char* name,
        std::vector<ConfigVariable*>& settings)
{
    try {
        XmlConfig section = config.getChild(name);

        std::vector<ConfigVariable*>::iterator i;
        for(i = settings.begin(); i != settings.end(); i++) {
            ConfigVariable* var = *i;

            try {
                ConfigInt* confint = dynamic_cast<ConfigInt*> (var);
                if(confint)
                    *confint = section.readInt(confint->getName().c_str());
                ConfigBool* confbool = dynamic_cast<ConfigBool*> (var);
                if(confbool) {
                    std::string str =
                        section.readString(confbool->getName().c_str());
                    if(str == "yes" || str == "1" || str == "on")
                        *confbool = true;
                    else if(str == "no" || str == "0" || str == "off")
                        *confbool = false;
                    else
                        throw Exception("No boolean value for setting '%s'.",
                                        confbool->getName().c_str());
                }
                ConfigString* confstring = dynamic_cast<ConfigString*> (var);
                if(confstring)
                    *confstring =
                        section.readString(confstring->getName().c_str());
            } catch(Exception& e) {
                LOG(("Skipping config '%s': %s", var->getName().c_str(),
                            e.what()));
            }
        }
    } catch(Exception& e) {
        LOG(("Couldn't find config section '%s', skipping...", name));
    }
}

void GameConfig::saveConfig()
{
    XmlStore xmlStore("netpanzer");

    saveSettings(xmlStore, "game", gamesettings);
    saveSettings(xmlStore, "player", playersettings);
    saveSettings(xmlStore, "visuals", visualssettings);
    saveSettings(xmlStore, "interface", interfacesettings);
    saveSettings(xmlStore, "radar", radarsettings);
    saveSettings(xmlStore, "server", serversettings);

    xmlStore.save(configfile.c_str());
}

void GameConfig::saveSettings(XmlStore& xmlstore, const char* name,
        std::vector<ConfigVariable*>& settings)
{
    XmlStore store = xmlstore.createChild(name);

    std::vector<ConfigVariable*>::iterator i;
    for(i = settings.begin(); i != settings.end(); i++) {
        ConfigVariable* var = *i;

        ConfigInt* confint = dynamic_cast<ConfigInt*> (var);
        if(confint)
            store.writeInt(confint->getName().c_str(), *confint);
            
        ConfigBool* confbool = dynamic_cast<ConfigBool*> (var);
        if(confbool)
            store.writeString(confbool->getName().c_str(),
                    *confbool ? "yes" : "no");
            
        ConfigString* confstring = dynamic_cast<ConfigString*> (var);
        if(confstring) {
            const char* string = 
                ((const std::string&)(*confstring)).c_str();
            store.writeString(confstring->getName().c_str(), string);
        }
    }
}

GameConfig* gameconfig = 0;
