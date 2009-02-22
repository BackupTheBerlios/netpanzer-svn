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
#include <sys/param.h>

#include "Util/Log.hpp"
#include "Core/NetworkGlobals.hpp"

#include "NetworkClient.hpp"

#include "Classes/Network/NetworkState.hpp"
#include "Classes/Network/NetPacketDebugger.hpp"

#include "ClientServerNetMessage.hpp"
#include "ConnectNetMessage.hpp"

#include "Interfaces/PlayerInterface.hpp"
#include "Classes/Network/ClientConnectDaemon.hpp"

#include "Interfaces/ConsoleInterface.hpp"

#include "Views/Components/Desktop.hpp"
#include "Views/Game/LoadingView.hpp"
#include "Views/Game/DisconectedView.hpp"

#include "Network/MessageRouter.hpp"
#include "Network/MessageClassHandler.hpp"

static ClientSocket * clientsocket = 0;
static unsigned short connection_status = _connection_status_no_connection;

void
NetworkClient::initialize()
{
    partServer();
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

bool
NetworkClient::joinServer(const std::string& server_name)
{
    partServer();
    
    LOG( ("Trying to join server '%s'.\n", server_name.c_str()) );
    try
    {
        clientsocket = new ClientSocket(NULL, server_name);
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
    {
        delete clientsocket;
    }
    clientsocket = 0;
    connection_status = _connection_status_no_connection;
}

void NetworkClient::sendMessage(NetMessage* message, size_t size)
{
    message->setSize(size);
    
    if ( !clientsocket )
    {
        MessageRouter::enqueueIncomingPacket( message, size, PlayerInterface::getLocalPlayerIndex(), 0);
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

class ClientPacketHandler : public MessageClassHandler
{
public:
    void handlePacket(const NetPacket* packet)
    {
        const NetMessage * message = packet->getNetMessage();
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

            case _net_message_id_transport_disconnect:
                NetworkClient::onClientDisconected(packet->fromClient, "Socket disconnected message");
                break;

            default:
                LOGGER.warning("Unknown messageid in clientnetmessage (id %d)",
                        message->message_id);
                break;
        }
    }
private:
    void netMessageClientKeepAlive(const NetMessage* )
    {
        // nothing
    }

    void netMessageClientSetKeepAliveState(const NetMessage* )
    {
        // nothing
    }

    void netMessageClientPingAck(const NetMessage* )
    {
        // nothing
    }

    void netMessageClientConnectAck(const NetMessage* )
    {
        ClientConnectJoinRequest join_request;

        connection_status = _connection_status_connected;

        join_request.setProtocolVersion(NETPANZER_PROTOCOL_VERSION);

        NetworkClient::sendMessage( &join_request, sizeof(ClientConnectJoinRequest));
    }

} clientpackethandler;

MessageClassHandler *
NetworkClient::getPacketHandler()
{
    return &clientpackethandler;
}