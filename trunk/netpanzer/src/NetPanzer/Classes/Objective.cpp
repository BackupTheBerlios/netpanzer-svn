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
#include "Objective.hpp"
#include "Outpost.hpp"
#include "PlayerInterface.hpp"
#include "ConsoleInterface.hpp"
#include "UnitProfileInterface.hpp"

Objective::Objective( short ID, iXY location, BoundBox area )
{
    objective_state.selection_state = false;
    objective_state.ID = ID;
    objective_state.location = location;
    objective_state.capture_area = area;
    objective_state.objective_status = _objective_status_null;
    objective_state.occupation_status = _occupation_status_unoccupied;
}

void Objective::objectiveMesgUpdateOccupation( ObjectiveMessage *message )
{
    const UpdateOccupationsStatus *occupation_update
        = (const UpdateOccupationsStatus *) message;

    objective_state.occupation_status = occupation_update->occupation_status;
    objective_state.occupying_player
        = PlayerInterface::getPlayerID(occupation_update->getOccupyingPlayerID());

    Outpost* outpost = dynamic_cast<Outpost*> (this);
    if(outpost) {
        outpost->unit_generation_on_flag = occupation_update->unit_gen_on;
        outpost->unit_generation_type = occupation_update->unit_type;
        UnitProfile* profile =
            UnitProfileInterface::getUnitProfile(
                    outpost->unit_generation_type );
        outpost->unit_generation_timer.changePeriod((float)profile->regen_time);
        outpost->unit_generation_timer.setTimeLeft(
                float(occupation_update->getTimeLeft()) / 128.0);
    }

    if( objective_state.occupation_status != _occupation_status_unoccupied ) {
        PlayerState *player_state;
        player_state = PlayerInterface::getPlayerState( objective_state.occupying_player );

        ConsoleInterface::postMessage( "'%s' has been occupied by '%s'",
                objective_state.name, player_state->getName().c_str() );
    }
}

void Objective::objectiveMesgSync( ObjectiveMessage *message )
{
    SyncObjective *sync_mesg;

    sync_mesg = (SyncObjective *) message;

    objective_state.objective_status = sync_mesg->objective_status;
    objective_state.occupation_status = sync_mesg->occupation_status;
    objective_state.occupying_player.setIndex(sync_mesg->getOccupyingPlayerID());
}


void Objective::getSyncData( SyncObjective &objective_sync_mesg )
{
    objective_sync_mesg.set( objective_state.ID,
                             objective_state.objective_status,
                             objective_state.occupation_status,
                             objective_state.occupying_player
                           );
}


void Objective::processMessage( ObjectiveMessage *message )
{
    switch( message->message_type ) {
    case _objective_mesg_update_occupation :
        objectiveMesgUpdateOccupation( message );
        break;

    case _objective_mesg_sync_objective :
        objectiveMesgSync( message );
        break;


    }

}
