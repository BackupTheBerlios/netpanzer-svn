#include <config.h>

#include <string.h>
#include "ServerSocket.hpp"
#include "Client.hpp"

Client::Client(ServerSocket* newserver)
		: tcpsocket(0), headerincomplete(false),
		  messageincomplete(0), udpenabled(false), tempoffset(0),
		  id(0), server(newserver)
{
	memset(&udpaddress, 0, sizeof(IPaddress));
}

Client::~Client()
{
	server->closeConnection(this);
}
