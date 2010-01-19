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

#include <string>
#include <vector>
#include "SDL.h"

namespace filesystem { class ReadFile; }

typedef struct MapHeader_s
{
    char netp_id_header[64];
    Uint16 id;
    char name[256];
    char description[1024];
    Uint16 width;
    Uint16 height;
    char tile_set[256];

    Uint16 thumbnail_width;
    Uint16 thumbnail_height;
} MapHeader;

class WorldMap
{
public:
    void dumpInfo();
    typedef int MapElementType;
    typedef std::vector<MapElementType> MapData;
    
    WorldMap();
    ~WorldMap();

    void loadMapFile(const std::string& filename);
    void loadHeader(filesystem::ReadFile &file);
    void loadTiles(filesystem::ReadFile &file);

    bool isMapLoaded() const            { return loaded; }

    std::string getName() const         { return std::string(header.name); }
    const char * getDescription() const { return header.description; }
    size_t getWidth() const             { return header.width; }
    size_t getHeight() const            { return header.height; }
    const char* getAssocTileSet() const { return header.tile_set; }
    Uint16 getThumbnailWidth() const    { return header.thumbnail_width; }
    Uint16 getThumbnailHeight() const   { return header.thumbnail_height; }

    size_t getSize() const { return getWidth() * getHeight(); }

    MapElementType getValue(size_t x, size_t y) const
    {
        assert(x < getWidth());
        assert(y < getHeight());
        return data[y*getWidth() + x];
    }

private:
    bool      loaded;
    MapHeader header;
    MapData   data;
};

#endif // ** _WORLDMAP_HPP
