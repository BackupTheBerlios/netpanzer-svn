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

#include "Core/CoreTypes.hpp"

#ifdef MSVC
#pragma pack(1)
#endif

class UnitMessage
{
private:
    Uint16 unit_id;
public:
    Uint8 message_id;

public:
    UnitMessage()
    {
        this->unit_id = 0xBADB;
        message_id = 0;
    }

    UnitMessage( UnitID unit_Id )
    {
        setHeader(unit_Id);
    }

    UnitID getUnitID() const
    {
        return ltoh16(unit_id);
    }

    void setHeader( UnitID unit_id )
    {
        this->unit_id = htol16(unit_id);
    }

} __attribute__((packed));

#ifdef MSVC
#pragma pack()
#endif

#endif // ** _UNITMESSAGE_HPP
