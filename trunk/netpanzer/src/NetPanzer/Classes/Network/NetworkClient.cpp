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
#include "Core/NetworkGlobals.hpp"

#include "NetworkClient.hpp"

#include "Classes/Network/NetworkState.hpp"

#include "ClientServerNetMessage.hpp"
#include "ConnectNetMessage.hpp"

#include "Interfaces/PlayerInterface.hpp"
#include "Classes/Network/ClientConnectDaemon.hpp"

#include "Interfaces/ConsoleInterface.hpp"

NetworkClient::NetworkClient( void )
        : NetworkInterface()
{
    connection_status = _connection_status_no_connection;
    connection_type = _connection_loop_back;
}

NetworkClient::~NetworkClient()
{
}

void NetworkClient::netMessageClientKeepAlive(const NetMessage* )
{
    // nothing
}

void NetworkClient::netMessageClientSetKeepAliveState(const NetMessage* )
{
    // nothing
}

void NetworkClient::netMessageClientPingAck(const NetMessage* )
{
    // nothing
}

void NetworkClient::netMessageClientConnectAck(const NetMessage* )
{
    ClientConnectJoinRequest join_request;

    connection_status = _connection_status_connected;

    join_request.setProtocolVersion(NETPANZER_PROTOCOL_VERSION);

    sendMessage( &join_request, sizeof(ClientConnectJoinRequest));
}


void NetworkClient::processNetMessage(const NetMessage* message)
{
    switch(message->message_id) {
        case _net_message_id_client_keep_alive:
            netMessageClientKeepAlive(message);
            break;

        case _net_message_id_client_set_keepalive_state: 
            netMessageClientSetKeepAliveState(message);
            break;

        case _net_message_id_client_ping_ack:
            netMessageClientPingAck(message);
            break;

        case _net_message_id_client_connect_ack:
            netMessageClientConnectAck(message);
            break;

        default:
            LOGGER.warning("Unknown messageid in clientnetmessage (id %d)",
                    message->message_id);
            break;
    }
}

