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
#include "Classes/Network/NetworkServer.hpp"

#include "ClientServerNetMessage.hpp"
#include "Classes/Network/ServerConnectDaemon.hpp"
#include "Classes/Network/NetworkState.hpp"

#include "Interfaces/PlayerInterface.hpp"
#include "Interfaces/ConsoleInterface.hpp"
#include "Interfaces/GameConfig.hpp"

NetworkServer* SERVER = 0;

NetworkServer::NetworkServer()
        : NetworkInterface(), serversocket(0)
{
    // nothing
}

NetworkServer::~NetworkServer()
{
    if ( serversocket )
        delete serversocket;
}

void NetworkServer::resetClientList()
{
    for(ClientList::iterator i = client_list.begin(); i != client_list.end();
            ++i)
    {
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
NetworkServer::openSession()
{
    // nothing
}

void
NetworkServer::hostSession()
{
    if ( serversocket )
    {
        delete serversocket;
        serversocket = 0;
    }
    
    serversocket = new ServerSocket(gameconfig->bindaddress,
                                    gameconfig->serverport);
    
}

void
NetworkServer::closeSession()
{
    if ( serversocket )
        delete serversocket;
    
    serversocket = 0;
}

void
NetworkServer::sendMessage(NetMessage *message, size_t size)
{
    ClientList::iterator i = client_list.begin();
    while ( i != client_list.end() )
    {
        if((*i)->wannadie)
        {
            ++i;
            continue;
        }

        try
        {
            sendMessage((*i)->client_id.getNetworkID(), message, size);
        }
        catch(std::exception& e)
        {
            LOG( ("Error while sending network packet.") );
            return;
        }
        
        ++i;
    }
}

void
NetworkServer::sendMessage(NetClientID network_id, NetMessage* message,
        size_t size)
{
    if(serversocket == 0)
        return;
    
    message->setSize(size);

    try
    {
        serversocket->sendMessage(network_id, message, size);
    }
    catch(std::exception& e)
    {
        LOG ( ("Network send error when sending to client %d: %s",
               network_id, e.what()) );
        dropClient(network_id);
        return;
    }

    NetworkState::incPacketsSent(size);

#ifdef NETWORKDEBUG
    NetPacketDebugger::logMessage("S", message);
#endif
}

bool
NetworkServer::getPacket(NetPacket* packet)
{
    if ( loop_back_recv_queue.isReady() )
    {
        loop_back_recv_queue.dequeue(packet);

#ifdef NETWORKDEBUG
        NetPacketDebugger::logPacket("R", packet);
#endif
        return true;
    }
    else if (receive_queue.isReady())
    {
        receive_queue.dequeue(packet);
        NetworkState::incPacketsReceived(packet->getSize());

        if (packet->getNetMessage()->message_class 
                == _net_message_class_client_server)
        {
            processNetPacket(packet);
        }

#ifdef NETWORKDEBUG
        NetPacketDebugger::logPacket("R", packet);
#endif
        return true;
    }

    return false;
}

void
NetworkServer::dropClient(NetClientID network_id)
{
    ClientList::iterator i = client_list.begin();
    while( i != client_list.end() )
    {
        ServerClientListData* data = *i;
        if(data->client_id.getNetworkID() == network_id)
        {
            ServerConnectDaemon::startClientDropProcess(data->client_id);
            return;
        }
        ++i;
    }
}

void
NetworkServer::shutdownClientTransport(NetClientID client_id)
{
    if (serversocket)
        serversocket->disconectClient(client_id);
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

void
NetworkServer::checkIncoming()
{
    cleanupClients();
}

void
NetworkServer::sendRemaining()
{
    if ( serversocket )
        serversocket->sendRemaining();
}

std::string
NetworkServer::getIP(NetClientID id) const
{
    if ( serversocket )
        return serversocket->getClientIP(id);

    return "No server";
}
