#include <config.h>

#include <sstream>
#include <stdexcept>

#include "SocketHeaders.hpp"
#include "SocketBase.hpp"

namespace network
{

#ifdef WINSOCK
int SocketBase::netrefcount = 0;
#endif

SocketBase::SocketBase()
    : sockfd(-1)
{
#ifdef WINSOCK
    if(netrefcount == 0) {
        WSADATA wsaData;
        WORD wVers = MAKEWORD(1, 1);
        int rc = WSAStartup(wVers, &wsaData);
        if(rc != 0) {
            std::stringstream msg;
            msg << "Failed to initialize winsock (Error code " << rc << ")";
            throw std::runtime_error(msg.str());
        }
    }
    netrefcount++;
#endif
}

SocketBase::~SocketBase()
{
    close();
    
#ifdef WINSOCK
    netrefcount--;
    if(netrefcount == 0) {
        WSACleanup();
    }
#endif
}

void
SocketBase::create(bool tcp)
{
    if(tcp)
        sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    else
        sockfd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if(sockfd < 0) {
        std::stringstream msg;
        msg << "Couldn't create socket: " << strerror(errno);
        throw std::runtime_error(msg.str());
    }
}

void
SocketBase::setNonBlocking()
{
#ifdef WIN32
    unsigned long mode = 1;
    ioctlsocket(sockfd, FIONBIO, &mode);
    // TODO error checking
#else
    if(fcntl(sockfd, F_SETFL, O_NONBLOCK) < 0) {
        std::stringstream msg;
        msg << "Couldn't set socket to nonblocking mode: " << strerror(errno);
        throw std::runtime_error(msg.str());
    }
#endif
}

void
SocketBase::close()
{
    if(sockfd >= 0) {
#ifdef WINSOCK
        closesocket(sockfd);
#else
        ::close(sockfd);
#endif
    }
}

}

