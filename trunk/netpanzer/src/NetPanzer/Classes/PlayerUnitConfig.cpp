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
#include "PlayerUnitConfig.hpp"
#include "GameConfig.hpp"

int
UnitGameInfo::getUnitPointValue(UnitType unit_type)
{
    switch(unit_type) {
        case _unit_type_valentine:
            return 2;
        case _unit_type_leopard:
            return 5;
        case _unit_type_abrams:
            return 6;
        case _unit_type_hammerhead:
            return 5;
        case _unit_type_lynx:
            return 1;
        case _unit_type_spahpanzer:
            return 4;
        case _unit_type_archer:
            return 5;
        case _unit_type_scorpion:
            return 3;
        case _unit_type_m109:
            return 4;
        case _unit_type_humvee:
            return 1;
        default:
            assert(false);
    }       
}



PlayerUnitConfig::PlayerUnitConfig()
{
}

void PlayerUnitConfig::initialize()
{
    max_allowed_units = gameconfig->maxunits / gameconfig->maxplayers;
    SDL_memset( unit_spawn_list, 0, sizeof(unit_spawn_list));
    int rem_units = max_allowed_units;
    unsigned int numunits;

    numunits = (rem_units < gameconfig->archer)?rem_units:gameconfig->archer;
    rem_units-=numunits;
    unit_spawn_list[ _unit_type_archer ] = numunits;
    
    numunits = (rem_units < gameconfig->bear)?rem_units:gameconfig->bear;
    rem_units-=numunits;
    unit_spawn_list[ _unit_type_spahpanzer ] = numunits;
    
    numunits = (rem_units < gameconfig->bobcat)?rem_units:gameconfig->bobcat;
    rem_units-=numunits;
    unit_spawn_list[ _unit_type_lynx ] = numunits;
    
    numunits = (rem_units < gameconfig->drake)?rem_units:gameconfig->drake;
    rem_units-=numunits;
    unit_spawn_list[ _unit_type_m109 ] = numunits;
    
    numunits = (rem_units < gameconfig->manta)?rem_units:gameconfig->manta;
    rem_units-=numunits;
    unit_spawn_list[ _unit_type_valentine ] = numunits;
    
    numunits = (rem_units < gameconfig->panther1)?rem_units:gameconfig->panther1;
    rem_units-=numunits;
    unit_spawn_list[ _unit_type_leopard ] = numunits;
    
    numunits = (rem_units < gameconfig->spanzer)?rem_units:gameconfig->spanzer;
    rem_units-=numunits;
    unit_spawn_list[ _unit_type_humvee ] = numunits;
    
    numunits = (rem_units < gameconfig->stinger)?rem_units:gameconfig->stinger;
    rem_units-=numunits;
    unit_spawn_list[ _unit_type_hammerhead ] = numunits;
    
    numunits = (rem_units < gameconfig->titan)?rem_units:gameconfig->titan;
    rem_units-=numunits;
    unit_spawn_list[ _unit_type_abrams ] = numunits;
    
    numunits = (rem_units < gameconfig->wolf)?rem_units:gameconfig->wolf;
    rem_units-=numunits;
    unit_spawn_list[ _unit_type_scorpion ] = numunits;
    
    unit_color = 0;
}

unsigned int PlayerUnitConfig::unitTotal( void )
{
    unsigned int total = 0;
    unsigned int i;

    for( i = 0; i < _MAX_UNIT_TYPES; i++ ) {
        total += unit_spawn_list[ i ];
    }

    return( total );
}
