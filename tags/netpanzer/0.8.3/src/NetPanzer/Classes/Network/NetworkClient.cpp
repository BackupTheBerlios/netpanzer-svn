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


#include "Util/Log.hpp"
#include "Core/NetworkGlobals.hpp"

#include "NetworkClient.hpp"

#include "Classes/Network/NetworkState.hpp"

#include "ClientServerNetMessage.hpp"
#include "ConnectNetMessage.hpp"

#include "Interfaces/PlayerInterface.hpp"
#include "Classes/Network/ClientConnectDaemon.hpp"

#include "Interfaces/ConsoleInterface.hpp"

#include "Views/Components/Desktop.hpp"
#include "Views/Game/LoadingView.hpp"
#include "Views/Game/DisconectedView.hpp"

NetworkClient *CLIENT = 0;

NetworkClient::NetworkClient( void )
        : NetworkInterface(), clientsocket(0)
{
    connection_status = _connection_status_no_connection;
}

NetworkClient::~NetworkClient()
{
    if ( clientsocket )
        delete clientsocket;
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
    switch(message->message_id)
    {
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

void
NetworkClient::onClientConnected(ClientSocket *s)
{
    (void)s;
}

void
NetworkClient::onClientDisconected(ClientSocket *s, const char *msg)
{
    (void)s;
    DisconectedView *dv = (DisconectedView *)Desktop::getView("DisconectedView");
    dv->setMsg(msg);
    
    Desktop::setVisibility("DisconectedView", true);
    
    delete clientsocket;
    clientsocket=0;
}

bool NetworkClient::joinServer(const std::string& server_name)
{
    if ( clientsocket )
        delete clientsocket;
    clientsocket = 0;
    
    LOG( ("Trying to join server '%s'.\n", server_name.c_str()) );
    try
    {
        clientsocket = new ClientSocket(this, server_name);
    }
    catch(std::exception& e)
    {
        LOG( ( "Couldn't connect to server:\n%s.", e.what()) );
        char text[128];
        snprintf(text, 128, "connection error: %s", e.what());
        LoadingView::append(text);
        return false;
    }

    return true;
}

void NetworkClient::partServer()
{
    if ( clientsocket )
        delete clientsocket;
    clientsocket = 0;
}

void NetworkClient::sendMessage(NetMessage* message, size_t size)
{
    message->setSize(size);
    
    if ( !clientsocket )
    {
        EnqueueIncomingPacket( message, size, PlayerInterface::getLocalPlayerIndex(), 0);
    }
    else
    {
        clientsocket->sendMessage(message, size);
        NetworkState::incPacketsSent(size);        
    }

#ifdef NETWORKDEBUG
    NetPacketDebugger::logMessage("S", message);
#endif
}

bool NetworkClient::getMessage(NetMessage *message)
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

    if ( message->message_class == _net_message_class_client_server )
    {
        processNetMessage( message );
    }

    NetworkState::incPacketsReceived(net_packet.getSize());

    return true;
}

void NetworkClient::checkIncoming()
{
   // nothing
}

void
NetworkClient::sendRemaining()
{
    if ( clientsocket )
        clientsocket->sendRemaining();
}
