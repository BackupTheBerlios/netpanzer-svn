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
#ifndef _NETWORK_SERVER_UNIX_HPP
#define _NETWORK_SERVER_UNIX_HPP

#include "Classes/Network/NetworkServer.hpp"
#include "Network/ServerSocket.hpp"

/** This class implements a bsd/unix socket implementation of the
 * NetworkServer interface.
 */
class NetworkServerUnix : public NetworkServer
{
public:
    NetworkServerUnix();
    virtual ~NetworkServerUnix();

    virtual void openSession();
    virtual void hostSession();
    virtual void closeSession();

    virtual void sendMessage(NetMessage *message, size_t size);
    virtual void sendMessage(NetClientID network_id,
            NetMessage *message, size_t size);
    void sendRemaining() { if ( serversocket ) serversocket->sendRemaining(); };

    virtual bool getPacket(NetPacket* message);

    virtual void shutdownClientTransport(NetClientID network_id);

    virtual void checkIncoming();

    std::string getIP(NetClientID network_id) const;

private:
    ServerSocket* serversocket;
};

#endif
