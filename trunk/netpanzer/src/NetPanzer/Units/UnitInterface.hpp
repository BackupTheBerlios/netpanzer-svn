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
#include "Units/UnitBlackBoard.hpp"

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
    UnitBucketArray unit_bucket_array;

    UnitInterface();
    ~UnitInterface();

    // type definitions
    typedef std::map<UnitID, Unit*> Units;
    typedef std::vector<UnitList> PlayerUnitList;

    // initialization/cleanup
    void initialize( const unsigned int max_units );
    void cleanUp();
    void reset();

    // unit getters
    Unit * getUnit( UnitID id )
    {
        Units::iterator i = units.find(id);
        return (i!=units.end())?i->second:0;
    }

    const Units& getUnits()
    {
        return units;
    }

    const UnitList& getPlayerUnits( const Uint16 player_id )
    {
        assert(player_id < playerUnitLists.size());
        return playerUnitLists[player_id];
    }

    size_t getUnitCount( const Uint16 player_index )
    {
        return getPlayerUnits(player_index).size();
    }

    size_t getTotalUnitCount()
    {
        return units.size();
    }

    // main loop methods
    void updateUnitStatus();

    // graphic methods
    void offloadGraphics( SpriteSorter &sorter );

    // unit creation
    Unit* createUnit( const unsigned short unit_type,
                             const iXY &location,
                             const Uint16 player_id);

    void spawnPlayerUnits( const iXY & location,
                                  const Uint16 player_id,
                                  const PlayerUnitConfig & unit_config );

    void destroyPlayerUnits( const Uint16 player_id );

    // message methods
    void processNetPacket( const NetPacket* packet );
    void sendMessage( const UnitMessage* message,
                             const PlayerState* player = 0 );

    void processNetMessage( const NetMessage *net_message );

    bool unitOccupiesLoc( iXY &unit_map_loc )
    {
        return unit_black_board.unitOccupiesLoc(unit_map_loc);
    }

    void markUnitLoc( iXY &unit_map_loc )
    {
        unit_black_board.markUnitLoc(unit_map_loc);
    }

    void unmarkUnitLoc( iXY &unit_map_loc )
    {
        unit_black_board.unmarkUnitLoc(unit_map_loc);
    }

private:
    friend class Unit;

    UnitBlackBoard unit_black_board;
    // Unit Message Handler Methods
    void unitManagerMesgEndLifecycle( const UnitMessage *message );

    // Network Message Handler Variables
    Timer message_timer;
    Timer no_guarantee_message_timer;
    UnitOpcodeEncoder opcode_encoder;

    // Network Message Handler Methods
    void sendOpcode( const UnitOpcode* opcode )
    {
        opcode_encoder.encode(opcode);
    }

    void unitCreateMessage( const NetMessage *net_message );
    void unitDestroyMessage( const NetMessage *net_message );
    void unitSyncMessage( const NetMessage *net_message );
    void unitOpcodeMessage( const NetMessage *net_message );
    void unitSyncIntegrityCheckMessage( const NetMessage *net_message );

    unsigned long  sync_units_iterator;
    bool           sync_units_complete_flag;
    unsigned short sync_units_list_index;
    Timer	  sync_units_packet_timer;
    unsigned long  sync_units_in_sync_count;
    unsigned long  sync_units_in_sync_partial_count;
    unsigned long  sync_units_total_units;

    Units units;
    PlayerUnitList playerUnitLists;
        
    unsigned int units_per_player;
    PlacementMatrix unit_placement_matrix;

    Uint16 lastUnitID;
    UnitID newUnitID();

    Unit* newUnit( const unsigned short unit_type,
                          const iXY &location,
                          const unsigned short player_index,
                          const UnitID id );

    void addUnit( Unit *unit );
    
    void removeUnit( Units::iterator i );
};

#endif // ** _UNITINTERFACE_HPP
