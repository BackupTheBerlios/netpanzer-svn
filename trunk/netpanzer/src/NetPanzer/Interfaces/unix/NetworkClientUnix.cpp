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
#include "NetworkClientUnix.hpp"

#include "gapp.hpp"

#include "NetworkState.hpp"
#include "NetMessageLog.hpp"

#include "Desktop.hpp"

// XXX stub implementation for now

NetworkClientUnix::NetworkClientUnix( void )
 : NetworkClient()
{
}

NetworkClientUnix::~NetworkClientUnix()
{
}

int NetworkClientUnix::openSession( int connection_type, int session_flags ) 
{
#if 0
  NetworkClient::openSession( connection_type, session_flags );

  if ( connection_type == _connection_loop_back )
   {
    NetworkClientUnix::connection_type = _connection_loop_back;
    return( true );
   }
  else
   {
    NetworkClientUnix::connection_type = connection_type;
    InitWinSock(gapp.hwndApp);
    InitDgramClient(gapp.hwndApp);
    //HACK -- needs to return something real from last two calls--
    return(true);
   }
#endif
  return true;
 }
    
int NetworkClientUnix::startEnumeration( ConnectionAddress address )   
 {
#if 0
  int iReturn;
  
  iReturn = AsyncGetHost(gapp.hwndApp, (LPSTR) address.string_rep );
 
  if(iReturn == WS_OK)
   { return( true ); }
  
  return( false ); 
#endif
  return false;
 }

int NetworkClientUnix::startEnumeration( )   
 {
  Desktop::setVisibility("IPAddressView", true);
  return( true ); 
 }

int NetworkClientUnix::stopEnumeration( void )
 {
  return( true );   
 }

int NetworkClientUnix::getSessionList( SessionList &list )
 {
#if 0
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
#endif
  return( true );
 }

int NetworkClientUnix::joinSession( void )
 {
#if 0
  if ( InitStreamClient(gapp.hwndApp) == true )
   {
    return( true );
   }
  else
   {
    return( false );
   }
#endif
  return false;
 }

int NetworkClientUnix::joinSession( int session_index ) 
 {
  
  return( false );
 }

int NetworkClientUnix::joinSession( const char session_name ) 
 {
  return( false );
 }

int NetworkClientUnix::setJoinSession( const char *session_name )
 {
  return( true );
 }

int NetworkClientUnix::closeSession( void ) 
 {
  //ShutdownWinSockClient();
  return( true );
 }
  
int NetworkClientUnix::sendMessage( NetMessage *message, unsigned long size, int flags ) 
 {
#if 0
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
#endif
  return true;
 }
 
int NetworkClientUnix::getMessage( NetMessage *message ) 
 {
#if 0
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

#endif

  return( false );
 }

