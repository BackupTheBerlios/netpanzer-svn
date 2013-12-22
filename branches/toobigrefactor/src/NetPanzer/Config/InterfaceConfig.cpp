/*
Copyright (C) 2013 Netpanzer Team. (www.netpanzer.org), Aaron Perez

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
 * 
 * Created on September 15, 2013, 11:00 PM
 */

#include "InterfaceConfig.hpp"
#include "2D/Color.hpp"

#include "json/json.h"
     
struct StringColor
{
    const char * name;
    int color;
};

static struct StringColor stringColorTable[] = {
    { "aqua", Color::unitAqua },
    { "yellow", Color::unitYellow },
    { "red", Color::unitRed },
    { "blue", Color::unitBlue },
    { "darkblue", Color::unitDarkBlue },
    { "lightgreen", Color::unitLightGreen },
    { "green", Color::unitGreen },
    { "bluegray", Color::unitBlueGray },
    { "darkred", Color::unitDarkRed },
    { "black", Color::unitBlack },
    { "darkgreen", Color::unitDarkGreen },
    { "white", Color::unitWhite },
    { "lightorange", Color::unitLightOrange },
    { "orange", Color::unitOrange },
    { "gray", Color::unitGray },
    { "darkgray", Color::unitDarkGray }
};

#define NUM_COLORS (sizeof(stringColorTable)/sizeof(StringColor))

#define DEFAULT_COLOR Color::white;
#define DEFAULT_STRCOLOR "white"
     
static int stringToColor(const NPString& str)
{
    for ( unsigned n = 0; n < NUM_COLORS; n++ )
    {
        if ( str == stringColorTable[n].name )
        {
            return stringColorTable[n].color;
        }
    }
    
    return DEFAULT_COLOR;
}

static const char * colorToString(const int c)
{
    for ( unsigned n = 0; n < NUM_COLORS; n++ )
    {
        if ( c == stringColorTable[n].color )
        {
            return stringColorTable[n].name;
        }
    }
    
    return DEFAULT_STRCOLOR;
}



InterfaceConfig::MiniMap::MiniMap()
 :  playerunitcolor(InterfaceConfig::_color_aqua),
    selectedunitcolor(InterfaceConfig::_color_white),
    alliedunitcolor(InterfaceConfig::_color_orange),
    playeroutpostcolor(InterfaceConfig::_color_blue),
    alliedoutpostcolor(InterfaceConfig::_color_orange),
    bigunitsize(false)
{
    
}

InterfaceConfig::MiniMap::~MiniMap()
{
    
}

void InterfaceConfig::MiniMap::load(const Json::Value& node)
{
    setPlayerUnitColor(    stringToColor(node.get("playerunitcolor", "aqua").asString()) );
    setSelectedUnitColor(  stringToColor(node.get("selectedunitcolor", "white").asString()) );
    setAlliedUnitColor(    stringToColor(node.get("alliedunitcolor", "orange").asString()) );
    setPlayerOutpostColor( stringToColor(node.get("playeroutpostcolor", "blue").asString()) );
    setAlliedOutpostColor( stringToColor(node.get("alliedoutpostcolor", "orange").asString()) );
    setBigUnitSize( node.get("bigunitsize", false).asBool() );
}

void InterfaceConfig::MiniMap::save(Json::Value& node) const
{
    
    node["playerunitcolor"]    = colorToString(getPlayerUnitColor());
    node["selectedunitcolor"]  = colorToString(getSelectedUnitColor());
    node["alliedunitcolor"]    = colorToString(getAlliedUnitColor());
    node["playeroutpostcolor"] = colorToString(getPlayerOutpostColor());
    node["alliedoutpostcolor"] = colorToString(getAlliedOutpostColor());
    node["bigunitsize"]        = useBigUnitSize();
    
}

InterfaceConfig::InterfaceConfig()
 :  show_health(true),
    show_flags(true),
    show_names(true),
    attacknotificationtime(5),
    vehicleselectioncolor(InterfaceConfig::_color_blue),
    unitinfodrawlayer(0),
    scrollrate(1000),
    rankposition_x(100),
    rankposition_y(100),
    language("en.po")
{
    
}

InterfaceConfig::~InterfaceConfig()
{
    
}

void InterfaceConfig::load(const Json::Value& node)
{
    setShowHealth( node.get("showhealth", true).asBool() );
    setShowFlags( node.get("showflags", true).asBool() );
    setShowNames( node.get("shownames", true).asBool() );
    
    setAttackNotificationTime( node.get("attacknotificationtime", 5).asInt() );

    setVehicleSelectionColor( stringToColor(node.get("vehicleselectioncolor", "blue").asString()));
    
    setUnitInfoLayer( node.get("unitinfolayer", 0).asInt() );
    setScrollRate( node.get("scrollrate", 1000).asInt() );
    setRankPositionX( node.get("rankposition_x", 100).asInt() );
    setRankPositionY( node.get("rankposition_y", 100).asInt() );
    setLanguage( node.get("language", "en.po").asString() );

    if ( node.isMember("minimap") )
    {
        minimap.load(node["minimap"]);
    }
}

void InterfaceConfig::save(Json::Value& node) const
{
    node["showhealth"] = showHealth();
    node["showflags"] = showFlags();
    node["shownames"] = showNames();
    
    node["attacknotificationtime"] = getAttackNotificationTime();
    node["vehicleselectioncolor"] = colorToString(getVehicleSelectionColor());
    node["unitinfolayer"] = getUnitInfoLayer();
    node["scrollrate"] = getScrollRate();
    node["rankposition_x"] = getRankPositionX();
    node["rankposition_y"] = getRankPositionY();
    node["language"] = getLanguage();
    
    Json::Value mm(Json::objectValue);
    minimap.save(mm);
    
    node["minimap"] = mm;
    
}

