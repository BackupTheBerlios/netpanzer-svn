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
#ifndef _UNITMESSAGETYPES_HPP
#define _UNITMESSAGETYPES_HPP

#include "UnitMessage.hpp"
#include "UnitID.hpp"
#include "Point.hpp"

enum { _umesg_ai_command,
       _umesg_weapon_hit,
       _umesg_end_lifecycle,
       _umesg_update_select_box_info,
	   _umesg_self_destruct };

enum { _command_move_to_loc,
       _command_attack_unit,
       _command_start_manual_move,
       _command_stop_manual_move,
       _command_manual_fire };
        
class UMesgAICommand : public UnitMessage
 {
  public:
   unsigned char command;
   PointXYi goal_loc;
   UnitID   target_id;
   unsigned char manual_move_orientation;
   PointXYi target_loc;
   
  UMesgAICommand() {}
  
  UMesgAICommand( UnitID unit_id, unsigned char flags )
   : UnitMessage(unit_id, flags )
   {
   
   }

  inline void setMoveToLoc( PointXYi &goal )
   {
    message_id = _umesg_ai_command;
	command = _command_move_to_loc;
	goal_loc = goal;
   }
 
  inline void setTargetUnit( UnitID &target )
   {
    message_id = _umesg_ai_command;
	command = _command_attack_unit;
    target_id = target;
   }
 
  inline void setStartManualMove( unsigned char orientation )
   {
    message_id = _umesg_ai_command;
	command = _command_start_manual_move;
    manual_move_orientation = orientation;
   }

  inline void setStopManualMove( void )
   {
    message_id = _umesg_ai_command;
	command = _command_stop_manual_move;
   }

  inline void setManualFire( PointXYi &target )
   {
    message_id = _umesg_ai_command;
	command = _command_manual_fire;
	target_loc = target;
   }

 };

class UMesgWeaponHit : public UnitMessage
 { 
  public:
   UnitID owner_id;
   PointXYi hit_location;
   unsigned short damage_factor;

 };

class UMesgEndLifeCycleUpdate : public UnitMessage
 {
  public:
   UnitID destroyed;
   UnitID destroyer;
   unsigned char unit_type;
    
  inline void set( UnitID &destroyed_unit, UnitID &destroyer_unit, unsigned char unit_type )
   {
    message_id = _umesg_end_lifecycle;
    message_flags = _umesg_flag_manager_request;
    destroyed = destroyed_unit;
    destroyer = destroyer_unit;
    UMesgEndLifeCycleUpdate::unit_type = unit_type;
   }

 };

enum { _select_box_allie_visibility,
       _select_box_flag_visiblity,
       _select_box_is_allied
     };

class UMesgUpdateSelectBoxInfo : public UnitMessage
 {
  public:   
   unsigned char request_type;
   boolean allie_flag_visiblity;
   boolean flag_visiblity;
   boolean allied_state;

   void set( unsigned char request, boolean state_value )
    {
     message_id = _umesg_update_select_box_info;
     request_type = request;
     switch( request )
      {
       case _select_box_allie_visibility :
        allie_flag_visiblity = state_value;
       break;

       case _select_box_flag_visiblity :
        flag_visiblity = state_value;
       break;

       case _select_box_is_allied :
        allied_state = state_value;
       break;

      } // ** switch
    } 
 };

class UMesgSelfDestruct : public UnitMessage
 { 
  public:

   UMesgSelfDestruct( )
    {
	 message_id = _umesg_self_destruct;
	}
 
 };


#endif // ** _UNITMESSAGETYPES_HPP
