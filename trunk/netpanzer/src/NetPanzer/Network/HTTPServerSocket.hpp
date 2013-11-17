/*
Copyright (C) 2012 by Aaron Perez <aaronps@gmail.com>

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
 * Created on December 23, 2012, 11:00 AM
 */

#ifndef HTTPSERVERSOCKET_HPP
#define	HTTPSERVERSOCKET_HPP

#include "Network/TCPSocket.hpp"
#include "Util/FileSystem.hpp"
#include "Core/CoreTypes.hpp"
#include <stdint.h>
#include <string>
#include <map>

#include "HTTP/HTTPRequest.hpp"

#define HTTPSERVERSOCKET_RBUFFER_SIZE (16384)
#define HTTPSERVERSOCKET_SBUFFER_SIZE (16384)

class WebServer;

class HTTPServerSocket : public network::TCPSocketObserver
{
public:
    HTTPServerSocket(WebServer * webserver);
    virtual ~HTTPServerSocket();
    
    void close();
    void work();
    
    const network::Address& getAddress() const;
    
protected:
    void onDataReceived(network::TCPSocket *so, const char *data, const int len);
    void onConnected(network::TCPSocket *so);
    void onDisconected(network::TCPSocket *so);
    void onSocketError(network::TCPSocket *so);

private:
    WebServer * webserver;
    network::TCPSocket * socket;
    
    filesystem::ReadFile * sending_file;
    unsigned int sending_file_remain;
    
    uint8_t receive_buffer[HTTPSERVERSOCKET_RBUFFER_SIZE];
    int receive_length;
    
    uint8_t send_buffer[HTTPSERVERSOCKET_SBUFFER_SIZE];
    size_t send_length;

    HTTPRequest request;
    
    int state;
    
    template<int> void onReceiveState(const char * data, const int len);
    
    void try_handle_request();
    bool decode_request();
    bool handle_request();
    
};

#endif	/* HTTPSERVERSOCKET_HPP */

