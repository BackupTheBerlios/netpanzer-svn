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
#include "StaticsInterface.hpp"

PArrayGrowable StaticsInterface::static_sprite_list;

void StaticsInterface::initialize( void )
 {
  static_sprite_list.initialize( 200, 25, 0xFFFFFFFF );
 }


void StaticsInterface::loadStatics( const char *file_path )
 {
  Sprite *sprite;
  FILE *input_file;
  unsigned short type;
  long  x_loc, y_loc;

  unsigned long static_count = 0;

  input_file = fopen ( file_path, "rt" );
  assert( input_file != 0 );

  while ( !feof( input_file ) )
   {
    fscanf( input_file,  "%d %d %d", &type, &x_loc, &y_loc );
	
	sprite->setAttrib( PointXYi( x_loc, y_loc ), 0 );

	static_sprite_list.add( sprite, static_count );
	static_count++;
   }

   static_sprite_list.resize( static_count );

 }

void StaticsInterface::offloadStatics( SpriteSorter &sorter )
 {
  Sprite *sprite;
  unsigned long static_count, static_index;

  static_count = static_sprite_list.getSize();
 
  for( static_index = 0; static_index < static_count; static_index++ )
   {
    sprite = (Sprite *) static_sprite_list[ static_index ];
	
    sorter.addSprite( sprite );
   } 
 }
