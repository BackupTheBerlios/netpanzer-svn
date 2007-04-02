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
#ifndef __SOCKETBASE_HPP__
#define __SOCKETBASE_HPP__

#include <iostream>
#include "SocketHeaders.hpp"
#include "Address.hpp"
#include "Util/NoCopy.hpp"

namespace network
{

/** Base class for sockets, this is intended for internal use only */
class SocketBase : public NoCopy
{
public:
    const Address& getAddress() const
    {
        return addr;
    }

    bool isConnecting() { return _isConnecting; };
protected:
    SocketBase();
    SocketBase(const Address &a) : addr(a) {};
    SocketBase(SOCKET fd, const Address &a);
    
    virtual ~SocketBase();
    
    virtual void onDataReady() = 0;
    virtual void onDisconected() {};
    virtual void onConnected() {};
    virtual void destroy() = 0;

protected:
    friend class SocketSet;
    friend class SocketManager;
    void create(bool tcp);
    void setNonBlocking();
    void doClose();
    void printError(std::ostream& out, int e);
    
    void bindSocketTo(const Address& toaddr);
    void bindSocket() { bindSocketTo(addr); };
    void setReuseAddr();
    void doListen();
    void doConnect();
    int  doSend(const void* data, size_t len);
    int  doReceive(void* buffer, size_t len);
    int  doSendTo(const Address& toaddr, const void* data, size_t len);
    size_t  doReceiveFrom(Address& fromaddr, void* buffer, size_t len);
    SOCKET doAccept(Address& fromaddr);
    
private:
    
    void connectionFinished() 
    {
        _isConnecting = false;
        onConnected();
    };
    
    bool _isConnecting;
    SOCKET sockfd;
    Address addr;
};

}

#endif

