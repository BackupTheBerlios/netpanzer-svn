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
#ifndef _MAPFILESTRUCT_HPP
#define _MAPFILESTRUCT_HPP

/*
.map structure
Header
Map data	-> unsigned short
Thumbnail in raw format
*/

class MAP_HEADER
{
  public:
	unsigned char	netp_id_header[64];
	unsigned short	id;
	unsigned char	name[256];
	unsigned char	description[1024];
	unsigned short	x_size;
	unsigned short	y_size;
    char            tile_set[256];

	unsigned short	thumbnail_x_pix;
	unsigned short	thumbnail_y_pix;

}; // end MAP_HEADER

#endif
