#ifndef __SOCKETBASE_HPP__
#define __SOCKETBASE_HPP__

namespace network
{

class SocketBase
{
protected:
    SocketBase();
    ~SocketBase();

protected:
    friend class SocketSet;
    void create(bool tcp);
    void setNonBlocking();
    void close();
    
    int sockfd;
#ifdef WINSOCK
    static int netrefcount;
#endif
};

}

#endif

