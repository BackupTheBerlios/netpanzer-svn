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
#include "BonusUnitPowerUp.hpp"

#include <stdlib.h>
#include "UnitTypes.hpp"
#include "UnitInterface.hpp"
#include "PlayerInterface.hpp"
#include "MapInterface.hpp"
#include "ConsoleInterface.hpp"

#include "Server.hpp"
#include "NetworkState.hpp"
#include "UnitNetMessage.hpp"
#include "PowerUpNetMessage.hpp"

#include "System/Sound.hpp"


SpritePacked BONUS_POWERUP_ANIM;
SpritePacked BONUS_POWERUP_ANIM_SHADOW;

BonusUnitPowerUp::BonusUnitPowerUp(iXY map_loc, int type)
        : PowerUp( map_loc, type )
{
    bonus_unit_type = rand() % _unit_type_humvee;

    bonus_unit_animation.setData( BONUS_POWERUP_ANIM );
    bonus_unit_animation.setAttrib( powerup_state.world_loc, iXY(0,0), 5 );

    bonus_unit_animation_shadow.setData( BONUS_POWERUP_ANIM_SHADOW );
    bonus_unit_animation_shadow.setAttrib( powerup_state.world_loc, iXY(0,0), 4 );
    bonus_unit_animation_shadow.setDrawModeBlend(&Palette::colorTableDarkenALot);

}


void BonusUnitPowerUp::spawnBonusUnits( UnitID &unit_id )
{
    UnitBase *unit;
    PlacementMatrix placement_matrix;
    PlayerID player_id;
    iXY map_loc;

    sound->playPowerUpSound();

    unit = UnitInterface::getUnit( unit_id );

    player_id = PlayerInterface::getPlayerID( unit_id.getPlayer() );

    MapInterface::pointXYtoMapXY( unit->unit_state.location, &map_loc );

    placement_matrix.reset( map_loc );


    for( int i = 0; i < 9; i++ ) {
        UnitBase *new_unit;
        iXY spawn_loc;

        placement_matrix.getNextEmptyLoc( &spawn_loc );

        new_unit = UnitInterface::createUnit(bonus_unit_type,
                                             spawn_loc,
                                             player_id );

        if ( new_unit != 0 ) {
            UnitRemoteCreate create_mesg(new_unit->unit_id, spawn_loc.x,
                spawn_loc.y, bonus_unit_type);
            SERVER->sendMessage( &create_mesg, sizeof( UnitRemoteCreate ), 0 );
        }

    }

    PowerUpHitMesg hit_mesg;
    hit_mesg.set( powerup_state.ID, unit_id, player_id );
    SERVER->sendMessage( &hit_mesg, sizeof( PowerUpHitMesg ), 0 );

    powerup_state.life_cycle_state = _power_up_lifecycle_state_inactive;

    PlayerID local_player_id;

    local_player_id = PlayerInterface::getLocalPlayerID();

    if( local_player_id == player_id ) {
        ConsoleInterface::postMessage( "YOU GOT A BONUS UNITS POWERUP" );
    }

}

void BonusUnitPowerUp::updateState( void )
{
    UnitID unit_id;

    if ( NetworkState::status == _network_state_server ) {
        if ( powerup_state.life_cycle_state == _power_up_lifecycle_state_active ) {
            if( isPowerUpHit( &unit_id ) == true ) {
                spawnBonusUnits( unit_id );
            }
        }
    }
}

void BonusUnitPowerUp::offloadGraphics( SpriteSorter &sorter )
{
    bonus_unit_animation.nextFrame();
    bonus_unit_animation_shadow.setFrame( bonus_unit_animation.getCurFrame() );

    sorter.addSprite( &bonus_unit_animation );
    sorter.addSprite( &bonus_unit_animation_shadow );
}

void BonusUnitPowerUp::onHit( PowerUpHitMesg *message  )
{
    PlayerID local_player_id;

    sound->playPowerUpSound();
    powerup_state.life_cycle_state = _power_up_lifecycle_state_inactive;

    local_player_id = PlayerInterface::getLocalPlayerID();

    if( local_player_id.getIndex() == message->player_id ) {
        ConsoleInterface::postMessage( "YOU GOT A BONUS UNITS POWERUP" );
    }

}
