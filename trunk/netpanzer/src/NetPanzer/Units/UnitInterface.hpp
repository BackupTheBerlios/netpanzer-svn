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
#include "Core/CoreTypes.hpp"

#include "ArrayUtil/BitArray.hpp"
#include "UnitBucketArray.hpp"
#include "UnitOpcodeEncoder.hpp"

class NetPacket;
class PlayerUnitConfig;
class SpriteSorter;

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
        Units::iterator i = units->find(id);
        return (i!=units->end())?i->second:0;
    }

    static const Units* getUnits()
    {
        return units;
    }
    
    static const UnitList* getPlayerUnits( const Uint16 player_id )
    {
        if ( player_id < playerUnitLists->size() )
        {
            return &(*playerUnitLists)[player_id];
        }
        return 0;
    }

    static size_t getPlayerUnitCount( const Uint16 player_id )
    {
        if ( player_id < playerUnitLists->size() )
        {
            return (*playerUnitLists)[player_id].size();
        }
        return 0;
    }

    static size_t getTotalUnitCount()
    {
        return units->size();
    }

    // main loop methods
    static void updateUnitStatus();

    // graphic methods
    static void offloadGraphics( SpriteSorter &sorter );

    // unit creation
    static Unit* createUnit(const unsigned short unit_type,
                            const iXY &location,
                            const Uint16 player_id);

    static void spawnPlayerUnits(const iXY & location,
                                 const Uint16 player_id,
                                 const PlayerUnitConfig & unit_config );

    static void destroyPlayerUnits( const Uint16 player_id );

    // message methods
    static void processNetPacket( const NetPacket* packet );
    static void sendMessage( const UnitMessage* message,
                             const PlayerState* player = 0 );

    static void processNetMessage( const NetMessage *net_message );

    static bool unitOccupiesLoc(const iXY &unit_map_loc)
    {
        return unit_black_board->getBit(unit_map_loc.x, unit_map_loc.y);
    }

    static void markUnitLoc(const iXY &unit_map_loc)
    {
        unit_black_board->setBit(unit_map_loc.x, unit_map_loc.y);
    }

    static void unmarkUnitLoc(const iXY &unit_map_loc)
    {
        unit_black_board->clearBit(unit_map_loc.x, unit_map_loc.y);
    }

private:
    friend class Unit;
    UnitInterface();
    ~UnitInterface();

    // Unit Message Handler Methods
    static void unitManagerMesgEndLifecycle( const UnitMessage *message );

    // Network Message Handler Methods
    static void sendOpcode( const UnitOpcode* opcode )
    {
        opcode_encoder->encode(opcode);
    }

    static void unitCreateMessage( const NetMessage *net_message );
    static void unitDestroyMessage( const NetMessage *net_message );
    static void unitSyncMessage( const NetMessage *net_message );
    static void unitOpcodeMessage( const NetMessage *net_message );
    static void unitSyncIntegrityCheckMessage( const NetMessage *net_message );

    static Unit* newUnit( const unsigned short unit_type,
                          const iXY &location,
                          const unsigned short player_index,
                          const UnitID id );

    static void addUnit( Unit *unit );
    static void removeUnit( Units::iterator i );

    static Units* units;
    static PlayerUnitList* playerUnitLists;
    static BitArray* unit_black_board;
    static UnitOpcodeEncoder* opcode_encoder;
};

#endif // ** _UNITINTERFACE_HPP
