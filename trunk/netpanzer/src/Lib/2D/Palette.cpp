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

#include "Scripts/ScriptManager.hpp"

bool  Palette::loaded = false;

std::string Palette::name;

ColorTable Palette::colorTable2080;
ColorTable Palette::colorTable4060;
ColorTable Palette::colorTable6040;
ColorTable Palette::colorTable8020;
ColorTable Palette::colorTableBrighten;
ColorTable Palette::colorTableDarkenALot;
ColorTable Palette::colorTableDarkenALittle;
ColorTable Palette::colorTableLightDark;
ColorTable Palette::darkGray256;
ColorTable Palette::darkbrown256;
ColorTable Palette::brightness256;
ColorTable Palette::green256;
SDL_Color   Palette::color[PALETTE_LENGTH];

// Palette
//---------------------------------------------------------------------------
Palette::Palette()
{
} // end Palette::Palette

// setColors
//---------------------------------------------------------------------------
void Palette::setColors()
{
    ScriptManager::runFileInTable("scripts/initcolors.lua", "Color");
} // end Palette::setColors

// setColorTables
//---------------------------------------------------------------------------
// Purpose: Creates some palettes which are specifically set to certain
//          best matched colors in the palette.
//---------------------------------------------------------------------------
void Palette::setColorTables()
{
    int num;
    
    // 256 shades of dark gray.
    darkGray256.init(256);
    for (num = 0; num < 256; num++) {
        int c = color[num].r/3;// dark gray
        darkGray256.setColor(num, int(findNearestColor(c, c, c))); // dark gray
    }

    darkbrown256.init(256);
    for (num = 0; num < 256; num++) {
        // the magic values are for palete color 33
        int r = abs(color[num].r - 0x16)/4.5;
        int g = abs(color[num].g - 0x1a)/4.5;
        int b = abs(color[num].b - 0x19)/4.5;
        darkbrown256.setColor(num, int(findNearestColor(0x16 + r,0x1a + g,0x19 + b)));
    }

    green256.init(256);
    for (num = 0; num < 256; num++) {
        // the magic values are for palete color 41
        int r = abs(color[num].r - 0x1b)/3;
        int g = abs(color[num].g - 0x3e)/3;
        int b = abs(color[num].b - 0x21)/3;
        green256.setColor(num, int(findNearestColor(0x1b + r,0x3e + g,0x21 + b)));
    }

    // 256 brightness values.
    brightness256.init(256);
    for (num = 0; num < 256; num++) {
        int c = color[num].r/1.3;// dark gray
        brightness256.setColor(num, int(findNearestColor(c, c, c))); // dark gray
    }

    char tablePath[512];
    snprintf(tablePath, 512, "cache/colorfilters/%s", name.c_str());
    if(!filesystem::exists(tablePath)) {
        filesystem::mkdir(tablePath);
    }
    char strBuf[512];

    // Best color match.
    sprintf(strBuf, "%s/2080.tbl", tablePath);
    colorTable2080.create(20, 80, strBuf);

    sprintf(strBuf, "%s/4060.tbl", tablePath);
    colorTable4060.create(40, 60, strBuf);

    sprintf(strBuf, "%s/6040.tbl", tablePath);
    colorTable6040.create(60, 40, strBuf);

    sprintf(strBuf, "%s/8020.tbl", tablePath);
    colorTable8020.create(80, 20, strBuf);

    // Brighten.
    sprintf(strBuf, "%s/Brighten.tbl", tablePath);
    colorTableBrighten.createBrightenFilter(strBuf, 256);

    // Darken.
    sprintf(strBuf, "%s/DarkenALot.tbl", tablePath);
    colorTableDarkenALot.createDarkenFilter(strBuf, 0.5f);
    
    sprintf(strBuf, "%s/DarkenALittle.tbl", tablePath);
    colorTableDarkenALittle.createDarkenFilter(strBuf, 0.15f);

    sprintf(strBuf, "%s/LightDark.tbl", tablePath);
    colorTableLightDark.createLightDarkFilter(strBuf);
} // end setColorTables

// loadACT
//---------------------------------------------------------------------------
// Purpose: Opens the specified .act palette file and loads it into the
//          programs defined palette, system palette is not changed.
//---------------------------------------------------------------------------
void Palette::loadACT(const std::string& newname)
{
    name = newname;
    std::string filename = "wads/" + name + ".act";

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
Uint8 Palette::findNearestColor(int r, int g, int b, const bool &ignoreIndexZero)
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

// init
//---------------------------------------------------------------------------
void Palette::init(const std::string& name)
{
    if (!loaded)
    {
        loadACT(name);
        setColors();
        setColorTables();
        loaded = true;
    }
} // end Palette::init
