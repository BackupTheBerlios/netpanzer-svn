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

#define _OPCODE_MESSAGE_LIMIT 488

enum { _net_message_id_opcode_mesg,
	   _net_message_id_ini_sync_mesg,
	   _net_message_id_destroy_unit,
	   _net_message_id_create_unit,
       _net_message_id_unit_sync_integrity_check
	 };

#pragma pack(1)

class UnitOpcodeMessage : public NetMessage
 {
  public:
  unsigned short code_size;
  unsigned char  opcode_count;

  unsigned char data[ _OPCODE_MESSAGE_LIMIT ];  

  UnitOpcodeMessage()
   {
    message_class = _net_message_class_unit;
    message_id = _net_message_id_opcode_mesg;
   }

  unsigned short realSize( void ); 

 };


class UnitIniSyncMessage : public NetMessage
 {
  public:
   unsigned char unit_type;
   UnitID unit_id;
   PointXYi location;
   UnitState unit_state;

  UnitIniSyncMessage()
   {
    message_class = _net_message_class_unit;
    message_id = _net_message_id_ini_sync_mesg;
   }
  
  unsigned short realSize( void ) 
   { 
    return( sizeof( UnitIniSyncMessage ) ); 
   }

 };

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
 
 };


class UnitRemoteCreate : public NetMessage
 {
  public:
   UnitID new_unit_id;
   PointXYi location;
   unsigned char unit_type;

  UnitRemoteCreate()
   {
    message_class = _net_message_class_unit;
    message_id = _net_message_id_create_unit;
   }

 };

class UnitSyncIntegrityCheck : public NetMessage
 {
  public:

  UnitSyncIntegrityCheck()
   {
    message_class = _net_message_class_unit;
    message_id = _net_message_id_unit_sync_integrity_check;
   }

 };

#pragma pack()

#endif 
