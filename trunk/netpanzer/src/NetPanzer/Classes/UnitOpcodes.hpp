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

#include "PObject.hpp"
#include "QueueTemplate.hpp"

enum { _unit_opcode_flag_sync = 0x01 };

#pragma pack(1)

typedef
 struct
  {
   unsigned char  opcode;
   unsigned char  player_index;
   unsigned short unit_index;
   unsigned char  flags;

   unsigned char op_data[7];
  } UnitOpcodeStruct;


typedef QueueTemplate< UnitOpcodeStruct > UnitOpcodeQueue;

class UnitOpcode
 {
  public:
   unsigned char  opcode;
   unsigned char  player_index;
   unsigned short unit_index;  
   unsigned char  flags;
 };

#define _UNIT_OPCODE_MOVE 1

class MoveOpcode : public UnitOpcode
 {
  public:
   unsigned long square;
   signed char   loc_x_offset;
   signed char   loc_y_offset;
   unsigned char pad[1];
 
   MoveOpcode( )
    {
	 flags = 0;
	 opcode = _UNIT_OPCODE_MOVE;
	}

 };

#define _UNIT_OPCODE_TURRET_TRACK_POINT 2

class TurretTrackPointOpcode : public UnitOpcode
 {
  public:
   unsigned short x;
   unsigned short y;
   boolean  activate;
   unsigned char pad[2];

   TurretTrackPointOpcode( )
    {
	 flags = 0;
	 opcode = _UNIT_OPCODE_TURRET_TRACK_POINT;
	}

 };

#define _UNIT_OPCODE_TURRET_TRACK_TARGET 3

class TurretTrackTargetOpcode : public UnitOpcode
 {
  public:
   unsigned char target_player_index;
   unsigned short target_unit_index;
   boolean activate;
   unsigned char pad[3];
 
 
   TurretTrackTargetOpcode( )
    {
	 flags = 0;
	 opcode = _UNIT_OPCODE_TURRET_TRACK_TARGET;
	}

 };

#define _UNIT_OPCODE_FIRE_WEAPON 4

class FireWeaponOpcode : public UnitOpcode
 {
  public:
   unsigned short x;
   unsigned short y;
   unsigned char pad[3];

   FireWeaponOpcode( )
    {
	 flags = 0;
	 opcode = _UNIT_OPCODE_FIRE_WEAPON;
	}
 };

#define _UNIT_OPCODE_SYNC_UNIT 5

class SyncUnitOpcode : public UnitOpcode
 {
  public:
   unsigned char pad[7];

  SyncUnitOpcode( )
   {
	 flags = 0;
	 opcode = _UNIT_OPCODE_SYNC_UNIT;
   }
 
 };

#define _UNIT_OPCODE_UPDATE_STATE 6

class UpdateStateUnitOpcode : public UnitOpcode
 {
  public:
   short hit_points;
   unsigned char pad[5];

  UpdateStateUnitOpcode( )
   {
	flags = 0;
	opcode = _UNIT_OPCODE_UPDATE_STATE;
   }
 
 };

#define _UNIT_OPCODE_DESTRUCT 7

class DestructUnitOpcode : public UnitOpcode
 {
  public:
   unsigned char pad[7];

  DestructUnitOpcode( )
   {
	flags = 0;
	opcode = _UNIT_OPCODE_DESTRUCT;
   }
 
 };


#pragma pack()

#endif
