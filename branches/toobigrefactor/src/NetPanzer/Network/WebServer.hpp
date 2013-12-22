/*
Copyright (C) 2013 Netpanzer Team. (www.netpanzer.org), Aaron Perez

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
 *
 * Created on October 9, 2013, 8:16 PM
 */

#ifndef WEBSERVER_HPP
#define	WEBSERVER_HPP

#include "Network/TCPListenSocket.hpp"
#include "ArrayUtil/PtrArray.hpp"

using network::TCPListenSocket;
using network::TCPSocketObserver;
using network::Address;

class HTTPServerSocket;

class WebServer : public TCPListenSocket::Observer
{
public:
    WebServer();
    ~WebServer();
    
    bool host();
    
    void onDisconnected(HTTPServerSocket * so);
    void logic();
    
protected:
    TCPSocketObserver * onNewConnection(TCPListenSocket *so, const Address &fromaddr);
    void onSocketError(TCPListenSocket *so);
    
private:
    WebServer(const WebServer&);
    void operator=(const WebServer&);
    
    TCPListenSocket * socket;
    PtrArray<HTTPServerSocket> http_sockets;
    
};

#endif	/* WEBSERVER_HPP */

