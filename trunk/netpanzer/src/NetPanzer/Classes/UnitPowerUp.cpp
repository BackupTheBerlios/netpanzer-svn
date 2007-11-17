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
#include "UnitPowerUp.hpp"

#include <stdlib.h>
#include "Structs/UnitTypes.hpp"
#include "Interfaces/UnitInterface.hpp"
#include "Classes/UnitMessageTypes.hpp"
#include "Interfaces/PlayerInterface.hpp"
#include "Interfaces/MapInterface.hpp"
#include "Interfaces/ConsoleInterface.hpp"

#include "Interfaces/Server.hpp"
#include "Classes/Network/NetworkServer.hpp"
#include "Classes/Network/NetworkState.hpp"
#include "Classes/Network/UnitNetMessage.hpp"
#include "Classes/Network/PowerUpNetMessage.hpp"

#include "System/Sound.hpp"
#include "Classes/Network/NetworkServer.hpp"


enum { _unit_powerup_hitpoints,
       _unit_powerup_range,
       _unit_powerup_firepower,
       _unit_powerup_speed,
       _unit_powerup_repair,
       _unit_powerup_reload,
       _unit_powerup_destruct,
       _unit_powerup_enum_count
     };

SpritePacked UNIT_POWERUP_ANIM;
SpritePacked UNIT_POWERUP_ANIM_SHADOW;

UnitPowerUp::UnitPowerUp(iXY map_loc, int type)
        : PowerUp( map_loc, type )
{
    unit_powerup_type = rand() % _unit_powerup_enum_count;
    unit_powerup_animation.setData( UNIT_POWERUP_ANIM );
    unit_powerup_animation.setAttrib( powerup_state.world_loc, iXY(0,0), 5 );

    unit_powerup_animation_shadow.setData( UNIT_POWERUP_ANIM_SHADOW );
    unit_powerup_animation_shadow.setAttrib( powerup_state.world_loc, iXY(0,0), 4 );
    unit_powerup_animation_shadow.setDrawModeBlend(&Palette::colorTableDarkenALot);

}

void UnitPowerUp::powerUpHitPoints( UnitState *unit_state)
{
    /*
    lua_Object hit_points;

    lua_pushnumber( (double) (unit_state->hit_points) );
    lua_pushnumber( (double) (unit_state->max_hit_points) );
    lua_callfunction( lua_getglobal("UnitPowerUpHitPoints") ); 
    hit_points = lua_getresult(1);
    unit_state->hit_points = (short) lua_getnumber( hit_points ); 
    unit_state->max_hit_points = unit_state->hit_points; 
    */
    float percent = (rand() % 100) / 100;
    int new_hitpoints = int(unit_state->max_hit_points +
			    (unit_state->max_hit_points * percent));
    unit_state->hit_points = new_hitpoints;
    unit_state->max_hit_points = new_hitpoints;

}

void UnitPowerUp::powerUpRange( UnitState *unit_state)
{
    float percent = (((rand() % 100) / 100) * 50) / 100;
    int new_weapon_range = int(unit_state->weapon_range +
			      (unit_state->weapon_range * percent));
    unit_state->weapon_range = new_weapon_range;
}

void UnitPowerUp::powerUpFirePower( UnitState *unit_state)
{
    int max_percent_increase = 50;
    float percent = (((rand() % 100) / 100) * max_percent_increase) / 100;

    int new_damage_factor = int(unit_state->damage_factor +
	    (unit_state->damage_factor * percent));
    unit_state->damage_factor = new_damage_factor;
}

void UnitPowerUp::powerUpSpeed( UnitState *unit_state)
{
    int new_speed_factor = int(unit_state->speed_factor + 
	    floor( (((rand() % 100) / 100) * 5) ));
    unit_state->speed_factor = new_speed_factor;
}

void UnitPowerUp::powerUpRepair( UnitState *unit_state)
{
    unit_state->hit_points = unit_state->max_hit_points;
}

void UnitPowerUp::powerUpReload( UnitState *unit_state)
{
    int max_percent_decrease = 50;
    float percent = (((rand() % 100) / 100) * max_percent_decrease) / 100;

    int new_reload_time = int(unit_state->reload_time -
			     (unit_state->reload_time * percent));
    unit_state->reload_time = new_reload_time;
}

void UnitPowerUp::powerUpDestruct( UnitID unit_id )
{
    UMesgSelfDestruct self_destruct;
    self_destruct.setHeader( unit_id, _umesg_flag_unique );
    UnitInterface::sendMessage( &self_destruct );
}


void UnitPowerUp::selectPowerUp( UnitID &unit_id )
{
    sound->playPowerUpSound();

    UnitBase* unit = UnitInterface::getUnit( unit_id );

    switch( unit_powerup_type ) {
    case _unit_powerup_hitpoints :
        powerUpHitPoints( &(unit->unit_state) );
        break;

    case _unit_powerup_range :
        powerUpRange( &(unit->unit_state) );
        break;

    case _unit_powerup_firepower :
        powerUpFirePower( &(unit->unit_state) );
        break;

    case _unit_powerup_speed :
        powerUpSpeed( &(unit->unit_state) );
        break;

    case _unit_powerup_repair :
        powerUpRepair( &(unit->unit_state) );
        break;

    case _unit_powerup_reload :
        powerUpReload( &(unit->unit_state) );
        break;

    case _unit_powerup_destruct :
        powerUpDestruct( unit_id );
        break;
    }

    PowerUpHitMesg hit_mesg;
    hit_mesg.set(powerup_state.ID, unit->player->getID(), unit_powerup_type);
    SERVER->sendMessage(&hit_mesg, sizeof(PowerUpHitMesg));

    powerup_state.life_cycle_state = _power_up_lifecycle_state_inactive;

    if(unit->player == PlayerInterface::getLocalPlayer()) {
        ConsoleInterface::postMessage(Color::unitAqua, "YOU GOT A %s POWERUP", powerupTypeToString( unit_powerup_type ) );
    }
}

char * UnitPowerUp::powerupTypeToString( int type )
{
    switch( type ) {
    case _unit_powerup_hitpoints :
        return( "UNIT HITPOINTS" );

    case _unit_powerup_range :
        return( "UNIT WEAPON RANGE" );

    case _unit_powerup_firepower :
        return( "UNIT FIREPOWER" );

    case _unit_powerup_speed :
        return( "UNIT SPEED" );

    case _unit_powerup_repair :
        return( "UNIT REPAIR" );

    case _unit_powerup_reload :
        return( "UNIT RELOAD TIME" );

    case _unit_powerup_destruct :
        return( "UNIT DESTRUCT" );
    }

    return("");
}


void UnitPowerUp::updateState( void )
{
    UnitID unit_id;

    if ( NetworkState::status == _network_state_server ) {
        if ( powerup_state.life_cycle_state == _power_up_lifecycle_state_active ) {
            if( isPowerUpHit( &unit_id ) == true ) {
                selectPowerUp( unit_id );
            }
        }
    }
}

void UnitPowerUp::offloadGraphics( SpriteSorter &sorter )
{
    unit_powerup_animation.nextFrame();
    unit_powerup_animation_shadow.setFrame( unit_powerup_animation.getCurFrame() );

    sorter.addSprite( &unit_powerup_animation );
    sorter.addSprite( &unit_powerup_animation_shadow );
}

void UnitPowerUp::onHit( PowerUpHitMesg *message  )
{
    PlayerID local_player_id;

    sound->playPowerUpSound();
    powerup_state.life_cycle_state = _power_up_lifecycle_state_inactive;

    local_player_id = PlayerInterface::getLocalPlayerID();

    if( local_player_id.getIndex() == message->getPlayerID() ) {
        ConsoleInterface::postMessage(Color::unitAqua, "YOU GOT A %s POWERUP", powerupTypeToString( message->getUnitPowerupType() ) );
    }

}
