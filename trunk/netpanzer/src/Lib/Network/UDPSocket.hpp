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
#ifndef __UDPSOCKET_HPP__
#define __UDPSOCKET_HPP__

#include <stdint.h>
#include <string.h>
#include "SocketBase.hpp"
#include "Address.hpp"

namespace network
{
    
class UDPSocket;    
    
class UDPSocketObserver {
public:    
    UDPSocketObserver() {};
    virtual ~UDPSocketObserver() {};
protected:
    friend class UDPSocket;
    virtual void onDataReceived(UDPSocket *so, const Address &from, const char *data, const int len) = 0;
};


class UDPSocket : public SocketBase
{
public:
    /** creates a new socket and binds it to the specified address and port or a
     * random port if port == 0
     */
//    UDPSocket(bool blocking = true);
//    UDPSocket(const Address& bindaddr, bool blocking = true);
    UDPSocket(UDPSocketObserver *o);
    UDPSocket(const Address& bindaddr, UDPSocketObserver *o);

    void destroy();

    /** send data to the specified address */
    void send(const Address& toaddr, const void* data, size_t datasize);
    /** receives data from socket. Returns the number of bytes read and stores
     * the address of the client who sent the data in addr.
     */
    size_t recv(Address& fromaddr, void* buffer, size_t bufsize);
protected:
    ~UDPSocket();
    void onDataReady();

private:
    void init(bool blocking);
    UDPSocketObserver *observer;
};

}

#endif

