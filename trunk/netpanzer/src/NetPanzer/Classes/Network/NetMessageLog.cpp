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
#include <config.h>
#include "NetMessageLog.hpp"

#include "Server.hpp"
#include "SystemNetMessage.hpp"
#include "ConnectNetMessage.hpp"
#include "PlayerNetMessage.hpp"
#include "UnitNetMessage.hpp"
#include "TerminalNetMesg.hpp"
#include "ObjectiveNetMessage.hpp"


char * NetMessageClassToString( NetMessage &message )
 {
  return( "Fuck That Shit" );

  /*
  int message_class = (int) message.message_class;
  
  switch ( message_class )
   {
     case _net_message_class_multi:
      return( "MultiPacket" );

	 case _net_message_class_client_server:
	  return( "Client/Server" );

     case _net_message_class_system:
      return( "System" );
          
	 case _net_message_class_connect:
	  return( "Connect" );

     case _net_message_class_player:
	  return( "Player"  );

     case _net_message_class_unit:
	  return( "Unit" );

     case _net_message_class_terminal:
	  return( "Terminal" );

     case _net_message_class_objective:
      return( "Objective" );

     default:
      return( "Unknown" );
   } // ** switch
  */
 }


char * MessageClassClient( int message_id )
 {
  return( "Fuck That Shit" );
  /*
  switch( message_id )
   {
    case 0:
     return( "Server ID" );

    default :
     return( "Unknown" );
   }
  */
 }

char * MessageClassServer( int message_id )
 {
  return( "Fuck That Shit" );
  /*
  switch( message_id )
   {
    //case _net_message_id_server_client_disconnect:
    // return( "Client Disconnect" );

    default :
     return( "Unknown" );
   }
  */
 }

char * MessageClassSystem( int message_id )
 {
  return( "Fuck That Shit" );
  /*
  switch( message_id )
   {
    case _net_message_id_system_set_view:
     return( "Set Player View" );

    case _net_message_id_system_reset_game_logic:
     return( "Reset Game Logic" );

	case _net_message_id_system_chat_mesg_req:
	 return( "Chat Mesg Request" );
      
    case _net_message_id_system_chat_mesg:
	 return( "Chat Mesg" );

    case _net_message_id_system_view_control:
	 return( "View Control" );

    case _net_message_id_system_ping_request:
	 return( "Ping Request" );

    case _net_message_id_system_ping_ack:
     return( "Ping Acknowledge" );

    default :
     return( "Unknown" );
   }
  */
 }

char * MessageClassConnect( int message_id )
 {
  return( "Fuck That Shit" );
  /*
  switch( message_id )
   {
    case _net_message_id_connect_join_game_request_ack:
     return( "Link Acknownledged" );

    case _net_message_id_client_start_connect:
     return( "Start Connection" );
         
    case _net_message_id_client_connect_request:
	 return( "Connect Request" );

    case _net_message_id_client_connect_result:
	 return( "Connect Result" );

    case _net_message_id_connect_client_settings:
	 return( "Client Game Settings" );

    case _net_message_id_client_connect_process_update:
	 return( "Connect Process Update" );

    case _net_message_id_client_connect_process_state_mesg:
	 return( "Connect Process State Mesg" );

    case _net_message_id_connect_server_game_setup:
	 return( "Server Game Setup" );

    case _net_message_id_connect_client_game_setup_ack: 
     return( "Game Setup Acknownledge" );

    default :
     return( "Unknown" );
   }
  */
 }

char * MessageClassPlayer( int message_id )
 {
  return( "Fuck That Shit" );
  /*
  switch( message_id )
   {
    case _net_message_id_player_connect_id :
     return( "Player Connect ID" );

	case _net_message_id_player_sync_state :
	 return( "Sync Player State" );

    case _net_message_id_player_score_update :
	 return( "Score Update" );

    case _net_message_id_player_alliance_request :
	 return( "Alliance Request" );
    
    case _net_message_id_player_alliance_update :
     return( "Alliance Update" );

    default :
     return( "Unknown" );
   }
  */
 }

char * MessageClassUnit( int message_id )
 {
  return( "Fuck That Shit" );
  /*
  switch( message_id )
   {
    case _net_message_id_opcode_mesg :
     return( "Opcode Message" );

	case _net_message_id_ini_sync_mesg :
	 return( "Initial Sync" );

    case _net_message_id_destroy_unit :
	 return( "Destroy Unit" );

    case _net_message_id_create_unit :
	 return( "Create Unit" );

    default :
     return( "Unknown" );
   }
  */
 }

char * MessageClassTerminal( int message_id )
 {
  return( "Fuck That Shit" );
  /*
  switch( message_id )
   {
    case _net_message_id_term_unit_cmd :
     return( "Unit Command" );
      
    case _net_message_id_term_unit_gen :
	 return( "Unit Generation Command" );

    default :
     return( "Unknown" );
   }
  */
 }


char * MessageClassObjective( int message_id )
 {
  return( "Fuck That Shit" );

  /*
  switch( message_id )
   {
    case _net_message_id_occupation_status_update :
     return( "Occupation Status Update" );

    default :
     return( "Unknown" );
   }
  */
 }




char * NetMessageIDtoString( NetMessage &message )
 {
  return( "Fuck That Shit" );

  /*
  int message_class = (int) message.message_class;
  int message_id = (int) message.message_id;

  switch ( message_class )
   {
     case _net_message_class_multi:
      return( "Standard Multi Message" );

	 
     case _net_message_class_client:
      return(  MessageClassClient( message_id )  );

     case _net_message_class_server:
	  return( MessageClassServer( message_id ) );
     
     case _net_message_class_system:
      return( MessageClassSystem( message_id ) );
          
	 case _net_message_class_connect:
	  return( MessageClassConnect( message_id ) );

     case _net_message_class_player:
	  return( MessageClassPlayer( message_id ) );

     case _net_message_class_unit:
	  return( MessageClassUnit( message_id ) );

     case _net_message_class_terminal:
	  return( MessageClassTerminal( message_id ) );

     case _net_message_class_objective:
      return( MessageClassObjective( message_id ) );

     default:
      return( "Unknown" );
   } // ** switch
   */
 }
