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
#ifndef _WORLDMAP_HPP
#define _WORLDMAP_HPP

#include "Aliasdef.h"
#include "PObject.hpp"
#include "MapFileStruct.hpp"
#include "WadMapTable.hpp"

typedef unsigned short MapElementType;

class WorldMap : virtual PObject
 {
  protected:
   boolean map_loaded;
   MAP_HEADER map_info;
   MapElementType *map_buffer;

  public:
     
  WorldMap();

  void loadMapFile( const char *file_path );
  
  void reMap( WadMapTable &mapping_table );
 
  inline boolean isMapLoaded( void )
   { return( map_loaded ); }
  
  inline unsigned char * getName( void )
   {
    return( map_info.name );   
   }
  
  inline unsigned short getXsize( void )
   {
    return( map_info.x_size );
   }
   
  inline unsigned short getYsize( void )
   {
    return( map_info.y_size );
   }

  inline unsigned long getMapSize( void )
   {
    return( map_info.x_size * map_info.y_size );
   }

  void getRawMapBuffer( MapElementType **raw_buffer )
   {
    *raw_buffer = map_buffer; 
   }

  inline void offsetToMapXY( unsigned long offset, unsigned short *map_x, unsigned short *map_y )
   {
    *map_y = (unsigned short ) ( offset / map_info.x_size ); 
    *map_x = (unsigned short ) ( offset - ( (*map_y) * map_info.x_size ) ); 
   }

  inline void mapXYtoOffset( unsigned short map_x, unsigned short map_y, unsigned long *offset )
   {
    *offset = (map_y * map_info.x_size ) + map_x; 
   }

   inline MapElementType mapValue( unsigned short map_x, unsigned short map_y )
    {
     unsigned long offset;
     mapXYtoOffset( map_x, map_y, &offset );  
     
     return( map_buffer[ offset ] );
    }
  
   inline MapElementType mapValue( unsigned long offset )
    {
     return( map_buffer[ offset ] );
    }
   
   char * getAssocTileSet( void )
    {
     return( map_info.tile_set );
    }
 };

#endif // ** _WORLDMAP_HPP 
