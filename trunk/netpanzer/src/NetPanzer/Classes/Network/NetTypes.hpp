/*
Copyright (C) 2004 by Matthias Braun

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
#ifndef __NETTYPES_HPP__
#define __NETTYPES_HPP__

#include <stdint.h>
#include "Util/Endian.hpp"

namespace net
{

typedef uint8_t UInt8;
typedef int8_t Int8;

class UInt32
{
private:
    uint32_t val;
public:
    UInt32(uint32_t val)
    {
        *this = val;
    }

    operator uint32_t() const
    {
        return ltoh32(val);
    }
    void operator=(uint32_t val)
    {
        this->val = htol32(val);
        return val;
    }
} __attribute__((packed));

class UInt16
{
private:
    uint16_t val;
public:
    UInt32(uint16_t val)
    {
        *this = val;
    }

    operator uint16_t() const
    {
        return ltoh16(val);
    }
    void operator=(uint16_t val)
    {
        this->val = htol16(val);
        return val;
    }
} __attribute__((packed));

} // end of namespace net

#endif

