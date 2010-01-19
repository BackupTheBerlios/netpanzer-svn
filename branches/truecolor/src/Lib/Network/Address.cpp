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

#include <sstream>
#include <string>
#include <string.h>

#include "Address.hpp"
#include "NetworkException.hpp"

namespace network
{

Address Address::ANY;

Address::Address()
{
    memset(&ss, 0, sizeof(ss));
    ss_len=sizeof(ss);
    ss.ss_family = AF_INET;
    //addr.sin_addr.s_addr = INADDR_ANY;
}

Address::Address(const Address& other)
{
    memcpy(&ss, &other.ss, sizeof(ss));
    ss_len=other.ss_len;
}

void
Address::operator=(const Address& other)
{
    if(&other == this) // ignore self assignment
        return;
    memcpy(&ss, &other.ss, sizeof(ss));
    ss_len=other.ss_len;
}

bool
Address::operator==(const Address& other) const
{
    return !memcmp(&ss,&other.ss,ss_len); // todo: check for ss_len too
}

std::string
Address::getIP() const
{
    // XXX quick hack to get it working until get full ss support
    // TODO: make a real ipaddress conversion function
    return std::string(inet_ntoa(((struct sockaddr_in &)ss).sin_addr));
}

unsigned short
Address::getPort() const
{
    // XXX quick hack to get it working until get full ss support
    // TODO: make a real port conversion function
    return ntohs(((struct sockaddr_in &)ss).sin_port);
}

Address
Address::resolve(const std::string& name, unsigned short port)
    throw(NetworkException)
{
    // TODO: make a better resolver (ex: getaddrinfo)
    Address result;
    // XXX quick hack to get it working until get full ss support
    ((struct sockaddr_in &)result.ss).sin_port = htons(port);
    
    if(name == "") {
        return result;
    }

    struct hostent* hentry = gethostbyname(name.c_str());
    if(!hentry) {
        std::stringstream msg;
        msg << "Couldn't resolve address '" << name;
#ifdef _WIN32
        msg << "' (code " << WSAGetLastError() << ")";
#else
        msg << "': " << hstrerror(h_errno);
#endif
        throw NetworkException(msg.str());

    }
    // XXX quick hack to get it working until get full ss support
    ((struct sockaddr_in &)result.ss).sin_addr.s_addr = ((struct in_addr*) hentry->h_addr)->s_addr;

    return result;
}

}
