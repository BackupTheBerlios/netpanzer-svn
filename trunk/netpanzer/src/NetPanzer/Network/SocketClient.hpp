#ifndef __SOCKETCLIENT_HPP__
#define __SOCKETCLIENT_HPP__

#include <stdint.h>

class ServerSocket;

namespace network {
class TCPSocket;
}

/** This class keep data from a single client that is connected to the server
 */
class SocketClient
{
public:
    SocketClient(ServerSocket* server);
    ~SocketClient();

    network::TCPSocket* socket;
    
    char tempbuffer[512];

    bool headerincomplete;
    bool messageincomplete;
    bool udpenabled;
    short tempoffset;

    /// this variable is set to true, when the Client should be removed from
    /// client list in the next iteratrion
    bool wantstodie;

    typedef uint32_t ID;
    ID id;

    ServerSocket* server;
};

#endif

