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

#include <string.h>

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
    initialize(9);
}

void PlayerUnitConfig::initialize( unsigned char max_allowed_units )
{
    PlayerUnitConfig::max_allowed_units = max_allowed_units;
    memset( unit_spawn_list, 0, sizeof( unsigned char ) * _MAX_UNIT_TYPES );
    // *******************  CHANGE UNIT COUNT HERE **************************
    unit_spawn_list[ _unit_type_valentine ] = 2;        // <-- RIGHT HERE
    unit_spawn_list[ _unit_type_leopard ] = 2;          // <-- RIGHT HERE
    unit_spawn_list[ _unit_type_abrams ] = 2;           // <-- RIGHT HERE
    unit_spawn_list[ _unit_type_hammerhead ] = 1;       // <-- RIGHT HERE
    unit_spawn_list[ _unit_type_humvee ] = 2;           // <-- RIGHT HERE
    unit_color = 0;
}

unsigned char PlayerUnitConfig::unitTotal( void )
{
    unsigned char total = 0;
    unsigned char i;

    for( i = 0; i < _MAX_UNIT_TYPES; i++ ) {
        total += unit_spawn_list[ i ];
    }

    return( total );
}
