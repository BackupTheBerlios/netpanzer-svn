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
#ifndef _PQUEUE_HPP
#define _PQUEUE_HPP

#include "PArray.hpp"
#include <stdlib.h>


class PQueue : public PArray
 {
  public:
   unsigned long front;
   unsigned long rear;

   PQueue( ) { }

   PQueue( unsigned long size );

   void initialize( unsigned long size );

   inline void enqueue(void *object )
    {
     rear = (rear + 1) % size;
     assert( front != rear );
     add( object, rear );
    }

   inline void* dequeue()
    {
     if ( front == rear )
       { return ( 0 ); }
     else
      {
       front = ( front + 1 ) % size;
       return( array[ front ] );
      }
      
    }
 
   inline bool isEmpty() const
   {
	   return front == rear;
   }

   inline bool isReady() const
   {
	   return front != rear;
   }
};

#endif // ** _PQUEUE_HPP
