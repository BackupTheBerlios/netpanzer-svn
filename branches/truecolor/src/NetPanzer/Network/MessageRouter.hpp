/*
Copyright (C) 2008 by Aaron Perez <aaronps@gmail.com>

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
 *
 * Created on December 13, 2008, 6:58 PM
 */

#ifndef _MESSAGEROUTER_HPP
#define	_MESSAGEROUTER_HPP

#include "Core/CoreTypes.hpp"

class MessageClassHandler;
class ClientSocket;
class NetPacket;

class MessageRouter
{
public:
    static void initialize(bool isServer);
    static void routePackets();
    static void routePacket(const NetPacket& np);
    static bool getNextPacket(NetPacket& np);
    static void enqueueIncomingPacket( const void *data, Uint16 size,
                                   Uint16 fromPlayer, ClientSocket *fromClient);
    static void setMessageClassHandler(MsgClassID c, MessageClassHandler * h);
    static void clearMessageClassHandler(MsgClassID c);
};

#endif	/* _MESSAGEROUTER_HPP */

