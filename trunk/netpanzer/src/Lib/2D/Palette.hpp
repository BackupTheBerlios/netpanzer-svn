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
#include <stdio.h>

#include "Color.hpp"
#include "ColorTable.hpp"
#include "SDL.h"

const size_t PALETTE_LENGTH = 256;

//--------------------------------------------------------------------------
class Palette
{
private:
    static std::string name;
    static bool loaded;

public:
    Palette();

    // Best color match blending tables.
    static ColorTable	colorTable2080;
    static ColorTable	colorTable4060;
    static ColorTable	colorTable6040;
    static ColorTable	colorTable8020;

    // Brighten.
    static ColorTable	colorTableBrighten;

    // Darken.
    static ColorTable	colorTableDarkenALittle;
    static ColorTable	colorTableDarkenALot;

    // LightDark
    static ColorTable	colorTableLightDark;

    // Specific color value tables.
    static ColorTable darkGray256;
    static ColorTable darkbrown256;
    static ColorTable brightness256;
    static ColorTable green256;

    // The current loaded palette.
    static SDL_Color color[PALETTE_LENGTH];         // This has any brightness value added to it.

    SDL_Color &operator [](size_t index)
    {
        assert(index < PALETTE_LENGTH);
        return color[index];
    }

    inline const SDL_Color &operator [](size_t index) const
    {
        assert(index < PALETTE_LENGTH);
        return color[index];
    }

    static void loadACT(const std::string& filename);
    static Uint8 findNearestColor(int r, int g, int b, const bool &ignoreIndexZero = false);
    static void init(const std::string& name);
    static std::string getName()
    {
        return name;
    }
    static void setColors();
    static void setColorTables();
};

#endif // end __Palette_hpp__
