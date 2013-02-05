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

#include "Port/MapData.hpp"
#include "Resources/ResourceManager.hpp"

const MapFile * MapInterface::main_map = 0;
SpawnList MapInterface::spawn_list;
char MapInterface::map_path[256];
MapInterface::MapListenerList MapInterface::listenerList;

bool MapInterface::loadMap( const char *file_path, bool load_tiles )
{
    // XXX @todo release the map
//    if ( main_map )
//    {
//        ResourceManager::releaseMap(main_map);
//    }
    
    char tile_set_path[256];

    strcpy( map_path, "/maps/");
    strcat( map_path, file_path );

    main_map = ResourceManager::getMap(file_path, ResourceManager::MAP_DATA);
    
    strcpy( tile_set_path, "wads/" );
    strcat( tile_set_path, main_map->tile_set );

    tile_set.loadTileSetInfo( tile_set_path );

    if ( load_tiles == true )
    {
        tile_set.loadTileSet(tile_set_path);
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


unsigned char MapInterface::getMovementValue( const iXY& map_loc )
{
    MapFile::ElementType tile_val = main_map->getValue( map_loc.x, map_loc.y );
    if ( tile_val != MapFile::InvalidElement )
    {
        switch( tile_set.getTileMovementValue( tile_val ) )
        {
            case 0 : return 1;
            case 1 : return 1;
            case 2 : return 25;
            case 3 : return 50;
            case 4 : return 0xff;
            case 5 : return 0xff;
            default: return 1;
        }
    }
    
    return( 0xff );
}

unsigned char MapInterface::getAverageColorMapXY( const iXY& map_loc )
{
    MapFile::ElementType map_value = main_map->getValue( map_loc.x, map_loc.y );
    
    return (map_value != MapFile::InvalidElement) ? tile_set.getAverageTileColor( map_value ) : 0;
}
