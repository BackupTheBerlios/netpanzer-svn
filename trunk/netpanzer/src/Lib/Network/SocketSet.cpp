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

#include "SocketHeaders.hpp"

#include <sstream>
#include <stdexcept>

#include "SocketSet.hpp"

namespace network
{

SocketSet::SocketSet()
{
    FD_ZERO(&set);
}

SocketSet::~SocketSet()
{
}

void
SocketSet::add(const SocketBase& socket)
{
    FD_SET(socket.sockfd, &set);
}

void
SocketSet::remove(const SocketBase& socket)
{
    FD_CLR(socket.sockfd, &set);
}

bool
SocketSet::select(unsigned int usec)
{
    struct timeval timeout;
    timeout.tv_sec = usec/1000000;
    timeout.tv_usec = usec%1000000;

    testset = set;
    int res = ::select(FD_SETSIZE, &testset, 0, 0, usec ? &timeout : 0);
#ifdef USE_WINSOCK
    if(res < 0) {
        std::stringstream msg;
        msg << "Select failed: " << WSAGetLastError();
        throw std::runtime_error(msg.str());
    }
#else
    if(res < 0) {
        std::stringstream msg;
        msg << "Select failed: " << strerror(errno);
        throw std::runtime_error(msg.str());
    }
#endif

    return res != 0;
}

}
