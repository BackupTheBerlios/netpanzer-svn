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

#include "Core/CoreTypes.hpp"
#include "Types/iXY.hpp"
#include <queue>

enum { _unit_opcode_flag_sync = 0x80 };

#ifdef MSVC
#pragma pack(1)
#endif

#define _UNIT_OPCODE_MOVE                   1
#define _UNIT_OPCODE_TURRET_TRACK_POINT     2
#define _UNIT_OPCODE_TURRET_TRACK_TARGET    3
#define _UNIT_OPCODE_FIRE_WEAPON            4
#define _UNIT_OPCODE_SYNC_UNIT              5
#define _UNIT_OPCODE_UPDATE_STATE           6
#define _UNIT_OPCODE_DESTRUCT               7

inline unsigned int getOpcodeSize(unsigned char code);

// do not use this directly, cast to 1 of the UnitOpcode classes...
struct UnitOpcodeStruct
{
private:
    Uint8 opcode;
    Uint16 unit_index;
    Uint8 op_data[7];
} __attribute__((packed));

typedef std::queue<UnitOpcodeStruct> UnitOpcodeQueue;

class UnitOpcode
{
protected:
    Uint8 opcode;
    
private:
    Uint16 unit_id;

public:
    Uint8 getOpcode() const
    {
        return opcode&0x7f;
    }

    bool isSyncFlag() const
    {
        return opcode & _unit_opcode_flag_sync;
    }

    size_t getSize() const
    {
        return getOpcodeSize(getOpcode());
    }

    void setUnitID(UnitID id)
    {
        unit_id = SDL_SwapLE16(id);
    }

    UnitID getUnitID() const
    {
        return SDL_SwapLE16(unit_id);
    }
} __attribute__((packed));

class MoveOpcode : public UnitOpcode
{
private:
    Uint32 square;

public:
    Sint8 loc_x_offset;
    Sint8 loc_y_offset;

    MoveOpcode( )
    {
        opcode = _UNIT_OPCODE_MOVE;

        square = 0;
        loc_x_offset = 0;
        loc_y_offset = 0;
    }

    void setSquare(Uint32 square)
    {
        this->square = SDL_SwapLE32(square);
    }

    Uint32 getSquare() const
    {
        return SDL_SwapLE32(square);
    }
} __attribute__((packed));

class TurretTrackPointOpcode : public UnitOpcode
{
private:
    Uint16 target_x;
    Uint16 target_y;

public:
    Uint8  activate;

    TurretTrackPointOpcode( UnitID unitid, Uint8 flag, bool _activate)
    {
        opcode = _UNIT_OPCODE_TURRET_TRACK_POINT | flag;
        setUnitID(unitid);
        target_x = target_y = 0;
        activate = _activate;
    }

    void setTarget(iXY pos)
    {
        target_x = SDL_SwapLE16(pos.x);
        target_y = SDL_SwapLE16(pos.y);
    }

    iXY getTarget() const
    {
        return iXY(SDL_SwapLE16(target_x), SDL_SwapLE16(target_y));
    }
} __attribute__((packed));

class TurretTrackTargetOpcode : public UnitOpcode
{
private:
    Uint16 targetUnitID;
public:
    Uint8 activate;

    TurretTrackTargetOpcode( UnitID unitid, Uint8 flag, bool _activate )
    {
        opcode = _UNIT_OPCODE_TURRET_TRACK_TARGET | flag;
        setUnitID(unitid);
        targetUnitID = 0;
        activate = _activate;
    }

    void setTargetUnitID(UnitID id)
    {
        targetUnitID = SDL_SwapLE16(id);
    }
    
    UnitID getTargetUnitID() const
    {
        return SDL_SwapLE16(targetUnitID);
    }
} __attribute__((packed));

class FireWeaponOpcode : public UnitOpcode
{
private:
    Uint16 x;
    Uint16 y;
    
public:

    FireWeaponOpcode( UnitID unitid )
    {
        opcode = _UNIT_OPCODE_FIRE_WEAPON;
        setUnitID(unitid);

        x = y = 0;
    }

    void setTarget(iXY target)
    {
        x = SDL_SwapLE16(target.x);
        y = SDL_SwapLE16(target.y);
    }

    iXY getTarget() const
    {
        return iXY(SDL_SwapLE16(x), SDL_SwapLE16(y));
    }
} __attribute__((packed));

class SyncUnitOpcode : public UnitOpcode
{
public:

    SyncUnitOpcode( UnitID unitid )
    {
        opcode = _UNIT_OPCODE_SYNC_UNIT;
        setUnitID(unitid);

    }

} __attribute__((packed));

class UpdateStateUnitOpcode : public UnitOpcode
{
private:
    Sint16 hit_points;
public:

    UpdateStateUnitOpcode( )
    {
        opcode = _UNIT_OPCODE_UPDATE_STATE;
    }

    void setHitPoints(Sint16 newhitpoints)
    {
        hit_points = SDL_SwapLE16(newhitpoints);
    }

    Sint16 getHitPoints() const
    {
        return SDL_SwapLE16(hit_points);
    }
} __attribute__((packed));

class DestructUnitOpcode : public UnitOpcode
{
public:

    DestructUnitOpcode( )
    {
        opcode = _UNIT_OPCODE_DESTRUCT;
    }
} __attribute__((packed));

#ifdef MSVC
#pragma pack()
#endif

inline unsigned int getOpcodeSize(unsigned char code)
{
    switch ( code )
    {
        case _UNIT_OPCODE_MOVE:
            return sizeof(MoveOpcode);

        case _UNIT_OPCODE_TURRET_TRACK_POINT:
            return sizeof(TurretTrackPointOpcode);

        case _UNIT_OPCODE_TURRET_TRACK_TARGET:
            return sizeof(TurretTrackTargetOpcode);

        case _UNIT_OPCODE_FIRE_WEAPON:
            return sizeof(FireWeaponOpcode);

        case _UNIT_OPCODE_SYNC_UNIT:
            return sizeof(SyncUnitOpcode);

        case _UNIT_OPCODE_UPDATE_STATE:
            return sizeof(UpdateStateUnitOpcode);

        case _UNIT_OPCODE_DESTRUCT:
            return sizeof(DestructUnitOpcode);

    }
    return sizeof(UnitOpcodeStruct);
}

#endif
