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

#include "TileEngine.hpp"
#include "blitters.hpp"
#include "ScreenSurface.hpp"

void TileEngine::blitWorld()
{
    unsigned long  x_left_length, x_left_offset, x_right_length, x_right_offset;
    unsigned long  y_top_length, y_top_offset, y_bottom_length, y_bottom_offset;
    bool           x_right_end_flag = false, y_bottom_end_flag = false;

    unsigned short map_x, map_y;
    unsigned long  world_x, world_y;

    unsigned char *tile_offset, *buffer_offset;

    unsigned long i,k;

    unsigned char  *double_buffer;

    MapElementType *map_buffer;
    MapElementType *world_row;
    MapElementType  tile_type;

    unsigned short map_width;
    unsigned short map_height;

    unsigned long next_map_row_offset;
    unsigned long next_tile_row_offset;
    unsigned long tile_size;

    unsigned long complete_columns;
    unsigned long complete_rows;

    tile_size = tile_set.getTileXsize();

    main_camera->getViewStart(screen->getPixX(), screen->getPixY(),
                              &world_x, &world_y);
    pointXYtoMapXY( world_x, world_y, &map_x, &map_y );

    map_width = main_map.getXsize();

    map_height = main_map.getYsize();

    main_map.getRawMapBuffer( &map_buffer );

    double_buffer = screen->mem;

    x_left_offset = (world_x % tile_size);
    x_left_length = (tile_size - x_left_offset);

    complete_columns = (screen->getPixX() - x_left_length) / tile_size;

    x_right_offset = 0;
    x_right_length = (screen->getPixX() - x_left_length) % tile_size;
    if ( x_right_length > 0 )
        x_right_end_flag = true;

    y_top_offset = (world_y % tile_size);
    y_top_length = (tile_size - y_top_offset);

    complete_rows = (screen->getPixY() - y_top_length) / tile_size;

    y_bottom_offset = 0;
    y_bottom_length = (screen->getPixY() - y_top_length) % tile_size;
    if ( y_bottom_length > 0 )
        y_bottom_end_flag = true;

    world_row =  map_buffer + (map_y * map_width) + map_x;

    // XXX
    next_tile_row_offset = screen->getPixX() * (tile_size - 1);

    next_map_row_offset = map_width - complete_columns - 1;

    // THE TOP ROW

    tile_type = *(world_row);

    tile_offset = tile_set.getTile( tile_type );
    tile_offset = tile_offset + (y_top_offset * tile_size ) + x_left_offset;

    buffer_offset = double_buffer;

    blit_partial_xy( tile_offset, buffer_offset, y_top_length, x_left_length );

    world_row++;

    tile_type = *(world_row);

    tile_offset = tile_set.getTile( tile_type );
    tile_offset = tile_offset + (y_top_offset * tile_size );

    buffer_offset += x_left_length;

    for( i=0; i < complete_columns; i++ ) {
        tile_type = *(world_row);

        tile_offset = tile_set.getTile( tile_type );
        tile_offset = tile_offset + (y_top_offset * tile_size );

        blit_partial_y( tile_offset, buffer_offset, y_top_length );

        world_row++;
        buffer_offset += tile_size;
    }

    if( x_right_end_flag == true ) {
        tile_type = *(world_row);
        tile_offset = tile_set.getTile( tile_type );
        tile_offset = tile_offset + (y_top_offset * tile_size );

        if (y_top_length > 0 && x_right_length > 0) {
            blit_partial_xy(tile_offset, buffer_offset, y_top_length, x_right_length);
        }

        buffer_offset += (screen->getPixX() * (y_top_length-1)) + x_right_length;

        world_row = world_row + next_map_row_offset;
    } else {
        buffer_offset += (screen->getPixX() * (y_top_length-1));

        world_row = world_row + next_map_row_offset;
    }

    // THE MIDDLE ROWS

    for( k=0; k < complete_rows; k++) {
        tile_type = *(world_row);

        tile_offset = tile_set.getTile( tile_type );
        tile_offset = tile_offset + x_left_offset;

        blit_partial_xy( tile_offset, buffer_offset, tile_size, x_left_length);

        world_row++;
        tile_type = *(world_row);
        tile_offset = tile_set.getTile( tile_type );
        buffer_offset += x_left_length;

        for( i=0; i < complete_columns; i++ ) {
            tile_type = *(world_row);
            tile_offset = tile_set.getTile( tile_type );
            
            blit_partial_y( tile_offset, buffer_offset, tile_size );

            world_row++;
            buffer_offset += tile_size;
        }


        if( x_right_end_flag == true ) {
            tile_type = *(world_row);
            tile_offset = tile_set.getTile( tile_type );
            if (tile_size > 0 && x_right_length > 0) {
                blit_partial_xy( tile_offset, buffer_offset, tile_size, x_right_length );
            }

            buffer_offset += next_tile_row_offset + x_right_length;

            world_row = world_row + next_map_row_offset;
        } else {
            buffer_offset += next_tile_row_offset;

            world_row = world_row + next_map_row_offset;
        }

    }

    // NOW THE LAST ROW IF NEEDED
    if( y_bottom_end_flag == true ) {
        tile_type = *(world_row);

        tile_offset = tile_set.getTile( tile_type );
        tile_offset = tile_offset + x_left_offset;

        if (y_bottom_length > 0 && x_left_length > 0) {
            blit_partial_xy( tile_offset, buffer_offset, y_bottom_length, x_left_length );
        }

        world_row++;

        tile_type = *(world_row);

        tile_offset = tile_set.getTile( tile_type );

        buffer_offset += x_left_length;

        for( i=0; i < complete_columns; i++ ) {
            tile_type = *(world_row);
            tile_offset = tile_set.getTile( tile_type );
            
            if (y_bottom_length > 0) {
                blit_partial_y( tile_offset, buffer_offset, y_bottom_length );
            }
            world_row++;
            buffer_offset += tile_size;
        }

        if( x_right_end_flag == true ) {
            tile_type = *(world_row);
            tile_offset = tile_set.getTile( tile_type );
            
            if (y_bottom_length > 0 && x_right_length > 0) {
                blit_partial_xy( tile_offset, buffer_offset, y_bottom_length, x_right_length );
            }

            buffer_offset += (screen->getPixX() * (y_bottom_length-1)) + x_right_length;

            world_row = world_row + next_map_row_offset;
        } else {
            buffer_offset += (screen->getPixX() * (y_bottom_length-1));

            world_row = world_row + next_map_row_offset;
        }
    }
}
