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
#ifndef __PARRAY_HPP__
#define __PARRAY_HPP__

#include <assert.h>
#include <stdlib.h>
#include "PObject.hpp"

class PArray : public PObject
{
protected:
    PObject **array;
    unsigned long size;
  
public:
    PArray()
	: array(0), size(0)
    { }
    PArray(unsigned long size);
    ~PArray();

    void initialize( unsigned long size );
  
    void sort( unsigned long sort_size, int (* compare)(const void *elem1,const void *elem2 ) );

  inline PObject * operator[]( unsigned long index)
   {
    assert( index < size );
    return( array[index] );
   }
 
  inline void add( PObject *object, unsigned long index )
   {
    assert( index < size );
    array[index] = object;
   }  

  inline unsigned long getSize( void )
   {
    return( size );
   }

  inline void deallocate( void )
   {
    if ( array != 0 )
     {
      delete( array );
      array = 0;
     }
    
    size = 0;
   }
 
 };

#endif // #ifndef __PARRAY_HPP__
