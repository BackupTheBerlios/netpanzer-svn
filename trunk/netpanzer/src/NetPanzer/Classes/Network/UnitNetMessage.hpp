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
#ifndef _UNITNETMESSAGE_HPP
#define _UNITNETMESSAGE_HPP

#include "NetPacket.hpp"
#include "UnitState.hpp"
#include "UnitID.hpp"
#include "UnitOpcodes.hpp"

#define _OPCODE_MESSAGE_LIMIT 488

enum { _net_message_id_opcode_mesg,
       _net_message_id_ini_sync_mesg,
       _net_message_id_destroy_unit,
       _net_message_id_create_unit,
       _net_message_id_unit_sync_integrity_check
     };

#ifdef MSVC
#pragma pack(1)
#endif

class UnitOpcodeMessage : public NetMessage
{
    friend class NetPacketDebugger;
private:
    uint16_t code_size;
    uint8_t opcode_count;
    uint8_t data[ _OPCODE_MESSAGE_LIMIT ];

public:
    UnitOpcodeMessage();
    unsigned short realSize( void );
    void reset(void);
    bool isFull(void);
    bool isEmpty(void);
    void add(UnitOpcode *opcode);
    bool extract(int opcodeNum, UnitOpcodeStruct* dest);

} __attribute__((packed));


class UnitIniSyncMessage : public NetMessage
{
public:
    uint8_t unit_type;
    UnitID unit_id;
private:
    uint32_t location_x;
    uint32_t location_y;
public:
    NetworkUnitState unit_state;

    UnitIniSyncMessage(uint8_t unit_type, UnitID unit_id,
        uint32_t location_x, uint32_t location_y);
    unsigned short realSize(void);
    uint32_t getLocX(void);
    uint32_t getLocY(void);
} __attribute__((packed));

// ** NOTE: A big, mother fucking HACK

class UnitRemoteDestroy : public NetMessage
{
public:
    UnitID unit_to_destroy;

    UnitRemoteDestroy()
    {
        message_class = _net_message_class_unit;
        message_id = _net_message_id_destroy_unit;
    }

} __attribute__((packed));


class UnitRemoteCreate : public NetMessage
{
public:
    UnitID new_unit_id;
private:
    uint32_t location_x;
    uint32_t location_y;
public:
    uint8_t unit_type;

    UnitRemoteCreate(UnitID id, uint32_t x, uint32_t y, uint8_t type);
    uint32_t getLocX(void);
    uint32_t getLocY(void);
} __attribute__((packed));

class UnitSyncIntegrityCheck : public NetMessage
{
public:
    UnitSyncIntegrityCheck()
    {
        message_class = _net_message_class_unit;
        message_id = _net_message_id_unit_sync_integrity_check;
    }

} __attribute__((packed));

#ifdef MSVC
#pragma pack()
#endif

#endif
