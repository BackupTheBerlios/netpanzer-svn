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
#include "PowerUpInterface.hpp"

#include <stdlib.h>


#include "GameConfig.hpp"

#include "MapInterface.hpp"
#include "PlayerInterface.hpp"

#include "BonusUnitPowerUp.hpp"
#include "UnitPowerUp.hpp"
#include "EnemyRadarPowerUp.hpp"

#include "NetworkState.hpp"
#include "Server.hpp"
#include "PowerUpNetMessage.hpp"

//#include "lua.h"
//#include "lualib.h"

PowerUpList PowerUpInterface::powerup_list;

int PowerUpInterface::power_up_limit;
int PowerUpInterface::power_up_regen_time_upper_bound = 300;
int PowerUpInterface::power_up_regen_time_lower_bound = 60;
   
Timer PowerUpInterface::regen_timer;

enum { _powerup_bonus_units,
       _powerup_unit,
       _powerup_enemy_radar,
     };

int  powerup_probability_table[3] = { _powerup_unit, 
                                      _powerup_bonus_units,
                                      _powerup_enemy_radar 
                                    };

PowerUpList::PowerUpList( void )
  : LinkListSingleTemplate< PowerUp >()
 {
  id_counter = 0;
  item_count = 0;
 }

PowerUpList::~PowerUpList( )
 {
  
 }

void PowerUpList::add( PowerUp *powerup )
 {
  powerup->powerup_state.ID = id_counter;

  id_counter++;

  addRear( powerup );

  item_count++;
 }
   
void PowerUpList::insert( PowerUp *powerup )
 {
  addRear( powerup );

  item_count++;
 }

PowerUp * PowerUpList::get( int ID )
 {
  PowerUp *powerup_ptr = 0;

  powerup_ptr = front;
  
  while( powerup_ptr != 0 )
   {
    if ( powerup_ptr->powerup_state.ID == ID ) 
     {
      return( powerup_ptr ); 
     }
    
    powerup_ptr = powerup_ptr->next;
   }

  return( 0 );
 }
    
PowerUp * PowerUpList::remove( int ID )
 {
  PowerUp *powerup_ptr;

  if ( front == 0 )
   return( 0 );

  powerup_ptr = front;

  if ( powerup_ptr->powerup_state.ID == ID )
   {
    item_count--;
    return( removeFront() );
   }
  
  while( powerup_ptr->next != 0 )
   {
    if ( powerup_ptr->next->powerup_state.ID == ID ) 
     {
      item_count--;
      return( removeAfter( powerup_ptr ) );      
     }
    
    powerup_ptr = powerup_ptr->next;
   }

  return( 0 );
 }

void PowerUpList::reset( void )
 {
  deallocate();
  item_count = 0;
  id_counter = 0;
 }

void PowerUpInterface::setPowerUpLimits( unsigned long map_size_x, unsigned long map_size_y )
 {
  int active_players;
  //lua_Object spawn_bound_upper, spawn_bound_lower;
  //lua_Object powerup_limit;

  active_players = PlayerInterface::getActivePlayerCount();

  //lua_pushnumber( (double) map_size_x );
  //lua_pushnumber( (double) map_size_y );
  //lua_pushnumber( (double) active_players );
  //lua_pushnumber( (double) power_up_regen_time_upper_bound );
  //lua_pushnumber( (double) power_up_regen_time_lower_bound ); 
  //lua_callfunction( lua_getglobal("PowerUpSetLimits") ); 
  //powerup_limit = lua_getresult(1);
  //spawn_bound_upper  = lua_getresult(2);
  //spawn_bound_lower  = lua_getresult(3);
  
  int player_factor = (( 0.10 ) * active_players);
  int power_up_limit = ( ( 0.0000625 ) * ( (map_size_x * map_size_y) ) );
  power_up_limit = power_up_limit + (power_up_limit * player_factor); 
 
  PowerUpInterface::power_up_limit = power_up_limit;
  power_up_regen_time_upper_bound =  300;
  power_up_regen_time_lower_bound =  60;
 }

void PowerUpInterface::generatePowerUp( void )
 {
  unsigned long map_size_x, map_size_y;
  PowerUp *power_up;
  float next_regen_interval;
  PowerUpCreateMesg create_mesg;
  PointXYi loc;

  if( (powerup_list.getItemCount() < power_up_limit) )     
   {
    map_size_x = MapInterface::getMapXsize();
    map_size_y = MapInterface::getMapYsize();   
           
    do 
     {
      loc.x = rand() % map_size_x;
      loc.y = rand() % map_size_y;
     }
    while( MapInterface::getMovementValue( loc ) == 0xFF );

    int prob_table_index;
    int powerup_type;

    prob_table_index = rand() % 3;
    powerup_type = powerup_probability_table[ prob_table_index ];

    
    switch( powerup_type )
     {
      case _powerup_bonus_units :
       power_up = new BonusUnitPowerUp( loc, powerup_type );
      break;
  
      case _powerup_unit :
       power_up = new UnitPowerUp( loc, powerup_type );
      break;

      case _powerup_enemy_radar :
       power_up = new EnemyRadarPowerUp( loc, powerup_type );      
      break; 
     }
    
    //power_up = new BonusUnitPowerUp( loc, _powerup_bonus_units);
    //power_up = new UnitPowerUp( loc, _powerup_unit );
    //power_up = new EnemyRadarPowerUp( loc, _powerup_enemy_radar );      
    
    powerup_list.add( power_up );
   
    create_mesg.set( power_up->powerup_state.map_loc,
                     power_up->powerup_state.ID,
                     power_up->powerup_state.type
                   );
    
    SERVER->sendMessage( &create_mesg, sizeof( PowerUpCreateMesg ), 0 );

    do 
     {
      next_regen_interval = rand() % (power_up_regen_time_upper_bound + 1);
     }
    while( next_regen_interval < power_up_regen_time_lower_bound );
   
    regen_timer.changePeriod( next_regen_interval );
   
    setPowerUpLimits( map_size_x, map_size_y );
   }
   
 } 

void PowerUpInterface::initialize( void )
 {
  int lua_return;
  BONUS_POWERUP_ANIM.load( "PowerUps\\Bolt.pak" );
  BONUS_POWERUP_ANIM.setFPS( 15 );
  BONUS_POWERUP_ANIM_SHADOW.load( "PowerUps\\BoltS.pak" );  

  UNIT_POWERUP_ANIM.setTo( BONUS_POWERUP_ANIM );
  UNIT_POWERUP_ANIM_SHADOW.setTo( BONUS_POWERUP_ANIM_SHADOW );
  
  ENEMY_RADAR_POWERUP_ANIM.setTo( BONUS_POWERUP_ANIM );
  ENEMY_RADAR_POWERUP_ANIM_SHADOW.setTo( BONUS_POWERUP_ANIM_SHADOW );

  srand( (unsigned)time( 0 ) );
  /*
  mathlib_open();
  lua_return = lua_dofile( "PowerUps\\UnitPowerUp.lua" ); 
  assert( lua_return == 0 );
  lua_return = lua_dofile( "PowerUps\\PowerUp.lua" ); 
  assert( lua_return == 0 );
  */
  //resetLogic();
 }
 
void PowerUpInterface::resetLogic( void )
 {
  unsigned long map_size_x, map_size_y;
  
  if( GameConfig::getPowerUpState() == false )
   { return; }

  powerup_list.reset();
  
  map_size_x = MapInterface::getMapXsize();
  map_size_y = MapInterface::getMapYsize();   
  
  setPowerUpLimits( map_size_x, map_size_y );

  regen_timer.changePeriod( power_up_regen_time_upper_bound );
 
  if ( NetworkState::status == _network_state_server )
   {
    generatePowerUp();
    generatePowerUp(); 
    generatePowerUp(); 
   }
 }

void PowerUpInterface::updateState( void )
 {
  PowerUp *powerup_ptr = 0;
  
  if( GameConfig::getPowerUpState() == false )
   { return; }

  
  if ( NetworkState::status == _network_state_server )
   {
    if( regen_timer.count() )
     {
      generatePowerUp();
     }
   }

  powerup_list.resetIterator( &powerup_ptr );

  while( powerup_ptr != 0 )
   {
    if( powerup_ptr->powerup_state.life_cycle_state == _power_up_lifecycle_state_inactive )
     {
      int delete_id;
      PowerUp *delete_ptr;
      
      delete_id = powerup_ptr->powerup_state.ID;
       
      powerup_list.incIteratorPtr( &powerup_ptr );

      delete_ptr = powerup_list.remove( delete_id );
      
      if ( delete_ptr != 0 )
       { delete( delete_ptr ); }
     }
    else
     {
      powerup_ptr->updateState();
      powerup_list.incIteratorPtr( &powerup_ptr );
     }
   
   }
 
 }

void PowerUpInterface::offloadGraphics( SpriteSorter &sorter )
 {
  PowerUp *powerup_ptr;
  
  if( GameConfig::getPowerUpState() == false )
   { return; }

  powerup_list.resetIterator( &powerup_ptr );

  while( powerup_ptr != 0 )
   {
    powerup_ptr->offloadGraphics( sorter );
       
    powerup_list.incIteratorPtr( &powerup_ptr );
   }  
 }

void PowerUpInterface::netMessagePowerUpCreate( NetMessage *message )
 {
  PowerUp *power_up;
  PowerUpCreateMesg *create_mesg;

  create_mesg = (PowerUpCreateMesg *) message;


  switch( create_mesg->type )
   {
    case _powerup_bonus_units :
     power_up = new BonusUnitPowerUp( create_mesg->map_loc, create_mesg->type  );
    break;
  
    case _powerup_unit :
     power_up = new UnitPowerUp( create_mesg->map_loc, create_mesg->type );
    break;

    case _powerup_enemy_radar :
     power_up = new EnemyRadarPowerUp( create_mesg->map_loc, create_mesg->type );      
    break; 
  }
  
  power_up->powerup_state.ID = create_mesg->ID;

  powerup_list.insert( power_up );
 }

void PowerUpInterface::netMessagePowerUpHit( NetMessage *message )
 {
  PowerUp *power_up;
  PowerUpHitMesg *hit_mesg;

  hit_mesg = (PowerUpHitMesg *) message;

  power_up = powerup_list.get( hit_mesg->ID );
  
  if (power_up != 0)
   {
    power_up->onHit( hit_mesg );
   }
 }
 
void PowerUpInterface::processNetMessages( NetMessage *message )
 {
  switch( message->message_id )
   {
    case _net_message_id_powerup_create :
     {
      netMessagePowerUpCreate( message );
     }
    break; 

    case _net_message_id_powerup_hit :
     {
      netMessagePowerUpHit( message );
     }
    break; 

   } // ** switch 
 }

void PowerUpInterface::syncPowerUps( PlayerID player_id )
 {
  PowerUpCreateMesg create_mesg;
  PowerUp *powerup_ptr;
  
  powerup_list.resetIterator( &powerup_ptr );

  while( powerup_ptr != 0 )
   {
    create_mesg.set( powerup_ptr->powerup_state.map_loc,
                     powerup_ptr->powerup_state.ID,
                     powerup_ptr->powerup_state.type
                   );
    
    SERVER->sendMessage( &create_mesg, sizeof( PowerUpCreateMesg ), player_id, 0 );
       
    powerup_list.incIteratorPtr( &powerup_ptr );
   }  
 }
  
