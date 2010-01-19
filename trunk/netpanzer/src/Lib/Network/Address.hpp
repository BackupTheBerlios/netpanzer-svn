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
#ifndef __ADDRESS_HPP__
#define __ADDRESS_HPP__

#include "SocketHeaders.hpp"
#include "NetworkException.hpp"
#include <string>

namespace network
{

class Address
{
public:
    Address();
    Address(const Address& other);
    
    /** resolves a hostname or IP-Number together with a port and returns a
     * new Address object.
     */
    static Address resolve(const std::string& name, unsigned short port) throw(NetworkException);

    /** returns the ip address of this Address as string */
    std::string getIP() const;
    /** returns the port of this address */
    unsigned short getPort() const;

    void operator=(const Address& other);
    bool operator==(const Address& other) const;

    struct sockaddr * getSockaddr() const { return (struct sockaddr *)&ss; };
    socklen_t getSockaddrLen() const
    {
#ifdef __APPLE__
        return sizeof(sockaddr_in); 
#else
        return ss_len;
#endif
    };
    socklen_t * getSockaddrLenPointer() { return &ss_len; };
    
    static Address ANY;

private:
    socklen_t ss_len;
    struct sockaddr_storage ss;
};

}

#endif

