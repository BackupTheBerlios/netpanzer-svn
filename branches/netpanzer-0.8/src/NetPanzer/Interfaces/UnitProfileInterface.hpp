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
#ifndef _UNITPROFILEINTERFACE_HPP
#define _UNITPROFILEINTERFACE_HPP

#include <string>

#include "UnitTypes.hpp"
#include "UnitState.hpp"

class UnitProfile
{
public:
    std::string unitname;

    short hit_points;
    short attack_factor;
    long  attack_range;
    long  defend_range;
    char  reaction_time;
    char  speed_factor;
    char  speed_rate;
    char  tracking_rate;
    short fuel_capacity;
    char  reload_time;
    char  repath_time;
    short regen_time;
    char  jammable;
    char  jamming_time;
    // ** for fuel truck only
    long  fueling_range;
    // ** for ecm truck only **
    long  jamming_range;
};

class UnitProfileInterface
{
protected:
    static UnitProfile profile_table[ _MAX_UNIT_TYPES ];

public:
    static void loadUnitProfiles( void );
    static UnitProfile * getUnitProfile( unsigned short unit_type );
};


#endif // ** _UNITPROFILEINTERFACE_HPP
