#include <config.h>

#include <sstream>
#include <stdexcept>

#include "SocketHeaders.hpp"
#include "TCPSocket.hpp"
#include "TCPListenSocket.hpp"

namespace network
{
    
TCPListenSocket::TCPListenSocket(const Address& newaddr, bool blocking)
{
    addr = newaddr;
    create(true);

    try {
        createBind(blocking);
    } catch(...) {
        close();
        throw;
    }
#ifdef WINSOCK
    this->blocking = blocking;
#endif
}

void
TCPListenSocket::createBind(bool blocking)
{
    int res = bind(sockfd, (struct sockaddr*) &addr.addr,
            sizeof(addr.addr));
    if(res < 0) {
        std::stringstream msg;
        msg << "Couldn't bind socket to address '"
            << addr.getIP() << "' port " << addr.getPort()
            << ": " << strerror(errno);
        throw std::runtime_error(msg.str());
    }

    res = listen(sockfd, 20);
    if(res < 0) {
        std::stringstream msg;
        msg << "Couldn't listen on socket: " << strerror(errno);
        throw std::runtime_error(msg.str());
    }

    if(!blocking)
        setNonBlocking();
}

TCPSocket*
TCPListenSocket::accept()
{
    Address addr;
    socklen_t socklen = sizeof(addr.addr);
    int clientsockfd 
        = ::accept(sockfd, (struct sockaddr*) &addr.addr, &socklen);

    if(clientsockfd < 0) {
#ifdef WINSOCK
        if(WSAGetLastError() == WSAEWOULDBLOCK)
            return 0;
        std::stringstream msg;
        msg << "Accept error (code " << WSAGetLastError();
        throw std::runtime_error(msg.str());               
#else
        if(errno == EWOULDBLOCK)
            return 0;
        std::stringstream msg;
        msg << "Accept error: " << strerror(errno);
        throw std::runtime_error(msg.str());
#endif
    }

    TCPSocket* result;
    try {
        result = new TCPSocket(clientsockfd, addr);
    } catch(...) {
        close();
        throw;
    }

#ifdef WINSOCK
    try {
        // TODO on win32 you have to explicitely set the client sockets to
        // nonblocking too
        if(!blocking)
            result->setNonBlocking();
    } catch(...) {
        delete result;
        throw;
    }
#endif

    return result;
}

}
