/*
Copyright (C) 2013 Netpanzer Team. (www.netpanzer.info), Aaron Perez

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
 * Created on October 9, 2013, 8:16 PM
 */

#include "WebServer.hpp"
#include "HTTPServerSocket.hpp"
#include "Util/Log.hpp"

#include "Interfaces/GameConfig.hpp"

WebServer::WebServer() : socket(0)
{
    
}

WebServer::~WebServer()
{
    if ( socket )
    {
        socket->destroy();
        socket = 0;
    }
}

bool WebServer::host()
{
    try
    {
        Address addr = Address::resolve(gameconfig->host.getBindAddress(),
                                        gameconfig->host.getPort(),
                                        true, true ); // tcp for binding
        
        socket = new TCPListenSocket(addr, this);
    }
    catch(...)
    {
        if (socket)
            socket->destroy();
        socket = 0;
        throw;
    }
    
    const Address& addr = socket->getAddress();
    LOGGER.warning("WebServer: listening on %s:%u", addr.getIP().c_str(), addr.getPort());
}

TCPSocketObserver * WebServer::onNewConnection(TCPListenSocket* so, const Address& fromaddr)
{
    (void)so;
    (void)fromaddr;
    LOGGER.warning("WebServer: New connection from %s:%u", fromaddr.getIP().c_str(), fromaddr.getPort());
    HTTPServerSocket * hss = new HTTPServerSocket(this);
    http_sockets.push_back(hss);
    return hss;
}

void WebServer::onSocketError(TCPListenSocket* so)
{
    (void)so;
    LOGGER.warning("WebServer: Listen socket error, something bad could happen from now");
}

void WebServer::onDisconnected(HTTPServerSocket* so)
{
    const Address& addr = so->getAddress();
    LOGGER.warning("WebServer: Disconnected from %s:%u", addr.getIP().c_str(), addr.getPort());
    for ( size_t n = 0; n < http_sockets.size(); n++ )
    {
        if ( http_sockets[n] == so )
        {
            http_sockets.erase(n);
            delete so;
            break;
        }
    }
}

void WebServer::logic()
{
    for ( size_t n = 0; n < http_sockets.size(); n++ )
    {
        http_sockets[n]->work();
        n++;
    }
}