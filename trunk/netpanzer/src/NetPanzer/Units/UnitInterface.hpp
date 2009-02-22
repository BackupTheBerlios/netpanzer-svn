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

#include <vector>
#include <map>
#include <cassert>
#include "Core/CoreTypes.hpp"

#include "UnitBucketArray.hpp"
#include "UnitOpcodeEncoder.hpp"

#include "Classes/PlacementMatrix.hpp"
#include "Classes/PlayerUnitConfig.hpp"

enum
{
    _search_all_players,
    _search_player,
    _search_exclude_player,
    _search_enemy_only
};

enum { _no_unit_found, _unit_player, _unit_allied, _unit_enemy };

class NetPacket;

class UnitInterface
{
public:
    // type definitions
    typedef std::map<UnitID, Unit*> Units;
    typedef std::vector<UnitList> PlayerUnitList;

    // initialization/cleanup
    static void initialize( const unsigned int max_units );
    static void cleanUp();
    static void reset();

    // unit getters
    static Unit * getUnit( UnitID id )
    {
        Units::iterator i = units.find(id);
        return (i!=units.end())?i->second:0;
    }

    static const Units& getUnits()
    {
        return units;
    }

    static const UnitList& getPlayerUnits( const Uint16 player_id )
    {
        assert(player_id < playerUnitLists.size());
        return playerUnitLists[player_id];
    }

    static size_t getUnitCount( const Uint16 player_index )
    {
        return getPlayerUnits(player_index).size();
    }

    static size_t getTotalUnitCount()
    {
        return units.size();
    }

    // main loop methods
    static void updateUnitStatus();

    // graphic methods
    static void offloadGraphics( SpriteSorter &sorter );

    // unit creation
    static Unit* createUnit( const unsigned short unit_type,
                             const iXY &location,
                             const Uint16 player_id);

    static void spawnPlayerUnits( const iXY & location,
                                  const Uint16 player_id,
                                  const PlayerUnitConfig & unit_config );

    static void destroyPlayerUnits( const Uint16 player_id );


    // unit querying
    static void queryPlayerUnitsAt( std::vector<UnitID>& working_list,
                                    const iXY& point,
                                    const Uint16 player_id );

    static void queryUnitsInWorldRect( std::vector<Unit *>& working_list,
                                       const iRect& rect );

    static void queryPlayerUnitsInWorldRect( std::vector<UnitID>& working_list,
                                             const iRect& rect,
                                             const Uint16 player_id );

    static void queryNonPlayerUnitsInWorldRect( std::vector<Unit *>& working_list,
                                                const iRect& rect,
                                                const Uint16 player_id );

    static bool queryClosestUnit( Unit **closest_unit_ptr,
                                  const iXY &loc,
                                  const Uint16 player_id,
                                  const unsigned char search_flags );

    static bool queryClosestUnit( Unit **closest_unit_ptr,
                                  const iRect &bounding_rect,
                                  const iXY &loc );

    static bool queryClosestEnemyUnit( Unit **closest_unit_ptr,
                                       const iXY &loc,
                                       const Uint16 player_index );

    static Unit * queryUnitAtMapLoc( const iXY & map_loc );

    static Unit * queryNonPlayerUnitAtWorld( const iXY & world_loc,
                                         const Uint16 player_id );

    static unsigned char queryUnitLocationStatus( const iXY & loc );

    // message methods
    static void processNetPacket( const NetPacket* packet );
    static void sendMessage( const UnitMessage* message,
                             const PlayerState* player = 0 );

    static void processNetMessage( const NetMessage *net_message );

private:
    friend class Unit;

    // Unit Message Handler Methods
    static void unitManagerMesgEndLifecycle( const UnitMessage *message );

    // Network Message Handler Variables
    static Timer message_timer;
    static Timer no_guarantee_message_timer;
    static UnitOpcodeEncoder opcode_encoder;

    // Network Message Handler Methods
    static void sendOpcode( const UnitOpcode* opcode )
    {
        opcode_encoder.encode(opcode);
    }

    static void unitCreateMessage( const NetMessage *net_message );
    static void unitDestroyMessage( const NetMessage *net_message );
    static void unitSyncMessage( const NetMessage *net_message );
    static void unitOpcodeMessage( const NetMessage *net_message );
    static void unitSyncIntegrityCheckMessage( const NetMessage *net_message );

    static unsigned long  sync_units_iterator;
    static bool           sync_units_complete_flag;
    static unsigned short sync_units_list_index;
    static Timer	  sync_units_packet_timer;
    static unsigned long  sync_units_in_sync_count;
    static unsigned long  sync_units_in_sync_partial_count;
    static unsigned long  sync_units_total_units;

    static Units units;
    static PlayerUnitList playerUnitLists;
        
    static UnitBucketArray unit_bucket_array;
    static unsigned int units_per_player;
    static PlacementMatrix unit_placement_matrix;

    static Uint16 lastUnitID;
    static UnitID newUnitID();

    static Unit* newUnit( const unsigned short unit_type,
                          const iXY &location,
                          const unsigned short player_index,
                          const UnitID id );

    static void addUnit( Unit *unit );
    
    static void removeUnit( Units::iterator i );

};

#endif // ** _UNITINTERFACE_HPP
