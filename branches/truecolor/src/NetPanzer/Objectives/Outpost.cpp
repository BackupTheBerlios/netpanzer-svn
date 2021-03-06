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

#include "Core/GlobalGameState.hpp"
#include "Classes/PlayerState.hpp"
#include "Outpost.hpp"

#include "Units/Unit.hpp"
#include "Units/UnitInterface.hpp"
#include "Units/UnitProfileInterface.hpp"
#include "Interfaces/PlayerInterface.hpp"
#include "Interfaces/MapInterface.hpp"
#include "Interfaces/ConsoleInterface.hpp"
#include "Interfaces/GameConfig.hpp"

#include "Classes/Network/NetworkServer.hpp"
#include "Classes/Network/NetworkState.hpp"
#include "Classes/Network/UnitNetMessage.hpp"
#include "Classes/Network/ObjectiveNetMessage.hpp"
#include "Classes/UnitMessageTypes.hpp"
#include "Classes/PlacementMatrix.hpp"

Outpost::Outpost( ObjectiveID ID, iXY location, BoundBox area )
        : Objective( ID, location, area )
{
    MapInterface::pointXYtoMapXY( location, &outpost_map_loc );
    objective_state.selection_box.max = location + iXY( 64, 32 );
    objective_state.selection_box.min = location + iXY( -224, -128 );
    objective_state.area.min = iXY( -400, -144 );
    objective_state.area.max = iXY(  350,  240 );
    objective_state.outpost_type = 0;
    

    unit_generation_type = 0;
    occupation_status_timer.changePeriod( 3 );
    unit_generation_timer.changePeriod( 1 );
    unit_collection_loc = outpost_map_loc + iXY( 13, 13 );
    unit_generation_loc = iXY( 1, 3 );
    occupation_pad_offset = iXY( 224, 48 );
    unit_generation_on_flag = false;
}

void
Outpost::attemptOccupationChange( Uint16 player_id )
{
    ObjectiveOccupationUpdate update_mesg;
    int player_status;

    PlayerState* player = PlayerInterface::getPlayer( player_id );
    player_status = player->getStatus();

    if ( objective_state.occupation_status == _occupation_status_unoccupied )
    {
        if ( player_status == _player_state_active )
        {
            objective_state.occupying_player = player;
            objective_state.occupying_player->incObjectivesHeld();
            objective_state.occupation_status = _occupation_status_occupied;
            unit_collection_loc = outpost_map_loc + iXY( 13, 13 );
            
            update_mesg.status_update.set(objective_state.ID,
                        objective_state.occupation_status,
                        objective_state.occupying_player->getID(),
                        unit_generation_on_flag,
                        unit_generation_type,
                        Uint32(unit_generation_timer.getTimeLeft() * 128.0));
            NetworkServer::broadcastMessage(&update_mesg, sizeof(ObjectiveOccupationUpdate));

            const PlayerState *player_state = objective_state.occupying_player;
            ConsoleInterface::postMessage(Color::cyan, false, 0, "'%s' has been occupied by '%s'",
                    objective_state.name, player_state->getName().c_str() );
        }
    }
    else if (objective_state.occupying_player != player)
    {
        if( player_status == _player_state_active  )
        {
            objective_state.occupying_player->decObjectivesHeld();
            objective_state.occupying_player = player;
            objective_state.occupying_player->incObjectivesHeld();
            objective_state.occupation_status = _occupation_status_occupied;
            unit_collection_loc = outpost_map_loc + iXY( 13, 13 );
            
            update_mesg.status_update.set(
                    objective_state.ID,
                    objective_state.occupation_status,
                    objective_state.occupying_player->getID(),
                    unit_generation_on_flag,
                    unit_generation_type,
                    Uint32(unit_generation_timer.getTimeLeft() * 128));
            NetworkServer::broadcastMessage(&update_mesg,
                    sizeof(ObjectiveOccupationUpdate));
            const PlayerState *player = objective_state.occupying_player;
            ConsoleInterface::postMessage(Color::cyan, false, 0, "'%s' has been occupied by '%s'",
                    objective_state.name, player->getName().c_str() );
        }
    }
}

void
Outpost::checkOccupationStatus()
{
    if ( occupation_status_timer.count() )
    {
        iRect bounding_area;

        // This was used for previous ocupation checks
        //iXY occupation_pad_loc;
        //occupation_pad_loc = objective_state.location + occupation_pad_offset;
        //bounding_area = objective_state.capture_area.getAbsRect( occupation_pad_loc );
        
        bounding_area = objective_state.area.getAbsRect(objective_state.location);

        std::vector<Unit*> unitsInArea;
        if ( objective_state.occupation_status == _occupation_status_occupied )
        {
            std::vector<UnitID> playerunits;
            UnitBucketArray::queryPlayerUnitsInWorldRect(playerunits,
                                    bounding_area,
                                    objective_state.occupying_player->getID() );

            if ( ! playerunits.empty() )
            {
                return;
            }

            UnitBucketArray::queryNonPlayerUnitsInWorldRect(unitsInArea,
                                    bounding_area,
                                    objective_state.occupying_player->getID() );

        }
        else
        {
            UnitBucketArray::queryUnitsInWorldRect( unitsInArea, bounding_area );
        }

        if ( ! unitsInArea.empty() )
        {
            std::vector<Unit*>::iterator i = unitsInArea.begin();

            Uint16 ocu_player = (*i)->player->getID();

            while ( ++i != unitsInArea.end() )
            {
                if ( (*i)->player->getID() != ocu_player )
                {
                    // more than one player in area, must be alone to capture
                    return;
                }
            }

            attemptOccupationChange( ocu_player );
        }

    } // ** if occupation_status_timer.count()

}

void
Outpost::generateUnits()
{   // XXX CHECK
    if ( NetworkState::status == _network_state_server )
    {
        if ( (unit_generation_on_flag == true) &&
            (objective_state.occupation_status == _occupation_status_occupied) )
        {
            if( (unit_generation_timer.count() == true) )
            {
                Unit *unit;
                iXY gen_loc;
                gen_loc = outpost_map_loc + unit_generation_loc;

                unit = UnitInterface::createUnit(unit_generation_type,
                        gen_loc, objective_state.occupying_player->getID());

                if ( unit != 0 )
                {
                    UnitRemoteCreate create_mesg(unit->player->getID(),
                            unit->id, gen_loc.x, gen_loc.y,
                            unit_generation_type);
                    NetworkServer::broadcastMessage(&create_mesg, sizeof(UnitRemoteCreate));

                    UMesgAICommand ai_command;
                    PlacementMatrix placement_matrix;
                    iXY collection_loc, loc;

                    collection_loc = /*outpost_map_loc +*/ unit_collection_loc;

                    placement_matrix.reset( collection_loc );
                    placement_matrix.getNextEmptyLoc( &loc );

                    ai_command.setHeader( unit->id);
                    ai_command.setMoveToLoc( loc );
                    UnitInterface::sendMessage( &ai_command );
                }
            } // ** if
        } // ** if
    } // ** if ( objective_state.occupation_status == _occupation_status_unoccupied )
    else
    {
        if( (unit_generation_timer.count() == true) )
        {
            unit_generation_timer.reset();
        }
    }

}

void
Outpost::updateStatus()
{
    if ( NetworkState::status == _network_state_server
            && gameconfig->capturebases == true)
    {
        checkOccupationStatus();
    }

    generateUnits();

}

void
Outpost::objectiveMesgChangeOutputLocation(const ObjectiveMessage* message)
{
    ChangeOutputLocation *msg;
    msg = (ChangeOutputLocation *) message;
    iXY temp;
    MapInterface::pointXYtoMapXY(iXY(msg->getPointX(), msg->getPointY()),
            &temp );
    unit_collection_loc = temp;
}

void
Outpost::objectiveMesgChangeUnitGeneration(const ObjectiveMessage* message)
{
    ChangeUnitGeneration *unit_gen_mesg;
    UnitProfile *profile;

    unit_gen_mesg = (ChangeUnitGeneration *) message;

    unit_generation_type = unit_gen_mesg->unit_type;
    unit_generation_on_flag = unit_gen_mesg->unit_gen_on;

    profile = global_game_state->unit_profile_interface->getUnitProfile( unit_generation_type );
    unit_generation_timer.changePeriod( (float) profile->regen_time );
}

void
Outpost::objectiveMesgDisownPlayerObjective(const ObjectiveMessage* message)
{
    const DisownPlayerObjective *disown_mesg =
        (const DisownPlayerObjective *) message;

    if( objective_state.occupation_status == _occupation_status_occupied ) {
        if (disown_mesg->getDisownedPlayerID() ==
                objective_state.occupying_player->getID() ) {
            objective_state.occupation_status = _occupation_status_unoccupied;

            ObjectiveOccupationUpdate update_mesg;
            update_mesg.status_update.set( objective_state.ID,
                                           objective_state.occupation_status,
                                           0,
                                           false,
                                           0,
                                           0);

            NetworkServer::broadcastMessage(&update_mesg, sizeof(ObjectiveOccupationUpdate));

            unit_generation_type = 0;
            UnitProfile *profile
                = global_game_state->unit_profile_interface->getUnitProfile( unit_generation_type );
            unit_generation_timer.changePeriod( (float) profile->regen_time );
            unit_generation_on_flag = false;
        }
    }
}

void
Outpost::getOutpostStatus( OutpostStatus &status )
{
    UnitProfile *profile;

    status.unit_generation_type = unit_generation_type;
    status.unit_generation_on_off = unit_generation_on_flag;
    status.unit_collection_loc = unit_collection_loc;

    profile = global_game_state->unit_profile_interface->getUnitProfile( unit_generation_type );
    if ( profile )
        status.unit_generation_time = (float) profile->regen_time;
    else
        status.unit_generation_time = 0;

    if ( unit_generation_on_flag == true ) {
        status.unit_generation_time_remaining = unit_generation_timer.getTimeLeft();
    } else {
        status.unit_generation_time_remaining = 0;
    }

}

void
Outpost::objectiveMesgUpdateOccupation(const ObjectiveMessage* message)
{
    const UpdateOccupationsStatus *occupation_update
        = (const UpdateOccupationsStatus *) message;

    if ( objective_state.occupation_status == _occupation_status_occupied )
    {
        objective_state.occupying_player->decObjectivesHeld();
    }
    
    objective_state.occupation_status = occupation_update->occupation_status;
    objective_state.occupying_player
        = PlayerInterface::getPlayer(occupation_update->getOccupyingPlayerID());

    unit_generation_on_flag = occupation_update->unit_gen_on;
    unit_generation_type = occupation_update->unit_type;
    
    UnitProfile* profile = global_game_state->unit_profile_interface->getUnitProfile( unit_generation_type );
    
    unit_generation_timer.changePeriod((float)profile->regen_time);
    unit_generation_timer.setTimeLeft(
                                float(occupation_update->getTimeLeft()) / 128.0);
    unit_collection_loc = outpost_map_loc + iXY( 13, 13 );

    if( objective_state.occupation_status != _occupation_status_unoccupied )
    {
        objective_state.occupying_player->incObjectivesHeld();
        
        ConsoleInterface::postMessage(Color::cyan, false, 0, "'%s' has been occupied by '%s'",
                objective_state.name, objective_state.occupying_player->getName().c_str() );
    }
}

void
Outpost::processMessage(const ObjectiveMessage* message)
{
    switch(message->message_type)
    {
        case _objective_mesg_change_unit_generation:
            objectiveMesgChangeUnitGeneration(message);
            break;

        case _objective_mesg_disown_player_objective:
            objectiveMesgDisownPlayerObjective(message);
            break;
            
        case _objective_mesg_change_output_location:
            objectiveMesgChangeOutputLocation(message);
            break;
            
        case _objective_mesg_update_occupation:
            objectiveMesgUpdateOccupation(message);
            break;
            
    }

    Objective::processMessage(message);
}

void
Outpost::offloadGraphics( SpriteSorter &sorter )
{
}
