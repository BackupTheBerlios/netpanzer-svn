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
#ifndef _NETPACKET_HPP
#define _NETPACKET_HPP

#include <stdint.h>

#include "NetMessageEnums.hpp"
#include "Network/SocketClient.hpp"

#define _MAX_NET_PACKET_SIZE 512

#ifdef MSVC
#pragma pack(1)
#endif
class NetPacket
{
public:
    SocketClient::ID toID;
    SocketClient::ID fromID;
    uint16_t packet_size;

    uint8_t  data[ _MAX_NET_PACKET_SIZE ];
}
__attribute__((packed));

struct NetMessageStruct
{
    uint16_t size;
    uint8_t  sequence;
    uint8_t  message_class;
    uint8_t  message_id;

    uint8_t  data[ _MAX_NET_PACKET_SIZE - 5 ];
}
__attribute__((packed));

class NetMessage
{
public:
    uint16_t size;
    uint8_t  message_class;
    uint8_t  message_id;

    size_t realSize()
    {
        return( sizeof( NetMessage ) );
    }
}
__attribute__((packed));

#define _MULTI_PACKET_LIMIT (_MAX_NET_PACKET_SIZE - 9)

class MultiMessage : public NetMessage
{
public:
    uint8_t  message_count;
    uint16_t message_size;

    uint8_t  data[ _MULTI_PACKET_LIMIT ];

    size_t realSize()
    {
        return( sizeof( MultiMessage ) - _MULTI_PACKET_LIMIT + message_size );
    }
}
__attribute__((packed));

#ifdef MSVC
#pragma pack()
#endif

#endif

