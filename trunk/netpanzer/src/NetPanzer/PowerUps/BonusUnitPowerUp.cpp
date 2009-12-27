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

#include "BonusUnitPowerUp.hpp"
#include "Core/GlobalEngineState.hpp"
#include "Core/GlobalGameState.hpp"

#include <stdlib.h>
#include "Units/UnitTypes.hpp"
#include "Units/UnitInterface.hpp"
#include "Interfaces/PlayerInterface.hpp"
#include "Interfaces/MapInterface.hpp"
#include "Interfaces/ConsoleInterface.hpp"
#include "Units/UnitProfileInterface.hpp"

#include "Classes/PlacementMatrix.hpp"
#include "Classes/Network/NetworkServer.hpp"
#include "Classes/Network/NetworkState.hpp"
#include "Classes/Network/UnitNetMessage.hpp"
#include "Classes/Network/PowerUpNetMessage.hpp"

#include "System/Sound.hpp"

BonusUnitPowerUp::BonusUnitPowerUp(iXY map_loc, int type)
        : PowerUp( map_loc, type )
{
    bonus_unit_type = rand() % global_game_state->unit_profile_interface->getNumUnitTypes();
}


void BonusUnitPowerUp::onHit( Unit * unit )
{
    PlacementMatrix placement_matrix;
    iXY map_pos;

    global_engine_state->sound_manager->playPowerUpSound();

    MapInterface::pointXYtoMapXY( unit->unit_state.location, &map_pos );

    placement_matrix.reset( map_pos );


    for( int i = 0; i < 9; i++ )
    {
        Unit *new_unit;
        iXY spawn_loc;

        placement_matrix.getNextEmptyLoc( &spawn_loc );

        new_unit = UnitInterface::createUnit(bonus_unit_type,
                                             spawn_loc,
                                             unit->player->getID() );

        if ( new_unit != 0 )
        {
            UnitRemoteCreate create_mesg(new_unit->player->getID(),
                    new_unit->id, spawn_loc.x, spawn_loc.y, bonus_unit_type);
            NetworkServer::broadcastMessage( &create_mesg, sizeof( UnitRemoteCreate ));
        }

    }

    PowerUpHitMesg hit_mesg;
    hit_mesg.set( ID, unit->player->getID() );
    NetworkServer::broadcastMessage( &hit_mesg, sizeof( PowerUpHitMesg ));

    life_cycle_state = _power_up_lifecycle_state_inactive;

    if(unit->player == PlayerInterface::getLocalPlayer())
    {
        ConsoleInterface::postMessage(Color::unitAqua, false, 0, "YOU GOT A BONUS UNITS POWERUP" );
    }
}

void BonusUnitPowerUp::onHitMessage( PowerUpHitMesg *message  )
{
    global_engine_state->sound_manager->playPowerUpSound();
    life_cycle_state = _power_up_lifecycle_state_inactive;

    if( PlayerInterface::getLocalPlayerIndex() == message->getPlayerID() )
    {
        ConsoleInterface::postMessage( Color::unitAqua, false, 0, "YOU GOT A BONUS UNITS POWERUP" );
    }
}
