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
#include "Structs/UnitTypes.hpp"
#include "Interfaces/UnitInterface.hpp"
#include "Interfaces/PlayerInterface.hpp"
#include "Interfaces/MapInterface.hpp"
#include "Interfaces/ConsoleInterface.hpp"
#include "Interfaces/UnitProfileInterface.hpp"

#include "Classes/Network/NetworkServer.hpp"
#include "Classes/Network/NetworkState.hpp"
#include "Classes/Network/UnitNetMessage.hpp"
#include "Classes/Network/PowerUpNetMessage.hpp"

#include "System/Sound.hpp"


SpritePacked BONUS_POWERUP_ANIM;
SpritePacked BONUS_POWERUP_ANIM_SHADOW;

BonusUnitPowerUp::BonusUnitPowerUp(iXY map_loc, int type)
        : PowerUp( map_loc, type )
{
    bonus_unit_type = rand() % UnitProfileInterface::getNumUnitTypes();

    bonus_unit_animation.setData( BONUS_POWERUP_ANIM );
    bonus_unit_animation.setAttrib( world_loc, iXY(0,0), 5 );

    bonus_unit_animation_shadow.setData( BONUS_POWERUP_ANIM_SHADOW );
    bonus_unit_animation_shadow.setAttrib( world_loc, iXY(0,0), 4 );
    bonus_unit_animation_shadow.setDrawModeBlend(&Palette::colorTableDarkenALot);

}


void BonusUnitPowerUp::spawnBonusUnits( UnitID &unit_id )
{
    PlacementMatrix placement_matrix;
    iXY map_pos;

    sound->playPowerUpSound();

    UnitBase* unit = UnitInterface::getUnit(unit_id);

    MapInterface::pointXYtoMapXY( unit->unit_state.location, &map_pos );

    placement_matrix.reset( map_pos );


    for( int i = 0; i < 9; i++ )
    {
        UnitBase *new_unit;
        iXY spawn_loc;

        placement_matrix.getNextEmptyLoc( &spawn_loc );

        new_unit = UnitInterface::createUnit(bonus_unit_type,
                                             spawn_loc,
                                             unit->player->getID() );

        if ( new_unit != 0 )
        {
            UnitRemoteCreate create_mesg(new_unit->player->getID(),
                    new_unit->id, spawn_loc.x, spawn_loc.y, bonus_unit_type);
            SERVER->broadcastMessage( &create_mesg, sizeof( UnitRemoteCreate ));
        }

    }

    PowerUpHitMesg hit_mesg;
    hit_mesg.set( ID, unit->player->getID() );
    SERVER->broadcastMessage( &hit_mesg, sizeof( PowerUpHitMesg ));

    life_cycle_state = _power_up_lifecycle_state_inactive;

    if(unit->player == PlayerInterface::getLocalPlayer())
    {
        ConsoleInterface::postMessage(Color::unitAqua, "YOU GOT A BONUS UNITS POWERUP" );
    }
}

void BonusUnitPowerUp::updateState( void )
{
    UnitID unit_id;

    if ( NetworkState::status == _network_state_server )
    {
        if ( life_cycle_state == _power_up_lifecycle_state_active )
        {
            if( isPowerUpHit( &unit_id ) == true )
            {
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
    sound->playPowerUpSound();
    life_cycle_state = _power_up_lifecycle_state_inactive;

    if( PlayerInterface::getLocalPlayerIndex() == message->getPlayerID() )
    {
        ConsoleInterface::postMessage( Color::unitAqua, "YOU GOT A BONUS UNITS POWERUP" );
    }
}
