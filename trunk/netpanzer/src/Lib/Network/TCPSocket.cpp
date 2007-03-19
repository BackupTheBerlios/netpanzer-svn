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

TCPSocket::TCPSocket(const Address& newaddr, bool blocking) :SocketBase(newaddr)
{
    create(true);
    init(blocking);
}

TCPSocket::TCPSocket(const Address& bindaddr, const Address& newaddr,
        bool blocking) : SocketBase(newaddr)
{
    create(true);
    bindSocketTo(bindaddr);
    init(blocking);
}

void
TCPSocket::init(bool blocking)
{
    try {
        doConnect();
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
    int res = doSend(data,size);
    if(res != (int) size) {
        std::stringstream msg;
        msg << "Send error: Couldn't send all data.";
        throw std::runtime_error(msg.str());
    }
}

size_t
TCPSocket::recv(void* buffer, size_t size)
{
    return doReceive(buffer,size);
}

TCPSocket::TCPSocket()
{
}

TCPSocket::TCPSocket(SOCKET fd, const Address& newaddr) : SocketBase(fd,newaddr)
{
}

}

