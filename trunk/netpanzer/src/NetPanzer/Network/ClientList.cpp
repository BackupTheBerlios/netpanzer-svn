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
#include <config.h>
#include "ClientList.hpp"

ClientList::ClientList()
        : nextid(0)
{}

ClientList::~ClientList()
{
    for(ClientIterator i = begin(); i != end(); i++) {
        SocketClient* client = *i;
        client->wantstodie = true;
        delete client;
    }
    clients.clear();
}

SocketClient* ClientList::add(ServerSocket* server, network::TCPSocket* socket)
{
    SocketClient* client = new SocketClient(server);
    client->socket = socket;
    client->id = nextid++;
    clients.push_back(client);

    return client;
}

SocketClient* ClientList::getClientFromID(SocketClient::ID id)
{
    for(ClientIterator i = begin(); i != end(); i++) {
        SocketClient* client = *i;
        if(client->id == id)
            return client;
    }

    return 0;
}

