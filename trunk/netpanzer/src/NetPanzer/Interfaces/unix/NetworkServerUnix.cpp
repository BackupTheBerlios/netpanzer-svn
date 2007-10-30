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

#include <assert.h>
#include "Util/Log.hpp"
#include "Core/NetworkGlobals.hpp"
#include "Interfaces/GameConfig.hpp"
#include "Classes/Network/NetworkState.hpp"
#include "Util/Exception.hpp"
#include "NetworkServerUnix.hpp"

#include "Classes/Network/NetPacketDebugger.hpp"

//#define NETWORKDEBUG

NetworkServerUnix::NetworkServerUnix()
        : NetworkServer(), serversocket(0)
{}

NetworkServerUnix::~NetworkServerUnix()
{
    if ( serversocket )
        delete serversocket;
}

std::string
NetworkServerUnix::getIP(NetClientID id) const
{
    return serversocket->getClientIP(id);
}

void
NetworkServerUnix::shutdownClientTransport(NetClientID client_id)
{
    if (serversocket)
        serversocket->disconectClient(client_id);
}

void
NetworkServerUnix::openSession()
{}

void
NetworkServerUnix::hostSession()
{
    if ( serversocket ) {
        delete serversocket;
        serversocket = 0;
    }
    serversocket = new ServerSocket(gameconfig->bindaddress,
            gameconfig->serverport);
}

void
NetworkServerUnix::closeSession()
{
    if ( serversocket )
        delete serversocket;
    serversocket = 0;
}

void
NetworkServerUnix::sendMessage(NetClientID network_id, NetMessage* message,
        size_t size)
{
    if(serversocket == 0)
        return;
    message->setSize(size);

    try {
        serversocket->sendMessage(network_id, message, size);
    } catch(std::exception& e) {
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

void
NetworkServerUnix::sendMessage(NetMessage *message, size_t size)
{
    for(ClientList::iterator i = client_list.begin(); i != client_list.end();
            ++i) {
        if((*i)->wannadie)
            continue;

        try {
            sendMessage((*i)->client_id.getNetworkID(), message, size);
        } catch(std::exception& e) {
            LOG( ("Error while sending network packet.") );
            return;
        }
    }
}

bool
NetworkServerUnix::getPacket(NetPacket* packet)
{
    if (loop_back_recv_queue.isReady() ) {
        loop_back_recv_queue.dequeue(packet);

#ifdef NETWORKDEBUG
        NetPacketDebugger::logPacket("R", packet);
#endif
        return true;
    } else if (receive_queue.isReady()) {
        receive_queue.dequeue(packet);
        NetworkState::incPacketsReceived(packet->getSize());

        if (packet->getNetMessage()->message_class 
                == _net_message_class_client_server) {
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
NetworkServerUnix::checkIncoming()
{
    cleanupClients();
}

