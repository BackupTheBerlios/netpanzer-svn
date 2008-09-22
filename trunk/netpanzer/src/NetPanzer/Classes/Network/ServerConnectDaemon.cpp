
#include "NetMessage.hpp"

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
#include <algorithm>
#include "Classes/Network/ServerConnectDaemon.hpp"
#include "Interfaces/ChatInterface.hpp"
#include "Core/NetworkGlobals.hpp"
#include "Interfaces/PlayerInterface.hpp"
#include "Units/UnitInterface.hpp"
#include "Objectives/ObjectiveInterface.hpp"
#include "PowerUps/PowerUpInterface.hpp"
#include "Interfaces/MapInterface.hpp"
#include "Classes/Network/NetworkServer.hpp"
#include "Interfaces/GameManager.hpp"
#include "Interfaces/GameConfig.hpp"

#include "PlayerNetMessage.hpp"
#include "ConnectNetMessage.hpp"
#include "SystemNetMessage.hpp"

#include "Interfaces/ConsoleInterface.hpp"
#include "Util/Log.hpp"

ServerConnectDaemon::ConnectionState 
        ServerConnectDaemon::connection_state = ServerConnectDaemon::connect_state_idle;

ServerConnectDaemon::ConnectQueue ServerConnectDaemon::connect_queue;

bool                ServerConnectDaemon::connection_lock_state = false;
ClientSocket      * ServerConnectDaemon::connect_client;
Timer		    ServerConnectDaemon::time_out_timer;
int	            ServerConnectDaemon::time_out_counter;
Timer               ServerConnectDaemon::sendunitpercent_timer;
UnitSync*           ServerConnectDaemon::connect_unit_sync = 0;

#define _SERVER_CONNECT_TIME_OUT_TIME (20.0)
#define _SERVER_CONNECT_RETRY_LIMIT   (5)
#define _SENDUNITPERCENT_TIME (1)


void ServerConnectDaemon::initialize(unsigned long max_players)
{
    (void) max_players;
    time_out_timer.changePeriod( _SERVER_CONNECT_TIME_OUT_TIME );
    sendunitpercent_timer.changePeriod( _SENDUNITPERCENT_TIME );
    connect_unit_sync = 0;
}

void ServerConnectDaemon::startConnectDaemon( unsigned long max_players )
{
    (void) max_players;
    time_out_timer.changePeriod( _SERVER_CONNECT_TIME_OUT_TIME );
    sendunitpercent_timer.changePeriod( _SENDUNITPERCENT_TIME );
}

void ServerConnectDaemon::shutdownConnectDaemon()
{
    delete connect_unit_sync;
    connect_unit_sync = 0;
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
    process_update.setSize(sizeof(ConnectProcessUpdate));

    ConnectQueueIterator i;
    for(i = connect_queue.begin(); i != connect_queue.end(); ++i)
    {
        process_update.setQueuePosition(queue_position);

        (*i)->sendMessage( &process_update, sizeof(ConnectProcessUpdate));

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

    if(join_request_mesg->getProtocolVersion() == NETPANZER_PROTOCOL_VERSION)
    {
        join_request_ack.setResultCode(_join_request_result_success);
    }
    else
    {
        join_request_ack.setResultCode(_join_request_result_invalid_protocol);
    }

    join_request_ack.setServerProtocolVersion(NETPANZER_PROTOCOL_VERSION);

    if( join_request_ack.getResultCode() == _join_request_result_success )
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
    join_request_ack.setSize(sizeof(ClientConnectJoinRequestAck));
    packet->fromClient->sendMessage(&join_request_ack,
                         sizeof(ClientConnectJoinRequestAck));
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
        ChatMesg chat_mesg;
        chat_mesg.message_scope=_chat_mesg_scope_server;
        chat_mesg.setSourcePlayerIndex(0);
        snprintf(chat_mesg.message_text, sizeof(chat_mesg.message_text), "%s",gameconfig->motd.c_str());
        chat_mesg.setSize(sizeof(ChatMesg));
        client->sendMessage( &chat_mesg, sizeof(chat_mesg));
    }

    SERVER->broadcastMessage( &connect_alert, sizeof(SystemConnectAlert));
}

void ServerConnectDaemon::resetConnectFsm()
{
    connection_state = connect_state_idle;
    time_out_timer.reset();
    time_out_counter = 0;
}


bool ServerConnectDaemon::connectStateIdle()
{
    if (!connect_queue.empty() && (connection_lock_state == false))
    {
        connect_client = connect_queue.front();
        connect_queue.pop_front();
        connection_state  = connect_state_wait_for_connect_request;

        ClientConnectStartConnect start_connect;
        start_connect.setSize(sizeof(ClientConnectStartConnect));
        
        connect_client->sendMessage( &start_connect,
                                     sizeof(ClientConnectStartConnect));

        time_out_timer.changePeriod( _SERVER_CONNECT_TIME_OUT_TIME );
        time_out_timer.reset();
        time_out_counter = 0;

        updateQueuedClients();
    }

    return true;
}


bool ServerConnectDaemon::connectStateWaitForConnectRequest(
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
            connect_client->playerIndex = player->getID();
        }
        connect_result.setSize(sizeof(ClientConnectResult));
        connect_client->sendMessage( &connect_result,
                                     sizeof(ClientConnectResult));

        if ( connection_state != connect_state_idle )
        {
            connection_state = connect_state_wait_for_client_settings;
        }
    }
    else if ( time_out_timer.count() )
    {
        resetConnectFsm();
    }
    
    return true;
}

bool ServerConnectDaemon::connectStateWaitForClientSettings(
        const NetMessage* message )
{
    PlayerState * player = PlayerInterface::getPlayer(connect_client->getPlayerIndex());
    
    if ( message && message->message_id == _net_message_id_connect_client_settings )
    {
            ConnectClientSettings *client_setting;

            client_setting = (ConnectClientSettings *) message;
            player->setName( client_setting->player_name );
            player->unit_config.setUnitColor( client_setting->unit_color );
            
            Uint8 flag = (Uint8) client_setting->getPlayerFlag();
            player->setFlag(flag);

            player->setID( connect_client->getPlayerIndex() );
            player->setStatus( _player_state_connecting );

            // ** send server game setting map, units, player, etc.
            ConnectMesgServerGameSettings* server_game_setup
                = GameManager::getServerGameSetup();
            server_game_setup->setSize(sizeof(ConnectMesgServerGameSettings));
            connect_client->sendMessage( server_game_setup,
                                         sizeof(ConnectMesgServerGameSettings));
            delete server_game_setup;

            time_out_timer.reset();
            if(connection_state != connect_state_idle)
            {
                connection_state = connect_state_wait_for_client_game_setup_ack;
            }
            return true;
    }

    if ( time_out_timer.count() )
    {
        player->setStatus( _player_state_free );
        resetConnectFsm();
    }

    return true;
}

bool ServerConnectDaemon::connectStateWaitForClientGameSetupAck(
        const NetMessage* message )
{
    PlayerState * player = PlayerInterface::getPlayer(connect_client->getPlayerIndex());
    if ( message != 0 )
    {
        if ( message->message_id == _net_message_id_connect_client_game_setup_ack )
        {
            PlayerConnectID player_connect_mesg(player->getNetworkPlayerState());
            player_connect_mesg.setSize(sizeof(PlayerConnectID));
            connect_client->sendMessage( &player_connect_mesg,
                                         sizeof(PlayerConnectID));
            
            PlayerInterface::startPlayerStateSync( connect_client->getPlayerIndex() );

            ConnectProcessStateMessage state_mesg;
            state_mesg.setMessageEnum(_connect_state_message_sync_player_info);
            state_mesg.setPercentComplete(0);
            state_mesg.setSize(sizeof(ConnectProcessStateMessage));
            connect_client->sendMessage( &state_mesg,
                                         sizeof(ConnectProcessStateMessage));

            if(connection_state != connect_state_idle)
            {
                connection_state = connect_state_player_state_sync;
            }
            
            return true;
        }
        else if ( message->message_id == _net_message_id_connect_client_game_setup_ping )
        {
            time_out_timer.reset();
            return true;
        }
    }

    if ( time_out_timer.count() )
    {
        player->setStatus( _player_state_free );
        resetConnectFsm();
    }

    return true;
}

bool ServerConnectDaemon::connectStatePlayerStateSync()
{
    ConnectProcessStateMessage state_mesg;
    int percent_complete;
    
    unsigned char buffer[_MAX_NET_PACKET_SIZE];
    unsigned int buffer_pos = 0;
    PlayerStateSync msg;
    msg.setSize(sizeof(PlayerStateSync));
    while ( buffer_pos+sizeof(PlayerStateSync) < _MAX_NET_PACKET_SIZE
            && percent_complete != 100)
    {
        if ( PlayerInterface::syncNextPlayerState( msg.player_state, &percent_complete) )
        {
            memcpy(buffer+buffer_pos, &msg, sizeof(PlayerStateSync));
            buffer_pos += sizeof(PlayerStateSync);
        }
    }

    if ( buffer_pos )
    {
        connect_client->sendMessage(buffer, buffer_pos);
    }
    
    state_mesg.setMessageEnum(_connect_state_message_sync_player_info_percent);
    state_mesg.setPercentComplete(percent_complete);
    state_mesg.setSize(sizeof(ConnectProcessStateMessage));
    connect_client->sendMessage( &state_mesg, sizeof(ConnectProcessStateMessage));
    
    if ( percent_complete == 100 )
    {
        delete connect_unit_sync;
        connect_unit_sync = new UnitSync(connect_client);
        sendunitpercent_timer.reset();

        state_mesg.setMessageEnum(_connect_state_message_sync_units);
        
        // size of message already set
        connect_client->sendMessage( &state_mesg,
                                     sizeof(ConnectProcessStateMessage));
        
        SERVER->addClientToSendList( connect_client );

        PlayerState *p = PlayerInterface::getPlayer(connect_client->getPlayerIndex());
        PlayerStateSync player_state_update( p->getNetworkPlayerState() );
        player_state_update.setSize(sizeof(PlayerStateSync));
        SERVER->broadcastMessage(&player_state_update, sizeof(PlayerStateSync));
        
        if(connection_state != connect_state_idle)
        {
            connection_state = connect_state_unit_sync;
        }
    }

    return true;
}

bool ServerConnectDaemon::connectStateUnitSync()
{
    ConnectProcessStateMessage state_mesg;
    state_mesg.setSize(sizeof(ConnectProcessStateMessage));
    
    int percent_complete = connect_unit_sync->getPercentComplete();

    // send a unit
    if(connect_unit_sync->sendNextUnit())
    {
        if ( sendunitpercent_timer.count() )
        {
            state_mesg.setMessageEnum(_connect_state_message_sync_units_percent);
            state_mesg.setPercentComplete(percent_complete);
            // size already set
            connect_client->sendMessage( &state_mesg,
                                         sizeof(ConnectProcessStateMessage));
            sendunitpercent_timer.reset();
        }
        return true;
    }

    // Sending finished
    state_mesg.setMessageEnum(_connect_state_message_sync_units_percent);
    state_mesg.setPercentComplete(100);
    // size already set
    connect_client->sendMessage( &state_mesg,
                                 sizeof(ConnectProcessStateMessage));

    UnitSyncIntegrityCheck unit_integrity_check_mesg;
    unit_integrity_check_mesg.setSize(sizeof(UnitSyncIntegrityCheck));
    connect_client->sendMessage( &unit_integrity_check_mesg,
                                 sizeof(UnitSyncIntegrityCheck));

    ObjectiveInterface::syncObjectives( connect_client );

    PowerUpInterface::syncPowerUps( connect_client );

    GameManager::spawnPlayer( connect_client->getPlayerIndex() );

    PlayerState * player = PlayerInterface::getPlayer(connect_client->getPlayerIndex());

    player->setStatus( _player_state_active );

    PlayerStateSync player_state_update
        (player->getNetworkPlayerState());

    SERVER->broadcastMessage( &player_state_update, sizeof(PlayerStateSync));

    state_mesg.setMessageEnum(_connect_state_sync_complete);
    // size already set
    connect_client->sendMessage( &state_mesg,
                                 sizeof(ConnectProcessStateMessage));
    sendConnectionAlert( connect_client );

    connection_state = connect_state_idle;
    return true;
}

void ServerConnectDaemon::connectFsm(const NetMessage* message)
{
    bool end_cycle = false;

    do {
        switch ( connection_state ) {
            case connect_state_idle:
                end_cycle = connectStateIdle();
                break;

            case connect_state_wait_for_connect_request:
                end_cycle = connectStateWaitForConnectRequest( message );
                break;

            case  connect_state_wait_for_client_settings:
                end_cycle = connectStateWaitForClientSettings( message );
                break;

            case connect_state_wait_for_client_game_setup_ack:
                end_cycle = connectStateWaitForClientGameSetupAck( message );
                break;

            case connect_state_player_state_sync:
                end_cycle = connectStatePlayerStateSync();
                break;

            case connect_state_unit_sync:
                end_cycle = connectStateUnitSync();
                break;

            default:
                assert("Bad connection state" == 0);

        } // ** switch
    } while( end_cycle == false );
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
    if( connection_state == connect_state_idle ) {
        return false;
    }

    return true;
}
