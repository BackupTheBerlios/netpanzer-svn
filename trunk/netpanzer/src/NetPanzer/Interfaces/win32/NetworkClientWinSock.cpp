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
#include "NetworkClientWinSock.hpp"

#include "gapp.hpp"

#include "NetworkState.hpp"
#include "NetMessageLog.hpp"

#include "codewiz.hpp"

#include "WinsockGlobals.h"
#include "WinsockServer.h"
#include "WinsockClient.h"

#include "Desktop.hpp"

NetworkClientWinSock::NetworkClientWinSock( void )
 : NetworkClient()
 {
  
 }

NetworkClientWinSock::~NetworkClientWinSock()
 {

 }

int NetworkClientWinSock::winsockErrorToNetworkError( int winsock_error )
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

int NetworkClientWinSock::openSession( int connection_type, int session_flags ) 
 {
  NetworkClient::openSession( connection_type, session_flags );

  if ( connection_type == _connection_loop_back )
   {
    NetworkClientWinSock::connection_type = _connection_loop_back;
    return( true );
   }
  else
   {
    NetworkClientWinSock::connection_type = connection_type;
    InitWinSock(gapp.hwndApp);
    InitDgramClient(gapp.hwndApp);
    //HACK -- needs to return something real from last two calls--
    return(true);
   }
 }
    
int NetworkClientWinSock::startEnumeration( ConnectionAddress address )   
 {
  int iReturn;
  
  iReturn = AsyncGetHost(gapp.hwndApp, (LPSTR) address.string_rep );
 
  if(iReturn == WS_OK)
   { return( true ); }
  
  return( false ); 
 }

int NetworkClientWinSock::startEnumeration( )   
 {
  Desktop::setVisibility("IPAddressView", true);
  return( true ); 
 }

int NetworkClientWinSock::stopEnumeration( void )
 {
  return( true );   
 }

int NetworkClientWinSock::getSessionList( SessionList &list )
 {
  SessionInfo session_info;


  session_list.deallocate();
  list.deallocate();

  QueryServer();

  if ( GetHostInfo( session_info.name, session_info.map, session_info.game_type,
                    &session_info.current_players, &session_info.max_players ) == true )
   {
    session_list.initialize( 1 );
    list.initialize( 1 );
    memcpy( &session_list[ 0 ], &session_info, sizeof( SessionInfo ) );
    memcpy( &list[ 0 ], &session_info, sizeof( SessionInfo ) );
   }
  
  return( true );
 }

int NetworkClientWinSock::joinSession( void )
 {
  if ( InitStreamClient(gapp.hwndApp) == true )
   {
    return( true );
   }
  else
   {
    return( false );
   }
 }

int NetworkClientWinSock::joinSession( int session_index ) 
 {
  
  return( false );
 }

int NetworkClientWinSock::joinSession( const char session_name ) 
 {
  return( false );
 }

int NetworkClientWinSock::setJoinSession( const char *session_name )
 {
  return( true );
 }

int NetworkClientWinSock::closeSession( void ) 
 {
  ShutdownWinSockClient();
  return( true );
 }
  
int NetworkClientWinSock::sendMessage( NetMessage *message, unsigned long size, int flags ) 
 {
  if ( connection_type == _connection_loop_back )
   {
	memmove( net_packet.data, (void *) message, size  );
    net_packet.packet_size = (unsigned short) size;
	loop_back_recv_queue.enqueue( net_packet );    
   }
  else
   {
    if ( connection_status == _connection_status_connected  ) 
     {  
      int ret_val; 

      message->size = size;

      if( flags & _network_send_no_guarantee )
       {
        ret_val = WSClientToServer( 0, ( char *) message, size );
       }
      else
       {
        ret_val = WSClientToServer( 1, ( char *) message, size );
       }

      if( ret_val != WS_OK )
       {
        connection_status = _connection_status_no_connection;
        return( winsockErrorToNetworkError( ret_val ) );
       }
      
      NetworkState::incPacketsSent( size );
     }
    } // ** else

  return( _network_ok );
 }
 
int NetworkClientWinSock::getMessage( NetMessage *message ) 
 {
  updateKeepAliveState();

  if ( receive_queue.isReady() )
   {
    receive_queue.dequeue( &net_packet );
    	    
    memmove(  (void *) message, net_packet.data, net_packet.packet_size );

    //LOG( ( "RECV >> Class: %s ID: %s", NetMessageClassToString( *message), NetMessageIDtoString( *message )  ) );

    if ( message->message_class == _net_message_class_client_server )
	 {  processNetMessage( message ); }

	NetworkState::incPacketsReceived( net_packet.packet_size );

    return( true );  
   }
  
  if ( non_guarantee_queue.isReady() )
   {
    non_guarantee_queue.dequeue( &net_packet );
    	    
    memmove(  (void *) message, net_packet.data, net_packet.packet_size );

    //LOG( ( "RECV >> Class: %s ID: %s", NetMessageClassToString( *message), NetMessageIDtoString( *message )  ) );

    if ( message->message_class == _net_message_class_client_server )
	 {  processNetMessage( message ); }

	NetworkState::incPacketsReceived( net_packet.packet_size );

    return( true );  
   }


  return( false );
 }

