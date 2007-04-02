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
#ifndef __SOCKETSET_HPP__
#define __SOCKETSET_HPP__

#include "SocketHeaders.hpp"
#include "SocketBase.hpp"
#include "Util/NoCopy.hpp"

namespace network
{

class SocketSet : public NoCopy
{
public:
    SocketSet();
    ~SocketSet();

    void clear();
    
    /** add a socket to the set that should be watched */
    void add(const SocketBase& socket);
    void add(const SocketBase* socket);
    void addWrite(const SocketBase* socket);
    
    /** removes a socket from the set */
    void remove(const SocketBase& socket);
    void removeWrite(const SocketBase* socket);
    /** Waits for input on the sockets in the set until a socket has input or a
     * timeout occurs. Returns false in case of timeout.
     */
    bool select(unsigned int timeout_usec = 0);
    /** after a select call you can test with this function if a specific socket
     * has data pending
     */
    bool dataPending(const SocketBase& socketbase) const
    {
        return FD_ISSET(socketbase.sockfd, &testreadset);
    }
    
    bool dataAvailable(const SocketBase* socketbase) const
    {
        return FD_ISSET(socketbase->sockfd, &testreadset);
    }
    
    bool isWriteable(const SocketBase* socketbase) const
    {
        return FD_ISSET(socketbase->sockfd,&testwriteset);
    }
    
    
private:
    SOCKET maxfd;
    fd_set readset;
    fd_set writeset;
    fd_set testreadset;
    fd_set testwriteset;
};

}

#endif

