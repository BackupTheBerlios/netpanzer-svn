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

UnitOpcodeMessage::UnitOpcodeMessage()
{
    message_class = _net_message_class_unit;
    message_id = _net_message_id_opcode_mesg;
    memset(data, 0, _OPCODE_MESSAGE_LIMIT);
}

unsigned short UnitOpcodeMessage::realSize( void )
{
    return( ( sizeof( UnitOpcodeMessage ) - _OPCODE_MESSAGE_LIMIT ) + code_size );
}

void UnitOpcodeMessage::reset(void)
{
    code_size = 0;
    opcode_count = 0;
    memset(data, 0, _OPCODE_MESSAGE_LIMIT);
    message_class = _net_message_class_unit;
    message_id = _net_message_id_opcode_mesg;
}

bool UnitOpcodeMessage::isEmpty(void)
{
    return opcode_count != 0;
}

bool UnitOpcodeMessage::isFull(void)
{
    return (code_size + sizeof(UnitOpcodeStruct)) > _OPCODE_MESSAGE_LIMIT;
}

void UnitOpcodeMessage::add(UnitOpcode *opcode)
{
    memcpy(data + code_size, opcode, sizeof(UnitOpcodeStruct));
    code_size += sizeof(UnitOpcodeStruct);
    opcode_count++;
}

bool UnitOpcodeMessage::extract(int opcodeNum, UnitOpcodeStruct* dest)
{
    if (opcodeNum >= opcode_count)
        return false;

    memcpy(dest, data + sizeof(UnitOpcodeStruct) * opcodeNum,
        sizeof(UnitOpcodeStruct));
    return true;
}


UnitIniSyncMessage::UnitIniSyncMessage(uint8_t type, UnitID id,
        uint32_t x, uint32_t y)
{
    message_class = _net_message_class_unit;
    message_id = _net_message_id_ini_sync_mesg;
    unit_type = type;
    unit_id = id;
    location_x = htol32(x);
    location_y = htol32(y);
}

unsigned short UnitIniSyncMessage::realSize( void )
{
    return( sizeof( UnitIniSyncMessage ) );
}

uint32_t UnitIniSyncMessage::getLocX(void)
{
    return ltoh32(location_x);
}

uint32_t UnitIniSyncMessage::getLocY(void)
{
    return ltoh32(location_y);
}
