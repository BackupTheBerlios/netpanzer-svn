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
#ifndef _UNITID_HPP
#define _UNITID_HPP

#include <stdint.h>

#ifdef MSVC
#pragma pack(1)
#endif

class UnitID
{
public:
    uint8_t player;
private:
    uint16_t index;
    uint16_t unique;

public:
    UnitID();
    void set(uint8_t player, uint16_t index, uint16_t unique );
    void set( uint8_t player, uint16_t index );
    uint8_t getPlayer() const;
    uint16_t getIndex() const;
    uint16_t getUnique() const;
    bool operator==( const UnitID& Uid ) const;
    bool operator!=( const UnitID& Uid ) const;
    bool operator<( const UnitID& Uid ) const;
    bool playerEqual( const UnitID& Uid ) const;
    void operator=( const UnitID& Uid );
} __attribute__((packed));

#ifdef MSVC
#pragma pack()
#endif

#endif
