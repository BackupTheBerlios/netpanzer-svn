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
#include <fstream>
#include <sstream>

#include "Util/FileSystem.hpp"
#include "Util/FileStream.hpp"
#include "Util/Log.hpp"
#include "Util/Exception.hpp"
#include "INIParser/Store.hpp"
#include "INIParser/Section.hpp"
#include "Interfaces/GameConfig.hpp"
#include "Core/NetworkGlobals.hpp"
#include "Views/Game/MiniMapView.hpp"
#include "Views/GameViewGlobals.hpp"

GameConfig::GameConfig(const std::string& configfile, bool usePhysFS)
    // VariableName("Name", value [, minimum, maximum])
    : hostorjoin("hostorjoin", _game_session_join, 0, _game_session_last-1),
      quickConnect("quickconnect", false),
      serverConnect("serverconnect", ""),
    
      playername("name", "Player"),
      masterservers("masterservers", "masterserver.netpanzer.org, masterserver2.netpanzer.org, masterserver.netpanzer.info"),
      
      serverport("serverport", NETPANZER_DEFAULT_PORT_TCP,0,65535),
      proxyserver("proxyserver",""),
      proxyserveruser("proxyserveruser",""),
      proxyserverpass("proxyserverpass",""),
      bindaddress("bindaddress", ""),
      gametype("gametype", _gametype_objective, 0, _gametype_last-1),
      maxplayers("maxplayers", 8, 1, 25),
      maxunits("maxunits", 500, 25, 10000),
      timelimit("timelimit", 50, 1),
      fraglimit("fraglimit", 500, 50),
      powerups("powerups", true),
      objectiveoccupationpercentage("objectivepercentage", 75, 1, 100),
      allowallies("allowallies", true),
      cloudcoverage("cloudcoverage", 0),
      respawntype("respawntype", _game_config_respawn_type_round_robin, 0,
                _game_config_respawn_type_last-1),
      windspeed("windspeed", 30),
      map("map", "Bad Neuburg"),
      mapcycle("mapcycle", "Cramped, Tight Quarters, Two Villages"),
      motd("motd",""),
      logging("logging", false),
      publicServer("public", true),
      
      screenresolution("resolution", 2, 0, 3),
      fullscreen("fullscreen", true),
      hardwareSurface("hardwareSurface", true),
      hardwareDoubleBuffer("hardwareDoubleBuffer", true),
      displayshadows("displayshadows", true),
      blendsmoke("blendsmoke", true),
      screengamma("gamma", 50, 0, 100),
#ifdef _WIN32
      usedirectx("usedirectx", true),
#endif

      enablesound("enable", true),
      enablemusic("music", true),
      musicvolume("musicvolume", 80, 0, 100),
      enableeffects("effects", true),
      effectsvolume("effectsvolume", 100, 0, 100),
      
      unitcolor("unitcolor", 0, 0, _color_last-1),
      playerflag("playerflag", "albania.bmp"),
      attacknotificationtime("attacknotificationtime", 5, 0, 100),
      vehicleselectioncolor("vehicleselectioncolor", _color_blue, 0, _color_last-1),
      unitselectionmode("unitselectionmode", _unit_selection_box_draw_mode_rect_edges, 0, _unit_selection_box_draw_mode_last-1),
      unitinfodrawlayer("unitinfodrawlayer", 0, 0, 1),
      drawunitdamage("drawunitdamage", true),
      drawunitreload("drawunitreload", false),
      drawunitflags("drawunitflags", true),
      consoletextdelay("consoletextdelay", 3, 1, 20),
      consoletextusage("consoletextusage", 25, 1, 100),
      scrollrate("scrollrate", 1000, 100, 10000),
      minimapposition("minimapposition", iXY(0,0)),
      minimapsize("minimapsize", iXY(194,194)),
      minimapdrawtype("minimapdrawtype", 0),
      gameinfoposition("gameinfoposition", iXY(0,0)),
      toolbarposition("toolbarposition", iXY(0,0)),
      rankposition("rankposition", iXY(0,0)),
      viewdrawbackgroundmode("viewdrawbackgroundmode",(int)VIEW_BACKGROUND_DARK_GRAY_BLEND),
                  
      radar_displayclouds("displayclouds", false),
      radar_playerunitcolor("playerunitcolor", _color_aqua, 0, _color_last-1),
      radar_selectedunitcolor("selectedunitcolor", _color_white, 0, _color_last-1),
      radar_alliedunitcolor("alliedunitcolor", _color_orange, 0, _color_last-1),
      radar_playeroutpostcolor("playeroutpostcolor", _color_blue, 0, _color_last-1),
      radar_alliedoutpostcolor("alliedoutpostcolor", _color_orange, 0, _color_last),
      radar_enemyoutpostcolor("enemyoutpostcolor", _color_red, 0, _color_last-1),
      radar_unitsize("unitsize", _mini_map_unit_size_small, 0, _mini_map_unit_size_last-1),
      radar_objectivedrawmode("objectivedrawmode", _mini_map_objective_draw_mode_outline_rect, 0, _mini_map_objective_draw_mode_last-1),
      radar_resizerate("resizerate", 400, 10, 1000),
      
      titan("titan",2),
      manta("manta",2),
      panther1("panther1",2),
      stinger("stinger",1),
      spanzer("spanzer",2),
      bobcat("bobcat",0),
      wolf("wolf",0),
      bear("bear",0),
      drake("drake",0),
      archer("archer",0)
{
    this->configfile = configfile;
    this->usePhysFS = usePhysFS;

    std::stringstream default_player;
    default_player << "Player" << (rand()%1000);
    playername=default_player.str();
    playersettings.push_back(&playername);
    playersettings.push_back(&masterservers);
    playersettings.push_back(&proxyserver);
    playersettings.push_back(&proxyserveruser);
    playersettings.push_back(&proxyserverpass);

    serversettings.push_back(&serverport);
    serversettings.push_back(&bindaddress);
    serversettings.push_back(&gametype);
    serversettings.push_back(&maxplayers);
    serversettings.push_back(&maxunits);
    serversettings.push_back(&timelimit);
    serversettings.push_back(&fraglimit);
    serversettings.push_back(&powerups);
    serversettings.push_back(&objectiveoccupationpercentage);
    serversettings.push_back(&allowallies);
    serversettings.push_back(&cloudcoverage);
    serversettings.push_back(&respawntype);
    serversettings.push_back(&windspeed);
    //serversettings.push_back(&map);
    serversettings.push_back(&mapcycle);
    serversettings.push_back(&motd);
    serversettings.push_back(&logging);
    serversettings.push_back(&publicServer);
   
    visualssettings.push_back(&screenresolution);
    visualssettings.push_back(&fullscreen);
    visualssettings.push_back(&hardwareSurface);
    visualssettings.push_back(&hardwareDoubleBuffer);
    visualssettings.push_back(&displayshadows);
    visualssettings.push_back(&blendsmoke);
    visualssettings.push_back(&screengamma);
#ifdef _WIN32
    visualssettings.push_back(&usedirectx);
#endif

    soundsettings.push_back(&enablesound);
    soundsettings.push_back(&enablemusic);
    soundsettings.push_back(&musicvolume);
    soundsettings.push_back(&enableeffects);
    soundsettings.push_back(&effectsvolume);

    interfacesettings.push_back(&unitcolor);
    interfacesettings.push_back(&playerflag);
    interfacesettings.push_back(&attacknotificationtime);
    interfacesettings.push_back(&vehicleselectioncolor);
    interfacesettings.push_back(&unitselectionmode);
    interfacesettings.push_back(&unitinfodrawlayer);
    interfacesettings.push_back(&drawunitdamage);
    interfacesettings.push_back(&drawunitreload);
    interfacesettings.push_back(&drawunitflags);
    interfacesettings.push_back(&consoletextdelay);
    interfacesettings.push_back(&consoletextusage);
    interfacesettings.push_back(&scrollrate);
    interfacesettings.push_back(&minimapposition);
    interfacesettings.push_back(&minimapsize);
    interfacesettings.push_back(&minimapdrawtype);
    interfacesettings.push_back(&gameinfoposition);
    interfacesettings.push_back(&toolbarposition);
    interfacesettings.push_back(&rankposition);
    interfacesettings.push_back(&viewdrawbackgroundmode);

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
    } catch(std::exception& e) {
        LOG(("couldn't read game configuration: %s", e.what()));
	LOG(("Using default config. (this is normal on first startup)"));
    }
}

GameConfig::~GameConfig()
{
    try {
        saveConfig();
    } catch(std::exception& e) {
        LOG(("couldn't save game configuration: %s", e.what()));
    }
}

void GameConfig::loadConfig()
{
    INI::Store inifile;
    if(usePhysFS) {
        IFileStream in(configfile);
        inifile.load(in);
    } else {
        std::ifstream in(configfile.c_str());
        inifile.load(in);
    }

    loadSettings(inifile.getSection("game"), gamesettings);
    loadSettings(inifile.getSection("player"), playersettings);
    if (playername.c_str()[0]==0) {
        std::stringstream default_player;
        default_player << "Player" << (rand()%1000);
        playername=default_player.str();
    }
    loadSettings(inifile.getSection("visuals"), visualssettings);
    loadSettings(inifile.getSection("sound"), soundsettings);
    loadSettings(inifile.getSection("interface"), interfacesettings);
    loadSettings(inifile.getSection("radar"), radarsettings);
    loadSettings(inifile.getSection("server"), serversettings);
    loadSpawnSettings(inifile.getSection("spawnconfig"),spawnsettings);
}

void GameConfig::loadSettings(const INI::Section& section,
        std::vector<ConfigVariable*>& settings)
{
    try {
        std::vector<ConfigVariable*>::iterator i;
        for(i = settings.begin(); i != settings.end(); i++) {
            ConfigVariable* var = *i;

            try {
                ConfigInt* confint = dynamic_cast<ConfigInt*> (var);
                if(confint) {
                    *confint = section.getIntValue(confint->getName());
                    continue;
                }

                ConfigXY* confxy = dynamic_cast<ConfigXY*> (var);
                if(confxy) {
                    confxy->set(section.getIntValue(confxy->getName() + "_x"),
                            section.getIntValue(confxy->getName() + "_y"));
                    continue;
                }

                ConfigBool* confbool = dynamic_cast<ConfigBool*> (var);
                if(confbool) {
                    std::string str =
                        section.getValue(confbool->getName());
                    if(str == "yes" || str == "1" || str == "on")
                        *confbool = true;
                    else if(str == "no" || str == "0" || str == "off")
                        *confbool = false;
                    else
                        throw Exception("No boolean value for setting '%s'.",
                                        confbool->getName().c_str());
                    continue;
                }

                ConfigStringSpecialChars* confstringspecial = dynamic_cast<ConfigStringSpecialChars*> (var);
                if(confstringspecial) {
                    *confstringspecial = section.getValue(confstringspecial->getName());
                    continue;
                }
                
                ConfigString* confstring = dynamic_cast<ConfigString*> (var);
                if(confstring)
                    *confstring = section.getValue(confstring->getName());


                // we have a value from config file in the variable now
                //var->setNonDefaultValue();
                // now each subclass changethis if the value has been changed
            } catch(std::exception& e) {
                LOG(("Skipping config '%s': %s", var->getName().c_str(),
                            e.what()));
            }
        }
    } catch(std::exception& e) {
        LOG(("Couldn't find config section '%s', skipping...",
                    section.getName().c_str()));
    }
}

void GameConfig::loadSpawnSettings(const INI::Section& section,
                                   std::vector<ConfigVariable*>& settings)
{
    INI::valuesIterator i = section.getValuesBegin();
    while ( i != section.getValuesEnd() )
    {
        ConfigInt * intvar = new ConfigInt( i->first, section.getIntValue(i->first) );
        settings.push_back(intvar);
        i++;
    }
}


void GameConfig::saveConfig()
{
    INI::Store inifile;

    saveSettings(inifile.getSection("game"), gamesettings);
    saveSettings(inifile.getSection("player"), playersettings);
    saveSettings(inifile.getSection("visuals"), visualssettings);
    saveSettings(inifile.getSection("sound"), soundsettings);
    saveSettings(inifile.getSection("interface"), interfacesettings);
    saveSettings(inifile.getSection("radar"), radarsettings);
    saveSettings(inifile.getSection("server"), serversettings);
    saveSettings(inifile.getSection("spawnconfig"),spawnsettings);

    if(usePhysFS) {
        OFileStream out (configfile);
        inifile.save(out);
    } else {
        std::ofstream out(configfile.c_str());
        inifile.save(out);
    }
}

void GameConfig::saveSettings(INI::Section& section,
    std::vector<ConfigVariable*>& settings)
{
    std::vector<ConfigVariable*>::iterator i;
    for(i = settings.begin(); i != settings.end(); i++) {
        ConfigVariable* var = *i;

        ConfigInt* confint = dynamic_cast<ConfigInt*> (var);
        if(confint)
            section.setIntValue(confint->getName(), *confint);
            
        ConfigBool* confbool = dynamic_cast<ConfigBool*> (var);
        if(confbool)
            section.setValue(confbool->getName(), *confbool ? "yes" : "no");

        ConfigXY* confxy = dynamic_cast<ConfigXY*> (var);
        if(confxy) {
            section.setIntValue(confxy->getName() + "_x",
                    ((const iXY&)(*confxy)).x);
            section.setIntValue(confxy->getName() + "_y",
                    ((const iXY&)(*confxy)).y);
        }

        ConfigString* confstring = dynamic_cast<ConfigString*> (var);
        if(confstring) {
            section.setValue(confstring->getName(),
                    (const std::string&) *confstring);
        }
    }
}

GameConfig* gameconfig = 0;
