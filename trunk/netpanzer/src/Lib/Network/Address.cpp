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

#include <stdexcept>
#include <sstream>

#include "SocketHeaders.hpp"
#include "Address.hpp"

namespace network
{

Address Address::ANY;

Address::Address()
{
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
}

Address::Address(const Address& other)
{
    memcpy(&addr, &other.addr, sizeof(addr));
}

void
Address::operator=(const Address& other)
{
    if(&other == this) // ignore self assignment
        return;
    memcpy(&addr, &other.addr, sizeof(addr));
}

bool
Address::operator==(const Address& other) const
{
    if(addr.sin_addr.s_addr == other.addr.sin_addr.s_addr
            && addr.sin_port == other.addr.sin_port)
        return true;

    return false;
}

std::string
Address::getIP() const
{
    return std::string(inet_ntoa(addr.sin_addr));
}

uint16_t
Address::getPort() const
{
    return ntohs(addr.sin_port);
}

Address
Address::resolve(const std::string& name, uint16_t port)
{
    Address result;
    result.addr.sin_port = htons(port);
    
    if(name == "") {
        return result;
    }

    struct hostent* hentry = gethostbyname(name.c_str());
    if(!hentry) {
#ifdef WINSOCK
        std::stringstream msg;
        msg << "Couldn't resolve address '" << name << "' (code "
            << WSAGetLastError() << ")";
        throw std::runtime_error(msg.str());
#else
        std::stringstream msg;
        msg << "Couldn't resolve address '" << name << "': "
            << hstrerror(h_errno);
        throw std::runtime_error(msg.str());
#endif
    }

    result.addr.sin_addr.s_addr = ((struct in_addr*) hentry->h_addr)->s_addr;

    return result;
}

}
