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

#include "ClientList.hpp"

class BasicGameInfo;

class ServerSocket
{
public:
	ServerSocket(Uint16 tcpport);
	~ServerSocket();

	void read();
	void sendMessage(Client::ID toclient, char* data, size_t datasize,
					 bool realiable = true);
	void removeClient(Client::ID clientid);

protected:
	friend class Client;
	void closeConnection(Client* client);

private:
	void acceptNewClients();
	void readTCP();
	void readClientTCP(Client* client);
	
	TCPsocket tcpsocket;
	SDLNet_SocketSet sockets;
	ClientList* clientlist;
};

#endif
