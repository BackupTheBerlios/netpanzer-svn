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

#include "PlayerState.hpp"
#include "Types/iXY.hpp"
#include "Util/Endian.hpp"

#ifdef MSVC
#pragma pack(1)
#endif

class ObjectiveMessage
{
public:
    uint8_t message_type;
private:
    uint16_t objective_id;

public:
    uint16_t getObjectiveID() const
    {
        return ltoh16(objective_id);
    }
    void setObjectiveID(uint16_t id)
    {
        objective_id = htol16(id);
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
    uint8_t occupation_status;
private:
    uint16_t occupying_player_id;
    uint32_t timeleft;

public:
    uint8_t  unit_type;
    bool     unit_gen_on;
        
    void set(uint16_t id, uint8_t status, uint16_t player_id,
            bool unit_gen_on, uint8_t unit_type, uint32_t timeleft)
    {
        message_type = _objective_mesg_update_occupation;
        setObjectiveID(id);
        occupation_status = status;
        occupying_player_id = htol16(player_id);
        this->unit_gen_on = unit_gen_on;
        this->timeleft = htol32(timeleft);
        this->unit_type = unit_type;
    }
   
    uint16_t getOccupyingPlayerID() const
    {
        return ltoh16(occupying_player_id);
    }
    uint32_t getTimeLeft() const
    {
        return ltoh32(timeleft);
    }
} __attribute__((packed));

class ChangeUnitGeneration : public ObjectiveMessage
{
public:
    uint8_t unit_type;
    uint8_t unit_gen_on;

    void set(uint16_t id, uint8_t unit_type, bool unit_generation_on)
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
    uint16_t disowned_player_id;

public:
    void set(uint16_t id, uint16_t disowned_player)
    {
        setObjectiveID(id);
        disowned_player_id = htol16(disowned_player);
        message_type = _objective_mesg_disown_player_objective;        
    }
    uint16_t getDisownedPlayerID() const
    {
        return ltoh16(disowned_player_id);
    }
} __attribute__((packed));

class SyncObjective : public ObjectiveMessage
{
public:
    uint8_t objective_status;
    uint8_t occupation_status;
private:
    uint16_t occupying_player_id;

public:
    void set(uint16_t id, uint8_t objective_status,
            uint8_t occupation_status, uint16_t occupying_player)
    {
        setObjectiveID(id);
        message_type = _objective_mesg_sync_objective;
                                                                             
        this->objective_status = objective_status;
        this->occupation_status = occupation_status;
        this->occupying_player_id = htol16(occupying_player);
    }
    uint16_t getOccupyingPlayerID() const
    {
        return ltoh16(occupying_player_id);
    }
} __attribute__((packed));

class ChangeOutputLocation : public ObjectiveMessage
{
private:
    int32_t new_point_x;
    int32_t new_point_y;
  
public:
    void set(uint16_t id, iXY point)
    {
        setObjectiveID(id);
        new_point_x = htol32(point.x);
        new_point_y = htol32(point.y);
        message_type = _objective_mesg_change_output_location;
    }
    int32_t getPointX() const
    {
        return ltoh32(new_point_x);
    }
    int32_t getPointY() const
    {
        return ltoh32(new_point_y);
    }
} __attribute__((packed));

#ifdef MSVC
#pragma pack()
#endif

#endif // ** _OBJECTIVE_MESSSAGE_TYPES_HPP
