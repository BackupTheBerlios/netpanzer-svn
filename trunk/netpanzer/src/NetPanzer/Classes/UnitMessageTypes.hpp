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
#ifndef _UNITMESSAGETYPES_HPP
#define _UNITMESSAGETYPES_HPP

#include "Classes/UnitMessage.hpp"

enum { _umesg_ai_command,
       _umesg_weapon_hit,
       _umesg_end_lifecycle,
       _umesg_update_select_box_info,
       _umesg_self_destruct };

enum { _command_move_to_loc,
       _command_attack_unit,
       _command_start_manual_move,
       _command_stop_manual_move,
       _command_manual_fire };

#ifdef MSVC
#pragma pack(1)
#endif

class UMesgAICommand : public UnitMessage
{
public:
    Uint8 command;
private:
    Sint32 goal_loc_x;
    Sint32 goal_loc_y;
    Uint16 target_id;
public:
    Uint8 manual_move_orientation;
private:
    Sint32 target_loc_x;
    Sint32 target_loc_y;
public:
    UMesgAICommand()
    {
        command = 0;
        goal_loc_x = goal_loc_y = 0;
        target_id = 0;
        manual_move_orientation = 0;
        target_loc_x = target_loc_y = 0;
    }

    UMesgAICommand(UnitID unit_id, unsigned char flags)
            : UnitMessage(unit_id, flags )
    {
        command = 0;
        goal_loc_x = goal_loc_y = 0;
        target_id = 0;
        manual_move_orientation = 0;
        target_loc_x = target_loc_y = 0;
    }

    void setMoveToLoc(const iXY& goal)
    {
        message_id = _umesg_ai_command;
        command = _command_move_to_loc;
        goal_loc_x = htol32(goal.x);
        goal_loc_y = htol32(goal.y);
        manual_move_orientation = 0;
    }

    void setTargetUnit(UnitID target)
    {
        message_id = _umesg_ai_command;
        command = _command_attack_unit;
        target_id = htol16(target);
        manual_move_orientation = 0;
    }

    void setStartManualMove(unsigned char orientation)
    {
        message_id = _umesg_ai_command;
        command = _command_start_manual_move;
        manual_move_orientation = orientation;
    }

    void setStopManualMove()
    {
        message_id = _umesg_ai_command;
        command = _command_stop_manual_move;
        manual_move_orientation = 0;
    }

    void setManualFire(const iXY& target)
    {
        message_id = _umesg_ai_command;
        command = _command_manual_fire;
        target_loc_x = htol32(target.x);
        target_loc_y = htol32(target.y);
        manual_move_orientation = 0;
    }

    iXY getGoalLoc() const
    {
        return iXY(ltoh32(goal_loc_x), ltoh32(goal_loc_y));
    }

    UnitID getTargetUnitID() const
    {
        return ltoh16(target_id);
    }

    iXY getTargetLoc() const
    {
        return iXY(ltoh32(target_loc_x), ltoh32(target_loc_y));
    }
}
__attribute__((packed));

class UMesgEndLifeCycleUpdate : public UnitMessage
{
private:
    Uint16 destroyed;
    Uint16 destroyer;

public:
    Uint8 unit_type;

    void set(UnitID destroyed_unit, UnitID destroyer_unit,
            unsigned char unit_type )
    {
        message_id = _umesg_end_lifecycle;
        message_flags = _umesg_flag_manager_request;
        destroyed = htol16(destroyed_unit);
        destroyer = htol16(destroyer_unit);
        this->unit_type = unit_type;
    }

    UnitID getDestroyed() const
    {
        return ltoh16(destroyed);
    }

    UnitID getDestroyer() const
    {
        return ltoh16(destroyer);
    }
}
__attribute__((packed));

class UMesgSelfDestruct : public UnitMessage
{
public:
    UMesgSelfDestruct()
    {
        message_id = _umesg_self_destruct;
    }
}
__attribute__((packed));

#ifdef MSVC
#pragma()
#endif

#endif
