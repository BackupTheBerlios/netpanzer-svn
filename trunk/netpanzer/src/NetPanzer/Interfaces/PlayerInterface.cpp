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
#include "PlayerInterface.hpp"

#include "UnitInterface.hpp"
#include "ObjectiveInterface.hpp"

#include "PlayerNetMessage.hpp"
#include "Server.hpp"

#include "ConsoleInterface.hpp"

//#include "WorldViewInterface.hpp"

// ** PlayerInterface Statics
PlayerState PlayerInterface::local_player_state;

PlayerState   *PlayerInterface::player_lists;
unsigned short PlayerInterface::max_players = 0;
boolean *      PlayerInterface::alliance_matrix;
unsigned short PlayerInterface::local_player_index = 0xFFFF;

NetMessageEncoder PlayerInterface::message_encoder;
unsigned short PlayerInterface::player_sync_index;
unsigned short PlayerInterface::player_sync_connect_player_index;
PlayerID PlayerInterface::player_sync_connect_id;
Timer PlayerInterface::player_sync_timer;

unsigned short PlayerInterface::respawn_rule_player_index = 0;

void PlayerInterface::initialize( unsigned short maxPlayers, unsigned char max_spawn_units )
 {  
  char temp_str[64];
  PlayerID player_id;
  unsigned long player_index;
  max_players = maxPlayers;

  player_lists = new PlayerState [ max_players ];
  assert( player_lists != NULL );

  for ( player_index = 0; player_index < max_players; player_index++ )
   {
    player_id.setIndex( player_index );
    player_lists[player_index].setID( player_id );
    player_lists[ player_index ].resetStats();
    player_lists[ player_index ].setStatus( _player_state_free );     
    player_lists[ player_index ].setFlag( 0 );
    player_lists[ player_index ].unit_config.initialize( max_spawn_units );   
    sprintf( temp_str, "Player %d", player_index );
    player_lists[ player_index ].setName( temp_str );
   }
 
  local_player_state.setID( PlayerID( 0xFFFF, 0xFFFFFFFF ) );
  local_player_state.setStatus( _player_state_allocated );

  alliance_matrix = new boolean [max_players * max_players];
  assert( alliance_matrix != NULL );
  resetAllianceMatrix();
 }

void PlayerInterface::reset( void )
 {
  resetPlayerStats();
  resetAllianceMatrix();
 }

void PlayerInterface::cleanUp( void )
 {
  delete [ ] player_lists;
  delete( alliance_matrix );
  max_players = 0;
 }

void PlayerInterface::setKill( unsigned short by_player_index, unsigned short on_player_index, unsigned short unit_type)
 {
  assert( (by_player_index < max_players) && (on_player_index < max_players) );
  
  player_lists[ by_player_index ].incKills( unit_type );
  player_lists[ on_player_index ].incLosses( unit_type );   
 }

void PlayerInterface::setKill( PlayerID &by_player, PlayerID &on_player, unsigned short unit_type )
 {
  assert( (by_player.getIndex() < max_players) && (on_player.getIndex() < max_players) );
  
  player_lists[ by_player.getIndex() ].incKills( unit_type );
  player_lists[ on_player.getIndex() ].incLosses( unit_type );   
 }

void PlayerInterface::setKill( UnitID &by_player, UnitID &on_player, unsigned short unit_type )
 {
  unsigned short by_player_index; 
  unsigned short on_player_index;

  by_player_index = by_player.getPlayer();
  on_player_index = on_player.getPlayer();
  
  assert( (by_player_index < max_players) && (on_player_index < max_players) );
  
  player_lists[ by_player_index ].incKills(unit_type);
  player_lists[ on_player_index ].incLosses(unit_type);   
 }


void PlayerInterface::setAlliance( PlayerID by_player, PlayerID with_player )
 {
  assert( (by_player.getIndex() < max_players) && (with_player.getIndex() < max_players) );
  
  *(alliance_matrix + (with_player.getIndex() * max_players) + by_player.getIndex() ) = _TRUE;
  //*(alliance_matrix + (by_player.getIndex() * max_players) + with_player.getIndex() ) = _TRUE;
 }
   

void PlayerInterface::setAlliance( unsigned short by_player, unsigned short with_player )
 {
 assert( (by_player < max_players) && (with_player < max_players) );
  
  *(alliance_matrix + (with_player * max_players) + by_player ) = _TRUE;
  //*(alliance_matrix + (by_player * max_players) + with_player ) = _TRUE;
 }

void PlayerInterface::clearAlliance( unsigned short by_player, unsigned short with_player )
 {
  assert( (by_player < max_players) && (with_player < max_players) );
  
  *(alliance_matrix + (with_player * max_players) + by_player ) = _FALSE; 
 } 

boolean PlayerInterface::isAllied( PlayerID player, PlayerID with_player )
 {
  boolean val;
  
  assert( (player.getIndex() < max_players) && (with_player.getIndex() < max_players) );
  
  val = *(alliance_matrix + (with_player.getIndex() * max_players) + player.getIndex() );
  
  return( val );

 }

boolean PlayerInterface::isAllied( unsigned short player, unsigned short with_player )
 {
  boolean val;
  
  assert( (player < max_players) && (with_player < max_players) );
  
  val = *(alliance_matrix + (with_player * max_players) + player );
  
  return( val );

 }

void PlayerInterface::lockPlayerStats( void )
 {
  unsigned long player_index;

  for ( player_index = 0; player_index < max_players; player_index++ )
   {
    player_lists[ player_index ].lockStats();   
   } // ** for

 }

void PlayerInterface::unlockPlayerStats( void )
 {
  unsigned long player_index;

  for ( player_index = 0; player_index < max_players; player_index++ )
   {
    player_lists[ player_index ].unlockStats();   
   } // ** for
 }


void PlayerInterface::resetPlayerStats( void )
 {
  unsigned long player_index;

  for ( player_index = 0; player_index < max_players; player_index++ )
   {
    player_lists[ player_index ].resetStats();   
   } // ** for
  
 }

void PlayerInterface::resetAllianceMatrix( void )
 {
  unsigned long i;
  unsigned long matrix_size;

  matrix_size = max_players * max_players;

  for ( i = 0; i < matrix_size; i++ )
   {
    alliance_matrix[ i ] = _FALSE;
   }
  
 }

int PlayerInterface::getActivePlayerCount( void )
 {
  unsigned long player_index;
  int count = 0;;

  for ( player_index = 0; player_index < max_players; player_index++ )
   {
    if ( player_lists[ player_index ].getStatus() == _player_state_active )
     { count++; }  
   } // ** for
  
  return( count );
 }

PlayerState * PlayerInterface::allocateLoopBackPlayer( void )
 {
  local_player_index = 0;
  
  player_lists[ local_player_index ].setStatus( _player_state_active );
  
  return( &player_lists[ local_player_index ] );
 }

PlayerState * PlayerInterface::allocateNewPlayer( void )
 {
  unsigned long player_index;

  for ( player_index = 0; player_index < max_players; player_index++ )
   {
    if ( player_lists[ player_index ].getStatus() == _player_state_free )
     {
      player_lists[ player_index ].setStatus( _player_state_allocated );
      player_lists[ player_index ].resetStats();
	  return( &player_lists[ player_index ] ); 
     } // ** if 
   
   } // ** for
  
  return( NULL );
 }

void PlayerInterface::spawnPlayer( unsigned short player_index, PointXYi &location )
 {
  assert( player_index < max_players );

  spawnPlayer( player_lists[ player_index ].getPlayerID(),
               location 
              ); 
 }

void PlayerInterface::spawnPlayer( PlayerID &player, PointXYi &location )
 {
  unsigned short player_index;
  
  player_index = player.getIndex();
  assert( player_index < max_players );

  if ( player_lists[ player_index ].getStatus() != _player_state_free )
   {
    UnitInterface::spawnPlayerUnits( location, 
                                     player, 
                                     player_lists[ player_index ].unit_config 
                                   );  
   
   } // ** if _player_state_active 
  
 }


boolean PlayerInterface::testRuleScoreLimit( long score_limit, PlayerState ** player_state )
 {
  unsigned long player_index;

  for ( player_index = 0; player_index < max_players; player_index++ )
   {
    if ( player_lists[ player_index ].getTotal() >= score_limit )
     {
      *player_state = &player_lists[ player_index ];
      return( _TRUE );
     } // ** if 
   
   } // ** for
  
  return( _FALSE );
 }

boolean PlayerInterface::testRuleObjectiveRatio( float precentage, PlayerState ** player_state )
 {
  unsigned long player_index;

  for ( player_index = 0; player_index < max_players; player_index++ )
   {
    if ( ObjectiveInterface::testRuleObjectiveOccupationRatio( player_index, precentage ) == _TRUE )  
     {
      *player_state = &player_lists[ player_index ];
      return( _TRUE );
     } // ** if 
   
   } // ** for
  
  return( _FALSE );
 }


boolean PlayerInterface::testRulePlayerRespawn( boolean *completed, PlayerState **player_state )
 {
  PlayerID player;

  if ( respawn_rule_player_index == max_players )
   {
    respawn_rule_player_index = 0;
    *completed = _TRUE; 
    return( _FALSE );
   }
  else
   {
    *completed = _FALSE;     
   }
  player = player_lists[ respawn_rule_player_index ].getPlayerID();
  
  if ( player_lists[ respawn_rule_player_index ].getStatus() == _player_state_active )
   if ( UnitInterface::getUnitCount( player.getIndex() ) == 0 )
    {
     *player_state = &player_lists[ respawn_rule_player_index ];
     respawn_rule_player_index++;
     return( _TRUE );
    } 

  respawn_rule_player_index++;  
  return( _FALSE );
 }

void PlayerInterface::startPlayerStateSync( PlayerID &connect_player  )
 {
  player_sync_index = 0;
  player_sync_connect_player_index = connect_player.getIndex(); 
  player_sync_connect_id = connect_player;
  player_sync_timer.changeRate( 4 );
  message_encoder.resetEncoder();
 }

boolean PlayerInterface::syncPlayerState( int *send_return_code, int *percent_complete )
 {
  PlayerStateSync sync_mesg;
  unsigned short player_count = 0;
  MultiMessage *encode_message;
  
  *percent_complete = -1;

  int send_ret_val;
  *send_return_code = _network_ok;
  

  if ( player_sync_timer.count() )
   {
    
    while( (player_sync_index != max_players) && 
           (player_count < 5) 
         )
     {
      if( player_sync_index != player_sync_connect_player_index )
       {
        memmove( &sync_mesg.player_state,
                 &player_lists[ player_sync_index ],
                 sizeof( PlayerState )
               );
       
        while( message_encoder.encodeMessage( &sync_mesg, sizeof(PlayerStateSync), &encode_message ) )
         {
          send_ret_val = SERVER->sendMessage( encode_message, encode_message->realSize(), 
                                              player_sync_connect_id, 0 ); 
          message_encoder.resetEncoder();
          
          if ( send_ret_val != _network_ok )
           {
            *send_return_code = send_ret_val; 
            return( _TRUE ); 
           }
         } // ** if  
       
       } // ** if

      player_sync_index++;
      player_count++;
     
     } // ** while 
    
    message_encoder.getEncodeMessage( &encode_message ); 
    if ( encode_message != NULL )
     {
      send_ret_val = SERVER->sendMessage( encode_message, encode_message->realSize(), player_sync_connect_id, 0 );
      
      if ( send_ret_val != _network_ok )
       { 
        *send_return_code = send_ret_val; 
        return( _TRUE ); 
       }

     } // ** if 

    if( (player_sync_index == max_players) )
     {
      *percent_complete = 100;
      return( _TRUE );
     } // ** if 
        
    float percent;
    percent = ( ( (float) player_sync_index) / ( (float) max_players ) ) * 100;
    *percent_complete = (int) percent;  
   } // ** if 
 
  return( _FALSE );
 
 }


void PlayerInterface::netMessageConnectID( NetMessage *message )
 {
  PlayerConnectID *connect_mesg;
  PlayerID player;

  connect_mesg = (PlayerConnectID *) message;
    
  player = connect_mesg->connect_state.getPlayerID();
  local_player_index = player.getIndex();

  memmove( &player_lists[ local_player_index ], 
           &connect_mesg->connect_state,
           sizeof( PlayerState )
         );
 
 }

void PlayerInterface::netMessageSyncState( NetMessage *message )
 {
  PlayerStateSync *sync_mesg;
  PlayerID player;
  unsigned short player_index;

  sync_mesg = ( PlayerStateSync *) message;
  player = sync_mesg->player_state.getPlayerID();
  player_index = player.getIndex();
  memcpy( &player_lists[ player_index ], 
          &sync_mesg->player_state,
           sizeof( PlayerState )
         ); 
 }

void PlayerInterface::netMessageScoreUpdate( NetMessage *message )
 {
  PlayerScoreUpdate *score_update;
  
  score_update = (PlayerScoreUpdate *) message;

  setKill( score_update->kill_by_player_index, 
           score_update->kill_on_player_index,
           score_update->unit_type );
 }

void PlayerInterface::netMessageAllianceRequest( NetMessage *message )
 {
  PlayerAllianceRequest *allie_request;
  PlayerState *player_state;

  allie_request = (PlayerAllianceRequest *) message;
  
  if ( allie_request->alliance_request_type == _player_make_alliance )
   {
    setAlliance( allie_request->allie_by_player_index, allie_request->allie_with_player_index );

    if( (allie_request->allie_by_player_index == local_player_index) ) 
     {
      player_state = getPlayerState( allie_request->allie_with_player_index );
      ConsoleInterface::postMessage( "Alliance created with %s.", player_state->getName() );
     } 
    else
     if( (allie_request->allie_with_player_index == local_player_index) )    
      {
       player_state = getPlayerState( allie_request->allie_by_player_index );
       ConsoleInterface::postMessage( "%s has allied with you.", player_state->getName() );
      } 
   }
  else
   {
    clearAlliance( allie_request->allie_by_player_index, allie_request->allie_with_player_index );

    if( (allie_request->allie_by_player_index == local_player_index) ) 
     {
      player_state = getPlayerState( allie_request->allie_with_player_index );
      ConsoleInterface::postMessage( "Alliance broken with %s.", player_state->getName() );
     } 
    else
     if( (allie_request->allie_with_player_index == local_player_index) )    
      {
       player_state = getPlayerState( allie_request->allie_by_player_index );
       ConsoleInterface::postMessage( "%s has broken their alliance with you.", player_state->getName() );
      } 

   }
 
  PlayerAllianceUpdate allie_update;

  allie_update.allie_by_player_index   = allie_request->allie_by_player_index;
  allie_update.allie_with_player_index = allie_request->allie_with_player_index;
  allie_update.alliance_update_type = allie_request->alliance_request_type;

  SERVER->sendMessage( &allie_update, sizeof( PlayerAllianceUpdate ), 0 );
 }

void PlayerInterface::netMessageAllianceUpdate( NetMessage *message )
 {
  PlayerAllianceUpdate *allie_update;
  PlayerState *player_state;

  allie_update = (PlayerAllianceUpdate *) message;
  
  if ( allie_update->alliance_update_type == _player_make_alliance )
   {
    setAlliance( allie_update->allie_by_player_index, allie_update->allie_with_player_index );
   
    if( (allie_update->allie_by_player_index == local_player_index) ) 
     {
      player_state = getPlayerState( allie_update->allie_with_player_index );
      ConsoleInterface::postMessage( "Alliance created with %s.", player_state->getName() );
     } 
    else
     if( (allie_update->allie_with_player_index == local_player_index) )    
      {
       player_state = getPlayerState( allie_update->allie_by_player_index );
       ConsoleInterface::postMessage( "%s has allied with you.", player_state->getName() );
      } 
   }
  else
   {
    clearAlliance( allie_update->allie_by_player_index, allie_update->allie_with_player_index );

    if( (allie_update->allie_by_player_index == local_player_index) ) 
     {
      player_state = getPlayerState( allie_update->allie_with_player_index );
      ConsoleInterface::postMessage( "Alliance broken with %s.", player_state->getName() );
     } 
    else
     if( (allie_update->allie_with_player_index == local_player_index) )    
      {
       player_state = getPlayerState( allie_update->allie_by_player_index );
       ConsoleInterface::postMessage( "%s has broken their alliance with you.", player_state->getName() );
      } 
   }
 }


void PlayerInterface::processNetMessage( NetMessage *message )
 {
  switch( message->message_id )
   {
    case _net_message_id_player_connect_id :
     netMessageConnectID( message );
    break;
     
    case _net_message_id_player_sync_state :
     netMessageSyncState( message );
    break;
   
    case _net_message_id_player_score_update :
     netMessageScoreUpdate( message );
    break;
   
    case _net_message_id_player_alliance_request :
     netMessageAllianceRequest( message );
    break;
   
    case _net_message_id_player_alliance_update :
     netMessageAllianceUpdate( message );
    break;
   
   } // ** switch
 }

void PlayerInterface::disconnectPlayerCleanup( PlayerID &player_id )
 {
  PlayerAllianceUpdate allie_update;

  unsigned short player_index;
  unsigned short disconnect_player_index;

  disconnect_player_index = player_id.getIndex();

  for ( player_index = 0; player_index < max_players; player_index++ )
   {
	if ( isAllied( disconnect_player_index, player_index ) == _TRUE )
	 {
	  allie_update.set( disconnect_player_index, player_index, _player_break_alliance ); 
      SERVER->sendMessage( &allie_update, sizeof( PlayerAllianceUpdate ), 0 );
	 } 
    
    if ( isAllied( player_index, disconnect_player_index ) == _TRUE )
	 {
	  allie_update.set( player_index, disconnect_player_index, _player_break_alliance ); 
      SERVER->sendMessage( &allie_update, sizeof( PlayerAllianceUpdate ), 0 );
	 } 
     
   } // ** for
  
  player_lists[ disconnect_player_index ].setStatus( _player_state_free );       

  PlayerStateSync player_state_update;
  PlayerState *player_state;

  player_state = getPlayerState( player_id );
  memmove( &player_state_update.player_state,
            player_state,
            sizeof( PlayerState )
         );

  SERVER->sendMessage( &player_state_update, sizeof( PlayerStateSync ), 0 );

 }
