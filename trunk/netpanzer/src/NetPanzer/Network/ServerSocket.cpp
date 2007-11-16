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
#include "Network/ServerSocket.hpp"
#include "Network/ClientSocket.hpp"
#include "Classes/Network/ClientServerNetMessage.hpp"
#include "Classes/Network/NetworkInterface.hpp"
#include "Util/Exception.hpp"
#include "Util/Log.hpp"


ServerSocket::ServerSocket(const string& bindaddress, Uint16 port)
        : socket(0)
{
  
    try {
        Address addr = Address::resolve(bindaddress, port);        
        socket = new TCPListenSocket(addr, this);

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
    
    ClientsMapIterator ci = clients.begin();
    while ( ci != clients.end() ) {
        delete ci->second;
        ci++;
    }
    clients.clear();
}

void
ServerSocket::onSocketError(TCPListenSocket *so)
{
    (void)so;
    LOGGER.debug("ServerSocket: Listen Socket error");

}

TCPSocketObserver *
ServerSocket::onNewConnection(TCPListenSocket *so, const Address &fromaddr)
{
    (void)so;
    (void)fromaddr;
    return new ClientSocket(this);
}

void
ServerSocket::onClientConnected(ClientSocket *s)
{
    NetClientID id = s->getId();
    if ( clients.find(id) != clients.end() ) {
        LOGGER.warning("ServerSocket: client connected already in list [%d]", id);
        return;
    }
    LOGGER.debug("ServerSocket: client connected [%d]", id);
    clients[id] = s;
    TransportClientAccept clientacceptmessage;
    clientacceptmessage.setSize(sizeof(TransportClientAccept));
    EnqueueIncomingPacket(&clientacceptmessage,
            sizeof(TransportClientAccept), 0, s->getId());
}

void
ServerSocket::onClientDisconected(ClientSocket *s, const char * msg)
{
    NetClientID id = s->getId();
    ClientsMapIterator i = clients.find(id);
    if ( i == clients.end() ) {
        LOGGER.warning("ServerSocket: disconnected client not in map %d, msg=%s", id, msg);
    } else {
        LOGGER.warning("ServerSocket: disconnected client %d msg=%s", id, msg);
        clients.erase(i);
        delete s;
    }
}

std::string
ServerSocket::getClientIP(NetClientID clientid)
{
    ClientsMapIterator i = clients.find(clientid);
    if ( i == clients.end() )
        return "Not a client";
    
    return i->second->getIPAddress();
}

void
ServerSocket::sendMessage(NetClientID toclient, const void* data, size_t datasize)
{
    ClientsMapIterator ci = clients.find(toclient);
    if ( ci == clients.end() ) { // XXX remove the throw
        throw Exception("message sent to unknown client.");
    }

    ci->second->sendMessage(data, datasize);
}

void
ServerSocket::disconectClient(NetClientID c)
{
    ClientsMapIterator ci = clients.find(c);
    if ( ci != clients.end() ) {
        LOGGER.warning("ServerSocket: disconnect client [%d]", ci->second->getId());
        delete ci->second;
        clients.erase(ci);
    } else {
        LOGGER.warning("ServerSocket: disconnect client not in list [%d]", c);
    }
}

void
ServerSocket::sendRemaining()
{
    ClientsMapIterator ci = clients.begin();
    while ( ci != clients.end() ) {
        ci->second->sendRemaining();
        ci++;
    }
}

