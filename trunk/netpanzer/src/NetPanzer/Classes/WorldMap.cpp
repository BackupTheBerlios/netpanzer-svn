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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory>

#include "WorldMap.hpp"
#include "MapData.hpp"
#include "FileSystem.hpp"
#include "Exception.hpp"

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

    for ( map_index = 0; map_index < map_size; map_index++ ) {
        tile_value = map_buffer[ map_index ];
        map_buffer[ map_index  ] = mapping_table[ tile_value ].remap_index;
    }

}


void WorldMap::loadMapFile( const char *file_path )
{
    unsigned long map_size;

    std::auto_ptr<ReadFile> file (FileSystem::openRead(file_path));

    if ( map_loaded == true ) {
        delete[] map_buffer;
        map_buffer = 0;
        map_loaded = false;
    }

    if(file->read(&map_info, sizeof( MAP_HEADER ), 1) != 1)
        throw Exception("Couldn't load mapfile '%s' (while reading mapinfo)",
                        file_path);

    map_size = (map_info.x_size * map_info.y_size);

    map_buffer = new MapElementType [ map_size ];
    assert( map_buffer != 0 );

    if(file->read(map_buffer, map_size, sizeof(MapElementType)) != sizeof(MapElementType))
        throw Exception("Couldn't load mapfile '%s' (while reading elements)",
                        file_path);

    map_loaded = true;
}
