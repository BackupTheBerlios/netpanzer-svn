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
#include "Humvee.hpp"
#include "UnitProfileInterface.hpp"
#include "Color.hpp"
#include "WorldViewInterface.hpp"
#include "Weapon.hpp"
#include "Sound.hpp"
#include "GameConfig.hpp"

#include "UnitGlobals.hpp"

// NOTE: Temp unit new sprites put in 
#include "DDHardSurface.hpp"

void Humvee::setUnitProperties( void )
 {
  UnitProfile *profile;
  
  profile = UnitProfileInterface::getUnitProfile( _unit_type_humvee ); 

  unit_state.hit_points = profile->hit_points;
  unit_state.max_hit_points = profile->hit_points;
  unit_state.damage_factor = profile->attack_factor;
  unit_state.defend_range = profile->defend_range;
  unit_state.speed_factor = profile->speed_factor;
  unit_state.speed_rate = profile->speed_rate;
  unit_state.reload_time = profile->reload_time;
  unit_state.weapon_range = profile->attack_range;
  unit_state.unit_type = _unit_type_humvee;
  select_info_box.setHitBarAttributes( profile->hit_points, Color::yellow );

 }
 
/*
void Humvee::offloadGraphics( SpriteSorter &sorter )
 {
  body_anim_shadow.setWorldPos( unit_state.location);

  if ( sorter.cullSprite( body_anim_shadow ) == false )
   {  
    body_anim.setWorldPos( unit_state.location );
    body_anim.setFrame( unit_state.body_angle.angle_int );

    body_anim_shadow.setFrame( unit_state.body_angle.angle_int );

    select_info_box.setBoxState( unit_state.select );
    select_info_box.setAttrib( unit_state.location, 1 );
    select_info_box.setHitPoints( unit_state.hit_points );
   
    sorter.forceAddSprite( &body_anim_shadow );
   }
       
  //sorter.addSprite( &body_anim_shadow );
 }
*/
 
Humvee::Humvee( iXY initial_loc, unsigned char color, unsigned char flag )
   : Vehicle( initial_loc )
 {
  setUnitProperties();
  
  if ( color == false)
   {
    body_anim.setData( gSpahPanzerBody );
    turret_anim.setData( gSpahPanzerTurret );
   }
  else
   {
    body_anim.setData( gSpahPanzerBodyDarkBlue );
    turret_anim.setData( gSpahPanzerTurretDarkBlue );
   }  

  body_anim_shadow.setData( gSpahPanzerBodyShadow );
  turret_anim_shadow.setData( gSpahPanzerTurretShadow );


  body_anim.setAttrib( iXY(0,0), iXY(0,0), unitLayer ); 
  turret_anim.setAttrib( iXY(0,0), iXY(0,0), unitLayer ); 
  body_anim_shadow.setAttrib( iXY(0,0), iXY(0,0), unitLayer);
  turret_anim_shadow.setAttrib( iXY(0,0), iXY(0,0), unitLayer); 

  select_info_box.setBoxAttributes( BoundBox( -20, -20, 20, 20), Color::blue );
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
unsigned short Humvee::launchProjectile()
{
	//SFX
	long distance = WorldViewInterface::getCameraDistance(unit_state.location);
	sound->playAmbientSound("lt-fire", distance );
	sound->playBattle();

	return Weapon::_shell;
}
