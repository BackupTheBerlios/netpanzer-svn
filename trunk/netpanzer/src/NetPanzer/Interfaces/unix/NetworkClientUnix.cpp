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
#include "Desktop.hpp"
#include "NetworkGlobals.hpp"
#include "NetworkState.hpp"
#include "NetworkClientUnix.hpp"
#include "LobbyView.hpp"

//#define NETWORKDEBUG

#ifdef NETWORKDEBUG
#include "NetPacketDebugger.hpp"
#endif

NetworkClientUnix::NetworkClientUnix( void )
        : NetworkClient(), clientsocket(0)
{}

NetworkClientUnix::~NetworkClientUnix()
{
    delete clientsocket;
}

int NetworkClientUnix::joinServer(const std::string& server_name)
{
    delete clientsocket;
    clientsocket = 0;
    LOG( ("Trying to join server '%s'.\n", server_name.c_str()) );
    try {
        clientsocket = new ClientSocket(server_name);
    } catch(Exception e) {
        LOG( ( "Couldn't connect to server:\n%s.", e.what()) );
        char text[128];
        snprintf(text, 128, "connection error: %s", e.what());
        lobbyView->scrollAndUpdate(text);
        return false;
    }

    connection_type = _connection_network;

    return true;
}

int NetworkClientUnix::partServer()
{
    delete clientsocket;
    clientsocket = 0;

    return true;
}

void NetworkClientUnix::sendMessage(NetMessage *message, size_t size, int flags)
{
    if ( connection_type == _connection_loop_back ) {
        memcpy(net_packet.data, message, size);
        net_packet.packet_size = (unsigned short) size;
        loop_back_recv_queue.enqueue( net_packet );
        return;
    }

    if(!clientsocket)
        return;

    message->setsize(size);

#ifdef NETWORKDEBUG
    NetPacketDebugger::logMessage("S", message);
#endif

    clientsocket->sendMessage((char*) message, size,
                              ! (flags & _network_send_no_guarantee) );

    NetworkState::incPacketsSent( size );
}

int NetworkClientUnix::getMessage(NetMessage *message)
{
    updateKeepAliveState();

    if ( receive_queue.isReady() ) {
        receive_queue.dequeue( &net_packet );

        memcpy(message, net_packet.data, net_packet.packet_size);

#ifdef NETWORKDEBUG
        NetPacketDebugger::logMessage("R", message);
#endif

        if ( message->message_class == _net_message_class_client_server ) {
            processNetMessage( message );
        }

        NetworkState::incPacketsReceived( net_packet.packet_size );

        return true;
    }

    return false;
}

void NetworkClientUnix::checkIncoming()
{
    if(clientsocket) {
        clientsocket->read();
    }
}
