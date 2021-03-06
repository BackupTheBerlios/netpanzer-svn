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
#include "UnitBucketArray.hpp"

UnitBucketArray::UnitBucketArray( )
 : UnitBucketArrayTemplate( )
 {
  map_x_sample_factor = 0;
  map_y_sample_factor = 0;
 }

UnitBucketArray::~UnitBucketArray( )
 {
  
 }

void UnitBucketArray::initialize( PointXYi map_size, PointXYi tile_size )
 {
  initialize( map_size, tile_size, 10, 10 );
 }

void UnitBucketArray::initialize( PointXYi map_size, PointXYi tile_size, long x_super_sample, long y_super_sample )
 {
  unsigned long rows, columns;
 
  assert( x_super_sample >= 1 );
  assert( y_super_sample >= 1 );

  UnitBucketArray::tile_size = tile_size;
  UnitBucketArray::map_size = map_size;
  UnitBucketArray::map_size_x = map_size.x;
  UnitBucketArray::map_size_y = map_size.y;

  map_x_sample_factor = x_super_sample;
  map_y_sample_factor = y_super_sample;

  while( (map_size.x % map_x_sample_factor) > 0 )
   { map_x_sample_factor++; }

  while( (map_size.y % map_y_sample_factor) > 0 )
   { map_y_sample_factor++; }

  pixel_x_sample_factor = tile_size.x * map_x_sample_factor;
  pixel_y_sample_factor = tile_size.y * map_y_sample_factor;

  rows = (unsigned long) map_size.y / map_y_sample_factor;
  columns = (unsigned long) map_size.x / map_x_sample_factor;

  UnitBucketArrayTemplate::initialize( rows, columns );
 }

Recti UnitBucketArray::worldRectToBucketRectClip( Recti &world_rect )
 {
  long bucket_max_x;
  long bucket_max_y;

  bucket_max_x = world_rect.max.x / pixel_x_sample_factor;
  if ( bucket_max_x >= (long) column_size )   
   { bucket_max_x = column_size - 1; }

  bucket_max_y = world_rect.max.y / pixel_y_sample_factor;
  if ( bucket_max_y >= (long) row_size )   
   { bucket_max_y = row_size  - 1; }
 
  return( Recti( world_rect.min.x / pixel_x_sample_factor,
                 world_rect.min.y / pixel_y_sample_factor,
                 bucket_max_x,
                 bucket_max_y
               )
         );
 }


UnitBucketList * UnitBucketArray::getBucketAssocWorldLoc( PointXYi world_loc )
 {
  long bucket_index;

  bucket_index = ((world_loc.y / pixel_y_sample_factor) * column_size) +
                  (world_loc.x / pixel_x_sample_factor);   
 
  assert( bucket_index < (long) size );
  
  return( &(array[ bucket_index ]) );
 }

UnitBucketList * UnitBucketArray::getBucketAssocMapLoc( PointXYi map_loc )
 {
  long bucket_index;

  bucket_index = ((map_loc.y / map_y_sample_factor) * column_size) +
                  (map_loc.x / map_x_sample_factor);   
 
  assert( bucket_index < (long) size );
  
  return( &(array[ bucket_index ]) );
 }

void UnitBucketArray::addUnit( UnitBase *unit )
 {
  UnitBucketPointer *unit_bucket_ptr;
  long bucket_index;
  
  bucket_index = ((unit->unit_state.location.y / pixel_y_sample_factor) * column_size) +
                  (unit->unit_state.location.x / pixel_x_sample_factor);   
 
  assert( bucket_index < (long) size );
 
  unit_bucket_ptr = new UnitBucketPointer( unit, 
                                           unit->unit_id.getIndex(),
                                           unit->unit_id.getPlayer() 
                                         );
  
  array[ bucket_index ].addFront( unit_bucket_ptr );
 }

void UnitBucketArray::addUnit( UnitBucketPointer *unit_bucket_ptr )
 {
  long bucket_index;
  UnitBase *unit;

  unit = unit_bucket_ptr->unit;
  
  bucket_index = ((unit->unit_state.location.y / pixel_y_sample_factor) * column_size) +
                  (unit->unit_state.location.x / pixel_x_sample_factor);   
 
  assert( bucket_index < (long) size );
  
  array[ bucket_index ].addFront( unit_bucket_ptr );
 }


long UnitBucketArray::getUnitBucketIndex( UnitID unit_id )
 {
  unsigned long unique_index;
  
  unique_index = unit_id.getIndex();

  for( unsigned long bucket_index = 0; bucket_index < size; bucket_index++ )
   {
    UnitBucketPointer *traversal_ptr;
    
    traversal_ptr = array[ bucket_index ].getFront();
    
    while( traversal_ptr != 0 )
     {
      if( traversal_ptr->index == unique_index )     
       return( (long) bucket_index );

      traversal_ptr = traversal_ptr->next;
     }  

   }
  
  return( -1 );
 }

UnitBase * UnitBucketArray::getUnit( UnitID unit_id )
 {
  unsigned long unique_index;
  
  unique_index = unit_id.getIndex();

  for( unsigned long bucket_index = 0; bucket_index < size; bucket_index++ )
   {
    UnitBucketPointer *traversal_ptr;
    
    traversal_ptr = array[ bucket_index ].getFront();
    
    while( traversal_ptr != 0 )
     {
      if( traversal_ptr->index == unique_index )     
       return( traversal_ptr->unit );

      traversal_ptr = traversal_ptr->next;
     }  

   }
  return( 0 );
 }


UnitBase * UnitBucketArray::getUnit( UnitID unit_id, unsigned long bucket_index )
 {
  unsigned long unique_index;
  
  unique_index = unit_id.getIndex();

  UnitBucketPointer *traversal_ptr;
    
  traversal_ptr = array[ bucket_index ].getFront();
    
  while( traversal_ptr != 0 )
   {
    if( traversal_ptr->index == unique_index )     
     return( traversal_ptr->unit );

    traversal_ptr = traversal_ptr->next;
   }  
 
  return( 0 );
 }

UnitBase * UnitBucketArray::getUnitAtWorldLoc( UnitID unit_id, PointXYi world_loc )
 {
  long bucket_index;
  unsigned long unique_index;

  unique_index = unit_id.getIndex();
  
  bucket_index = ((world_loc.y / pixel_y_sample_factor) * column_size) +
                  (world_loc.x / pixel_x_sample_factor);   
 
  assert( bucket_index < (long) size );

  UnitBucketPointer *traversal_ptr;
    
  traversal_ptr = array[ bucket_index ].getFront();
    
  while( traversal_ptr != 0 )
   {
    if( traversal_ptr->index == unique_index )     
     return( traversal_ptr->unit );

    traversal_ptr = traversal_ptr->next;
   }  
 
  return( 0 );
 }

UnitBase * UnitBucketArray::getUnitAtMapLoc( UnitID unit_id, PointXYi map_loc )
 {
  long bucket_index;
  unsigned long unique_index;

  unique_index = unit_id.getIndex();
  
  bucket_index = ((map_loc.y / map_y_sample_factor) * column_size) +
                  (map_loc.x / map_x_sample_factor);   
 
  assert( bucket_index < (long) size );

  UnitBucketPointer *traversal_ptr;
    
  traversal_ptr = array[ bucket_index ].getFront();
    
  while( traversal_ptr != 0 )
   {
    if( traversal_ptr->index == unique_index )     
     return( traversal_ptr->unit );

    traversal_ptr = traversal_ptr->next;
   }  
 
  return( 0 );
 }

bool UnitBucketArray::moveUnit( UnitID unit_id, unsigned long from_bucket_index, unsigned long to_bucket_index )
 {
  bool found = false;
  UnitBucketPointer *traversal_ptr;
  UnitBucketPointer *move_ptr;
  unsigned long unique_index;

  unique_index = unit_id.getIndex();
    
  traversal_ptr = array[ from_bucket_index ].getFront();
    
  while( (traversal_ptr != 0) && (found == false) )
   {
    if( traversal_ptr->index == unique_index )     
     {
      move_ptr = traversal_ptr;
      traversal_ptr = traversal_ptr->next;
      array[ from_bucket_index ].removeObject( move_ptr );
      array[ to_bucket_index ].addFront( move_ptr );
      found = true;
     }
    else
     {
      traversal_ptr = traversal_ptr->next;
     }
   }  
  
  if ( found == false )
   {
    from_bucket_index = getUnitBucketIndex( unit_id );
    if( from_bucket_index  != -1 )
     {
      return( moveUnit( unit_id, from_bucket_index, to_bucket_index ) );
     }     
    else
     return( false );
   }
  
  return( true );
 }

bool UnitBucketArray::deleteUnitBucketPointer( UnitID unit_id, PointXYi world_loc )
 {
  long bucket_index;
  unsigned long unique_index;

  unique_index = unit_id.getIndex();

  bucket_index = worldLocToBucketIndex( world_loc );

  UnitBucketPointer *traversal_ptr;
    
  traversal_ptr = array[ bucket_index ].getFront();
    
  while( traversal_ptr != 0 )
   {
    if( traversal_ptr->index == unique_index )     
     {
      array[ bucket_index ].deleteObject( traversal_ptr );
      return( true );
     }
    
    traversal_ptr = traversal_ptr->next;
   }         
  
  return( false );
 }
