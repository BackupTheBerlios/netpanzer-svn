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
#include "NetworkServer.hpp"

#include "ClientServerNetMessage.hpp"
#include "ServerConnectDaemon.hpp"

#include "PlayerInterface.hpp"
#include "ConsoleInterface.hpp"

NetworkServer::NetworkServer()
        : NetworkInterface()
{
}

NetworkServer::~NetworkServer()
{
}

void NetworkServer::resetClientList()
{
    for(ClientList::iterator i = client_list.begin(); i != client_list.end();
            ++i) {
        delete *i;
    }
    client_list.clear();
}

bool NetworkServer::addClientToSendList(const PlayerID& client_player_id)
{
    ServerClientListData *client_data = new ServerClientListData;

    client_data->client_id = client_player_id;
    client_list.push_back(client_data);

    return true;
}

void NetworkServer::removeClientFromSendList(const PlayerID &client_player_id)
{
    for(ClientList::iterator i = client_list.begin(); i != client_list.end();
            ++i) {
        ServerClientListData* data = *i;
        if(data->client_id == client_player_id) {
            data->wannadie = true;
            return;
        }
    }
}

void NetworkServer::netPacketClientKeepAlive(const NetPacket* )
{
    // nothing
}

void NetworkServer::netPacketServerPingRequest(const NetPacket* )
{
    // nothing
}

void NetworkServer::netPacketTransportClientAccept(const NetPacket* packet)
{
    ClientMesgConnectAck connect_ack_mesg;
    sendMessage(packet->fromID, &connect_ack_mesg,sizeof(ClientMesgConnectAck));
}

void NetworkServer::processNetPacket(const NetPacket* packet)
{
    const NetMessage* message = packet->getNetMessage();
    switch(message->message_id) {
        case _net_message_id_server_keep_alive: 
            netPacketClientKeepAlive(packet);
            break;

        case _net_message_id_server_ping_request:
            netPacketServerPingRequest(packet);
            break;

        case _net_message_id_transport_client_accept:
            netPacketTransportClientAccept(packet);
            break;
            
        default:
            LOGGER.warning("Unknown networkserverpacket: id:%d.",
                    message->message_id);
            break;
    }
}

void
NetworkServer::dropClient(SocketClient::ID network_id)
{
    for(ClientList::iterator i = client_list.begin(); i != client_list.end();
            ++i) {
        ServerClientListData* data = *i;
        if(data->client_id.getNetworkID() == network_id) {
            ServerConnectDaemon::startClientDropProcess(data->client_id);
            return;
        }
    }
}

void
NetworkServer::cleanupClients()
{
    for(ClientList::iterator i = client_list.begin(); i != client_list.end();
            ) {
        ServerClientListData* data = *i;
        if(data->wannadie) {
            delete data;
            i = client_list.erase(i);
        } else {
            ++i;
        }
    }
}

