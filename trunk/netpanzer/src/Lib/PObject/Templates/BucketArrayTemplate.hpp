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
#ifndef _BUCKETARRAYTEMPLATE_HPP
#define _BUCKETARRAYTEMPLATE_HPP


#include "Pobject.hpp"
#include "LinkListDoubleTemplate.hpp"


template< class TYPE >
class BucketArrayTemplate : public PObject
 {
  protected:
   unsigned long size;
   unsigned long row_size;
   unsigned long column_size;
   LinkListDoubleTemplate< TYPE > *array;

  public:

   BucketArrayTemplate( );

   ~BucketArrayTemplate( );
   
   BucketArrayTemplate( unsigned long size );

   BucketArrayTemplate( unsigned long rows, unsigned long columns );

   void initialize( unsigned long size );

   void initialize( unsigned long rows, unsigned long columns );
 
   void deallocate( void );

   inline unsigned long getSize( void )
    { return( size ); }
   
   inline void addObject( unsigned long bucket_index, TYPE *object )
    {
     assert( bucket_index < size );
     
     array[ bucket_index ].addFront( object );
    }

   inline void addObject( unsigned long row, unsigned long column, TYPE *object )
    {
     unsigned long bucket_index;
     
     bucket_index = (row * column_size) + column;
     assert( bucket_index < size );

     array[ bucket_index ].addFront( object );
    }
  
   inline LinkListDoubleTemplate< TYPE > * getBucket( unsigned long bucket_index )
    {
     assert( bucket_index < size );
     return( &(array[ bucket_index ]) );
    }

   inline LinkListDoubleTemplate< TYPE > * getBucket( unsigned long row, unsigned long column )
    {
     unsigned long bucket_index;
     
     bucket_index = (row * column_size) + column;

     assert( bucket_index < size );
     return( &(array[ bucket_index ]) );
    }
 
 };


template< class TYPE >
BucketArrayTemplate< TYPE >::BucketArrayTemplate( )
 {
  array = NULL;
  size = 0;
  row_size = 0;
  column_size = 0;
 }

template< class TYPE >
BucketArrayTemplate< TYPE >::~BucketArrayTemplate()
 {
  deallocate();
 }


template< class TYPE >
BucketArrayTemplate< TYPE >::BucketArrayTemplate( unsigned long size )
 {
  array = NULL;
  size = 0;
  row_size = 0;
  column_size = 0;
  initialize( size );
 }

template< class TYPE >
BucketArrayTemplate< TYPE >::BucketArrayTemplate( unsigned long rows, unsigned long columns )
 {
  array = NULL;
  size = 0;
  row_size = 0;
  column_size = 0;
  initialize( rows, columns );
 }

template< class TYPE >
void BucketArrayTemplate< TYPE >::initialize( unsigned long size )
 {
  if ( array != NULL )
   { deallocate(); }

  BucketArrayTemplate< TYPE >::size = size;
  
  array = new LinkListDoubleTemplate< TYPE > [ size ];
  
  assert( array != NULL );
 }

template< class TYPE >
void BucketArrayTemplate< TYPE >::initialize( unsigned long rows, unsigned long columns )
 {
  if ( array != NULL )
   { deallocate(); }

  row_size = rows;
  column_size = columns;

  BucketArrayTemplate< TYPE >::size = row_size * column_size;
  
  array = new LinkListDoubleTemplate< TYPE > [ size ];
  
  assert( array != NULL );
 }


template< class TYPE >
void BucketArrayTemplate< TYPE >::deallocate( void )
 {
  for( unsigned long bucket_index = 0; bucket_index < size; bucket_index++ )
   {
    array[ bucket_index ].deallocate();
   }
  
  delete [size] array;
  
  array = NULL;
  size = 0;
  row_size = 0;
  column_size = 0;
 }

#endif // ** _BUCKETARRAYTEMPLATE_HPP
