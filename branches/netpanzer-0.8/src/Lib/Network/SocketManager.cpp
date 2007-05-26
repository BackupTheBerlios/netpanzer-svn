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

#include <list>
#include "SocketManager.hpp"
#include "Util/Log.hpp"

namespace network {

list<SocketBase *> SocketManager::socketList;
list<SocketBase *> SocketManager::newSockets;
list<SocketBase *> SocketManager::deletedSockets;
SocketSet SocketManager::sset;

void
SocketManager::handleEvents()
{
    list<SocketBase *>::iterator i;

    if (!newSockets.empty() || !deletedSockets.empty()) {
        if (!deletedSockets.empty()) {
            for (i = deletedSockets.begin(); i!=deletedSockets.end(); i++) {
                LOGGER.debug("SocketManager:: Removing socket [%d,%08lx]",(*i)->sockfd, (unsigned long)(*i));
                socketList.remove(*i);
                delete *i;
            }
            deletedSockets.clear();
        }

        if (!newSockets.empty()) {
            for (i = newSockets.begin(); i!=newSockets.end(); i++) {
                LOGGER.debug("SocketManager:: Adding socket [%d,%08lx]",(*i)->sockfd, (unsigned long)(*i));
                socketList.push_front(*i);
            }
            newSockets.clear();
        }

        sset.clear();
        for (i = socketList.begin(); i!=socketList.end(); i++) {
            if ((*i)->isConnecting()) {
                LOGGER.debug("SocketManager:: Setting socket [%d,%08lx] to write",(*i)->sockfd, (unsigned long)(*i));
                sset.addWrite(*i);
            } else {
                LOGGER.debug("SocketManager:: Setting socket [%d,%08lx] to read",(*i)->sockfd, (unsigned long)(*i));
                sset.add(*i);
            }
        }
    }
    
    if (!sset.select(0))
        return;

    for (i = socketList.begin(); i!=socketList.end(); i++) {
        if ((*i)->isConnecting()) {
            if (sset.isWriteable(*i)) {
                sset.removeWrite(*i);
                sset.add(*i);
                (*i)->connectionFinished();
            }
        } else {
            if (sset.dataAvailable(*i)) {
                (*i)->onDataReady();
            }
        }
    }




}




}
