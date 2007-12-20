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

#include <sstream>
#include "SocketManager.hpp"
#include "Util/Log.hpp"

namespace network {

SocketManager::Sockets SocketManager::socketList;
SocketManager::Sockets SocketManager::newSockets;
SocketManager::Sockets SocketManager::deletedSockets;
SocketSet SocketManager::sset;

void
SocketManager::handleEvents()
{
    SocketsIterator i;

    if (!deletedSockets.empty()) {
        for (i = deletedSockets.begin(); i!=deletedSockets.end(); i++) {
            LOGGER.debug("SocketManager:: Removing socket [%d,%08lx]",(*i)->sockfd, (unsigned long)(*i));
            socketList.erase(*i);
            delete *i;
        }
        deletedSockets.clear();
    }

    if (!newSockets.empty()) {
        for (i = newSockets.begin(); i!=newSockets.end(); i++) {
            LOGGER.debug("SocketManager:: Adding socket [%d,%08lx]",(*i)->sockfd, (unsigned long)(*i));
            socketList.insert(*i);
        }
        newSockets.clear();
    }

    // Make the socketset every time, not all OS works the same way
    sset.clear();
    for (i = socketList.begin(); i!=socketList.end(); i++) {
        if ((*i)->isConnecting()) {
            sset.addWrite(*i);
        } else {
            sset.add(*i);
        }
    }

    int r = sset.select(0);
    if ( !r ) // most common first
        return;
    
    if ( r > 0 ) {    
        for (i = socketList.begin(); i!=socketList.end(); i++) {
            if ((*i)->isConnecting()) {
                if (sset.isWriteable(*i)) {
                    (*i)->connectionFinished();
                }
            } else {
                if (sset.dataAvailable(*i)) {
                    (*i)->onDataReady();
                }
            }
        }
    } else { // some error happened
        int error = sset.getError();
        if ( IS_INVALID_SOCKET(error) ) {
            removeInvalidSockets();
        } else if ( !IS_INTERRUPTED(error) ) { // beware: is NOT interrupted
            std::stringstream msg;
            msg << "SocketManager: BAD BAD ERROR " << NETSTRERROR(error);
            LOGGER.debug(msg.str().c_str());
        }
    }
}

void
SocketManager::removeInvalidSockets()
{
    LOGGER.debug("Finding invalid sockets in the set...");
    SocketsIterator i;
    int error;
    for (i = socketList.begin(); i!=socketList.end(); i++) {
        sset.clear();
        if ((*i)->isConnecting()) {
            sset.addWrite(*i);
        } else {
            sset.add(*i);
        }

        if ( sset.select() < 0 ) {
            error = GET_NET_ERROR();
            if ( IS_INVALID_SOCKET(error) ) {
                LOGGER.warning("SocketManager: FOUND Invalid socket, removing...");
                (*i)->onSocketError();
                removeSocket(*i);
            } else {
                std::stringstream msg;
                msg << "SocketManager: Error while finding invalid sockets " << NETSTRERROR(error);
                LOGGER.debug(msg.str().c_str());
            }
        }
    }
}

}
