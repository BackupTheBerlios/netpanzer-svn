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
#include "TileInterface.hpp"
#include "MapInterface.hpp"


TileSet TileInterface::tile_set;

void TileInterface::loadTileSet( const char *file_path )
 {
  tile_set.loadTileSet( file_path );
 } 

PointXYi TileInterface::getTileSize( void )
 {
  return( PointXYi( tile_set.getTileXsize(), tile_set.getTileYsize() ) ); 
 }

PIX TileInterface::getWorldPixColor(int worldX, int worldY)
{
	int tileX = worldX / tile_set.getTileXsize();
	int tileY = worldY / tile_set.getTileYsize();

	int subPixX = worldX % tile_set.getTileXsize();
	int subPixY = worldY % tile_set.getTileYsize();

	int tileValue = MapInterface::MapValue(tileX, tileY);

	return tile_set.getTilePixel(tileValue, subPixX, subPixY);
}

long TileInterface::getWorldPixMovementValue(int worldX, int worldY)
{
	int tileX = worldX / tile_set.getTileXsize();
	int tileY = worldY / tile_set.getTileYsize();

	//int subPixX = worldX % tile_set.getTileXsize();
	//int subPixY = worldY % tile_set.getTileYsize();

	int tileValue = MapInterface::MapValue(tileX, tileY);

	return tile_set.getTileMovementValue(tileValue);
}
