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
#ifndef _UNIXSERVER_H
#define _UNIXSERVER_H

#include <stdint.h>
#include "ClientList.hpp"
#include "Network/TCPListenSocket.hpp"
#include "Network/SocketSet.hpp"

class ServerSocket
{
public:
    ServerSocket(const std::string& bindaddress, uint16_t port);
    ~ServerSocket();

    void read();
    void sendMessage(SocketClient::ID toclient, const void* data,
            size_t datasize);
    void removeClient(SocketClient::ID clientid);

protected:
    friend class SocketClient;
    void closeConnection(SocketClient* client);

private:
    void acceptNewClients();
    void readTCP();
    void readClientTCP(SocketClient* client);

    network::TCPListenSocket* socket;
    network::SocketSet sockets;
    ClientList* clientlist;
};

#endif
