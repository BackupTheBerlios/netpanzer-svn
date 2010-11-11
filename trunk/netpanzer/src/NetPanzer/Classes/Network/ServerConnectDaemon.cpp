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

#include <algorithm>
#include "ServerConnectDaemon.hpp"
#include "NetworkServer.hpp"
#include "NetMessage.hpp"
#include "NetMessageEncoder.hpp"
#include "PlayerNetMessage.hpp"
#include "ConnectNetMessage.hpp"
#include "SystemNetMessage.hpp"

#include "Objectives/ObjectiveInterface.hpp"
#include "Resources/ResourceManager.hpp"
#include "PowerUps/PowerUpInterface.hpp"
#include "Units/UnitInterface.hpp"
#include "Core/NetworkGlobals.hpp"
#include "Util/Log.hpp"

#include "Interfaces/ConsoleInterface.hpp"
#include "Interfaces/ChatInterface.hpp"
#include "Interfaces/PlayerInterface.hpp"
#include "Interfaces/MapInterface.hpp"
#include "Interfaces/GameManager.hpp"
#include "Interfaces/GameConfig.hpp"

enum ConnectionState
{
    connect_state_idle,
    connect_state_wait_for_connect_request,
    connect_state_wait_for_client_settings,
    connect_state_wait_for_client_game_setup_ack,
    connect_state_player_state_sync,
    connect_state_sync_flags,
    connect_state_unit_sync
 };


ServerConnectDaemon::ConnectQueue ServerConnectDaemon::connect_queue;

int                 ServerConnectDaemon::connection_state = connect_state_idle;
bool                ServerConnectDaemon::connection_lock_state = false;
ClientSocket      * ServerConnectDaemon::connect_client;
Timer		    ServerConnectDaemon::time_out_timer;
int	            ServerConnectDaemon::time_out_counter;
Timer               ServerConnectDaemon::sendunitpercent_timer;
int                 ServerConnectDaemon::sync_count = 0;
int                 ServerConnectDaemon::sync_end = 0;
int                 ServerConnectDaemon::sync_total = 0;
int                 ServerConnectDaemon::sync_done = 0;

#define _SERVER_CONNECT_TIME_OUT_TIME (20.0)
#define _SERVER_CONNECT_RETRY_LIMIT   (5)
#define _SENDUNITPERCENT_TIME (1)


void ServerConnectDaemon::initialize(unsigned long max_players)
{
    (void) max_players;
    time_out_timer.changePeriod( _SERVER_CONNECT_TIME_OUT_TIME );
    sendunitpercent_timer.changePeriod( _SENDUNITPERCENT_TIME );
}

void ServerConnectDaemon::startConnectDaemon( unsigned long max_players )
{
    (void) max_players;
    time_out_timer.changePeriod( _SERVER_CONNECT_TIME_OUT_TIME );
    sendunitpercent_timer.changePeriod( _SENDUNITPERCENT_TIME );
}

void ServerConnectDaemon::shutdownConnectDaemon()
{
    ConnectMesgNetPanzerServerDisconnect server_disconnect;

    SERVER->broadcastMessage( &server_disconnect,
                         sizeof(ConnectMesgNetPanzerServerDisconnect));
}

bool ServerConnectDaemon::inConnectQueue( ClientSocket *client )
{
    if ( (connection_state != connect_state_idle) &&
         (connect_client == client)
       )
    {
        return true;
    }
    
    ConnectQueueIterator i;
    i = std::find(connect_queue.begin(), connect_queue.end(), client);
    if ( i != connect_queue.end() )
    {
        return true;
    }
    
    return false;
}

void ServerConnectDaemon::updateQueuedClients()
{
    unsigned long queue_position = 1;
    ConnectProcessUpdate process_update;

    ConnectQueueIterator i;
    for(i = connect_queue.begin(); i != connect_queue.end(); ++i)
    {
        process_update.setQueuePosition(queue_position);

        (*i)->sendMessage( &process_update, sizeof(ConnectProcessUpdate));
        (*i)->sendRemaining();
        queue_position++;
    }

}

void ServerConnectDaemon::netPacketClientDisconnect(const NetPacket *packet)
{
    // nothing, useless, remove
}

void ServerConnectDaemon::netPacketClientJoinRequest(const NetPacket* packet)
{
    const ClientConnectJoinRequest* join_request_mesg
        = (const ClientConnectJoinRequest *) packet->getNetMessage();
    ClientConnectJoinRequestAck join_request_ack;

    join_request_ack.setResultCode(_join_request_result_success);

    if ( join_request_mesg->getProtocolVersion() != NETPANZER_PROTOCOL_VERSION )
    {
        join_request_ack.setResultCode(_join_request_result_invalid_protocol);
    }
    else
    {
        if ( !inConnectQueue( packet->fromClient ) )
        {
            if (connect_queue.size() > 25)
            {
                join_request_ack.setResultCode(_join_request_result_server_busy);
            }
            else
            {
                connect_queue.push_back(packet->fromClient);
            }
        }
    }

    join_request_ack.setServerProtocolVersion(NETPANZER_PROTOCOL_VERSION);
    packet->fromClient->sendMessage(&join_request_ack,
                         sizeof(ClientConnectJoinRequestAck));
    packet->fromClient->sendRemaining();
}

void ServerConnectDaemon::processNetPacket(const NetPacket* packet)
{
    const NetMessage* message = packet->getNetMessage();

    switch(message->message_id) {
        case _net_message_id_connect_netPanzer_client_disconnect:
            netPacketClientDisconnect(packet);
            break;

        case _net_message_id_connect_join_game_request:
            netPacketClientJoinRequest(packet);
            break;

        default :
            connectFsm(message);
            break;
    }
}

void ServerConnectDaemon::sendConnectionAlert(ClientSocket * client)
{
    SystemConnectAlert connect_alert;
    
    PlayerState *player_state = 0;

    player_state = PlayerInterface::getPlayer( client->getPlayerIndex() );

    connect_alert.set( client->getPlayerIndex(), _connect_alert_mesg_connect );
    
    ConsoleInterface::postMessage(Color::cyan, true, player_state->getFlag(), 
                                  "'%s' [%s] has joined the game.",
                                  player_state->getName().c_str(),
                                  client->getIPAddress().c_str() );

    if ( ((std::string)gameconfig->motd).length() > 0 )
    {
    	ChatInterface::serversayTo(client->getPlayerIndex(), gameconfig->motd.c_str());
    }

    SERVER->broadcastMessage( &connect_alert, sizeof(SystemConnectAlert));
}

void ServerConnectDaemon::resetConnectFsm()
{
    connection_state = connect_state_idle;
    time_out_timer.reset();
    time_out_counter = 0;
}


void ServerConnectDaemon::connectStateIdle()
{
    if (!connect_queue.empty() && (connection_lock_state == false))
    {
        connect_client = connect_queue.front();
        connect_queue.pop_front();
        connection_state  = connect_state_wait_for_connect_request;

        ClientConnectStartConnect start_connect;
        
        connect_client->sendMessage( &start_connect,
                                     sizeof(ClientConnectStartConnect));
        connect_client->sendRemaining();
        time_out_timer.changePeriod( _SERVER_CONNECT_TIME_OUT_TIME );
        time_out_timer.reset();
        time_out_counter = 0;

        updateQueuedClients();
    }
}


void ServerConnectDaemon::connectStateWaitForConnectRequest(
        const NetMessage* message)
{
    if (message && message->message_id == _net_message_id_client_connect_request )
    {

        ClientConnectResult connect_result;
        PlayerState * player = PlayerInterface::allocateNewPlayer();

        if ( player == 0 )
        {
            connect_result.result_code = _connect_result_server_full;
            resetConnectFsm();
        }
        else
        {
            connect_result.result_code = _connect_result_success;
            time_out_timer.reset();
            connect_client->player_id = player->getID();
        }
        connect_client->sendMessage( &connect_result,
                                     sizeof(ClientConnectResult));
        connect_client->sendRemaining();
        if ( connection_state != connect_state_idle )
        {
            connection_state = connect_state_wait_for_client_settings;
        }
    }
    else if ( time_out_timer.count() )
    {
        resetConnectFsm();
    }
}

void ServerConnectDaemon::connectStateWaitForClientSettings(
        const NetMessage* message )
{
    PlayerState * player = PlayerInterface::getPlayer(connect_client->getPlayerIndex());
    
    if ( message && message->message_id == _net_message_id_connect_client_settings )
    {
            ConnectClientSettings *client_setting;

            client_setting = (ConnectClientSettings *) message;
            player->setName( client_setting->player_name );
            ResourceManager::getFlag(connect_client->getPlayerIndex())->bufferToFrame(client_setting->player_flag, sizeof(ConnectClientSettings::player_flag));
            player->setStatus( _player_state_connecting );

            // ** send server game setting map, units, player, etc.
            ConnectMesgServerGameSettings* server_game_setup
                = GameManager::getServerGameSetup();
            connect_client->sendMessage( server_game_setup,
                                         sizeof(ConnectMesgServerGameSettings));
            connect_client->sendRemaining();
            delete server_game_setup;

            time_out_timer.reset();
            if(connection_state != connect_state_idle)
            {
                connection_state = connect_state_wait_for_client_game_setup_ack;
            }
    }
    else if ( time_out_timer.count() )
    {
        player->setStatus( _player_state_free );
        resetConnectFsm();
    }
}

void ServerConnectDaemon::connectStateWaitForClientGameSetupAck(
        const NetMessage* message )
{
    PlayerState * player = PlayerInterface::getPlayer(connect_client->getPlayerIndex());
    if ( message != 0 )
    {
        if ( message->message_id == _net_message_id_connect_client_game_setup_ack )
        {
            PlayerConnectID player_connect_mesg(connect_client->getPlayerIndex());
            connect_client->sendMessage( &player_connect_mesg,
                                         sizeof(PlayerConnectID));
            
            sync_count = 0;
            sync_end = PlayerInterface::getMaxPlayers();

            ConnectProcessStateMessage state_mesg;
            state_mesg.setMessageEnum(_connect_state_message_sync_player_info);
            state_mesg.setPercentComplete(0);
            connect_client->sendMessage( &state_mesg,
                                         sizeof(ConnectProcessStateMessage));
            connect_client->sendRemaining();
            if(connection_state != connect_state_idle)
            {
                connection_state = connect_state_player_state_sync;
            }
        }
        else if ( message->message_id == _net_message_id_connect_client_game_setup_ping )
        {
            time_out_timer.reset();
        }
    }
    else if ( time_out_timer.count() )
    {
        player->setStatus( _player_state_free );
        resetConnectFsm();
    }
}

void ServerConnectDaemon::connectStatePlayerStateSync()
{
    ConnectProcessStateMessage state_mesg;

    NetMessageEncoder encoder;
    PlayerStateSync pss;

    do
    {
        PlayerInterface::getPlayer(sync_count)->getNetworkPlayerState(pss.player_state);
        if ( ! encoder.encodeMessage(&pss, sizeof(pss)) )
        {
            break;
        }
    } while ( ++sync_count < sync_end );

    // here encoder *always* has something
    connect_client->sendMessage(encoder.getEncodedMessage(),
                                encoder.getEncodedLen());
    
    state_mesg.setMessageEnum(_connect_state_message_sync_player_info_percent);
    state_mesg.setPercentComplete((sync_count*100)/sync_end);

    connect_client->sendMessage( &state_mesg, sizeof(state_mesg));
    connect_client->sendRemaining();

    if ( sync_count == sync_end )
    {
        sync_count = 0;
        sync_done = 0;
        sync_total = PlayerInterface::countPlayers();
        // sync_end = current value = max players;
        
        connection_state = connect_state_sync_flags;
    }
}

void ServerConnectDaemon::connectStateSyncFlags()
{
    ConnectProcessStateMessage state_mesg;

    NetMessageEncoder encoder;

    while ( sync_count < sync_end && ! PlayerInterface::isPlayerActive(sync_count) )
    {
        sync_count += 1;
    }

    if ( sync_count < sync_end && PlayerInterface::isPlayerActive(sync_count) )
    {
        PlayerFlagSync pfs;
        pfs.player_id = sync_count;
        ResourceManager::getFlag(sync_count)->frameToBuffer(pfs.player_flag, sizeof(pfs.player_flag));
        connect_client->sendMessage(&pfs, sizeof(pfs));
        sync_done += 1;
        sync_count += 1;
    }

    if ( sync_done == sync_total )
    {
        sync_count = 0;
        sync_done = 0;
        sync_end = 0;
        sync_total = UnitInterface::getTotalUnitCount();

        if ( sync_total )
        {
            sync_end = UnitInterface::getUnits().rbegin()->first;
        }

        sync_end += 1;

        sendunitpercent_timer.reset();

        state_mesg.setMessageEnum(_connect_state_message_sync_units);

        // size of message already set
        connect_client->sendMessage( &state_mesg,
                                     sizeof(ConnectProcessStateMessage));

        SERVER->addClientToSendList( connect_client );

        PlayerState *p = PlayerInterface::getPlayer(connect_client->getPlayerIndex());
        PlayerStateSync player_state_update( p->getNetworkPlayerState() );
        SERVER->broadcastMessage(&player_state_update, sizeof(PlayerStateSync));

        if(connection_state != connect_state_idle)
        {
            connection_state = connect_state_unit_sync;
        }
    }
}

void ServerConnectDaemon::connectStateUnitSync()
{
    ConnectProcessStateMessage state_mesg;
    
    NetMessageEncoder encoder;
    UnitInterface::Units::const_iterator i;
    iXY unit_map_loc;
    UnitBase* unit;

    do
    {
        i = UnitInterface::getUnits().lower_bound(sync_count);
        if ( i == UnitInterface::getUnits().end() || i->first >= sync_end )
        {
            sync_count = sync_end;
            sync_done = sync_total;
            break;
        }

        unit = i->second;
        MapInterface::pointXYtoMapXY(unit->unit_state.location, &unit_map_loc);
        UnitRemoteCreate urc(unit->player->getID(),
                             unit->id,
                             unit_map_loc.x,
                             unit_map_loc.y,
                             unit->unit_state.unit_type);

        if ( ! encoder.encodeMessage(&urc, sizeof(urc)) )
        {
            break;
        }

        sync_done += 1;
        sync_count = i->first + 1;

    } while ( sync_count < sync_end );

    // here encoder *always* has something
    connect_client->sendMessage(encoder.getEncodedMessage(),
                                encoder.getEncodedLen());

    if ( sync_count < sync_end )
    {
        if ( sendunitpercent_timer.count() )
        {
            state_mesg.setMessageEnum(_connect_state_message_sync_units_percent);
            state_mesg.setPercentComplete((sync_done*100)/sync_total);
            connect_client->sendMessage( &state_mesg, sizeof(state_mesg));
            sendunitpercent_timer.reset();
        }
        return;
    }

    // Sending finished
    state_mesg.setMessageEnum(_connect_state_message_sync_units_percent);
    state_mesg.setPercentComplete(100);
    connect_client->sendMessage( &state_mesg, sizeof(state_mesg));

    UnitSyncIntegrityCheck unit_integrity_check_mesg;
    connect_client->sendMessage( &unit_integrity_check_mesg,
                                 sizeof(UnitSyncIntegrityCheck));

    ObjectiveInterface::syncObjectives( connect_client );

    PowerUpInterface::syncPowerUps( connect_client );

    //GameManager::spawnPlayer( connect_client->getPlayerIndex() );

    PlayerState * player = PlayerInterface::getPlayer(connect_client->getPlayerIndex());

    player->setStatus( _player_state_active );

    PlayerFlagSync pfs;
    pfs.player_id = connect_client->player_id;
    ResourceManager::getFlag(connect_client->player_id)->frameToBuffer(pfs.player_flag, sizeof(pfs.player_flag));
    SERVER->broadcastMessage(&pfs, sizeof(pfs));

    PlayerStateSync player_state_update
        (player->getNetworkPlayerState());

    SERVER->broadcastMessage( &player_state_update, sizeof(PlayerStateSync));

    state_mesg.setMessageEnum(_connect_state_sync_complete);
    // size already set
    connect_client->sendMessage( &state_mesg,
                                 sizeof(ConnectProcessStateMessage));
    sendConnectionAlert( connect_client );

    connection_state = connect_state_idle;
}

void ServerConnectDaemon::connectFsm(const NetMessage* message)
{
    switch ( connection_state )
    {
        case connect_state_idle:
            connectStateIdle();
            break;

        case connect_state_wait_for_connect_request:
            connectStateWaitForConnectRequest( message );
            break;

        case  connect_state_wait_for_client_settings:
            connectStateWaitForClientSettings( message );
            break;

        case connect_state_wait_for_client_game_setup_ack:
            connectStateWaitForClientGameSetupAck( message );
            break;

        case connect_state_player_state_sync:
            connectStatePlayerStateSync();
            break;

        case connect_state_sync_flags:
            connectStateSyncFlags();
            break;

        case connect_state_unit_sync:
            connectStateUnitSync();
            break;

        default:
            assert("Bad connection state" == 0);
    } // ** switch
}

void ServerConnectDaemon::connectProcess(const NetMessage* message)
{
    connectFsm( message );
}

void ServerConnectDaemon::connectProcess()
{
    connectFsm( 0 );
}

void
ServerConnectDaemon::removeClientFromQueue(ClientSocket *client)
{
    connect_queue.remove(client);
    
    if( connect_client == client )
    {
        connection_state = connect_state_idle;
    }    
}

void ServerConnectDaemon::lockConnectProcess()
{
    connection_lock_state = true;
}

void ServerConnectDaemon::unlockConnectProcess()
{
    connection_lock_state = false;
}

bool ServerConnectDaemon::getConnectLockStatus()
{
    return( connection_lock_state );
}

bool ServerConnectDaemon::isConnecting()
{
    return connection_state != connect_state_idle;
}
