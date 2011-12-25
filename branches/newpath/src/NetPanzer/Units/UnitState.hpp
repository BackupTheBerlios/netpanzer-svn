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
#ifndef _UNITSTATE_HPP
#define _UNITSTATE_HPP

#include <string.h>

#include "ArrayUtil/BoundBox.hpp"
#include "Types/Angle.hpp"
#include "Interfaces/MapInterface.hpp"
#include "UnitLifecycles.hpp"

enum { _threat_level_all_clear,
       _threat_level_under_attack,
       _threat_level_defending
     };

class UnitState
{
public:
    unsigned char   unit_type;

    /// true if the unit is currently selected
    bool            select;

    // world pixel location of the unit
    iXY             location;

    // map xy location
    iXY             map_location;

    // world pixel location of top left tile corner
    iXY             map_px_location;

    // 1/256 of a tile
    iXY             subtile_location;

    BoundBox        bbox;

    AngleInt        body_angle;
    AngleInt        turret_angle;

    unsigned short  orientation;
    unsigned short  speed_rate;
    unsigned short  speed_factor;
    short           tilespeed;

    unsigned short  reload_time;
    short	    max_hit_points;
    short           hit_points;
    unsigned short  damage_factor;
    unsigned long   weapon_range;
    unsigned long   defend_range;

    unsigned char   threat_level;
    unsigned char   lifecycle_state;

    UnitState( );

    bool bounds(const iXY& loc) const
    {
        return( bbox.bounds( location, loc ) );
    }

    int percentDamageInt();
    float percentDamageFloat();

    void setInitialLocation(const iXY& map_loc)
    {
        map_location = map_loc;

        subtile_location.x = (map_loc.x << 8 ) + 128;
        subtile_location.y = (map_loc.y << 8 ) + 128;

        MapInterface::mapXYtoTopPointXY(map_loc, location);

        location.x += 128>>3;
        location.y += 128>>3;
    }

    void subtileMove(const int x, const int y)
    {
        subtile_location.x += x;
        location.x = subtile_location.x >> 3;
        map_location.x = location.x >> 5;

        subtile_location.y += y;
        location.y = subtile_location.y >> 3;
        map_location.y = location.y >> 5;


//        MapInterface::mapXYtoTopPointXY(map_location, location);

//        location.x += (subtile_location.x & 0xff) >> 3;
//        location.y += (subtile_location.y & 0xff) >> 3;

    }

};

#endif

