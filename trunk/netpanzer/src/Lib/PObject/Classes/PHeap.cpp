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
#include "PHeap.hpp"

PHeap::PHeap( unsigned long heap_size )
  : PArrayGrowable( heap_size )
 {
  root = 1;
  last = 0;
 }
  
PHeap::PHeap( void )  
 : PArrayGrowable( )
 {
  root = 1;
  last = 0;
 }

void PHeap::initialize( unsigned long size, unsigned long growIncrement, 
                        unsigned long growLimit ) 
 {
  PArrayGrowable::initialize( size, growIncrement, growLimit );
  root = 1;
  last = 0;
 }
  


void PHeap::shiftUp( void )
 {
  PObject *temp;
  unsigned long pred; 
  unsigned long ptr;
  
  ptr = last;
  
  pred = ptr >> 1;  // / 2
   
  while ( (ptr > root) && (compare_func( array[pred], array[ptr] ) > 0)  )
   {
     temp = array[ ptr ];
     array[ ptr  ] = array[ pred ];
     array[ pred ] = temp;
   
     ptr = pred;
     
     pred = ptr >> 1; 
   }

 }
  
void PHeap::shiftDown( void )
 {
  unsigned long ptr, succ;
  PObject *root_temp;
  
  ptr = root; 
  succ = ptr << 1;  // * 2
  
  if ( succ < last )
   if ( (compare_func( array[succ + 1], array[ succ ] ) < 0) )
     succ++;
   
  root_temp = array[ root ];
  

   bool not_done = false;

   if( (succ <= last) )
    {
     if ( (compare_func( array[succ], root_temp ) < 0) )
      not_done = true;
    }

   while ( not_done )
    {
     array[ ptr ] = array[ succ ];
    
     ptr = succ;
     succ = ptr << 1;
    
     if ( succ < last )
      if ( (compare_func( array[succ + 1], array[ succ ] ) < 0) )
          succ++;

     
     not_done = false; 
     if( (succ <= last) )
      {
       if ( (compare_func( array[succ], root_temp ) < 0) )
        not_done = true;
      }
    
    } // ** while

  array[ ptr ] = root_temp;
 }
