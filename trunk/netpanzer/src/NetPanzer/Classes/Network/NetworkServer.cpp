
#include "Bot/BotManager.hpp"

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


#include "Resources/ResourceManager.hpp"

#include "NetworkServer.hpp"
#include <algorithm>

#include "NetMessage.hpp"
#include "Core/CoreTypes.hpp"

#include "Util/Log.hpp"
#include "Classes/Network/NetworkServer.hpp"

#include "ClientServerNetMessage.hpp"
#include "Classes/Network/ServerConnectDaemon.hpp"
#include "Classes/Network/NetworkState.hpp"

#include "Interfaces/PlayerInterface.hpp"
#include "Interfaces/ConsoleInterface.hpp"
#include "Interfaces/GameConfig.hpp"

#include "Objectives/ObjectiveInterface.hpp"
#include "SystemNetMessage.hpp"
#include "ConnectNetMessage.hpp"
#include "Resources/ResourceManagerMessages.hpp"

#include "Network/MessageClassHandler.hpp"

typedef std::list<ServerClientListData*> ClientList;
static ClientList client_list;
static ClientList connecting_clients;



class ListenSocket : public TCPListenSocketObserver,
                     public ClientSocketObserver
{
public:
    ListenSocket() : socket(0) {};
    ~ListenSocket()
    {
        if ( socket )
        {
            socket->destroy();
        }
    }

    void listen(const std::string& address, const int port)
    {
        if ( socket )
        {
            socket->destroy();
            socket = 0;
        }

        try
        {
            Address addr = Address::resolve(address, port);
            socket = new TCPListenSocket(addr, this);
        }
        catch(...)
        {
            if (socket)
            {
                socket->destroy();
            }
            socket = 0;
            throw;
        }
    }

    void stopListen()
    {
        if ( socket )
        {
            socket->destroy();
            socket = 0;
        }
    }

    void onSocketError(TCPListenSocket *so, const char * msg)
    {
        (void)so;
        LOGGER.warning("NetworkServer: Listen Socket error: '%s'", msg);
        LOGGER.warning("Something bad could happen from now");

    }

    TCPSocketObserver * onNewConnection(TCPListenSocket *so, const Address &fromaddr)
    {
        (void)so;
        (void)fromaddr;
        return new ClientSocket(this);
    }

    void onClientConnected(ClientSocket *s)
    {
        int id = s->getId();
        LOGGER.debug("NetworkServer: client connected [%d]", id);

        ClientMesgConnectAck connect_ack_mesg;
        connect_ack_mesg.setSize(sizeof(ClientMesgConnectAck));
        s->sendMessage( &connect_ack_mesg,sizeof(ClientMesgConnectAck));

        ServerClientListData *client_data = new ServerClientListData;
        client_data->client_socket = s;
        connecting_clients.push_back(client_data);
    }

private:
    TCPListenSocket * socket;
};

static Timer keep_alive_emit_timer;

static ListenSocket listener;

void
NetworkServer::resetClientList()
{
    for(ClientList::iterator i = client_list.begin(); i != client_list.end();
            ++i)
    {
        delete (*i)->client_socket;
        delete *i;
    }
    client_list.clear();
    for(ClientList::iterator i = connecting_clients.begin(); i != connecting_clients.end();
            ++i)
    {
        delete (*i)->client_socket;
        delete *i;
    }
    connecting_clients.clear();
}

bool
NetworkServer::addClientToSendList( ClientSocket * client )
{
    ClientList::iterator i = connecting_clients.begin();
    while ( i != connecting_clients.end() )
    {
        if( (*i)->client_socket == client )
        {
            client_list.push_back(*i);
            client_list.erase(i);
            break;
        }
        i++;
    }

    if ( i == connecting_clients.end() )
    {
        LOGGER.warning("Unknown client not added to list");
    }

    return true;
}

void
NetworkServer::cleanUpClientList()
{
    ClientList::iterator i = client_list.begin();
    while ( i != client_list.end() )
    {
        if( (*i)->wannadie )
        {
            delete (*i)->client_socket;
            delete (*i);
            i = client_list.erase(i);
        }
        i++;
    }
    i = connecting_clients.begin();
    while ( i != connecting_clients.end() )
    {
        if( (*i)->wannadie )
        {
            delete (*i)->client_socket;
            delete (*i);
            i = connecting_clients.erase(i);
        }
        i++;
    }
}

void
NetworkServer::dropClient(Uint16 playerid)
{
    if ( BotManager::isBot(playerid) )
    {
        BotManager::removeBot(playerid);
    }
    else
    {
        ClientList::iterator i = client_list.begin();
        while( i != client_list.end() && (*i)->client_socket->getPlayerIndex() != playerid )
        {
            ++i;
        }

        if ( i != client_list.end() )
        {
            // XXX hack
            onClientDisconected((*i)->client_socket, "dropped");
        }
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
    resetClientList();
    listener.listen(gameconfig->bindaddress, gameconfig->serverport);
}

void
NetworkServer::closeSession()
{
    listener.stopListen();
}



void
NetworkServer::broadcastMessage(NetMessage *message, size_t size)
{
    ClientList::iterator i = client_list.begin();
    while ( i != client_list.end() )
    {
        if( ! (*i)->wannadie )
        {
            try
            {
                message->setSize(size);
                (*i)->client_socket->sendMessage( message, size);
            }
            catch(std::exception& e)
            {
                // XXX will it close by itself?
                LOGGER.warning ("Network broadcast error when sending to player %d: %s",
                       (*i)->client_socket->getPlayerIndex(), e.what() );
            }
        }
        ++i;
    }
}

void
NetworkServer::sendMessage(Uint16 player_index, NetMessage* message,
        size_t size)
{
    if ( BotManager::isBot(player_index) || socket == 0 )
    {
        return;
    }
    
    ClientList::iterator i = client_list.begin();
    while ( i != client_list.end() )
    {
        if ( ! (*i)->wannadie
             && (*i)->client_socket->getPlayerIndex() == player_index )
        {
            try
            {
                message->setSize(size);
                (*i)->client_socket->sendMessage( message, size);

                NetworkState::incPacketsSent(size);
                return; // premature exit
            }
            catch (NetworkException e)
            {
                LOGGER.warning ("Network send error when sending to player %d: %s",
                       player_index, e.what() );
            }            
        }
        ++i;
    }

    // if didn't found player we are here.
    LOGGER.warning("NetworkServer: sendMessage to unknown client: (index) %d", player_index);
}

void
NetworkServer::sendRemaining()
{
    ClientList::iterator i = client_list.begin();
    while ( i != client_list.end() )
    {
        if ( ! (*i)->wannadie )
        {
            (*i)->client_socket->sendRemaining();
        }
        i++;
    }

    i = connecting_clients.begin();
    while ( i != connecting_clients.end() )
    {
        if ( ! (*i)->wannadie )
        {
            (*i)->client_socket->sendRemaining();
        }
        i++;
    }

}

ClientSocket *
NetworkServer::getClientSocketByPlayerIndex ( Uint16 index )
{
    ClientList::iterator i = client_list.begin();
    while ( i != client_list.end() )
    {
        if ( ! (*i)->wannadie
             && (*i)->client_socket->getPlayerIndex() == index )
        {
            return (*i)->client_socket;
        }
        i++;
    }

    return NULL;
}

std::string
NetworkServer::getIP(Uint16 player_index)
{
    if ( BotManager::isBot(player_index) )
    {
        return "Bot";
    }
    ClientList::iterator i = client_list.begin();
    while ( i != client_list.end() )
    {
        if ( ! (*i)->wannadie
             && (*i)->client_socket->getPlayerIndex() == player_index )
        {
            return (*i)->client_socket->getIPAddress();
        }
        
        i++;
    }

    return "Not a client";
}

void
NetworkServer::onClientDisconected(ClientSocket *s, const char * msg)
{
    LOGGER.debug("NetworkServer::onClientDisconected( %d, '%s')", s->getId(), msg);

    bool cleandisconnect = false;
    bool sendalert = true;

    if ( ServerConnectDaemon::inConnectQueue(s) )
    {
        // player was connecting and dropped.
        ServerConnectDaemon::removeClientFromQueue(s);
        sendalert = false;
        LOGGER.debug("NetworkServer::onClientDisconected player was connecting");
    }



//    if ( NetworkInterface::receive_queue.isReady() )
//    {
//        unsigned long frontsave = NetworkInterface::receive_queue.front;
//        while ( NetworkInterface::receive_queue.isReady() )
//        {
//            LOGGER.debug("NetworkServer::onClientDisconected there was a packet");
//            NetPacket packet;
//
//            NetworkInterface::receive_queue.dequeue(&packet);
//
//            if ( packet.fromClient == s )
//            {
//                LOGGER.debug("NetworkServer::onClientDisconected the packet was from our friend");
//                const NetMessage * netmessage = packet.getNetMessage();
//                if (   netmessage->message_class == _net_message_class_connect
//                    && netmessage->message_id == _net_message_id_connect_netPanzer_client_disconnect )
//                {
//                    LOGGER.debug("NetworkServer::onClientDisconected so is a clean disconnect");
//                    cleandisconnect = true;
//                }
//            }
//        }
//        NetworkInterface::receive_queue.front = frontsave;
//    }

    Uint16 player_index = s->getPlayerIndex();

    ClientList::iterator i = client_list.begin();
    while ( i != client_list.end() )
    {
        if ( (*i)->client_socket == s )
        {
            LOGGER.debug("NetworkServer:onClientDisconnected found client in list, preparing to die [%d]", player_index);
            (*i)->wannadie = true;
            break;
        }
        ++i;
    }

    i = connecting_clients.begin();
    while ( i != connecting_clients.end() )
    {
        if ( (*i)->client_socket == s )
        {
            LOGGER.debug("NetworkServer:onClientDisconnected found client in connecting list, preparing to die [%d]", player_index);
            (*i)->wannadie = true;
            break;
        }
        ++i;
    }

    if ( player_index != 0xffff )
    {
        PlayerState * player = PlayerInterface::getPlayer(player_index);
        if ( player && sendalert )
        {
            ConsoleInterface::postMessage(Color::cyan, true, player->getFlag(),
                                      "'%s' has left.",
                                      player->getName().c_str());
        }

        ObjectiveInterface::disownPlayerObjectives( player_index );

        UnitInterface::destroyPlayerUnits( player_index );

        ResourceManagerReleaseFlagMessage releasemsg;
        releasemsg.setFlagID(player->getFlag());
        ResourceManager::releaseFlag(player->getFlag());

        PlayerInterface::disconnectPlayerCleanup( player_index );

        broadcastMessage(&releasemsg, sizeof(releasemsg));

        if ( sendalert )
        {
            SystemConnectAlert msg;
            if ( cleandisconnect )
            {
                msg.set( player_index, _connect_alert_mesg_disconnect);
            }
            else
            {
                msg.set( player_index, _connect_alert_mesg_client_drop );
            }

            broadcastMessage(&msg, sizeof(msg));
        }
    }
}

class ServerPacketHandler : public MessageClassHandler
{
public:
    void handlePacket(const NetPacket* packet)
    {
        const NetMessage* message = packet->getNetMessage();
        switch(message->message_id)
        {
            case _net_message_id_server_keep_alive:
                netPacketClientKeepAlive(packet);
                break;

            case _net_message_id_server_ping_request:
                netPacketServerPingRequest(packet);
                break;

            case _net_message_id_transport_disconnect:
                NetworkServer::onClientDisconected(packet->fromClient, "Socket disconnected message");
                break;

            default:
                LOGGER.warning("Unknown networkserverpacket: id:%d.",
                        message->message_id);
                break;
        }
    }
private:
    void netPacketClientKeepAlive(const NetPacket* )
    {
        // nothing
    }

    void netPacketServerPingRequest(const NetPacket* )
    {
        // nothing
    }

} serverpackethandler;

MessageClassHandler *
NetworkServer::getPacketHandler()
{
    return &serverpackethandler;
}
