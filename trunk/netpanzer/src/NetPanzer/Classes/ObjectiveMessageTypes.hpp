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
    uint16_t getObjectiveID();
    void setObjectiveID(uint16_t id);
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

public:
    void set(unsigned short id, unsigned char status, PlayerID &player);
    
    uint16_t getOccupyingPlayerID();
} __attribute__((packed));

class ChangeUnitGeneration : public ObjectiveMessage
{
public:
    unsigned char unit_type;
    bool unit_gen_on;

    void set(unsigned short id, unsigned char unit_type,
        bool unit_generation_on);
}
__attribute__((packed));

class DisownPlayerObjective : public ObjectiveMessage
{
private:
    uint16_t disowned_player_id;

public:
    void set(unsigned short id, PlayerID &disowned_player);
    uint16_t getDisownedPlayerID(void);
} __attribute__((packed));

class SyncObjective : public ObjectiveMessage
{
public:
    uint8_t objective_status;
    uint8_t occupation_status;
private:
    uint16_t occupying_player_id;

public:
    void set(unsigned short id,
                  unsigned char objective_status,
                  unsigned char occupation_status,
                  PlayerID occupying_player);
    uint16_t getOccupyingPlayerID(void);
} __attribute__((packed));

class ChangeOutputLocation : public ObjectiveMessage
{
private:
    int32_t new_point_x;
    int32_t new_point_y;
  
public:
    void set(unsigned short id, iXY point);
    int32_t getPointX(void);
    int32_t getPointY(void);
} __attribute__((packed));

#ifdef MSVC
#pragma pack()
#endif

#endif // ** _OBJECTIVE_MESSSAGE_TYPES_HPP
