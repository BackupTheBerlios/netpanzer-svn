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
#include "UnitProfileInterface.hpp"
#include "Gdatstct.hpp"
#include "Color.hpp"
#include "ProjectileInterface.hpp"
#include "NetworkState.hpp"

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
 
Leopard::Leopard( PointXYi initial_loc, unsigned char color, unsigned char flag )
   : Vehicle( initial_loc )
 {
  setUnitProperties();

  if ( color == false)
   {
    body_anim.setData( gLeopardBody );
    turret_anim.setData( gLeopardTurret );
   }
  else
   {
    body_anim.setData( gLeopardBodyDarkBlue );
    turret_anim.setData( gLeopardTurretDarkBlue );
   }


  body_anim.setAttrib( PointXYi(0,0), PointXYi(0,0), unitLayer ); 
  turret_anim.setAttrib( PointXYi(0,0), PointXYi(0,0), unitLayer );

  body_anim_shadow.setData( gLeopardBodyShadow );
  turret_anim_shadow.setData( gLeopardTurretShadow ); 
  body_anim_shadow.setAttrib( PointXYi(0,0), PointXYi(0,0), unitLayer ); 
  turret_anim_shadow.setAttrib( PointXYi(0,0), PointXYi(0,0), unitLayer );

  select_info_box.setBoxAttributes( BoundBox( -25, -25, 25, 25), Color::blue );
  select_info_box.setBoxState( false );
  select_info_box.setFlag( flag );

  body_anim_shadow.attachSprite( &body_anim, PointXYi(0,0) );
  body_anim_shadow.attachSprite( &turret_anim_shadow, PointXYi(0,0) ); 
  body_anim_shadow.attachSprite( &turret_anim, PointXYi(0,0) ); 
  body_anim_shadow.attachSprite( &select_info_box, PointXYi(0,0) );

  //FLAGS_DBASE.get_sprite_index( flag, select_info_box.unit_flag );
  //FLAGS_DBASE.get_sprite_name( "allie", select_info_box.allie_flag );

 }

void  Leopard::fireWeapon( PointXYi &target_loc )
{
  reload_counter = 0;
  ProjectileInterface::newProjectile( 3, unit_state.unit_type, unit_id, unit_state.damage_factor, 
                                      unit_state.location, 
                                      target_loc
                                     );

  if ( NetworkState::status == _network_state_server )
   {
    FireWeaponOpcode fire_opcode;
    fire_opcode.opcode = _UNIT_OPCODE_FIRE_WEAPON;
    fire_opcode.unit_index = unit_id.getIndex();
	fire_opcode.player_index = unit_id.getPlayer();
    fire_opcode.x = target_loc.x;
    fire_opcode.y = target_loc.y;    
    sendOpcode( &fire_opcode );
   }
}
