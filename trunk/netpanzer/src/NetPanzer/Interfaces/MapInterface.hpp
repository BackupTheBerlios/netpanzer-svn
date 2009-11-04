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

#include <list>

#include "TileInterface.hpp"
#include "Classes/WorldMap.hpp"
#include "Core/GlobalGameState.hpp"

struct SDL_Color;

class MapEventListener
{
public:
    virtual ~MapEventListener() {}
protected:
    virtual void onMapLoadedEvent() = 0;
private:
    friend class MapInterface;
};


class MapInterface : protected TileInterface
{
public:
    static bool loadMap(const char *file_path, bool load_tiles);

    static unsigned char getMovementValue( iXY map_loc );

    static void addMapEventListener(MapEventListener *lis)
    {
        listenerList.push_back(lis);
    }
    
    static void removeMapEventListener(MapEventListener *lis)
    {
        listenerList.remove(lis);
    }
    
    static void getMapPointSize(iXY *map_size)
    {
        map_size->x = global_game_state->world_map->getWidth() * global_game_state->tile_set->getTileXsize();
        map_size->y = global_game_state->world_map->getHeight() * global_game_state->tile_set->getTileYsize();
    }

    static iXY getSize()
    {
        return iXY(global_game_state->world_map->getWidth(), global_game_state->world_map->getHeight());
    }

    static WorldMap::MapElementType MapValue(size_t x, size_t y)
    {
        return global_game_state->world_map->getValue(x, y);
    }

    static void mapXYtoPointXY(unsigned short map_x, unsigned short map_y,
                               size_t *point_x, size_t *point_y )
    {
        *point_x = (map_x * TILE_WIDTH) + (TILE_WIDTH / 2);
        *point_y = (map_y * TILE_HEIGHT) + (TILE_HEIGHT / 2);
    }

    static void mapXYtoPointXY(const iXY& map_loc, iXY *loc)
    {
        loc->x = (map_loc.x * TILE_WIDTH) + (TILE_WIDTH / 2);
        loc->y = (map_loc.y * TILE_HEIGHT) + (TILE_HEIGHT / 2);
    }

    static void pointXYtoMapXY(size_t point_x, size_t point_y,
            unsigned short *map_x, unsigned short *map_y )
    {
        *map_x = point_x / TILE_WIDTH;
        *map_y = point_y / TILE_HEIGHT;
    }

    static void pointXYtoMapXY(const iXY& point, iXY *map_loc)
    {
        map_loc->x = point.x / TILE_WIDTH;
        map_loc->y = point.y / TILE_HEIGHT;
    }

    static size_t mapXYtoOffset(size_t map_x, size_t map_y)
    {
        return (map_y * global_game_state->world_map->getWidth()) + map_x;
    }

    static size_t mapXYtoOffset(const iXY& map_loc)
    {
        return mapXYtoOffset(map_loc.x, map_loc.y);
    }

    static void offsetToMapXY(size_t offset, iXY *map_loc)
    {
        map_loc->y = offset/global_game_state->world_map->getWidth();
        map_loc->x = offset - (map_loc->y * global_game_state->world_map->getWidth());
    }

protected:
    typedef std::list<MapEventListener *> MapListenerList;
    static MapListenerList listenerList;

    static char map_path[256];
    static const int TILE_WIDTH = 32;
    static const int TILE_HEIGHT = 32;

    static void finishMapLoad();
};

#endif
