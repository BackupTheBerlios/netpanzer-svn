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
#ifndef _SYSTEMNETMESSAGE_HPP
#define _SYSTEMNETMESSAGE_HPP

#include <string.h>

#include "Point.hpp"
#include "PlayerState.hpp"
#include "NetPacket.hpp"

#ifdef MSVC
#pragma pack(1)
#endif

enum { _net_message_id_system_set_view,
       _net_message_id_system_reset_game_logic,
	   _net_message_id_system_chat_mesg_req,
	   _net_message_id_system_chat_mesg,
	   _net_message_id_system_view_control,
	   _net_message_id_system_ping_request,
	   _net_message_id_system_ping_ack,
       _net_message_id_system_connect_alert
	  };


class SystemSetPlayerView : public NetMessage
 {
  public:
   PointXYi camera_loc;

  SystemSetPlayerView()
   {
    message_class = _net_message_class_system;
    message_id = _net_message_id_system_set_view;
   }
 
 } __attribute__((packed));


class SystemResetGameLogic : public NetMessage
 {
  public:

  SystemResetGameLogic()
   {
    message_class = _net_message_class_system;
    message_id = _net_message_id_system_reset_game_logic;
   }
 
 } __attribute__((packed));

enum { _chat_mesg_scope_player_set,
       _chat_mesg_scope_alliance,
       _chat_mesg_scope_enemies,
       _chat_mesg_scope_all,
       _chat_mesg_scope_server
     };


class SystemChatMesgRequest : public NetMessage
 {
  public:
   unsigned char message_scope;
   unsigned char player_set[32];   
   
   unsigned short source_player_index;
   char message_text[150];
 
   SystemChatMesgRequest( )
    {
     reset();
    }
   
   void reset( void )
    {
     message_class = _net_message_class_system;
     message_id = _net_message_id_system_chat_mesg_req;
     memset( player_set, 0, sizeof( unsigned char) * 32 );
     message_scope = _chat_mesg_scope_all;
    }

   void setMessageScope( unsigned char scope )
    {
     message_scope = scope;
    }
 
   void setPlayerSet( unsigned short player_index )
    {
     unsigned long index;
     unsigned char shift;
     unsigned char mask = 1;
  
     index = ( player_index );
     shift = (unsigned char) ( 7 - (index & (unsigned long) 7 ) ); // 7 - (index % 8)
     index = index >> 3;                                           // index / 8
     mask = mask << shift;
  
     player_set[index] = player_set[ index ] | mask;
    }

   void clearPlayerSet( unsigned short player_index )
    {
     unsigned long index;
     unsigned char shift;
     unsigned char mask = 1;
  
     index = (player_index);
     shift = (unsigned char) ( 7 - (index & (unsigned long) 7 ) ); 
     index = index >> 3;
     mask = ~(mask << shift);

     player_set[ index ] = player_set[index] & mask; 
    }
 
 } __attribute__((packed));


class SystemChatMesg: public NetMessage
 {
  public:   
   unsigned char  message_scope;
   unsigned short source_player_index;
   char message_text[150];

   SystemChatMesg() 
    {
     message_class = _net_message_class_system;
     message_id = _net_message_id_system_chat_mesg;
    }
 
 } __attribute__((packed));



enum { _view_control_flag_visible_on  = 0x01,
       _view_control_flag_visible_off = 0x02,
       _view_control_flag_center_abs  = 0x04,
       _view_control_flag_close_all   = 0x08    
     }; 

class SystemViewControl : public NetMessage
 {
  public:
   unsigned char action_flags; 
   char view_name[32];
 
  SystemViewControl()
   {
    message_class = _net_message_class_system;
    message_id = _net_message_id_system_view_control;
    action_flags = 0;
   }

  void set( char *name, unsigned char flags )
   {
    action_flags = flags;
    strcpy( view_name, name );
   }
 
 } __attribute__((packed));


class SystemPingRequest : public NetMessage
 {
  public:
   unsigned short client_player_index;

  SystemPingRequest()
   {
    message_class = _net_message_class_system;
    message_id = _net_message_id_system_ping_request;
   }

 } __attribute__((packed));

class SystemPingAcknowledge : public NetMessage
 {
  public:

  SystemPingAcknowledge()
   {
    message_class = _net_message_class_system;
    message_id = _net_message_id_system_ping_ack;
   }

 } __attribute__((packed));


enum { _connect_alert_mesg_connect,
       _connect_alert_mesg_disconnect,
       _connect_alert_mesg_client_drop
     };

class SystemConnectAlert : public NetMessage
 {
  public:
   PlayerID player_id;
   unsigned char alert_enum;
  
  SystemConnectAlert()
   {
    message_class = _net_message_class_system;
    message_id = _net_message_id_system_connect_alert;
   }
 
  void set( PlayerID &player, unsigned char alert_type )
   {
    player_id = player;
    alert_enum = alert_type;
   }
 
 } __attribute__((packed));

#ifdef MSVC
#pragma pack()
#endif

#endif // ** _SYSTEMNETMESSAGE_HPP
