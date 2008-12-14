/*
Copyright (C) 1998 Pyrosoft Inc. (www.pyrosoftgames.com), Matthew Bogue
 
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
#ifndef _NETWORKINTERFACE_HPP
#define _NETWORKINTERFACE_HPP

#include "NetPacketQueues.hpp"
#include "Network/MessageClassHandler.hpp"

void EnqueueIncomingPacket( const void *data, Uint16 size,
                            Uint16 fromPlayer, ClientSocket *fromClient);

class NetworkInterface : public MessageClassHandler
{
public:
    NetworkInterface();
    virtual ~NetworkInterface();

    static NetPacketQueue receive_queue;
    static bool getPacket( NetPacket * packet);
//protected:
//    void handlePacket(NetPacket* packet) = 0;

};

extern NetworkInterface * NETWORKINTERFACE;

#endif // ** _NETWORKINTERFACE_HPP
