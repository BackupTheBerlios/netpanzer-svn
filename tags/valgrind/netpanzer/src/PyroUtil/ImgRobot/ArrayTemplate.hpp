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
#ifndef _ARRAYTEMPLATE_HPP
#define _ARRAYTEMPLATE_HPP

#include "aliasdef.h"
#include <assert.h>
#include <stdlib.h>
#include "Pobject.hpp"

template< class TYPE >
class ArrayTemplate : public PObject
 {
  protected:
   unsigned long size;
   TYPE *array;
  
  public:

  ArrayTemplate() 
   { 
    size = 0;
    array = 0; 
   }

  ArrayTemplate( unsigned long size );
  ~ArrayTemplate( void );

  void initialize( unsigned long size );
  
  inline TYPE &operator[]( unsigned long index)
   {
    assert( index < size );
    return( array[index] );
   }
 
  inline void add( TYPE object, unsigned long index )
   {
    assert( index < size );
    memmove( (void *) &array[index], (void *) &object, sizeof( TYPE ) );
   }  

  inline unsigned long getSize( void )
   {
    return( size );
   }
 
  inline void deallocate( void )
   {
    if ( array != 0 )
     {
      delete [size] array;
      array = 0;
     }
    
    size = 0;
   }
 
 };



template< class TYPE >
ArrayTemplate< TYPE >::ArrayTemplate( unsigned long size )
 {
  ArrayTemplate< TYPE >::size = size;
  array = new TYPE [ size ];
  assert( array != 0 );
 }

template< class TYPE >
ArrayTemplate< TYPE >::~ArrayTemplate( void )
 {
  if ( array != 0 )
   delete [size] array;
 }

template< class TYPE >
void ArrayTemplate< TYPE >::initialize( unsigned long size )
 { 
  ArrayTemplate< TYPE >::size = size;
  
  if ( array != 0 )
   {
    delete [size] array ;
    array = 0;
   } 
  
  array = new TYPE [ size ];
  assert( array != 0 );
 }
 

#endif // ** _ARRAYTEMPLATE_HPP
