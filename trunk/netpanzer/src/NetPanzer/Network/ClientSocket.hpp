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

#include "ProxyServer.hpp"
#include "Network/TCPSocket.hpp"

class ClientSocket
{
public:
    ClientSocket(const std::string& serveraddress);
    ~ClientSocket();

    void read();
    void sendMessage(const void* data, size_t datasize);
    ProxyServer proxy;

private:
    void readTCP();

    network::TCPSocket* socket;
};

#endif
