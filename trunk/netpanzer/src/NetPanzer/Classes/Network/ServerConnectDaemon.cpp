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
#include "ServerConnectDaemon.hpp"

#include "NetworkGlobals.hpp"

#include "PlayerInterface.hpp"
#include "UnitInterface.hpp"
#include "ObjectiveInterface.hpp"
#include "PowerUpInterface.hpp"
#include "MapInterface.hpp"
#include "Server.hpp"
#include "GameManager.hpp"
#include "GameConfig.hpp"

#include "PlayerNetMessage.hpp"
#include "ConnectNetMessage.hpp"
#include "SystemNetMessage.hpp"

#include "codewiz.hpp"
#include "ConsoleInterface.hpp"

enum { _connect_state_idle,
       _connect_state_wait_for_connect_request,
       _connect_state_attempt_player_alloc,
       _connect_state_wait_for_client_settings,
       _connect_state_wait_for_client_game_setup_ack,
       _connect_state_player_state_sync,
	   _connect_state_unit_sync
     };
 

unsigned char       ServerConnectDaemon::connection_state = _connect_state_idle;
bool             ServerConnectDaemon::connection_lock_state = false; 
PlayerID            ServerConnectDaemon::connect_player_id;
PlayerState        *ServerConnectDaemon::connect_player_state;
ServerConnectQueue  ServerConnectDaemon::connect_queue;
Timer		        ServerConnectDaemon::time_out_timer;
int	                ServerConnectDaemon::time_out_counter;

#define _SERVER_CONNECT_TIME_OUT_TIME (20.0)
#define _SERVER_CONNECT_RETRY_LIMIT   (5)


void ServerConnectDaemon::initialize( unsigned long max_players )
 {
  connect_queue.initialize( 25 );  
  time_out_timer.changePeriod( _SERVER_CONNECT_TIME_OUT_TIME );
 }

void ServerConnectDaemon::startConnectDaemon( unsigned long max_players )
 {
  connect_queue.initialize( 25 );  
  time_out_timer.changePeriod( _SERVER_CONNECT_TIME_OUT_TIME );
 }
   
void ServerConnectDaemon::shutdownConnectDaemon( void )
 {
  ConnectMesgNetPanzerServerDisconnect server_disconnect;

  SERVER->sendMessage( &server_disconnect, 
                        sizeof(ConnectMesgNetPanzerServerDisconnect), 
                        0 );
 }

void ServerConnectDaemon::startConnectionProcess( PlayerID new_player_id )
 {
  ConnectQueueElement connect_request;


  if ( !inConnectQueue( new_player_id) )
   {

    connect_request.new_player_id  = new_player_id;
    connect_request.connect_status = _connect_status_waiting;

    connect_queue.enqueue( connect_request );

    ClientConnectJoinRequestAck link_ack;

    SERVER->sendMessage( &link_ack, 
                         sizeof(ClientConnectJoinRequestAck), 
                         new_player_id, 0 );
   }
 }

bool ServerConnectDaemon::inConnectQueue( PlayerID &new_player_id )
 {
  ConnectQueueElement connect_request;  

  unsigned long iterator;
  bool       completed;

  connect_queue.resetIterator( &iterator );

  connect_request = connect_queue.incIterator( &iterator, &completed );
  
  while( completed == false )
   {
	if ( connect_request.new_player_id.getDPID() == new_player_id.getDPID() )
	 { return( true ); }

    connect_request = connect_queue.incIterator( &iterator, &completed );      
   }
  
  if ( (connection_state != _connect_state_idle) &&
       (connect_player_id.getDPID() == new_player_id.getDPID()  )
	 )
   return( true );
  
  return( false );
 } 


void ServerConnectDaemon::updateQueuedClients( void )
 {
  ConnectQueueElement connect_request;  
  ConnectProcessUpdate process_update;

  unsigned long iterator;
  bool       completed;
  unsigned long queue_position = 1;

  connect_queue.resetIterator( &iterator );

  connect_request = connect_queue.incIterator( &iterator, &completed );
  
  while( completed == false )
   {
	process_update.queue_position = queue_position;

    SERVER->sendMessage( &process_update, 
                         sizeof(ConnectProcessUpdate), 
                         connect_request.new_player_id, 0 );

    connect_request = connect_queue.incIterator( &iterator, &completed );   
   
    queue_position++;
   }

 }

void ServerConnectDaemon::netMessageClientDisconnect( NetMessage *message )
 {
  ConnectMesgNetPanzerClientDisconnect *client_disconnect;

  client_disconnect = (ConnectMesgNetPanzerClientDisconnect *) message;

  startDisconnectionProcess( client_disconnect->client_id );  
 }

void ServerConnectDaemon::netMessageClientJoinRequest( NetMessage *message )
 {
  ClientConnectJoinRequest *join_request_mesg;
  ClientConnectJoinRequestAck join_request_ack;
  PlayerID new_player_id;
   
  join_request_mesg = (ClientConnectJoinRequest *) message;
  
  new_player_id = PlayerID(0, join_request_mesg->client_transport_id );

  if( (strstr( join_request_mesg->code_word, _NETPANZER_CODEWORD) != 0) &&
      (join_request_mesg->protocol_version == _NETPANZER_PROTOCOL_VERSION) 
    )
   {
    join_request_ack.result_code = _join_request_result_success;
   }
  else
   {
    join_request_ack.result_code = _join_request_result_invalid_protocol;
   }  
    
  join_request_ack.server_protocol_version = _NETPANZER_PROTOCOL_VERSION;

  if( join_request_ack.result_code == _join_request_result_success )
   {

    ConnectQueueElement connect_request;
   

    if ( !inConnectQueue( new_player_id) )
     {
      connect_request.new_player_id  = new_player_id;
      connect_request.connect_status = _connect_status_waiting;

      if ( connect_queue.isFull() == true )
       { join_request_ack.result_code = _join_request_result_server_busy; }
      else
       { connect_queue.enqueue( connect_request ); }
          
     }

   }
 
   SERVER->sendMessage( &join_request_ack, 
                        sizeof(ClientConnectJoinRequestAck), 
                        new_player_id, 
                        0 );
	   
 }

void ServerConnectDaemon::processNetMessage( NetMessage *message )
 {
  switch (message->message_id)
   {
    case _net_message_id_connect_netPanzer_client_disconnect :
     netMessageClientDisconnect( message );
    break;

    case _net_message_id_connect_join_game_request :
     netMessageClientJoinRequest( message );
    break;
       		
	default :
	 connectFsm( message );
   	break;
   } // ** switch 
 
 }       

void ServerConnectDaemon::sendConnectionAlert( PlayerID &player_id, int alert_enum )
 {
  SystemConnectAlert connect_alert;
  PlayerState *player_state = 0;
  
  player_state = PlayerInterface::getPlayerState( player_id );

  
  switch( alert_enum )
   {
    case _connect_alert_mesg_connect : 
     {
      connect_alert.set( player_id, _connect_alert_mesg_connect );
      ConsoleInterface::postMessage( "%s has joined the game.", player_state->getName() );
     } break;

    case _connect_alert_mesg_disconnect : 
     {
      connect_alert.set( player_id, _connect_alert_mesg_disconnect );
      ConsoleInterface::postMessage( "%s has left the game.", player_state->getName() );
     } break;
    
    case _connect_alert_mesg_client_drop : 
     {
      connect_alert.set( player_id, _connect_alert_mesg_client_drop );
      ConsoleInterface::postMessage( "Connection to %s has been unexpectedly broken.", player_state->getName() );   
     } break;

    default :
     assert(0);
   } // ** switch 
    
  SERVER->sendMessage( &connect_alert, sizeof(SystemConnectAlert), 0 ); 
 }

void ServerConnectDaemon::resetConnectFsm( void )
 {
  connection_state = _connect_state_idle;
  time_out_timer.reset();
  time_out_counter = 0;
 }


bool ServerConnectDaemon::connectStateIdle( void )
 {
  if ( connect_queue.isReady() && (connection_lock_state == false) )
   {
    ConnectQueueElement connect_request;

	connect_request   = connect_queue.dequeue();
	connect_player_id = connect_request.new_player_id;
	connection_state  = _connect_state_wait_for_connect_request;
		  
	ClientConnectStartConnect start_connect;
          
	int send_ret_val;
    
    send_ret_val  = SERVER->sendMessage( &start_connect, 
                                         sizeof(ClientConnectStartConnect), 
                                         connect_player_id, 0 );
		  
	if( send_ret_val != _network_ok )
     {
      resetConnectFsm();
      return( true );
     }
    
    time_out_timer.changePeriod( _SERVER_CONNECT_TIME_OUT_TIME );
    time_out_timer.reset();
	time_out_counter = 0;

	updateQueuedClients();
   }
  
  return( true );
 }


bool ServerConnectDaemon::connectStateWaitForConnectRequest( NetMessage *message )
 {
  if ( message != 0 )
   {
    if ( message->message_id == _net_message_id_client_connect_request )
     {
      connection_state = _connect_state_attempt_player_alloc;
      return( false );
     }
    else
     {
      if ( time_out_timer.count() )
	   { 		
        resetConnectFsm();
        return( true );
	   }
	    
      return( true );        
     }          
         
   } // ** if ( message != 0 )
  else
   {
    if ( time_out_timer.count() )
	 {
      resetConnectFsm();
      return( true );    		
	 }
		  
    return( true );        
   }
  
  return( true );      
 }

bool ServerConnectDaemon::connectStateAttemptPlayerAlloc( void )
 {
  ClientConnectResult connect_result;
  connect_player_state = PlayerInterface::allocateNewPlayer();
    
  int send_ret_val;

   if ( connect_player_state == 0 )
    {
     connect_result.result_code = _connect_result_server_full;
    	            
	 send_ret_val = SERVER->sendMessage( &connect_result, 
                                         sizeof(ClientConnectResult), 
                                         connect_player_id, 0 );
         
     resetConnectFsm();
     return( true );
    }
   else
    {          
     connect_result.result_code = _connect_result_success;
    
     send_ret_val = SERVER->sendMessage( &connect_result, 
                                         sizeof(ClientConnectResult), 
                                         connect_player_id, 0 );

  	 if( send_ret_val != _network_ok )
      {
       connect_player_state->setStatus( _player_state_free );
       resetConnectFsm();
       return( true );
      }

     
     time_out_timer.reset();
     connection_state = _connect_state_wait_for_client_settings;     
	 return( true );
    }

 }

bool ServerConnectDaemon::connectStateWaitForClientSettings( NetMessage *message )
 {
  int send_ret_val;

  if ( message != 0 )
   {
    if ( message->message_id == _net_message_id_connect_client_settings )
     {
      ConnectClientSettings *client_setting;
            
      client_setting = (ConnectClientSettings *) message;
      connect_player_state->setName( client_setting->player_name );
      connect_player_state->unit_config.setUnitColor( client_setting->unit_color );
      connect_player_state->setFlag( client_setting->player_flag );

      connect_player_state->setID( connect_player_id.getDPID() );
      connect_player_state->setStatus( _player_state_connecting );
      connect_player_id = connect_player_state->getPlayerID();
        		   
      // ** send server game setting map, units, player, etc.
      ConnectMesgServerGameSettings server_game_setup;
	  GameManager::getServerGameSetup( &server_game_setup ); 

	  send_ret_val = SERVER->sendMessage( &server_game_setup, 
                                          sizeof(ConnectMesgServerGameSettings), 
                                          connect_player_id, 0 );
	  
      if( send_ret_val != _network_ok )
       {
        connect_player_state->setStatus( _player_state_free );
        resetConnectFsm();
        return( true );
       }
      	           		    				         						
      time_out_timer.reset();
	  connection_state = _connect_state_wait_for_client_game_setup_ack;
      return( true );
      }
     else
      {
       if ( time_out_timer.count() )
 	    { 		
         connect_player_state->setStatus( _player_state_free );
         resetConnectFsm();
         return( true );
	    }
	    
       return( true);        
      }
    } // ** if ( message != 0 )
   else
    {
     if ( time_out_timer.count() )
 	  { 		
       connect_player_state->setStatus( _player_state_free );
       resetConnectFsm();
       return( true );
	  }
	    
     return( true );        
    }     
   
   return( true );      
 }

bool ServerConnectDaemon::connectStateWaitForClientGameSetupAck( NetMessage *message )
 {
  int send_ret_val;

  if ( message != 0 )
   {
    if ( message->message_id == _net_message_id_connect_client_game_setup_ack )
     {           
	  PlayerConnectID player_connect_mesg;
			
	  memmove( &player_connect_mesg.connect_state,
               connect_player_state,
               sizeof( PlayerState )
              );
            

      send_ret_val = SERVER->sendMessage( &player_connect_mesg, sizeof(PlayerConnectID), connect_player_id, 0 );

   	  if( send_ret_val != _network_ok )
       {
        connect_player_state->setStatus( _player_state_free );
        resetConnectFsm();
        return( true );
       }

	  PlayerInterface::startPlayerStateSync( connect_player_id );
         
	  ConnectProcessStateMessage state_mesg;
	  state_mesg.message_enum = _connect_state_message_sync_player_info;
      send_ret_val = SERVER->sendMessage( &state_mesg, sizeof(ConnectProcessStateMessage), connect_player_id, 0 );			   

   	  if( send_ret_val != _network_ok )
       {
        connect_player_state->setStatus( _player_state_free );
        resetConnectFsm();
        return( true );
       }

	  connection_state = _connect_state_player_state_sync;           
	 
      SERVER->activateKeepAlive( connect_player_id );
     }
    else
     if ( message->message_id == _net_message_id_connect_client_game_setup_ping )
      {
       time_out_timer.reset();
       return( true );
      }
     else
      {
       if ( time_out_timer.count() )
 	    { 		
         connect_player_state->setStatus( _player_state_free );
         resetConnectFsm();
         return( true );
	    }
	    
       return( true );        
      }
   
   }
  else
   {
    if ( time_out_timer.count() )
 	 { 		
      connect_player_state->setStatus( _player_state_free );
      resetConnectFsm();
      return( true );
	 }
	    
    return( true );   
   }	   	
  
  return( true );
 }

bool ServerConnectDaemon::connectStatePlayerStateSync( void )
 {
  ConnectProcessStateMessage state_mesg;
  int send_ret_val;
  int percent_complete;

  if ( PlayerInterface::syncPlayerState( &send_ret_val, &percent_complete ) == true )
   {
    if( send_ret_val != _network_ok )
     {
      connect_player_state->setStatus( _player_state_free );
      resetConnectFsm();
      return( true );      
     }

	state_mesg.message_enum = _connect_state_message_sync_player_info_percent;
    state_mesg.percent_complete = percent_complete;
    send_ret_val = SERVER->sendMessage( &state_mesg, sizeof(ConnectProcessStateMessage), connect_player_id, 0 );			

    if( send_ret_val != _network_ok )
     {
      connect_player_state->setStatus( _player_state_free );
      resetConnectFsm();
      return( true );      
     }
    
    UnitInterface::startRemoteUnitSync( connect_player_id );
		  		  
	state_mesg.message_enum = _connect_state_message_sync_units;
    send_ret_val = SERVER->sendMessage( &state_mesg, sizeof(ConnectProcessStateMessage), connect_player_id, 0 );			
		  
    if( send_ret_val != _network_ok )
     {
      connect_player_state->setStatus( _player_state_free );
      resetConnectFsm();
      return( true );      
     }
    
   	SERVER->addClientToSendList( connect_player_id );

    PlayerStateSync player_state_update;

    memmove( &player_state_update.player_state,
             connect_player_state,
             sizeof( PlayerState )
           );

    SERVER->sendMessage( &player_state_update, sizeof(PlayerStateSync), 0 );

  
    connection_state = _connect_state_unit_sync;
    return( true );
   }
  else 
   if ( percent_complete > 0 )
    {
  	 state_mesg.message_enum = _connect_state_message_sync_player_info_percent;
     state_mesg.percent_complete = percent_complete;
     send_ret_val = SERVER->sendMessage( &state_mesg, sizeof(ConnectProcessStateMessage), connect_player_id, 0 );			

     if( send_ret_val != _network_ok )
      {
       connect_player_state->setStatus( _player_state_free );
       resetConnectFsm();
       return( true );      
      }
    }

  return( true );
 }

bool ServerConnectDaemon::connectStateUnitSync( void )
 {
  ConnectProcessStateMessage state_mesg;
  int send_ret_val;
  int percent_complete;

  if ( UnitInterface::syncRemoteUnits( &send_ret_val, &percent_complete ) == false )
   {
    if( send_ret_val != _network_ok )
     {
      resetConnectFsm();
      return( true );      
     }

	state_mesg.message_enum = _connect_state_message_sync_units_percent;
    state_mesg.percent_complete = percent_complete;
    send_ret_val = SERVER->sendMessage( &state_mesg, sizeof(ConnectProcessStateMessage), connect_player_id, 0 );			

    if( send_ret_val != _network_ok )
     {
      resetConnectFsm();
      return( true );      
     }
    
    UnitSyncIntegrityCheck unit_integrity_check_mesg;
    send_ret_val = SERVER->sendMessage( &unit_integrity_check_mesg, sizeof(UnitSyncIntegrityCheck), connect_player_id, 0 );			

    if( send_ret_val != _network_ok )
     {
      resetConnectFsm();
      return( true );      
     }

    
    ObjectiveInterface::syncObjectives( connect_player_id );
          
    PowerUpInterface::syncPowerUps( connect_player_id );

    GameManager::spawnPlayer( connect_player_id );

    connect_player_state->setStatus( _player_state_active );

    PlayerStateSync player_state_update;

    memmove( &player_state_update.player_state,
             connect_player_state,
             sizeof( PlayerState )
           );
          
    SERVER->sendMessage( &player_state_update, sizeof(PlayerStateSync), 0 );
          
	state_mesg.message_enum = _connect_state_sync_complete;
    send_ret_val = SERVER->sendMessage( &state_mesg, sizeof(ConnectProcessStateMessage), connect_player_id, 0 );			

    if( send_ret_val != _network_ok )
     {
      resetConnectFsm();
      return( true );      
     }
      		  

    sendConnectionAlert( connect_player_id, _connect_alert_mesg_connect );

    connection_state = _connect_state_idle;
    return( true );
   }
  else 
   if ( percent_complete > 0 )
    {
 	 state_mesg.message_enum = _connect_state_message_sync_units_percent;
     state_mesg.percent_complete = percent_complete;
     send_ret_val = SERVER->sendMessage( &state_mesg, sizeof(ConnectProcessStateMessage), connect_player_id, 0 );			

     if( send_ret_val != _network_ok )
      {
       resetConnectFsm();
       return( true );      
      }

    }
   
   return( true );
 }

void ServerConnectDaemon::connectFsm( NetMessage *message )
 {
  bool end_cycle = false;
  
  do
   {
    switch ( connection_state )
     {
      case _connect_state_idle :
	   {
        if ( connectStateIdle() )		
		 { end_cycle = true; }
        else
         { end_cycle = false; }
	   
       } break;
               
      case _connect_state_wait_for_connect_request : 
       {
        if ( connectStateWaitForConnectRequest( message ) )		
		 { end_cycle = true; }
        else
         { end_cycle = false; }
       } break;

      
      case _connect_state_attempt_player_alloc :
       {
        if ( connectStateAttemptPlayerAlloc( ) )		
		 { end_cycle = true; }
        else
         { end_cycle = false; }     
       } break; 


      case  _connect_state_wait_for_client_settings :
       {
        if ( connectStateWaitForClientSettings( message ) )		
		 { end_cycle = true; }
        else
         { end_cycle = false; }
       } break;
       
      case _connect_state_wait_for_client_game_setup_ack :
	   {
        if ( connectStateWaitForClientGameSetupAck( message ) )		
		 { end_cycle = true; }
        else
         { end_cycle = false; }
	   }
	  break;

      case _connect_state_player_state_sync :
       { 
        if ( connectStatePlayerStateSync( ) )		
		 { end_cycle = true; }
        else
         { end_cycle = false; }     
       } break;
     
      case _connect_state_unit_sync :
	   {
        if ( connectStateUnitSync( ) )		
		 { end_cycle = true; }
        else
         { end_cycle = false; }     
	   } break;

	 } // ** switch
   
   } while( end_cycle == false );


 } // ** connectFsm( NetMessage *message )
 

void ServerConnectDaemon::connectProcess( NetMessage *message)
 {
  connectFsm( message );
 }

void ServerConnectDaemon::connectProcess( void )
 {
  connectFsm( 0 );
 }

bool ServerConnectDaemon::disconnectClient( PlayerID player_id )
 {
  SERVER->shutdownClientTransport( player_id );

  if ( SERVER->removeClientFromSendList( player_id ) == true )
   { 
    ObjectiveInterface::disownPlayerObjectives( player_id );
    UnitInterface::destroyPlayerUnits( player_id );
    PlayerInterface::disconnectPlayerCleanup( player_id );  
   
    return( true );
   }
 
  return( false );
 }


void ServerConnectDaemon::startDisconnectionProcess( PlayerID player_id )
 {
  if ( disconnectClient( player_id ) == true )
  {
   sendConnectionAlert( player_id, _connect_alert_mesg_disconnect ); 
  }
 }

void ServerConnectDaemon::startClientDropProcess( PlayerID player_id )
 {
  if ( disconnectClient( player_id ) == true )
   {
    sendConnectionAlert( player_id, _connect_alert_mesg_client_drop ); 
   }
 }

void ServerConnectDaemon::lockConnectProcess( void )
 {
  connection_lock_state = true;
 }

void ServerConnectDaemon::unlockConnectProcess( void )
 {
  connection_lock_state = false;
 }

bool ServerConnectDaemon::getConnectLockStatus( void )       
 {
  return( connection_lock_state );
 }

bool ServerConnectDaemon::isConnecting( void )
 {
  if( connection_state == _connect_state_idle )
   { return ( false ); }
  
  return( true );
 }
