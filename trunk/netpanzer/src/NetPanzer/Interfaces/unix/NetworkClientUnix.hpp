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
#ifndef _NETWORK_CLIENT_WINSOCK_HPP
#define _NETWORK_CLIENT_WINSOCK_HPP

#include "NetworkClient.hpp"
#include "UILib/Network/ClientSocket.hpp"

class NetworkClientUnix : public NetworkClient
{
public:  
	NetworkClientUnix();
	virtual ~NetworkClientUnix();

	virtual int openSession();
   
	virtual int startEnumeration(ConnectionAddress address);
	virtual int startEnumeration();
	virtual int stopEnumeration();
	virtual int getSessionList(SessionList &list);

	virtual int joinSession();
	virtual int joinSession(int session_index);
	virtual int joinSession(const char* session_name);
	virtual int setJoinSession(const char *session_name);
	virtual int closeSession();
  
	virtual void sendMessage(NetMessage *message, size_t size, int flags);
	virtual int getMessage(NetMessage *message);

private:
	ClientSocket* clientsocket;
};

#endif // ** _NETWORK_CLIENT_WINSOCK_HPP
