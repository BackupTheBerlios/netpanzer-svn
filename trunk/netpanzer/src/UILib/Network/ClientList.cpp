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
{
}

ClientList::~ClientList()
{
}

Client* ClientList::add(TCPsocket socket)
{
	Client* client = new Client();
	client->tcpsocket = socket;
	client->id = nextid++;
	clients.push_back(client);

	return client;
}

Client* ClientList::getClientFromID(Client::ID id)
{
	for(ClientIterator i = begin(); i != end(); i++)
	{
		Client* client = *i;
		if(client->id == id)
			return client;
	}

	return 0;
}

void ClientList::remove(Client* client)
{
	for(ClientIterator i = begin(); i != end(); i++)
	{
		if(client == *i) {
			clients.erase(i);
			break;
		}
	}
	delete client;
}

void ClientList::removeAll()
{
	clients.clear();
}
