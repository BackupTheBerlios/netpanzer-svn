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
#ifndef __Palette_hpp__
#define __Palette_hpp__

#include <string>

#include <stdint.h>

const size_t PALETTE_LENGTH = 256;

typedef struct
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t nothing;
} RGBColor;

//--------------------------------------------------------------------------
class Palette
{
private:
    static bool loaded;

    static uint8_t full_alpha16[256*256*16];

    static uint8_t blend_darkenalot[256*256];
    static uint8_t blend_darkenalittle[256*256];
    static uint8_t blend_lightdark[256*256];
    static uint8_t blend_brighten[256*256];

    static uint8_t blend_2080[256*256];
    static uint8_t blend_4060[256*256];
    static uint8_t blend_6040[256*256];
    static uint8_t blend_8020[256*256];

    static uint8_t filter_darkgray[256];
    static uint8_t filter_darkbrown[256];
    static uint8_t filter_brightness[256];
    static uint8_t filter_green[256];

public:
    inline static uint8_t * fullAlphaTable()   { return full_alpha16; }
    inline static uint8_t * blendDarkenALot() { return blend_darkenalot; }
    inline static uint8_t * blendDarkenALittle() { return blend_darkenalittle; }
    inline static uint8_t * blendLightDark() { return blend_lightdark; }
    inline static uint8_t * blendBrighten() { return blend_brighten; }
    inline static uint8_t * blend2080() { return blend_2080; }
    inline static uint8_t * blend4060() { return blend_4060; }
    inline static uint8_t * blend6040() { return blend_6040; }
    inline static uint8_t * blend8020() { return blend_8020; }

    inline static uint8_t * filterDarkGray() { return filter_darkgray; }
    inline static uint8_t * filterDarkBrown() { return filter_darkbrown; }
    inline static uint8_t * filterBrightness() { return filter_brightness; }
    inline static uint8_t * filterGreen() { return filter_green; }

    // The current loaded palette.
    static RGBColor color[PALETTE_LENGTH];         // This has any brightness value added to it.

    static void loadACT(const std::string& filename);
    static uint8_t findNearestColor(int r, int g, int b, const bool &ignoreIndexZero = false);
    static void init(const std::string& name);
};

#endif // end __Palette_hpp__
