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
#ifndef _CONNECTNETMESSAGE_HPP
#define _CONNECTNETMESSAGE_HPP

#include <time.h>
#include "NetPacket.hpp"
#include "PlayerState.hpp"

enum { _net_message_id_connect_join_game_request, 
       _net_message_id_connect_join_game_request_ack,
       _net_message_id_client_start_connect,
       _net_message_id_client_connect_request,
	   _net_message_id_client_connect_result,
	   _net_message_id_connect_client_settings,
	   _net_message_id_client_connect_process_update,
	   _net_message_id_client_connect_process_state_mesg,
	   _net_message_id_connect_server_game_setup,
	   _net_message_id_connect_client_game_setup_ping,
       _net_message_id_connect_client_game_setup_ack,

       _net_message_id_connect_client_abort,
       
       _net_message_id_connect_netPanzer_client_disconnect,
       _net_message_id_connect_netPanzer_server_disconnect 
     };

#ifdef MSVC
#pragma pack(1)
#endif

class ClientConnectJoinRequest : public NetMessage
 {
  public:
   DWORD client_transport_id;
   char  code_word[24];
   DWORD protocol_version;

  ClientConnectJoinRequest()
   {
    message_class = _net_message_class_connect;
    message_id = _net_message_id_connect_join_game_request;
   }

 } __attribute__((packed));

enum { _join_request_result_success,
       _join_request_result_invalid_protocol,
       _join_request_result_server_busy
     };

class ClientConnectJoinRequestAck : public NetMessage
 {
  public:
    int   result_code;
    int   server_connection_state;
    DWORD server_protocol_version;

  ClientConnectJoinRequestAck()
   {
    message_class = _net_message_class_connect;
    message_id = _net_message_id_connect_join_game_request_ack;
   }

 } __attribute__((packed));

class ClientConnectStartConnect : public NetMessage
 {
  public:
   
  ClientConnectStartConnect()
   {
    message_class = _net_message_class_connect;
    message_id = _net_message_id_client_start_connect;
   }
 
 } __attribute__((packed));

class ClientConnectRequest : public NetMessage
 {
  public:

  ClientConnectRequest()
   {
    message_class = _net_message_class_connect;
    message_id = _net_message_id_client_connect_request;
   }
 
 } __attribute__((packed));


enum { _connect_result_success,
       _connect_result_server_busy,
       _connect_result_server_full
     };

class ClientConnectResult : public NetMessage
 {
  public:
   unsigned char result_code;

  ClientConnectResult()
   {
    message_class = _net_message_class_connect;
    message_id = _net_message_id_client_connect_result;
   }
  
 } __attribute__((packed));


class ConnectProcessUpdate : public NetMessage
 {
  public:
   unsigned long queue_position;

  ConnectProcessUpdate()
   {
    message_class = _net_message_class_connect;
    message_id = _net_message_id_client_connect_process_update;
   }
  
 } __attribute__((packed));
 
enum { _connect_state_message_load_game_data,
       _connect_state_message_sync_player_info,
       _connect_state_message_sync_player_info_percent,
	   _connect_state_message_sync_units,
       _connect_state_message_sync_units_percent,
	   _connect_state_sync_complete
	 };

class ConnectProcessStateMessage : public NetMessage
 {
  public:
   unsigned long message_enum;
   int           percent_complete;

  ConnectProcessStateMessage()
   {
    message_class = _net_message_class_connect;
    message_id = _net_message_id_client_connect_process_state_mesg;
   }
  
 } __attribute__((packed));

class ConnectClientSettings : public NetMessage
 {
  public:
   char player_name[64];
   short player_flag;
   unsigned char unit_color;

  ConnectClientSettings()
   {
    message_class = _net_message_class_connect;
    message_id = _net_message_id_connect_client_settings;
   }
 
  void set( char *player_name, unsigned char unit_color, short player_flag )
   {
    strcpy( ConnectClientSettings::player_name, player_name );
    ConnectClientSettings::unit_color = unit_color;
    ConnectClientSettings::player_flag = player_flag;
   }

 } __attribute__((packed));

class ConnectMesgServerGameSettings : public NetMessage
 {
  public:
   unsigned short max_players;
   unsigned short max_units;
   char           map_name[32];
   int	          cloud_coverage;
   float          wind_speed;
   int            game_type;
   bool        map_cycle_state;
   bool        powerup_state;
   int            frag_limit;
   int            time_limit;
   time_t         elapsed_time;

  ConnectMesgServerGameSettings()
   {
    message_class = _net_message_class_connect;
    message_id = _net_message_id_connect_server_game_setup;
   }  
 
 } __attribute__((packed));

class ConnectMesgClientGameSetupAck : public NetMessage
 {
  public:
   
  ConnectMesgClientGameSetupAck()
   {
    message_class = _net_message_class_connect;
    message_id = _net_message_id_connect_client_game_setup_ack;
   }   
 } __attribute__((packed));

class ConnectMesgClientGameSetupPing : public NetMessage
 {
  public:
   
  ConnectMesgClientGameSetupPing()
   {
    message_class = _net_message_class_connect;
    message_id = _net_message_id_connect_client_game_setup_ping;
   }   
 } __attribute__((packed));

class ConnectMesgNetPanzerClientDisconnect : public NetMessage
 {
  public:
   PlayerID client_id; 

  ConnectMesgNetPanzerClientDisconnect()
   {
    message_class = _net_message_class_connect;
    message_id = _net_message_id_connect_netPanzer_client_disconnect;
   }   
 
 } __attribute__((packed));

class ConnectMesgNetPanzerServerDisconnect : public NetMessage
 {
  public:

  ConnectMesgNetPanzerServerDisconnect()
   {
    message_class = _net_message_class_connect;
    message_id = _net_message_id_connect_netPanzer_server_disconnect;
   }   
 
 } __attribute__((packed));

#ifdef MSVC
#pragma pack()
#endif

#endif // ** _CONNECTNETMESSAGE_HPP
