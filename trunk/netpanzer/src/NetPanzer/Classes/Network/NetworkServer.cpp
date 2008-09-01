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

#include "NetMessage.hpp"

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
        : NetworkInterface(), socket(0)
{
    // nothing
}

NetworkServer::~NetworkServer()
{
    if ( socket )
        socket->destroy();
}

void NetworkServer::resetClientList()
{
    for(ClientList::iterator i = client_list.begin(); i != client_list.end();
            ++i)
    {
        delete (*i)->client_socket;
        delete *i;
    }
    client_list.clear();
}

bool NetworkServer::addClientToSendList( ClientSocket * client )
{
    ServerClientListData *client_data = new ServerClientListData;

    client_data->client_socket = client;
    client_list.push_back(client_data);

    return true;
}

void NetworkServer::netPacketClientKeepAlive(const NetPacket* )
{
    // nothing
}

void NetworkServer::netPacketServerPingRequest(const NetPacket* )
{
    // nothing
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
    if ( socket )
    {
        socket->destroy();
        socket = 0;
    }

    resetClientList();
    
    try
    {
        Address addr = Address::resolve(gameconfig->bindaddress,
                                        gameconfig->serverport);
        
        socket = new TCPListenSocket(addr, this);

    }
    catch(...)
    {
        if (socket)
            socket->destroy();
        socket = 0;
        throw;
    }
}

void
NetworkServer::closeSession()
{
    if ( socket )
        socket->destroy();
    
    socket = 0;
}

void
NetworkServer::broadcastMessage(NetMessage *message, size_t size)
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
            message->setSize(size);
            (*i)->client_socket->sendMessage( message, size);
        }
        catch(std::exception& e)
        {
            // XXX handle properly
            LOG( ("Error while sending network packet.") );
            return;
        }
        
        ++i;
    }
}

void
NetworkServer::sendMessage(Uint16 player_index, NetMessage* message,
        size_t size)
{
    if( socket == 0 )
        return;
    
    ClientList::iterator i = client_list.begin();
    while ( i != client_list.end() && (*i)->client_socket->getPlayerIndex() != player_index )
    {
        ++i;
    }

    if ( i != client_list.end() )
    {
        try
        {
            message->setSize(size);
            (*i)->client_socket->sendMessage( message, size);

            NetworkState::incPacketsSent(size);
            
#ifdef NETWORKDEBUG
            NetPacketDebugger::logMessage("S", message);
#endif

        }
        catch (NetworkException e)
        {
            LOG ( ("Network send error when sending to client (index) %d: %s",
                   player_index, e.what()) );
            dropClient((*i)->client_socket);
        }
    }
    else
    {
        LOGGER.warning("NetworkServer: sendMessage to unknown client: (index) %d", player_index);
    }
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
NetworkServer::dropClient(ClientSocket * client)
{
    ClientList::iterator i = client_list.begin();
    while( i != client_list.end() && (*i)->client_socket != client )
    {
        ++i;
    }
    
    if ( i != client_list.end() )
    {
        ServerConnectDaemon::startClientDropProcess((*i)->client_socket);
    }
}

void
NetworkServer::shutdownClientTransport( ClientSocket * client )
{
    
    ClientList::iterator i = client_list.begin();
    while ( i != client_list.end() && (*i)->client_socket != client)
    {
        if ( (*i)->client_socket == client )
        {
            LOGGER.warning("NetworkServer: disconnecting client [%d]", client->getPlayerIndex());
            client_list.erase(i);
            delete (*i)->client_socket;
            delete *i;
            break;
        }
        ++i;
    }
}

void
NetworkServer::cleanupClients()
{
    ClientList::iterator i = client_list.begin();
    while( i != client_list.end() )
    {
        ServerClientListData* data = *i;
        if(data->wannadie)
        {
            delete data->client_socket;
            delete data;
            i = client_list.erase(i);
        }
        else
        {
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
    ClientList::iterator i = client_list.begin();
    while ( i != client_list.end() )
    {
        (*i)->client_socket->sendRemaining();
        i++;
    }
}

std::string
NetworkServer::getIP(Uint16 player_index)
{
    ClientList::iterator i = client_list.begin();
    while ( i != client_list.end() )
    {
        if ( (*i)->client_socket->getPlayerIndex() == player_index )
        {
            return (*i)->client_socket->getIPAddress();
        }
        
        i++;
    }

    return "Not a client";
}

void
NetworkServer::onSocketError(TCPListenSocket *so)
{
    (void)so;
    LOGGER.warning("NetworkServer: Listen Socket error, something bad could happen from now");

}

TCPSocketObserver *
NetworkServer::onNewConnection(TCPListenSocket *so, const Address &fromaddr)
{
    (void)so;
    (void)fromaddr;
    return new ClientSocket(this);
}

void
NetworkServer::onClientConnected(ClientSocket *s)
{
    int id = s->getId();
    LOGGER.debug("NetworkServer: client connected [%d]", id);
    //clients[id] = s;
    
    // this class handle this packet and it only sends the answer
    // send it here right now
    ClientMesgConnectAck connect_ack_mesg;
    connect_ack_mesg.setSize(sizeof(ClientMesgConnectAck));
    s->sendMessage( &connect_ack_mesg,sizeof(ClientMesgConnectAck));
}

void
NetworkServer::onClientDisconected(ClientSocket *s, const char * msg)
{
    // XXX WARNING possible double free when shutdownclienttransport is called
    LOGGER.warning("NetworkServer::onClientDisconected( %d, '%s')", s->getId(), msg);
    if ( ServerConnectDaemon::inConnectQueue(s) )
    {
        LOGGER.warning("NetworkServer: client bad disconnected while connecting [%d]", s->getId());
        ServerConnectDaemon::startClientDropProcess(s);
    }
    else
    {
        ClientList::iterator i = client_list.begin();
        while ( i != client_list.end() )
        {
            if ( (*i)->client_socket == s )
            {
                LOGGER.warning("NetworkServer: client bad disconnection [%d]", s->getId());
                ServerConnectDaemon::startClientDropProcess(s);
                break;
            }
            ++i;
        }
    }
}

ClientSocket *
NetworkServer::getClientSocketByPlayerIndex ( Uint16 index )
{
    ClientList::iterator i = client_list.begin();
    while ( i != client_list.end() )
    {
        if ( (*i)->client_socket->getPlayerIndex() == index )
        {
            return (*i)->client_socket;
        }
        i++;
    }
    
    return NULL;
}
