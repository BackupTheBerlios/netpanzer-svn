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
#ifndef _PLAYERNETMESSAGE_HPP
#define _PLAYERNETMESSAGE_HPP

#include "NetPacket.hpp"


enum { _net_message_id_player_connect_id,
	   _net_message_id_player_sync_state,
	   _net_message_id_player_score_update,
	   _net_message_id_player_alliance_request,
	   _net_message_id_player_alliance_update,
	 };

#pragma pack(1)

class PlayerConnectID : public NetMessage
 {
  public:
   PlayerState connect_state;

  PlayerConnectID()
   {
    message_class = _net_message_class_player;
    message_id = _net_message_id_player_connect_id;
   }
 
 };


class PlayerStateSync : public NetMessage
 {
  public:
   PlayerState player_state;

  PlayerStateSync()
   {
    message_class = _net_message_class_player;
    message_id = _net_message_id_player_sync_state;
   }
 
 };
 
class PlayerScoreUpdate : public NetMessage
 {
  public:
   unsigned short kill_by_player_index;
   unsigned short kill_on_player_index;
   unsigned char  unit_type;
  
  PlayerScoreUpdate()
   {
    message_class = _net_message_class_player;
    message_id = _net_message_id_player_score_update;
   }
 };


enum { _player_make_alliance,
       _player_break_alliance
     };

class PlayerAllianceRequest : public NetMessage
 {
  public:
   unsigned short allie_by_player_index;
   unsigned short allie_with_player_index;
   unsigned char  alliance_request_type;
       
  PlayerAllianceRequest()
   {
    message_class = _net_message_class_player;
    message_id = _net_message_id_player_alliance_request;
   }
 };


class PlayerAllianceUpdate : public NetMessage
 {
  public:
   unsigned short allie_by_player_index;
   unsigned short allie_with_player_index;
   unsigned char  alliance_update_type;
   
  PlayerAllianceUpdate()
   {
    message_class = _net_message_class_player;
    message_id = _net_message_id_player_alliance_update;
   }
 
  void set( unsigned short by_player_index, unsigned short with_player_index, unsigned char update_type )
   {
	allie_by_player_index = by_player_index;
    allie_with_player_index = with_player_index;
    alliance_update_type = update_type;
   }
 };

#pragma pack()

#endif // ** _PLAYERNETMESSAGE_HPP
