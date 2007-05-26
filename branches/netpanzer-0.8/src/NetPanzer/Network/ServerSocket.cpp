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
#include "ServerSocket.hpp"
#include "ClientSocket.hpp"
#include "ClientServerNetMessage.hpp"
#include "NetworkInterface.hpp"
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
    
    map<NetClientID, ClientSocket *>::iterator ci = clients.begin();
    while ( ci != clients.end() ) {
        delete ci->second;
        ci++;
    }
    clients.clear();
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
    LOGGER.debug("ServerSocket::onClientConnected() [%d]", s->getId());
    clients[s->getId()] = s;
    TransportClientAccept clientacceptmessage;
    clientacceptmessage.setSize(sizeof(TransportClientAccept));
    EnqueueIncomingPacket(&clientacceptmessage,
            sizeof(TransportClientAccept), 0, s->getId());
}

void
ServerSocket::onClientDisconected(ClientSocket *s)
{
    LOGGER.debug("ServerSocket::onClientDisconnected() [%d]", s->getId());
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

void
ServerSocket::sendMessage(NetClientID toclient, const void* data, size_t datasize)
{
    map<NetClientID, ClientSocket *>::iterator ci = clients.find(toclient);
    if ( ci == clients.end() ) {
        throw Exception("message sent to unknown client.");
    }

    ci->second->sendMessage(data, datasize);
}

void
ServerSocket::disconectClient(NetClientID c)
{
    LOGGER.debug("ServerSocket::disconectClient() [%d]", c);
    map<NetClientID, ClientSocket *>::iterator ci = clients.find(c);
    if ( ci != clients.end() ) {
        delete ci->second;
        clients.erase(ci);
    } else {
        LOGGER.warning("ServerSocket::disconectClient() Disconecting unknown client [%d]", c);
    }
}

void
ServerSocket::sendRemaining()
{
    map<NetClientID, ClientSocket *>::iterator ci = clients.begin();
    while ( ci != clients.end() ) {
        ci->second->sendRemaining();
        ci++;
    }
}

