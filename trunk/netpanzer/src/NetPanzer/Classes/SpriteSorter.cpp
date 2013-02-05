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


#include <algorithm>
#include <functional>
#include "Util/Exception.hpp"
#include "SpriteSorter.hpp"

SpriteSorter SPRITE_SORTER;

class SpriteCompare
            : public std::binary_function<const Sprite*, const Sprite*, bool>
{
public:
    bool operator()(const Sprite* sprite1, const Sprite* sprite2) const
    {
        long y1 = sprite1->world_pos.y;
        long y2 = sprite2->world_pos.y;

        //if (y1 > y2) return +1;
        if (y1 < y2) return true; else return false;

        // Tie based on the real comparison basis - impose an arbitrary
        // but consistent sort order to break the tie.

        //if (sprite1 > sprite2) return +1;
        if (sprite1 < sprite2) return true; else return false;

        // We should never get here, unless the same sprite is
        // inserted in the list twice
        throw Exception("sprite_key called to compare the same sprite against itself!");
    }
};

SpriteSorter::SpriteSorter( )
{
    initialize();
}

void SpriteSorter::initialize( )
{
    unsigned long i;

    for ( i = 0; i < 3; i++ ) {
        sprite_lists[i].initialize(50);
    }

    sprite_lists[3].initialize(500);
    sprite_lists[4].initialize(500);
    sprite_lists[5].initialize(500);

    for ( i = 6; i < _MAX_HEIGHT_LEVELS; i++ )
    {
        sprite_lists[i].initialize(50);
    }

    for ( i = 0; i < _MAX_HEIGHT_LEVELS; i++ )
    {
        max_sprite_stats[i] = 0;
    }
}

void SpriteSorter::reset( void )
{
    for ( size_t i = 0; i < _MAX_HEIGHT_LEVELS; i++ )
    {
        sprite_lists[i].fastClear();
    }

}

void SpriteSorter::reset( iRect &world_win )
{
    world_window = world_win;
    reset();
}

void SpriteSorter::addSprite(Sprite *sprite)
{
    if ( sprite->isVisible( world_window ) )
    {
        forceAddSprite(sprite);
    }
}

void SpriteSorter::forceAddSprite(Sprite *sprite)
{
    unsigned char height;

    height = sprite->getSpriteHeight();

    assert( height < _MAX_HEIGHT_LEVELS );

    sprite_lists[height].push_back(sprite);
    
}

void SpriteSorter::sortLists()
{
    unsigned long i;

    for ( i = 0; i < _MAX_HEIGHT_LEVELS; i++ )
    {
        sort(sprite_lists[i].begin(), sprite_lists[i].end(),SpriteCompare());
    }
}

void SpriteSorter::blitLists( Surface *render_surf )
{
    for ( size_t h = 0; h < _MAX_HEIGHT_LEVELS; h++ )
    {
        for ( size_t n = 0; n < sprite_lists[h].getLastIndex(); n++ )
        {
            sprite_lists[ h ][n]->blitAll( render_surf, world_window );
        }

        if ( sprite_lists[h].getLastIndex() > max_sprite_stats[ h ] )
        {
            max_sprite_stats[ h ] = sprite_lists[h].getLastIndex();
        }
    }
}

unsigned long SpriteSorter::getMaxSprites(unsigned long height_level) const
{
    return( max_sprite_stats[ height_level ] );
}

