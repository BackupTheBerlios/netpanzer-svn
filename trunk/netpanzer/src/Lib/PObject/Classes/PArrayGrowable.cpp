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

#include <config.h>
#include "PArrayGrowable.hpp"
#include <assert.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

PArrayGrowable::PArrayGrowable()
 {
  array = 0;
  size = 0;
  grow_increment = 1;
  grow_limit = 0xFFFFFFFF;
 }  

PArrayGrowable::PArrayGrowable( unsigned long size, unsigned long growIncrement, 
                                unsigned long growLimit )
 {
  initialize( size , growIncrement, growLimit );
 }
 

PArrayGrowable::PArrayGrowable( unsigned long size, unsigned long growIncrement )
 {
  initialize( size , growIncrement, 0xFFFFFFFF );
 }

PArrayGrowable::PArrayGrowable( unsigned long size )
 {
  initialize( size , 1, 0xFFFFFFFF );
 }
 
void PArrayGrowable::initialize( unsigned long size, unsigned long growIncrement, 
                                 unsigned long growLimit )
 {
  PArrayGrowable::size = size;
   
  grow_increment = growIncrement;
  
  grow_limit = growLimit;

  assert( (size > 0) && (growIncrement > 0) );
  
  if ( array != 0 )
   {
    free( array );
    array = 0;
   }
  
  array = (void**) malloc( sizeof(void*) * size );
 
  assert( array != 0 ); 
 }
 
PArrayGrowable::~PArrayGrowable()
 {
  free( array );
 }

void* PArrayGrowable::operator[]( unsigned long index)
 {
  if ( index >= size )
   {
    unsigned long new_size;
    new_size = (index + 1 + grow_increment);  
    if ( new_size <= grow_limit )
     {
      array = (void**) realloc( array, sizeof(void*) * new_size );
      memset( &array[ size ], 0, sizeof(void*) * (new_size - size) ); 
      size = new_size;
      assert( array != 0 );     
     }
    else
     assert( 0 );
   } 
    
   return( array[index] );
 }
 
void PArrayGrowable::add(void *object, unsigned long index )
 {
  if ( index >= size )
   {
    unsigned long new_size;
    new_size = (index + 1 + grow_increment);  
    if ( new_size <= grow_limit )
     {
      array = (void**) realloc( array, sizeof(void*) * new_size );
      memset( &array[ size ], 0, sizeof(void*) * (new_size - size) ); 
      size = new_size;
      assert( array != 0 );     
     }
    else
     assert( 0 );
   } 

   array[index] = object;
 }  

void PArrayGrowable::resize( unsigned long size )
 {
  array = (void**) realloc( array, sizeof(void*) * size );
  PArrayGrowable::size = size;
  assert( array != 0 );          
 }

void PArrayGrowable::setGrowLimit( unsigned long limit )
 {
  grow_limit = limit;
 }

void PArrayGrowable::setGrowIncrement( unsigned long growIncrement )
 {
  grow_increment = growIncrement;
 }

// XXX what is sorting pointers good for?!?
void PArrayGrowable::sort( unsigned long sort_size, int (* compare)(const void *elem1,const void *elem2 ) )
 {
  qsort( array, sort_size, sizeof(void*), compare );
 }
