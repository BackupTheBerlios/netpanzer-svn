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
    : SocketBase(newaddr)
{
    try {
        create(true);
        setReuseAddr();
        bindSocket();
        doListen();
    
        if(!blocking)
            setNonBlocking();

    } catch(...) {
        close();
        throw;
    }
#ifdef USE_WINSOCK
    this->blocking = blocking;
#endif
}

TCPSocket*
TCPListenSocket::accept()
{
    Address newaddr;
    SOCKET newsock;
    newsock = doAccept(newaddr);
    if (newsock == INVALID_SOCKET)
        return 0; 
    TCPSocket* result;
    try {
        result = new TCPSocket(newsock, newaddr);
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
