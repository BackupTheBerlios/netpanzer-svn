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
#include "UnitState.hpp"

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
    state.location_x = location.x;
    state.location_y = location.y;
    state.bbox_min_x = bbox.min.x;
    state.bbox_min_y = bbox.min.y;
    state.bbox_max_x = bbox.max.x;
    state.bbox_max_y = bbox.max.y;

    state.body_angle = body_angle.getNetworkAngleInt();
    state.turret_angle = turret_angle.getNetworkAngleInt();
    state.orientation = orientation;
    state.speed_rate = speed_rate;
    state.speed_factor = speed_factor;
    
    state.reload_time = reload_time;
    state.max_hit_points = max_hit_points;
    state.hit_points = hit_points;
    state.damage_factor = damage_factor;
    state.weapon_range = weapon_range;
    state.defend_range = defend_range;
    
    state.threat_level = threat_level;
    state.lifecycle_state = lifecycle_state;

    return state;
}

void UnitState::setFromNetworkUnitState(const NetworkUnitState& state)
{
    select = false;

    unit_type = state.unit_type;
    location.x = state.location_x;
    location.y = state.location_y;
    bbox.min.x = state.bbox_min_x;
    bbox.min.y = state.bbox_min_y;
    bbox.max.x = state.bbox_max_x;
    bbox.max.y = state.bbox_max_y;

    body_angle.setFromNetworkAngleInt(state.body_angle);
    turret_angle.setFromNetworkAngleInt(state.turret_angle);

    orientation = state.orientation;
    speed_rate = state.speed_rate;
    speed_factor = state.speed_factor;
    
    reload_time = state.reload_time;
    max_hit_points = state.max_hit_points;
    hit_points = state.hit_points;
    damage_factor = state.damage_factor;
    weapon_range = state.weapon_range;
    defend_range = state.defend_range;

    threat_level = state.threat_level;
    lifecycle_state = state.lifecycle_state;
}

