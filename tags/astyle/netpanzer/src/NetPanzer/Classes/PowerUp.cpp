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
#include "PowerUp.hpp"

#include "UnitBlackBoard.hpp"
#include "UnitInterface.hpp"
#include "MapInterface.hpp"

PowerUp::PowerUp()
{
    powerup_state.life_cycle_state = _power_up_lifecycle_state_active;
}

PowerUp::PowerUp(iXY map_loc, int ID, int type)
{
    powerup_state.map_loc = map_loc;
    powerup_state.ID = ID;
    powerup_state.type = type;

    MapInterface::mapXYtoPointXY( map_loc, &(powerup_state.world_loc) );
    powerup_state.life_cycle_state = _power_up_lifecycle_state_active;
}

PowerUp::PowerUp(iXY map_loc, int type)
{
    powerup_state.map_loc = map_loc;
    powerup_state.ID = -1;
    powerup_state.type = type;

    MapInterface::mapXYtoPointXY( map_loc, &(powerup_state.world_loc) );
    powerup_state.life_cycle_state = _power_up_lifecycle_state_active;
}

bool PowerUp::isPowerUpHit( UnitID *unit_id )
{
    if( UnitBlackBoard::unitOccupiesLoc( powerup_state.map_loc ) == true ) {
        if( UnitInterface::quearyUnitAtMapLoc( powerup_state.map_loc, unit_id ) == true ) {
            return( true );
        }
    }

    return( false );
}
