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

#include "Util/NoCopy.hpp"

template< class TYPE >
class LinkListSingleTemplate : public NoCopy
{
protected:
    TYPE *front;
    TYPE *rear;

public:
    LinkListSingleTemplate()
            : front(0), rear(0)
    {}

    ~LinkListSingleTemplate()
    {
        deallocate();
    }

    void addRear(TYPE *object)
    {
        object->next = 0;

        if(rear != 0)
            rear->next = object;
        else {
            front = object;
        }
        
        rear = object;
    }

    void addFront( TYPE *object )
    {
        object->next = front;
        front = object;

        if(rear == 0)
            rear = object;
    }

    void insertAfter( TYPE *after, TYPE *object )
    {
        assert( after != 0 );

        object->next = after->next;
        after->next = object;
        if( after == rear ) {
            rear = object;
        }
    }

    void deleteFront()
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

    void deleteAfter( TYPE *after )
    {
        TYPE *delete_ptr;
        assert( after != 0 );

        delete_ptr = after->next;

        after->next = delete_ptr->next;

        if( delete_ptr == rear ) {
            rear = after;
        }

        delete delete_ptr;
    }

    TYPE* removeFront()
    {
        assert( front != 0 );
        
        TYPE *delete_ptr = front;

        if ( front != 0 ) {
            if (front == rear) {
                front = 0;
                rear = 0;
            } else {
                front = front->next;
            }

            return delete_ptr;
        }

        return 0;
    }

    TYPE* removeAfter(TYPE *after)
    {
        assert( after != 0 );
        
        TYPE *delete_ptr;

        delete_ptr = after->next;

        after->next = delete_ptr->next;

        if(delete_ptr == rear) {
            rear = after;
        }

        return delete_ptr;
    }
    
    size_t getSize() const
    {
        size_t count = 0;

        for(TYPE* p = front; p != 0; p = p->next)
            count++;

        return count;
    }

    void deallocate()
    {
        TYPE *traversal_ptr;
        TYPE *delete_ptr;

        traversal_ptr = front;

        while( traversal_ptr != 0 ) {
            delete_ptr = traversal_ptr;
            traversal_ptr = traversal_ptr->next;
            delete delete_ptr;
        }

        front = 0;
        rear = 0;
    }
};

#endif // ** _LINKLISTSINGLETEMPLATE_HPP
