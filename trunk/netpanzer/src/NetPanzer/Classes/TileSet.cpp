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


#include "Classes/TileSet.hpp"

#include <iostream>
#include <istream>

#include <memory>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cassert>

#include "Util/FileStream.hpp"
#include "Util/FileSystem.hpp"
#include "Util/Exception.hpp"
#include "2D/Surface.hpp"
#include "Util/Log.hpp"

TileSet::TileSet( )
{
    tile_set_loaded = false;
}

TileSet::~TileSet()
{
    freeTiles();
}

void
TileSet::freeTiles()
{
    for ( std::vector<Surface*>::iterator i = tiles.begin();
                    i != tiles.end(); ++i )
    {
        delete(*i);
    }
    tiles.resize(0);
}

void TileSet::readTileSetHeader(IFileStream & file)
{
    std::getline(file, header.image_file, ':');
    std::getline(file, header.unused, ':');
    file >> header.version;
    file.ignore(1);
    file >> header.x_pix;
    file.ignore(1);
    file >> header.y_pix;
    file.ignore(1);
    file >> header.tile_count;

    LOGGER.info("TSinfo %s (%s) version %d with %d tiles of %dx%d",
                header.image_file.c_str(), header.unused.c_str(), header.version,
                header.tile_count, header.x_pix, header.y_pix);
}

void TileSet::computeTileConsts( void )
{
    tile_size = header.x_pix * header.y_pix;
}

void TileSet::loadTileSetInfo( const char *file_path )
{
    IFileStream file(file_path);

    freeTiles();
    tile_set_loaded = false;

    readTileSetHeader(file);
    loadTileAttributes(file);

    tile_set_loaded = true;

    computeTileConsts();
}

void TileSet::loadTileSet( const char *file_path )
{
    try {
        IFileStream file(file_path);

        freeTiles();
        tile_set_loaded = false;

        readTileSetHeader(file);
        loadTileAttributes(file);

        Surface big;

        std::string iname("tilesets/");
        iname += header.image_file;

        big.loadPNG(iname.c_str());
        SDL_Rect r = { 0, 0, header.x_pix, header.y_pix };

        tiles.resize(header.tile_count);
        for ( unsigned int n = 0; n < tiles.size(); ++n )
        {
                tiles[n] = new Surface(header.x_pix, header.y_pix, 1);
                big.bltRect(&r, *tiles[n], 0, 0);
                tiles[n]->optimize();
                r.x += header.x_pix;
                if ( (unsigned int)r.x >= big.getWidth() )
                {
                    r.x = 0;
                    r.y += header.y_pix;
                }
        }

        tile_set_loaded = true;
	
	computeTileConsts();
    } catch(std::exception& e) {
	throw Exception("Couldn't load tileset '%s': %s",
		file_path, e.what());
    }
}

void
TileSet::loadTileAttributes(IFileStream& file)
{
    int number;
    tile_attributes.resize(header.tile_count);

    std::vector<TileAttributes>::iterator i;
    for ( i = tile_attributes.begin(); i != tile_attributes.end(); ++i )
    {
        file >> number;
        file.ignore(1);
        file >> number;
        i->attrib = number;
        file.ignore(1);
        file >> number;
        i->move_value = number;
        file.ignore(1);
        file >> number;
        i->avg_color.r = number;
        file.ignore(1);
        file >> number;
        i->avg_color.g = number;
        file.ignore(1);
        file >> number;
        i->avg_color.b = number;
    }
}
