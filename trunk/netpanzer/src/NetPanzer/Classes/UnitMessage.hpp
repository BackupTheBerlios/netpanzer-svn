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
#ifndef _UNITMESSAGE_HPP
#define _UNITMESSAGE_HPP

#include <stdint.h>
#include "UnitBase.hpp"

enum { _umesg_flag_unique          = 0x01,
       _umesg_flag_broadcast       = 0x02,
       _umesg_flag_player          = 0x04,
       _umesg_flag_notplayer       = 0x08,
       _umesg_flag_send            = 0x0F,
       _umesg_flag_queue           = 0x20,
       _umesg_flag_manager_request = 0x40
     };

#ifdef MSVC
#pragma pack(1)
#endif

class UnitMessage
{
public:
    UnitID unit_id;
    uint8_t message_id;
    uint8_t message_flags;

public:

    UnitMessage()
    {}
    UnitMessage( UnitID unit_id, unsigned char flags );
    UnitMessage( unsigned char flags );

    inline void setHeader( UnitID unit_id, unsigned char flags )
    {
        UnitMessage::unit_id = unit_id;
        message_flags = flags;
    }

    inline void setHeader( unsigned char flags )
    {
        message_flags = flags;
    }

    inline bool isFlagged(unsigned char flags) const
    {
        if ( (flags & message_flags) == flags )
            return true;

        return false;
    }
} __attribute__((packed));

#ifdef MSVC
#pragma pack()
#endif

#endif // ** _UNITMESSAGE_HPP
