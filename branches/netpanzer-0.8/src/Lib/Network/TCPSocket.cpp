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
#include "Util/Log.hpp"


namespace network
{

TCPSocket::TCPSocket(SOCKET fd, const Address& newaddr, TCPSocketObserver *o)
    throw(NetworkException)
    : SocketBase(fd,newaddr), observer(o)
{

}

TCPSocket::TCPSocket(const Address& address, TCPSocketObserver *o)
    throw(NetworkException)
    : SocketBase(address,true), observer(o)
{
    doConnect();
}

TCPSocket::~TCPSocket()
{ }

void
TCPSocket::destroy()
{
    observer=0;
    doClose();
}

size_t
TCPSocket::send(const void* data, size_t size) throw(NetworkException)
{
    int res = doSend(data,size);
    if (!res && !observer) // disconnected
        return size; // as is disconnected, avoid catching of unsend data
    return res;
}

void
TCPSocket::onConnected()
{
    if (observer)
        observer->onConnected(this);
}

void
TCPSocket::onDisconected()
{
    if (observer)
        observer->onDisconected(this);
    destroy();
}

void
TCPSocket::onDataReady()
{
    char buffer[4096];
    int len;
    do {
        len = doReceive(buffer,sizeof(buffer));
        if (len && observer)
            observer->onDataReceived(this, buffer,len);
    } while (len && observer);
}

}
