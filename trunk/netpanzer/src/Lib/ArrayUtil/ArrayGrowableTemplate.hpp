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
#ifndef _ARRAYGROWABLETEMPLATE_HPP
#define _ARRAYGROWABLETEMPLATE_HPP

#include <assert.h>

#include "Util/NoCopy.hpp"

template< class TYPE >
class ArrayGrowableTemplate : public NoCopy
{
protected:
    std::vector<TYPE> elems;

public:
    ArrayGrowableTemplate();

    ArrayGrowableTemplate( unsigned long size );

    ~ArrayGrowableTemplate( void );

    void initialize( unsigned long size );

    TYPE & operator[]( unsigned long index);

    void add( TYPE object, unsigned long index );

    void resize( unsigned long size );

    unsigned long getSize( void ) const
    {
        return( size );
    }

    void deallocate( void )
    {
        if ( array != 0 ) {
            free( array );
            array = 0;
        }

        size = 0;
    }

};

template< class TYPE >
ArrayGrowableTemplate< TYPE >::ArrayGrowableTemplate()
{
}

template< class TYPE >
ArrayGrowableTemplate< TYPE >::
ArrayGrowableTemplate(unsigned long size)
{
    elems.resize(size);
}

template< class TYPE >
void ArrayGrowableTemplate< TYPE >::
initialize(unsigned long size)
{
    elems.resize(size);
}

template< class TYPE >
ArrayGrowableTemplate< TYPE >::~ArrayGrowableTemplate()
{
}

template< class TYPE >
TYPE & ArrayGrowableTemplate< TYPE >::operator[]( unsigned long index)
{
    if(index >= elems.size()) {
        elems.resize(index);
    }
    
    return elems.at(index);
}

template< class TYPE >
void ArrayGrowableTemplate< TYPE >::add( TYPE object, unsigned long index )
{
    if(index >= elems.size()) {
        elems.resize(index);
    }

    elems[index] = object;
}

template< class TYPE >
void ArrayGrowableTemplate< TYPE >::resize(unsigned long size)
{
    elems.resize(size);
}

#endif // ** _ARRAYGROWABLETEMPLATE_HPP
