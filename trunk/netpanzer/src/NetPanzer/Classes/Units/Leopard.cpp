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
#include "Leopard.hpp"
#include "Interfaces/UnitProfileInterface.hpp"
#include "2D/Color.hpp"
#include "Interfaces/WorldViewInterface.hpp"
#include "Classes/Weapons/Weapon.hpp"
#include "System/Sound.hpp"

#include "UnitGlobals.hpp"

void Leopard::setUnitProperties( void )
{
    UnitProfile *profile;

    profile = UnitProfileInterface::getUnitProfile( _unit_type_leopard );

    unit_state.hit_points = profile->hit_points;
    unit_state.max_hit_points = profile->hit_points;
    unit_state.damage_factor = profile->attack_factor;
    unit_state.defend_range = profile->defend_range;
    unit_state.speed_factor = profile->speed_factor;
    unit_state.speed_rate = profile->speed_rate;
    unit_state.reload_time = profile->reload_time;
    unit_state.weapon_range = profile->attack_range;
    unit_state.unit_type = _unit_type_leopard;
    select_info_box.setHitBarAttributes( profile->hit_points, Color::yellow );

}

Leopard::Leopard(PlayerState* player, UnitID id, iXY initial_loc,
        unsigned char color, unsigned char flag )
        : Vehicle(player, id, initial_loc)
{
    setUnitProperties();

    if ( color == false) {
        body_anim.setData( gLeopardBody );
        turret_anim.setData( gLeopardTurret );
    } else {
        body_anim.setData( gLeopardBodyDarkBlue );
        turret_anim.setData( gLeopardTurretDarkBlue );
    }


    body_anim.setAttrib( iXY(0,0), iXY(0,0), unitLayer );
    turret_anim.setAttrib( iXY(0,0), iXY(0,0), unitLayer );

    body_anim_shadow.setData( gLeopardBodyShadow );
    turret_anim_shadow.setData( gLeopardTurretShadow );
    body_anim_shadow.setAttrib( iXY(0,0), iXY(0,0), unitLayer );
    turret_anim_shadow.setAttrib( iXY(0,0), iXY(0,0), unitLayer );

    select_info_box.setBoxAttributes( BoundBox( -25, -25, 25, 25), Color::blue );
    select_info_box.setBoxState( false );
    select_info_box.setFlag( flag );

    body_anim_shadow.attachSprite( &body_anim, iXY(0,0) );
    body_anim_shadow.attachSprite( &turret_anim_shadow, iXY(0,0) );
    body_anim_shadow.attachSprite( &turret_anim, iXY(0,0) );
    body_anim_shadow.attachSprite( &select_info_box, iXY(0,0) );
}

//-----------------------------------------------------------------
/**
 * Make noise.
 * @return projectile_type
 */
unsigned short Leopard::launchProjectile()
{
    //SFX
    long distance = WorldViewInterface::getCameraDistance(unit_state.location);
    sound->playAmbientSound("mt-fire", distance );
    sound->playBattle();

    return Weapon::_shell;
}
//-----------------------------------------------------------------
void Leopard::soundSelected()
{
    sound->playSound("mt-selected");
}
