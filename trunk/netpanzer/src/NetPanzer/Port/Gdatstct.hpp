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
#ifndef _GDATSTCT_HPP
#define _GDATSTCT_HPP

//***************************************************************************

struct tile_dbase_header_type
{
    char  pyro_id_header[64]; // file text identification;
    short pyro_tile_set_id;   // internal tile set identification;
    short tile_x_size;        // x_size of tile in pixels
    short tile_y_size;        // y_size of tile in pixels
    short tile_size;          // tile size in bytes
    short tile_count;         // tile_count;
    unsigned char palette[768];       // pallette for tileset;
};

class tile_dbase
{
public:
    short dbase_loaded;
    tile_dbase_header_type header;
    unsigned char *tile_data;

    tile_dbase();
    void load_dbase( char *dbase_path );
    void unload_dbase();
};

#endif
