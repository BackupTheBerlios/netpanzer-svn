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
#ifndef _SAFEPOINTER_HPP
#define _SAFEPOINTER_HPP

#include "aliasdef.h"


#include "PObject.hpp"

class SafePointer : public PObject
 {
  public:
  bool valid;

  PObject *object_ptr;

  SafePointer() 
   { 
    valid = false;
    object_ptr = 0; 
   }

  SafePointer( PObject *object )
   {
    object_ptr = object;
        valid = true;
   }
 
  inline bool isValid( void )
   {
    return ( valid );
   } 
  
  inline void destroy( void )
   {
    if( valid == true )
     { 
      delete( object_ptr );
      object_ptr = 0;
     }
    
    valid = false;
   } 
 
  inline void assign( PObject *object )
   {
    object_ptr = object;
        valid = true;
   }

  inline void operator=( SafePointer &pointer )
   {
    valid = pointer.valid;
        object_ptr = pointer.object_ptr; 
   }
 
  inline PObject * operator->()
   {
    assert( valid == true );
        return( object_ptr );
   } 
 };

//**************************************************************

template< class TYPE >
class SafePointerTemplate : public PObject
 {
  public:
  bool valid;

  TYPE *object_ptr;

  SafePointerTemplate() 
   { 
    valid = false;
    object_ptr = 0; 
   }

  SafePointerTemplate( TYPE *object )
   {
    object_ptr = object;
    valid = true;
   }
 
  inline bool isValid( void )
   {
    return ( valid );
   } 
  
  inline void destroy( void )
   {
    if( valid == true )
     { 
      delete( object_ptr );
      object_ptr = 0;
     }
    valid = false;
   } 
 
  inline void assign( TYPE *object )
   {
    object_ptr = object;
        valid = true;
   }

  inline void operator=( SafePointerTemplate &pointer )
   {
    valid = pointer.valid;
        object_ptr = pointer.object_ptr; 
   }
 
  inline TYPE * operator->()
   {
    assert( valid == true );
        return( object_ptr );
   } 
 };

#endif // ** _SAFEPOINTER
