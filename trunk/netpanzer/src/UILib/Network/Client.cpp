#include <config.h>

#include <string.h>
#include "Client.hpp"

Client::Client()
		: tcpsocket(0), headerincomplete(false),
		  messageincomplete(0), udpenabled(false), tempoffset(0),
		  id(0)
{
	memset(&udpaddress, 0, sizeof(IPaddress));
}

Client::~Client()
{
	SDLNet_TCP_Close(tcpsocket);
}
