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
#include "NetworkServerWinSock.hpp"

#include "gapp.hpp"

#include "NetMessageLog.hpp"

#include "WinsockGlobals.h"
#include "WinsockClientList.h"
#include "WinsockServer.h"

#include "NetworkState.hpp"

#include "ServerConnectDaemon.hpp"

NetworkServerWinSock::NetworkServerWinSock()
  : NetworkServer()
 {

 }

NetworkServerWinSock::~NetworkServerWinSock()
 {

 }

int NetworkServerWinSock::winsockErrorToNetworkError( int winsock_error )
 {
  switch( winsock_error )
   {
    case WS_OK : 
     return( _network_ok );
 
    case WS_CONNECTION_LOST :   
     return( _network_err_send_connection_lost );

    case WS_PLAYER_NOT_FOUND :  
     return( _network_err_send_player_lost );

    case WS_NETWORK_DOWN :      
     return( _network_err_send_transport_broken );
     
    case WS_SOCKET_ERROR :      
     return( _network_err_send_transport_broken );
    
    case WS_WINSOCK_ERROR :     
     return( _network_err_send_transport_broken );

    default :
     return( _network_failed );
   }
 }

void NetworkServerWinSock::shutdownClientTransport( PlayerID &client_id )
 {
  DelClientByWinsockID( client_id.getDPID() );
 }


int NetworkServerWinSock::openSession( int connection_type, int session_flags )
 {
  return( InitWinSock(gapp.hwndApp) );
 
 }

int NetworkServerWinSock::hostSession( void )
 {
  //winsock hack
  if(InitStreamServer(gapp.hwndApp) == false) return false;

  if(InitDGramServer(gapp.hwndApp)== false) return false;

  return true;
 }

int NetworkServerWinSock::closeSession( void )
 {
  ShutdownWinSockServer();
  return( true );
 }

int NetworkServerWinSock::sendMessage( NetMessage *message, unsigned long size, int flags )
 {
  int sock_ret_val;
  int net_error_code;

  message->size = size;
 
  //LOG( ( "SEND >> Class: %s ID: %s", NetMessageClassToString( *message), NetMessageIDtoString( *message )  ) );
  
  ServerClientListData *iterator = 0;
  ServerClientListData *client_data_ptr = 0;

  client_list.resetIterator( &iterator );

  client_data_ptr = client_list.incIteratorPtr( &iterator );
  
  while( client_data_ptr != 0 )
   {
    //winsock hack
    if( flags & _network_send_no_guarantee )
     {
      if ( dontSendUDPHackFlag == false )
       {
        message->sequence = client_data_ptr->no_guarantee_sequence_counter;
        client_data_ptr->no_guarantee_sequence_counter++;

        sock_ret_val = WSSend( 0, client_data_ptr -> client_id.getDPID(), (char *) message, (DWORD) size);
       }
      else
       {
        client_data_ptr->no_guarantee_sequence_counter += 10;
        dontSendUDPHackFlag = false; 
       }
     }
    else
     {
      sock_ret_val = WSSend( 1, client_data_ptr -> client_id.getDPID(), (char *) message, (DWORD) size);
     }

    if( sock_ret_val != WS_OK )
     { 
      net_error_code = winsockErrorToNetworkError( sock_ret_val );
      
      ServerConnectDaemon::startClientDropProcess( client_data_ptr->client_id );
     }

    client_data_ptr = client_list.incIteratorPtr( &iterator );
   }

  
  NetworkState::incPacketsSent( size ); 
  
  return( true );
 }

int NetworkServerWinSock::sendMessage( NetMessage *message, unsigned long size, PlayerID &player_id, int flags )
 {
  ServerClientListData *client_data_ptr = 0;
  int ret_val;

  message->size = size;

  //LOG( ( "SEND >> Class: %s ID: %s", NetMessageClassToString( *message), NetMessageIDtoString( *message )  ) );
  
  //winsock hack
  if( flags & _network_send_no_guarantee )
   {
    client_data_ptr = client_list.getClientData( player_id );

    if( client_data_ptr == 0 )
     { return( _network_failed ); }
    
    message->sequence = client_data_ptr->no_guarantee_sequence_counter;
    client_data_ptr->no_guarantee_sequence_counter++;
    
    ret_val = WSSend( 0, player_id.getDPID(), (char *) message, (DWORD) size); 
   }
  else
   {
    ret_val = WSSend( 1, player_id.getDPID(), (char *) message, (DWORD) size);
   }

  NetworkState::incPacketsSent( size ); 

  if( ret_val != WS_OK )
   return( winsockErrorToNetworkError( ret_val ) );

  return( _network_ok );
 }

int NetworkServerWinSock::getMessage( NetMessage *message )
 {
  updateKeepAliveState();
      
  if ( loop_back_recv_queue.isReady() )
   {
    loop_back_recv_queue.dequeue( &net_packet );
	memmove( (void *) message, net_packet.data, net_packet.packet_size );

	return( true );
   }
  else
   {
    if ( receive_queue.isReady() )
     {
      receive_queue.dequeue( &net_packet );

      memmove(  (void *) message, net_packet.data, net_packet.packet_size );
	  NetworkState::incPacketsReceived( net_packet.packet_size );
	    
	  //LOG( ( "RECV >> Class: %s ID: %s", NetMessageClassToString( *message), NetMessageIDtoString( *message )  ) );        
        
      if ( message->message_class == _net_message_class_client_server )
	   { processNetMessage( message ); }

	  return( true );        
	 }
   } // ** else 
    
  return( false );
 }
