#ifndef __SOCKETBASE_HPP__
#define __SOCKETBASE_HPP__

#include "Util/NoCopy.hpp"

namespace network
{

class SocketBase : public NoCopy
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

