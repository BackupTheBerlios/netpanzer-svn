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
#ifndef _TILESET_HPP
#define _TILESET_HPP

#include <vector>
#include "2D/Surface.hpp"
#include "Types/iXY.hpp"
#include "SDL.h"
#include "Util/FileStream.hpp"

namespace filesystem { class ReadFile; }

typedef struct TileSetHeader_s
{
    unsigned int version;
    unsigned int x_pix;
    unsigned int y_pix;
    unsigned int tile_count;
    std::string image_file;
    std::string unused;
} TileSetHeader;

typedef struct TileAttributes_s
{
    Uint8 attrib;
    Uint8 move_value;
    SDL_Color avg_color;
} TileAttributes;

class TileSet
{
public:
    static const std::string& getName() { return *name; }

    static void load(const char * tsname);
    static void loadImages();

    static bool tileImagesLoaded() { return tiles != 0; }

    static void cleanUp();

    static Surface * getTile(unsigned long index) { return (*tiles)[index]; }

    static unsigned int getTileXsize() { return header->x_pix; }
    static unsigned int getTileYsize() { return header->y_pix; }
    static unsigned int getTileCount() { return header->tile_count; }

    static const SDL_Color* getAverageTileColor(unsigned long index)
    {
        return &(*tile_attributes)[ index ].avg_color;
    }

    static char getTileMovementValue( unsigned long index )
    {
        return (*tile_attributes)[ index ].move_value;
    }

private:
    TileSet();
    ~TileSet();
    static TileSetHeader*               header;
    static std::vector<TileAttributes>* tile_attributes;
    static std::vector<Surface*>*       tiles;
    static std::string*                 name;

    static void readTileSetHeader(IFileStream& file);
    static void loadTileAttributes(IFileStream& file);
    static void freeTiles();
};

#endif // ** _TILESET_HPP
