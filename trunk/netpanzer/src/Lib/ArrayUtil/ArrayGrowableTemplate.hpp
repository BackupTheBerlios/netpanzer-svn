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
    unsigned long size;
    unsigned long grow_increment;
    unsigned long grow_limit;
    TYPE *array;

public:

    ArrayGrowableTemplate();

    ArrayGrowableTemplate( unsigned long size, unsigned long growIncrement,
                           unsigned long growLimit );

    ArrayGrowableTemplate( unsigned long size, unsigned long growIncrement );

    ArrayGrowableTemplate( unsigned long size );

    ~ArrayGrowableTemplate( void );

    void initialize( unsigned long size, unsigned long growIncrement,
                     unsigned long growLimit );

    TYPE & operator[]( unsigned long index);

    void add( TYPE object, unsigned long index );

    void resize( unsigned long size );

    void setGrowLimit( unsigned long limit );

    void setGrowIncrement( unsigned long growIncrement );

    inline unsigned long getSize( void ) const
    {
        return( size );
    }

    inline void deallocate( void )
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
    array = 0;
    size = 0;
    grow_increment = 1;
    grow_limit = 0xFFFFFFFF;
}

template< class TYPE >
ArrayGrowableTemplate< TYPE >::
ArrayGrowableTemplate( unsigned long size, unsigned long growIncrement,
                       unsigned long growLimit )
        : array(0)
{
    initialize( size , growIncrement, growLimit );
}


template< class TYPE >
ArrayGrowableTemplate< TYPE >::
ArrayGrowableTemplate( unsigned long size, unsigned long growIncrement )
        : array(0)
{
    initialize( size , growIncrement, 0xFFFFFFFF );
}


template< class TYPE >
ArrayGrowableTemplate< TYPE >::
ArrayGrowableTemplate( unsigned long size )
        : array(0)
{
    initialize( size , 1, 0xFFFFFFFF );
}

template< class TYPE >
void ArrayGrowableTemplate< TYPE >::
initialize( unsigned long size, unsigned long growIncrement,
            unsigned long growLimit )
{
    ArrayGrowableTemplate< TYPE >::size = size;

    grow_increment = growIncrement;

    grow_limit = growLimit;

    assert( (size > 0) && (growIncrement > 0) );

    if ( array != 0 ) {
        free( array );
        array = 0;
    }

    array = (TYPE *) malloc( sizeof(TYPE) * size );

    assert( array != 0 );
}

template< class TYPE >
ArrayGrowableTemplate< TYPE >::~ArrayGrowableTemplate( void )
{
    free( array );
}

template< class TYPE >
TYPE & ArrayGrowableTemplate< TYPE >::operator[]( unsigned long index)
{
    if ( index >= size ) {
        unsigned long new_size;
        new_size = (index + 1 + grow_increment);
        if ( new_size <= grow_limit ) {
            array = (TYPE *) realloc( array, sizeof(TYPE) * new_size );
            memset( &array[ size ], 0, sizeof(TYPE) * (new_size - size) );
            size = new_size;
            assert( array != 0 );
        } else
            assert( 0 );
    }

    return( array[index] );
}

template< class TYPE >
void ArrayGrowableTemplate< TYPE >::add( TYPE object, unsigned long index )
{
    if ( index >= size ) {
        unsigned long new_size;
        new_size = (index + 1 + grow_increment);
        if ( new_size <= grow_limit ) {
            array = (TYPE *) realloc( array, sizeof(TYPE) * new_size );
            memset( &array[ size ], 0, sizeof(TYPE) * (new_size - size) );
            size = new_size;
            assert( array != 0 );
        } else
            return;
    }

    memmove( (void *) &array[index], (void *) &object , sizeof( TYPE ) );
}

template< class TYPE >
void ArrayGrowableTemplate< TYPE >::resize( unsigned long size )
{
    array = (TYPE *) realloc( array, sizeof(TYPE) * size );
    assert( array != 0 );
}

template< class TYPE >
void ArrayGrowableTemplate< TYPE >::setGrowLimit( unsigned long limit )
{
    grow_limit = limit;
}

template< class TYPE >
void ArrayGrowableTemplate< TYPE >::setGrowIncrement( unsigned long growIncrement )
{
    grow_increment = growIncrement;
}


#endif // ** _ARRAYGROWABLETEMPLATE_HPP
