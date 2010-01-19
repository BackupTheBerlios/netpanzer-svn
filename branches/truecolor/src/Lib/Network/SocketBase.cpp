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
    if(tcp)
    {
        sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    }
    else
    {
        sockfd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    }

    //LOGGER.debug("SocketBase:: Create [%s:%d] socket", (tcp)?"tcp":"udp",sockfd);
    
    if(sockfd == INVALID_SOCKET)
    {
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
    if ( res == SOCKET_ERROR )
    {
        lastError = GET_NET_ERROR();
        doClose();
        std::stringstream msg;
        msg << "Couldn't set socket to nonblocking mode: " << NETSTRERROR(lastError);
        throw NetworkException(msg.str());
    }
}

void
SocketBase::bindSocketTo(const Address& toaddr) throw(NetworkException)
{
    int res = bind(sockfd, toaddr.getSockaddr(), toaddr.getSockaddrLen());
    if(res == SOCKET_ERROR)
    {
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
    if(res == SOCKET_ERROR)
    {
        lastError = GET_NET_ERROR();
        doClose();
        std::stringstream msg;
        msg << "Couldn't set SO_REUSEADDR: " << NETSTRERROR(lastError);
        throw NetworkException(msg.str());
    }
}

void
SocketBase::doListen() throw(NetworkException)
{
    int res = listen(sockfd, 20);
    if(res == SOCKET_ERROR)
    {
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
    if(res == SOCKET_ERROR)
    {
        lastError = GET_NET_ERROR();
        if (IS_CONNECT_INPROGRESS(lastError))
        {
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
SocketBase::doSend(const void* data, size_t len)
{
    int res = send(sockfd, (const char*) data, len, SEND_FLAGS);
    if(res == SOCKET_ERROR)
    {
        lastError = GET_NET_ERROR();
        if ( IS_DISCONECTED(lastError) )
        {
            onDisconected();
            return 0;
        }
        
        if ( IS_IGNORABLE_ERROR(lastError) )
        {
            return 0;
        }

        notifyError("Send");
    }
    return res;
}

int
SocketBase::doReceive(void* buffer, size_t len)
{
    int res = recv(sockfd, (char*) buffer, len, RECV_FLAGS);
    if(res == SOCKET_ERROR)
    {
        lastError = GET_NET_ERROR();
        if ( IS_DISCONECTED(lastError) )
        {
            onDisconected();
            return 0;
        }
        
        if ( IS_IGNORABLE_ERROR(lastError) )
        {
            return 0;
        }

        notifyError("Receive");
    }
    
    if (!res) {
        //LOGGER.debug("SocketBase::doReceive Disconected from server");
        onDisconected();
    }
    
    return res;
}

int
SocketBase::doSendTo(const Address& toaddr, const void* data, size_t len)
{
    int res = sendto(sockfd, (const char*) data, len, SEND_FLAGS,
                toaddr.getSockaddr(), toaddr.getSockaddrLen());
    if(res == SOCKET_ERROR)
    {
        lastError = GET_NET_ERROR();
        if ( IS_SENDTO_IGNORABLE(lastError) )
        {
            return 0;
        }

        notifyError("SendTo");
    }
    return res;
}

size_t
SocketBase::doReceiveFrom(Address& fromaddr, void* buffer, size_t len)
{
    int res = recvfrom(sockfd, (char*) buffer, len, RECV_FLAGS,
            fromaddr.getSockaddr(), fromaddr.getSockaddrLenPointer());
    if(res == SOCKET_ERROR)
    {
        lastError = GET_NET_ERROR();
        if ( IS_RECVFROM_IGNORABLE(lastError) )
        {
            return 0;
        }

        notifyError("ReceiveFrom");
    }
    return res;
}

SOCKET
SocketBase::doAccept(Address& fromaddr)
{
    SOCKET newsock;
    newsock = accept(sockfd, fromaddr.getSockaddr(),
                             fromaddr.getSockaddrLenPointer());
    if (newsock == INVALID_SOCKET)
    {
        lastError = GET_NET_ERROR();
        if ( ! IS_ACCEPT_IGNORABLE(lastError) )
        {
            notifyError("Accept");
        }
    }
    return newsock;
}

void
SocketBase::doClose()
{
    //LOGGER.debug("SocketBase:: Closing [%d] socket", sockfd);
    SocketManager::removeSocket(this);
    closesocket(sockfd);
}

void
SocketBase::notifyError(const char * errorType)
{
    std::stringstream msg;
    msg << errorType << " error: " << NETSTRERROR(lastError);
    //LOGGER.debug("SocketBase::notifyError '%s'", msg.str().c_str());
    onSocketError(msg.str().c_str());
}

}

