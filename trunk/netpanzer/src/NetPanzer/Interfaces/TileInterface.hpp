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
#ifndef _TILEINTERFACE_HPP
#define _TILEINTERFACE_HPP

//#include "2D/Surface.hpp"
#include "TileSet.hpp"
#include "Types/iXY.hpp"
//#include "2D/Palette.hpp"

class TileInterface
{
protected:
    static TileSet tile_set;

public:
    static iXY getTileSize( void );

    static Uint8 getWorldPixColor(int worldX, int worldY);
    static long getWorldPixMovementValue(int worldX, int worldY);
};

#endif // ** _TILEINTERFACE_HPP
