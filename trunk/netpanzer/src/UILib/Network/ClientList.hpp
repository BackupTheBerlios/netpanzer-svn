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
#ifndef _CLIENTLIST_H
#define _CLIENTLIST_H

#include <vector>
#include <SDL_net.h>
#include "Client.hpp"

class ClientList
{
public:
	ClientList();
	~ClientList();
	
	Client* add(TCPsocket socket);
	
	Client* getClientFromID(Client::ID id);

	typedef std::vector<Client*>::iterator ClientIterator;
	ClientIterator begin()
	{ return clients.begin(); }
	ClientIterator end()
	{ return clients.end(); }

	void remove(Client* client);
	void removeAll();
private:
	std::vector<Client*> clients;
	Client::ID nextid;
};

#endif // _WINSOCKCLIENTLIST_H
