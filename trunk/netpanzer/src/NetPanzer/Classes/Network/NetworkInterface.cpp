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
#include <config.h>

#include <assert.h>
#include "Util/Log.hpp"
#include "NetworkInterface.hpp"

void EnqueueIncomingPacket(const void *message, Uint16 message_size,
                           NetClientID toID, NetClientID fromID,
                           ClientSocket *fromClient)
{
    static NetPacket TEMP_PACKET;

    TEMP_PACKET.toID = toID;
    TEMP_PACKET.fromID = fromID;
    TEMP_PACKET.fromClient = fromClient;
    assert(message_size <= _MAX_NET_PACKET_SIZE);

    memcpy(TEMP_PACKET.data, message, message_size);
    NetworkInterface::receive_queue.enqueue( TEMP_PACKET );
}

NetPacketQueue NetworkInterface::loop_back_recv_queue;
NetPacketQueue NetworkInterface::receive_queue;

NetworkInterface::NetworkInterface()
{
    loop_back_recv_queue.initialize( 100 );
    receive_queue.initialize( 200 );
}

NetworkInterface::~NetworkInterface()
{
    loop_back_recv_queue.deallocate();
    receive_queue.deallocate();
}

