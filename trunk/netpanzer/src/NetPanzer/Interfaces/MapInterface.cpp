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
#include "Core/GlobalGameState.hpp"
#include "Classes/SpawnList.hpp"
#include "Util/Log.hpp"
#include <cstdlib>
#include <cstring>
#include <cstdio>

MapInterface::MapListenerList MapInterface::listenerList;
int* MapInterface::map_data = 0;
unsigned int MapInterface::width = 0;
unsigned int MapInterface::height = 0;
char MapInterface::tileset_name[256];
SpawnList* MapInterface::spawn_list = 0;

bool MapInterface::load(const char *file_path)
{
    char path[256];

    strcpy( path, file_path );
    strcat( path, ".npm" );

    std::auto_ptr<filesystem::ReadFile> file(filesystem::openRead(path));

    // magick is sizes of: netp_id_header + id + name + description
    file->seek(64 + 2 + 256 + 1024);
    width = file->readULE16();
    height = file->readULE16();
    file->read(tileset_name, sizeof(tileset_name),1);

    file->readULE16(); // skip thumbnail_width
    file->readULE16(); // skip thumbnail_height

    if ( map_data )
    {
        delete[] map_data;
    }

    int total_size = width*height;
    map_data = new int[total_size];
    int* map_ptr = map_data;

    for(; total_size != 0; --total_size)
    {
        *map_ptr = file->readULE16();
        ++map_ptr;
    }

    TileSet::load(tileset_name);

    strcpy( path, file_path );
    strcat( path, ".spn" );

    if ( spawn_list )
    {
        delete spawn_list;
    }

    spawn_list = new SpawnList();
    spawn_list->loadSpawnFile(path);

    notifyMapLoad();
    
    return true;
}

void MapInterface::cleanUp()
{
    if ( map_data )
    {
        delete[] map_data;
    }

    if ( spawn_list )
    {
        delete spawn_list;
    }

    map_data = 0;
    width = 0;
    height = 0;
    tileset_name[0] = 0;
}

void MapInterface::notifyMapLoad()
{
    MapListenerList::iterator i = listenerList.begin();
    while ( i != listenerList.end() )
    {
        (*i)->onMapLoadedEvent();
        i++;
    }
}

unsigned char MapInterface::getMovementValue( iXY map_loc )
{
    unsigned short tile_val;
    char move_val;

    if (      (map_loc.x >= 0) && ((size_t) map_loc.x < width )
              && (map_loc.y >= 0) && ((size_t) map_loc.y <height )
       ) {
        tile_val = getValue( (unsigned short) map_loc.x, (unsigned short) map_loc.y ) ;
        move_val = TileSet::getTileMovementValue( tile_val );

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

unsigned char
MapInterface::getWorldPixMovementValue(int worldX, int worldY)
{
    int tileX = worldX / TileSet::getTileXsize();
    int tileY = worldY / TileSet::getTileYsize();

    if(tileX >= (int) width || tileY >= (int) height || tileX < 0 || tileY < 0 )
    {
        LOGGER.warning("query for worldpixmovement outside map (%d,%d)",
                worldX, worldY);
        return 0xff;
    }
    int tileValue = getValue(tileX, tileY);

    return TileSet::getTileMovementValue(tileValue);
}
