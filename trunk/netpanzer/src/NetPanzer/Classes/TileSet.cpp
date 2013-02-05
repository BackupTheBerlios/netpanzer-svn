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

#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#include "Util/FileSystem.hpp"
#include "Util/Exception.hpp"

TileSet::TileSet( )
{
    tile_info = 0;
    tile_data = 0;
    tile_set_loaded = false;
    tile_count = 0;
}

TileSet::~TileSet()
{
    delete[] tile_info;
    delete[] tile_data;
}

void TileSet::readTileDbHeader(filesystem::ReadFile& file,
        TILE_DBASE_HEADER *dbHeader)
{
    file.read(&dbHeader->netp_id_header, sizeof(dbHeader->netp_id_header), 1);
    dbHeader->version = file.readULE16();
    dbHeader->x_pix = file.readULE16();
    dbHeader->y_pix = file.readULE16();
    dbHeader->tile_count = file.readULE16();
    file.read(&dbHeader->palette, sizeof(dbHeader->palette), 1);
}

void TileSet::computeTileConsts( void )
{
    tile_size = tile_set_info.x_pix * tile_set_info.y_pix;
}

void TileSet::loadTileSetInfo( const char *file_path )
{
    filesystem::ReadFile file(file_path);

    delete[] tile_data;
    tile_data = 0;
    delete[] tile_info;
    tile_info = 0;
    tile_set_loaded = false;

    readTileDbHeader(file, &tile_set_info);

    tile_count = tile_set_info.tile_count;

    tile_set_loaded = true;

    computeTileConsts();
}

void TileSet::loadTileSet( const char *file_path )
{
    unsigned long  tile_buffer_size;

    try
    {
        filesystem::ReadFile file(file_path);

        delete[] tile_data;
        tile_data = 0;
        delete[] tile_info;
        tile_info = 0;
        tile_set_loaded = false;

        readTileDbHeader(file, &tile_set_info);

        tile_buffer_size = (tile_set_info.x_pix * tile_set_info.y_pix) * tile_set_info.tile_count;

        tile_info =  new TILE_HEADER [ tile_set_info.tile_count ];
        assert( tile_info != 0 );

        tile_data =  new unsigned char[tile_buffer_size];
        assert( tile_data != 0 );

        file.read(tile_info, tile_set_info.tile_count, sizeof(TILE_HEADER));
        file.read( tile_data, tile_buffer_size, 1);

        tile_set_loaded = true;
        
        computeTileConsts();
    }
    catch(std::exception& e)
    {
        throw Exception("Couldn't load tileset '%s': %s",
                         file_path, e.what());
    }
}
