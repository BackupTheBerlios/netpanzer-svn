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
#ifndef __TCPSOCKET_HPP__
#define __TCPSOCKET_HPP__

#include "SocketBase.hpp"
#include "Address.hpp"

namespace network
{

class TCPSocket : public SocketBase
{
public:
    /** connects to a remote host */
    TCPSocket(const Address& address, bool blocking = true);
    TCPSocket(const Address& bindaddr, const Address& address,
            bool blocking = true);
    ~TCPSocket();

    /** send data to the socket */
    void send(const void* data, size_t datasize);
    /** receives data from the socket and copies it into the buffer.
     * returns number of read bytes
     */
    size_t recv(void* buffer, size_t bufsize);

    /** returns the address associated with this socket */
    const Address& getAddress() const
    {
        return addr;
    }

private:
    friend class TCPListenSocket;
    friend class SocketStreamBuf;

    TCPSocket();
    TCPSocket(SOCKET fd, const Address& addr);

    void init(const Address& bindaddr, bool blocking);

    Address addr;
};

}

#endif

