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
    
#define DEFINE_SLIDER(name) \
    Label  * label_##name; \
    Slider * slider_##name; \
    Label  * label_##name##_value
    
    DEFINE_SLIDER(maxplayers);
    DEFINE_SLIDER(maxunits);
    DEFINE_SLIDER(occupation);
    DEFINE_SLIDER(timelimit);
    DEFINE_SLIDER(fraglimit);
    DEFINE_SLIDER(clouds);
    DEFINE_SLIDER(wind);
    
#undef DEFINE_SLIDER
    
    Label  * label_gametype;
    Choice * choice_gametype;
    
    NPString selectedMap;
    std::vector<NPString> mapList;
    
};

#endif	/* HOSTGAMELAYER_HPP */

