/*
Copyright (C) 2013 Netpanzer Team. (www.netpanzer.org), Aaron Perez

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

 *
 * Created on February 21, 2013, 8:15 PM
 */

#ifndef TEXTRENDERINGSYSTEM_HPP
#define	TEXTRENDERINGSYSTEM_HPP

class Surface;
class UString;

#include <stdint.h>

#include "Types/iXY.hpp"

#include "ft2build.h"
#include FT_FREETYPE_H

class TextRenderingSystem
{
public:
    enum { FONT_HEIGHT = 16 };
    enum { FONT_WIDTH  = 16 };
    enum { CHARS_IN_TABLE = 65536 };

    struct FontData
    {
        uint16_t char_to_index[CHARS_IN_TABLE];
        uint8_t  face_index[CHARS_IN_TABLE];
        uint8_t  normal_advance[CHARS_IN_TABLE];
        uint8_t  first_advance[CHARS_IN_TABLE];
    } __attribute__((packed));

    
    static bool initialize();
    static void finalize();

    inline static unsigned line_height() { return font_line_height; }

    static void renderString(const UString& str, Surface& dest);
    static void renderMultilineString(const UString& str, Surface& dest, const int firstLineOffset = 0);
    
    static void getRenderedStringArea(const UString& str, iXY& area);
    static int getMultilineStringHeight(const UString& str, const int width, const int firstLineOffset = 0);
    
    static FT_Face* getFaces();
    static unsigned getNumFaces();
    
    inline static uint8_t getNormalAdvance(uint16_t ch) { return font_data->normal_advance[ch]; }
    inline static uint8_t getFirstAdvance(uint16_t ch) { return font_data->first_advance[ch]; }
    
private:
    static unsigned font_line_height;
    static FontData * font_data;

};

#endif	/* TEXTRENDERINGSYSTEM_HPP */
