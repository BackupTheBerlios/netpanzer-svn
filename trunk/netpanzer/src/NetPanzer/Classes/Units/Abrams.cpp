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

#include "Abrams.hpp"
#include "Interfaces/UnitProfileInterface.hpp"
#include "2D/Color.hpp"
#include "Interfaces/WorldViewInterface.hpp"
#include "Classes/Weapons/Weapon.hpp"
#include "System/Sound.hpp"

#include "UnitGlobals.hpp"

void Abrams::setUnitProperties( void )
{
    UnitProfile *profile;

    profile = UnitProfileInterface::getUnitProfile( _unit_type_abrams );

    unit_state.hit_points = profile->hit_points;
    unit_state.max_hit_points = profile->hit_points;
    unit_state.damage_factor = profile->attack_factor;
    unit_state.defend_range = profile->defend_range;
    unit_state.speed_factor = profile->speed_factor;
    unit_state.speed_rate = profile->speed_rate;
    unit_state.reload_time = profile->reload_time;
    unit_state.weapon_range = profile->attack_range;
    unit_state.unit_type = _unit_type_abrams;
    select_info_box.setHitBarAttributes( profile->hit_points, Color::yellow );

}

Abrams::Abrams(PlayerState* player, UnitID id, iXY initial_loc,
        unsigned char color, unsigned char flag )
        : Vehicle(player, id, initial_loc)
{
    setUnitProperties();

    if ( color == false) {
        body_anim.setData( gAbramsBody );
        turret_anim.setData( gAbramsTurret );
    } else {
        body_anim.setData( gAbramsBodyDarkBlue );
        turret_anim.setData( gAbramsTurretDarkBlue );
    }

    body_anim_shadow.setData( gAbramsBodyShadow );
    turret_anim_shadow.setData( gAbramsTurretShadow );


    body_anim.setAttrib( iXY(0,0), iXY(0,0), unitLayer );
    turret_anim.setAttrib( iXY(0,0), iXY(0,0), unitLayer );
    body_anim_shadow.setAttrib( iXY(0,0), iXY(0,0), unitLayer);
    turret_anim_shadow.setAttrib( iXY(0,0), iXY(0,0), unitLayer);

    select_info_box.setBoxAttributes( BoundBox( -30, -30, 30, 30), Color::blue );
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
unsigned short Abrams::launchProjectile()
{
    //SFX
    long distance = WorldViewInterface::getCameraDistance(unit_state.location);
    sound->playAmbientSound("ht-fire", distance );
    sound->playBattle();

    return Weapon::_shell;
}
//-----------------------------------------------------------------
void Abrams::soundSelected()
{
    sound->playSound("ht-selected");
}
