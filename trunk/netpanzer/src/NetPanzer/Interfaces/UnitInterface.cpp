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

#include <iostream>

#include "UnitInterface.hpp"
#include "UnitProfileInterface.hpp"
#include "PlayerInterface.hpp"
#include "MapInterface.hpp"
#include "WorldViewInterface.hpp"

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

#include "Types/iXY.hpp"
#include "ArrayUtil/Timer.hpp"
#include "Server.hpp"
#include "NetworkState.hpp"
#include "NetMessageEncoder.hpp"
#include "Console.hpp"

#include "UnitMessageTypes.hpp"
#include "PlayerNetMessage.hpp"

#include "System/Sound.hpp"
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

unsigned long   UnitInterface::sync_units_iterator;
bool	        UnitInterface::sync_units_complete_flag;
unsigned short  UnitInterface::sync_units_list_index;
unsigned long   UnitInterface::sync_units_total_units;
unsigned long   UnitInterface::sync_units_in_sync_count;
unsigned long   UnitInterface::sync_units_in_sync_partial_count;
size_t          UnitInterface::units_per_player;
Timer		UnitInterface::sync_units_packet_timer;
PlayerID	UnitInterface::sync_units_remote_player;


// ******************************************************************

void UnitInterface::initialize( unsigned long max_units )
{
    max_players = PlayerInterface::getMaxPlayers();

    unit_lists = new UnitList [ max_players ];

    unit_bucket_array.initialize(MapInterface::getSize(), TileInterface::getTileSize() );

    unique_generator = 0;
    message_timer.changeRate( 8 );
    no_guarantee_message_timer.changeRate( 15 );
    opcode_encoder.initialize( _opcode_encoder_send_method_guarantee );
    no_guarantee_opcode_encoder.initialize( _opcode_encoder_send_method_no_guarantee );

    units_per_player = max_units;
}

// ******************************************************************

void UnitInterface::cleanUp( void )
{
    delete[] unit_lists;
}

void UnitInterface::reset( void )
{
    for(size_t i = 0; i < max_players; i++ ) {
        unit_lists[i].clear();
    }

    unit_bucket_array.initialize( MapInterface::getSize(), TileInterface::getTileSize() );
}

// ******************************************************************
unsigned long UnitInterface::getTotalUnitCount( void )
{
    unsigned long unit_total = 0;

    for( int i = 0; i < max_players; i++ ) {
        unit_total += unit_lists[ i ].size();
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
    else if (  message->isFlagged( _umesg_flag_broadcast ) ) {
	for(size_t list_index = 0; list_index < max_players; list_index++) {
	    UnitList* unitlist = & unit_lists[list_index];
	    for(UnitList::iterator i = unitlist->begin();
		    i != unitlist->end(); ++i) {
		i->processMessage( message );
	    } // ** for
	} // ** for
    } else if ( message->isFlagged( _umesg_flag_manager_request ) ) {
  	processManagerMessage( message );
    } // ** if _umesg_flag_manager_request
}

// ******************************************************************

void UnitInterface::updateUnitStatus()
{
    for(size_t list_index = 0; list_index < max_players; list_index++) {
	UnitList* unitlist = & unit_lists[list_index];
	
	for(UnitList::iterator i = unitlist->begin();
		i != unitlist->end(); /* moved to end of loop */) {
	    UnitBase* unit = *i;
	    
            if (unit->unit_state.lifecycle_state == _UNIT_LIFECYCLE_INACTIVE) {
                // Particle Shit Hack
                ParticleInterface::addHit(unit->unit_state);

                //SFX
                sound->playAmbientSound("expl",
                        WorldViewInterface::getCameraDistance(
                            unit->unit_state.location ) );
		
                deleteUnit(unit);
		i = unitlist->erase(i);
		continue;
            }
	    
	    unsigned long pre_update_bucket_index;
    	    unsigned long post_update_bucket_index;

	    pre_update_bucket_index = unit_bucket_array.worldLocToBucketIndex( unit->unit_state.location );

	    unit->updateState();

	    post_update_bucket_index = unit_bucket_array.worldLocToBucketIndex( unit->unit_state.location );

	    if ( post_update_bucket_index != pre_update_bucket_index ) {
	    	unit_bucket_array.moveUnit( unit->unit_id, pre_update_bucket_index, post_update_bucket_index );
	    }
	    
    	    ++i;
        } // ** for
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
        if ( unit_lists[list_index].contains( new_index ) == true )
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
    default:
        assert("unknown unit_type" == 0);
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

    unit_lists[player_index].insert( unit, new_index );

    unit->unit_id.set(player_index, new_index, 0);

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

    if (unit_lists[player_index].size() >= units_per_player)
	return 0;

    unit = newUnit( unit_type, location, player_index );

    addNewUnit( unit, player );

    return unit;
}

// ******************************************************************

void UnitInterface::spawnPlayerUnits( const iXY &location,
                                      const PlayerID &player, const PlayerUnitConfig &unit_config )
{
    iXY next_loc;
    UnitBase *unit;
    unsigned long unit_type_index;
    unsigned long unit_spawn_count;
    unsigned long unit_spawn_index;

    PUBLIC_MESSAGE_ENCODER.resetEncoder();

    unit_placement_matrix.reset( location );

    for ( unit_type_index = 0; unit_type_index < _MAX_UNIT_TYPES; unit_type_index++ ) {

        unit_spawn_count = unit_config.getSpawnUnitCount( unit_type_index );

        for ( unit_spawn_index = 0; unit_spawn_index < unit_spawn_count; unit_spawn_index++ ) {
            unit_placement_matrix.getNextEmptyLoc( &next_loc );
            unit = createUnit( unit_type_index, next_loc, player );

            assert(unit != 0);
            UnitRemoteCreate create_mesg(unit->unit_id, next_loc.x,
                next_loc.y, unit->unit_state.unit_type);
            PUBLIC_MESSAGE_ENCODER.encodeMessage( &create_mesg,
                                                  sizeof( create_mesg )
                                                );

        } // ** for unit_spawn_index

    } // ** for unit_type_index

    PUBLIC_MESSAGE_ENCODER.sendEncodedMessage();
}

// ******************************************************************

bool UnitInterface::queryUnitsAt(std::vector<UnitID>& working_list,
                      const iXY& point, PlayerID player_id,
                      unsigned char search_flags, bool find_first  )
{
    for(size_t list_index = 0; list_index < max_players; list_index++) {
        if (   ( (search_flags == _search_exclude_player) && (player_id.getIndex() != list_index) )
                || ( (search_flags == _search_player) && (player_id.getIndex() == list_index ) )
           ) {
	    UnitList* unitlist = & unit_lists[list_index];

	    for(UnitList::iterator i = unitlist->begin();
		    i != unitlist->end(); ++i) {
		if(i->unit_state.bounds(point)) {
		    if(find_first)
		    	return true;

		    working_list.push_back(i->unit_id);
		}
            }
        } // ** if search flags
    } // ** for

    if( (find_first == false) && (working_list.size() > 0) )
        return true;

    return false;
} // ** quearyUnitsKeySearch

// ******************************************************************

bool UnitInterface::queryUnitsAt(std::vector<UnitID>& working_list,
                      const iRect& rect, PlayerID player_id,
                      unsigned char search_flags, bool find_first  )
{
    for(size_t list_index = 0; list_index < max_players; list_index++) {
        if (   ( (search_flags == _search_exclude_player) && (player_id.getIndex() != list_index) )
                || ( (search_flags == _search_player) && (player_id.getIndex() == list_index ) )
           ) {

	    UnitList* unitlist = & unit_lists[list_index];
	    for(UnitList::iterator i = unitlist->begin();
		    i != unitlist->end(); ++i) {
                if(rect.contains(i->unit_state.location)) {
                    if(find_first)
                        return true;

                    working_list.push_back(i->unit_id);
                }
            }
        } // ** if search flags
    } // ** for

    if( (find_first == false) && (working_list.size() > 0) )
        return true;

    return false;
} // ** quearyUnitsKeySearch

/****************************************************************************/

bool UnitInterface::quearyClosestUnit( UnitBase **closest_unit_ptr,
                                       iXY &loc,
                                       PlayerID &player_id,
                                       unsigned char search_flags )
{
    long closest_magnitude = 0;
    UnitBase* closest_unit = 0;

    for(size_t list_index = 0; list_index < max_players; list_index++) {
        if (   ( (search_flags == _search_exclude_player) && (player_id.getIndex() != list_index) )
                || ( (search_flags == _search_player) && (player_id.getIndex() == list_index ) )
                || (search_flags == _search_all_players)
           ) {

            if ( PlayerInterface::getPlayerStatus( list_index ) == _player_state_active ) {
                UnitList* unitlist = & unit_lists[list_index] ;
                                
                for(UnitList::iterator i = unitlist->begin();
                        i != unitlist->end(); ++i) {
                    UnitBase* unit = *i;
                    iXY delta;
                    long temp_mag;

                    if ( closest_unit == 0 ) {
                        closest_unit = unit;
                        delta  = loc - unit->unit_state.location;
                        closest_magnitude = long(delta.mag2());
                    } else {
                        delta  = loc - unit->unit_state.location;
                        temp_mag = long(delta.mag2());

                        if ( closest_magnitude > temp_mag ) {
                            closest_unit = unit;
                            closest_magnitude = temp_mag;
                        }
                    } // ** else
                } // ** for
            }
        } // ** if search flags

    } // ** for

    if( closest_unit != 0 ) {
        *closest_unit_ptr = closest_unit;
        return true;
    }

    *closest_unit_ptr = 0;
    return false;
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
    UnitBase *closest_unit = 0;
    long closest_magnitude = 0;

    for(size_t list_index = 0; list_index < max_players; list_index++) {
        if ( ( player_index != list_index ) &&
                ( PlayerInterface::isAllied( player_index, list_index ) == false )
           ) {

            UnitList* unitlist = & unit_lists[list_index];
           
            for(UnitList::iterator i = unitlist->begin();
                    i != unitlist->end(); ++i) {
                UnitBase* unit = *i;
                iXY delta;
                long temp_mag;

                if ( closest_unit == 0 ) {
                    closest_unit = unit;
                    delta  = loc - unit->unit_state.location;
                    closest_magnitude = long(delta.mag2());
                } else {
                    delta  = loc - unit->unit_state.location;
                    temp_mag = long(delta.mag2());

                    if ( closest_magnitude > temp_mag ) {
                        closest_unit = unit;
                        closest_magnitude = temp_mag;
                    }
                } // ** else
            } // ** for
        } // ** if search flags
    } // ** for

    if( closest_unit != 0 ) {
        *closest_unit_ptr = closest_unit;
        return true;
    }

    *closest_unit_ptr = 0;
    return false;
}

// ******************************************************************


unsigned char UnitInterface::quearyUnitLocationStatus( iXY loc )
{
    PlayerID player_id;
    unsigned long player_index;

    player_id = PlayerInterface::getLocalPlayerID();
    player_index = player_id.getIndex();

    for(size_t list_index = 0; list_index < max_players; list_index++) {

        UnitList* unitlist = & unit_lists[list_index];

        for(UnitList::iterator i = unitlist->begin();
                i != unitlist->end(); ++i) {
            UnitBase* unit = *i;
            UnitState* unit_state = &unit->unit_state;
            
            if ( unit_state->bounds( loc ) ) {
                if ( list_index == player_index )
                    return _unit_player;
                else
                    if ( PlayerInterface::isAllied( player_index, list_index ) )
                        return _unit_allied;
                    else
                        return _unit_enemy;
            } // ** if
        } // ** while
    } // ** for

    return _no_unit_found;
}

// ******************************************************************

bool UnitInterface::quearyUnitAtMapLoc( iXY map_loc, UnitID *queary_unit_id )
{
    iXY unit_map_loc;

    for(size_t list_index = 0; list_index < max_players; list_index++) {
        UnitList* unitlist = & unit_lists[list_index];

        for(UnitList::iterator i = unitlist->begin();
                i != unitlist->end(); ++i) {
            UnitBase* unit = *i;
            UnitState* unit_state = & unit->unit_state;
            
            MapInterface::pointXYtoMapXY( unit_state->location, &unit_map_loc );
            if( map_loc == unit_map_loc ) {
                *queary_unit_id = unit->unit_id;
                return true;
            }
        } // ** for
    } // ** for

    return false;
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

    // show score on server display
    const std::string& player1 =
        PlayerInterface::getPlayerState(lifecycle_update->destroyer.getPlayer())
            ->getName();
    const std::string& player2 =
        PlayerInterface::getPlayerState(lifecycle_update->destroyed.getPlayer())
            ->getName();
    const std::string& unitname1 =
        UnitProfileInterface::getUnitProfile(lifecycle_update->unit_type)
            ->unitname;
    int unittype2 =
        UnitInterface::getUnit(lifecycle_update->destroyer)
            ->unit_state.unit_type;
    const std::string& unitname2 =
        UnitProfileInterface::getUnitProfile(unittype2)->unitname;
    // TODO display unit names...
    *Console::server << "'" << player1 << "' killed a '" << unitname1
            << "' from '" << player2 << "' with his '" << unitname2 << "'.";

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
                    iXY(sync_message->getLocX(), sync_message->getLocY()),
                    player_index );

    unit->unit_id = sync_message->unit_id;
    unit->in_sync_flag = false;

    unit_lists[player_index].insert(unit, sync_message->unit_id);
    unit_bucket_array.addUnit( unit );

    unit->unit_state.setFromNetworkUnitState(sync_message->unit_state);
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

        UnitList::iterator i =
            unit_lists[unit_id.getPlayer()].find(unit_id.getIndex());
        if(i != unit_lists[unit_id.getPlayer()].end())
            unit_lists[unit_id.getPlayer()].erase(i);
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
                    iXY(create_mesg->getLocX(), create_mesg->getLocY()),
                    player_index );

    unit->unit_id = create_mesg->new_unit_id;
    unit_lists[player_index].insert(unit, create_mesg->new_unit_id);
    unit_bucket_array.addUnit( unit );
}

// ******************************************************************

void UnitInterface::unitSyncIntegrityCheckMessage(NetMessage* )
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

#if 0
void UnitInterface::startRemoteUnitSync( PlayerID &remote_player )
{
    sync_units_remote_player = remote_player;

    sync_units_list_index = 0;
    sync_units_in_sync_count = 0;
    sync_units_in_sync_partial_count = 0;
    sync_units_total_units = getTotalUnitCount();

    unit_lists[sync_units_list_index].resetIteratorAsync(&sync_units_iterator);

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
        sync_message.location_x = unit_map_loc.x;
        sync_message.location_y = unit_map_loc.y;
        sync_message.unit_state = unit->unit_state.getNetworkUnitState();

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
#endif

void UnitInterface::destroyPlayerUnits( PlayerID &player_id )
{
    UMesgSelfDestruct self_destruct;
    self_destruct.setHeader( _umesg_flag_unique );

    UnitList& unitlist = unit_lists[player_id.getIndex()];
    for(UnitList::iterator i = unitlist.begin();
            i != unitlist.end(); ++i) {
        UnitBase* unit = *i;
        unit->processMessage( &self_destruct );
    } // ** while
}

