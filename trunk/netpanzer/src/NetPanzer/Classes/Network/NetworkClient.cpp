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

#include "Util/Log.hpp"
#include "NetworkGlobals.hpp"

#include "NetworkClient.hpp"

#include "NetworkState.hpp"

#include "ClientServerNetMessage.hpp"
#include "ConnectNetMessage.hpp"

#include "PlayerInterface.hpp"
#include "ClientConnectDaemon.hpp"

#include "ConsoleInterface.hpp"

NetworkClient::NetworkClient( void )
        : NetworkInterface()
{
    connection_status = _connection_status_no_connection;
    connection_type = _connection_loop_back;

    keep_alive_state = false;
    keep_alive_emit_timer.changePeriod( _CLIENT_KEEP_ALIVE_SEND_INTERVAL ) ;
    keep_alive_timer.changePeriod( _SERVER_KEEP_ALIVE_THRESHOLD );

    ping_timer.changePeriod( _SERVER_PING_INTERVAL );
}

NetworkClient::~NetworkClient( void )
{
}

void NetworkClient::netMessageClientKeepAlive(NetMessage* )
{
    LOG ( ("keep alive.") );
    keep_alive_timer.reset();
}

void NetworkClient::netMessageClientSetKeepAliveState( NetMessage *message )
{
    ClientMesgSetKeepAlive *set_keepalive_state_mesg;

    set_keepalive_state_mesg = (ClientMesgSetKeepAlive *) message;

    keep_alive_state = set_keepalive_state_mesg->keep_alive_state;
    keep_alive_timer.reset();
    LOG( ("Set Keepalive from server: %d", keep_alive_state) );
}

void NetworkClient::netMessageClientPingAck(NetMessage* )
{
    LOG( ("ping -> pong.") );
    NetworkState::ping_time = (now() - NetworkState::ping_time_stamp) * 1000;
}

void NetworkClient::netMessageClientConnectAck( NetMessage *message )
{
    ClientMesgConnectAck  *connect_ack_mesg;
    ClientConnectJoinRequest join_request;

    connect_ack_mesg = (ClientMesgConnectAck  *) message;

    connection_status = _connection_status_connected;

    client_transport_id = connect_ack_mesg->getClientTransportID();

    join_request.setTransportID(client_transport_id);
    strcpy( join_request.code_word, _NETPANZER_CODEWORD );
    join_request.setProtocolVersion(_NETPANZER_PROTOCOL_VERSION);

    sendMessage( &join_request, sizeof(ClientConnectJoinRequest), 0 );
}


void NetworkClient::processNetMessage( NetMessage *message )
{
    switch( message->message_id ) {
    case _net_message_id_client_keep_alive :
        netMessageClientKeepAlive( message );
        break;

    case _net_message_id_client_set_keepalive_state :
        netMessageClientSetKeepAliveState( message );
        break;

    case _net_message_id_client_ping_ack :
        netMessageClientPingAck( message );
        break;

    case _net_message_id_client_connect_ack :
        netMessageClientConnectAck( message );
        break;

    default:
        LOG( ("Unknown messageid in NetworkClient::processNetMessage") );
        break;
    }
}

void NetworkClient::activateKeepAlive( void )
{
    keep_alive_state = true;
}

void NetworkClient::deactivateKeepAlive( void )
{
    keep_alive_state = false;
}

void NetworkClient::updateKeepAliveState( void )
{
    if ( keep_alive_state == false )
        return;

    if( keep_alive_timer.count() ) {
        LOG( ("Server Keep Alive Timed Out") );
        ClientConnectDaemon::serverConnectionBroken();
        keep_alive_state = false;
        connection_status = _connection_status_no_connection;
        partServer();
    } else {
        if ( keep_alive_emit_timer.count() ) {
            ServerMesgKeepAlive keep_alive_mesg;

            keep_alive_mesg.setClientID(
                PlayerInterface::getLocalPlayerID().getIndex());

            sendMessage( &keep_alive_mesg, sizeof(ServerMesgKeepAlive), 0 );
        }
    }

    if( ping_timer.count() ) {
        ServerMesgPingRequest  ping_request_mesg;
        ping_request_mesg.setClientID(
            PlayerInterface::getLocalPlayerID().getIndex());

        NetworkState::ping_time_stamp = now();

        sendMessage( &ping_request_mesg, sizeof(ServerMesgPingRequest), _network_send_no_guarantee );
    }
}

