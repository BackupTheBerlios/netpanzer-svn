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
#include "UDPSocket.hpp"
#include "Address.hpp"

namespace network
{

UDPSocket::UDPSocket(bool blocking)
{
    init(Address::ANY, blocking);
}

UDPSocket::UDPSocket(const Address& addr, bool blocking)
{
    init(addr, blocking);
}

void
UDPSocket::init(const Address& addr, bool blocking)
{
    create(false);

    try {
        int res = bind(sockfd, (struct sockaddr*) &addr.addr,
                sizeof(addr.addr));
        if(res < 0) {
            std::stringstream msg;
            msg << "Couldn't bind socket to address '"
                << addr.getIP() << "' port " << addr.getPort()
                << ": " << strerror(errno);
            throw std::runtime_error(msg.str());
        }

        if(!blocking)
            setNonBlocking();
    } catch(...) {
        close();
        throw;
    }
}

UDPSocket::~UDPSocket()
{
}

void
UDPSocket::send(const Address& addr, const void* data, size_t datasize)
{
    int res = sendto(sockfd, (const char*) data, datasize, 0,
            (struct sockaddr*) &addr.addr, sizeof(addr.addr));
    if(res < 0) {
#ifdef USE_WINSOCK
        std::stringstream msg;
        msg << "Send error (code " << WSAGetLastError() << ")";
        throw std::runtime_error(msg.str());
#else
        std::stringstream msg;
        msg << "Send error: " << strerror(errno);        
        throw std::runtime_error(msg.str());
#endif
    }
    if(res != (int) datasize) {
        std::stringstream msg;
        msg << "Send error: not all data sent.";
        throw std::runtime_error(msg.str());
    }
}

size_t
UDPSocket::recv(Address& addr, void* buffer, size_t bufsize)
{
    socklen_t socklen = sizeof(addr.addr);
    int res = recvfrom(sockfd, (char*) buffer, bufsize, 0,
            (struct sockaddr*) &addr.addr, &socklen);
    if(res < 0) {
#ifdef USE_WINSOCK
        if(WSAGetLastError() == WSAEWOULDBLOCK)
            return 0;
        std::stringstream msg;
        msg << "Receive error (code " << WSAGetLastError() << ")";
        throw std::runtime_error(msg.str());
#else
        if(errno == EWOULDBLOCK)
            return 0;
        std::stringstream msg;
        msg << "Receive error: " << strerror(errno);
        throw std::runtime_error(msg.str());
#endif
    }

    return res;
}

}
