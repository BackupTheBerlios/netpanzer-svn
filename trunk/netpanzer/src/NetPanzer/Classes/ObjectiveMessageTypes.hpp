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
#ifndef _OBJECTIVE_MESSSAGE_TYPES_HPP
#define _OBJECTIVE_MESSSAGE_TYPES_HPP

#include "PlayerState.hpp"

#ifdef MSVC
#pragma pack(1)
#endif

class ObjectiveMessage : public PObject
 {
  public:
   unsigned char message_type;
   unsigned short objective_id;

 } __attribute__((packed));

enum { _objective_mesg_update_occupation,
       _objective_mesg_change_unit_generation, 
       _objective_mesg_disown_player_objective,
       _objective_mesg_sync_objective
	 };

class UpdateOccupationsStatus : public ObjectiveMessage
 {
  public:
   unsigned char occupation_status;
   PlayerID occupying_player;
      
   void set( unsigned short id, unsigned char status, PlayerID &player )
    {
     objective_id = id;
     occupation_status = status;
     occupying_player = player;
     message_type = _objective_mesg_update_occupation;
    } 

 } __attribute__((packed));

class ChangeUnitGeneration : public ObjectiveMessage
 {
  public:
   unsigned char unit_type;
   bool unit_gen_on;    

  void set( unsigned short id, unsigned char unit_type, bool unit_generation_on )
   {
    objective_id = id;
    ChangeUnitGeneration::unit_type = unit_type;
    unit_gen_on = unit_generation_on;
    message_type = _objective_mesg_change_unit_generation;
   }

 } __attribute__((packed));

class DisownPlayerObjective : public ObjectiveMessage
 {
  public:
   PlayerID disowned_player_id;

  void set( unsigned short id, PlayerID &disowned_player )
   {
	objective_id = id;
	disowned_player_id = disowned_player;
	message_type = _objective_mesg_disown_player_objective;
   }
 
 } __attribute__((packed));

class SyncObjective : public ObjectiveMessage
 {
  public:
   unsigned char objective_status;
   unsigned char occupation_status;
   PlayerID occupying_player;

  void set( unsigned short id, 
            unsigned char objective_status, 
            unsigned char occupation_status,
            PlayerID occupying_player )
   {
	objective_id = id;
	SyncObjective::objective_status = objective_status;
    SyncObjective::occupation_status = occupation_status;
    SyncObjective::occupying_player = occupying_player;
    message_type = _objective_mesg_sync_objective;
   }
 
 } __attribute__((packed));

#ifdef MSVC
#pragma pack()
#endif

#endif // ** _OBJECTIVE_MESSSAGE_TYPES_HPP
