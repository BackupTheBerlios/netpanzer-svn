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

#include <assert.h>
#include "blitters.hpp"
#include "ScreenSurface.hpp"
#include <string.h>

void blit_partial_xy(const unsigned char *tile_ptr, unsigned char *buffer_ptr,
                     short y_size, short x_size)
{
    assert(tile_ptr != 0);
    assert(buffer_ptr != 0);
    assert(y_size > 0);
    assert(x_size > 0);

    // XXX 32 should be at least a define, but probably a variable.
    // Pass in class instead of data pointer.
    for(int y=0; y<y_size; y++) {
        memcpy(buffer_ptr, tile_ptr, x_size);
        buffer_ptr += screen->getPixX();
        tile_ptr   += 32;
    }
}

void blit_partial_y(const unsigned char *tile_ptr, unsigned char *buffer_ptr,
                    short y_size)
{
    int y;
    // XXX remove 'magic' 32
    for(y=0; y<y_size; y++) {
        memcpy(buffer_ptr, tile_ptr, 32);
        tile_ptr += 32;
        buffer_ptr += screen->getPixX();
    }
}

void general_blitter(unsigned char x_size, unsigned char y_size,
                     unsigned long frame_offset, const unsigned char *buffer_ptr,
                     unsigned char *dbuffer_ptr)
{
    int x,y;

    dbuffer_ptr += frame_offset;
    for(y=0; y<y_size; y++) {
        for(x=0; x<x_size; x++) {
            if(buffer_ptr[x] != 0)
                dbuffer_ptr[x]=buffer_ptr[x];
        }
        buffer_ptr += screen->getPixX();
        dbuffer_ptr += screen->getPixX();
    }
}


void blit_selector_square( unsigned char x_size, unsigned char y_size,
                           unsigned long frame_offset, unsigned char *dbuffer_ptr )
{
    int y;
    const char color = 0x47;
    dbuffer_ptr += frame_offset;
    memset(dbuffer_ptr, color, x_size);  //top
    dbuffer_ptr += screen->getPixX();
    for(y = 1; y < (y_size-1); y++) {
        dbuffer_ptr[0] = color;  //left
        dbuffer_ptr[x_size-1] = color;  //right
        dbuffer_ptr += screen->getPixX();
    }
    memset(dbuffer_ptr, color, x_size); //bottom
}

