#include <config.h>

#include <string.h>
#include <assert.h>
#include "ServerSocket.hpp"
#include "SocketClient.hpp"

SocketClient::SocketClient(ServerSocket* newserver)
        : tcpsocket(0), headerincomplete(false),
        messageincomplete(0), udpenabled(false), tempoffset(0),
        wantstodie(false), id(0), server(newserver)
{
    memset(&udpaddress, 0, sizeof(IPaddress));
}

SocketClient::~SocketClient()
{
    assert(wantstodie == true);

    server->closeConnection(this);
}

