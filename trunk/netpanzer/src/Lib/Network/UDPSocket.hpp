#ifndef __UDPSOCKET_HPP__
#define __UDPSOCKET_HPP__

#include <stdint.h>
#include <string.h>
#include "SocketBase.hpp"
#include "Address.hpp"

namespace network
{

class UDPSocket : public SocketBase
{
public:
    /** creates a new socket and binds it to the specified address and port or a
     * random port if port == 0
     */
    UDPSocket(bool blocking = true);
    UDPSocket(const Address& addr, bool blocking = true);
    ~UDPSocket();

    /** send data to the specified address */
    void send(const Address& addr, const void* data, size_t datasize);
    /** receives data from socket. Returns the number of bytes read and stores
     * the address of the client who sent the data in addr.
     */
    size_t recv(Address& addr, void* buffer, size_t bufsize);

private:
    void init(const Address& addr, bool blocking);
};

}

#endif

