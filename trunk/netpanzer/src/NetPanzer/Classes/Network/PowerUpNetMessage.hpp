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

#include "NetPacket.hpp"

#include "UnitID.hpp"
#include "PlayerID.hpp"

enum { _net_message_id_powerup_create,
       _net_message_id_powerup_hit
     };

#ifdef MSVC
#pragma pack(1)
#endif

class PowerUpCreateMesg : public NetMessage
{
public:
    int32_t map_loc_x;
    int32_t map_loc_y;
    int32_t ID;
    int32_t type;

    void set( iXY map_loc, int ID, int type )
    {
        PowerUpCreateMesg::map_loc_x = map_loc.x;
        PowerUpCreateMesg::map_loc_y = map_loc.y;
        PowerUpCreateMesg::ID = ID;
        PowerUpCreateMesg::type = type;
    }

    PowerUpCreateMesg()
    {
        message_class = _net_message_class_powerup;
        message_id = _net_message_id_powerup_create;
    }
} __attribute__((packed));

class PowerUpHitMesg : public NetMessage
{
public:
    int32_t  ID;
    UnitID   unit_id;
    uint16_t player_id;
    int32_t  unit_powerup_type;

    void set( int ID, UnitID unit_id, PlayerID player_id )
    {
        PowerUpHitMesg::ID = ID;
        PowerUpHitMesg::unit_id = unit_id;
        PowerUpHitMesg::player_id = player_id.getIndex();
    }

    PowerUpHitMesg()
    {
        message_class = _net_message_class_powerup;
        message_id = _net_message_id_powerup_hit;
    }

} __attribute__((packed));

#ifdef MSVC
#pragma pack()
#endif

#endif // ** _POWERUP_NET_MESSAGE_HPP
