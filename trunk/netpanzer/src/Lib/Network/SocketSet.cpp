#include <config.h>

#include <sstream>
#include <stdexcept>

#include "SocketHeaders.hpp"
#include "SocketSet.hpp"

namespace network
{

SocketSet::SocketSet()
{
    FD_ZERO(&set);
}

SocketSet::~SocketSet()
{
}

void
SocketSet::add(const SocketBase& socket)
{
    FD_SET(socket.sockfd, &set);
}

void
SocketSet::remove(const SocketBase& socket)
{
    FD_CLR(socket.sockfd, &set);
}

bool
SocketSet::select(unsigned int usec)
{
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = usec;

    testset = set;
    int res = ::select(FD_SETSIZE, &testset, 0, 0, &timeout);
    if(res < 0) {
        std::stringstream msg;
        msg << "Select failed: " << strerror(errno);
        throw std::runtime_error(msg.str());
    }

    return res != 0;
}

}
