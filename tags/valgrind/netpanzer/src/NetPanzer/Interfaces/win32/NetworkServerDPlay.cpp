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
#include "NetworkServerDPlay.hpp"

#include "gapp.hpp"

#include "NetMessageLog.hpp"

#include "DirectPlay.h"
#include "DPlayError.hpp"
#include "DirectDrawGlobals.hpp"

#include "MouseInterface.hpp"

#include "NetworkState.hpp"

#include "ServerConnectDaemon.hpp"

// XXX Code broken and probably not neccessary when the socket stuff is
// working
#if 0

NetworkServerDPlay::NetworkServerDPlay()
  : NetworkServer()
 {

 }

NetworkServerDPlay::~NetworkServerDPlay()
 {

 }

int NetworkServerDPlay::openSession( int connection_type, int session_flags )
 {
  unsigned int DPlayReturnValue;
  unsigned int ServProvReturn;
   
  DPlayReturnValue = InitializeDirectPlay( gapp.hwndApp);
     
  if( DPlayReturnValue != true )
   { return( false ); }

  ServProvReturn = SetServProv( gapp.hwndApp, TCPIP );
  if( ServProvReturn == 0 ) 
   { return( false ); }

  return( true );
 }

int NetworkServerDPlay::hostSession( void )
 {
  HRESULT hr;
  bool minimize;

  DDraw.setGDIStatus(true);
  minimize = MinimizeOrNot( gapp.hwndApp );    

  //SetServProv( gapp.hwndApp, TCPIP );

  hr = HostSession( gapp.hwndApp );                 
  
  if (minimize == false)
   { 
    DDraw.setGDIStatus(false);
   }  
  else
   {
    OpenIcon( gapp.hwndApp ); 
    DDraw.restoreAll();
    DDraw.setGDIStatus( false ); 
   }

  MouseInterface::hideHardwareCursor();   

  if( hr != 1 )
  { return( false ); }

  return true;
 }

int NetworkServerDPlay::closeSession( void )
 {
  ShutDownConnection();
  return( true );
 }

int NetworkServerDPlay::sendMessage( NetMessage *message, unsigned long size, int flags )
 {
  HRESULT hr;
  message->size = size;
 
  //LOG( ( "SEND >> Class: %s ID: %s", NetMessageClassToString( *message), NetMessageIDtoString( *message )  ) );
  
  ServerClientListData *iterator = 0;
  ServerClientListData *client_data_ptr = 0;

  client_list.resetIterator( &iterator );

  client_data_ptr = client_list.incIteratorPtr( &iterator );
  
  while( client_data_ptr != 0 )
   {
     hr = ServerToOneClient( true, client_data_ptr->client_id.getDPID(), (void *) message, size ); 
  
     if ( hr != DP_OK )
      {
       LOG( ( DPlayErrorToString( hr ) ) );
	
       #ifdef _USE_DPLAY_KEEPALIVE_CHECKING
        if (hr == DPERR_INVALIDPLAYER )
          {
           ServerConnectDaemon::startClientDropProcess( client_data_ptr->client_id );
          }
        else
         if (hr == DPERR_GENERIC )
          {
           ServerConnectDaemon::startClientDropProcess( client_data_ptr->client_id );
          }
       #endif
      }
    
    client_data_ptr = client_list.incIteratorPtr( &iterator );
   }

  
  NetworkState::incPacketsSent( size ); 
  
  return( true );
 }

int NetworkServerDPlay::sendMessage( NetMessage *message, unsigned long size, PlayerID &player_id, int flags )
 {
  HRESULT hr;

  message->size = size;

  //LOG( ( "SEND >> Class: %s ID: %s", NetMessageClassToString( *message), NetMessageIDtoString( *message )  ) );
  
  hr = ServerToOneClient( true, player_id.getDPID(), (void *) message, size ); 
  
  if ( hr != DP_OK )
   { 
    LOG( ( DPlayErrorToString( hr ) ) ); 

    #ifdef _USE_DPLAY_KEEPALIVE_CHECKING
     if (hr == DPERR_INVALIDPLAYER )
      {
       ServerConnectDaemon::startClientDropProcess( player_id );
      }
     else
      if (hr == DPERR_GENERIC )
       {
        ServerConnectDaemon::startClientDropProcess( player_id );
       }
    #endif

    return( _network_err_send_player_lost ); 
   }
 
  NetworkState::incPacketsSent( size ); 

  return( _network_ok );
 }



void NetworkServerDPlay::handleDPSystemMessage( NetPacket *packet )
 {
  LPDPMSG_GENERIC lpMesg;

  lpMesg = (LPDPMSG_GENERIC) packet->data;
  switch( lpMesg->dwType )
   {
   	case DPSYS_CREATEPLAYERORGROUP:
     {
      LPDPMSG_CREATEPLAYERORGROUP lpCreateMesg;	  
	  lpCreateMesg = (LPDPMSG_CREATEPLAYERORGROUP) lpMesg;
      PlayerID player;
	  
	  player.setIndex(0);
	  player.setDPID( lpCreateMesg->dpId );
	  
	  ServerConnectDaemon::startConnectionProcess( player );
	 } break;

    case DPSYS_DESTROYPLAYERORGROUP :
     {
      /*
      LPDPMSG_DESTROYPLAYERORGROUP lpDestroyMesg;	  
	  lpDestroyMesg = (LPDPMSG_DESTROYPLAYERORGROUP) lpMesg;
      PlayerID player;
	  
	  player.setIndex(0);
	  player.setDPID( lpDestroyMesg->dpId );
	  
	  if( client_list.getFullClientID( &player )  ) 
       {
        ServerConnectDaemon::startClientDropProcess( player );
       }
      */
     } break;

   }// ** switch

 }

int NetworkServerDPlay::getMessage( NetMessage *message )
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

      if ( net_packet.fromID == DPID_SYSMSG )
	   {
	    handleDPSystemMessage( &net_packet );
	   }
      else
       {
        memmove(  (void *) message, net_packet.data, net_packet.packet_size );
	    NetworkState::incPacketsReceived( net_packet.packet_size );
	    
	    //LOG( ( "RECV >> Class: %s ID: %s", NetMessageClassToString( *message), NetMessageIDtoString( *message )  ) );        
        
        if ( message->message_class == _net_message_class_client_server )
	     { processNetMessage( message ); }

	    return( true );        
	   }
     } // if ( receive_queue.isReady() )
   } // ** else 
    
  return( false );
 }

#endif
