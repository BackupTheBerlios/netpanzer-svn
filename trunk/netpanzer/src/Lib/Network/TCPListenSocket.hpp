#ifndef __TCPLISTENSOCKET_HPP__
#define __TCPLISTENSOCKET_HPP__

#include <assert.h>
#include "TCPSocket.hpp"

namespace network
{

class TCPListenSocket : public TCPSocket
{
public:
    /** creates a new listen socket and binds it to the specified address */
    TCPListenSocket(const Address& bindaddr, bool blocking = true);
    /** creates a new listen socket and binds it to the specified port */
    TCPListenSocket(uint16_t port, bool blocking = true);

    /** accepts a new client and returns it's socket or 0 when no client
     * is connecting and we're in nonblocking mode 
     */
    TCPSocket* accept();

private:
    void createBind(bool blocking);
    
    // forbidden on listening socket
    void send(const void* , size_t )
    { assert(false); }
    size_t recv(void* , size_t )
    { assert(false); }

#ifdef WINSOCK
    bool blocking;
#endif
};

}

#endif

