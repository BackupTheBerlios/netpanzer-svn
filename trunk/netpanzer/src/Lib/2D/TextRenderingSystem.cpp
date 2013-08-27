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
*/

#include "TextRenderingSystem.hpp"
#include "Util/Log.hpp"

#include "Surface.hpp"
#include "Types/UString.hpp"

#include <string.h>

#include "Util/FileSystem.hpp"

#include "ft2build.h"

#include FT_FREETYPE_H
static FT_Library library;

#define NUM_FACES (2)

static FT_Face faces[NUM_FACES];
static bool faces_loaded[NUM_FACES] = { false, false };

static bool lib_loaded = false;
unsigned TextRenderingSystem::font_line_height = TextRenderingSystem::FONT_HEIGHT;

static int baseline = 0;

TextRenderingSystem::FontData * TextRenderingSystem::font_data = 0;

bool TextRenderingSystem::initialize()
{
    int face_1_height = 0
       ,face_2_height = 0
       ,face_1_descent = 0
       ,face_2_descent = 0;

    int error = FT_Init_FreeType( &library );
    if ( error )
    {
        LOGGER.warning("Error initializing freetype");
    }
    else
    {
        lib_loaded = true;
        error = FT_New_Face( library, filesystem::getRealName("data/DroidSans.ttf").c_str(), 0, &faces[0] );
//        error = FT_New_Face( library, "DejaVuSans.ttf", 0, &face );
//        error = FT_New_Face( library, "DroidSansFallback.ttf", 0, &face );
        if ( error == FT_Err_Unknown_File_Format )
        {
            LOGGER.warning("Error loading DejaVuSans: unknown format");
            return false;
        }
        else if ( error )
        {
            LOGGER.warning("Error loading DejaVuSans: some other error");
            return false;
        }
        else
        {
            faces_loaded[0] = true;
            error = FT_Set_Pixel_Sizes( faces[0], /* handle to face object */
                                       0, /* pixel_width */
                                       FONT_HEIGHT ); /* pixel_height */

            face_1_height = faces[0]->size->metrics.height / 64;
            face_1_descent = (-faces[0]->size->metrics.descender) / 64;

            LOGGER.warning("Face_1: height=%d descent=%d", face_1_height, face_1_descent);
        }

        error = FT_New_Face( library, filesystem::getRealName("data/DroidSansFallback.ttf").c_str(), 0, &faces[1] );
//        error = FT_New_Face( library, "DejaVuSans.ttf", 0, &face_2 );
        if ( error == FT_Err_Unknown_File_Format )
        {
            LOGGER.warning("Error loading DroidSansFallback: unknown format");
            return false;
        }
        else if ( error )
        {
            LOGGER.warning("Error loading DroidSansFallback: some other error");
            return false;
        }
        else
        {
            faces_loaded[1] = true;

            error = FT_Set_Pixel_Sizes( faces[1], /* handle to face object */
                                       0, /* pixel_width */
                                       FONT_HEIGHT ); /* pixel_height */

            face_2_height = faces[1]->size->metrics.height / 64;
            face_2_descent = (-faces[1]->size->metrics.descender) / 64;

            LOGGER.warning("Face_2: height=%d descent=%d", face_2_height, face_2_descent);

        }
    }

    int max_height = (face_1_height > face_2_height) ? face_1_height : face_2_height;

    if ( face_1_descent != face_2_descent )
    {
        // WARNING this could crash later with some strange combinations of fonts
        int max_descent = (face_1_descent > face_2_descent) ? face_1_descent : face_2_descent;
        baseline = (max_height - 1) - max_descent;
    }
    else
    {
        baseline = (max_height - 1) - face_1_descent;
    }

    font_line_height = max_height;

    font_data = reinterpret_cast<FontData*>(malloc(sizeof(FontData)));

    if ( ! filesystem::exists("cache/fontdata.dat") )
    {
        LOGGER.warning("Font data missing, please run fontdatagen");
    }

    filesystem::ReadFile f("/cache/fontdata.dat");

    for ( int n = 0; n < CHARS_IN_TABLE; n++ )
    {
        font_data->char_to_index[n] = f.readULE16();
    }
    
    f.read(font_data->face_index,     CHARS_IN_TABLE, 1);
    f.read(font_data->normal_advance, CHARS_IN_TABLE, 1);
    f.read(font_data->first_advance,  CHARS_IN_TABLE, 1);
    
    f.close();
    
    return true;
}

void TextRenderingSystem::finalize()
{
    if ( font_data )
    {
        free(font_data);
        font_data = 0;
    }
    
    for ( int n = 0; n < NUM_FACES; n++ )
    {
        if ( faces_loaded[n] )
        {
            FT_Done_Face( faces[n] );
            faces_loaded[n] = false;
        }
    }

    if ( lib_loaded )
        FT_Done_FreeType( library );

    lib_loaded = false;
}

void TextRenderingSystem::renderString(const UString& str, Surface& dest)
{
    const int count = str.length();
    if ( ! count )
    {
        return;
    }

    int x_pos = 0;
    FT_GlyphSlot slot;

    dest.fill(0);

    for (int index = 0; index < count; index++)
    {
        const uint16_t uchar = str[index];
        FT_Face face = faces[font_data->face_index[uchar]];
        if ( FT_Load_Glyph( face, font_data->char_to_index[uchar], FT_LOAD_RENDER ) )
        {
            continue; // skip this char
        }
        
        slot = face->glyph;
        
        if ( ! slot->bitmap.width  || ! slot->bitmap.rows )
        {
            x_pos += font_data->normal_advance[uchar];
            continue;
        }

        int dest_x = x_pos + slot->bitmap_left;
        if ( dest_x < 0 )
        {
            dest_x = 0;
        }
        
        const int new_base = baseline - slot->bitmap_top;

        // @todo if new_base < 0; line should start in -new_base, so it won't crash.
        for ( int line = 0; line < slot->bitmap.rows; line++ )
        {
            const int dest_y = new_base + line;
            unsigned char * m = dest.pixPtr(dest_x, dest_y);
            unsigned char * buf = &slot->bitmap.buffer[slot->bitmap.pitch * line];

            for ( int col = 0; col < slot->bitmap.width; col++ )
            {
                m[col] = buf[col] >> 4;
            }
        }

        if ( dest_x )
        {
            x_pos += font_data->normal_advance[uchar];
        }
        else
        {
            x_pos += font_data->first_advance[uchar];
        }
        
    }
}

void TextRenderingSystem::getRenderedStringArea(const UString& str, iXY& area)
{
    int w = 0;
    
    if ( str.length() )
    {
        w += font_data->first_advance[str[0]];
        for ( unsigned n = 1; n < str.length(); n++ )
        {
            w += font_data->normal_advance[str[n]];
        }
    }
    
    area.x = w;
    area.y = line_height();
}

FT_Face* TextRenderingSystem::getFaces()
{
    return faces;
}

unsigned TextRenderingSystem::getNumFaces()
{
    return NUM_FACES;
}
