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
#ifndef _UNITLIST_HPP
#define _UNITLIST_HPP



#include "PArray.hpp"
#include "UnitBase.hpp"
#include "UnitID.hpp"
#include "LinkListSingleTemplate.hpp"

class UnitPointer 
 {
  public:
   UnitBase *unit;
   unsigned long index;
   
   UnitPointer() { unit = NULL; next = NULL; }
   
   UnitPointer( UnitBase *unit )
    {
	 UnitPointer::unit = unit;
	 next = NULL;
	}

   UnitPointer *next;
 };

typedef LinkListSingleTemplate< UnitPointer  > UnitPointerList; 
 
class UnitList : public UnitPointerList
 {
  protected:
   unsigned long size;

  public:
  unsigned long contains;

  UnitList() 
   { 
    size = 0;
    contains = 0; 
   }  
  
  UnitList( unsigned long size );
  
  void initialize( unsigned long size );
  
  
  inline UnitPointer * operator[]( unsigned long index )
   {
	UnitPointer *traversal_ptr;
    //assert( index < size );
  	 
	traversal_ptr = front;	

	while( (traversal_ptr != NULL) )
	 {
	  if ( traversal_ptr->index == index )
	   return( traversal_ptr );
	  
	  traversal_ptr = traversal_ptr->next;
	 }
  
   	return( NULL );
   }
  
  
  inline void add( UnitBase *unit, unsigned long index )
   {	
	UnitPointer *unit_pointer;

	//assert( index < size );

	unit_pointer = new UnitPointer( unit );
    add( unit_pointer, index );
    
   }  

  inline void add( UnitPointer *unit_pointer, unsigned long index )
   {
    //assert( index < size );	
	UnitPointer *traversal_ptr;
	traversal_ptr = front;

	unit_pointer->index = index;

	
    if( front == NULL )
     {
	  UnitPointerList::addFront( unit_pointer );
      contains++;
	  return;
	 }
	else
	 {
	  if ( index < front->index )
	   { 
	    UnitPointerList::addFront( unit_pointer ); 
        contains++;
		return;
	   }
	  else
	   {
		traversal_ptr = front;
		
		while( traversal_ptr != NULL )
		 {
		  if( index >= traversal_ptr->index  )
		   {
			if( (traversal_ptr->next != NULL) )
			 {
			  if ( (index <= traversal_ptr->next->index )  )
			   {
			    UnitPointerList::insertAfter(traversal_ptr, unit_pointer ); 
			    contains++;
				return;
			   }
			 }
		   	else
		   	 {
			  insertAfter( traversal_ptr, unit_pointer );
              contains++;
			  return;
			 }
		   } // ** if		 
		 
		  traversal_ptr = traversal_ptr->next;
		 } // ** while
	   } // ** else 
	 
	 } // ** else
	
	return;
   }  

  inline boolean isValid( unsigned long index )
   {
    UnitPointer *traversal_ptr;
	  
	//assert( index < size );

	traversal_ptr = front;	

	while( (traversal_ptr != NULL) )
	 {
	  if ( traversal_ptr->index == index )
	   return( _TRUE );
	  
	  traversal_ptr = traversal_ptr->next;
	 }
  
    return( _FALSE );
   }

  inline boolean isValid( UnitID &unitID )
   {
    unsigned long index = unitID.getIndex();
   	UnitPointer *traversal_ptr;
	//assert( index < size );

	traversal_ptr = front;	

	while( (traversal_ptr != NULL) )
	 {
	  if ( traversal_ptr->index == index )
	   return( _TRUE );
	  
	  traversal_ptr = traversal_ptr->next;
	 }
  
    return( _FALSE );
   }

  inline void add( UnitBase *unit, UnitID &unitID )
   {
    unsigned long index;
    index = unitID.getIndex();
    
	//assert( index < size );

    UnitPointer *unit_pointer;
        
	unit_pointer = new UnitPointer( unit );
    add( unit_pointer, index );
        
   }
 
  inline UnitBase * incIterator( UnitPointer **iterator )
   {
    UnitBase *unit;
	if( (*iterator) != NULL )
	 {
	  unit = (*iterator)->unit;
	  *iterator = (*iterator)->next;
	  return( unit ); 
   	 }
   	else
	 { return( NULL ); }
   }  
  

  inline UnitPointer * incIteratorPtr( UnitPointer **iterator )
   {
	UnitPointer *unit_pointer;

	if( (*iterator) != NULL )
	 {
	  unit_pointer = (*iterator);
	  (*iterator) = (*iterator)->next;
	  return( unit_pointer ); 
   	 }
   	else
	 { return( NULL ); }
   }

  inline void resetIterator( UnitPointer **iterator )
   {
   	(*iterator) = front;
   }
    
  inline void resetIteratorAsync( unsigned long *iterator )
   {
   	(*iterator) = 0;
   }

  inline UnitBase * incIteratorAsync( unsigned long *iterator, boolean *completed )
   {
   	UnitPointer *traversal_ptr;

	traversal_ptr = front;	

	while( (traversal_ptr != NULL) )
	 {
	  if ( traversal_ptr->index >= (*iterator) )
	   {
	   	if ( traversal_ptr->next != NULL )
		 { (*iterator) = traversal_ptr->next->index; }
	   	else
		 { (*iterator)++; }
	   
	   	(*completed) = _FALSE;
		return( traversal_ptr->unit );
	   }
	  
	  traversal_ptr = traversal_ptr->next;
	 }
  
    (*completed) = _TRUE;
	return( NULL );
   }  


  boolean addFirstFree( UnitBase *unit, unsigned long *new_index );
        
  inline UnitBase * remove( unsigned long index )
   {
    UnitBase *unit;
    UnitPointer *traversal_ptr;
	//assert( (index < size) );
	
	traversal_ptr = front;

	while( traversal_ptr != NULL )
	 {
	  if ( traversal_ptr->index == index )
	   {
	    if ( contains > 0 )
 	     contains--;

 	     unit = traversal_ptr->unit;
	     traversal_ptr->unit = NULL;

		 return( unit );
	   }
	  traversal_ptr = traversal_ptr->next;
	 } // ** while 
    
    return( NULL ); 
   }

  inline UnitBase * remove( UnitID &unitID )
   {
    unsigned long index;
    index = unitID.getIndex();
  
    return( remove( index ) );   
   }

  void removeAll( void ); 

  inline unsigned long getSize( void )
   { return( size ); }

  inline unsigned long containsItems( void )
   { return( contains );  }
  
  void dispose( unsigned long index );
  void dispose( UnitID &unitID );
  void disposeAll( void );

  void deleteUnit( UnitID &unitID );

  void fill( void );
  void reset( void );
  void cleanUp( void );

 };

 #endif
