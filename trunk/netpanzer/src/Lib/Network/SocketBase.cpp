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


#include <iostream>
using namespace std;

#include <sstream>
#include <string.h>

#include "NetworkException.hpp"
#include "SocketHeaders.hpp"
#include "SocketBase.hpp"
#include "SocketManager.hpp"
#include "Util/Log.hpp"

namespace network
{

#ifdef _WIN32

class WinSockInit {
public:
    WinSockInit() {
        WSADATA wsaData;
        WORD wVers = MAKEWORD(2, 0);
        int rc = WSAStartup(wVers, &wsaData);
        if(rc != 0) {
	    fprintf(stderr, "Failed to initialize winsock: %d\n", rc);
	    exit(1);
        }
    }

    ~WinSockInit() {
	WSACleanup();
    }
};
WinSockInit _WinSockInit;
#endif

SocketBase::~SocketBase()
{

}

SocketBase::SocketBase(const Address &a, bool isTcp)
    throw(NetworkException)
    : addr(a)
{
    create(isTcp);
    SocketManager::addSocket(this);
    setNonBlocking();
    _isConnecting=false;
}

SocketBase::SocketBase(SOCKET fd, const Address &a)
    throw(NetworkException)
    : sockfd(fd), addr(a)
{
    SocketManager::addSocket(this);
    setNonBlocking();
    _isConnecting=false;
}

void
SocketBase::create (bool tcp) throw(NetworkException)
{
    if(tcp) {
        sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    } else {
        sockfd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    }

    LOGGER.debug("SocketBase:: Create [%s:%d] socket", (tcp)?"tcp":"udp",sockfd);
    
    if(sockfd == INVALID_SOCKET) {
        lastError = GET_NET_ERROR();
        std::stringstream msg;
        msg << "Couldn't create socket: " << NETSTRERROR(lastError);
        throw NetworkException(msg.str());
    }
}

void
SocketBase::setNonBlocking() throw(NetworkException)
{
    int res;
#ifdef _WIN32
    unsigned long mode = 1;
    res = ioctlsocket(sockfd, FIONBIO, &mode);
#else
    res = fcntl(sockfd, F_SETFL, O_NONBLOCK);
#endif
    if ( res == SOCKET_ERROR ) {
        lastError = GET_NET_ERROR();
//        doClose();
        std::stringstream msg;
        msg << "Couldn't set socket to nonblocking mode: " << NETSTRERROR(lastError);
        LOGGER.warning("%s", msg.str().c_str());
    }
}

void
SocketBase::bindSocketTo(const Address& toaddr) throw(NetworkException)
{
    int res = bind(sockfd, toaddr.getSockaddr(), toaddr.getSockaddrLen());
    if(res == SOCKET_ERROR) {
        lastError = GET_NET_ERROR();
        doClose();
        std::stringstream msg;
        msg << "Couldn't bind socket to address '"
            << toaddr.getIP() << "' port " << toaddr.getPort()
            << ": " << NETSTRERROR(lastError);
        throw NetworkException(msg.str());           
    }
}

void
SocketBase::setReuseAddr() throw(NetworkException)
{
    SETSOCKOPT_PARAMTYPE val = 1;
    int res = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    if(res == SOCKET_ERROR) {
        lastError = GET_NET_ERROR();
//        doClose();
        std::stringstream msg;
        msg << "Couldn't set SO_REUSEADDR: " << NETSTRERROR(lastError);
        LOGGER.warning("%s", msg.str().c_str());
    }
}

void
SocketBase::setNoDelay() throw(NetworkException)
{
    SETSOCKOPT_PARAMTYPE val = 1;
    int res = setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &val, sizeof(val));
    if(res == SOCKET_ERROR) {
        lastError = GET_NET_ERROR();
//        doClose();
        std::stringstream msg;
        msg << "Couldn't set TCP_NODELAY: " << NETSTRERROR(lastError);
        LOGGER.warning("%s", msg.str().c_str());
    }
}


void
SocketBase::doListen() throw(NetworkException)
{
    int res = listen(sockfd, 20);
    if(res == SOCKET_ERROR) {
        lastError = GET_NET_ERROR();
        doClose();
        std::stringstream msg;
        msg << "Couldn't listen on socket: " << NETSTRERROR(lastError);
        throw NetworkException(msg.str());
    }
}

void
SocketBase::doConnect() throw(NetworkException)
{
    int res = connect(sockfd, addr.getSockaddr(), addr.getSockaddrLen());
    if(res == SOCKET_ERROR) {
        lastError = GET_NET_ERROR();
        if (IS_CONNECT_INPROGRESS(lastError)) {
            _isConnecting = true;
            return;
        }
        doClose();
        std::stringstream msg;
        msg << "Couldn't connect to '" << addr.getIP() << "' port "
            << addr.getPort() << ": " << NETSTRERROR(lastError);
        throw NetworkException(msg.str());
    }
}

int
SocketBase::doSend(const void* data, size_t len) throw(NetworkException)
{
    int res = send(sockfd, (const char*) data, len, SEND_FLAGS);
    if(res == SOCKET_ERROR) {
        lastError = GET_NET_ERROR();
        if ( IS_IGNORABLE_ERROR(lastError) )
            return 0;

        if ( ! IS_DISCONECTED(lastError) ) {
            std::stringstream msg;
            msg << "Send error: " << NETSTRERROR(lastError);
            LOGGER.warning("%s", msg.str().c_str());
        }
        
        onDisconected();
        return 0;
    }
    return res;
}

int
SocketBase::doReceive(void* buffer, size_t len) throw(NetworkException)
{
    int res = recv(sockfd, (char*) buffer, len, RECV_FLAGS);
    if(res == SOCKET_ERROR) {
        lastError = GET_NET_ERROR();
        if ( IS_IGNORABLE_ERROR(lastError) )
            return 0;

        if ( ! IS_DISCONECTED(lastError) ) {
            std::stringstream msg;
            msg << "Read error: " << NETSTRERROR(lastError);
            LOGGER.warning("%s", msg.str().c_str());
        }

        onDisconected();
        return 0;
    }
    
    if (!res) {
        LOGGER.debug("SocketBase::doReceive Disconected from server");
        onDisconected();
    }
    
    return res;
}

int
SocketBase::doSendTo(const Address& toaddr, const void* data, size_t len) throw(NetworkException)
{
    int res = sendto(sockfd, (const char*) data, len, SEND_FLAGS,
                toaddr.getSockaddr(), toaddr.getSockaddrLen());
    if(res == SOCKET_ERROR) {
        lastError = GET_NET_ERROR();
        if ( ! IS_SENDTO_IGNORABLE(lastError) )
        {
            std::stringstream msg;
            msg << "Send error: " << NETSTRERROR(lastError);
            LOGGER.warning("%s", msg.str().c_str());
        }
        return 0;
    }
    return res;
}

size_t
SocketBase::doReceiveFrom(Address& fromaddr, void* buffer, size_t len) throw(NetworkException)
{
    int res = recvfrom(sockfd, (char*) buffer, len, RECV_FLAGS,
            fromaddr.getSockaddr(), fromaddr.getSockaddrLenPointer());
    if ( res == SOCKET_ERROR )
    {
        lastError = GET_NET_ERROR();
        if ( ! IS_RECVFROM_IGNORABLE(lastError) )
        {
            std::stringstream msg;
            msg << "Receive error: " << NETSTRERROR(lastError);
            LOGGER.warning("%s", msg.str().c_str());
        }

        return 0;
    }
    return res;
}

SOCKET
SocketBase::doAccept(Address& fromaddr) throw(NetworkException)
{
    SOCKET newsock;
    newsock= accept(sockfd, fromaddr.getSockaddr(), fromaddr.getSockaddrLenPointer());
    if ( newsock == INVALID_SOCKET )
    {
        lastError = GET_NET_ERROR();
        if ( ! IS_ACCEPT_IGNORABLE(lastError) )
        {
            std::stringstream msg;
            msg << "Accept error: " << NETSTRERROR(lastError);
            LOGGER.warning("%s", msg.str().c_str());
        }
    }
    return newsock;
}

void
SocketBase::doClose()
{
    LOGGER.debug("SocketBase:: Closing [%d] socket", sockfd);
    SocketManager::removeSocket(this);
    closesocket(sockfd);
}

}

