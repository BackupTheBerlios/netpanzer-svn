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
#include "UnitInterface.hpp"
#include "PlayerInterface.hpp"
#include "MapInterface.hpp"
#include "WorldViewInterface.hpp"
#include "ConsoleInterface.hpp"

#include "Vehicle.hpp"
#include "Abrams.hpp"
#include "Valentine.hpp"
#include "Leopard.hpp"
#include "Hammerhead.hpp"
#include "Humvee.hpp"
#include "Lynx.hpp"
#include "Scorpion.hpp"
#include "SpahPanzer.hpp"
#include "M109.hpp"
#include "Archer.hpp"

#include "Timer.hpp"
#include "Server.hpp"
#include "NetworkState.hpp"
#include "NetMessageEncoder.hpp"

#include "UnitMessageTypes.hpp"
#include "PlayerNetMessage.hpp"

#include "Sound.hpp"
#include "ParticleInterface.hpp"


UnitList * UnitInterface::unit_lists;
UnitBucketArray UnitInterface::unit_bucket_array;

unsigned short UnitInterface::max_players;
PlacementMatrix UnitInterface::unit_placement_matrix;

unsigned short UnitInterface::unique_generator;

UnitOpcodeEncoder UnitInterface::opcode_encoder;
UnitOpcodeEncoder UnitInterface::no_guarantee_opcode_encoder;

Timer UnitInterface::message_timer;
Timer UnitInterface::no_guarantee_message_timer;


UnitPointer *UnitInterface::unit_position_enum_iterator;
unsigned short UnitInterface::unit_position_enum_list_index;
unsigned short UnitInterface::unit_position_enum_local_player_index;

unsigned short UnitInterface::unit_cycle_player_index;
unsigned long  UnitInterface::unit_cycle_iterator;

unsigned long  UnitInterface::sync_units_iterator;
bool	       UnitInterface::sync_units_complete_flag;
unsigned short UnitInterface::sync_units_list_index;
unsigned long  UnitInterface::sync_units_total_units;
unsigned long  UnitInterface::sync_units_in_sync_count;
unsigned long  UnitInterface::sync_units_in_sync_partial_count;
Timer		   UnitInterface::sync_units_packet_timer;
PlayerID	   UnitInterface::sync_units_remote_player;


// ******************************************************************

void UnitInterface::initialize( unsigned long max_units )
{
    unsigned int i;

    max_players = PlayerInterface::getMaxPlayers();

    unit_lists = new UnitList [ max_players ];

    for( i = 0; i < max_players; i++ ) {
        unit_lists[i].initialize( max_units );
    }

    unit_bucket_array.initialize( MapInterface::getMapSize(), TileInterface::getTileSize() );

    unique_generator = 0;
    message_timer.changeRate( 8 );
    no_guarantee_message_timer.changeRate( 15 );
    opcode_encoder.initialize( _opcode_encoder_send_method_guarantee );
    no_guarantee_opcode_encoder.initialize( _opcode_encoder_send_method_no_guarantee );
}

// ******************************************************************

void UnitInterface::cleanUp( void )
{
    unsigned int i;

    for( i = 0; i < max_players; i++ ) {
        unit_lists[i].cleanUp();
    }

    delete[] unit_lists;
}

void UnitInterface::reset( void )
{
    unsigned int i;

    for( i = 0; i < max_players; i++ ) {
        unit_lists[i].reset();
    }

    unit_bucket_array.initialize( MapInterface::getMapSize(), TileInterface::getTileSize() );
}

// ******************************************************************
unsigned long UnitInterface::getTotalUnitCount( void )
{
    unsigned long unit_total = 0;

    for( int i = 0; i < max_players; i++ ) {
        unit_total += unit_lists[ i ].containsItems();
    }

    return( unit_total );
}


void UnitInterface::sendMessage( UnitMessage *message )
{
    UnitBase *unit;
    // ** NOTE: INCOMPLETE

    if ( message->isFlagged( _umesg_flag_unique ) ) {
        unit = getUnit( message->unit_id );

        if ( unit != 0 ) {
            unit->processMessage( message );
        }
    } // ** if _umesg_flag_unique
    else
        if (  message->isFlagged( _umesg_flag_broadcast ) ) {
            UnitPointer *iterator;
            unsigned long list_index;

            for( list_index = 0; list_index < max_players; list_index++) {
                unit_lists[ list_index ].resetIterator( &iterator );

                unit = unit_lists[ list_index ].incIterator( &iterator );
                while( unit != 0 ) {
                    unit->processMessage( message );

                    unit = unit_lists[ list_index ].incIterator( &iterator );
                } // ** while

            } // ** for


        } // ** if _mesg_flag_broadcast
        else
            if ( message->isFlagged( _umesg_flag_manager_request ) ) {
                processManagerMessage( message );
            } // ** if _umesg_flag_manager_request
}

// ******************************************************************

void UnitInterface::updateUnitStatus( void )
{
    UnitBase *unit;
    UnitPointer *iterator;
    unsigned long list_index;

    for( list_index = 0; list_index < max_players; list_index++) {
        unit_lists[ list_index ].resetIterator( &iterator );

        unit = unit_lists[ list_index ].incIterator( &iterator );
        while( unit != 0 ) {
            if ( unit->unit_state.lifecycle_state == _UNIT_LIFECYCLE_INACTIVE ) {
                // Particle Shit Hack
                ParticleInterface::addHit(unit->unit_state);

                //SFX
                sound->playAmbientSound("expl",
                                        WorldViewInterface::getCameraDistance( unit->unit_state.location ) );

                deleteUnit( unit );
            } else {
                unsigned long pre_update_bucket_index;
                unsigned long post_update_bucket_index;

                pre_update_bucket_index = unit_bucket_array.worldLocToBucketIndex( unit->unit_state.location );

                unit->updateState();

                post_update_bucket_index = unit_bucket_array.worldLocToBucketIndex( unit->unit_state.location );

                if ( post_update_bucket_index != pre_update_bucket_index ) {
                    //ConsoleInterface::postMessage( "Moving Unit from bucket %d to bucket %d",
                    //                               pre_update_bucket_index, post_update_bucket_index );

                    unit_bucket_array.moveUnit( unit->unit_id, pre_update_bucket_index, post_update_bucket_index );
                }
            }

            unit = unit_lists[ list_index ].incIterator( &iterator );
        } // ** while

    } // ** for

    if ( NetworkState::status == _network_state_server ) {
        if ( message_timer.count() ) {
            opcode_encoder.sendOpcodeMessage();
        }

        if ( no_guarantee_message_timer.count() ) {
            no_guarantee_opcode_encoder.sendOpcodeMessage();
        }
    }

}

void UnitInterface::offloadGraphics( SpriteSorter &sorter )
{
    iRect world_window_rect;
    iRect bucket_rect;
    UnitBucketList *bucket_list;
    UnitBucketPointer *traversal_ptr;

    world_window_rect = sorter.getWorldWindow();
    bucket_rect = unit_bucket_array.worldRectToBucketRectClip( world_window_rect );

    for( long row_index = bucket_rect.min.y; row_index <= bucket_rect.max.y; row_index++ ) {
        for( long column_index = bucket_rect.min.x; column_index <= bucket_rect.max.x; column_index++ ) {
            bucket_list = unit_bucket_array.getBucket( row_index, column_index );

            traversal_ptr = bucket_list->getFront();

            while( traversal_ptr != 0 ) {
                traversal_ptr->unit->offloadGraphics( sorter );
                traversal_ptr = traversal_ptr->next;
            } // ** while
        } // ** for
    } // ** for

}

// ******************************************************************

bool UnitInterface::isUniqueIndex( unsigned short new_index )
{
    unsigned long list_index;

    for( list_index = 0; list_index < max_players; list_index++) {
        if ( unit_lists[list_index].isValid( new_index ) == true )
            return( false );
    }

    return( true );
}

// ******************************************************************

unsigned short UnitInterface::uniqueIndex( void )
{
    unsigned short new_index;

    do {
        new_index = unique_generator++;
    } while( isUniqueIndex( new_index ) == false );

    return( new_index );
}

// ******************************************************************


UnitBase * UnitInterface::newUnit( unsigned short unit_type,
                                   const iXY &location,
                                   unsigned short player_index )
{
    UnitBase *unit = 0;
    PlayerState *player_state;
    bool color_flag;
    unsigned char unit_flag;

    if ( player_index == PlayerInterface::getLocalPlayerIndex() ) {
        color_flag = true;
    } else {
        color_flag = false;
    }

    player_state = PlayerInterface::getPlayerState( player_index );
    unit_flag = player_state->getFlag();

    switch( unit_type ) {
    case _unit_type_valentine : {
            unit = new Valentine( location, color_flag, unit_flag );
        }
        break;

    case _unit_type_leopard : {
            unit = new Leopard( location, color_flag, unit_flag );
        }
        break;

    case _unit_type_abrams : {
            unit = new Abrams( location, color_flag, unit_flag );
        }
        break;

    case _unit_type_hammerhead : {
            unit = new Hammerhead( location, color_flag, unit_flag );
        }
        break;

    case _unit_type_humvee : {
            unit = new Humvee( location, color_flag, unit_flag );
        }
        break;

    case _unit_type_lynx : {
            unit = new Lynx( location, color_flag, unit_flag );
        }
        break;

    case _unit_type_m109 : {
            unit = new M109( location, color_flag, unit_flag );
        }
        break;

    case _unit_type_spahpanzer : {
            unit = new SpahPanzer( location, color_flag, unit_flag );
        }
        break;

    case _unit_type_scorpion : {
            unit = new Scorpion( location, color_flag, unit_flag );
        }
        break;

    case _unit_type_archer : {
            unit = new Archer ( location, color_flag, unit_flag );
        }
        break;


    } // ** switch

    return( unit );
}

// ******************************************************************

void UnitInterface::addNewUnit( UnitBase *unit, const PlayerID &player )
{
    unsigned short new_index;
    unsigned short player_index;

    player_index = player.getIndex();

    new_index = uniqueIndex( );

    unit_lists[ player_index ].add( unit, new_index );

    unit->unit_id.index = new_index;
    unit->unit_id.player = player_index;
    unit->unit_id.unique = 0;

    unit_bucket_array.addUnit( unit );
}
// ******************************************************************
void UnitInterface::deleteUnit( UnitID unit_id )
{
    UnitBase *unit = 0;

    unit = getUnit( unit_id );

    if( unit == 0 )
        return;

    deleteUnit( unit );
}

// ******************************************************************
void UnitInterface::deleteUnit( UnitBase *unit )
{
    unit_bucket_array.deleteUnitBucketPointer( unit->unit_id, unit->unit_state.location );
    unit_lists[ unit->unit_id.getPlayer() ].deleteUnit( unit->unit_id );
}


void UnitInterface::sortBucketArray( void )
{
    unsigned long bucket_count;
    unsigned long bucket_index;
    UnitBucketList *bucket_list;
    UnitBucketPointer *traversal_ptr;

    bucket_count = unit_bucket_array.getSize();

    for( bucket_index = 0; bucket_index < bucket_count; bucket_index++ ) {
        bucket_list = unit_bucket_array.getBucket( bucket_index );

        traversal_ptr = bucket_list->getFront();

        while( traversal_ptr != 0 ) {
            unsigned long unit_bucket_index;

            unit_bucket_index = unit_bucket_array.worldLocToBucketIndex( traversal_ptr->unit->unit_state.location );

            if( unit_bucket_index != bucket_index ) {
                unit_bucket_array.moveUnit( traversal_ptr->unit->unit_id, bucket_index, unit_bucket_index );
            }

            traversal_ptr = traversal_ptr->next;
        } // ** while
    }
}

// ******************************************************************
UnitBase * UnitInterface::createUnit( unsigned short unit_type,
                                      const iXY &location,
                                      const PlayerID &player
                                    )
{
    UnitBase *unit;
    unsigned short player_index;

    player_index = player.getIndex();

    if ( unit_lists[ player_index ].containsItems() == unit_lists[ player_index ].getSize() )
        return( 0 );

    unit = newUnit( unit_type, location, player_index );

    addNewUnit( unit, player );

    return( unit );
}

// ******************************************************************

void UnitInterface::spawnPlayerUnits( const iXY &location,
                                      const PlayerID &player, const PlayerUnitConfig &unit_config )
{
    iXY next_loc;
    UnitBase *unit;
    UnitRemoteCreate create_mesg;
    unsigned long unit_type_index;
    unsigned long unit_spawn_count;
    unsigned long unit_spawn_index;


    // 1000 UNIT HACK
    // REMEMEBER THAT YOU MUST GUARANTEE THAT, "X" UNIT ARE ALLOWED VIA MENU
    /*
      unsigned long unit_count = 0;
      long map_x_size = MapInterface::getMapXsize();
      long map_y_size = MapInterface::getMapYsize(); 
     
      for( long map_y = 0; map_y < map_y_size; map_y++ )
       {
        for( long map_x = 0; map_x < map_x_size; map_x++ )
       	 {
    	  if ( MapInterface::getMovementValue( iXY( map_x, map_y ) ) != 0xFF )
    	   {
    		if( unit_count < 1000 )	//  <- CHANGE YOUR UNIT COUNT HERE
    		 {
              unit = createUnit( _unit_type_humvee, iXY( map_x, map_y ), player );    
    		  unit_count++;
    		 }
    	   }
    	 }   
       }  
    */
    // THE REAL SPAWN PLAYER UNITS FUNCTION

    PUBLIC_MESSAGE_ENCODER.resetEncoder();

    unit_placement_matrix.reset( location );

    for ( unit_type_index = 0; unit_type_index < _MAX_UNIT_TYPES; unit_type_index++ ) {

        unit_spawn_count = unit_config.getSpawnUnitCount( unit_type_index );

        for ( unit_spawn_index = 0; unit_spawn_index < unit_spawn_count; unit_spawn_index++ ) {
            unit_placement_matrix.getNextEmptyLoc( &next_loc );
            unit = createUnit( unit_type_index, next_loc, player );

            assert(unit != 0);
            create_mesg.new_unit_id = unit->unit_id;
            create_mesg.location = next_loc;
            create_mesg.unit_type = unit->unit_state.unit_type;
            PUBLIC_MESSAGE_ENCODER.encodeMessage( &create_mesg,
                                                  sizeof( create_mesg )
                                                );

        } // ** for unit_spawn_index

    } // ** for unit_type_index

    PUBLIC_MESSAGE_ENCODER.sendEncodedMessage();

}

// ******************************************************************

bool UnitInterface::
quearyUnitsKeySearch( UnitIDList *working_list,
                      int (* key_func )( void *key, UnitState *comp ),
                      void *key, PlayerID player_id,
                      unsigned char search_flags,
                      bool find_first  )
{
    UnitPointer *unit_ptr;
    UnitPointer *iterator;
    unsigned long list_index;
    unsigned long work_index;

    work_index = working_list->containsItems();

    for( list_index = 0; list_index < max_players; list_index++) {
        if (   ( (search_flags == _search_exclude_player) && (player_id.getIndex() != list_index) )
                || ( (search_flags == _search_player) && (player_id.getIndex() == list_index ) )
           ) {

            unit_lists[ list_index ].resetIterator( &iterator );

            unit_ptr = unit_lists[ list_index ].incIteratorPtr( &iterator );
            while( unit_ptr != 0 ) {
                if ( key_func( key, &(unit_ptr->unit->unit_state) ) == 0 ) {
                    if ( find_first == true )
                        return( true );

                    working_list->add( unit_ptr->unit->unit_id, work_index );
                    work_index++;
                } // ** if key_func

                unit_ptr = unit_lists[ list_index ].incIteratorPtr( &iterator );
            } // ** while

        } // ** if search flags

    } // ** for

    if( (find_first == false) && (work_index > 0) )
        return( true );

    return( false );
} // ** quearyUnitsKeySearch

// ******************************************************************

bool UnitInterface::quearyClosestUnit( UnitBase **closest_unit_ptr,
                                       iXY &loc,
                                       PlayerID &player_id,
                                       unsigned char search_flags )
{
    UnitPointer *unit_ptr;
    UnitPointer *iterator;
    unsigned long list_index;
    UnitPointer *closest_unit = 0;
    long closest_magnitude = 0;

    for( list_index = 0; list_index < max_players; list_index++) {
        if (   ( (search_flags == _search_exclude_player) && (player_id.getIndex() != list_index) )
                || ( (search_flags == _search_player) && (player_id.getIndex() == list_index ) )
                || (search_flags == _search_all_players)
           ) {

            if ( PlayerInterface::getPlayerStatus( list_index ) == _player_state_active ) {
                unit_lists[ list_index ].resetIterator( &iterator );

                unit_ptr = unit_lists[ list_index ].incIteratorPtr( &iterator );
                while( unit_ptr != 0 ) {
                    iXY delta;
                    long temp_mag;

                    if ( closest_unit == 0 ) {
                        closest_unit = unit_ptr;
                        delta  = loc - unit_ptr->unit->unit_state.location;
                        closest_magnitude = long(delta.mag2());
                    } else {
                        delta  = loc - unit_ptr->unit->unit_state.location;
                        temp_mag = long(delta.mag2());

                        if ( closest_magnitude > temp_mag ) {
                            closest_unit = unit_ptr;
                            closest_magnitude = temp_mag;
                        }
                    } // ** else

                    unit_ptr = unit_lists[ list_index ].incIteratorPtr( &iterator );
                } // ** while
            }
        } // ** if search flags

    } // ** for

    if( closest_unit != 0 ) {
        *closest_unit_ptr = closest_unit->unit;
        return( true );
    }

    *closest_unit_ptr = 0;
    return( false );
}

bool UnitInterface::quearyClosestUnit( UnitBase **closest_unit_ptr, iRect &bounding_rect, iXY &loc )
{
    UnitBase *closest_unit = 0;
    long closest_magnitude = 0;
    iRect bucket_rect;
    UnitBucketList *bucket_list;
    UnitBucketPointer *traversal_ptr;

    bucket_rect = unit_bucket_array.worldRectToBucketRect( bounding_rect );

    for( long row_index = bucket_rect.min.y; row_index <= bucket_rect.max.y; row_index++ ) {
        for( long column_index = bucket_rect.min.x; column_index <= bucket_rect.max.x; column_index++ ) {
            bucket_list = unit_bucket_array.getBucket( row_index, column_index );

            traversal_ptr = bucket_list->getFront();

            while( traversal_ptr != 0 ) {
                iXY delta;
                long temp_mag;

                if ( closest_unit == 0 ) {
                    closest_unit = traversal_ptr->unit;
                    delta  = loc - traversal_ptr->unit->unit_state.location;
                    closest_magnitude = long(delta.mag2());
                } else {
                    delta  = loc - traversal_ptr->unit->unit_state.location;
                    temp_mag = long(delta.mag2());

                    if ( closest_magnitude > temp_mag ) {
                        closest_unit = traversal_ptr->unit;
                        closest_magnitude = temp_mag;
                    }
                }

                traversal_ptr = traversal_ptr->next;
            } // ** while
        } // ** for
    } // ** for

    if( closest_unit != 0 ) {
        *closest_unit_ptr = closest_unit;
        return( true );
    }

    *closest_unit_ptr = 0;
    return( false );
}

// ******************************************************************
bool UnitInterface::quearyClosestEnemyUnit( UnitBase **closest_unit_ptr,
        iXY &loc,
        unsigned short player_index
                                          )
{
    UnitPointer *unit_ptr;
    UnitPointer *iterator;
    unsigned long list_index;
    UnitPointer *closest_unit = 0;
    long closest_magnitude = 0;

    for( list_index = 0; list_index < max_players; list_index++) {
        if ( ( player_index != list_index ) &&
                ( PlayerInterface::isAllied( player_index, list_index ) == false )
           ) {
            unit_lists[ list_index ].resetIterator( &iterator );

            unit_ptr = unit_lists[ list_index ].incIteratorPtr( &iterator );
            while( unit_ptr != 0 ) {
                iXY delta;
                long temp_mag;

                if ( closest_unit == 0 ) {
                    closest_unit = unit_ptr;
                    delta  = loc - unit_ptr->unit->unit_state.location;
                    closest_magnitude = long(delta.mag2());
                } else {
                    delta  = loc - unit_ptr->unit->unit_state.location;
                    temp_mag = long(delta.mag2());

                    if ( closest_magnitude > temp_mag ) {
                        closest_unit = unit_ptr;
                        closest_magnitude = temp_mag;
                    }
                } // ** else

                unit_ptr = unit_lists[ list_index ].incIteratorPtr( &iterator );
            } // ** while

        } // ** if search flags

    } // ** for

    if( closest_unit != 0 ) {
        *closest_unit_ptr = closest_unit->unit;
        return( true );
    }

    *closest_unit_ptr = 0;
    return( false );
}

// ******************************************************************


unsigned char UnitInterface::quearyUnitLocationStatus( iXY loc )
{
    PlayerID player_id;
    unsigned long player_index;
    UnitBase *unit_ptr;
    UnitState *unit_state;
    UnitPointer *iterator;
    unsigned long list_index;

    player_id = PlayerInterface::getLocalPlayerID();
    player_index = player_id.getIndex();

    for( list_index = 0; list_index < max_players; list_index++) {
        unit_lists[ list_index ].resetIterator( &iterator );

        unit_ptr = unit_lists[ list_index ].incIterator( &iterator );
        while( unit_ptr != 0 ) {
            unit_state = &unit_ptr->unit_state;
            if ( unit_state->bounds( loc ) ) {
                if ( list_index == player_index )
                    return( _unit_player );
                else
                    if ( PlayerInterface::isAllied( player_index, list_index ) )
                        return( _unit_allied );
                    else
                        return( _unit_enemy );
            } // ** if

            unit_ptr = unit_lists[ list_index ].incIterator( &iterator );
        } // ** while

    } // ** for

    return( _no_unit_found );
}

// ******************************************************************

bool UnitInterface::quearyUnitAtMapLoc( iXY map_loc, UnitID *queary_unit_id )
{
    UnitBase *unit_ptr;
    UnitState *unit_state;
    UnitPointer *iterator;
    unsigned long list_index;
    iXY unit_map_loc;


    for( list_index = 0; list_index < max_players; list_index++) {
        unit_lists[ list_index ].resetIterator( &iterator );

        unit_ptr = unit_lists[ list_index ].incIterator( &iterator );
        while( unit_ptr != 0 ) {
            unit_state = &unit_ptr->unit_state;
            MapInterface::pointXYtoMapXY( unit_state->location, &unit_map_loc );
            if( map_loc == unit_map_loc ) {
                *queary_unit_id = unit_ptr->unit_id;
                return( true );
            }

            unit_ptr = unit_lists[ list_index ].incIterator( &iterator );
        } // ** while

    } // ** for

    return( false );
}

// ******************************************************************

void UnitInterface::startUnitPositionEnumeration( void )
{
    unit_position_enum_list_index = 0;

    unit_position_enum_local_player_index = PlayerInterface::getLocalPlayerIndex();

    unit_lists[ unit_position_enum_list_index ].resetIterator( &unit_position_enum_iterator );
}

// ******************************************************************

bool UnitInterface::unitPositionEnumeration( iXY *position, unsigned char *unit_disposition, unsigned char *threat_level  )
{
    UnitBase *unit_ptr;

    unit_ptr = unit_lists[ unit_position_enum_list_index ].incIterator( &unit_position_enum_iterator );

    if ( unit_ptr == 0 ) {
        // ** NOTE: enumerates all units **
        unit_position_enum_list_index++;

        if ( unit_position_enum_list_index == max_players) {
            return( false );
        }

        unit_lists[ unit_position_enum_list_index ].resetIterator( &unit_position_enum_iterator );
    } else {
        if ( unit_position_enum_list_index == unit_position_enum_local_player_index ) {
            *unit_disposition = _unit_player;
        } else
            if ( PlayerInterface::isAllied( unit_position_enum_list_index , unit_position_enum_local_player_index ) ) {
                *unit_disposition =  _unit_allied;
            } else {
                *unit_disposition =  _unit_enemy;
            }

        *position = unit_ptr->unit_state.location;
        *threat_level = unit_ptr->unit_state.threat_level;
        return( true );
    }

    return ( true );
}

// ******************************************************************
void UnitInterface::resetUnitCycleIterator( unsigned long *iterator )
{
    unit_cycle_player_index = PlayerInterface::getLocalPlayerIndex();

    unit_lists[ unit_cycle_player_index ].resetIteratorAsync( iterator );
}

// ******************************************************************
iXY UnitInterface::unitPositionCycle( unsigned long *iterator )
{
    UnitBase *unit_ptr;
    bool  completed;

    if ( unit_lists[ unit_cycle_player_index ].containsItems() == 0 )
        return( iXY( 0, 0 ) );

    unit_ptr = unit_lists[ unit_cycle_player_index ].incIteratorAsync( iterator, &completed );

    if ( unit_ptr == 0 ) {
        unit_lists[ unit_cycle_player_index ].resetIteratorAsync( iterator );
        unit_ptr = unit_lists[ unit_cycle_player_index ].incIteratorAsync( iterator, &completed );
    }

    return( unit_ptr->unit_state.location );
}

// ******************************************************************

void UnitInterface::processManagerMessage( UnitMessage *message )
{
    switch( message->message_id ) {
    case _umesg_end_lifecycle :
        unitManagerMesgEndLifecycle( message );
        break;

    } // ** switch
}

// ******************************************************************

void UnitInterface::unitManagerMesgEndLifecycle( UnitMessage *message )
{
    UMesgEndLifeCycleUpdate *lifecycle_update;

    lifecycle_update = (UMesgEndLifeCycleUpdate *) message;

    PlayerInterface::setKill( lifecycle_update->destroyer,
                              lifecycle_update->destroyed,
                              lifecycle_update->unit_type );

    PlayerScoreUpdate score_update;

    score_update.kill_on_player_index = lifecycle_update->destroyed.getPlayer();
    score_update.kill_by_player_index = lifecycle_update->destroyer.getPlayer();
    score_update.unit_type = lifecycle_update->unit_type;

    SERVER->sendMessage( &score_update, sizeof( PlayerScoreUpdate ), 0 );
}


// ******************************************************************

void UnitInterface::unitSyncMessage( NetMessage *net_message )
{
    UnitIniSyncMessage *sync_message;
    unsigned short player_index;
    UnitBase *unit;

    sync_message = (UnitIniSyncMessage *) net_message;

    player_index = sync_message->unit_id.getPlayer();

    unit = newUnit( sync_message->unit_type,
                    sync_message->location,
                    player_index );

    unit->unit_id = sync_message->unit_id;
    unit->in_sync_flag = false;

    unit_lists[ player_index ].add( (UnitBase *) unit, sync_message->unit_id );
    unit_bucket_array.addUnit( unit );

    unit->unit_state = sync_message->unit_state;
    unit->unit_state.select = false;
}

// ******************************************************************

void UnitInterface::unitOpcodeMessage( NetMessage *net_message )
{
    UnitOpcodeMessage *opcode_message;
    UnitOpcodeStruct  opcode;
    UnitBase	        *unit;

    opcode_message = (UnitOpcodeMessage *) net_message;

    opcode_encoder.setDecodeMessage( opcode_message );

    while( opcode_encoder.decodeMessage( &opcode ) ) {
        unit = getUnit( opcode.player_index, opcode.unit_index );

        if ( unit != 0 ) {
            unit->evalCommandOpcode( &opcode );
        }
        // NOTE: PUT SOME LOG CODE HERE TO NOTICE UNAVAILABLE UNIT
    } // ** while

}

// ******************************************************************

void UnitInterface::unitDestroyMessage( NetMessage *net_message )
{
    UnitRemoteDestroy *remote_destroy;
    UnitID unit_id;
    UnitBase *unit;

    remote_destroy = (UnitRemoteDestroy *) net_message;
    unit_id = remote_destroy->unit_to_destroy;

    // Particle Shit Hack
    unit = getUnit( unit_id );
    if ( unit != 0 ) {
        ParticleInterface::addHit(unit->unit_state);

        //SFX
        sound->playAmbientSound("expl",
                                WorldViewInterface::getCameraDistance( unit->unit_state.location ) );

        unit_lists[ unit_id.getPlayer() ].deleteUnit( unit_id );
    }
}

// ******************************************************************

void UnitInterface::unitCreateMessage( NetMessage *net_message )
{
    UnitRemoteCreate *create_mesg;
    UnitBase *unit;
    unsigned short player_index;

    create_mesg = (UnitRemoteCreate *) net_message;

    player_index = create_mesg->new_unit_id.getPlayer();

    unit = newUnit( create_mesg->unit_type,
                    create_mesg->location,
                    player_index );

    unit->unit_id = create_mesg->new_unit_id;
    unit_lists[ player_index ].add( (UnitBase *) unit, create_mesg->new_unit_id );
    unit_bucket_array.addUnit( unit );
}

// ******************************************************************

void UnitInterface::unitSyncIntegrityCheckMessage( NetMessage *net_message )
{
    sortBucketArray();
}

// ******************************************************************
void UnitInterface::processNetMessage( NetMessage *net_message )
{
    switch( net_message->message_id ) {
    case _net_message_id_ini_sync_mesg : {
            unitSyncMessage( net_message );
        }
        break;

    case _net_message_id_opcode_mesg : {
            unitOpcodeMessage( net_message );
        }
        break;

    case _net_message_id_destroy_unit : {
            unitDestroyMessage( net_message );
        }
        break;

    case _net_message_id_create_unit : {
            unitCreateMessage( net_message );
        }
        break;

    case _net_message_id_unit_sync_integrity_check : {
            unitSyncIntegrityCheckMessage( net_message );
        }
        break;

    } // ** switch

}

// ******************************************************************

void UnitInterface::startRemoteUnitSync( PlayerID &remote_player )
{
    sync_units_remote_player = remote_player;

    sync_units_list_index = 0;
    sync_units_in_sync_count = 0;
    sync_units_in_sync_partial_count = 0;
    sync_units_total_units = getTotalUnitCount();

    unit_lists[ sync_units_list_index ].resetIteratorAsync( &sync_units_iterator );

    sync_units_packet_timer.changeRate( 10 );
}

bool UnitInterface::syncRemoteUnits( int *send_return_code, int *percent_complete )
{
    UnitIniSyncMessage sync_message;
    UnitBase *unit;

    *percent_complete = -1;

    int send_ret_val;
    *send_return_code = _network_ok;

    if ( !sync_units_packet_timer.count() )
        return( true );

    unit = unit_lists[ sync_units_list_index ].incIteratorAsync( &sync_units_iterator, &sync_units_complete_flag );

    if ( sync_units_complete_flag == true ) {
        sync_units_list_index++;

        if ( sync_units_list_index >= max_players) {
            *percent_complete = 100;
            return( false );
        }

        unit_lists[ sync_units_list_index ].resetIteratorAsync( &sync_units_iterator );
    } else {
        iXY unit_map_loc;
        MapInterface::pointXYtoMapXY( unit->unit_state.location, &unit_map_loc );

        sync_message.unit_type  = unit->unit_state.unit_type;
        sync_message.unit_id    = unit->unit_id;
        sync_message.location   = unit_map_loc;
        sync_message.unit_state = unit->unit_state;

        send_ret_val = SERVER->sendMessage( sync_units_remote_player,
                                            &sync_message, sizeof( UnitIniSyncMessage ), 0 );

        if ( send_ret_val != _network_ok ) {
            *send_return_code = send_ret_val;
            return( false );
        }

        unit->syncUnit();

        sync_units_in_sync_count++;
        sync_units_in_sync_partial_count++;
        if ( sync_units_in_sync_partial_count >= 10 ) {
            float percent;
            percent = ( ( (float) sync_units_in_sync_count) / ( (float) sync_units_total_units ) ) * 100;
            *percent_complete = (int) percent;
            sync_units_in_sync_partial_count = 0;
        }

        return( true );
    }

    return ( true );
}

void UnitInterface::destroyPlayerUnits( PlayerID &player_id )
{
    UMesgSelfDestruct self_destruct;
    UnitBase *unit_ptr;
    UnitPointer *iterator;

    unsigned short player_index;

    self_destruct.setHeader( _umesg_flag_unique );

    player_index = player_id.getIndex();

    unit_lists[ player_index ].resetIterator( &iterator );

    unit_ptr = unit_lists[ player_index ].incIterator( &iterator );
    while( unit_ptr != 0 ) {
        unit_ptr->processMessage( &self_destruct );
        unit_ptr = unit_lists[ player_index ].incIterator( &iterator );
    } // ** while

}

