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
#ifndef _BLITTERS_H
#define _BLITTERS_H

void blit_partial_xy( unsigned char *tile_ptr,unsigned char *buffer_ptr,short y_size,short x_size );


void blit_partial_y( unsigned char *tile_ptr, unsigned char *buffer_ptr, short y_size);

void general_blitter( unsigned char x_size, unsigned char y_size, unsigned long frame_offset, 
                      unsigned char *buffer_ptr, unsigned char *dbuffer_ptr );

void blit_selector_square( unsigned char x_size, unsigned char y_size,
                           unsigned long frame_offset, unsigned char *dbuffer_ptr );

#endif
