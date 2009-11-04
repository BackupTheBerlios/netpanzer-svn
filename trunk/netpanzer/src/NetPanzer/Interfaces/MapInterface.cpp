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
#include <cstdlib>
#include <cstring>
#include <cstdio>


char MapInterface::map_path[256];
MapInterface::MapListenerList MapInterface::listenerList;

bool MapInterface::loadMap(const char *file_path, bool load_tiles)
{
    char path[256];
    char tile_set_path[256];

    strcpy( map_path, file_path );

    strcpy( path, file_path );
    strcat( path, ".npm" );

    WorldMap * map = new WorldMap();
    map->loadMapFile( path );
    if ( global_game_state->world_map )
    {
        delete global_game_state->world_map;
    }
    global_game_state->world_map = map;

    strcpy( tile_set_path, "tilesets/" );
    strcat( tile_set_path, global_game_state->world_map->getAssocTileSet() );

    TileSet * ts = new TileSet();
    if ( load_tiles == true )
    {
        ts->loadTileSet(tile_set_path);
    }
    else
    {
        ts->loadTileSetInfo(tile_set_path);
    }

    if ( global_game_state->tile_set )
    {
        delete global_game_state->tile_set;
    }
    global_game_state->tile_set = ts;

    finishMapLoad();
    return true;
}

void MapInterface::finishMapLoad( void )
{
    char path[256];

    strcpy( path, map_path );
    strcat( path, ".spn" );

    SpawnList * sl = new SpawnList();
    sl->loadSpawnFile(path);
    if ( global_game_state->spawn_list )
    {
        delete global_game_state->spawn_list;
    }
    global_game_state->spawn_list = sl;

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

    if (      (map_loc.x >= 0) && ((size_t) map_loc.x < global_game_state->world_map->getWidth() )
              && (map_loc.y >= 0) && ((size_t) map_loc.y < global_game_state->world_map->getHeight() )
       ) {
        tile_val = global_game_state->world_map->getValue( (unsigned short) map_loc.x, (unsigned short) map_loc.y ) ;
        move_val = global_game_state->tile_set->getTileMovementValue( tile_val );

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
