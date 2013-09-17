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

#include <math.h>
#include <memory>

#include "Palette.hpp"
#include "Util/FileSystem.hpp"
#include "Util/Exception.hpp"

#include "Util/Log.hpp"
#include "Color.hpp"

bool  Palette::loaded = false;

uint8_t Palette::full_alpha16[256*256*16];

uint8_t Palette::blend_darkenalot[256*256];
uint8_t Palette::blend_darkenalittle[256*256];
uint8_t Palette::blend_lightdark[256*256];
uint8_t Palette::blend_brighten[256*256];

uint8_t Palette::blend_2080[256*256];
uint8_t Palette::blend_4060[256*256];
uint8_t Palette::blend_6040[256*256];
uint8_t Palette::blend_8020[256*256];

uint8_t Palette::filter_darkgray[256];
uint8_t Palette::filter_darkbrown[256];
uint8_t Palette::filter_brightness[256];
uint8_t Palette::filter_green[256];

RGBColor   Palette::color[PALETTE_LENGTH];

// loadACT
//---------------------------------------------------------------------------
// Purpose: Opens the specified .act palette file and loads it into the
//          programs defined palette, system palette is not changed.
//---------------------------------------------------------------------------
void Palette::loadACT(const std::string& newname)
{
    std::string filename = "wads/" + newname + ".act";

    filesystem::ReadFile file(filename);
    if ( file.isOpen() )
    {
        if ( file.fileLength() < 768 )
        {
            throw Exception("Paletter file '%s' too small", filename.c_str());
        }
        
        for (int i = 0; i < 256; i++)
        {
            file.read(&color[i], 3, 1);
        }
    }
    else
    {
        throw Exception("Can't find palette file '%s':", filename.c_str());
    }
} // end Palette::loadACT

// findNearestColor
//---------------------------------------------------------------------------
// Purpose: Walks through the palette and finds the nearest mathcing color
//          index.
//---------------------------------------------------------------------------
uint8_t Palette::findNearestColor(int r, int g, int b, const bool &ignoreIndexZero)
{
    int   bestDist = 10000000;
    int   best     = 0;
    int   start    = ignoreIndexZero ? 1 : 0;

    for (int i = start; i < 256; i++) {
        int dr = color[i].r-r;
        int dg = color[i].g-g;
        int db = color[i].b-b;
        int dist = (dr * dr) + (dg * dg) + (db * db);

        if (dist < bestDist) {
            bestDist = dist;
            best = i;
        }
    }

    return best;

} // end Palette::findNearestColor

static bool loadTable(const char * fname, uint8_t * buffer, size_t size)
{
    filesystem::ReadFile f(fname);
    if ( f.isOpen() )
    {
        f.read(buffer, size, 1);
        LOGGER.warning("Loaded '%s'",fname);
    }

    return f.isOpen();
}

// init
//---------------------------------------------------------------------------
void Palette::init(const std::string& name)
{
    if (!loaded)
    {
        loadACT(name);

        Color::updateColors();

        loadTable("cache/filters/blend_darkenalot.tbl",    blend_darkenalot,    256*256);
        loadTable("cache/filters/blend_darkenalittle.tbl", blend_darkenalittle, 256*256);
        loadTable("cache/filters/blend_lightdark.tbl",     blend_lightdark,     256*256);
        loadTable("cache/filters/blend_brighten.tbl",      blend_brighten,      256*256);
        loadTable("cache/filters/blend_2080.tbl",          blend_2080,          256*256);
        loadTable("cache/filters/blend_4060.tbl",          blend_4060,          256*256);
        loadTable("cache/filters/blend_6040.tbl",          blend_6040,          256*256);
        loadTable("cache/filters/blend_8020.tbl",          blend_8020,          256*256);

        loadTable("cache/filters/filter_darkgray.fil",     filter_darkgray,     256);
        loadTable("cache/filters/filter_darkbrown.fil",    filter_darkbrown,    256);
        loadTable("cache/filters/filter_brightness.fil",   filter_brightness,   256);
        loadTable("cache/filters/filter_green.fil",        filter_green,        256);
        
        loadTable("cache/filters/full_alpha16.tbl",        full_alpha16,        256*256*16);

        loaded = true;
    }
} // end Palette::init
