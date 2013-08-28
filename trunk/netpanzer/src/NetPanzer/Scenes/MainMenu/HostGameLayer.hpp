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
 * Created on March 12, 2013, 9:04 PM
 */

#ifndef HOSTGAMELAYER_HPP
#define	HOSTGAMELAYER_HPP

#include <vector>

#include "Core/CoreTypes.hpp"

#include "2D/ComponentLayer.hpp"

class MapThumbnailComponent;
class Button;
class Label;
class Slider;
class Choice;

class HostGameLayer : public ComponentLayer
{
public:
    HostGameLayer();
    ~HostGameLayer();

private:
    void recalculateComponentLocations();
    void handleComponentEvents();
    
    void refreshMaps();
    void setSelectedMap(const NPString& map_name);
    const NPString& getNextMapName(const NPString& name) const;
    const NPString& getPrevMapName(const NPString& name) const;
    
    Component * area;
    
    MapThumbnailComponent * mapthumbnail;
    
    Button * button_prevmap;
    Button * button_nextmap;
    Label  * label_mapnamelabel;
    Label  * label_mapsizelabel;
    Label  * label_mapobjectiveslabel;
    Label  * label_mapname;
    Label  * label_mapsize;
    Label  * label_mapobjectives;
    
    Label  * label_maxplayers;
    Slider * slider_maxplayers;
    Label  * label_maxplayers_value;
    
    Label  * label_maxunits;
    Slider * slider_maxunits;
    Label  * label_maxunits_value;
    
    Label  * label_occupation;
    Slider * slider_occupation;
    Label  * label_occupation_value;
    
    Label  * label_timelimit;
    Slider * slider_timelimit;
    Label  * label_timelimit_value;
    
    Label  * label_fraglimit;
    Slider * slider_fraglimit;
    Label  * label_fraglimit_value;
    
    Label  * label_clouds;
    Slider * slider_clouds;
    Label  * label_clouds_value;
            
    Label  * label_wind;
    Slider * slider_wind;
    Label  * label_wind_value;
    
    Label  * label_gametype;
    Choice * choice_gametype;
    
    NPString selectedMap;
    std::vector<NPString> mapList;
    
};

#endif	/* HOSTGAMELAYER_HPP */

