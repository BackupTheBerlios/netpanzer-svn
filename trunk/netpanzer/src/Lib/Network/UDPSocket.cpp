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

//UDPSocket::UDPSocket(bool blocking) : SocketBase(Address::ANY) { init(blocking); }

//UDPSocket::UDPSocket(const Address& bindaddr, bool blocking) : SocketBase(bindaddr) { init(blocking); }

UDPSocket::UDPSocket(UDPSocketObserver *o) : SocketBase(Address::ANY)
{
    observer=o;
    init(false);
}

UDPSocket::UDPSocket(const Address& bindaddr, UDPSocketObserver *o)
    : SocketBase(bindaddr)
{
    observer=o;
    init(false);
}

UDPSocket::~UDPSocket()
{
}

void
UDPSocket::destroy()
{
    observer=0;
    doClose();
}

void
UDPSocket::init(bool blocking)
{
    create(false);

    try {
        bindSocket();
        if(!blocking)
            setNonBlocking();
    } catch(...) {
        doClose();
        throw;
    }
}

void
UDPSocket::send(const Address& toaddr, const void* data, size_t datasize)
{
    int res = doSendTo(toaddr,data,datasize);
    if(res != (int) datasize) {
        std::stringstream msg;
        msg << "Send error: not all data sent.";
        throw std::runtime_error(msg.str());
    }
}

size_t
UDPSocket::recv(Address& fromaddr, void* buffer, size_t bufsize)
{
    return doReceiveFrom(fromaddr,buffer,bufsize);
}

void
UDPSocket::onDataReady()
{
    Address a;
    char buffer[4096];
    int len;
    do {
        len=doReceiveFrom(a,buffer,sizeof(buffer));
        if (len && observer)
            observer->onDataReceived(this,a,buffer,len);
    } while (len);
}

}
