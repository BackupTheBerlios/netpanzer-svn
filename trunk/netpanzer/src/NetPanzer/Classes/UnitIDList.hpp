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
#ifndef _UNITIDLIST_HPP
#define _UNITIDLIST_HPP

#include "UnitID.hpp"
#include "ArrayUtil/ArrayGrowableTemplate.hpp"

typedef ArrayGrowableTemplate< UnitID > UnitIDListTemplate;

class UnitIDList : public UnitIDListTemplate
{
public:
    unsigned long contains;

    UnitIDList() : UnitIDListTemplate()
    {
        contains = 0;
    }

    void initialize( unsigned long size, unsigned long growIncrement, unsigned long growLimit )
    {
        UnitIDListTemplate::initialize( size, growIncrement, growLimit );
    }

    inline void add( UnitID object, unsigned long index )
    {
        UnitIDListTemplate::add( object, index );
        if ( contains < size )
            contains++;
    }

    inline void removeAll( void )
    {
        contains = 0;
    }

    inline unsigned long containsItems( void )
    {
        return( contains );
    }
};

#endif // ** _UNITIDLIST_HPP
