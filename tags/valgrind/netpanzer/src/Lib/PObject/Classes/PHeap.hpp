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
#ifndef _PHEAP_HPP
#define _PHEAP_HPP

#include "PArrayGrowable.hpp"

class PHeap : public PArrayGrowable
 {
  protected:
  int (* compare_func)( PObject *object1, PObject *object2);  
  unsigned long root;
  unsigned long last;
  
  public:

  PHeap( unsigned long heap_size );
  
  PHeap( void );

  void initialize( unsigned long size, unsigned long growIncrement, 
                   unsigned long growLimit );
  
  inline void reset( void )
   {
    root = 1;
    last = 0;
   }
  
  void setCompare( int (* compare_func_ptr)( PObject *object1, PObject *object2) )
   {
    compare_func = compare_func_ptr;
   }
  
  void shiftUp( void );
  
  void shiftDown( void );
  
 };

#endif // ** _PHEAP_HPP
