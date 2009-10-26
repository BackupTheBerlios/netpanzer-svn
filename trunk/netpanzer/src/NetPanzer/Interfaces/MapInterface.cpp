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

#include "Interfaces/MapInterface.hpp"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "SDL_endian.h"

#include "Port/MapData.hpp"

WorldMap MapInterface::main_map;
SpawnList MapInterface::spawn_list;
char MapInterface::map_path[256];
MapInterface::MapListenerList MapInterface::listenerList;

bool MapInterface::loadMap(const char *file_path, bool load_tiles)
{
    char path[256];
    char tile_set_path[256];

    strcpy( map_path, file_path );

    strcpy( path, file_path );
    strcat( path, ".npm" );
    main_map.loadMapFile( path );

    strcpy( tile_set_path, "tilesets/" );
    strcat( tile_set_path, main_map.getAssocTileSet() );

    if ( load_tiles == true )
    {
        tile_set.loadTileSet(tile_set_path);
    }
    else
    {
        tile_set.loadTileSetInfo(tile_set_path);
    }

    finishMapLoad();
    return true;
}

void MapInterface::finishMapLoad( void )
{
    char path[256];

    MapListenerList::iterator i = listenerList.begin();
    while ( i != listenerList.end() )
    {
        (*i)->onMapLoadedEvent();
        i++;
    }
    
    strcpy( path, map_path );

    strcat( path, ".spn" );
    spawn_list.loadSpawnFile( path );
}

unsigned char MapInterface::getMovementValue( iXY map_loc )
{
    unsigned short tile_val;
    char move_val;

    if (      (map_loc.x >= 0) && ((size_t) map_loc.x < main_map.getWidth() )
              && (map_loc.y >= 0) && ((size_t) map_loc.y < main_map.getHeight() )
       ) {
        tile_val = main_map.getValue( (unsigned short) map_loc.x, (unsigned short) map_loc.y ) ;
        move_val = tile_set.getTileMovementValue( tile_val );

        switch( move_val ) {
        case 0 :
            return( 1 );

        case 1 :
            return( 1 );

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

const SDL_Color * MapInterface::getAverageColorPointXY( iXY &point_loc )
{
    WorldMap::MapElementType map_value =
        main_map.getValue(point_loc.x/32, point_loc.y/32);

    return( tile_set.getAverageTileColor( map_value ) );
}

const SDL_Color * MapInterface::getAverageColorMapXY( iXY &map_loc )
{
    unsigned short map_value;

    map_value = main_map.getValue( map_loc.x, map_loc.y );

    return( tile_set.getAverageTileColor( map_value ) );
}
