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
#ifndef _QUEUETEMPLATE_HPP
#define _QUEUETEMPLATE_HPP

#include "ArrayTemplate.hpp"

template< class TYPE >
class QueueTemplate : public ArrayTemplate< TYPE >
{
public:
    unsigned long front;
    unsigned long rear;

    QueueTemplate( )
    { }

    QueueTemplate( unsigned long size );

    void initialize( unsigned long size );

    inline void reset( void )
    {
        front = 0;
        rear = 0;
    }

    inline bool enqueue( TYPE & object )
    {
        add( object, (rear + 1) % size );
        rear = (rear + 1) % size;

        if ( front == rear )
            return( false );

        return( true );
    }

    inline TYPE dequeue( void )
    {
        assert( front != rear );

        front = ( front + 1 ) % size;
        return( array[ front ] );
    }

    inline void pop( void )
    {
        assert( front != rear );

        front = ( front + 1 ) % size;
    }

    inline TYPE getFirst( void )
    {
        assert( front != rear );

        return( array[ (( front + 1 ) % size) ] );
    }

    inline TYPE * getFirstPtr( void )
    {
        assert( front != rear );

        return( &(array[ (( front + 1 ) % size) ]) );
    }


    inline bool isEmpty( void ) const
    {
        return front == rear;
    }

    inline bool isFull ( void ) const
    {
        return front == (rear + 1) % size;
    }

    inline bool isReady( void ) const
    {
        return front != rear;
    }

    inline unsigned long itemCount( void ) const
    {
        if ( front > rear )
            return ( (rear+1) + ( (size-1) - front ) );
        else
            return ( rear - front );
    }
};


template< class TYPE >
QueueTemplate< TYPE >::QueueTemplate( unsigned long size )
        : ArrayTemplate<TYPE>( size )
{
    front = 0;
    rear = 0;
}

template< class TYPE >
void QueueTemplate< TYPE >::initialize( unsigned long size )
{
    ArrayTemplate< TYPE >::initialize( size );
    front = 0;
    rear = 0;
}


#endif // ** _QUEUETEMPLATE_HPP
