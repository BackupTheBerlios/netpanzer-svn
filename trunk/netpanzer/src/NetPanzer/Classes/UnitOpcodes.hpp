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
#ifndef _UNITOPCODES_HPP
#define _UNITOPCODES_HPP

#include "UnitBase.hpp"
#include <stdint.h>
#include <queue>

enum { _unit_opcode_flag_sync = 0x01 };

/** The following is a tricky macro to ensure a struct has a specific size. THe
 * check is done at compiletime. The trick is that C doesn't allow duplicate
 * case labels...
 */
#define ASSERT_SIZE(mystruct, size)                                     \
    namespace TRICKYTESTS { static inline void mystruct##_test() {      \
        int i=0; switch(i) { case 0: ; case (sizeof(mystruct) == (size)): ; } \
    } }

#ifdef MSVC
#pragma pack(1)
#endif

// do not use this directly, cast to 1 of the UnitOpcode classes...
struct UnitOpcodeStruct
{
public:
    uint8_t opcode;
private:
    uint16_t unit_index;
public:
    uint8_t flags;
private:
    uint8_t op_data[7];
} __attribute__((packed));

typedef std::queue<UnitOpcodeStruct> UnitOpcodeQueue;

class UnitOpcode
{
public:
    uint8_t opcode;
    //uint8_t player_index;
private:
    uint16_t unit_id;
public:
    uint8_t flags;

    static size_t getSize()
    {
        return sizeof(UnitOpcodeStruct);
    }

    void setUnitID(UnitID id)
    {
        unit_id = htol16(id);
    }

    UnitID getUnitID() const
    {
        return ltoh16(unit_id);
    }
} __attribute__((packed));

ASSERT_SIZE(UnitOpcode, sizeof(UnitOpcodeStruct)-7);

#define _UNIT_OPCODE_MOVE 1

class MoveOpcode : public UnitOpcode
{
private:
    uint32_t square;

public:
    int8_t loc_x_offset;
    int8_t loc_y_offset;
    uint8_t pad[1];

    MoveOpcode( )
    {
        flags = 0;
        opcode = _UNIT_OPCODE_MOVE;

        square = 0;
        loc_x_offset = 0;
        loc_y_offset = 0;
        pad[0] = 0;
    }

    void setSquare(uint32_t square)
    {
        this->square = htol32(square);
    }

    uint32_t getSquare() const
    {
        return ltoh32(square);
    }
} __attribute__((packed));

ASSERT_SIZE(MoveOpcode, 7 + sizeof(UnitOpcode))

#define _UNIT_OPCODE_TURRET_TRACK_POINT 2

class TurretTrackPointOpcode : public UnitOpcode
{
private:
    uint16_t target_x;
    uint16_t target_y;

public:
    uint8_t  activate;
    uint8_t pad[2];

    TurretTrackPointOpcode( )
    {
        flags = 0;
        opcode = _UNIT_OPCODE_TURRET_TRACK_POINT;

        target_x = target_y = 0;
        activate = false;
        pad[0] = pad[1] = 0;
    }

    void setTarget(iXY pos)
    {
        target_x = htol16(pos.x);
        target_y = htol16(pos.y);
    }

    iXY getTarget() const
    {
        return iXY(ltoh16(target_x), ltoh16(target_y));
    }
} __attribute__((packed));

ASSERT_SIZE(TurretTrackPointOpcode, 7 + sizeof(UnitOpcode))

#define _UNIT_OPCODE_TURRET_TRACK_TARGET 3

class TurretTrackTargetOpcode : public UnitOpcode
{
private:
    uint16_t targetUnitID;
public:
    uint8_t activate;
    uint8_t pad[4];

    TurretTrackTargetOpcode( )
    {
        flags = 0;
        opcode = _UNIT_OPCODE_TURRET_TRACK_TARGET;

        targetUnitID = 0;
        activate = false;
        pad[0] = pad[1] = pad[2] = pad[3] = 0;
    }

    void setTargetUnitID(UnitID id)
    {
        targetUnitID = htol16(id);
    }
    
    UnitID getTargetUnitID() const
    {
        return ltoh16(targetUnitID);
    }
} __attribute__((packed));

ASSERT_SIZE(TurretTrackTargetOpcode, 7 + sizeof(UnitOpcode))

#define _UNIT_OPCODE_FIRE_WEAPON 4

class FireWeaponOpcode : public UnitOpcode
{
private:
    uint16_t x;
    uint16_t y;
    uint8_t pad[3];
    
public:

    FireWeaponOpcode( )
    {
        flags = 0;
        opcode = _UNIT_OPCODE_FIRE_WEAPON;

        x = y = 0;
        pad[0] = pad[1] = pad[2] = 0;
    }

    void setTarget(iXY target)
    {
        x = htol16(target.x);
        y = htol16(target.y);
    }

    iXY getTarget() const
    {
        return iXY(ltoh32(x), ltoh32(y));
    }
} __attribute__((packed));

ASSERT_SIZE(FireWeaponOpcode, 7 + sizeof(UnitOpcode))

#define _UNIT_OPCODE_SYNC_UNIT 5

class SyncUnitOpcode : public UnitOpcode
{
public:
    uint8_t pad[7];

    SyncUnitOpcode( )
    {
        flags = 0;
        opcode = _UNIT_OPCODE_SYNC_UNIT;

        for(int i=0;i<7;i++)
            pad[i] = 0;
    }

} __attribute__((packed));

ASSERT_SIZE(SyncUnitOpcode, 7 + sizeof(UnitOpcode))

#define _UNIT_OPCODE_UPDATE_STATE 6

class UpdateStateUnitOpcode : public UnitOpcode
{
public:
    int16_t hit_points;
    uint8_t pad[5];

    UpdateStateUnitOpcode( )
    {
        flags = 0;
        opcode = _UNIT_OPCODE_UPDATE_STATE;

        for(int i=0; i<5; i++)
            pad[i] = 0;
    }
} __attribute__((packed));

ASSERT_SIZE(UpdateStateUnitOpcode, 7 + sizeof(UnitOpcode))

#define _UNIT_OPCODE_DESTRUCT 7

class DestructUnitOpcode : public UnitOpcode
{
public:
    unsigned char pad[7];

    DestructUnitOpcode( )
    {
        flags = 0;
        opcode = _UNIT_OPCODE_DESTRUCT;

        for(int i=0; i<7; i++)
            pad[i] = 0;
    }
} __attribute__((packed));

ASSERT_SIZE(DestructUnitOpcode, 7 + sizeof(UnitOpcode))

#ifdef MSVC
#pragma pack()
#endif

#endif
