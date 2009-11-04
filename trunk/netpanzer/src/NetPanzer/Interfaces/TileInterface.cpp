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

#include "TileInterface.hpp"
#include "Interfaces/MapInterface.hpp"
#include "Util/Log.hpp"

iXY TileInterface::getTileSize( void )
{
    return( iXY( global_game_state->tile_set->getTileXsize(), global_game_state->tile_set->getTileYsize() ) );
}

long TileInterface::getWorldPixMovementValue(int worldX, int worldY)
{
    int tileX = worldX / global_game_state->tile_set->getTileXsize();
    int tileY = worldY / global_game_state->tile_set->getTileYsize();

    if(tileX >= (int) global_game_state->world_map->getWidth()
            || tileY >= (int) global_game_state->world_map->getHeight()
            || tileX < 0 || tileY < 0 ) 
    {
        LOGGER.warning("query for worldpixmovement outside map (%d,%d)",
                worldX, worldY);
        return 0xffff;
    }                                                                               
    int tileValue = MapInterface::MapValue(tileX, tileY);

    return global_game_state->tile_set->getTileMovementValue(tileValue);
}
