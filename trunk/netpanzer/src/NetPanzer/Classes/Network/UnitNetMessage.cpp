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

#include "UnitNetMessage.hpp"
#include "UnitOpcodes.hpp"

UnitIniSyncMessage::UnitIniSyncMessage(uint8_t type, uint16_t player_id,
        UnitID id, uint32_t x, uint32_t y)
{
    message_class = _net_message_class_unit;
    message_id = _net_message_id_ini_sync_mesg;
    unit_type = type;
    this->player_id = htol16(player_id);
    unit_id = htol16(id);
    location_x = htol32(x);
    location_y = htol32(y);
}

unsigned short UnitIniSyncMessage::realSize() const
{
    return( sizeof( UnitIniSyncMessage ) );
}

uint32_t UnitIniSyncMessage::getLocX() const
{
    return ltoh32(location_x);
}

uint32_t UnitIniSyncMessage::getLocY() const
{
    return ltoh32(location_y);
}

UnitRemoteCreate::UnitRemoteCreate(uint16_t player_id, UnitID id,
        uint32_t x, uint32_t y, uint8_t type)
{
    message_class = _net_message_class_unit;
    message_id = _net_message_id_create_unit;
    this->player_id = htol16(player_id);
    new_unit_id = htol16(id);
    location_x = htol32(x);
    location_y = htol32(y);
    unit_type = type;
}

uint32_t UnitRemoteCreate::getLocX() const
{
    return ltoh32(location_x);
}

uint32_t UnitRemoteCreate::getLocY() const
{
    return ltoh32(location_y);
}
