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


#include <iostream>
#include <algorithm>
#include <cassert>

#include "Core/GlobalEngineState.hpp"
#include "Core/GlobalGameState.hpp"

#include "UnitInterface.hpp"
#include "UnitProfileInterface.hpp"
#include "UnitBucketArray.hpp"
#include "UnitOpcodeDecoder.hpp"
#include "Unit.hpp"

#include "Classes/PlacementMatrix.hpp"
#include "Classes/PlayerUnitConfig.hpp"
#include "Classes/TileSet.hpp"

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

BitArray*                      UnitInterface::unit_black_board = 0;
UnitInterface::Units*          UnitInterface::units = 0;
UnitInterface::PlayerUnitList* UnitInterface::playerUnitLists = 0;

UnitOpcodeEncoder* UnitInterface::opcode_encoder = 0;

// ******************************************************************

static unsigned int units_per_player = 0;

static Timer* message_timer = 0;
static Timer* no_guarantee_message_timer = 0;


// ******************************************************************

static UnitID lastUnitID = 0;

UnitID
newUnitID()
{
    UnitID newID = lastUnitID++;
    while( UnitInterface::getUnit(newID) != 0 )
    {
        newID = lastUnitID++;
    }

    return newID;
}

void
UnitInterface::initialize( const unsigned int max_units )
{
    cleanUp();

    unit_black_board = new BitArray(MapInterface::getWidth(),
                                    MapInterface::getHeight());

    UnitBucketArray::initialize();

    units = new Units();

    playerUnitLists = new PlayerUnitList();
    playerUnitLists->resize(PlayerInterface::getMaxPlayers());

    lastUnitID = 0;

    message_timer = new Timer();
    message_timer->changeRate( 8 );

    no_guarantee_message_timer = new Timer();
    no_guarantee_message_timer->changeRate( 15 );

    opcode_encoder = new UnitOpcodeEncoder();

    if ( max_units )
    {
        units_per_player = max_units / PlayerInterface::getMaxPlayers();
    }
    else
    {
        units_per_player = 0;
    }
}

// ******************************************************************

void UnitInterface::cleanUp()
{
    if ( unit_black_board )
    {
        delete unit_black_board;
        unit_black_board = 0;
    }

    UnitBucketArray::cleanUp();

    if ( playerUnitLists )
    {
        delete playerUnitLists;
        playerUnitLists = 0;
    }

    if ( units )
    {
        for(Units::iterator i = units->begin(); i != units->end(); ++i)
        {
            delete i->second;
        }

        delete units;
        units = 0;
    }

    if ( message_timer )
    {
        delete message_timer;
        message_timer = 0;
    }

    if ( no_guarantee_message_timer )
    {
        delete no_guarantee_message_timer;
        no_guarantee_message_timer = 0;
    }

    if ( opcode_encoder )
    {
        delete opcode_encoder;
        opcode_encoder = 0;
    }

}

// ******************************************************************

void
UnitInterface::reset()
{
    cleanUp();
    initialize(gameconfig->maxunits);
}

// ******************************************************************

void
UnitInterface::updateUnitStatus()
{
    iXY prev_unit_loc;
//    iXY unit_map_loc;

    for(Units::iterator i = units->begin(); i != units->end(); /*nothing*/ )
    {
        Unit* unit = i->second;

        if (unit->unit_state.lifecycle_state == _UNIT_LIFECYCLE_INACTIVE)
        {
            removeUnit(i++);
            continue;
        }

        ++i;

        prev_unit_loc = unit->unit_state.location;

        unit->updateState();

        if ( prev_unit_loc != unit->unit_state.location )
        {
            UnitBucketArray::moveUnit( unit, prev_unit_loc);
        }
    }

    if ( NetworkState::status == _network_state_server )
    {
        if (message_timer->count())
        {
            opcode_encoder->send();
        }
    }
}

// ******************************************************************

void
UnitInterface::offloadGraphics( SpriteSorter& sorter )
{
    Unit * unit = 0;

    std::vector<Unit*> unit_list;
    UnitBucketArray::queryUnitsInWorldRectBuckets(unit_list, sorter.getWorldWindow());

    std::vector<Unit*>::iterator i = unit_list.begin();
    while ( i != unit_list.end() )
    {
        unit = *i;
        ++i;

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

// ******************************************************************

Unit*
UnitInterface::createUnit( const unsigned short unit_type,
                           const iXY &location,
                           const Uint16 player_id )
{
    if ((*playerUnitLists)[player_id].size() >= units_per_player)
    {
        return 0;
    }

    Unit* unit = newUnit(unit_type, location, player_id, newUnitID());
    addUnit(unit);

    return unit;
}

// ******************************************************************

void
UnitInterface::spawnPlayerUnits( const iXY &location,
                                 const Uint16 player_id,
                                 const PlayerUnitConfig &unit_config )
{
    iXY next_loc;
    Unit *unit;
    unsigned int utype;
    unsigned int numunits;

    NetMessageEncoder encoder;

    PlacementMatrix::reset( location );

    LOGGER.warning("Spawning player: %d", player_id);

    for ( utype = 0; utype < global_game_state->unit_profile_interface->getNumUnitTypes(); ++utype )
    {
        numunits = unit_config.getSpawnUnitCount( utype );
        while ( numunits )
        {
            --numunits;

            PlacementMatrix::getNextEmptyLoc( &next_loc );
            unit = createUnit(utype, next_loc, player_id);

            if ( unit )
            {
                UnitRemoteCreate create_mesg(unit->player->getID(), unit->id,
                            next_loc.x, next_loc.y, unit->unit_state.unit_type);

                encoder.encodeMessage(&create_mesg, sizeof(create_mesg));
            }
        }
    }

    encoder.sendEncodedMessage();
}

// ******************************************************************

void
UnitInterface::destroyPlayerUnits( const Uint16 player_id )
{
    UMesgSelfDestruct self_destruct;
    self_destruct.setHeader(0);

    const UnitList* unitlist = getPlayerUnits(player_id);
    if ( unitlist )
    {
        for(UnitList::const_iterator i = unitlist->begin(); i != unitlist->end(); ++i)
        {
            Unit* unit = *i;
            unit->processMessage(&self_destruct);
        }
    }
}

// ******************************************************************

Unit *
UnitInterface::newUnit( const unsigned short unit_type,
                        const iXY &location,
                        const unsigned short player_index,
                        const UnitID id)
{
    Unit* unit = 0;
    unsigned char unit_flag;

    PlayerState* player = PlayerInterface::getPlayer( player_index );
    unit_flag = player->getFlag();

    if ( unit_type < global_game_state->unit_profile_interface->getNumUnitTypes() )
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

void
UnitInterface::addUnit( Unit *unit )
{
    units->insert(std::make_pair(unit->id, unit));
    (*playerUnitLists)[unit->player->getID()].push_back(unit);
    iXY map_location;
    MapInterface::pointXYtoMapXY(unit->unit_state.location, &map_location);
    markUnitLoc(map_location);
    UnitBucketArray::addUnit(unit);
}

// ******************************************************************

void
UnitInterface::removeUnit( Units::iterator i )
{
    Unit* unit = i->second;

    // unit explosion particles
    ParticleInterface::addHit(unit->unit_state);

    // unit explosion sound
    global_engine_state->sound_manager->playAmbientSound("expl",
            WorldViewInterface::getCameraDistance( unit->unit_state.location ));

    iXY map_location;
    MapInterface::pointXYtoMapXY(unit->unit_state.location, &map_location);
    unmarkUnitLoc(map_location);

    UnitBucketArray::removeUnit(unit);

    UnitList& plist = (*playerUnitLists)[unit->player->getID()];
    plist.erase(std::remove(plist.begin(), plist.end(), unit), plist.end());

    units->erase(i);
    delete unit;
}

// ******************************************************************



// ******************************************************************



// ******************************************************************

// ******************************************************************

void
UnitInterface::processNetPacket( const NetPacket* packet )
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

// ******************************************************************

void
UnitInterface::sendMessage( const UnitMessage* message,
                            const PlayerState* player )
{
    if ( message->message_id == _umesg_weapon_hit )
    {
        std::vector<Unit*> unit_list;
        UnitBucketArray::queryUnitsAtWorldLocBucket(unit_list,
                                ((UMesgWeaponHit*)message)->getHitLocation());
        std::vector<Unit*>::iterator i = unit_list.begin();
        while ( i != unit_list.end() )
        {
            (*i)->processMessage(message);
            ++i;
        }
    }
    else if ( message->message_id == _umesg_end_lifecycle )
    {
        unitManagerMesgEndLifecycle(message);
    }
    else
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
}

// ******************************************************************

void
UnitInterface::processNetMessage( const NetMessage* net_message )
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

void
UnitInterface::unitManagerMesgEndLifecycle( const UnitMessage* message )
{
    const UMesgEndLifeCycleUpdate *lifecycle_update
        = (const UMesgEndLifeCycleUpdate *) message;

    Unit* unit1 = getUnit(lifecycle_update->getDestroyer());
    Unit* unit2 = getUnit(lifecycle_update->getDestroyed());
    
    if(unit1 == 0 || unit2 == 0)
    {
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
        global_game_state->unit_profile_interface->getUnitProfile(unittype1)->unitname;

    int unittype2 = unit2->unit_state.unit_type;
    const std::string& unitname2 =
        global_game_state->unit_profile_interface->getUnitProfile(unittype2)->unitname;

    if(Console::server)
    {
        *Console::server << "'" << player1->getName() << "' killed a '"
            << unitname2 << "' from '" << player2->getName()
            << "' with his '" << unitname1 << "'." << std::endl;
    }

    // killing own units doesn't give score
    if(player1 != player2)
    {
        PlayerInterface::setKill(unit1->player, unit2->player,
                (UnitType) lifecycle_update->unit_type);

        PlayerScoreUpdate score_update;
        score_update.set(player1->getID(), player2->getID(),
                (UnitType) lifecycle_update->unit_type);
        NetworkServer::broadcastMessage(&score_update, sizeof(PlayerScoreUpdate));
    }
}

// ******************************************************************

void
UnitInterface::unitCreateMessage( const NetMessage* net_message )
{
    const UnitRemoteCreate* create_mesg
        = (const UnitRemoteCreate *) net_message;

    Uint16 player_index = create_mesg->getPlayerID();

    try
    {
        std::map<UnitID, Unit*>::iterator uit = units->find(create_mesg->getUnitID());
        if ( uit != units->end() )
        {
            LOGGER.warning("UnitInterface::unitCreateMessage() Received an existing unit [%d]",
                            create_mesg->getUnitID());
            return;
        }
        iXY unitpos(create_mesg->getLocX(), create_mesg->getLocY());
        Unit* unit = newUnit(create_mesg->unit_type, unitpos,
                                 player_index, create_mesg->getUnitID());
        addUnit(unit);
        // remove unit from blackboard in client (we are client here)
        unmarkUnitLoc( unitpos );
    }
    catch(std::exception& e)
    {
        LOGGER.warning("UnitInterface::unitCreateMessage() Couldn't create new unit '%s'", e.what());
    }
}

// ******************************************************************

void
UnitInterface::unitDestroyMessage( const NetMessage *net_message )
{
    const UnitRemoteDestroy* remote_destroy
        = (const UnitRemoteDestroy *) net_message;

    Units::iterator i = units->find(remote_destroy->getUnitToDestroy());
    if(i != units->end())
    {
        removeUnit(i);
    }
}

// ******************************************************************

void
UnitInterface::unitSyncMessage( const NetMessage *net_message )
{
    const UnitIniSyncMessage* sync_message 
        = (const UnitIniSyncMessage *) net_message;

    try
    {
        std::map<UnitID, Unit*>::iterator uit = units->find(sync_message->getUnitID());
        if ( uit != units->end() )
        {
            LOGGER.warning("UnitInterface::unitSyncMessage() Received an existing unit [%d]",
                            sync_message->getUnitID());
            return;
        }
        Unit* unit = newUnit(sync_message->unit_type,
                iXY(sync_message->getLocX(), sync_message->getLocY()),
                sync_message->getPlayerID(), sync_message->getUnitID());
        unit->in_sync_flag = false;
        addUnit(unit);
    }
    catch(std::exception& e)
    {
        LOGGER.warning("UnitInterface::unitSyncMessage() Couldn't sync unit '%s'", e.what());
    }
}

// ******************************************************************

void
UnitInterface::unitOpcodeMessage( const NetMessage *net_message )
{
    UnitOpcodeDecoder decoder;
    decoder.setMessage(net_message);

    UnitOpcode* opcode;
    while(decoder.decode(&opcode))
    {
        Unit* unit = getUnit(opcode->getUnitID());

        if(!unit)
        {
            LOGGER.debug("Update for non-existant unit: %d",
                    opcode->getUnitID());
            continue;
        }
        
        unit->evalCommandOpcode(opcode);
    }
}

// ******************************************************************

void
UnitInterface::unitSyncIntegrityCheckMessage( const NetMessage* )
{
    UnitBucketArray::sort();
}
