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
#ifndef _PARRAYGROWABLE_HPP
#define _PARRAYGROWABLE_HPP

#include "PObject.hpp"
#include "stdlib.h"

class PArrayGrowable : public PObject
 {
  protected:
   unsigned long size;
   unsigned long grow_increment;
   unsigned long grow_limit;
   PObject **array;
  
  public:
  
  PArrayGrowable();

  PArrayGrowable( unsigned long size, unsigned long growIncrement, 
                  unsigned long growLimit );
  
  PArrayGrowable( unsigned long size, unsigned long growIncrement );

  PArrayGrowable( unsigned long size );
  
  ~PArrayGrowable( void );

  void initialize( unsigned long size, unsigned long growIncrement, 
                   unsigned long growLimit );
  
  PObject * operator[]( unsigned long index);
 
  void add( PObject *object, unsigned long index );
  
  void resize( unsigned long size );

  void setGrowLimit( unsigned long limit );

  void setGrowIncrement( unsigned long growIncrement );

  inline unsigned long getSize( void )
   {
    return( size );
   }
  
  void sort( unsigned long sort_size, int (* compare)( const void *elem1, const void *elem2 ) );
    
  inline void deallocate( void )
   {
    if ( array != 0 )
     {
      free( array );
      array = 0;
     }
    
    size = 0;
   }
 
 };

#endif
