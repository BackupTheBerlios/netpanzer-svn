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

#include "PowerUps/PowerUp.hpp"
#include "Core/GlobalGameState.hpp"

#include "Units/UnitInterface.hpp"
#include "Interfaces/MapInterface.hpp"
#include "Classes/Network/NetworkState.hpp"

Surface PowerUp::POWERUP_ANIM;
Surface PowerUp::POWERUP_ANIM_SHADOW;

PowerUp::PowerUp(const iXY& map_loc, PowerUpID ID, int type)
{
    this->map_loc = map_loc;
    this->ID = ID;
    this->type = type;

    MapInterface::mapXYtoPointXY( map_loc, &(this->world_loc) );

    this->life_cycle_state = _power_up_lifecycle_state_active;

    sprite.setData( POWERUP_ANIM );
    sprite.setAttrib( world_loc, iXY(0,0), 5 );

    sprite_shadow.setData( POWERUP_ANIM_SHADOW );
    sprite_shadow.setAttrib( world_loc, iXY(0,0), 4 );
    sprite_shadow.setDrawModeBlend(64); // dark a lot
}
