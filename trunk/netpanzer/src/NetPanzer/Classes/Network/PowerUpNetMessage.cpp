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

#include "Types/iXY.hpp"
#include "PowerUpNetMessage.hpp"


PowerUpCreateMesg::PowerUpCreateMesg()
{
    message_class = _net_message_class_powerup;
    message_id = _net_message_id_powerup_create;
}

void PowerUpCreateMesg::set(iXY map_loc, int ID, int type)
{
    PowerUpCreateMesg::map_loc_x = htol32(map_loc.x);
    PowerUpCreateMesg::map_loc_y = htol32(map_loc.y);
    PowerUpCreateMesg::ID = htol32(ID);
    PowerUpCreateMesg::type = htol32(type);
}

int32_t PowerUpCreateMesg::getLocX(void)
{
    return ltoh32(map_loc_x);
}

int32_t PowerUpCreateMesg::getLocY(void)
{
    return ltoh32(map_loc_y);
}

int32_t PowerUpCreateMesg::getID(void)
{
    return ltoh32(ID);
}

int32_t PowerUpCreateMesg::getType(void)
{
    return ltoh32(type);
}


PowerUpHitMesg::PowerUpHitMesg()
{
    message_class = _net_message_class_powerup;
    message_id = _net_message_id_powerup_hit;
}

void PowerUpHitMesg::set(int ID, UnitID unit_id, PlayerID player_id, int type)
{
    PowerUpHitMesg::ID = htol32(ID);
    PowerUpHitMesg::unit_id = htol16(unit_id);
    PowerUpHitMesg::player_id = htol16(player_id.getIndex());
    unit_powerup_type = type;
}

int32_t PowerUpHitMesg::getID(void)
{
    return ltoh32(ID);
}

uint16_t PowerUpHitMesg::getPlayerID(void)
{
    return ltoh16(player_id);
}

int32_t PowerUpHitMesg::getUnitPowerupType(void)
{
    return ltoh32(unit_powerup_type);
}
