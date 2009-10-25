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
#include "Scripts/ScriptManager.hpp"

const size_t PALETTE_LENGTH = 256;

//--------------------------------------------------------------------------
class Palette
{
public:
    Palette();
    static SDL_Color color[PALETTE_LENGTH];

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

    static Uint8 findNearestColor(int r, int g, int b, const bool &ignoreIndexZero = false);
    static void init();
    static void setColors();
    static int makeColor(lua_State *L);
    static void registerScript();
};

#endif // end __Palette_hpp__
