#ifndef __TCPSOCKET_HPP__
#define __TCPSOCKET_HPP__

#include "SocketBase.hpp"
#include "Address.hpp"

namespace network
{

class TCPSocket : public SocketBase
{
public:
    /** connects to a remote host */
    TCPSocket(const Address& address, bool blocking = true);
    ~TCPSocket();

    /** send data to the socket */
    void send(const void* data, size_t datasize);
    /** receives data from the socket and copies it into the buffer.
     * returns number of read bytes
     */
    size_t recv(void* buffer, size_t bufsize);
    /** accepts a new client and returns it's socket or 0 when no client
     * is connecting and we're in nonblocking mode 
     */
    TCPSocket* accept();

    /** returns the address associated with this socket */
    const Address& getAddress() const
    {
        return addr;
    }

private:
    friend class TCPListenSocket;
    friend class SocketStreamBuf;

    TCPSocket();
    TCPSocket(int fd, const Address& addr);

    Address addr;
};

}

#endif

