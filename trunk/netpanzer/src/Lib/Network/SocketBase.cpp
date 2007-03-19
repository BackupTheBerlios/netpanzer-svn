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

#include <sstream>
#include <stdexcept>

#include "SocketHeaders.hpp"
#include "SocketBase.hpp"

namespace network
{

#ifdef USE_WINSOCK
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

SocketBase::SocketBase()
{
}

SocketBase::~SocketBase()
{
    close();
}

void
SocketBase::printError(std::ostream& out, int e)
{
#ifdef USE_WINSOCK
    out << "Winsock error " << e;
#else
    out << strerror(e);
#endif
}

void
SocketBase::create(bool tcp)
{
    if(tcp)
        sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    else
        sockfd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if(sockfd == INVALID_SOCKET) {
        int error = GET_NET_ERROR();
	std::stringstream msg;
	msg << "Couldn't create socket: ";
        printError(msg,error);
	throw std::runtime_error(msg.str());
    }
}

void
SocketBase::bindSocketTo(const Address& toaddr)
{
    int res = bind(sockfd, toaddr.getSockaddr(), toaddr.getSockaddrLen());
    if(res == SOCKET_ERROR) {
        int error = GET_NET_ERROR();
        std::stringstream msg;
        msg << "Couldn't bind socket to address '"
            << toaddr.getIP() << "' port " << toaddr.getPort()
            << ": ";
        printError(msg,error);
        throw std::runtime_error(msg.str());           
    }
}

void
SocketBase::setReuseAddr()
{
    int val = 1;
    int res = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    if(res == SOCKET_ERROR) {
        int error = GET_NET_ERROR();
        std::stringstream msg;
        msg << "Couldn't set SO_REUSEADDR: ";
        printError(msg,error);
        throw std::runtime_error(msg.str());
    }
}

void
SocketBase::setNonBlocking()
{
    int res;
#ifdef USE_WINSOCK
    unsigned long mode = 1;
    res = ioctlsocket(sockfd, FIONBIO, &mode);
#else
    res = fcntl(sockfd, F_SETFL, O_NONBLOCK);
#endif
    if ( res == SOCKET_ERROR ) {
        int error = GET_NET_ERROR();
	std::stringstream msg;
	msg << "Couldn't set socket to nonblocking mode: ";
        printError(msg,error);
	throw std::runtime_error(msg.str());
    }
}

void
SocketBase::doListen()
{
    int res = listen(sockfd, 20);
    if(res == SOCKET_ERROR) {
        int error = GET_NET_ERROR();
        std::stringstream msg;
        msg << "Couldn't listen on socket: ";
        printError(msg,error);
        throw std::runtime_error(msg.str());
    }
}

void
SocketBase::doConnect()
{
    int res = connect(sockfd, addr.getSockaddr(), addr.getSockaddrLen());
    if(res == SOCKET_ERROR) {
        int error = GET_NET_ERROR();
        std::stringstream msg;
        msg << "Couldn't connect to '" << addr.getIP() << "' port "
            << addr.getPort() << ": ";
        printError(msg,error);
        throw std::runtime_error(msg.str());
    }
}

int
SocketBase::doSend(const void* data, size_t len)
{
    int res = send(sockfd, (const char*) data, len, 0);
    if(res == SOCKET_ERROR) {
        int error = GET_NET_ERROR();
        std::stringstream msg;
        msg << "Send error: ";
        printError(msg,error);
        throw std::runtime_error(msg.str());
    }
    return res;
}

int
SocketBase::doReceive(void* buffer, size_t len)
{
    int res = recv(sockfd, (char*) buffer, len, 0);
    if(res == SOCKET_ERROR) {
        int error = GET_NET_ERROR();
        if ( error == NETERROR_WOULDBLOCK)
            return 0;
        std::stringstream msg;
        msg << "Read error: ";
        printError(msg,error);
        throw std::runtime_error(msg.str());
    }
    return res;
}

int
SocketBase::doSendTo(const Address& toaddr, const void* data, size_t len)
{
    int res = sendto(sockfd, (const char*) data, len, 0,
                toaddr.getSockaddr(), toaddr.getSockaddrLen());
    if(res == SOCKET_ERROR) {
        int error = GET_NET_ERROR();
        std::stringstream msg;
        msg << "Send error: ";
        printError(msg,error);
        throw std::runtime_error(msg.str());
    }
    return res;
}

size_t
SocketBase::doReceiveFrom(Address& fromaddr, void* buffer, size_t len)
{
    int res = recvfrom(sockfd, (char*) buffer, len, 0,
            fromaddr.getSockaddr(), fromaddr.getSockaddrLenPointer());
    if(res == SOCKET_ERROR) {
        int error = GET_NET_ERROR();
        if ( error == NETERROR_WOULDBLOCK)
            return 0;
        std::stringstream msg;
        msg << "Receive error: " << strerror(errno);
        printError(msg,error);
        throw std::runtime_error(msg.str());
    }
    return res;
}

SOCKET
SocketBase::doAccept(Address& fromaddr)
{
    SOCKET newsock;
    newsock= accept(sockfd, fromaddr.getSockaddr(), fromaddr.getSockaddrLenPointer());
    if (newsock == INVALID_SOCKET) {
        int error = GET_NET_ERROR();
        if ( error == NETERROR_WOULDBLOCK )
            return INVALID_SOCKET; // XXX this could be better
        std::stringstream msg;
        msg << "Accept error: ";
        printError(msg,error);
        throw std::runtime_error(msg.str());
    }
    return newsock;
}

void
SocketBase::close()
{
    closesocket(sockfd);
}

}

