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

#include "Log.hpp"
#include "NetworkServer.hpp"

#include "ClientServerNetMessage.hpp"
#include "ServerConnectDaemon.hpp"

#include "NetMessageLog.hpp"

#include "codewiz.hpp"
#include "ConsoleInterface.hpp"

//***************************************************************
// Class: ServerClientList
//***************************************************************

void ServerClientList::addClient( ServerClientListData *client_data )
 {
  addRear( client_data );
 }
  
bool ServerClientList::removeClient( PlayerID client_id )
 {
  ServerClientListData *client_data_ptr;

  if ( front == 0 )
   return( false );

  client_data_ptr = front;

  if ( client_data_ptr->client_id == client_id  )
   {
    deleteFront();
    return( true );    
   }
  
  while( client_data_ptr->next != 0 )
   {
    if ( client_data_ptr->next->client_id == client_id ) 
     {
      deleteAfter( client_data_ptr );
      return( true ); 
     }
    
    client_data_ptr = client_data_ptr->next;
   }

  return( false );
 }

ServerClientListData * ServerClientList::getClientData( PlayerID client_id )
 {
  ServerClientListData *client_data_ptr;

  client_data_ptr = front;
  
  while( client_data_ptr != 0 )
   {
    if ( client_data_ptr->client_id == client_id ) 
     {
      return( client_data_ptr ); 
     }
    
    client_data_ptr = client_data_ptr->next;
   }

  return( 0 );
 }

bool ServerClientList::getFullClientID( PlayerID *client_id )
 {
  ServerClientListData *client_data_ptr;

  client_data_ptr = front;
  
  while( client_data_ptr != 0 )
   {
    if ( client_data_ptr->client_id.getNetworkID() == client_id->getNetworkID() ) 
     {
      (*client_id) = client_data_ptr->client_id;
      return( true );
     }
    
    client_data_ptr = client_data_ptr->next;
   }

  return( false );
 }


//***************************************************************
// Class: NetworkServer
//***************************************************************

NetworkServer::NetworkServer( void )
  : NetworkInterface()
 {
  keep_alive_emit_timer.changePeriod( _SERVER_KEEP_ALIVE_SEND_INTERVAL );
  resetClientList();
  dontSendUDPHackFlag = false;
 }
  
NetworkServer::~NetworkServer()
 {
  client_list.deallocate();
 }

void NetworkServer::resetClientList( void )
 {
  client_list.deallocate();
 }

bool NetworkServer::addClientToSendList( PlayerID &client_player_id )
 {
  ServerClientListData *client_data = 0;

  client_data = new ServerClientListData;
  
  if( client_data == 0 )
   { 
    return( false );
   }
  
  client_data->client_id = client_player_id;
  client_data->keep_alive_timer.changePeriod( _CLIENT_KEEP_ALIVE_THRESHOLD );
  
  client_list.addClient( client_data );
  
  return( true );
 }
   
bool NetworkServer::removeClientFromSendList( PlayerID &client_player_id ) 
 {
  return(  client_list.removeClient( client_player_id ) );
 }

bool NetworkServer::activateKeepAlive( PlayerID &client_player_id )
 {
  ServerClientListData *client_data = 0;

  client_data = client_list.getClientData( client_player_id );
    
  if( client_data == 0 )
   { 
    LOG( ("NetworkServer::activateKeepAlive -- Could Not Find Client To Activate Keep Alive" ) );
    return( false );
   }
  
  client_data->keep_alive_state = true;
  client_data->keep_alive_timer.reset();
  
  ClientMesgSetKeepAlive set_keep_alive_mesg;

  set_keep_alive_mesg.keep_alive_state = true;
  
  sendMessage( &set_keep_alive_mesg, sizeof(ClientMesgSetKeepAlive), client_data->client_id, 0  );  
  return( true ); 
 } 

bool NetworkServer::deactivateKeepAlive( PlayerID &client_player_id ) 
 {
  ServerClientListData *client_data = 0;

  client_data = client_list.getClientData( client_player_id );
    
  if( client_data == 0 )
   { 
    return( false );
   }
  
  client_data->keep_alive_state = false;
  client_data->keep_alive_timer.reset();
  
  ClientMesgSetKeepAlive set_keep_alive_mesg;

  set_keep_alive_mesg.keep_alive_state = false;
  
  sendMessage( &set_keep_alive_mesg, sizeof(ClientMesgSetKeepAlive), client_data->client_id, 0  );  
  return( true ); 
 }

void NetworkServer::netMessageClientKeepAlive( NetMessage *message )
 {
  ServerClientListData *client_data = 0;
  ServerMesgKeepAlive  *client_keepalive = 0;
  client_keepalive = (ServerMesgKeepAlive *) message;
      
  client_data = client_list.getClientData( client_keepalive->client_id );

  if( client_data == 0 )
   { 
    LOG( ("Invalid ClientID for KeepAlive") );
    return;
   }
      
  if ( client_data->keep_alive_state == true )
   {
    client_data->keep_alive_timer.reset();
   }
             
 }

void NetworkServer::netMessageServerPingRequest( NetMessage *message )
 {
  ServerMesgPingRequest *ping_request_mesg;
  ClientMesgPingAck      ping_ack_mesg;

  ping_request_mesg = (ServerMesgPingRequest *) message;
  
  sendMessage( &ping_ack_mesg, sizeof( ClientMesgPingAck ), ping_request_mesg->client_id, _network_send_no_guarantee );             
 }

void NetworkServer::netMessageTransportClientAccept( NetMessage *message )
 {
  TransportClientAccept *client_accept_mesg;
  ClientMesgConnectAck  connect_ack_mesg;
  PlayerID player_id;

  client_accept_mesg = (TransportClientAccept *) message;

  connect_ack_mesg.client_transport_id = client_accept_mesg->client_transport_id;
  
  player_id = PlayerID( 0, connect_ack_mesg.client_transport_id ); 

  sendMessage( &connect_ack_mesg, sizeof( ClientMesgConnectAck ), player_id, 0 );             
 }


void NetworkServer::processNetMessage( NetMessage *message )
 {
  switch ( message->message_id )
   {

    case _net_message_id_server_keep_alive :
     { netMessageClientKeepAlive( message ); } 
    break;

    case _net_message_id_server_ping_request :
     { netMessageServerPingRequest( message ); } 
    break;

    case _net_message_id_transport_client_accept :
     { netMessageTransportClientAccept( message ); } 
    break;

   } // ** switch

 }



void NetworkServer::updateKeepAliveState( void )
 {
  bool send_server_keep_alive_mesg = false;
  ServerClientListData *iterator = 0;
  ServerClientListData *client_data_ptr = 0;

  if ( keep_alive_emit_timer.count() )
   {
    send_server_keep_alive_mesg = true;
   }
  
  client_list.resetIterator( &iterator );

  client_data_ptr = client_list.incIteratorPtr( &iterator );
  
  while( client_data_ptr != 0 )
   {
    if ( client_data_ptr->keep_alive_state == true )
     {
      if( client_data_ptr->keep_alive_timer.count() )
       { 
        LOG( ("Client %d Keep Alive Timed Out", client_data_ptr->client_id.getIndex() ) ); 
        ServerConnectDaemon::startClientDropProcess( client_data_ptr->client_id );
       }
     
      if( send_server_keep_alive_mesg == true )
       {
        ClientMesgKeepAlive server_keepalive;
        
        sendMessage( &server_keepalive, sizeof( ClientMesgKeepAlive ), client_data_ptr->client_id, 0 );          
       }
     }
    client_data_ptr = client_list.incIteratorPtr( &iterator );
   } 
 
 }

void NetworkServer::dropClient( PlayerID client_id )
 {
  client_list.getFullClientID( &client_id );
  ServerConnectDaemon::startClientDropProcess( client_id );
 }
