#ifndef __UDPSOCKET_HPP__
#define __UDPSOCKET_HPP__

#include <stdint.h>
#include <string.h>
#include "SocketBase.hpp"

namespace network
{

class Address;

class UDPSocket : public SocketBase
{
public:
    /** creates a socket and binds it to the specified port or a random one when
     * port == 0*/
    UDPSocket(uint16_t port = 0, bool blocking = true);
    ~UDPSocket();

    /** send data to the specified address */
    void send(const Address& addr, const void* data, size_t datasize);
    /** receives data from socket. Returns the number of bytes read and stores
     * the address of the client who sent the data in addr.
     */
    size_t recv(Address& addr, void* buffer, size_t bufsize);
};

}

#endif

