#include <config.h>

#include <sstream>
#include <stdexcept>

#include "SocketHeaders.hpp"
#include "Address.hpp"
#include "TCPSocket.hpp"

namespace network
{

TCPSocket::TCPSocket(const Address& newaddr, bool blocking)
    : addr(newaddr)
{
    create(true);

    try {
        int res = connect(sockfd, (struct sockaddr*) &addr.addr,
                sizeof(addr.addr));
        if(res < 0) {
            std::stringstream msg;
            msg << "Couldn't connect to '" << addr.getIP() << "' port "
                << addr.getPort() << ": " << strerror(errno);
            throw std::runtime_error(msg.str());
        }

        if(!blocking)
            setNonBlocking();
    } catch(...) {
        close();
        throw;
    }
}

TCPSocket::~TCPSocket()
{
}

void
TCPSocket::send(const void* data, size_t size)
{
    int res = ::send(sockfd, (const char*) data, size, 0);
    if(res < 0) {
#ifdef WINSOCK
        std::stringstream msg;
        msg << "Send Error (code " << WSAGetLastError() << ")";
        throw std::runtime_error(msg.str());
#else
        std::stringstream msg;
        msg << "Send error: " << strerror(errno);
        throw std::runtime_error(msg.str());
#endif
    }
    if(res != (int) size) {
        std::stringstream msg;
        msg << "Send error: Couldn't send all data.";
        throw std::runtime_error(msg.str());
    }
}

size_t
TCPSocket::recv(void* buffer, size_t size)
{
    int res = ::recv(sockfd, (char*) buffer, size, 0);

    if(res < 0) {
#ifdef WINSOCK
        if(WSAGetLastError() == WSAEWOULDBLOCK)
            return 0;
        std::stringstream msg;
        msg << "Read error (code " << WSAGetLastError() << ")";
        throw std::runtime_error(msg.str());
#else
        if(errno == EWOULDBLOCK)
            return 0;
        std::stringstream msg;
        msg << "Read error: " << strerror(errno);
        throw std::runtime_error(msg.str());
#endif
    }

    return res;
}

TCPSocket::TCPSocket()
{
}

TCPSocket::TCPSocket(int fd, const Address& newaddr)
    : addr(newaddr)
{
    sockfd = fd;
}

}

