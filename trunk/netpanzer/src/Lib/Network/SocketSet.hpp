#ifndef __SOCKETSET_HPP__
#define __SOCKETSET_HPP__

#include "SocketHeaders.hpp"
#include "SocketBase.hpp"

namespace network
{

class SocketSet
{
public:
    SocketSet();
    ~SocketSet();

    /** add a socket to the set that should be watched */
    void add(const SocketBase& socket);
    /** removes a socket from the set */
    void remove(const SocketBase& socket);
    /** Waits for input on the sockets in the set until a socket has input or a
     * timeout occurs. Returns false in case of timeout.
     */
    bool select(unsigned int usec);
    /** after a select call you can test with this function if a specific socket
     * has data pending
     */
    bool dataPending(const SocketBase& socketbase) const
    {
        return FD_ISSET(socketbase.sockfd, &testset);
    }
    
private:
    fd_set set;
    fd_set testset;
};

}

#endif

