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
#ifndef _POWERUP_NET_MESSAGE_HPP
#define _POWERUP_NET_MESSAGE_HPP

#include "NetMessage.hpp"

#include "PlayerID.hpp"
#include "UnitBase.hpp"

enum { _net_message_id_powerup_create,
       _net_message_id_powerup_hit
     };

#ifdef MSVC
#pragma pack(1)
#endif

class PowerUpCreateMesg : public NetMessage
{
private:
    int32_t map_loc_x;
    int32_t map_loc_y;
    int32_t ID;
    int32_t type;

public:
    PowerUpCreateMesg()
    {
        message_class = _net_message_class_powerup;
        message_id = _net_message_id_powerup_create;
    }
    void set(iXY map_loc, int ID, int type)
    {
        this->map_loc_x = htol32(map_loc.x);
        this->map_loc_y = htol32(map_loc.y);
        this->ID = htol32(ID);                      
        this->type = htol32(type);
    }
    int32_t getLocX() const
    {
        return ltoh32(map_loc_x);
    }
    int32_t getLocY() const
    {
        return ltoh32(map_loc_y);
    }
    int32_t getID() const
    {
        return ltoh32(ID);
    }
    int32_t getType() const
    {
        return ltoh32(type);
    }
} __attribute__((packed));

class PowerUpHitMesg : public NetMessage
{
private:
    int32_t  ID;
    uint16_t player_id;
    int32_t  unit_powerup_type;

public:
    PowerUpHitMesg()
    {
        message_class = _net_message_class_powerup;
        message_id = _net_message_id_powerup_hit;
    }
    void set(int ID, uint16_t player_id, int type=0)
    {
        this->ID = htol32(ID);
        this->player_id = htol16(player_id);
        this->unit_powerup_type = htol32(type);
    }
    int32_t getID() const
    {
        return ltoh32(ID);
    }
    uint16_t getPlayerID() const
    {
        return ltoh16(player_id);
    }
    int32_t getUnitPowerupType() const
    {
        return ltoh32(unit_powerup_type);
    }
} __attribute__((packed));

#ifdef MSVC
#pragma pack()
#endif

#endif // ** _POWERUP_NET_MESSAGE_HPP
