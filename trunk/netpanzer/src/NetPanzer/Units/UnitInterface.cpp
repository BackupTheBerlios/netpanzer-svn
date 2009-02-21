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
#include <algorithm>

#include "UnitInterface.hpp"
#include "UnitProfileInterface.hpp"
#include "UnitBucketArray.hpp"
#include "UnitOpcodeDecoder.hpp"
#include "UnitBlackBoard.hpp"
#include "Unit.hpp"

#include "Interfaces/PlayerInterface.hpp"
#include "Interfaces/MapInterface.hpp"
#include "Interfaces/WorldViewInterface.hpp"

#include "Types/iXY.hpp"
#include "Util/Timer.hpp"
#include "Classes/Network/NetworkServer.hpp"
#include "Classes/Network/NetworkState.hpp"
#include "Classes/Network/NetMessageEncoder.hpp"
#include "Interfaces/Console.hpp"

#include "Classes/UnitMessageTypes.hpp"
#include "Classes/Network/PlayerNetMessage.hpp"
#include "Classes/Network/NetPacket.hpp"
#include "Classes/Network/TerminalNetMesg.hpp"

#include "System/Sound.hpp"
#include "Particles/ParticleInterface.hpp"
#include "Util/Log.hpp"
#include "Interfaces/GameConfig.hpp"


//UnitList * UnitInterface::unit_lists;
UnitInterface::Units UnitInterface::units;
UnitInterface::PlayerUnitList UnitInterface::playerUnitLists;
UnitBucketArray UnitInterface::unit_bucket_array;

PlacementMatrix UnitInterface::unit_placement_matrix;

UnitID UnitInterface::lastUnitID;

UnitOpcodeEncoder UnitInterface::opcode_encoder;

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


// ******************************************************************

void UnitInterface::initialize( unsigned long max_units )
{
    playerUnitLists.resize(PlayerInterface::getMaxPlayers());

    unit_bucket_array.initialize(MapInterface::getSize(), TileInterface::getTileSize() );

    lastUnitID = 0;
    message_timer.changeRate( 8 );
    no_guarantee_message_timer.changeRate( 15 );

    units_per_player = max_units;
}

// ******************************************************************

void UnitInterface::cleanUp()
{
    unit_bucket_array.cleanUp();

    playerUnitLists.clear();

    for(Units::iterator i = units.begin(); i != units.end(); ++i)
    {
        delete i->second;
    }
    units.clear();
}

void
UnitInterface::reset()
{
    playerUnitLists.clear();
    playerUnitLists.resize(PlayerInterface::getMaxPlayers());

    unit_bucket_array.initialize( MapInterface::getSize(), TileInterface::getTileSize() );

    for( Units::iterator i = units.begin();i != units.end(); ++i )
    {
        delete i->second;
    }
    units.clear();
}

void
UnitInterface::processNetPacket(const NetPacket* packet)
{
    const NetMessage* message = packet->getNetMessage();
    const TerminalUnitCmdRequest* terminal_command =
        (const TerminalUnitCmdRequest*) message;

    const PlayerState* player 
        = PlayerInterface::getPlayer(packet->fromPlayer);
    if(player == 0)
    {
        LOGGER.warning("UnitInterface: Player not found '%u'?!?",
                packet->fromPlayer);
        return;
    }
    
    sendMessage(&(terminal_command->comm_request) , player);
}

void
UnitInterface::sendMessage(const UnitMessage* message,const PlayerState* player)
{
    if (message->isFlagged(_umesg_flag_unique))
    {
        Unit* unit = getUnit(message->getUnitID());
        if(unit == 0)
        {
            return;
        }
        
        if(player && unit->player != player)
        {
            LOGGER.warning(
                "Terminal request for unit (%u) not owned by player (%u).\n",
                unit->id, player->getID());
            return;
        }
                    
        unit->processMessage(message);
    }
    else if (message->isFlagged( _umesg_flag_broadcast) )
    {
        if(message->message_id != _umesg_weapon_hit)
        {
            LOGGER.warning("Broadcast flag only allowed for weapon hit.");
            if(player)
            {
                LOGGER.warning("from player %u.\n", player->getID());
            }
            return;
        }
            
        for(Units::iterator i = units.begin(); i != units.end(); ++i)
        {
            Unit* unit = i->second;
            unit->processMessage(message);
        }
    }
    else if (message->isFlagged( _umesg_flag_manager_request) )
    {
        if(player)
        {
            LOGGER.warning(
                    "UnitManagerMessage sent out by player %u not allowed.",
                    player->getID());
            return;
        }
  	processManagerMessage(message);
    }
}

// ******************************************************************

void UnitInterface::removeUnit(Units::iterator i)
{
    Unit* unit = i->second;
    
    // unit explosion particles
    ParticleInterface::addHit(unit->unit_state);
    
    // unit explosion sound
    sound->playAmbientSound("expl",                   
            WorldViewInterface::getCameraDistance( unit->unit_state.location ));
    
    unit_bucket_array.removeUnit(unit);

    UnitList& plist = playerUnitLists[unit->player->getID()];
    plist.erase(std::remove(plist.begin(), plist.end(), unit), plist.end());
    
    units.erase(i);
    delete unit;
}

// ******************************************************************

void UnitInterface::updateUnitStatus()
{
    for(Units::iterator i = units.begin(); i != units.end(); /*nothing*/ )
    {
        Unit* unit = i->second;
	    
        if (unit->unit_state.lifecycle_state == _UNIT_LIFECYCLE_INACTIVE)
        {
            removeUnit(i++);
            continue;
        }

        ++i;
	    
        unsigned int bucket_before;
        bucket_before = unit_bucket_array.worldLocToBucketIndex(
                                                    unit->unit_state.location );

        unit->updateState();

        unsigned int bucket_after;
        bucket_after = unit_bucket_array.worldLocToBucketIndex(
                                                    unit->unit_state.location );

        if ( bucket_before != bucket_after )
        {
            unit_bucket_array.moveUnit( unit, bucket_before, bucket_after );
        }
    }

    if ( NetworkState::status == _network_state_server )
    {
        if (message_timer.count())
        {
            opcode_encoder.send();
        }
    }
}

void UnitInterface::offloadGraphics(SpriteSorter& sorter)
{
    iRect world_window_rect;
    iRect bucket_rect;
    UnitList::iterator bucket_iter;
    Unit * unit = 0;

    world_window_rect = sorter.getWorldWindow();
    unit_bucket_array.worldRectToBucketRect(world_window_rect, bucket_rect);

    for(long row_index = bucket_rect.min.y;
            row_index <= bucket_rect.max.y; row_index++ )
    {
        for(long column_index = bucket_rect.min.x;
                column_index <= bucket_rect.max.x; column_index++ )
        {
            UnitList & bucket_list = unit_bucket_array.getBucket(row_index, column_index);

            for(bucket_iter = bucket_list.begin();
                    bucket_iter != bucket_list.end(); ++bucket_iter)
            {
                unit = *bucket_iter;


                unit->body_anim_shadow.setWorldPos( unit->unit_state.location);

                if ( sorter.cullSprite( unit->body_anim_shadow ) == false )
                {
                    // Body
                    unit->body_anim.setWorldPos( unit->unit_state.location );
                    unit->body_anim.setFrame( unit->unit_state.body_angle.angle_int );

                    // Turret
                    unit->turret_anim.setWorldPos( unit->unit_state.location );
                    unit->turret_anim.setFrame( unit->unit_state.turret_angle.angle_int );

                    // Body Shadow
                    unit->body_anim_shadow.setFrame( unit->unit_state.body_angle.angle_int );

                    // Turret Shadow
                    unit->turret_anim_shadow.setWorldPos( unit->unit_state.location );
                    unit->turret_anim_shadow.setFrame( unit->unit_state.turret_angle.angle_int );

                    unit->select_info_box.setBoxState( unit->unit_state.select );

                    //Added layer selection to the selection box info.
                    unit->select_info_box.setAttrib( unit->unit_state.location,
                            gameconfig->unitinfodrawlayer );
                    unit->select_info_box.setHitPoints( unit->unit_state.hit_points );

                    sorter.forceAddSprite( &(unit->body_anim_shadow) );
                }
            }
        }
    }
}

// ******************************************************************

UnitID UnitInterface::newUnitID()
{
    UnitID newID = lastUnitID++;
    while(getUnit(newID) != 0)
    {
        newID = lastUnitID++;
    }

    return newID;
}

// ******************************************************************

Unit * UnitInterface::newUnit( unsigned short unit_type,
                                   const iXY &location,
                                   unsigned short player_index,
                                   UnitID id)
{
    Unit* unit = 0;
    bool color_flag;
    unsigned char unit_flag;

    if ( player_index == PlayerInterface::getLocalPlayerIndex() ) {
        color_flag = true;
    } else {
        color_flag = false;
    }

    PlayerState* player = PlayerInterface::getPlayer( player_index );
    unit_flag = player->getFlag();

    if ( unit_type < UnitProfileInterface::getNumUnitTypes() )
    {
        unit = new Unit(player, unit_type, id, location);
    }
    else
    {   // XXX change for a error window
        assert("unknown unit_type" == 0);        
    }

    return unit;
}

// ******************************************************************

void UnitInterface::addNewUnit(Unit *unit)
{
    units.insert(std::make_pair(unit->id, unit));

    playerUnitLists[unit->player->getID()].push_back(unit);

    unit_bucket_array.addUnit(unit);
}

// ******************************************************************

Unit*
UnitInterface::getUnit(UnitID id)
{
    Units::iterator i = units.find(id);
    if(i == units.end())
    {
        return 0;
    }

    return i->second;
}

// ******************************************************************
Unit* UnitInterface::createUnit( unsigned short unit_type,
                                      const iXY &location,
                                      Uint16 player_id)
{
    if (playerUnitLists[player_id].size() >= units_per_player)
    {
        return 0;
    }

    Unit* unit = newUnit(unit_type, location, player_id, newUnitID());
    addNewUnit(unit);

    return unit;
}

// ******************************************************************

void UnitInterface::spawnPlayerUnits(const iXY &location,
                                     Uint16 player_id,
                                     const PlayerUnitConfig &unit_config)
{
    iXY next_loc;
    Unit *unit;
    unsigned long unit_type_index;
    unsigned long unit_spawn_count;
    unsigned long unit_spawn_index;

    NetMessageEncoder encoder;

    unit_placement_matrix.reset( location );

    for ( unit_type_index = 0; unit_type_index < UnitProfileInterface::getNumUnitTypes(); unit_type_index++ )
    {

        unit_spawn_count = unit_config.getSpawnUnitCount( unit_type_index );
        for ( unit_spawn_index = 0; unit_spawn_index < unit_spawn_count; unit_spawn_index++ )
        {
            unit_placement_matrix.getNextEmptyLoc( &next_loc );
            unit = createUnit(unit_type_index, next_loc, player_id);

            assert(unit != 0);
            UnitRemoteCreate create_mesg(unit->player->getID(), unit->id,
                    next_loc.x, next_loc.y, unit->unit_state.unit_type);
            encoder.encodeMessage(&create_mesg, sizeof(create_mesg));
        } // ** for unit_spawn_index
    } // ** for unit_type_index

    encoder.sendEncodedMessage();
}

// ******************************************************************

void
UnitInterface::queryUnitsAt(std::vector<UnitID>& working_list,
        const iXY& point, Uint16 player_id, unsigned char search_flags)
{
    UnitList & ubl = unit_bucket_array.getBucketAssocWorldLoc(point);
    for(UnitList::iterator i = ubl.begin(); i != ubl.end(); ++i)
    {
        Unit* unit = *i;
        if(!unit->unit_state.bounds(point))
            continue;

        if(search_flags == _search_exclude_player
                && unit->player->getID() == player_id)
            continue;
        if(search_flags == _search_player
                && unit->player->getID() != player_id)
            continue;

        working_list.push_back(unit->id);
    }
}

// ******************************************************************

void
UnitInterface::queryUnitsAt(std::vector<UnitID>& working_list,
        const iRect& rect, Uint16 player_id, unsigned char search_flags)
{
    for(Units::iterator i = units.begin(); i != units.end(); ++i) {
        Unit* unit = i->second;
        if(!rect.contains(unit->unit_state.location))
            continue;

        if(search_flags == _search_exclude_player
                && unit->player->getID() == player_id)
            continue;
        if(search_flags == _search_player
                && unit->player->getID() != player_id)
            continue;

        working_list.push_back(unit->id);
    }
}

/****************************************************************************/

bool UnitInterface::queryClosestUnit( Unit **closest_unit_ptr,
                                       iXY &loc, Uint16 player_id,
                                       unsigned char search_flags )
{
    long closest_magnitude = 0;
    Unit* closest_unit = 0;

    for(Units::iterator i = units.begin(); i != units.end(); ++i) {
        Unit* unit = i->second;

        if(search_flags == _search_exclude_player
                && unit->player->getID() == player_id)
            continue;
        if(search_flags == _search_player
                && unit->player->getID() != player_id)
            continue;

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
        }
    }

    if(closest_unit != 0) {
        *closest_unit_ptr = closest_unit;
        return true;
    }

    *closest_unit_ptr = 0;
    return false;
}

bool UnitInterface::queryClosestUnit( Unit **closest_unit_ptr, iRect &bounding_rect, iXY &loc )
{
    Unit *closest_unit = 0;
    long closest_magnitude = 0;
    iRect bucket_rect;
    UnitList::iterator bucket_iter;

    unit_bucket_array.worldRectToBucketRect( bounding_rect, bucket_rect );

    for( long row_index = bucket_rect.min.y; row_index <= bucket_rect.max.y; row_index++ )
    {
        for( long column_index = bucket_rect.min.x; column_index <= bucket_rect.max.x; column_index++ )
        {
            UnitList &bucket_list = unit_bucket_array.getBucket( row_index, column_index );

            for ( bucket_iter = bucket_list.begin();
                    bucket_iter != bucket_list.end(); ++bucket_iter)
            {
                iXY delta;
                long temp_mag;

                if ( closest_unit == 0 )
                {
                    closest_unit = *bucket_iter;
                    delta  = loc - (*bucket_iter)->unit_state.location;
                    closest_magnitude = long(delta.mag2());
                }
                else
                {
                    delta  = loc - (*bucket_iter)->unit_state.location;
                    temp_mag = long(delta.mag2());

                    if ( closest_magnitude > temp_mag )
                    {
                        closest_unit = *bucket_iter;
                        closest_magnitude = temp_mag;
                    }
                }
            }
        }
    }

    if( closest_unit != 0 )
    {
        *closest_unit_ptr = closest_unit;
        return true;
    }

    *closest_unit_ptr = 0;
    return false;
}

// ******************************************************************

bool UnitInterface::queryClosestEnemyUnit(Unit **closest_unit_ptr,
        iXY &loc, Uint16 player_index)
{
    Unit *closest_unit = 0;
    long closest_magnitude = 0;

    for(Units::iterator i = units.begin(); i != units.end(); ++i) {
        Unit* unit = i->second;
        Uint16 unitPlayerID = unit->player->getID();
        
        if(unitPlayerID == player_index
                || PlayerInterface::isAllied(player_index, unitPlayerID))
            continue;

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
        }
    }

    if( closest_unit != 0 ) {
        *closest_unit_ptr = closest_unit;
        return true;
    }

    *closest_unit_ptr = 0;
    return false;
}

// ******************************************************************

unsigned char UnitInterface::queryUnitLocationStatus(iXY loc)
{
    Uint16 player_id = PlayerInterface::getLocalPlayerIndex();

    std::vector<UnitID> locUnits;
    queryUnitsAt(locUnits, loc, player_id, 0);
    if(locUnits.size() == 0) {
        return _no_unit_found;
    }

    UnitID id = locUnits[0];
    Unit* unit = getUnit(id);
    if(!unit) {
        return _no_unit_found;
    }
    if(unit->player->getID() == player_id) {
        return _unit_player;
    }
    if(PlayerInterface::isAllied(player_id, unit->player->getID())) {
        return _unit_allied;
    }

    return _unit_enemy;
}

// ******************************************************************

bool UnitInterface::queryUnitAtMapLoc(iXY map_loc, UnitID *queary_unit_id)
{
    iXY unit_map_loc;

    for(Units::iterator i = units.begin(); i != units.end(); ++i) {
        Unit* unit = i->second;
        UnitState* unit_state = & unit->unit_state;
            
        MapInterface::pointXYtoMapXY( unit_state->location, &unit_map_loc );
        if( map_loc == unit_map_loc ) {
            *queary_unit_id = unit->id;
            return true;
        }
    }

    return false;
}

// ******************************************************************

void UnitInterface::processManagerMessage(const UnitMessage* message)
{
    switch(message->message_id)
    {
        case _umesg_end_lifecycle:
            unitManagerMesgEndLifecycle(message);
            break;
        default:
            LOGGER.warning("Unknown unit Manage Message type (%u).",
                    message->message_id);
#ifdef DEBUG
            assert(false);
#endif
    }
}

// ******************************************************************

void UnitInterface::unitManagerMesgEndLifecycle(const UnitMessage* message)
{
    const UMesgEndLifeCycleUpdate *lifecycle_update
        = (const UMesgEndLifeCycleUpdate *) message;

    Unit* unit1 = getUnit(lifecycle_update->getDestroyer());
    Unit* unit2 = getUnit(lifecycle_update->getDestroyed());
    if(unit1 == 0 || unit2 == 0) {
        LOGGER.warning(
                "Unit in EndLifeCycle message doesn't exist anymore"
                "(u1: %u u2: %u)", lifecycle_update->getDestroyer(),
                lifecycle_update->getDestroyed());
        return;
    }
    PlayerState* player1 = unit1->player;
    PlayerState* player2 = unit2->player;

    int unittype1 = unit1->unit_state.unit_type;
    const std::string& unitname1 = 
        UnitProfileInterface::getUnitProfile(unittype1)->unitname;
    int unittype2 = unit2->unit_state.unit_type;
    const std::string& unitname2 =
        UnitProfileInterface::getUnitProfile(unittype2)->unitname;
    if(Console::server) {
        *Console::server << "'" << player1->getName() << "' killed a '"
            << unitname2 << "' from '" << player2->getName() 
            << "' with his '" << unitname1 << "'." << std::endl;
    }

    // killing own units doesn't give score
    if(player1 != player2) {
        PlayerInterface::setKill(unit1->player, unit2->player,
                (UnitType) lifecycle_update->unit_type);
        
        PlayerScoreUpdate score_update;
        score_update.set(player1->getID(), player2->getID(),
                (UnitType) lifecycle_update->unit_type);
        NetworkServer::broadcastMessage(&score_update, sizeof(PlayerScoreUpdate));
    }
}


// ******************************************************************

void UnitInterface::unitSyncMessage(const NetMessage *net_message)
{
    const UnitIniSyncMessage* sync_message 
        = (const UnitIniSyncMessage *) net_message;

    try
    {
        std::map<UnitID, Unit*>::iterator uit = units.find(sync_message->getUnitID());
        if ( uit != units.end() )
        {
            LOGGER.warning("UnitInterface::unitSyncMessage() Received an existing unit [%d]",
                            sync_message->getUnitID());
            return;
        }
        Unit* unit = newUnit(sync_message->unit_type,
                iXY(sync_message->getLocX(), sync_message->getLocY()),
                sync_message->getPlayerID(), sync_message->getUnitID());
        unit->in_sync_flag = false;
        addNewUnit(unit);
    }
    catch(std::exception& e)
    {
        LOGGER.warning("UnitInterface::unitSyncMessage() Couldn't sync unit '%s'", e.what());
    }
}

// ******************************************************************

void UnitInterface::unitOpcodeMessage(const NetMessage *net_message)
{
    UnitOpcodeDecoder decoder;
    decoder.setMessage(net_message);

    UnitOpcode* opcode;
    while(decoder.decode(&opcode)) {
        Unit* unit = getUnit(opcode->getUnitID());

        if(!unit) {
            LOGGER.debug("Update for non-existant unit: %d",
                    opcode->getUnitID());
            continue;
        }
        
        unit->evalCommandOpcode(opcode);
    }
}

// ******************************************************************

void UnitInterface::unitDestroyMessage(const NetMessage *net_message)
{
    const UnitRemoteDestroy* remote_destroy 
        = (const UnitRemoteDestroy *) net_message;

    Units::iterator i = units.find(remote_destroy->getUnitToDestroy());
    if(i != units.end())
    {
        removeUnit(i);
    }
}

// ******************************************************************

void UnitInterface::unitCreateMessage(const NetMessage* net_message)
{
    const UnitRemoteCreate* create_mesg 
        = (const UnitRemoteCreate *) net_message;

    Uint16 player_index = create_mesg->getPlayerID();

    try
    {
        std::map<UnitID, Unit*>::iterator uit = units.find(create_mesg->getUnitID());
        if ( uit != units.end() )
        {
            LOGGER.warning("UnitInterface::unitCreateMessage() Received an existing unit [%d]",
                            create_mesg->getUnitID());
            return;
        }
        iXY unitpos(create_mesg->getLocX(), create_mesg->getLocY());
        Unit* unit = newUnit(create_mesg->unit_type, unitpos,
                                 player_index, create_mesg->getUnitID());
        addNewUnit(unit);
        // remove unit from blackboard in client (we are client here)
        UnitBlackBoard::unmarkUnitLoc( unitpos );
    }
    catch(std::exception& e)
    {
        LOGGER.warning("UnitInterface::unitCreateMessage() Couldn't create new unit '%s'", e.what());
    }
}

// ******************************************************************

void UnitInterface::unitSyncIntegrityCheckMessage(const NetMessage* )
{
    unit_bucket_array.sort();
}

// ******************************************************************
void UnitInterface::processNetMessage(const NetMessage* net_message)
{
    switch(net_message->message_id)
    {
        case _net_message_id_ini_sync_mesg:
            unitSyncMessage(net_message);
            break;

        case _net_message_id_opcode_mesg:
            unitOpcodeMessage(net_message);
            break;

        case _net_message_id_destroy_unit:
            unitDestroyMessage(net_message);
            break;

        case _net_message_id_create_unit:
            unitCreateMessage(net_message);
            break;

        case _net_message_id_unit_sync_integrity_check:
            unitSyncIntegrityCheckMessage(net_message);
            break;

        default:
            LOGGER.warning("Unknown message id in UnitMessage (%d)",
                    net_message->message_id);
#ifdef DEBUG
            assert(false);
#endif
            break;
    }
}

// ******************************************************************

void UnitInterface::destroyPlayerUnits(Uint16 player_id)
{
    UMesgSelfDestruct self_destruct;
    self_destruct.setHeader(0, _umesg_flag_unique);

    UnitList& unitlist = playerUnitLists[player_id];
    for(UnitList::iterator i = unitlist.begin(); i != unitlist.end(); ++i)
    {
        Unit* unit = *i;
        unit->processMessage(&self_destruct);
    }
}

