/* 
 * File:   fontdatagen.cpp
 * Author: krom
 *
 * Created on March 7, 2013, 10:21 PM
 */

#include <stdio.h>

#include "2D/TextRenderingSystem.hpp"
#include "Util/Endian.hpp"

#include "ft2build.h"
#include FT_FREETYPE_H

#include <stdint.h>


#define CHARS_IN_TABLE (65536)

/*
 * 
 */
int main(int argc, char** argv)
{
    printf("fontdatagen for NetPanzer V 0.9\n");
    TextRenderingSystem::initialize();
    
    
    FT_Face* faces = TextRenderingSystem::getFaces();
    const unsigned nfaces = TextRenderingSystem::getNumFaces();

    unsigned max_index = 0;
    unsigned max_ccode = 0;
    
    bool    done[CHARS_IN_TABLE] = { false };
    uint16_t char_to_index[CHARS_IN_TABLE] = {0};
    uint8_t face_index[CHARS_IN_TABLE] = {0};
    uint8_t normal_advance[CHARS_IN_TABLE] = {0};
    uint8_t first_advance[CHARS_IN_TABLE] = {0};
    
    for ( unsigned f = 0; f < nfaces; f++ )
    {
        FT_Face face = faces[f];
        if ( face->charmap->encoding != FT_ENCODING_UNICODE )
        {
            printf("Font %d doesn't have unicode support\n", f);
            return 1;
        }        
        
        FT_ULong charcode;
        FT_UInt  gindex;
        FT_GlyphSlot slot;
        
        charcode = FT_Get_First_Char(face, &gindex);
        while ( gindex )
        {
            if ( gindex > max_index ) max_index = gindex;
            if ( charcode > max_ccode ) max_ccode = charcode;
            
            if ( charcode < CHARS_IN_TABLE )
            {
//                printf("%5d: %5d\n", charcode, gindex);
                if ( ! done[charcode] )
                {
                    done[charcode] = true;
                    char_to_index[charcode] = htol16(gindex);
                    face_index[charcode] = f;
                    FT_Load_Glyph( face, gindex, FT_LOAD_RENDER );
                    slot = face->glyph;

                    first_advance[charcode] = normal_advance[charcode] = slot->advance.x >> 6;
                    if ( slot->bitmap_left < 0 )
                    {
                        first_advance[charcode] -= slot->bitmap_left;
                    }

                }
            }
            
            charcode = FT_Get_Next_Char(face, charcode, &gindex);
        }
    }
    
    uint16_t cidx = FT_Get_Char_Index( faces[0], 0);
    
    FT_Load_Glyph(faces[0], cidx, FT_LOAD_RENDER);
    
    cidx = htol16(cidx);
    FT_GlyphSlot slot = faces[0]->glyph;
    uint8_t na = slot->advance.x;
    uint8_t fa = na;
    if ( slot->bitmap_left < 0 )
    {
        fa -= slot->bitmap_left;
    }
    
    for ( int n = 0; n < CHARS_IN_TABLE; n++ )
    {
        if ( ! done[n] )
        {
            char_to_index[n] = cidx;
            face_index[n] = 0;
            normal_advance[n] = na;
            first_advance[n] = fa;
        }
    }
    
    printf("max index %d\nmax charcode %d\n", max_index, max_ccode);

    FILE *f = fopen("cache/fontdata.dat", "wb");
    if ( ! f )
    {
        printf("Error creating file cache/fontdata.dat");
        TextRenderingSystem::finalize();
        return 1;
    }
    
    fwrite(char_to_index, 2, CHARS_IN_TABLE, f);
    fwrite(face_index, 1, CHARS_IN_TABLE, f);
    fwrite(normal_advance, 1, CHARS_IN_TABLE, f);
    fwrite(first_advance, 1, CHARS_IN_TABLE, f);
    
    fclose(f);
    
    TextRenderingSystem::finalize();
    
    return 0;
}

