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
#include <config.h>

#include <math.h>
#include <memory>

#include "Palette.hpp"
#include "Util/FileSystem.hpp"
#include "Util/Exception.hpp"
#include "Util/UtilInterface.hpp"

float Palette::brightness = 1.0f;

std::string Palette::name;

ColorTable Palette::colorTable2080;
ColorTable Palette::colorTable4060;
ColorTable Palette::colorTable6040;
ColorTable Palette::colorTable8020;
//ColorTable Palette::colorTableSolid;
//ColorTable Palette::colorTableSolidTrans0;
ColorTable Palette::colorTableBrighten;
ColorTable Palette::colorTableDarkenALot;
ColorTable Palette::colorTableDarkenALittle;
ColorTable Palette::colorTableLightDark;
ColorTable Palette::fire;
ColorTable Palette::gray16;
ColorTable Palette::gray64;
ColorTable Palette::gray128;
ColorTable Palette::gray256;
ColorTable Palette::darkGray256;
ColorTable Palette::brightness256;
ColorTable Palette::red32;
ColorTable Palette::green32;
ColorTable Palette::blue32;
//ColorTable Palette::earth256;
SDL_Color   Palette::color[PALETTE_LENGTH];
SDL_Color   Palette::originalColor[PALETTE_LENGTH];
ColorTable Palette::gradientWhite2Green;
ColorTable Palette::gradientWhite2Blue;
ColorTable Palette::gradientWhite2Red;

// Palette
//---------------------------------------------------------------------------
Palette::Palette()
{
} // end Palette::Palette

// setColors
//---------------------------------------------------------------------------
void Palette::setColors()
{
    Color::unitAqua        = findNearestColor(  0, 204, 255);
    Color::unitYellow      = findNearestColor(255, 255,   0);
    Color::unitRed         = findNearestColor(255,   0,   0);
    Color::unitBlue        = findNearestColor(  3,   3, 255);
    Color::unitDarkBlue    = findNearestColor(  0,   0, 128);
    Color::unitLightGreen  = findNearestColor(163, 214, 159);
    Color::unitGreen       = findNearestColor( 31, 166,  36);
    Color::unitBlueGray    = findNearestColor( 43,  85, 131);
    Color::unitDarkRed     = findNearestColor(150,   0,   0);
    Color::unitBlack       = findNearestColor(  6,   2,   2);
    Color::unitDarkGreen   = findNearestColor( 26,  88,  36);
    Color::unitWhite       = findNearestColor(250, 253, 248);
    Color::unitLightOrange = findNearestColor(246, 210,  99);
    Color::unitOrange      = findNearestColor(205, 144,   9);
    Color::unitGray        = findNearestColor(186, 186, 186);
    Color::unitDarkGray    = findNearestColor( 99, 100, 102);

    // Grays
    Color::black   = findNearestColor(  0,   0,   0, true);
    Color::gray32  = findNearestColor( 32,  32,  32);
    Color::gray64  = findNearestColor( 64,  64,  64);
    Color::gray96  = findNearestColor( 96,  96,  96);
    Color::gray128 = findNearestColor(128, 128, 128);
    Color::gray160 = findNearestColor(160, 160, 160);
    Color::gray192 = findNearestColor(192, 192, 192);
    Color::gray224 = findNearestColor(224, 224, 224);
    Color::white   = findNearestColor(255, 255, 255);

    // Dark Colors !FIXME!
    Color::darkGray    = Color::gray64;
    Color::darkRed     = findNearestColor(128,   0,   0); // fixed
    Color::darkGreen   = findNearestColor(  0, 128,   0); // fixed
    Color::darkBlue    = findNearestColor(  0,   0, 128); // fixed
    Color::darkYellow  = findNearestColor(255, 168,  18); // fixed
    Color::darkOrange  = findNearestColor(255, 140,   0); // fixed
    Color::darkMagenta = findNearestColor(148,   0, 211); // fixed
    Color::darkCyan    = findNearestColor(  0, 206, 209); // fixed

    // Normal Colors
    Color::gray    = Color::gray128;
    Color::red     = findNearestColor(255,   0,   0); // fixed
    Color::green   = findNearestColor(  0, 255,   0); // fixed
    Color::blue    = findNearestColor(  0,   0, 255); // fixed
    Color::yellow  = findNearestColor(255, 255,   0); // fixed
    Color::orange  = findNearestColor(255, 128,   0); // fixed
    Color::magenta = findNearestColor(255,   0, 255); // fixed
    Color::cyan    = findNearestColor(  0, 255, 255); // fixed
    //Color::brown   = findNearestColor(128,  42,  42); // fixed
    Color::brown   = findNearestColor(200,  80,  80); // new brown is lighter
    
    //tan                   210 180 140   0.8235 0.7059 0.5490
    Color::tan            = findNearestColor(210, 180, 140); // fixed
    //chartreuse            127 255   0   0.4980 1.0000 0.0000
    Color::chartreuse     = findNearestColor(127, 255,   0); // fixed
    //cobalt_green           61 145  64   0.2400 0.5700 0.2500
    Color::cobaltGreen    = findNearestColor( 61, 145,  64); // fixed
    //emerald_green           0 201  87   0.0000 0.7900 0.3400
    Color::emeraldGreen   = findNearestColor(  0, 201,  87); // fixed
    //forest_green           34 139  34   0.1333 0.5451 0.1333
    Color::forestGreen    = findNearestColor( 34, 139,  34); // fixed
    //olive_green_dark       85 107  47   0.3333 0.4196 0.1843
    Color::darkOliveGreen = findNearestColor( 85, 107,  47); // fixed
    //terre_verte            56  94  15   0.2200 0.3700 0.0600
    Color::terreVerte     = findNearestColor( 56,  94,  15); // fixed

    // Normal Colors !FIXME!
    Color::lightGray    = Color::gray192;
    Color::lightRed     = findNearestColor(255, 160, 122); // fixed
    Color::lightGreen   = findNearestColor( 50, 205,  50); // fixed
    Color::lightBlue    = findNearestColor(135, 206, 250); // fixed
    Color::lightYellow  = findNearestColor(255, 255, 224); // fixed
    Color::lightOrange  = findNearestColor(237, 145,  33); // fixed
    Color::lightMagenta = findNearestColor(  0, 255, 255);
    Color::lightCyan    = findNearestColor(224, 255, 255); // fixed

} // end Palette::setColors

// setColorTables
//---------------------------------------------------------------------------
// Purpose: Creates some palettes which are specifically set to certain
//          best matched colors in the palette.
//---------------------------------------------------------------------------
void Palette::setColorTables()
{
    // 16 shades of fire.
    fire.init(16);
    fire.setColor( 0, findNearestColor(255, 252, 159));
    fire.setColor( 1, findNearestColor(255, 237, 146));
    fire.setColor( 2, findNearestColor(255, 229, 126));
    fire.setColor( 3, findNearestColor(255, 198, 130));
    fire.setColor( 4, findNearestColor(255, 213, 102));
    fire.setColor( 5, findNearestColor(255, 211,  68));
    fire.setColor( 6, findNearestColor(255, 191,  62));
    fire.setColor( 7, findNearestColor(253, 180,  42));
    fire.setColor( 8, findNearestColor(250, 160,  52));
    fire.setColor( 9, findNearestColor(251, 157,  12));
    fire.setColor(10, findNearestColor(243, 130,   3));
    fire.setColor(11, findNearestColor(229, 135,  34));
    fire.setColor(12, findNearestColor(238, 105,   1));
    fire.setColor(13, findNearestColor(221, 107,   6));
    fire.setColor(14, findNearestColor(222,  87,   3));
    fire.setColor(15, findNearestColor(223,  67,   0));
    //fire[ 9] = findNearestColor(SDL_Color(201,  53,   1));
    //fire[10] = findNearestColor(SDL_Color(171,  21,   0));
    //fire[11] = findNearestColor(SDL_Color(140,   3,   0));

    // 16 shades of gray.
    gray16.init(16);
    int num;
    for (num = 0; num < 16; num++)
        gray16.setColor(num, findNearestColor(16*(16-num)-1, 16*(16-num)-1, 16*(16-num)-1));

    // 64 shades of gray.
    gray64.init(64);
    for (num = 0; num < 64; num++) {
        gray64.setColor(num, findNearestColor(num*2, num*2, num*2));
    }
    // 128 shades of gray.
    gray128.init(128);
    int index;
    for (num = 0; num < 128; num++) {
        index = num * 2 + 1;
        gray128.setColor(num, findNearestColor(index, index, index));
    }

    // 256 shades of gray.
    gray256.init(256);
    for (num = 0; num < 256; num++) {
        int c = int(color[num].r+color[num].g+color[num].b)/3; //brightness
        int nearestColor = findNearestColor((int) (c * 1.2f),
                                            (int) (c * 1.2f),
                                            (int) (c * 1.2f));
        gray256.setColor(num, nearestColor);
    }

    // 256 shades of dark gray.
    darkGray256.init(256);
    for (num = 0; num < 256; num++) {
        int c = int(color[num].r+color[num].g+color[num].b)/3; //brightness
        int nearestColor = findNearestColor((int) (float(c) / 2.0f),
                                            (int) (float(c) / 2.0f),
                                            (int) (float(c) / 2.0f));
        darkGray256.setColor(num, nearestColor);
    }

    /*// 64 shades of gray.
    for (num = 0; num < 64; num++)
    {
    	grayLower64[num] = findNearestColor(SDL_Color(num, num, num));
    }
    // 64 shades of gray.
    for (num = 0; num < 64; num++)
    {
    	grayUpper64[num] = findNearestColor(SDL_Color(num*2, num*2, num*2));
    }
    */
    // 256 brightness values.
    brightness256.init(256);
    for (num = 0; num < 256; num++) {
        // this is brihtness int(color[num].r+color[num].g+color[num].b)/3;
        brightness256.setColor(num, int(color[num].r+color[num].g+color[num].b)/3);
    }

    // 32 shades of red.
    red32.init(32);
    for (num = 0; num < 31; num++) {
        red32.setColor(num, findNearestColor(num*8, 0, 0));
    }

    // 32 shades of green.
    green32.init(32);
    for (num = 0; num < 31; num++) {
        green32.setColor(num, findNearestColor(0, num*8, 0));
    }

    // 32 shades of blue.
    blue32.init(32);
    for (num = 0; num < 31; num++) {
        blue32.setColor(num, findNearestColor(0, 0, num*8));
    }

    // white 2 green
    {
        SDL_Color c[8];

        ramp(c, 0, 255, 255, 255, 7, 0, 255, 0);

        gradientWhite2Green.init(8);

        for (int i = 0; i < gradientWhite2Green.getColorCount(); i++)
        {
            gradientWhite2Green.setColor(i, findNearestColor(c[i].r, c[i].g, c[i].b, true));
        }
    }

    // white 2 blue
    {
        SDL_Color c[8];

        ramp(c, 0, 255, 255, 255, 7, 0, 0, 255);

        gradientWhite2Blue.init(8);

        for (int i = 0; i < gradientWhite2Blue.getColorCount(); i++)
        {
            gradientWhite2Blue.setColor(i, findNearestColor(c[i].r, c[i].g, c[i].b, true));
        }
    }

    // white 2 red
    {
        SDL_Color c[8];

        ramp(c, 0, 255, 255, 255, 7, 255, 0, 0);

        gradientWhite2Red.init(8);

        for (int i = 0; i < gradientWhite2Red.getColorCount(); i++)
        {
            gradientWhite2Red.setColor(i, findNearestColor(c[i].r, c[i].g, c[i].b, true));
        }
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

    //sprintf(strBuf, "%sSolidTrans0", tablePath);
    //colorTableSolidTrans0.createTrans0(0, 100, strBuf);

    //sprintf(strBuf, "%sSolid", tablePath);
    //colorTableSolid.create(0, 100, strBuf);

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

    std::auto_ptr<filesystem::ReadFile> file (filesystem::openRead(filename));

    try {
	for (int i = 0; i < 256; i++) {
	    file->read(&color[i], 3, 1);
	    originalColor[i] = color[i];
	}
    } catch(std::exception& e) {
	throw Exception("Error while reading Palette '%s': %s",
		filename.c_str(), e.what());
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

// ramp
//---------------------------------------------------------------------------
void Palette::ramp(SDL_Color table [], int startRGB, int r1, int g1, int b1, int endRGB, int r2, int g2, int b2)
{
    float r;
    float g;
    float b;

    r = float(r2 - r1) / float(endRGB - startRGB);
    g = float(g2 - g1) / float(endRGB - startRGB);
    b = float(b2 - b1) / float(endRGB - startRGB);

    for(int i = startRGB; i <= endRGB; i++) {
        table[i].r = r1 + int(r * (i - startRGB));
        table[i].g = g1 + int(g * (i - startRGB));
        table[i].b = b1 + int(b * (i - startRGB));
    }

} // end Palette::ramp

// init
//---------------------------------------------------------------------------
void Palette::init(const std::string& name)
{
    loadACT(name);
    setColors();
    setColorTables();
    setBrightnessAbsolute(brightness);
} // end Palette::init

// setBrightnessAbsolute
//---------------------------------------------------------------------------
void Palette::setBrightnessAbsolute(float brightness)
{
    Palette::brightness = brightness;

    for (int i = 0; i < 256; i++) {
        // Set the colors based on the original colors.
        int red = (int) (float(originalColor[i].r) * brightness);
        if (red > 255) {
            red = 255;
        } else if (red < 0) {
            red = 0;
        }

        int green = (int) (float(originalColor[i].g) * brightness);
        if (green > 255) {
            green = 255;
        } else if (green < 0) {
            green = 0;
        }

        int blue = (int) (float(originalColor[i].b) * brightness);
        if (blue > 255) {
            blue = 255;
        } else if (blue < 0) {
            blue = 0;
        }

        //		curRed   = Palette::color[y].red + ((255 - Palette::color[y].red) * (x-128) / 127);
        //		curGreen = Palette::color[y].green + ((255 - Palette::color[y].green) * (x-128) / 127);
        //		curBlue  = Palette::color[y].blue + ((255 - Palette::color[y].blue) * (x-128) / 127);

        // Save the modified values into the palette.
        color[i].r = red;
        color[i].g = green;
        color[i].b = blue;
    }

} // end Palette::setBrightnessAbsolute
