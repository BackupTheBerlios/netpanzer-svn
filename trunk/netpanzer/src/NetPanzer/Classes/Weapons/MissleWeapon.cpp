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

#include "MissleWeapon.hpp"
#include "UnitMessageTypes.hpp"
#include "UnitInterface.hpp"
#include "NetworkState.hpp"
#include "WorldViewInterface.hpp"
#include "Sound.hpp"
#include "ParticleInterface.hpp"
#include "WeaponGlobals.hpp"
#include "Math.hpp"
#include "GameConfig.hpp"
#include "FlashParticle2D.hpp"


float MissleWeapon::thrustForce = gMissleThrustForce;

MissleWeapon::MissleWeapon(UnitID &owner, unsigned short owner_type_id, unsigned short damage, iXY &start, iXY &end)
        : Weapon(owner, owner_type_id, damage, start, end)
{
    velocity     = thrustForce * 2;
    acceleration = 0.0f;
    launched     = false;
    curWait      = 0.0f;
    totalWait    = 0.0f;

    // Set the correct missle type.
    if (owner_type_id == _unit_type_archer) {
        shell.setData(gMissleMediumPackedSurface);
        shellShadow.setData(gMissleMediumPackedSurface);

    } else {
        shell.setData(gMissleSmallPackedSurface);
        shellShadow.setData(gMissleSmallPackedSurface);
    }

    shell.setFrame(getGoalAngle(start, end));
    shell.setAttrib(location, weaponLayer);

    shellShadow.setDrawModeBlend(&Palette::colorTableDarkenALittle);
    shellShadow.setFrame(getGoalAngle(start, end));
    shellShadow.setAttrib(location, weaponShadowLayer);

    thrust.setDrawModeBlend(&Palette::colorTableBrighten);
    thrust.setData(gMissleThrustPackedSurface);
    thrust.setAttrib(location, weaponPuffLayer);

    groundLight.setDrawModeBlend(&Palette::colorTableBrighten);
    groundLight.setData(gMissleGroundLightPackedSurface);
    groundLight.setAttrib(location, weaponPuffLayer);
}

void MissleWeapon::fsmFlight()
{
    bool end_cycle = false;

    float dt = TimerInterface::getTimeSlice();

    do {
        switch( fsmFlight_state ) {
        case _fsmFlight_idle : {
                end_cycle = true;
            }
            break;

        case _fsmFlight_in_flight : {
                if (velocity < gMissleMaxVelocity) {
                    acceleration += thrustForce * dt;
                    velocity     += acceleration * dt;
                } else {
                    velocity = gMissleMaxVelocity;
                }


                if (path.increment(&location, (short) velocity) == true ) {
                    fsmFlight_state = _fsmFlight_on_target;
                    end_cycle = true;
                } else {
                    end_cycle = true;
                }

                if (!launched) {
                    launched = true;
                }

                // **  Particle Shit
                ParticleInterface::addMissleFlightPuff(location, direction, curWait, totalWait, Weapon::owner_type_id);

            }
            break;

        case _fsmFlight_on_target : {
                UMesgWeaponHit weapon_hit;

                if ( NetworkState::status == _network_state_server ) {
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
                sound->playAmbientSound("hit",
                                        WorldViewInterface::getCameraDistance( location ) );

                // **  Particle Shit
                ParticleInterface::addMiss(location, Weapon::owner_type_id);
                end_cycle = true;
            }
            break;

        }

    } while ( end_cycle == false );
}

void MissleWeapon::updateStatus( void )
{
    if ( fsm_timer.count() ) {
        fsmFlight();
    }

    shell.setWorldPos(location);

    iXY thrustOffset(int(-10.0f * direction.x), int(-10.0f * direction.y));

    thrust.setWorldPos(location + thrustOffset);

    int heightFromGround = 14;

    groundLight.setWorldPos(location.x + thrustOffset.x, location.y + thrustOffset.y + heightFromGround);

    if (gameconfig->displayshadows) {
        shellShadow.setWorldPos(location.x - 20, location.y);
    }
}

void MissleWeapon::offloadGraphics(SpriteSorter &sorter)
{
    sorter.addSprite(&shell);
    sorter.addSprite(&thrust);
    //sorter.addSprite(&groundLight);

    if (gameconfig->displayshadows) {
        sorter.addSprite(&shellShadow);
    }
}

