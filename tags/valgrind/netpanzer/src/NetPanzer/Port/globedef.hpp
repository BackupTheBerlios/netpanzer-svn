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
#ifndef _GLOBEDEF_HPP
#define _GLOBEDEF_HPP

#define _TILE_SIZE                   32
#define _TILE_BYTE_SIZE            1024
#define _NEXT_TILE_ROW_OFFSET    0x6580   // _DBUFFER_WIDTH * (TILE_SIZE - 1)
                                          // + X_OFFSET_TO_VIEW
#define _TILE_SIZE_INDEX_SHIFT       10   // = 4096 pixels or bytes

#define _MAX_PLAYERS 16


#endif
