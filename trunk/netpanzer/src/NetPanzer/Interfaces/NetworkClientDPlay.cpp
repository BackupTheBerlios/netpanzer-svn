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
#include "stdafx.hpp"
#include "NetworkClientDPlay.hpp"

#include "gapp.hpp"

#include "NetworkState.hpp"
#include "NetMessageLog.hpp"

#include "Codewiz.hpp"

#include "DirectPlay.h"
#include "DPlayError.hpp"
#include "DirectDrawGlobals.hpp"

#include "MouseInterface.hpp"

#include "ClientConnectDaemon.hpp"

NetworkClientDPlay::NetworkClientDPlay( void )
 : NetworkClient()
 {
  
 }

NetworkClientDPlay::~NetworkClientDPlay()
 {

 }


int NetworkClientDPlay::openSession( int connection_type, int session_flags ) 
 {
  if ( connection_type == _connection_loop_back )
   {
    NetworkClientDPlay::connection_type = _connection_loop_back;
    return( _TRUE );
   }
  else
   {
    NetworkClientDPlay::connection_type = connection_type;
  
    unsigned int DPlayReturnValue;
    unsigned int ServProvReturn;
   
    DPlayReturnValue = InitializeDirectPlay( gapp.hwndApp);
     
    if( DPlayReturnValue != TRUE )
     { return( _FALSE ); }

    ServProvReturn = SetServProv( gapp.hwndApp, TCPIP );
    if( ServProvReturn == 0 ) 
     { return( _FALSE ); }

    return(_TRUE);
   }
 }
    
int NetworkClientDPlay::startEnumeration( ConnectionAddress address )   
 {
  
  return( _FALSE ); 
 }

int NetworkClientDPlay::startEnumeration( )   
 {
  HRESULT hr;
  BOOL minimize;

  Screen->setGDIStatus(true);
  
  minimize = MinimizeOrNot(gapp.hwndApp);
  hr =	EnumerateGames( gapp.hwndApp );

  if (minimize == _FALSE)
   {
	Screen->setGDIStatus(false);
   }

  if( (hr == DPERR_USERCANCEL) || (hr == DPERR_EXCEPTION) || (hr == DPERR_GENERIC) )
   {
    return( _FALSE );
   }

  MouseInterface::hideHardwareCursor();   

  return( _TRUE ); 
 }

int NetworkClientDPlay::stopEnumeration( void )
 {
  StopAsyncGameEnumeration( gapp.hwndApp );
  return( _TRUE );   
 }

int NetworkClientDPlay::getSessionList( SessionList &list )
 {
  HRESULT hr;
  int  game_count;
  int  game_index;  

  session_list.deallocate();
  list.deallocate();

  hr =	EnumerateGames( gapp.hwndApp );

  if( (hr == DPERR_USERCANCEL) || (hr == DPERR_EXCEPTION) || (hr == DPERR_GENERIC) )
   {
    return( _FALSE );
   }

  game_count = GetNumGames();
  
  session_list.initialize( game_count );
  list.initialize( game_count );

  for( game_index = 0; game_index < game_count; game_index++ )
   {
    GetGameName( game_index, session_list[ game_index ].name );
    GetGameName( game_index, list[ game_index ].name );
   }
 
  return( _TRUE );
 }

int NetworkClientDPlay::joinSession( void )
 {
  HRESULT hr;

  hr = JoinSession( gapp.hwndApp );

  if ( hr == FALSE )
   { return( _FALSE ); }

  GetServerPlayerDPID( &server_DPID );
  return( _TRUE );
 }

int NetworkClientDPlay::joinSession( int session_index ) 
 {
  
  return( _FALSE );
 }

int NetworkClientDPlay::joinSession( const char session_name ) 
 {
  return( _FALSE );
 }
 
int NetworkClientDPlay::setJoinSession( const char *session_name )
 {
  if ( SetSelectedGame( session_name ) == FALSE )
   { return( _FALSE ); }

  return( _TRUE );
 }
  
int NetworkClientDPlay::closeSession( void ) 
 {
  ShutDownConnection();
  return( _TRUE );
 }
  
int NetworkClientDPlay::sendMessage( NetMessage *message, unsigned long size, int flags ) 
 {
  if ( connection_type == _connection_loop_back )
   {
	memmove( net_packet.data, (void *) message, size  );
    net_packet.packet_size = (unsigned short) size;
	loop_back_recv_queue.enqueue( net_packet );    
   }
  else
   {
    //if ( connection_status == _connection_status_connected  ) 
     {  
      HRESULT hr;

      message->size = size;
        
      //LOG( ( "SEND >> Class: %s ID: %s", NetMessageClassToString( *message), NetMessageIDtoString( *message )  ) );

      hr = ClientToServer( ( void *) message, server_DPID, size );

      if ( hr != DP_OK )
       { 
        LOG( ( DPlayErrorToString( hr ) ) ); 
       
        #ifdef _USE_DPLAY_KEEPALIVE_CHECKING
        if (hr == DPERR_INVALIDPLAYER )
         {
          ClientConnectDaemon::serverConnectionBroken();
          keep_alive_state = _FALSE;
          connection_status = _connection_status_no_connection;
          closeSession();
         }
        #endif

        return( _network_err_send_player_lost ); 
       }
        
      NetworkState::incPacketsSent( size );
     }
    } // ** else

  return( _network_ok );
 }
 
void NetworkClientDPlay::handleDPSystemMessage( NetPacket *packet )
 {
  LPDPMSG_GENERIC lpMesg;

  lpMesg = (LPDPMSG_GENERIC) (net_packet.data);
  switch( lpMesg->dwType )
   {
    case DPSYS_SESSIONLOST :
     {
      LOG( ("DPlay System Message : Session Lost" ) );
             
      #ifdef _USE_DPLAY_KEEPALIVE_CHECKING 
      ClientConnectDaemon::serverConnectionBroken();
      connection_status = _connection_status_no_connection;
      keep_alive_state = _FALSE;
      ShutDownConnection();
      #endif

     }break;
   
   }      
 }

int NetworkClientDPlay::getMessage( NetMessage *message ) 
 {
  updateKeepAliveState();

  if ( receive_queue.isReady() )
   {
    receive_queue.dequeue( &net_packet );
    	    
    if ( net_packet.fromID == DPID_SYSMSG )
	 {
      handleDPSystemMessage( &net_packet );      
     }
    else
     {
      memmove(  (void *) message, net_packet.data, net_packet.packet_size );

      //LOG( ( "RECV >> Class: %s ID: %s", NetMessageClassToString( *message), NetMessageIDtoString( *message )  ) );

      if ( message->message_class == _net_message_class_client_server )
	   {  processNetMessage( message ); }

	  NetworkState::incPacketsReceived( net_packet.packet_size );

      return( _TRUE );  
     }
   }
  return( _FALSE );
 }
