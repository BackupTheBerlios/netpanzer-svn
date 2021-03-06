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
#ifndef _NETWORK_CLIENT_HPP
#define _NETWORK_CLIENT_HPP

#include <string>

#include "NetworkInterface.hpp"
#include "NetworkReturnCodes.hpp"
#include "Network/ClientSocket.hpp"

#include "Util/Timer.hpp"
#include "ArrayUtil/ArrayTemplate.hpp"

enum { _connection_status_no_connection,
       _connection_status_connected
     };

#define _CLIENT_KEEP_ALIVE_SEND_INTERVAL   (2)  // in seconds
#define _SERVER_KEEP_ALIVE_THRESHOLD     (120)  // in seconds
#define _SERVER_PING_INTERVAL              (5)  // in seconds

class NetworkClient : public NetworkInterface, ClientSocketObserver
{
protected:
    NetPacket net_packet;
    unsigned short connection_status;

    void netMessageClientKeepAlive(const NetMessage* message);
    void netMessageClientSetKeepAliveState(const NetMessage* message);
    void netMessageClientPingAck(const NetMessage* message);
    void netMessageClientConnectAck(const NetMessage* message);

    void processNetMessage(const NetMessage* message);

    void onClientConnected(ClientSocket *s);
    void onClientDisconected(ClientSocket *s, const char *msg);    

public:
    NetworkClient ();
    virtual ~NetworkClient ();

    bool joinServer(const std::string& server_name);
    void partServer();

    void sendMessage(NetMessage* message, size_t size);
    void sendRemaining();

    bool getMessage(NetMessage *message);


    void checkIncoming();

private:
    ClientSocket* clientsocket;
};

extern NetworkClient *CLIENT;

#endif // ** _NETWORK_CLIENT_HPP
