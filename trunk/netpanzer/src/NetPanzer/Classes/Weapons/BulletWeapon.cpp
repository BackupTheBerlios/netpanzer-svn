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
#include "stdafx.hpp"
#include "BulletWeapon.hpp"
#include "UnitMessageTypes.hpp"
#include "UnitInterface.hpp"
#include "NetworkState.hpp"
#include "StaticsInterface.hpp"
#include "WorldViewInterface.hpp"
#include "DDHardSurface.hpp"
#include "dsound.hpp"
#include "ParticleInterface.hpp"
#include "WeaponGlobals.hpp"


float BulletWeapon::velocity = gBulletVelocity;

BulletWeapon::	BulletWeapon(UnitID &owner, unsigned short owner_type_id, unsigned short damage, PointXYi &start, PointXYi &end)
 : Weapon(owner, owner_type_id, damage, start, end)
{
	setSurface();
}

void BulletWeapon::setSurface()
{
}

void BulletWeapon::fsmFlight()
{
	boolean end_cycle = _FALSE;
	
	float dt = TimerInterface::getTimeSlice();

	do
	{
		switch( fsmFlight_state )
		{
			case _fsmFlight_idle :
			{
				end_cycle = _TRUE;
			} break;

			case _fsmFlight_in_flight :
			{
				if (path.increment(&location, velocity) == _TRUE )
				{
					fsmFlight_state = _fsmFlight_on_target;
					end_cycle = _TRUE;
				}
				else
				{
					end_cycle = _TRUE;
				}

			} break;

			case _fsmFlight_on_target :
			{
				UMesgWeaponHit weapon_hit;

				if ( NetworkState::status == _network_state_server )
				{
					weapon_hit.setHeader( _umesg_flag_broadcast );
					weapon_hit.message_id = _umesg_weapon_hit;
					weapon_hit.owner_id = owner_id;
					weapon_hit.hit_location = location;
					weapon_hit.damage_factor = damage_factor;
					UnitInterface::sendMessage( &weapon_hit );
				}

				fsmFlight_state = _fsmFlight_idle;
				lifecycle_status = _lifecycle_weapon_in_active;

				//SFX
				dsound.PlayAmbientSound( _not_applicable, 
				                         _hit_target, 
										 WorldViewInterface::getCameraDistance( location ) );

				// **  Particle Shit
				iXY loc = iXY( location.x, location.y );
				ParticleInterface::addDirtKick(loc);
				end_cycle = _TRUE;
			} break;

		}

	} while ( end_cycle == _FALSE ); 
}

void BulletWeapon::updateStatus( void )
 {
  if ( fsm_timer.count() )
   fsmFlight();
 
 }

void BulletWeapon::offloadGraphics( SpriteSorter &sorter )
 {
  //shell.setWorldPos( location );

  //sorter.addSprite( &shell );
 }
