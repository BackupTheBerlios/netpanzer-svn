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

#include "Exception.hpp"
#include "Log.hpp"
#include "Desktop.hpp"
#include "NetworkGlobals.hpp"
#include "NetworkState.hpp"
#include "NetworkClientUnix.hpp"
#include "LobbyView.hpp"

// #define NETWORKDEBUG

NetworkClientUnix::NetworkClientUnix( void )
        : NetworkClient(), clientsocket(0)
{}

NetworkClientUnix::~NetworkClientUnix()
{
    delete clientsocket;
}

int NetworkClientUnix::openSession()
{
    return true;
}

int NetworkClientUnix::startEnumeration(ConnectionAddress )
{
    LOG( ("Server enumeration not implemented yet!") );
    Desktop::setVisibility("IPAddressView", true);
    return false;
}

int NetworkClientUnix::startEnumeration( )
{
    LOG( ("Server enumeration not implemented yet!") );
    Desktop::setVisibility("IPAddressView", true);
    return false;
}

int NetworkClientUnix::stopEnumeration( void )
{
    return true;
}

int NetworkClientUnix::getSessionList(SessionList& )
{
    LOG( ("SessionList enumeration not implemented yet!") );

    return false;
}

int NetworkClientUnix::joinSession()
{
    return false;
}

int NetworkClientUnix::joinSession(int )
{
    LOG ( ("Join by session ID not implemented!") );
    return false;
}

int NetworkClientUnix::joinSession(const char* session_name)
{
    delete clientsocket;
    clientsocket = 0;
    LOG( ("Trying to join server '%s'.\n", session_name) );
    try {
        clientsocket = new ClientSocket(session_name, _NETPANZER_DEFAULT_PORT_TCP);
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

int NetworkClientUnix::setJoinSession(const char* )
{
    return false;
}

int NetworkClientUnix::closeSession()
{
    delete clientsocket;
    clientsocket = 0;

    return true;
}

void NetworkClientUnix::sendMessage(NetMessage *message, size_t size, int flags)
{
    if ( connection_type == _connection_loop_back ) {
        memmove( net_packet.data, (void *) message, size  );
        net_packet.packet_size = (unsigned short) size;
        loop_back_recv_queue.enqueue( net_packet );
        return;
    }

    if(!clientsocket)
        return;

#ifdef NETWORKDEBUG
    LOG( ( "SEND >> Class: %d ID: %d", message->message_class,
           message->message_id) );
#endif

    message->size = size;
    clientsocket->sendMessage((char*) message, message->size,
                              ! (flags & _network_send_no_guarantee) );

    NetworkState::incPacketsSent( size );
}

int NetworkClientUnix::getMessage(NetMessage *message)
{
    updateKeepAliveState();

    if ( receive_queue.isReady() ) {
        receive_queue.dequeue( &net_packet );

        memmove(  (void *) message, net_packet.data, net_packet.packet_size );

#ifdef NETWORKDEBUG
        LOG( ( "RECV >> Class: %d ID: %d", message->message_class,
               message->message_id) );
#endif

        if ( message->message_class == _net_message_class_client_server ) {
            processNetMessage( message );
        }

        NetworkState::incPacketsReceived( net_packet.packet_size );

        return true;
    }

    if ( non_guarantee_queue.isReady() ) {
        non_guarantee_queue.dequeue( &net_packet );

        memmove(  (void *) message, net_packet.data, net_packet.packet_size );

#ifdef NETWORKDEBUG
        LOG( ( "RECV >> Class: %d ID: %d", message->message_class,
               message->message_id )  );
#endif

        if ( message->message_class == _net_message_class_client_server ) {
            processNetMessage( message );
        }

        NetworkState::incPacketsReceived( net_packet.packet_size );

        return( true );
    }

    return false;
}

void NetworkClientUnix::checkIncoming()
{
    if(clientsocket) {
        clientsocket->read();
    }
}
