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
#ifndef _UNIT_BUCKET_ARRAY_HPP
#define _UNIT_BUCKET_ARRAY_HPP

#include "BucketArrayTemplate.hpp"

#include "UnitBase.hpp"
#include "UnitID.hpp"
#include "Point.hpp"

class UnitBucketPointer 
 {
  public:
   UnitBase *unit;
   unsigned long index;
   unsigned long player_index;
   
   UnitBucketPointer() 
    { 
     unit = NULL; 
     next = NULL; 
     prev = NULL;
    }
   
   UnitBucketPointer( UnitBase *unit )
    {
	 UnitBucketPointer::unit = unit;
     next = NULL; 
     prev = NULL;
	}

   UnitBucketPointer( UnitBase *unit,  unsigned long index,  unsigned long player_index )
    {
	 UnitBucketPointer::unit = unit;
     UnitBucketPointer::index = index;
     UnitBucketPointer::player_index = player_index;
     next = NULL; 
     prev = NULL;
	}

   UnitBucketPointer *next;
   UnitBucketPointer *prev;
 };


typedef BucketArrayTemplate< UnitBucketPointer > UnitBucketArrayTemplate;
typedef LinkListDoubleTemplate< UnitBucketPointer > UnitBucketList;

class UnitBucketArray : public UnitBucketArrayTemplate
 {
  protected:
   long map_x_sample_factor;
   long map_y_sample_factor;
   long pixel_x_sample_factor;
   long pixel_y_sample_factor;
   PointXYi map_size;
   long map_size_x;
   long map_size_y;
   PointXYi tile_size;

  public:

  UnitBucketArray( );
  ~UnitBucketArray( );

  void initialize( PointXYi map_size, PointXYi tile_size);
  void initialize( PointXYi map_size, PointXYi tile_size, long x_super_sample, long y_super_sample );

  inline long getXSample( void )
   { return( map_x_sample_factor ); }

  inline long getYSample( void )
   { return( map_y_sample_factor ); }
  
  inline PointXYi getSample( void )
   { return( PointXYi( map_x_sample_factor, map_y_sample_factor) ); }
  
  inline unsigned long mapLocToBucketIndex( PointXYi map_loc )
   {
    long bucket_index;

    bucket_index = ((map_loc.y / map_y_sample_factor) * column_size) +
                    (map_loc.x / map_x_sample_factor);   
    
    return( bucket_index );
   }

  inline unsigned long worldLocToBucketIndex( PointXYi world_loc )
   {
    long bucket_index;

    bucket_index = ((world_loc.y / pixel_y_sample_factor) * column_size) +
                    (world_loc.x / pixel_x_sample_factor);   
    
    return( bucket_index );
   }
  
  inline PointXYi mapLocToBucketLoc( PointXYi map_loc )
   {
    return( PointXYi( (map_loc.x / map_x_sample_factor), (map_loc.y / map_y_sample_factor) ) );
   }

  inline PointXYi worldLocToBucketLoc( PointXYi world_loc )
   {
    return( PointXYi(  (world_loc.x / pixel_x_sample_factor) , (world_loc.y / pixel_y_sample_factor) ) );
   }
  
  inline Recti worldRectToBucketRect( Recti &world_rect )
   {
    return( Recti( world_rect.min.x / pixel_x_sample_factor,
                   world_rect.min.y / pixel_y_sample_factor,
                   world_rect.max.x / pixel_x_sample_factor,
                   world_rect.max.y / pixel_y_sample_factor  )
          );
   }

  Recti worldRectToBucketRectClip( Recti &world_rect );


  inline Recti mapRectToBucketRect( Recti &map_rect )
   {
    return( Recti( map_rect.min.x / map_x_sample_factor,
                   map_rect.min.y / map_y_sample_factor,
                   map_rect.max.x / map_x_sample_factor,
                   map_rect.max.y / map_y_sample_factor  )
          );
   }

  UnitBucketList * getBucketAssocWorldLoc( PointXYi world_loc );
  UnitBucketList * getBucketAssocMapLoc( PointXYi map_loc );
 
  void addUnit( UnitBase *unit );
  void addUnit( UnitBucketPointer *unit_bucket_ptr );

  long getUnitBucketIndex( UnitID unit_id );

  UnitBase * getUnit( UnitID unit_id );
  UnitBase * getUnit( UnitID unit_id, unsigned long bucket_index );
  UnitBase * getUnitAtWorldLoc( UnitID unit_id, PointXYi world_loc );
  UnitBase * getUnitAtMapLoc( UnitID unit_id, PointXYi map_loc );

  boolean moveUnit( UnitID unit_id, unsigned long from_bucket_index, unsigned long to_bucket_index );
  
  boolean deleteUnitBucketPointer( UnitID unit_id, PointXYi world_loc );
 };



#endif // ** _UNIT_BUCKET_ARRAY_HPP
