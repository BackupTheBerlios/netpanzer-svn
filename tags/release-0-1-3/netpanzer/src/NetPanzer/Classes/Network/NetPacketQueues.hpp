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
#ifndef _NETPACKETQUEUES_HPP
#define _NETPACKETQUEUES_HPP

#include <string.h>
#include "NetPacket.hpp"
#include "ArrayUtil/QueueTemplate.hpp"
#include "ArrayUtil/ArrayTemplate.hpp"
#include "ArrayUtil/Timer.hpp"

class NetPacketQueue : public QueueTemplate< NetPacket >
{
public:

    void add( NetPacket *object, unsigned long index )
    {
        unsigned long net_packet_size;

        assert( index < size );
        net_packet_size = (sizeof( NetPacket ) - _MAX_NET_PACKET_SIZE) + object->packet_size;
        memcpy(&array[index], object, net_packet_size);
    }

    void dequeue( NetPacket *object )
    {
        unsigned long net_packet_size;

        assert( front != rear );
        front = ( front + 1 ) % size;
        net_packet_size = (sizeof( NetPacket ) - _MAX_NET_PACKET_SIZE) + array[ front ].packet_size;
        memcpy(object, &array[ front ], net_packet_size);
    }

    void enqueue( NetPacket &object )
    {
        add(&object, (rear + 1) % size);
        rear = (rear + 1) % size;
    }
};

class NetPacketBlock
{
public:

    NetPacket     packet;
    bool       in_use;
    bool       out_of_order;
    unsigned char sequence;

    NetPacketBlock( )
    {
        in_use = false;
        out_of_order = false;
    }

    unsigned short getPacketSize()
    {
        return( packet.packet_size);
    }
};

typedef ArrayTemplate< NetPacketBlock > NetPacketBlockArray;

#endif // ** _NETPACKETQUEUES_HPP
