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
#ifndef __TCPLISTENSOCKET_HPP__
#define __TCPLISTENSOCKET_HPP__

#include <assert.h>
#include "SocketBase.hpp"

namespace network
{

class TCPListenSocket : public SocketBase
{
public:
    /** creates a new listen socket and binds it to the specified address */
    TCPListenSocket(const Address& bindaddr, bool blocking = true);

    /** accepts a new client and returns it's socket or 0 when no client
     * is connecting and we're in nonblocking mode 
     */
    TCPSocket* accept();

private:
    // forbidden on listening socket
    void send(const void* , size_t )
    { assert(false); }
    size_t recv(void* , size_t )
    { assert(false); }

#ifdef USE_WINSOCK
    bool blocking;
#endif
};

}

#endif

