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
#include "ServerConnectDaemon.hpp"
#include "ChatInterface.hpp"
#include "NetworkGlobals.hpp"
#include "PlayerInterface.hpp"
#include "UnitInterface.hpp"
#include "ObjectiveInterface.hpp"
#include "PowerUpInterface.hpp"
#include "MapInterface.hpp"
#include "Server.hpp"
#include "NetworkServer.hpp"
#include "GameManager.hpp"
#include "GameConfig.hpp"
#include "SelectionBoxSprite.hpp"

#include "PlayerNetMessage.hpp"
#include "ConnectNetMessage.hpp"
#include "SystemNetMessage.hpp"

#include "ConsoleInterface.hpp"
#include "Util/Log.hpp"

ServerConnectDaemon::ConnectionState
                    ServerConnectDaemon::connection_state 
						= ServerConnectDaemon::connect_state_idle;
bool                ServerConnectDaemon::connection_lock_state = false;
PlayerID            ServerConnectDaemon::connect_player_id;
PlayerState*        ServerConnectDaemon::connect_player_state;
std::list<ConnectQueueElement> ServerConnectDaemon::connect_queue;
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

    SERVER->sendMessage( &server_disconnect,
                         sizeof(ConnectMesgNetPanzerServerDisconnect));
}

bool ServerConnectDaemon::inConnectQueue( PlayerID &new_player_id )
{
    std::list<ConnectQueueElement>::iterator i;
    for(i = connect_queue.begin(); i != connect_queue.end(); ++i) {
        const ConnectQueueElement& connect_request = *i;

        if ( connect_request.new_player_id.getNetworkID() ==
                new_player_id.getNetworkID() ) {
            return true;
        }
    }

    if ( (connection_state != connect_state_idle) &&
            (connect_player_id.getNetworkID() == new_player_id.getNetworkID()  )
       )
        return true;

    return false;
}

void ServerConnectDaemon::updateQueuedClients()
{
    unsigned long queue_position = 1;

    std::list<ConnectQueueElement>::iterator i;
    for(i = connect_queue.begin(); i != connect_queue.end(); ++i) {
        const ConnectQueueElement& connect_request = *i;
      
        ConnectProcessUpdate process_update;
        process_update.setQueuePosition(queue_position);

        SERVER->sendMessage( connect_request.new_player_id, &process_update,
                             sizeof(ConnectProcessUpdate));

        queue_position++;
    }

}

void ServerConnectDaemon::netPacketClientDisconnect(const NetPacket *packet)
{
    // Note: this invalidates the ConnectMesgNetPanzerClientDisconnect playerID
    // value, just use the networkID, this way avoid possible fake packet
    // to kick other player.
    // In a future protocol version PlayerID should be removed from this packet.
    PlayerState *player = PlayerInterface::getPlayerByNetworkID(packet->fromID);
    if (player)
        startDisconnectionProcess(player->getPlayerID());
}

void ServerConnectDaemon::netPacketClientJoinRequest(const NetPacket* packet)
{
    const ClientConnectJoinRequest* join_request_mesg
        = (const ClientConnectJoinRequest *) packet->getNetMessage();
    ClientConnectJoinRequestAck join_request_ack;
    PlayerID new_player_id;

    new_player_id = PlayerID(0, packet->fromID);

    if(join_request_mesg->getProtocolVersion() == NETPANZER_PROTOCOL_VERSION) {
        join_request_ack.setResultCode(_join_request_result_success);
    } else {
        join_request_ack.setResultCode(_join_request_result_invalid_protocol);
    }

    join_request_ack.setServerProtocolVersion(NETPANZER_PROTOCOL_VERSION);

    if( join_request_ack.getResultCode() == _join_request_result_success ) {
        ConnectQueueElement connect_request;

        if ( !inConnectQueue( new_player_id) ) {
            connect_request.new_player_id  = new_player_id;
            connect_request.connect_status = _connect_status_waiting;

            if (connect_queue.size() > 25) {
                join_request_ack.setResultCode(_join_request_result_server_busy);
            } else {
                connect_queue.push_back(connect_request);
            }
        }
    }

    SERVER->sendMessage(packet->fromID, &join_request_ack,
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

void ServerConnectDaemon::sendConnectionAlert(PlayerID &player_id, int alert_enum)
{
    SystemConnectAlert connect_alert;
    PlayerState *player_state = 0;

    player_state = PlayerInterface::getPlayerState( player_id );


    switch( alert_enum ) {
    case _connect_alert_mesg_connect : {
            connect_alert.set( player_id, _connect_alert_mesg_connect );
            ConsoleInterface::postMessage( "'%s' has joined the game.",
                    player_state->getName().c_str() );
           const char* motd=gameconfig->motd.c_str();
           if (*motd!='\0'){
               ChatMesg chat_mesg;
               chat_mesg.message_scope=_chat_mesg_scope_server;
               chat_mesg.setSourcePlayerIndex(0);
               snprintf(chat_mesg.message_text, sizeof(chat_mesg.message_text), "%s",gameconfig->motd.c_str());
               SERVER->sendMessage(player_state->getNetworkID(),&chat_mesg, sizeof(chat_mesg));
           }

        }
        break;

    case _connect_alert_mesg_disconnect : {
            connect_alert.set( player_id, _connect_alert_mesg_disconnect );
            ConsoleInterface::postMessage( "'%s' has left the game.",
                    player_state->getName().c_str() );
        }
        break;

    case _connect_alert_mesg_client_drop : {
            connect_alert.set( player_id, _connect_alert_mesg_client_drop );
            ConsoleInterface::postMessage( "'%s' has left the game for some reason.",
                    player_state->getName().c_str() );
        }
        break;

    default :
        assert(0);
    } // ** switch

    SERVER->sendMessage( &connect_alert, sizeof(SystemConnectAlert));
}

void ServerConnectDaemon::resetConnectFsm()
{
    connection_state = connect_state_idle;
    time_out_timer.reset();
    time_out_counter = 0;
}


bool ServerConnectDaemon::connectStateIdle()
{
    if (!connect_queue.empty() && (connection_lock_state == false)) {
        ConnectQueueElement connect_request;

        connect_request   = connect_queue.front();
        connect_queue.pop_front();
        connect_player_id = connect_request.new_player_id;
        connection_state  = connect_state_wait_for_connect_request;

        ClientConnectStartConnect start_connect;

        SERVER->sendMessage(connect_player_id, &start_connect,
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
    if (message && message->message_id == _net_message_id_client_connect_request ) {

        ClientConnectResult connect_result;
        connect_player_state = PlayerInterface::allocateNewPlayer();

        if ( connect_player_state == 0 ) {
            connect_result.result_code = _connect_result_server_full;
            resetConnectFsm();
        } else {
            connect_result.result_code = _connect_result_success;
            time_out_timer.reset();
        }

        SERVER->sendMessage( connect_player_id, &connect_result,
                                                sizeof(ClientConnectResult));

        if ( connection_state != connect_state_idle )
            connection_state = connect_state_wait_for_client_settings;

    } else if ( time_out_timer.count() )
        resetConnectFsm();

    return true;
}

bool ServerConnectDaemon::connectStateWaitForClientSettings(
        const NetMessage* message )
{
    if ( message && message->message_id == _net_message_id_connect_client_settings ) {
            ConnectClientSettings *client_setting;

            client_setting = (ConnectClientSettings *) message;
            connect_player_state->setName( client_setting->player_name );
            connect_player_state->unit_config.setUnitColor( client_setting->unit_color );
			Uint8 flag = (Uint8) client_setting->getPlayerFlag();
            connect_player_state->setFlag(flag);

            connect_player_state->setID( connect_player_id.getNetworkID() );
            connect_player_state->setStatus( _player_state_connecting );
            connect_player_id = connect_player_state->getPlayerID();

            // ** send server game setting map, units, player, etc.
            ConnectMesgServerGameSettings* server_game_setup
                = GameManager::getServerGameSetup();
            SERVER->sendMessage(connect_player_id,
                    server_game_setup, sizeof(ConnectMesgServerGameSettings));
            delete server_game_setup;

            time_out_timer.reset();
			if(connection_state != connect_state_idle) {
				connection_state = connect_state_wait_for_client_game_setup_ack;
			}
            return true;
    }

    if ( time_out_timer.count() ) {
        connect_player_state->setStatus( _player_state_free );
        resetConnectFsm();
    }

    return true;
}

bool ServerConnectDaemon::connectStateWaitForClientGameSetupAck(
        const NetMessage* message )
{
    if ( message != 0 ) {
        if ( message->message_id == _net_message_id_connect_client_game_setup_ack ) {
            PlayerConnectID player_connect_mesg
                (connect_player_state->getNetworkPlayerState());

            SERVER->sendMessage(connect_player_id, &player_connect_mesg,
                    sizeof(PlayerConnectID));
            PlayerInterface::startPlayerStateSync( connect_player_id );

            ConnectProcessStateMessage state_mesg;
            state_mesg.setMessageEnum(_connect_state_message_sync_player_info);
            state_mesg.setPercentComplete(0);
            SERVER->sendMessage(connect_player_id, &state_mesg,
                    sizeof(ConnectProcessStateMessage));
			if(connection_state != connect_state_idle) {
				connection_state = connect_state_player_state_sync;
			}
			return true;
        } else if ( message->message_id == _net_message_id_connect_client_game_setup_ping ) {
            time_out_timer.reset();
            return true;
        }
    }

    if ( time_out_timer.count() ) {
        connect_player_state->setStatus( _player_state_free );
        resetConnectFsm();
    }

    return true;
}

bool ServerConnectDaemon::connectStatePlayerStateSync()
{
    ConnectProcessStateMessage state_mesg;
    int percent_complete;

    if (PlayerInterface::syncPlayerState( &percent_complete ) == true ) {
        state_mesg.setMessageEnum(_connect_state_message_sync_player_info_percent);
        state_mesg.setPercentComplete(percent_complete);
        SERVER->sendMessage(connect_player_id, &state_mesg,
                sizeof(ConnectProcessStateMessage));
        delete connect_unit_sync;
        connect_unit_sync = new UnitSync();
        sendunitpercent_timer.reset();

        state_mesg.setMessageEnum(_connect_state_message_sync_units);
        SERVER->sendMessage( connect_player_id, &state_mesg,
                sizeof(ConnectProcessStateMessage));
        SERVER->addClientToSendList( connect_player_id );

        PlayerStateSync player_state_update
            (connect_player_state->getNetworkPlayerState());

        SERVER->sendMessage(&player_state_update, sizeof(PlayerStateSync));
		if(connection_state != connect_state_idle) {
			connection_state = connect_state_unit_sync;
		}
        return true;
    } else if ( percent_complete > 0 ) {
        state_mesg.setMessageEnum(_connect_state_message_sync_player_info_percent);
        state_mesg.setPercentComplete(percent_complete);
        SERVER->sendMessage(connect_player_id, &state_mesg,
                sizeof(ConnectProcessStateMessage));
    }

    return true;
}

bool ServerConnectDaemon::connectStateUnitSync()
{
    ConnectProcessStateMessage state_mesg;
    int percent_complete = connect_unit_sync->getPercentComplete();

    // send a unit
    if(connect_unit_sync->sendNextUnit(connect_player_id)) {
        if ( sendunitpercent_timer.count() ) {
            state_mesg.setMessageEnum(_connect_state_message_sync_units_percent);
            state_mesg.setPercentComplete(percent_complete);
            SERVER->sendMessage(connect_player_id, &state_mesg,
                    sizeof(ConnectProcessStateMessage));
            sendunitpercent_timer.reset();
        }
        return true;
    }

    // Sending finished
    state_mesg.setMessageEnum(_connect_state_message_sync_units_percent);
    state_mesg.setPercentComplete(100);
    SERVER->sendMessage( connect_player_id, &state_mesg,
            sizeof(ConnectProcessStateMessage));

    UnitSyncIntegrityCheck unit_integrity_check_mesg;
    SERVER->sendMessage( connect_player_id, &unit_integrity_check_mesg,
            sizeof(UnitSyncIntegrityCheck));

    ObjectiveInterface::syncObjectives( connect_player_id );

    PowerUpInterface::syncPowerUps( connect_player_id );

    GameManager::spawnPlayer( connect_player_id );

    connect_player_state->setStatus( _player_state_active );

    PlayerStateSync player_state_update
        (connect_player_state->getNetworkPlayerState());

    SERVER->sendMessage( &player_state_update, sizeof(PlayerStateSync));

    state_mesg.setMessageEnum(_connect_state_sync_complete);
    SERVER->sendMessage( connect_player_id, &state_mesg,
            sizeof(ConnectProcessStateMessage));
    sendConnectionAlert( connect_player_id, _connect_alert_mesg_connect );

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

bool ServerConnectDaemon::disconnectClient( PlayerID player_id )
{
    SERVER->shutdownClientTransport(player_id.getNetworkID());

    SERVER->removeClientFromSendList(player_id);
    ObjectiveInterface::disownPlayerObjectives(player_id.getIndex());
    UnitInterface::destroyPlayerUnits(player_id.getIndex());
    PlayerInterface::disconnectPlayerCleanup( player_id );

	for(std::list<ConnectQueueElement>::iterator i = connect_queue.begin();
			i != connect_queue.end(); /*nothing */) {
		if(i->new_player_id == player_id) {
			i = connect_queue.erase(i);
			continue;
		}

		++i;
	}
	if(connect_player_id == player_id) {
		connection_state = connect_state_idle;
	}

    return true;
}


void ServerConnectDaemon::startDisconnectionProcess( PlayerID player_id )
{
    if ( disconnectClient( player_id ) == true ) {
        sendConnectionAlert( player_id, _connect_alert_mesg_disconnect );
    }
}

void ServerConnectDaemon::startClientDropProcess( PlayerID player_id )
{
    if ( disconnectClient( player_id ) == true ) {
        sendConnectionAlert( player_id, _connect_alert_mesg_client_drop );
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
