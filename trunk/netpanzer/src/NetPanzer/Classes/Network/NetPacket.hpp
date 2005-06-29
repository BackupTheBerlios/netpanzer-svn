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
#include <string.h>

#include "Util/Endian.hpp"
#include "NetMessage.hpp"

static const uint16_t _MAX_NET_PACKET_SIZE=512;
typedef uint32_t NetClientID;

#ifdef MSVC
#pragma pack(1)
#endif
class NetPacket
{
public:
    NetClientID toID;
    NetClientID fromID;

    uint8_t  data[ _MAX_NET_PACKET_SIZE ];

    const NetMessage* getNetMessage() const
    {
        return (const NetMessage*) data;
    }

    size_t getSize() const
    {
        return sizeof(NetClientID) * 2 + getNetMessage()->getSize();
    }
}
__attribute__((packed));

struct NetMessageStruct
{
    uint16_t size;
    uint8_t  message_class;
    uint8_t  message_id;

    uint8_t  data[ _MAX_NET_PACKET_SIZE - 4 ];
}
__attribute__((packed));

#define _MULTI_PACKET_LIMIT (_MAX_NET_PACKET_SIZE - 9)

class MultiMessage : public NetMessage
{
public:
    /* header part */
    uint8_t  message_count;
    /* data */
    uint8_t  data[ _MULTI_PACKET_LIMIT ];

    static size_t getHeaderSize()
    {
        return sizeof(NetMessage) + sizeof(uint8_t);
    }
} __attribute__((packed));

#ifdef MSVC
#pragma pack()
#endif

#endif

