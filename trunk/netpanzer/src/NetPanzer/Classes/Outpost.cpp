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
#include "Outpost.hpp"

#include "UnitInterface.hpp"
#include "UnitProfileInterface.hpp"
#include "PlayerInterface.hpp"
#include "MapInterface.hpp"
#include "ConsoleInterface.hpp"

#include "Server.hpp"
#include "NetworkState.hpp"
#include "UnitNetMessage.hpp"
#include "ObjectiveNetMessage.hpp"
#include "UnitMessageTypes.hpp"
Outpost::Outpost( short ID, iXY location, BoundBox area )
        : Objective( ID, location, area )
{
    MapInterface::pointXYtoMapXY( location, &outpost_map_loc );
    objective_state.selection_box.max = location + iXY( 64, 32 );
    objective_state.selection_box.min = location + iXY( -224, -128 );
    objective_state.area.min = iXY( -400, -144 );
    objective_state.area.max = iXY(  400,  240 );
    objective_state.outpost_type = 0;
    

    unit_generation_type = _unit_type_humvee;
    occupation_status_timer.changePeriod( 3 );
    unit_generation_timer.changePeriod( 1 );
    unit_collection_loc = outpost_map_loc + iXY( 13, 13 );
    unit_generation_loc = iXY( 1, 3 );
    occupation_pad_offset = iXY( 224, 48 );
    unit_generation_on_flag = false;

    select_box.setAttrib( location, 3 );
    select_box.setBoxAttributes( area, 252 );
}


void Outpost::attemptOccupationChange( UnitID &unit_id )
{
    ObjectiveOccupationUpdate update_mesg;
    PlayerState *player_state_ptr;
    int player_index;
    int player_status;

    player_index = unit_id.getPlayer();
    player_state_ptr = PlayerInterface::getPlayerState( player_index );

    if ( player_state_ptr != 0 ) {
        player_status = player_state_ptr->getStatus();
    } else {
        player_status  = _player_state_free;
    }

    if ( objective_state.occupation_status == _occupation_status_unoccupied ) {
        if ( player_status == _player_state_active ) {
            objective_state.occupying_player = PlayerInterface::getPlayerID( player_index );
            objective_state.occupation_status = _occupation_status_occupied;
            update_mesg.status_update.set( objective_state.ID,
                                           objective_state.occupation_status,
                                           objective_state.occupying_player
                                         );
            SERVER->sendMessage( &update_mesg, sizeof( ObjectiveOccupationUpdate ), 0 );

            PlayerState *player_state;
            player_state = PlayerInterface::getPlayerState( objective_state.occupying_player );

            ConsoleInterface::postMessage( "%s has been occupied by %s",
                    objective_state.name, player_state->getName().c_str() );
        }
    } else {
        if ( unit_id.getPlayer() != objective_state.occupying_player.getIndex() ) {
            if( player_status == _player_state_active  ) {
                objective_state.occupying_player = PlayerInterface::getPlayerID( player_index );
                objective_state.occupation_status = _occupation_status_occupied;
                update_mesg.status_update.set( objective_state.ID,
                                               objective_state.occupation_status,
                                               objective_state.occupying_player
                                             );
                SERVER->sendMessage( &update_mesg, sizeof( ObjectiveOccupationUpdate ), 0 );

                PlayerState *player_state;
                player_state = PlayerInterface::getPlayerState( objective_state.occupying_player );

                ConsoleInterface::postMessage( "%s has been occupied by %s",
                        objective_state.name, player_state->getName().c_str() );
            }
        }
    }
}

void Outpost::checkOccupationStatus( void )
{
    if( occupation_status_timer.count()  )	//
    {
        UnitBase *unit_ptr;
        iRect bounding_area;
        iXY occupation_pad_loc;

        occupation_pad_loc = objective_state.location + occupation_pad_offset;
        bounding_area = objective_state.capture_area.getAbsRect( occupation_pad_loc );


        UnitInterface::quearyClosestUnit( &unit_ptr,
                                          bounding_area,
                                          occupation_pad_loc
                                        );

        if ( unit_ptr != 0 )
        {
            iXY unit_loc;
            unit_loc = unit_ptr->unit_state.location;
            if ( objective_state.capture_area.bounds( occupation_pad_loc, unit_loc ) ) {
                attemptOccupationChange( unit_ptr->unit_id );
            }

        } // ** if unit_ptr != 0

    } // ** if occupation_status_timer.count()

}


void Outpost::generateUnits( void )
{
    if ( NetworkState::status == _network_state_server ) {

        if ( (unit_generation_type != _unit_type_null) &&
                (objective_state.occupation_status == _occupation_status_occupied) ) {
            if( (unit_generation_timer.count() == true) &&
                    (unit_generation_on_flag == true)
              ) {
                UnitBase *unit;
                iXY gen_loc;
                gen_loc = outpost_map_loc + unit_generation_loc;

                unit = UnitInterface::createUnit(unit_generation_type,
                                                 gen_loc,
                                                 objective_state.occupying_player );

                if ( unit != 0 ) {
                    UnitRemoteCreate create_mesg;

                    create_mesg.new_unit_id = unit->unit_id;
                    create_mesg.location = gen_loc;
                    create_mesg.unit_type = unit_generation_type;
                    SERVER->sendMessage( &create_mesg, sizeof( UnitRemoteCreate ), 0 );

                    UMesgAICommand ai_command;
                    PlacementMatrix placement_matrix;
                    iXY collection_loc, loc;

                    collection_loc = /*outpost_map_loc +*/ unit_collection_loc;

                    placement_matrix.reset( collection_loc );
                    placement_matrix.getNextEmptyLoc( &loc );

                    ai_command.setHeader( unit->unit_id, _umesg_flag_unique );
                    ai_command.setMoveToLoc( loc );
                    UnitInterface::sendMessage( &ai_command );
                }

            } // ** if
        } // ** if
    } // ** if ( objective_state.occupation_status == _occupation_status_unoccupied )
    else {
        if( (unit_generation_timer.count() == true) ) {
            unit_generation_timer.reset();
        }
    }

}


void Outpost::updateStatus( void )
{
    if ( NetworkState::status == _network_state_server ) {
        checkOccupationStatus();
    }

    generateUnits();

}

void Outpost::objectiveMesgChangeOutputLocation( ObjectiveMessage *message ){
    ChangeOutputLocation *msg;
    msg = (ChangeOutputLocation *) message;
    iXY temp;
    MapInterface::pointXYtoMapXY( msg->new_point, &temp );
    unit_collection_loc = temp;
}

void Outpost::objectiveMesgChangeUnitGeneration( ObjectiveMessage *message )
{
    ChangeUnitGeneration *unit_gen_mesg;
    UnitProfile *profile;

    unit_gen_mesg = (ChangeUnitGeneration *) message;

    unit_generation_type = unit_gen_mesg->unit_type;
    unit_generation_on_flag = unit_gen_mesg->unit_gen_on;

    profile = UnitProfileInterface::getUnitProfile( unit_generation_type );
    unit_generation_timer.changePeriod( (float) profile->regen_time );
}

void Outpost::objectiveMesgDisownPlayerObjective( ObjectiveMessage *message )
{
    DisownPlayerObjective *disown_mesg;

    disown_mesg = (DisownPlayerObjective *) message;

    if( objective_state.occupation_status == _occupation_status_occupied ) {
        if ( disown_mesg->disowned_player_id == objective_state.occupying_player ) {
            objective_state.occupation_status = _occupation_status_unoccupied;

            ObjectiveOccupationUpdate update_mesg;
            update_mesg.status_update.set( objective_state.ID,
                                           objective_state.occupation_status,
                                           objective_state.occupying_player
                                         );

            SERVER->sendMessage( &update_mesg, sizeof( ObjectiveOccupationUpdate ), 0 );

            unit_generation_type = _unit_type_humvee;

            UnitProfile *profile;

            profile = UnitProfileInterface::getUnitProfile( unit_generation_type );
            unit_generation_timer.changePeriod( (float) profile->regen_time );

            unit_generation_on_flag = false;

        } // ** if

    } // ** if

}

void Outpost::getOutpostStatus( OutpostStatus &status )
{
    UnitProfile *profile;

    status.unit_generation_type = unit_generation_type;
    status.unit_generation_on_off = unit_generation_on_flag;
    status.unit_collection_loc = unit_collection_loc;

    if ( unit_generation_type != _unit_type_null ) {
        profile = UnitProfileInterface::getUnitProfile( unit_generation_type );
        status.unit_generation_time = (float) profile->regen_time;
    } else {
        status.unit_generation_time = 0;
    }

    if ( unit_generation_on_flag == true ) {
        status.unit_generation_time_remaining = unit_generation_timer.getTimeLeft();
    } else {
        status.unit_generation_time_remaining = 0;
    }

}

void Outpost::processMessage( ObjectiveMessage *message )
{
    switch( message->message_type ) {
    case _objective_mesg_change_unit_generation :
        objectiveMesgChangeUnitGeneration( message );
        break;

    case _objective_mesg_disown_player_objective :
        objectiveMesgDisownPlayerObjective( message );
        break;
    case _objective_mesg_change_output_location :
        objectiveMesgChangeOutputLocation( message );
        break;
    } // ** switch

    Objective::processMessage( message );
}

void Outpost::offloadGraphics( SpriteSorter &sorter )
{
    if( objective_state.selection_state == true ) {
        sorter.addSprite( &select_box );
    }
}
