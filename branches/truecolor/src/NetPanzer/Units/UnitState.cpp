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

#include "SDL_endian.h"
#include "Units/UnitState.hpp"

UnitState::UnitState()
{
    body_angle.set( 0, 10 );
    turret_angle.set( 0, 10 );

    // ** NOTE: TEMPORARY UNTIL UNIT PROPERITIES CODED

    bbox.setBounds( iXY( -25, -15 ), iXY( 25, 35 ) );

    select = false;
    orientation = 0;
    speed_rate = 20;
    speed_factor = 2;

    hit_points = 100;
    damage_factor = 30;
    weapon_range = 409600;
    defend_range = 100;
    reload_time = 45;

    threat_level = _threat_level_all_clear;
}

int UnitState::percentDamageInt( void )
{
    int damage_percentage;

    damage_percentage = (int)  ( (1 - ( ( (float) hit_points ) / ( (float) max_hit_points) ) ) * 100 );

    return( damage_percentage );
}

float UnitState::percentDamageFloat( void )
{
    float damage_percentage;

    damage_percentage = ( (1 - ( ( (float) hit_points ) / ( (float) max_hit_points) ) ) * 100 );

    return( damage_percentage );
}

NetworkUnitState UnitState::getNetworkUnitState() const
{
    NetworkUnitState state;

    state.unit_type = unit_type;
    state.location_x = SDL_SwapLE32(location.x);
    state.location_y = SDL_SwapLE32(location.y);
    state.bbox_min_x = SDL_SwapLE32(bbox.min.x);
    state.bbox_min_y = SDL_SwapLE32(bbox.min.y);
    state.bbox_max_x = SDL_SwapLE32(bbox.max.x);
    state.bbox_max_y = SDL_SwapLE32(bbox.max.y);

    state.body_angle = body_angle.getNetworkAngleInt();
    state.turret_angle = turret_angle.getNetworkAngleInt();
    state.orientation = SDL_SwapLE16(orientation);
    state.speed_rate = SDL_SwapLE16(speed_rate);
    state.speed_factor = SDL_SwapLE16(speed_factor);
    
    state.reload_time = SDL_SwapLE16(reload_time);
    state.max_hit_points = SDL_SwapLE16(max_hit_points);
    state.hit_points = SDL_SwapLE16(hit_points);
    state.damage_factor = SDL_SwapLE16(damage_factor);
    state.weapon_range = SDL_SwapLE32(weapon_range);
    state.defend_range = SDL_SwapLE32(defend_range);
    
    state.threat_level = threat_level;
    state.lifecycle_state = lifecycle_state;

    return state;
}

void UnitState::setFromNetworkUnitState(const NetworkUnitState& state)
{
    select = false;

    unit_type = state.unit_type;
    location.x = SDL_SwapLE32(state.location_x);
    location.y = SDL_SwapLE32(state.location_y);
    bbox.min.x = SDL_SwapLE32(state.bbox_min_x);
    bbox.min.y = SDL_SwapLE32(state.bbox_min_y);
    bbox.max.x = SDL_SwapLE32(state.bbox_max_x);
    bbox.max.y = SDL_SwapLE32(state.bbox_max_y);

    body_angle.setFromNetworkAngleInt(state.body_angle);
    turret_angle.setFromNetworkAngleInt(state.turret_angle);

    orientation = SDL_SwapLE16(state.orientation);
    speed_rate = SDL_SwapLE16(state.speed_rate);
    speed_factor = SDL_SwapLE16(state.speed_factor);
    
    reload_time = SDL_SwapLE16(state.reload_time);
    max_hit_points = SDL_SwapLE16(state.max_hit_points);
    hit_points = SDL_SwapLE16(state.hit_points);
    damage_factor = SDL_SwapLE16(state.damage_factor);
    weapon_range = SDL_SwapLE32(state.weapon_range);
    defend_range = SDL_SwapLE32(state.defend_range);

    threat_level = state.threat_level;
    lifecycle_state = state.lifecycle_state;
}
