/*
Copyright (C) 2004 by Matthias Braun <matze@braunis.de>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
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
    init(Address::ANY, blocking);
}

TCPSocket::TCPSocket(const Address& bindaddr, const Address& newaddr,
        bool blocking) : addr(newaddr)
{
    init(bindaddr, blocking);
}

void
TCPSocket::init(const Address& bindaddr, bool blocking)
{
    create(true);

    try {
        int res = bind(sockfd, (struct sockaddr*) &bindaddr.addr,
                sizeof(bindaddr.addr));
#ifdef USE_WINSOCK
        if(res == SOCKET_ERROR) {
#else
        if(res < 0) {
#endif
            std::stringstream msg;
            msg << "Couldn't bind socket to address '"
                << bindaddr.getIP() << "' port " << bindaddr.getPort()
                << ": ";
            printError(msg);
            throw std::runtime_error(msg.str());           
        }
        
        res = connect(sockfd, (struct sockaddr*) &addr.addr,
                sizeof(addr.addr));
#ifdef USE_WINSOCK
        if(res == SOCKET_ERROR) {
#else
        if(res < 0) {
#endif
            std::stringstream msg;
            msg << "Couldn't connect to '" << addr.getIP() << "' port "
                << addr.getPort() << ": ";
            printError(msg);
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
        std::stringstream msg;
        msg << "Send error: ";
        printError(msg);
        throw std::runtime_error(msg.str());
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
#ifdef USE_WINSOCK
        if(WSAGetLastError() == WSAEWOULDBLOCK)
            return 0;
#else
        if(errno == EWOULDBLOCK)
            return 0;
#endif
        std::stringstream msg;
        msg << "Read error: ";
        printError(msg);
        throw std::runtime_error(msg.str());
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

