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
#include "WorldMap.hpp"
#include "MapData.hpp"

#include <stdlib.h>

#include <stdio.h>
#include <string.h>

WorldMap::WorldMap()
 {
  map_buffer = 0;
 }

void WorldMap::reMap( WadMapTable &mapping_table )
 {
  unsigned long map_size;
  unsigned long map_index;
  unsigned short tile_value;

  map_size = map_info.x_size * map_info.y_size;
  
  for ( map_index = 0; map_index < map_size; map_index++ )
   {
    tile_value = map_buffer[ map_index ];
    map_buffer[ map_index  ] = mapping_table[ tile_value ].remap_index;
   }

 }


void WorldMap::loadMapFile( const char *file_path )
 {
  FILE *infile;
  //map_file_header_type map_header;
  unsigned long map_size;
  
/*  
  infile = fopen( file_path, "rb" );  
  assert( infile != 0 );  

  if ( map_loaded == true )
   {
    delete( map_buffer );
    map_buffer = 0;
    map_loaded = false;
   }
  
  fread( &map_header, sizeof( map_file_header_type ), 1, infile );

  map_info.x_size = map_header.map_x_size;

  map_info.y_size = map_header.map_y_size;

  strcpy( (char *) map_info.name, map_header.map_name );
    
  map_buffer = new MapElementType [ map_header.map_data_size ];
  assert( map_buffer != 0 );
    
  fread( map_buffer, map_header.map_data_size, 1, infile );

  fclose( infile );    
  
  map_loaded = true;
  
*/  
  //*****************************************************************
  
  infile = fopen( file_path, "rb" );  
  assert( infile != 0 );  

  if ( map_loaded == true )
   {
    delete( map_buffer );
    map_buffer = 0;
    map_loaded = false;
   }
  
  fread( &map_info, sizeof( MAP_HEADER ), 1, infile );
    
  map_size = (map_info.x_size * map_info.y_size);

  map_buffer = new MapElementType [ map_size ];
  assert( map_buffer != 0 );
    
  fread( map_buffer, map_size, sizeof(MapElementType), infile );

  fclose( infile );    
  
  map_loaded = true;
  
 }
