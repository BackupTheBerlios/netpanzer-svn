#include <config.h>

#include <string.h>
#include <assert.h>
#include "ServerSocket.hpp"
#include "SocketClient.hpp"

SocketClient::SocketClient(ServerSocket* newserver)
        : socket(0), headerincomplete(false),
        messageincomplete(0), udpenabled(false), tempoffset(0),
        wantstodie(false), id(0), server(newserver)
{
}

SocketClient::~SocketClient()
{
    assert(wantstodie == true);

    server->closeConnection(this);
}

