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
#include "Types/iXY.hpp"

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
    UMesgAICommand()
    {
        command = 0;
        goal_loc_x = goal_loc_y = 0;
        target_id = 0;
    }

    UMesgAICommand( UnitID unit_id )
            : UnitMessage( unit_id )
    {
        command = 0;
        goal_loc_x = goal_loc_y = 0;
        target_id = 0;
    }

    void setMoveToLoc(const iXY& goal)
    {
        message_id = _umesg_ai_command;
        command = _command_move_to_loc;
        goal_loc_x = SDL_SwapLE32(goal.x);
        goal_loc_y = SDL_SwapLE32(goal.y);
    }

    void setTargetUnit(UnitID target)
    {
        message_id = _umesg_ai_command;
        command = _command_attack_unit;
        target_id = SDL_SwapLE16(target);
    }

    void setManualFire(const iXY& target)
    {
        message_id = _umesg_ai_command;
        command = _command_manual_fire;
        goal_loc_x = SDL_SwapLE32(target.x);
        goal_loc_y = SDL_SwapLE32(target.y);
    }

    iXY getGoalLoc() const
    {
        return iXY(SDL_SwapLE32(goal_loc_x), SDL_SwapLE32(goal_loc_y));
    }

    UnitID getTargetUnitID() const
    {
        return SDL_SwapLE16(target_id);
    }

    iXY getTargetLoc() const
    {
        return iXY(SDL_SwapLE32(goal_loc_x), SDL_SwapLE32(goal_loc_y));
    }
}
__attribute__((packed));

class UMesgWeaponHit : public UnitMessage
{
private:
    Uint16 owner_id;
    Sint32  hit_location_x;
    Sint32  hit_location_y;
    Uint16 damage_factor;
        
public:
    void setOwnerUnitID(UnitID id)
    {
        owner_id = SDL_SwapLE16(id);
    }

    UnitID getOwnerUnitID() const
    {
        return SDL_SwapLE16(owner_id);
    }

    void setHitLocation(const iXY& point)
    {
        hit_location_x = SDL_SwapLE32(point.x);
        hit_location_y = SDL_SwapLE32(point.y);
    }

    iXY getHitLocation() const
    {
        return iXY(SDL_SwapLE32(hit_location_x), SDL_SwapLE32(hit_location_y));
    }

    void setDamageFactor(Uint16 damage_factor)
    {
        this->damage_factor = SDL_SwapLE16(damage_factor);
    }
    Uint16 getDamageFactor() const
    {
        return SDL_SwapLE16(damage_factor);
    }
} __attribute__((packed));

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
        destroyed = SDL_SwapLE16(destroyed_unit);
        destroyer = SDL_SwapLE16(destroyer_unit);
        this->unit_type = unit_type;
    }

    UnitID getDestroyed() const
    {
        return SDL_SwapLE16(destroyed);
    }

    UnitID getDestroyer() const
    {
        return SDL_SwapLE16(destroyer);
    }
}
__attribute__((packed));

enum { _select_box_allie_visibility,
       _select_box_flag_visiblity,
       _select_box_is_allied
     };

class UMesgUpdateSelectBoxInfo : public UnitMessage
{
public:
    Uint8 request_type;
    Uint8 allie_flag_visiblity;
    Uint8 flag_visiblity;
    Uint8 allied_state;

    void set(unsigned char request, bool state_value)
    {
        message_id = _umesg_update_select_box_info;
        request_type = request;
        switch( request ) {
        case _select_box_allie_visibility :
            allie_flag_visiblity = state_value;
            break;

        case _select_box_flag_visiblity :
            flag_visiblity = state_value;
            break;

        case _select_box_is_allied :
            allied_state = state_value;
            break;

        } // ** switch
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
