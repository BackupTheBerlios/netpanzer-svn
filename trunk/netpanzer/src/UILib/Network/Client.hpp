#ifndef __CLIENT_HPP__
#define __CLIENT_HPP__

#include <SDL_net.h>

/** This class keep data from a single client that is connected to the server
 */
class Client
{
public:
	Client();
	~Client();
	
	TCPsocket tcpsocket;
	IPaddress udpaddress;

	char tempbuffer[512];
 
	bool headerincomplete;
	bool messageincomplete;
	bool udpenabled;
	short tempoffset;

	typedef Uint32 ID;
	ID id;
};

#endif

