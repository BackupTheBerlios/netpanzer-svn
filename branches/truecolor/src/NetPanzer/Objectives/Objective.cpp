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

#include "Objectives/Objective.hpp"
#include "Outpost.hpp"
#include "Interfaces/PlayerInterface.hpp"
#include "Interfaces/ConsoleInterface.hpp"
#include "Units/UnitProfileInterface.hpp"
#include "Util/Log.hpp"

Objective::Objective(ObjectiveID ID, iXY location, BoundBox area)
{
    objective_state.ID = ID;
    objective_state.location = location;
    objective_state.capture_area = area;
    objective_state.objective_status = _objective_status_null;
    objective_state.occupation_status = _occupation_status_unoccupied;
    objective_state.occupying_player = 0;
}

void
Objective::objectiveMesgSync(const ObjectiveMessage* message)
{
    const SyncObjective *sync_mesg = (const SyncObjective*) message;

    if(sync_mesg->getOccupyingPlayerID() >= PlayerInterface::getMaxPlayers()) {
        LOGGER.warning("Malformed ObjectvieMesgSync");
        return;
    }

    objective_state.objective_status = sync_mesg->objective_status;
    objective_state.occupation_status = sync_mesg->occupation_status;
    if(objective_state.occupation_status != _occupation_status_unoccupied) {
        objective_state.occupying_player =
            PlayerInterface::getPlayer(sync_mesg->getOccupyingPlayerID());
    } else {
        objective_state.occupying_player = 0;
    }
}

void Objective::getSyncData(SyncObjective& objective_sync_mesg)
{
    Uint16 player_id = 0;
    if(objective_state.occupation_status != _occupation_status_unoccupied
            && objective_state.occupying_player) {
        player_id = objective_state.occupying_player->getID();
    }
    objective_sync_mesg.set(objective_state.ID,
                            objective_state.objective_status,
                            objective_state.occupation_status,
                            player_id);
}


void Objective::processMessage(const ObjectiveMessage* message)
{
    switch(message->message_type) {
        case _objective_mesg_sync_objective:
            objectiveMesgSync(message);
            break;

        case _objective_mesg_change_unit_generation:
        case _objective_mesg_disown_player_objective:
        case _objective_mesg_change_output_location:
        case _objective_mesg_update_occupation:
            break;

        default:
            LOGGER.warning("Unknown ObjectiveMessage received (type %d)",
                    message->message_type);
    }
}
