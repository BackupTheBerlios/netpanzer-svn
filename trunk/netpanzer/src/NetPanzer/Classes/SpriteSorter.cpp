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
#include "SpriteSorter.hpp"

SpriteSorter SPRITE_SORTER;


int sprite_key( const void *elem1, const void *elem2 )
 {

  // !KLUDGE! for stupid Micro$oft.......
  //
  // Their stupid qsort implementation compares the same element
  // against itself.  We must safeguard against this and return
  // "same" in this case.  (And only in this case.....)

  if (elem1 == elem2) return 0;

  Sprite *sprite1 = *(Sprite **) (elem1);
  Sprite *sprite2 = *(Sprite **) (elem2);

  long y1 = sprite1->world_pos.y;
  long y2 = sprite2->world_pos.y;

  // @FD 06/25/98 - Rewrote these tests.  I think there was a bug on
  // with (y1 > y1) test.  Also, for sprites that tie, I will impose
  // a second comparison to break the tie, so that our comparison
  // function is always consistent.  Whoever the bonehead at Microsoft
  // was who wrote their qsort not only compares the same element
  // against itself, but they also coded it in such a way that it
  // can get stuck in a loop and never terminate (or even crash) if
  // the comparison function is not consistent.  Morons.
  
  //if( y1 > y1 )
  // return( 1 );
  //else
  // if( y1 < y2 )
  //  return( -1 );

  if (y1 > y2) return +1;
  if (y1 < y2) return -1;

  // Tie based on the real comparison basis - impose an arbitrary 
  // but consistent sort order to break the tie.
  
  if (sprite1 > sprite2) return +1;
  if (sprite1 < sprite2) return -1;

  // We should never get here, unless the same sprite is
  // inserted in the list twice
  FUBAR("sprite_key called to compare the same sprite against itself!");
  return 0;
}


SpriteSorter::SpriteSorter( )
 {
  initialize();
 }

void SpriteSorter::initialize( )
 {
  unsigned long i;

  for ( i = 0; i < 3; i++ )
   {
    sprite_lists[ i ].initialize( 50, 50, 0xFFFFFFFF );    
	list_counts[ i ] = 0;
   }

  sprite_lists[ 3 ].initialize( 500, 100, 0xFFFFFFFF );
  sprite_lists[ 4 ].initialize( 500, 100, 0xFFFFFFFF );
  sprite_lists[ 5 ].initialize( 500, 100, 0xFFFFFFFF );

  for ( i = 6; i < _MAX_HEIGHT_LEVELS; i++ )
   {
    sprite_lists[ i ].initialize( 50, 50, 0xFFFFFFFF );
    list_counts[ i ] = 0;
   }

  for ( i = 0; i < _MAX_HEIGHT_LEVELS; i++ )
   {
    max_sprite_stats[ i ] = 0;
   }

 }

void SpriteSorter::reset( void )
 {
  unsigned long i;

  for ( i = 0; i < _MAX_HEIGHT_LEVELS; i++ )
   {
    list_counts[ i ] = 0;
   }

 }

void SpriteSorter::reset( Recti &world_win )
 {
  world_window = world_win;
  reset();
 }

void SpriteSorter::addSprite( Sprite *sprite )
 {
  unsigned char height;
  
  if ( sprite->isVisible( world_window ) )
   {
    height = sprite->getHeight();
	
	assert( height < _MAX_HEIGHT_LEVELS );
	 
	sprite_lists[ height ].add( sprite, list_counts[ height ] );
    list_counts[ height ]++;
   }
 
 }

 
void SpriteSorter::forceAddSprite( Sprite *sprite )
 {
  unsigned char height;

  height = sprite->getHeight();
	
  assert( height < _MAX_HEIGHT_LEVELS );
	 
  sprite_lists[ height ].add( sprite, list_counts[ height ] );
  list_counts[ height ]++;
 }


void SpriteSorter::sortLists( void ) 
 {
  unsigned long i;

  for ( i = 0; i < _MAX_HEIGHT_LEVELS; i++ )
   {
    sprite_lists[ i ].sort( list_counts[ i ], sprite_key );   
   }
 
 }


void SpriteSorter::blitLists( Surface *render_surf )
 {
  Sprite *sprite;
  unsigned long list_index, sprite_index;

  //sortLists();

  for ( list_index = 0; list_index < _MAX_HEIGHT_LEVELS; list_index++ )
   {
    for ( sprite_index = 0; sprite_index < list_counts[ list_index]; sprite_index++ )
	 {
	  sprite = (Sprite *) sprite_lists[ list_index ][ sprite_index ];
	  sprite->blitAll( render_surf, world_window );   
     }
   
    if ( list_counts[ list_index ] > max_sprite_stats[ list_index ] )
	 {
	  max_sprite_stats[ list_index ] = list_counts[ list_index ];
	 } 
   }

 }

unsigned long SpriteSorter::getMaxSprites( unsigned long height_level )
 {
  return( max_sprite_stats[ height_level ] );
 }
