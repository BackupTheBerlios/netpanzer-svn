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
#ifndef _LINKLISTDOUBLETEMPLATE_HPP
#define _LINKLISTDOUBLETEMPLATE_HPP

#include <stdlib.h>

template< class TYPE >
class LinkListDoubleTemplate
{
protected:
    TYPE *front;
    TYPE *rear;

public:

    LinkListDoubleTemplate()
            : front(0), rear(0)
    { }

    LinkListDoubleTemplate( unsigned long size );

    ~LinkListDoubleTemplate( void );

    void initialize( unsigned long size );

    inline TYPE * getFront( void )
    {
        return( front );
    }

    inline TYPE * getRear( void )
    {
        return( rear );
    }

    inline void addRear( TYPE *object )
    {
        object->next = 0;
        object->prev = 0;

        if ( (front == 0) && (rear == 0) ) {
            front = object;
            rear = object;
        } else {
            object->prev = rear;
            rear->next = object;
            rear = object;
        }
    }

    inline void addFront( TYPE *object )
    {
        object->next = 0;
        object->prev = 0;

        if ( (front == 0) && (rear == 0) ) {
            front = object;
            rear = object;
        } else {
            object->next = front;
            front->prev = object;
            front = object;
        }
    }

    inline void insertAfter( TYPE *after, TYPE *object )
    {
        assert( after != 0 );

        object->next = 0;
        object->prev = 0;

        if( after->next != 0 ) {
            after->next->prev = object;
        }

        object->next = after->next;
        after->next = object;
        object->prev = after;

        if( after == rear ) {
            rear = object;
        }
    }

    inline void insertBefore( TYPE *before, TYPE *object )
    {
        assert( before != 0 );

        object->next = 0;
        object->prev = 0;

        if( before->prev != 0 ) {
            before->prev->next = object;
        }

        object->prev = before->prev;
        before->prev = object;
        object->next = before;

        if( before == front ) {
            front = object;
        }
    }

    inline void deleteObject( TYPE *object )
    {
        if( object->prev != 0 ) {
            object->prev->next = object->next;
        }

        if( object->next != 0 ) {
            object->next->prev = object->prev;
        }

        if( object == front ) {
            front = object->next;
        }

        if( object == rear ) {
            rear = object->prev;
        }

        delete( object );
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
                front->prev = 0;
            }

            delete( delete_ptr );
        }
    }

    inline void deleteRear( void )
    {
        TYPE *delete_ptr;

        delete_ptr = rear;

        if ( rear != 0 ) {
            if (front == rear) {
                front = 0;
                rear = 0;
            } else {
                rear = rear->prev;
                rear->next = 0;
            }

            delete( delete_ptr );
        }
    }

    inline void deleteAfter( TYPE *after )
    {
        TYPE *delete_ptr;
        assert( after != 0 );

        delete_ptr = after->next;

        if( after->next != 0 ) {
            if( after->next->next != 0 ) {
                after->next->next->prev = after;
                after->next = after->next->next ;
            } else {
                after->next = 0;
            }

            if( delete_ptr == rear ) {
                rear = after;
            }

            delete( delete_ptr );
        }
    }

    inline void deleteBefore( TYPE *before )
    {
        TYPE *delete_ptr;
        assert( before != 0 );

        delete_ptr = before->prev;

        if( before->prev != 0 ) {
            if( before->prev->prev != 0 ) {
                before->prev->prev->next = before;
                before->prev = before->prev->prev;
            } else {
                before->prev = 0;
            }

            if( delete_ptr == front ) {
                front = before;
            }

            delete( delete_ptr );
        }
    }

    inline void removeObject( TYPE *object )
    {
        if( object->prev != 0 ) {
            object->prev->next = object->next;
        }

        if( object->next != 0 ) {
            object->next->prev = object->prev;
        }

        if( object == front ) {
            front = object->next;
        }

        if( object == rear ) {
            rear = object->prev;
        }
    }


    inline TYPE* removeFront( void )
    {
        TYPE *delete_ptr;

        delete_ptr = front;

        if ( front != 0 ) {
            if (front == rear) {
                front = 0;
                rear = 0;
            } else {
                front = front->next;
                front->prev = 0;
            }

            return( delete_ptr );
        }

        return( 0 );
    }

    inline TYPE* removeRear( void )
    {
        TYPE *delete_ptr;

        delete_ptr = rear;

        if ( rear != 0 ) {
            if (front == rear) {
                front = 0;
                rear = 0;
            } else {
                rear = rear->prev;
                rear->next = 0;
            }

            return( delete_ptr );
        }
        return( 0 );
    }

    inline TYPE* removeAfter( TYPE *after )
    {
        TYPE *delete_ptr;
        assert( after != 0 );

        delete_ptr = after->next;

        if( after->next != 0 ) {
            if( after->next->next != 0 ) {
                after->next->next->prev = after;
                after->next = after->next->next ;
            } else {
                after->next = 0;
            }

            if( delete_ptr == rear ) {
                rear = after;
            }

            return( delete_ptr );
        }

        return( 0 );
    }

    inline TYPE* removeBefore( TYPE *before )
    {
        TYPE *delete_ptr;
        assert( before != 0 );

        delete_ptr = before->prev;

        if( before->prev != 0 ) {
            if( before->prev->prev != 0 ) {
                before->prev->prev->next = before;
                before->prev = before->prev->prev;
            } else {
                before->prev = 0;
            }

            if( delete_ptr == front ) {
                front = before;
            }

            return( delete_ptr );
        }
        return( 0 );
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
LinkListDoubleTemplate< TYPE >::LinkListDoubleTemplate( unsigned long size )
        : front(0), rear(0)
{
    initialize( size );
}

template< class TYPE >
LinkListDoubleTemplate< TYPE >::~LinkListDoubleTemplate( void )
{
    deallocate();
}

template< class TYPE >
void LinkListDoubleTemplate< TYPE >::initialize( unsigned long size )
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


#endif // ** _LINKLISTDOUBLETEMPLATE_HPP
