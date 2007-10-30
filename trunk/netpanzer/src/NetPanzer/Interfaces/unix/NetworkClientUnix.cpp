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

#include "Util/Exception.hpp"
#include "Util/Log.hpp"
#include "Core/NetworkGlobals.hpp"
#include "Classes/Network/NetworkState.hpp"
#include "Interfaces/unix/NetworkClientUnix.hpp"

#include "Views/Components/Desktop.hpp"
#include "Views/Game/LobbyView.hpp"
#include "Views/Game/DisconectedView.hpp"

#include "Network/SocketManager.hpp"

//#define NETWORKDEBUG

#ifdef NETWORKDEBUG
#include "NetPacketDebugger.hpp"
#endif

NetworkClientUnix::NetworkClientUnix()
        : NetworkClient(), clientsocket(0)
{}

NetworkClientUnix::~NetworkClientUnix()
{
    delete clientsocket;
}

void
NetworkClientUnix::onClientConnected(ClientSocket *s)
{
    (void)s;
}

void
NetworkClientUnix::onClientDisconected(ClientSocket *s, const char *msg)
{
    (void)s;
    DisconectedView *dv = (DisconectedView *)Desktop::getView("DisconectedView");
    dv->setMsg(msg);
    Desktop::setVisibility("DisconectedView", true);
    delete clientsocket;
    clientsocket=0;
}

bool NetworkClientUnix::joinServer(const std::string& server_name)
{
    if ( clientsocket )
        delete clientsocket;
    clientsocket = 0;
    
    LOG( ("Trying to join server '%s'.\n", server_name.c_str()) );
    try {
        clientsocket = new ClientSocket(this, server_name);
    } catch(std::exception& e) {
        LOG( ( "Couldn't connect to server:\n%s.", e.what()) );
        char text[128];
        snprintf(text, 128, "connection error: %s", e.what());
        lobbyView->scrollAndUpdate(text);
        return false;
    }

    connection_type = _connection_network;

    return true;
}

void NetworkClientUnix::partServer()
{
    if ( clientsocket )
        delete clientsocket;
    clientsocket = 0;
}

void NetworkClientUnix::sendMessage(NetMessage* message, size_t size)
{
    message->setSize(size);
    
    if (connection_type == _connection_loop_back) {
        net_packet.fromID = 0;
        net_packet.toID = 0;
        memcpy(net_packet.data, message, size);
        loop_back_recv_queue.enqueue(net_packet);
#ifdef NETWORKDEBUG
        NetPacketDebugger::logMessage("LS", message);
#endif
        return;
    }

    if(!clientsocket)
        return;

#ifdef NETWORKDEBUG
    NetPacketDebugger::logMessage("S", message);
#endif

    clientsocket->sendMessage(message, size);

    NetworkState::incPacketsSent(size);
}

bool NetworkClientUnix::getMessage(NetMessage *message)
{
    if(clientsocket == 0)
        return false;
    if(!receive_queue.isReady())
        return false;
    
    receive_queue.dequeue( &net_packet );
    memcpy(message, net_packet.data, net_packet.getSize());

#ifdef NETWORKDEBUG
    NetPacketDebugger::logMessage("R", message);
#endif

    if ( message->message_class == _net_message_class_client_server ) {
        processNetMessage( message );
    }

    NetworkState::incPacketsReceived(net_packet.getSize());

    return true;
}

void NetworkClientUnix::checkIncoming()
{
   
    
}
