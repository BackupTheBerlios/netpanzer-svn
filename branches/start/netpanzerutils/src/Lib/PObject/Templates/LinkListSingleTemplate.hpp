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
#ifndef _LINKLISTSINGLETEMPLATE_HPP
#define _LINKLISTSINGLETEMPLATE_HPP

#include <stdlib.h>

template< class TYPE >
class LinkListSingleTemplate
{
protected:
    TYPE *front;
    TYPE *rear;

public:

    LinkListSingleTemplate()
            : front(0), rear(0)
    {}

    LinkListSingleTemplate( unsigned long size );

    ~LinkListSingleTemplate( void );

    void initialize( unsigned long size );

    inline void addRear( TYPE *object )
    {
        object->next = 0;

        if ( (front == 0) && (rear == 0) ) {
            front = object;
            rear = object;
        } else {
            rear->next = object;
            rear = rear->next;
        }
    }

    inline void addFront( TYPE *object )
    {
        object->next = 0;

        if ( (front == 0) && (rear == 0) ) {
            front = object;
            rear = object;
            object->next = 0;
        } else {
            object->next = front;
            front = object;
        }
    }

    inline void insertAfter( TYPE *after, TYPE *object )
    {
        assert( after != 0 );

        object->next = after->next;
        after->next = object;
        if( after == rear ) {
            rear = object;
        }
    }

    inline void deleteFront( void )
    {
        TYPE *delete_ptr;

        delete_ptr = front;

        if ( front != 0 ) {
            if (front == rear) {
                front = 0;
                rear = 0;
            } else {
                front = front->next;
            }

            delete( delete_ptr );
        }
    }

    inline void deleteAfter( TYPE *after )
    {
        TYPE *delete_ptr;
        assert( after != 0 );

        delete_ptr = after->next;

        after->next = delete_ptr->next;

        if( delete_ptr == rear ) {
            rear = after;
        }

        delete( delete_ptr );
    }

    inline TYPE * removeFront( void )
    {
        TYPE *delete_ptr;
        assert( front != 0 );

        delete_ptr = front;

        if ( front != 0 ) {
            if (front == rear) {
                front = 0;
                rear = 0;
            } else {
                front = front->next;
            }

            return( delete_ptr );
        }

        return( 0 );
    }

    inline TYPE * removeAfter( TYPE *after )
    {
        TYPE *delete_ptr;
        assert( after != 0 );

        delete_ptr = after->next;

        after->next = delete_ptr->next;

        if( delete_ptr == rear ) {
            rear = after;
        }

        return( delete_ptr );
    }



    inline unsigned long getSize( void )
    {
        TYPE *traversal_ptr;
        unsigned long count = 0;

        traversal_ptr = front;
        while( traversal_ptr != 0 ) {
            traversal_ptr = traversal_ptr->next;
            count++;
        }

        return( count );
    }

    inline void deallocate( void )
    {
        TYPE *traversal_ptr;
        TYPE *delete_ptr;

        traversal_ptr = front;

        while( traversal_ptr != 0 ) {
            delete_ptr = traversal_ptr;
            traversal_ptr = traversal_ptr->next;
            delete( delete_ptr );
        }

        front = 0;
        rear = 0;
    }

};



template< class TYPE >
LinkListSingleTemplate< TYPE >::LinkListSingleTemplate( unsigned long size )
        : front(0), rear(0)
{
    initialize( size );
}

template< class TYPE >
LinkListSingleTemplate< TYPE >::~LinkListSingleTemplate( void )
{
    deallocate();
}

template< class TYPE >
void LinkListSingleTemplate<TYPE>::initialize( unsigned long size )
{
    TYPE *object;

    if( front != 0 ) {
        deallocate();
    }

    for( unsigned long i; i < size; i++ ) {
        object = new TYPE;
        addRear( object );
    }

}


#endif // ** _LINKLISTSINGLETEMPLATE_HPP
