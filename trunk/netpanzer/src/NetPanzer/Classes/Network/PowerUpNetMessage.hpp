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
    PowerUpCreateMesg();
    void set(iXY map_loc, int ID, int type);
    int32_t getLocX(void);
    int32_t getLocY(void);
    int32_t getID(void);
    int32_t getType(void);
} __attribute__((packed));

class PowerUpHitMesg : public NetMessage
{
private:
    int32_t  ID;
    UnitID   unit_id;
    uint16_t player_id;
    int32_t  unit_powerup_type;

public:
    PowerUpHitMesg();
    void set(int ID, UnitID unit_id, PlayerID player_id, int type=0);
    int32_t getID(void);
    uint16_t getPlayerID(void);
    int32_t getUnitPowerupType(void);
} __attribute__((packed));

#ifdef MSVC
#pragma pack()
#endif

#endif // ** _POWERUP_NET_MESSAGE_HPP
