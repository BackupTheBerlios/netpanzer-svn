/*
Copyright (C) 2003 Matthias Braun
 
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

#define FILEMAGIC   "NPMP"
#define MAPFORMAT   (0001)

struct MapHeader
{
    char filemagic[4];
    uint32_t formatversion;
    uint32_t width;
    uint32_t height;
    uint16_t tilesetmapcount;
};

struct MapTileSetMapping
{
    char tilesetname[32];
    uint16_t tilenamecount;
};

struct MapTileSetMappingEntry
{
    char tilename[32];
};

typedef uint16_t maptile;

Map::Map()
{
};
