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
#ifndef _LINKLISTDOUBLETEMPLATE_HPP
#define _LINKLISTDOUBLETEMPLATE_HPP

#include "aliasdef.h"

#include <stdlib.h>
#include "Pobject.hpp"

template< class TYPE >
class LinkListDoubleTemplate : public PObject
 {
  protected:
   TYPE *front;
   TYPE *rear;
   
  public:

  LinkListDoubleTemplate() 
   { 
	front = NULL;
	rear = NULL;
   }

  LinkListDoubleTemplate( unsigned long size );  
  
  ~LinkListDoubleTemplate( void );

  void initialize( unsigned long size );
  
  inline TYPE * getFront( void )
   { return( front ); }

  inline TYPE * getRear( void )
   { return( rear ); }
  
  inline void addRear( TYPE *object )
   {
	object->next = NULL;
    object->prev = NULL;

	if ( (front == NULL) && (rear == NULL) )
	 {
	  front = object;
	  rear = object;
	 }
   	else
	 {
	  object->prev = rear;
      rear->next = object;
	  rear = object;
	 }   
   }
  
  inline void addFront( TYPE *object )
   {
	object->next = NULL;
    object->prev = NULL;

	if ( (front == NULL) && (rear == NULL) )
	 {
	  front = object;
	  rear = object;
	 }
   	else
	 {
	  object->next = front;
      front->prev = object;
	  front = object;
	 }   
   }

  inline void insertAfter( TYPE *after, TYPE *object )
   {
    assert( after != NULL );
	
	object->next = NULL;
    object->prev = NULL;
	
    if( after->next != NULL )
     { after->next->prev = object; }

    object->next = after->next;
    after->next = object;
    object->prev = after;

 	if( after == rear )
	 {
	  rear = object;
	 }	 
   }

  inline void insertBefore( TYPE *before, TYPE *object )
   {
    assert( before != NULL );
	
	object->next = NULL;
    object->prev = NULL;
	
    if( before->prev != NULL )
     { before->prev->next = object; }

    object->prev = before->prev;
    before->prev = object;
    object->next = before;

 	if( before == front )
	 {
	  front = object;
	 }	 
   }

  inline void deleteObject( TYPE *object )
   {
    if( object->prev != NULL )
     { object->prev->next = object->next; }

    if( object->next != NULL )
     { object->next->prev = object->prev; }
    
    if( object == front )
     { front = object->next; }

    if( object == rear )
     { rear = object->prev; }
    
    delete( object );
   }  

  inline void deleteFront( void )
   {
    TYPE *delete_ptr;
	
	delete_ptr = front;

	if ( front != NULL )
	 {
	  if (front == rear)
       {
        front = NULL;
        rear = NULL;
       }
      else
       { 
        front = front->next; 
        front->prev = NULL;        
       }
       
      delete( delete_ptr );
   	 }
   }

  inline void deleteRear( void )
   {
    TYPE *delete_ptr;
	
	delete_ptr = rear;

	if ( rear != NULL )
	 {
	  if (front == rear)
       {
        front = NULL;
        rear = NULL;
       }
      else
       { 
        rear = rear->prev; 
        rear->next = NULL; 
       }

      delete( delete_ptr );
   	 }
   }
   
  inline void deleteAfter( TYPE *after )
   {
    TYPE *delete_ptr;
	assert( after != NULL );
	
	delete_ptr = after->next;
    
	if( after->next != NULL )
     {
      if( after->next->next != NULL )
       {
   	    after->next->next->prev = after;
        after->next = after->next->next ;
       }
      else
       { after->next = NULL; }

      if( delete_ptr == rear )
	   {
	    rear = after;
	   }	 
 
      delete( delete_ptr );
     }
   }

  inline void deleteBefore( TYPE *before )
   {
    TYPE *delete_ptr;
	assert( before != NULL );
	
	delete_ptr = before->prev;
    
	if( before->prev != NULL )
     {
      if( before->prev->prev != NULL )
       {
   	    before->prev->prev->next = before;
        before->prev = before->prev->prev;
       }
      else
       { before->prev = NULL; }

      if( delete_ptr == front )
	   {
	    front = before;
	   }	 
 
      delete( delete_ptr );
     }
   }

  inline void removeObject( TYPE *object )
   {
    if( object->prev != NULL )
     { object->prev->next = object->next; }

    if( object->next != NULL )
     { object->next->prev = object->prev; }
    
    if( object == front )
     { front = object->next; }

    if( object == rear )
     { rear = object->prev; } 
   }  

 
  inline TYPE* removeFront( void )
   {
    TYPE *delete_ptr;
	
	delete_ptr = front;

	if ( front != NULL )
	 {
	  if (front == rear)
       {
        front = NULL;
        rear = NULL;
       }
      else
       { 
        front = front->next; 
        front->prev = NULL;        
       }

      return( delete_ptr );
   	 }
    
    return( NULL );
   }

  inline TYPE* removeRear( void )
   {
    TYPE *delete_ptr;
	
	delete_ptr = rear;

	if ( rear != NULL )
	 {
	  if (front == rear)
       {
        front = NULL;
        rear = NULL;
       }
      else
       { 
        rear = rear->prev; 
        rear->next = NULL; 
       }

      return( delete_ptr );
   	 }
    return( NULL );
   }
   
  inline TYPE* removeAfter( TYPE *after )
   {
    TYPE *delete_ptr;
	assert( after != NULL );
	
	delete_ptr = after->next;
    
	if( after->next != NULL )
     {
      if( after->next->next != NULL )
       {
   	    after->next->next->prev = after;
        after->next = after->next->next ;
       }
      else
       { after->next = NULL; }

      if( delete_ptr == rear )
	   {
	    rear = after;
	   }	 
 
      return( delete_ptr );
     }
    
    return( NULL );
   }

  inline TYPE* removeBefore( TYPE *before )
   {
    TYPE *delete_ptr;
	assert( before != NULL );
	
	delete_ptr = before->prev;
    
	if( before->prev != NULL )
     {
      if( before->prev->prev != NULL )
       {
   	    before->prev->prev->next = before;
        before->prev = before->prev->prev;
       }
      else
       { before->prev = NULL; }

      if( delete_ptr == front )
	   {
	    front = before;
	   }	 
 
      return( delete_ptr );
     }
    return( NULL );
   }

 
  inline unsigned long getSize( void )
   {
	TYPE *traversal_ptr;
	unsigned long count = 0;

	traversal_ptr = front;	
	while( traversal_ptr != NULL )
	 {
	  traversal_ptr = traversal_ptr->next;
	  count++;
	 }
   	
	return( count );
   }
 
  inline void deallocate( void )
   {
	TYPE *traversal_ptr;
	TYPE *delete_ptr;

	traversal_ptr = front;	
	
	while( traversal_ptr != NULL )
	 {
	  delete_ptr = traversal_ptr;
	  traversal_ptr = traversal_ptr->next;	  
	  delete( delete_ptr );
	 }
   	
   	front = NULL;
  	rear = NULL;
   }
 
 };



template< class TYPE >
LinkListDoubleTemplate< TYPE >::LinkListDoubleTemplate( unsigned long size )
 {
  initialize( size );
 }

template< class TYPE >
LinkListDoubleTemplate< TYPE >::~LinkListDoubleTemplate( void )
 {
  deallocate();
 }

template< class TYPE >
void LinkListDoubleTemplate< TYPE >::initialize( unsigned long size )
 { 
  TYPE *object;
  
  if( front != NULL )
   { deallocate() };

  for( unsigned long i; i < size; i++ )
   {
	object = new TYPE;
   	addRear( object );
   }

 }
 

#endif // ** _LINKLISTDOUBLETEMPLATE_HPP
