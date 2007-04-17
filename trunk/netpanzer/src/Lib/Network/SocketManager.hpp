/*
Copyright (C) 2007 by Aaron Perez <aaronps@gmail.com>

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

#ifndef _SOCKETMANAGER_HPP
#define _SOCKETMANAGER_HPP

#include <list>
#include "SocketBase.hpp"
#include "TCPSocket.hpp"
#include "SocketSet.hpp"

using namespace std;

namespace network {
    
class SocketManager
{
public:
    SocketManager() {};
    ~SocketManager() {};
    
    static void handleEvents();
    
protected:
    friend class SocketBase; // the only allowed to add/remove
    static void addSocket(SocketBase *s)
    {
        newSockets.push_front(s);
    }
    
    static void removeSocket(SocketBase *s)
    {
        if ( !newSockets.empty() ) {
            list<SocketBase *>::iterator i = newSockets.begin();
            while (i != newSockets.end()) {
                if ( *i == s ) {
                    newSockets.erase(i);
                    return;
                }
                i++;
            }
        }
        deletedSockets.push_front(s);
    }

private:
    
    static SocketSet sset;
    static list<SocketBase *> socketList;
    static list<SocketBase *> newSockets;
    static list<SocketBase *> deletedSockets;
    
};

} // namespace

#endif
