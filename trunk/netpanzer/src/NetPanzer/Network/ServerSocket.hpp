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
#ifndef _SERVERSOCKET_H
#define _SERVERSOCKET_H

#include <map>
#include <stdint.h>
#include "Network/TCPListenSocket.hpp"
#include "ClientSocket.hpp"

using namespace network;
using namespace std;

class ServerSocket : public TCPListenSocketObserver, public ClientSocketObserver
{
public:
    ServerSocket(const string& bindaddress, uint16_t port);
    ~ServerSocket();

    void sendMessage(NetClientID toclient, const void* data,
            size_t datasize);
    void removeClient(NetClientID clientid);

    string getClientIP(NetClientID);
    void disconectClient(NetClientID c);
    void sendRemaining();

protected:
    network::TCPSocketObserver * onNewConnection(TCPListenSocket *so,const Address &fromaddr);
    void onClientConnected(ClientSocket *s);
    void onClientDisconected(ClientSocket *s);
    friend class SocketClient;

private:
    TCPListenSocket * socket;
    map<NetClientID, ClientSocket *> clients;

};

#endif
