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

#include "Util/Endian.hpp"
#include "ObjectiveMessageTypes.hpp"

void ChangeUnitGeneration::set(unsigned short id, unsigned char unit_type,
    bool unit_generation_on)
{
    setObjectiveID(id);
    ChangeUnitGeneration::unit_type = unit_type;
    unit_gen_on = unit_generation_on;
    message_type = _objective_mesg_change_unit_generation;
}

void DisownPlayerObjective::set( unsigned short id, PlayerID &disowned_player )
{
    setObjectiveID(id);
    disowned_player_id = htol16(disowned_player.getIndex());
    message_type = _objective_mesg_disown_player_objective;
}

uint16_t DisownPlayerObjective::getDisownedPlayerID() const
{
    return ltoh16(disowned_player_id);
}

void SyncObjective::set(unsigned short id,
              unsigned char objective_status,
              unsigned char occupation_status,
              PlayerID occupying_player)
{
    setObjectiveID(id);
    message_type = _objective_mesg_sync_objective;

    SyncObjective::objective_status = objective_status;
    SyncObjective::occupation_status = occupation_status;
    SyncObjective::occupying_player_id = occupying_player.getIndex();
}

uint16_t SyncObjective::getOccupyingPlayerID() const
{
    return ltoh16(occupying_player_id);
}

void ChangeOutputLocation::set(unsigned short id, iXY point)
{
    setObjectiveID(id);
    new_point_x = htol32(point.x);
    new_point_y = htol32(point.y);
    message_type = _objective_mesg_change_output_location;
}

int32_t ChangeOutputLocation::getPointX() const
{
    return ltoh32(new_point_x);
}

int32_t ChangeOutputLocation::getPointY() const
{
    return ltoh32(new_point_y);
}

