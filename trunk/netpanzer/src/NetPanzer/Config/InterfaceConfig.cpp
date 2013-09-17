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
#include "ConfigGetter.hpp"
#include "2D/Color.hpp"
     
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

void InterfaceConfig::MiniMap::load(const JSONNode& node)
{
    ConfigGetter cg(node);
    
    setPlayerUnitColor(    stringToColor(cg.getString("playerunitcolor", "aqua")) );
    setSelectedUnitColor(  stringToColor(cg.getString("selectedunitcolor", "white")) );
    setAlliedUnitColor(    stringToColor(cg.getString("alliedunitcolor", "orange")) );
    setPlayerOutpostColor( stringToColor(cg.getString("playeroutpostcolor", "blue")) );
    setAlliedOutpostColor( stringToColor(cg.getString("alliedoutpostcolor", "orange")) );
    setBigUnitSize( cg.getBool("bigunitsize", false) );
}

void InterfaceConfig::MiniMap::save(JSONNode& node) const
{
    
    node.push_back(JSONNode("playerunitcolor",    colorToString(getPlayerUnitColor())));
    node.push_back(JSONNode("selectedunitcolor",  colorToString(getSelectedUnitColor())));
    node.push_back(JSONNode("alliedunitcolor",    colorToString(getAlliedUnitColor())));
    node.push_back(JSONNode("playeroutpostcolor", colorToString(getPlayerOutpostColor())));
    node.push_back(JSONNode("alliedoutpostcolor", colorToString(getAlliedOutpostColor())));
    node.push_back(JSONNode("bigunitsize", useBigUnitSize()));
    
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

void InterfaceConfig::load(const JSONNode& node)
{
    ConfigGetter cg(node);
    
    setShowHealth( cg.getBool("showhealth", true) );
    setShowFlags( cg.getBool("showflags", true) );
    setShowNames( cg.getBool("shownames", true) );
    
    setAttackNotificationTime( cg.getInt("attacknotificationtime", 1, 20, 5) );

    setVehicleSelectionColor( stringToColor(cg.getString("vehicleselectioncolor", "blue")));
    
    setUnitInfoLayer( cg.getInt("unitinfolayer", 0, 7, 0) );
    setScrollRate( cg.getInt("scrollrate", 1, 1000, 1000) );
    setRankPositionX( cg.getInt("rankposition_x", 0, 4096, 100) );
    setRankPositionY( cg.getInt("rankposition_y", 0, 4096, 100) );
    setLanguage( cg.getString("language", "en.po") );
    
    JSONNode::const_iterator i = node.find("minimap");
    if ( i != node.end() )
    {
        minimap.load(*i);
    }
}

void InterfaceConfig::save(JSONNode& node) const
{
    node.push_back(JSONNode("showhealth",   showHealth()));
    node.push_back(JSONNode("showflags",    showFlags()));
    node.push_back(JSONNode("shownames",    showNames()));
    
    node.push_back(JSONNode("attacknotificationtime",   getAttackNotificationTime()));
    node.push_back(JSONNode("vehicleselectioncolor",    colorToString(getVehicleSelectionColor())));
    node.push_back(JSONNode("unitinfolayer",            getUnitInfoLayer()));
    node.push_back(JSONNode("scrollrate",               getScrollRate()));
    node.push_back(JSONNode("rankposition_x",           getRankPositionX()));
    node.push_back(JSONNode("rankposition_y",           getRankPositionY()));
    node.push_back(JSONNode("language",                 getLanguage()));
    
    JSONNode mm;
    mm.set_name("minimap");
    minimap.save(mm);
    
    node.push_back(mm);
}

