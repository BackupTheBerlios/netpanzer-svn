/*
Copyright (C) 2003 by Matthias Braun
 
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

#include <stdint.h>
#include "Line.hpp"

namespace UI
{
    static inline void putPixel(SDL_Surface* surface, size_t x, size_t y,
                                Color color)
    {
        // XXX hardcoded to 8 bit for now...
        uint8_t* pixels = (uint8_t*) surface->pixels;
        pixels[y * surface->pitch + x] = color; 
    }
    
    // bresenham algorithm for drawing a line
    void drawLine(SDL_Surface* surface, iXY from, iXY to, Color color)
    {
        int x_unit = 1;
        int y_unit = 1;
        int delta_x = to.x - from.x;
        int delta_y = to.y - from.y;
        int errorterm = 0;

        if(delta_y < 0) {
            delta_y = -delta_y;
            y_unit = -1;
        }
        if(delta_x < 0) {
            delta_x = -delta_x;
            x_unit = -1;
        }

        iXY pos = from;

        if(delta_x > delta_y) {
            int length = delta_x + 1;
            for(int i=0; i<length; i++) {
                putPixel(surface, pos.x, pos.y, color);
                pos.x += x_unit;
                errorterm += y_unit;
                if(errorterm > delta_x) {
                    errorterm -= delta_x;
                    pos.y += y_unit;
                }
            }
        } else {
            int length = delta_y + 1;

            for(int i=0; i<length; i++) {
                putPixel(surface, pos.x, pos.y, color);
                pos.y += y_unit;
                errorterm += x_unit;
                if(errorterm > delta_y) {
                    errorterm -= delta_y;
                    pos.x += x_unit;
                }

            }
        }
    }
} // end of namespace UI
