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
#include "MapInterface.hpp"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "MapData.hpp"

WorldMap MapInterface::main_map;
SpawnList MapInterface::spawn_list;
WadMapTable MapInterface::wad_mapping_table;
Surface MapInterface::mini_map_surface;
char MapInterface::map_path[256];

bool MapInterface::startMapLoad( const char *file_path, bool load_tiles, unsigned long partitions )
{
    char path[256];
    char tile_set_path[256];

    strcpy( map_path, file_path );

    strcpy( path, file_path );
    strcat( path, ".npm" );
    main_map.loadMapFile( path );

    strcpy( tile_set_path, "wads/" );
    strcat( tile_set_path, main_map.getAssocTileSet() );

    tile_set.loadTileSetInfo( tile_set_path );
    generateMappingTable();
    main_map.reMap( wad_mapping_table );

    if ( load_tiles == true ) {
        if ( tile_set.startPartitionTileSetLoad( tile_set_path, wad_mapping_table, partitions ) == false ) {
            finishMapLoad();
            return( false );
        }

    } else {
        tile_set.loadTileSetInfo( tile_set_path, wad_mapping_table );
        finishMapLoad();
        return( false );
    }

    return( true );
}

bool MapInterface::loadMap( int *percent_complete )
{
    if( tile_set.partitionTileSetLoad( wad_mapping_table, percent_complete ) == false ) {
        finishMapLoad();
        return( false );
    } else {
        return( true );
    }
}


void MapInterface::finishMapLoad( void )
{
    char path[256];

    wad_mapping_table.deallocate();

    buildMiniMapSurface();

    strcpy( path, map_path );

    strcat( path, ".spn" );
    spawn_list.loadSpawnFile( path );
}

/*
void MapInterface::LoadMap( const char *file_path, bool load_tiles )
 {
  char path[80];
  
  strcpy( path, file_path );
  strcat( path, ".npm" );
  
  tile_set.loadTileSetInfo( "./wads/wad.tls" );
  generateMappingTable();
  main_map.reMap( wad_mapping_table );
  
  if ( load_tiles == true )
   {
    tile_set.loadTileSet( "./wads/wad.tls", wad_mapping_table ); 
   }
  else
   {
    tile_set.loadTileSetInfo( "./wads/wad.tls", wad_mapping_table ); 
   }
 
  wad_mapping_table.deallocate();
  
  buildMiniMapSurface();
 
  strcpy( path, file_path );
  strcat( path, ".spn" );
  spawn_list.loadSpawnFile( path );  
 }
*/

void MapInterface::generateMappingTable()
{
    unsigned short tile_count;
    unsigned long  map_size;
    MapElementType *map_buffer;
    unsigned long  map_index;

    tile_count = tile_set.getTileCount();
    map_size = main_map.getMapSize();

    wad_mapping_table.initialize( tile_count );
    wad_mapping_table.resetMappingTable();

    main_map.getRawMapBuffer( &map_buffer );

    // ** Find all of the used tiles **
    unsigned short tile_index;

    for ( map_index = 0; map_index < map_size; map_index++ ) {
        tile_index = map_buffer[ map_index ];

        wad_mapping_table[ tile_index ].is_used = true;
    } // ** for

    // ** Remap the used tiles **
    unsigned short mapping_index = 0;

    for ( tile_index = 0; tile_index < tile_count; tile_index++ ) {
        if ( wad_mapping_table[ tile_index ].is_used == true ) {
            wad_mapping_table[ tile_index ].remap_index = mapping_index;
            mapping_index++;
        }
    }

    wad_mapping_table.used_tile_count = mapping_index;

}

void MapInterface::buildMiniMapSurface()
{
    unsigned long map_x_size, x_index;
    unsigned long map_y_size, y_index;
    MapElementType map_value;
    unsigned char  avg_color;

    map_x_size = main_map.getXsize();
    map_y_size = main_map.getYsize();
    mini_map_surface.create( map_x_size, map_y_size );

    for( y_index = 0; y_index < map_y_size; y_index++ ) {
        for ( x_index = 0; x_index < map_x_size; x_index++ ) {
            map_value = main_map.mapValue( x_index, y_index );
            avg_color = tile_set.getAverageTileColor( map_value );
            mini_map_surface._putPixel( x_index, y_index, (PIX) avg_color );
        } // ** for x_index
    } // ** for y_index
}

unsigned char MapInterface::getMovementValue( iXY map_loc )
{
    unsigned short tile_val;
    char move_val;

    if (      (map_loc.x >= 0) && (map_loc.x < main_map.getXsize() )
              && (map_loc.y >= 0) && (map_loc.y < main_map.getYsize() )
       ) {
        tile_val = main_map.mapValue( (unsigned short) map_loc.x, (unsigned short) map_loc.y ) ;
        move_val = tile_set.getTileMovementValue( tile_val );

        switch( move_val ) {
        case 0 :
            return( 1 );

        case 1 :
            return( 5 );

        case 2 :
            return( 25 );

        case 3 :
            return( 50 );

        case 4 :
            return( 0xFF );

        case 5 :
            return( 0xFF );
        } // ** switch

        return( 1 );
    } else {
        return( 0xFF );
    }

}

unsigned char MapInterface::getAverageColorPointXY( iXY &point_loc )
{
    unsigned long offset;
    unsigned short map_value;

    main_map.mapXYtoOffset( (unsigned short )  point_loc.x  / 32, (unsigned short )  point_loc.y  / 32, &offset );

    map_value = main_map.mapValue( offset );

    return( tile_set.getAverageTileColor( map_value ) );
}

unsigned char MapInterface::getAverageColorMapXY( iXY &map_loc )
{
    unsigned short map_value;

    map_value = main_map.mapValue( map_loc.x, map_loc.y );

    return( tile_set.getAverageTileColor( map_value ) );
}
