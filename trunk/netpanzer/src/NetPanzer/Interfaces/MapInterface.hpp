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

#include "TileInterface.hpp"
#include "WorldMap.hpp"
#include "SpawnList.hpp"
#include "WadMapTable.hpp"

#include "2D/Surface.hpp"

class MapLoadCallback
{
public:
    virtual void MapLoadProgress(float percent);
};

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
    static void getMapPointSize(iXY *map_size)
    {
        map_size->x = main_map.getWidth() * tile_set.getTileXsize();
        map_size->y = main_map.getHeight() * tile_set.getTileYsize();
    }

    static iXY getSize()
    {
        return iXY(main_map.getWidth(), main_map.getHeight());
    }

    static size_t getWidth()
    {
        return main_map.getWidth();
    }

    static size_t getHeight()
    {
        return main_map.getHeight();
    }

    static WorldMap::MapElementType MapValue(size_t x, size_t y)
    {
        return main_map.getValue(x, y);
    }

    static WorldMap::MapElementType MapValue(size_t offset)
    {
        return main_map.getValue(offset);
    }

    static void offsetToPointXY(size_t offset, size_t* point_x, size_t *point_y )
    {
        *point_y = ( offset  /  main_map.getWidth() );

        *point_x = ( offset - ( (*point_y) * main_map.getWidth() ) ) * 32
                   + (32 / 2);

        *point_y = (*point_y) * 32 + (32 / 2);
    }

    static void offsetToMapXY(size_t offset, size_t& x, size_t& y)
    {
        y = offset / main_map.getWidth();
        x = offset - y * main_map.getWidth();
    }

    static void offsetToMapXY(size_t offset, iXY *map_loc)
    {
        size_t map_x, map_y;
        offsetToMapXY(offset, map_x, map_y);
        map_loc->x = map_x;
        map_loc->y = map_y;
    }

    static void mapXYtoPointXY( unsigned short map_x, unsigned short map_y,
                                size_t *point_x, size_t *point_y )
    {
        *point_x = (map_x * 32) + (32 / 2);
        *point_y = (map_y * 32) + (32 / 2);
    }

    static void mapXYtoPointXY( iXY map_loc, iXY *loc )
    {
        loc->x = (map_loc.x * 32) + (32 / 2);
        loc->y = (map_loc.y * 32) + (32 / 2);
    }

    static void pointXYtoMapXY( size_t point_x,
                                       size_t point_y, unsigned short *map_x, unsigned short *map_y )
    {
        *map_x = (unsigned short )  point_x  / 32;
        *map_y = (unsigned short )  point_y /  32;
    }

    static void pointXYtoMapXY( iXY point, iXY *map_loc )
    {
        map_loc->x = (unsigned short )  point.x  / 32;
        map_loc->y = (unsigned short )  point.y  / 32;
    }

    static size_t mapXYtoOffset(size_t map_x, size_t map_y)
    {
        return map_y * main_map.getWidth() + map_x;
    }

    static size_t mapXYtoOffset(iXY &map_loc)
    {
        return map_loc.y * main_map.getWidth() + map_loc.x;
    }

    static void markLocHack( const iXY &loc )
    {
        main_map.setMapValue(loc.x, loc.y, 27);
    }

    static void unmarkLocHack( const iXY &loc )
    {
        main_map.setMapValue(loc.x, loc.y, 28);
    }

    static void normalizePointXY( size_t point_x, size_t point_y, size_t *norm_x, size_t *norm_y )
    {
        unsigned short map_x, map_y;

        pointXYtoMapXY( point_x, point_y, &map_x, &map_y );
        mapXYtoPointXY( map_x, map_y, norm_x, norm_y );
    }

    static WorldMap * getMap( void )
    {
        return( &main_map );
    }

protected:
    static void finishMapLoad();

public:
    static bool startMapLoad(const char *file_path, bool load_tiles, size_t partitions);
    static bool loadMap( int *percent_complete );

    static bool isMapLoaded()
    {
        return( main_map.isMapLoaded() );
    }

    static Surface * getMiniMapSurface()
    {
        return ( &mini_map_surface );
    }

    static unsigned char getMovementValue( iXY map_loc );

    static unsigned char getAverageColorPointXY( iXY &point_loc );

    static unsigned char getAverageColorMapXY( iXY &map_loc );

    static iXY getFreeSpawnPoint()
    {
        return spawn_list.getFreeSpawnPoint();
    }

    static SpawnList* getSpawnList()
    {
        return &spawn_list;
    }
};

#endif
