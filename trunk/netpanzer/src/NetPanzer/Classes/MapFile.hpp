/*
Copyright (C) 2004 Hollis Blanchard <hollis@penguinppc.org>
 
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
#ifndef _MAPFILE_HPP
#define _MAPFILE_HPP

#include "Util/FileSystem.hpp"

class MapFile {
public:
    void load(ReadFile& file);

    unsigned char   netp_id_header[64];
    unsigned short  id;
    unsigned char   name[256];
    unsigned char   description[1024];
    unsigned short  x_size;
    unsigned short  y_size;
    char            tile_set[256];

    unsigned short  thumbnail_x_pix;
    unsigned short  thumbnail_y_pix;
};

#endif // ** _MAPFILE_HPP
