/*
Copyright (C) 2004 by Matthias Braun <matze@braunis.de>

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
#ifndef __SOCKETCLIENT_HPP__
#define __SOCKETCLIENT_HPP__

#include <stdint.h>
#include "NetPacket.hpp"

class ServerSocket;

namespace network {
class TCPSocket;
}

/** This class keep data from a single client that is connected to the server
 */
class SocketClient
{
public:
    SocketClient(ServerSocket* server);
    ~SocketClient();

    network::TCPSocket* socket;
    
    char tempbuffer[_MAX_NET_PACKET_SIZE];
    uint16_t tempoffset;

    /// this variable is set to true, when the Client should be removed from
    /// client list in the next iteratrion
    bool wantstodie;

    NetClientID id;

    ServerSocket* server;
};

#endif

