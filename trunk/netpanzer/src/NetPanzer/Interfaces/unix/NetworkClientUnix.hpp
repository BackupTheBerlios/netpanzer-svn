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
#ifndef _NETWORK_CLIENT_UNIX_HPP
#define _NETWORK_CLIENT_UNIX_HPP

#include "Classes/Network/NetworkClient.hpp"
#include "Network/ClientSocket.hpp"

class NetworkClientUnix : public NetworkClient, public ClientSocketObserver
{
public:
    NetworkClientUnix();
    virtual ~NetworkClientUnix();

    virtual bool joinServer(const std::string& server_name);
    virtual void partServer();

    virtual void sendMessage(NetMessage* message, size_t size);
    void sendRemaining() { if ( clientsocket ) clientsocket->sendRemaining(); };
    virtual bool getMessage(NetMessage *message);

    virtual void checkIncoming();
protected:
    void onClientConnected(ClientSocket *s);
    void onClientDisconected(ClientSocket *s, const char *msg);    
    
private:
    ClientSocket* clientsocket;
};

#endif
