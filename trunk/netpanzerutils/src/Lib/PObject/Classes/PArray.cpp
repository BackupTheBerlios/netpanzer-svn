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
#include "PArray.hpp"
#include <stdlib.h>

PArray::PArray( unsigned long size )
{
    PArray::size = size;
    array = new void* [ size ];
    assert( array != 0 );
}

PArray::~PArray( void )
{
    delete[] array;
}

void PArray::initialize( unsigned long size )
{
    PArray::size = size;

    if( array != 0 ) {
        delete[] array;
        array = 0;
    }
    array = new void* [ size ];
    assert( array != 0 );
}

void PArray::sort( unsigned long sort_size, int (* compare)(const void *elem1,const void *elem2 ) )
{
    if( sort_size > size ) {
        assert(0);
        return;
    }

    qsort( array, sort_size, sizeof(void*), compare );
}
