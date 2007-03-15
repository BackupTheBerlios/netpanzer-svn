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
#ifdef USE_WINSOCK
    this->blocking = blocking;
#endif
}

void
TCPListenSocket::createBind(bool blocking)
{
    int val = 1;
    int res = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    if(res == SOCKET_ERROR) {
        int error = GET_NET_ERROR();
        std::stringstream msg;
        msg << "Couldn't set SO_REUSEADDR: ";
        printError(msg,error);
        throw std::runtime_error(msg.str());
    }
    
    res = bind(sockfd, addr.getSockaddr(), addr.getSockaddrLen());
    if(res == SOCKET_ERROR) {
        int error = GET_NET_ERROR();
        std::stringstream msg;
        msg << "Couldn't bind socket to address '"
            << addr.getIP() << "' port " << addr.getPort()
            << ": ";
        printError(msg,error);
        throw std::runtime_error(msg.str());
    }

    res = listen(sockfd, 20);
    if(res == SOCKET_ERROR) {
        int error = GET_NET_ERROR();
        std::stringstream msg;
        msg << "Couldn't listen on socket: ";
        printError(msg,error);
        throw std::runtime_error(msg.str());
    }

    if(!blocking)
        setNonBlocking();
}

TCPSocket*
TCPListenSocket::accept()
{
    Address addr;
    SOCKET newsock;
    newsock= ::accept(sockfd, addr.getSockaddr(), addr.getSockaddrLenPointer());
    if (newsock == INVALID_SOCKET) {
        int error = GET_NET_ERROR();
        if ( error == NETERROR_WOULDBLOCK )
            return 0;
        std::stringstream msg;
        msg << "Accept error: ";
        printError(msg,error);
        throw std::runtime_error(msg.str());
    }

    TCPSocket* result;
    try {
        result = new TCPSocket(newsock, addr);
    } catch(...) {
        close();
        throw;
    }

#ifdef USE_WINSOCK
    try {
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
