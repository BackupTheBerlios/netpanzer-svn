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
#ifndef __UILIB_NETWORK_CLIENTSOCKET_HPP__
#define __UILIB_NETWORK_CLIENTSOCKET_HPP__

#include <string>

#include "NetPacket.hpp"
#include "ProxyServer.hpp"
#include "Network/TCPSocket.hpp"

class ClientSocket;

class ClientSocketObserver
{
public:
    ClientSocketObserver(){};
    virtual ~ClientSocketObserver(){};
protected:
    friend class ClientSocket;
    virtual void onClientConnected(ClientSocket *cso) = 0;
    virtual void onClientDisconected(ClientSocket *cso) = 0;
};

class ClientSocket : public network::TCPSocketObserver
{
public:
    ClientSocket(ClientSocketObserver *o, const std::string& serveraddress);
    ClientSocket(ClientSocketObserver *o);
    ~ClientSocket();

    //void read();
    void sendMessage(const void* data, size_t datasize);
    ProxyServer proxy;

    NetClientID getId() { return id; };
    std::string getIPAddress();
    
protected:
    void onDataReceived(network::TCPSocket *so, const char *data, const int len);
    void onConnected(network::TCPSocket *so);
    void onDisconected(network::TCPSocket *so);

private:
    void initId();
    ClientSocketObserver * observer;
    network::TCPSocket* socket;

    char tempbuffer[_MAX_NET_PACKET_SIZE];
    uint16_t tempoffset;
    NetClientID id;
};

#endif
