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

#include "Gdatstct.hpp"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <memory>

#include "Util/Log.hpp"
#include "Util/Exception.hpp"
#include "Util/FileSystem.hpp"

//*****************************************************************************

tile_dbase::tile_dbase( void )
{
    dbase_loaded = false;

}
//*****************************************************************************

void tile_dbase::load_dbase( char *dbase_path )
{
    long  tile_buffer_size;

    std::auto_ptr<ReadFile> file (FileSystem::openRead(dbase_path));

    if(file->read( &header, sizeof( tile_dbase_header_type ), 1) != 1)
        throw Exception("Error while reading database %s", dbase_path);

    tile_buffer_size = header.tile_size * header.tile_count;

    tile_data = (unsigned char *) malloc( sizeof(unsigned char) * tile_buffer_size );

    if(file->read(tile_data, tile_buffer_size, 1) != 1)
        throw Exception("Error while reading database %s", dbase_path);

    dbase_loaded = true;
}

//*****************************************************************************

void tile_dbase::unload_dbase()
{
    if(!dbase_loaded)
        return;

    free(tile_data);
    tile_data = 0;
    dbase_loaded = false;
}
