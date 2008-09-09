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
#include <vector>

using namespace std;

#include "Units/UnitTypes.hpp"
#include "Units/UnitState.hpp"
#include "2D/PackedSurface.hpp"

class UnitProfile
{
public:
    std::string unitname;
    unsigned short unit_type;

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
    std::string imagefile;
    PackedSurface bodySprite;
    PackedSurface bodyShadow;
    PackedSurface turretSprite;
    PackedSurface turretShadow;
    std::string soundSelected;
    std::string fireSound;
    std::string weaponType;
    short boundBox;
};

class UnitProfileInterface
{
protected:
    static vector<UnitProfile *> profiles;
    static void clearProfiles();
    
public:
    static void loadUnitProfiles( void );
    static UnitProfile * getUnitProfile( unsigned short unit_type );
    static UnitProfile * getProfileByName( const std::string &name );
    static unsigned short getNumUnitTypes()
    {
        return profiles.size();
    }
};


#endif // ** _UNITPROFILEINTERFACE_HPP
