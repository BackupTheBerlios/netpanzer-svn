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

#include "SocketHeaders.hpp"
#include "SocketSet.hpp"
#include "NetworkException.hpp"

namespace network
{

SocketSet::SocketSet() : maxfd(0)
{
    FD_ZERO(&readset);
    FD_ZERO(&writeset);
}

SocketSet::~SocketSet()
{
}

void
SocketSet::clear()
{
    maxfd=0;
    FD_ZERO(&readset);
    FD_ZERO(&writeset);
}

void
SocketSet::add(const SocketBase* socket)
{
    FD_SET(socket->sockfd, &readset);
    SETMAXFD(maxfd,socket->sockfd);
}

void
SocketSet::addWrite(const SocketBase* socket)
{
    FD_SET(socket->sockfd, &writeset);
    SETMAXFD(maxfd,socket->sockfd);
}

void
SocketSet::remove(const SocketBase* socket)
{
    FD_CLR(socket->sockfd, &readset);
}

void
SocketSet::removeWrite(const SocketBase* socket)
{
    FD_CLR(socket->sockfd, &writeset);
}

bool
SocketSet::select(unsigned int usec) throw(NetworkException)
{
    struct timeval timeout;
    timeout.tv_sec = usec/1000000;
    timeout.tv_usec = usec%1000000;

#ifdef USE_WINSOCK
    if( (readset.fd_count==0) && (writeset.fd_count==0) )
	return false;
#endif
    
    int res = ::select(maxfd+1, &readset, &writeset, 0, &timeout);
    if ( res == SOCKET_ERROR ) {
        int error = GET_NET_ERROR();
        std::stringstream msg;
        msg << "Select failed: " << NETSTRERROR(error);
        throw NetworkException(msg.str());
    }

    return res != 0;
}

}
