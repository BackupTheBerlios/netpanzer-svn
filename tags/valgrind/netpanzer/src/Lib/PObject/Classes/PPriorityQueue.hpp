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
#ifndef _PPRIORITYQUEUE_HPP
#define _PPRIORITYQUEUE_HPP

#include <stdlib.h>
#include "PHeap.hpp"

class PPriorityQueue : public PHeap
 {
  public:
  
  PPriorityQueue( unsigned long queue_size );
  
  PPriorityQueue( void );
  
  inline void push( PObject *object )
   {
    last++;
    add( object, last);
    shiftUp();
   }
  
  inline PObject * pop( void )
   {
    PObject *object;
    if ( array[ root ] == 0 ) 
     {
      return( 0 );
     }
    else
     { 
      object = array[ root ];
      array[ root ] =  0;
      array[ root ] = array[ last ];  
      last--;
      shiftDown();
      return( object );
     }

   } 



 };

#endif // * _PPRIORITYQUEUE_HPP
