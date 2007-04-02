/*
Copyright (C) 2003 Matthias Braun <matze@braunis.de>
 
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

#include <string.h>
#include <assert.h>
#include <sstream>
#include "Util/Log.hpp"
#include "ServerSocket.hpp"

#include "ClientServerNetMessage.hpp"
#include "NetworkInterface.hpp"
#include "NetPacket.hpp"
#include "NetMessage.hpp"
#include "NetworkGlobals.hpp"
#include "GameConfig.hpp"
#include "PlayerInterface.hpp"
#include "Client.hpp"
#include "ConsoleInterface.hpp"
#include "Util/Exception.hpp"

#include "ClientSocket.hpp"


ServerSocket::ServerSocket(const std::string& bindaddress, uint16_t port)
        : socket(0)
{
  
    try {
        network::Address addr
            = network::Address::resolve(bindaddress, port);        
        socket = new network::TCPListenSocket(addr, this);

    } catch(...) {
        if (socket)
            socket->destroy();
        throw;
    }
}

ServerSocket::~ServerSocket()
{
    if (socket)
        socket->destroy();
    
    std::map<NetClientID, ClientSocket *>::iterator ci = clients.begin();
    while ( ci != clients.end() ) {
        delete ci->second;
        ci++;
    }
    clients.clear();
}

network::TCPSocketObserver *
ServerSocket::onNewConnection(network::TCPListenSocket *so, const network::Address &fromaddr)
{
    return new ClientSocket(this);
}

void
ServerSocket::onClientConnected(ClientSocket *s)
{
    LOGGER.warning("Client Connected [%d]", s->getId());
    clients[s->getId()] = s;
    TransportClientAccept clientacceptmessage;
    clientacceptmessage.setSize(sizeof(TransportClientAccept));
    EnqueueIncomingPacket(&clientacceptmessage,
            sizeof(TransportClientAccept), 0, s->getId());
}

void
ServerSocket::onClientDisconected(ClientSocket *s)
{
    LOGGER.warning("Client Disconnected [%d]", s->getId());
    clients.erase(s->getId());
    delete s;
}

std::string
ServerSocket::getClientIP(NetClientID clientid)
{
    ClientSocket *s = clients[clientid];
    if ( !s )
        return "Not a client";
    
    return s->getIPAddress();
}

NetClientID
ServerSocket::addLoopbackClient()
{
    //SocketClient* client = clientlist->add(this, 0);
    //return client->id;
    return 0;
}

void
ServerSocket::sendMessage(NetClientID toclient, const void* data, size_t datasize)
{
    ClientSocket *cs = clients[toclient];
    if ( !cs ) {
        throw Exception("message sent to unknown client.");
    }

    cs->sendMessage(data, datasize);
}

void
ServerSocket::disconectClient(NetClientID c)
{
    LOGGER.warning("Disconecting client [%d]", c);
    ClientSocket *s = clients[c];
    if ( s ) {
        delete s;
        clients.erase(c);
    } else {
        LOGGER.warning("Disconecting unknown client [%d]", c);
    }
}

