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
#ifndef _MAPINTERFACE_HPP
#define _MAPINTERFACE_HPP

#include "PObject.hpp"
#include "TileInterface.hpp"
#include "WorldMap.hpp"
#include "SpawnList.hpp"
#include "WadMapTable.hpp"
#include "Point.hpp"

#include "Surface.hpp"

class MapInterface : protected TileInterface
{
protected:
 	static WorldMap main_map;
  	static SpawnList spawn_list;
   	static WadMapTable wad_mapping_table;
	static Surface mini_map_surface;
	static char map_path[256]; 

protected:
	static void generateMappingTable( void );
	static void buildMiniMapSurface( void );

public: 
   static inline void getMapPointSize(PointXYi *map_size)
   {
  	   map_size->x = main_map.getXsize() * tile_set.getTileXsize();
	   map_size->y = main_map.getYsize() * tile_set.getTileYsize(); 
   }
   
   static inline void getMapSize(PointXYi *map_size)
   {
	   map_size->x = main_map.getXsize();
	   map_size->y = main_map.getYsize(); 
   }

   static inline PointXYi getMapSize()
   {
	   return( PointXYi( main_map.getXsize(), main_map.getYsize() ) ); 
   }
      
   static inline unsigned long getMapXsize()
   {
	   return( (unsigned long) (main_map.getXsize()) );
   }
   
   static inline unsigned long getMapYsize()
   {
	   return( (unsigned long) (main_map.getYsize()) );
   }

   static inline MapElementType MapValue(unsigned short map_x,
		   								 unsigned short map_y)
   {
	   return( main_map.mapValue(map_x, map_y) ); 
   }
   
   static inline MapElementType MapValue(unsigned long offset)
   {
	   return( main_map.mapValue(offset) ); 
   }
    
   static void offsetToPointXY( unsigned long offset, 
		   unsigned long *point_x, unsigned long *point_y )
   {
  	   *point_y = ( offset  /  main_map.getXsize() );
	   
	   *point_x = ( offset - ( (*point_y) * main_map.getXsize() ) ) * 32 
		   + (32 / 2);
             
  	   *point_y = (*point_y) * 32 + (32 / 2); 
   }

   static inline void offsetToMapXY( unsigned long offset, unsigned short *map_x, unsigned short *map_y )
   {
 	   main_map.offsetToMapXY( offset, map_x, map_y );
   }
               
   static inline void offsetToMapXY( unsigned long offset, PointXYi *map_loc )
   {
 	   unsigned short map_x, map_y;
  	   main_map.offsetToMapXY( offset, &map_x, &map_y );
   	   map_loc->x = map_x;
	   map_loc->y = map_y;
   }

   static void mapXYtoPointXY( unsigned short map_x, unsigned short map_y,
		   unsigned long *point_x, unsigned long *point_y )
   {
 	   *point_x = (map_x * 32) + (32 / 2);
  	   *point_y = (map_y * 32) + (32 / 2); 
   }

   static void mapXYtoPointXY( PointXYi map_loc, PointXYi *loc )
   {
	   loc->x = (map_loc.x * 32) + (32 / 2);
	   loc->y = (map_loc.y * 32) + (32 / 2); 
   }

   static inline void pointXYtoMapXY( unsigned long point_x,
		   unsigned long point_y, unsigned short *map_x, unsigned short *map_y )
   {
	   *map_x = (unsigned short )  point_x  / 32;
	   *map_y = (unsigned short )  point_y /  32; 
   }

   static inline void pointXYtoMapXY( PointXYi point, PointXYi *map_loc )
   {
	   map_loc->x = (unsigned short )  point.x  / 32;
	   map_loc->y = (unsigned short )  point.y  / 32; 
   }

   static inline void mapXYtoOffset( unsigned short map_x, unsigned short map_y,
		   unsigned long *offset )
   {
	   main_map.mapXYtoOffset( map_x, map_y, offset );
   }

   static inline void mapXYtoOffset( PointXYi &map_loc, unsigned long *offset )
   {
	   main_map.mapXYtoOffset( map_loc.x, map_loc.y, offset );
   } 

   static inline void markLocHack( const PointXYi &loc )
   {
	   unsigned short *map_buffer;
	   unsigned long offset;
	   main_map.getRawMapBuffer( &map_buffer );
	   offset = ( loc.y * getMapXsize() ) + loc.x;
	   map_buffer[ offset ] = 27;
   }
   
   static inline void unmarkLocHack( const PointXYi &loc )
   {
	   unsigned short *map_buffer;
	   unsigned long offset;
	   main_map.getRawMapBuffer( &map_buffer );
	   offset = ( loc.y * getMapXsize() ) + loc.x;
	   map_buffer[ offset ] = 28;
   }

   static void normalizePointXY( unsigned long point_x, unsigned long point_y, unsigned long *norm_x, unsigned long *norm_y )
   {
   	   unsigned short map_x, map_y;
  
	   pointXYtoMapXY( point_x, point_y, &map_x, &map_y );
	   mapXYtoPointXY( map_x, map_y, norm_x, norm_y ); 
   }
   
   static inline void MapInterface::getRawMapBuffer( MapElementType **raw_buffer )
   {
  	   main_map.getRawMapBuffer( raw_buffer );
   }

   static inline WorldMap * getMap( void )
   {
  	   return( &main_map );
   } 

protected: 
   static void finishMapLoad();
public:
   static bool startMapLoad( const char *file_path, bool load_tiles, unsigned long partitions );
   static bool loadMap( int *percent_complete );
   
   static inline bool isMapLoaded()
   { return( main_map.isMapLoaded() ); }    

   static inline Surface * getMiniMapSurface()
   { 
	   return ( &mini_map_surface );
   }
   
   static unsigned char getMovementValue( PointXYi map_loc );

   static unsigned char getAverageColorPointXY( PointXYi &point_loc );
 
   static unsigned char getAverageColorMapXY( PointXYi &map_loc );
            
   static inline void getFreeSpawnPoint( PointXYi *spawn_loc )
   {
	   spawn_list.getFreeSpawnPoint( spawn_loc );
   }
 
   static inline SpawnList * getSpawnList( void )
   {
	   return( &spawn_list );
   }
};

#endif
