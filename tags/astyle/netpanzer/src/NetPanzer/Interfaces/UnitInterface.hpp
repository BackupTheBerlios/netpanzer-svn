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
#ifndef _UNITINTERFACE_HPP
#define _UNITINTERFACE_HPP

#include "UnitList.hpp"
#include "UnitBucketArray.hpp"
#include "UnitMessage.hpp"
#include "PlayerState.hpp"
#include "Timer.hpp"
#include "PlacementMatrix.hpp"
#include "UnitIDList.hpp"


#include "UnitNetMessage.hpp"
#include "UnitOpcodeEncoder.hpp"

enum { _search_all_players,
       _search_player,
       _search_exclude_player,
       _search_enemy_only };

enum { _no_unit_found, _unit_player, _unit_allied, _unit_enemy };

class UnitInterface
{
protected:
    static UnitList *unit_lists;
    static UnitBucketArray unit_bucket_array;
    static unsigned short max_players;
    static PlacementMatrix unit_placement_matrix;

    static unsigned short unique_generator;
    static bool isUniqueIndex( unsigned short new_index );
    static unsigned short uniqueIndex( void );


    static UnitBase * newUnit( unsigned short unit_type,
                               const iXY &location,
                               unsigned short player_index );

    static void addNewUnit( UnitBase *unit, const PlayerID &player  );

    static void deleteUnit( UnitID unit_id );
    static void deleteUnit( UnitBase *unit );

    static void sortBucketArray( void );

public:

    static void initialize( unsigned long max_units );
    static void cleanUp( void );
    static void reset( void );

    static inline UnitList * getUnitList( unsigned long player )
    {
        assert( (player < max_players) );
        return ( &unit_lists[ player ] );
    }

    static inline UnitList * getUnitList( const UnitID &unit_id )
    {
        assert( (unit_id.getPlayer() < max_players) );
        return ( &unit_lists[ unit_id.getPlayer() ] );
    }

    static inline unsigned long getUnitCount( unsigned short player_index )
    {
        assert( (player_index < max_players) );
        return( unit_lists[ player_index ].containsItems() );
    }

    static unsigned long getTotalUnitCount( void );

    static inline UnitPointer * getUnitPointer( const UnitID &unit_id )
    {
        assert( (unit_id.getPlayer() < max_players) );
        return( unit_lists[ unit_id.getPlayer() ][ unit_id.getIndex() ] );
    }

    static inline UnitBase * getUnit( const UnitID &unit_id )
    {
        UnitPointer *unit_pointer;
        assert ( (unit_id.getPlayer() < max_players) );

        unit_pointer = unit_lists[ unit_id.getPlayer() ][ unit_id.getIndex() ];

        if ( unit_pointer != 0 ) {
            return( unit_pointer->unit  );
        }

        return( 0 );
    }

    static inline UnitBase * getUnit( unsigned char  player_index,
                                      unsigned short unit_index    )
    {
        UnitPointer *unit_pointer;
        assert ( player_index < max_players );

        unit_pointer = unit_lists[ player_index ][ unit_index ];

        if ( unit_pointer != 0 ) {
            return( unit_pointer->unit  );
        }

        return( 0 );
    }

    static inline bool isValid( const UnitID &unit_id )
    {
        assert( unit_id.getPlayer() < max_players );
        return( unit_lists[ unit_id.getPlayer() ].isValid( unit_id ) );
    }

    static void sendMessage( UnitMessage *message );

    static void updateUnitStatus( void );

    static void offloadGraphics( SpriteSorter &sorter );

    static UnitBase * createUnit( unsigned short unit_type,
                                  const iXY &location,
                                  const PlayerID &player );

    static void spawnPlayerUnits( const iXY &location,
                                  const PlayerID &player,
                                  const PlayerUnitConfig &unit_config );

    static bool quearyUnitsKeySearch( UnitIDList *working_list,
                                      int (* key_func )( void *key, UnitState *comp ),
                                      void *key, PlayerID player_id,
                                      unsigned char search_flags,
                                      bool find_first );

    static bool quearyClosestUnit( UnitBase **closest_unit_ptr,
                                   iXY &loc,
                                   PlayerID &player_id,
                                   unsigned char search_flags );

    static bool quearyClosestUnit( UnitBase **closest_unit_ptr,
                                   iRect &bounding_rect,
                                   iXY &loc );

    static bool quearyClosestEnemyUnit( UnitBase **closest_unit_ptr,
                                        iXY &loc,
                                        unsigned short player_index );

    static bool quearyUnitAtMapLoc( iXY map_loc, UnitID *queary_unit_id );

    static unsigned char quearyUnitLocationStatus( iXY loc );


protected:
    static UnitPointer *unit_position_enum_iterator;
    static unsigned short unit_position_enum_list_index;
    static unsigned short unit_position_enum_local_player_index;

    static unsigned short unit_cycle_player_index;
    static unsigned long  unit_cycle_iterator;
public:
    // Unit positions, almost exclusivly for mini map
    static void startUnitPositionEnumeration( void );
    static bool unitPositionEnumeration( iXY *position, unsigned char *unit_disposition, unsigned char *threat_level );

    static void resetUnitCycleIterator( unsigned long *iterator );
    static iXY unitPositionCycle( unsigned long *iterator );

protected:
    // Unit Message Handler Methods
    static void processManagerMessage( UnitMessage *message );
    static void unitManagerMesgEndLifecycle( UnitMessage *message );

protected:
    // Network Message Handler Variables
    static Timer message_timer;
    static Timer no_guarantee_message_timer;
    static UnitOpcodeEncoder opcode_encoder;
    static UnitOpcodeEncoder no_guarantee_opcode_encoder;

    // Network Message Handler Methods
    static inline void sendOpcode( UnitOpcode *opcode )
    {
        opcode_encoder.encodeOpcode( opcode );
    }

    static inline void sendOpcodeNG( UnitOpcode *opcode )
    {
        no_guarantee_opcode_encoder.encodeOpcode( opcode );
    }

    static void unitSyncMessage( NetMessage *net_message );
    static void unitOpcodeMessage( NetMessage *net_message );
    static void unitDestroyMessage( NetMessage *net_message );
    static void unitCreateMessage(  NetMessage *net_message );
    static void unitSyncIntegrityCheckMessage( NetMessage *net_message );

protected:
    static unsigned long  sync_units_iterator;
    static bool	      sync_units_complete_flag;
    static unsigned short sync_units_list_index;
    static Timer		  sync_units_packet_timer;
    static PlayerID	      sync_units_remote_player;
    static unsigned long  sync_units_in_sync_count;
    static unsigned long  sync_units_in_sync_partial_count;
    static unsigned long  sync_units_total_units;

public:
    static void startRemoteUnitSync( PlayerID &remote_player );
    static bool syncRemoteUnits( int *send_return_code, int *percent_complete );

public:
    static void processNetMessage( NetMessage *net_message );
    static void destroyPlayerUnits( PlayerID &player_id );


};
#endif // ** _UNITINTERFACE_HPP
