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
#ifndef _OBJECTIVE_MESSSAGE_TYPES_HPP
#define _OBJECTIVE_MESSSAGE_TYPES_HPP

#include "SDL_endian.h"

#include "Core/CoreTypes.hpp"
#include "Classes/PlayerState.hpp"
#include "Types/iXY.hpp"

#ifdef MSVC
#pragma pack(1)
#endif

class ObjectiveMessage
{
public:
    Uint8 message_type;
private:
    ObjectiveID objective_id;

public:
    ObjectiveID getObjectiveID() const
    {
        return ObjectiveID_fromPortable(objective_id); // XXX protocol
    }
    void setObjectiveID(ObjectiveID id)
    {
        objective_id = ObjectiveID_toPortable(id); // XXX protocol
    }
} __attribute__((packed));

enum { _objective_mesg_update_occupation,
       _objective_mesg_change_unit_generation,
       _objective_mesg_disown_player_objective,
       _objective_mesg_sync_objective,
       _objective_mesg_change_output_location
     };

class UpdateOccupationsStatus : public ObjectiveMessage
{
public:
    Uint8 occupation_status;
private:
    Uint16 occupying_player_id;
    Uint32 timeleft;

public:
    Uint8  unit_type;
    bool   unit_gen_on;
        
    void set(ObjectiveID id, Uint8 status, Uint16 player_id,
            bool unit_gen_on, Uint8 unit_type, Uint32 timeleft)
    {
        message_type = _objective_mesg_update_occupation;
        setObjectiveID(id);
        occupation_status = status;
        occupying_player_id = SDL_SwapLE16(player_id);
        this->unit_gen_on = unit_gen_on;
        this->timeleft = SDL_SwapLE32(timeleft);
        this->unit_type = unit_type;
    }
   
    Uint16 getOccupyingPlayerID() const
    {
        return SDL_SwapLE16(occupying_player_id);
    }
    Uint32 getTimeLeft() const
    {
        return SDL_SwapLE32(timeleft);
    }
} __attribute__((packed));

class ChangeUnitGeneration : public ObjectiveMessage
{
public:
    Uint8 unit_type;
    Uint8 unit_gen_on;

    void set(ObjectiveID id, Uint8 unit_type, bool unit_generation_on)
    {
        setObjectiveID(id);
        this->unit_type = unit_type;
        unit_gen_on = unit_generation_on;                             
        message_type = _objective_mesg_change_unit_generation;
    }
}
__attribute__((packed));

class DisownPlayerObjective : public ObjectiveMessage
{
private:
    Uint16 disowned_player_id;

public:
    void set(ObjectiveID id, Uint16 disowned_player)
    {
        setObjectiveID(id);
        disowned_player_id = SDL_SwapLE16(disowned_player);
        message_type = _objective_mesg_disown_player_objective;        
    }
    Uint16 getDisownedPlayerID() const
    {
        return SDL_SwapLE16(disowned_player_id);
    }
} __attribute__((packed));

class SyncObjective : public ObjectiveMessage
{
public:
    Uint8 objective_status;
    Uint8 occupation_status;
private:
    Uint16 occupying_player_id;

public:
    void set(ObjectiveID id, Uint8 objective_status,
            Uint8 occupation_status, Uint16 occupying_player)
    {
        setObjectiveID(id);
        message_type = _objective_mesg_sync_objective;
                                                                             
        this->objective_status = objective_status;
        this->occupation_status = occupation_status;
        this->occupying_player_id = SDL_SwapLE16(occupying_player);
    }
    Uint16 getOccupyingPlayerID() const
    {
        return SDL_SwapLE16(occupying_player_id);
    }
} __attribute__((packed));

class ChangeOutputLocation : public ObjectiveMessage
{
private:
    Sint32 new_point_x;
    Sint32 new_point_y;
  
public:
    void set(ObjectiveID id, iXY point)
    {
        setObjectiveID(id);
        new_point_x = SDL_SwapLE32(point.x);
        new_point_y = SDL_SwapLE32(point.y);
        message_type = _objective_mesg_change_output_location;
    }
    Sint32 getPointX() const
    {
        return SDL_SwapLE32(new_point_x);
    }
    Sint32 getPointY() const
    {
        return SDL_SwapLE32(new_point_y);
    }
} __attribute__((packed));

#ifdef MSVC
#pragma pack()
#endif

#endif // ** _OBJECTIVE_MESSSAGE_TYPES_HPP
