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
RGBColor   Palette::color[PALETTE_LENGTH];
RGBColor   Palette::originalColor[PALETTE_LENGTH];
ColorTable Palette::gradientWhite2Green;
ColorTable Palette::gradientWhite2Blue;
ColorTable Palette::gradientWhite2Red;

// Palette
//---------------------------------------------------------------------------
Palette::Palette()
{
} // end Palette::Palette

// setBrightness
//---------------------------------------------------------------------------
void Palette::setBrightness(float brightness)
{
    assert(brightness >= -1.0f && brightness <= +1.0f);

    Palette::brightness = brightness;

} // end Palette::setBrightness

// setColors
//---------------------------------------------------------------------------
void Palette::setColors()
{
    Color::unitAqua        = findNearestColor(RGBColor(  0, 204, 255));
    Color::unitYellow      = findNearestColor(RGBColor(255, 255,   0));
    Color::unitRed         = findNearestColor(RGBColor(255,   0,   0));
    Color::unitBlue        = findNearestColor(RGBColor(  3,   3, 255));
    Color::unitDarkBlue    = findNearestColor(RGBColor(  0,   0, 128));
    Color::unitLightGreen  = findNearestColor(RGBColor(163, 214, 159));
    Color::unitGreen       = findNearestColor(RGBColor( 31, 166,  36));
    Color::unitBlueGray    = findNearestColor(RGBColor( 43,  85, 131));
    Color::unitDarkRed     = findNearestColor(RGBColor(150,   0,   0));
    Color::unitBlack       = findNearestColor(RGBColor(  6,   2,   2));
    Color::unitDarkGreen   = findNearestColor(RGBColor( 26,  88,  36));
    Color::unitWhite       = findNearestColor(RGBColor(250, 253, 248));
    Color::unitLightOrange = findNearestColor(RGBColor(246, 210,  99));
    Color::unitOrange      = findNearestColor(RGBColor(205, 144,   9));
    Color::unitGray        = findNearestColor(RGBColor(186, 186, 186));
    Color::unitDarkGray    = findNearestColor(RGBColor( 99, 100, 102));

    // Grays
    Color::black   = findNearestColor(RGBColor(  0,   0,   0), true);
    Color::gray32  = findNearestColor(RGBColor( 32,  32,  32));
    Color::gray64  = findNearestColor(RGBColor( 64,  64,  64));
    Color::gray96  = findNearestColor(RGBColor( 96,  96,  96));
    Color::gray128 = findNearestColor(RGBColor(128, 128, 128));
    Color::gray160 = findNearestColor(RGBColor(160, 160, 160));
    Color::gray192 = findNearestColor(RGBColor(192, 192, 192));
    Color::gray224 = findNearestColor(RGBColor(224, 224, 224));
    Color::white   = findNearestColor(RGBColor(255, 255, 255));

    // Dark Colors !FIXME!
    Color::darkGray    = Color::gray64;
    Color::darkRed     = findNearestColor(RGBColor(128,   0,   0)); // fixed
    Color::darkGreen   = findNearestColor(RGBColor(  0, 128,   0)); // fixed
    Color::darkBlue    = findNearestColor(RGBColor(  0,   0, 128)); // fixed
    Color::darkYellow  = findNearestColor(RGBColor(255, 168,  18)); // fixed
    Color::darkOrange  = findNearestColor(RGBColor(255, 140,   0)); // fixed
    Color::darkMagenta = findNearestColor(RGBColor(148,   0, 211)); // fixed
    Color::darkCyan    = findNearestColor(RGBColor(  0, 206, 209)); // fixed

    // Normal Colors
    Color::gray    = Color::gray128;
    Color::red     = findNearestColor(RGBColor(255,   0,   0)); // fixed
    Color::green   = findNearestColor(RGBColor(  0, 255,   0)); // fixed
    Color::blue    = findNearestColor(RGBColor(  0,   0, 255)); // fixed
    Color::yellow  = findNearestColor(RGBColor(255, 255,   0)); // fixed
    Color::orange  = findNearestColor(RGBColor(255, 128,   0)); // fixed
    Color::magenta = findNearestColor(RGBColor(255,   0, 255)); // fixed
    Color::cyan    = findNearestColor(RGBColor(  0, 255, 255)); // fixed
    Color::brown   = findNearestColor(RGBColor(128,  42,  42)); // fixed

    //tan                   210 180 140   0.8235 0.7059 0.5490
    Color::tan            = findNearestColor(RGBColor(210, 180, 140)); // fixed
    //chartreuse            127 255   0   0.4980 1.0000 0.0000
    Color::chartreuse     = findNearestColor(RGBColor(127, 255,   0)); // fixed
    //cobalt_green           61 145  64   0.2400 0.5700 0.2500
    Color::cobaltGreen    = findNearestColor(RGBColor( 61, 145,  64)); // fixed
    //emerald_green           0 201  87   0.0000 0.7900 0.3400
    Color::emeraldGreen   = findNearestColor(RGBColor(  0, 201,  87)); // fixed
    //forest_green           34 139  34   0.1333 0.5451 0.1333
    Color::forestGreen    = findNearestColor(RGBColor( 34, 139,  34)); // fixed
    //olive_green_dark       85 107  47   0.3333 0.4196 0.1843
    Color::darkOliveGreen = findNearestColor(RGBColor( 85, 107,  47)); // fixed
    //terre_verte            56  94  15   0.2200 0.3700 0.0600
    Color::terreVerte     = findNearestColor(RGBColor( 56,  94,  15)); // fixed

    // Normal Colors !FIXME!
    Color::lightGray    = Color::gray192;
    Color::lightRed     = findNearestColor(RGBColor(255, 160, 122)); // fixed
    Color::lightGreen   = findNearestColor(RGBColor( 50, 205,  50)); // fixed
    Color::lightBlue    = findNearestColor(RGBColor(135, 206, 250)); // fixed
    Color::lightYellow  = findNearestColor(RGBColor(255, 255, 224)); // fixed
    Color::lightOrange  = findNearestColor(RGBColor(237, 145,  33)); // fixed
    Color::lightMagenta = findNearestColor(RGBColor(  0, 255, 255));
    Color::lightCyan    = findNearestColor(RGBColor(224, 255, 255)); // fixed

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
    fire.setColor( 0, findNearestColor(RGBColor(255, 252, 159)));
    fire.setColor( 1, findNearestColor(RGBColor(255, 237, 146)));
    fire.setColor( 2, findNearestColor(RGBColor(255, 229, 126)));
    fire.setColor( 3, findNearestColor(RGBColor(255, 198, 130)));
    fire.setColor( 4, findNearestColor(RGBColor(255, 213, 102)));
    fire.setColor( 5, findNearestColor(RGBColor(255, 211,  68)));
    fire.setColor( 6, findNearestColor(RGBColor(255, 191,  62)));
    fire.setColor( 7, findNearestColor(RGBColor(253, 180,  42)));
    fire.setColor( 8, findNearestColor(RGBColor(250, 160,  52)));
    fire.setColor( 9, findNearestColor(RGBColor(251, 157,  12)));
    fire.setColor(10, findNearestColor(RGBColor(243, 130,   3)));
    fire.setColor(11, findNearestColor(RGBColor(229, 135,  34)));
    fire.setColor(12, findNearestColor(RGBColor(238, 105,   1)));
    fire.setColor(13, findNearestColor(RGBColor(221, 107,   6)));
    fire.setColor(14, findNearestColor(RGBColor(222,  87,   3)));
    fire.setColor(15, findNearestColor(RGBColor(223,  67,   0)));
    //fire[ 9] = findNearestColor(RGBColor(201,  53,   1));
    //fire[10] = findNearestColor(RGBColor(171,  21,   0));
    //fire[11] = findNearestColor(RGBColor(140,   3,   0));

    // 16 shades of gray.
    gray16.init(16);
    int num;
    for (num = 0; num < 16; num++)
        gray16.setColor(num, findNearestColor(RGBColor(16*(16-num)-1, 16*(16-num)-1, 16*(16-num)-1)));

    // 64 shades of gray.
    gray64.init(64);
    for (num = 0; num < 64; num++) {
        gray64.setColor(num, findNearestColor(RGBColor(num*2, num*2, num*2)));
    }
    // 128 shades of gray.
    gray128.init(128);
    int index;
    for (num = 0; num < 128; num++) {
        index = num * 2 + 1;
        gray128.setColor(num, findNearestColor(RGBColor(index, index, index)));
    }

    // 256 shades of gray.
    gray256.init(256);
    for (num = 0; num < 256; num++) {
        int c            = color[num].getBrightnessInt();
        int nearestColor = findNearestColor(RGBColor((int) (c * 1.2f),
                                            (int) (c * 1.2f),
                                            (int) (c * 1.2f)));
        gray256.setColor(num, nearestColor);
    }

    // 256 shades of dark gray.
    darkGray256.init(256);
    for (num = 0; num < 256; num++) {
        int c            = color[num].getBrightnessInt();
        int nearestColor = findNearestColor(RGBColor((int) (float(c) / 2.0f),
                                            (int) (float(c) / 2.0f),
                                            (int) (float(c) / 2.0f)));
        darkGray256.setColor(num, nearestColor);
    }

    /*// 64 shades of gray.
    for (num = 0; num < 64; num++)
    {
    	grayLower64[num] = findNearestColor(RGBColor(num, num, num));
    }
    // 64 shades of gray.
    for (num = 0; num < 64; num++)
    {
    	grayUpper64[num] = findNearestColor(RGBColor(num*2, num*2, num*2));
    }
    */
    // 256 brightness values.
    brightness256.init(256);
    for (num = 0; num < 256; num++) {
        brightness256.setColor(num, color[num].getBrightnessInt());
    }

    // 32 shades of red.
    red32.init(32);
    for (num = 0; num < 31; num++) {
        red32.setColor(num, findNearestColor(RGBColor(num*8, 0, 0)));
    }

    // 32 shades of green.
    green32.init(32);
    for (num = 0; num < 31; num++) {
        green32.setColor(num, findNearestColor(RGBColor(0, num*8, 0)));
    }

    // 32 shades of blue.
    blue32.init(32);
    for (num = 0; num < 31; num++) {
        blue32.setColor(num, findNearestColor(RGBColor(0, 0, num*8)));
    }

    // white 2 green
    {
        RGBColor c[8];

        ramp(c, 0, 255, 255, 255, 7, 0, 255, 0);

        gradientWhite2Green.init(8);

        for (int i = 0; i < gradientWhite2Green.getColorCount(); i++)
        {
            gradientWhite2Green.setColor(i, findNearestColor(c[i], true));
        }
    }

    // white 2 blue
    {
        RGBColor c[8];

        ramp(c, 0, 255, 255, 255, 7, 0, 0, 255);

        gradientWhite2Blue.init(8);

        for (int i = 0; i < gradientWhite2Blue.getColorCount(); i++)
        {
            gradientWhite2Blue.setColor(i, findNearestColor(c[i], true));
        }
    }

    // white 2 red
    {
        RGBColor c[8];

        ramp(c, 0, 255, 255, 255, 7, 255, 0, 0);

        gradientWhite2Red.init(8);

        for (int i = 0; i < gradientWhite2Red.getColorCount(); i++)
        {
            gradientWhite2Red.setColor(i, findNearestColor(c[i], true));
        }
    }

    char tablePath[512];
    snprintf(tablePath, 512, "cache/colorfilters/%s", name.c_str());
    if(!FileSystem::exists(tablePath)) {
        FileSystem::mkdir(tablePath);
    }
    char strBuf[512];
    //progressView->scrollAndUpdate("");

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

    std::auto_ptr<ReadFile> file (FileSystem::openRead(filename));

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
uint8_t Palette::findNearestColor(const RGBColor &rgb, const bool &ignoreIndexZero)
{
    float bestDist = 10000000.0f;
    int   best     = 0;
    int   start    = ignoreIndexZero ? 1 : 0;

    float vPic = sqrt(rgb.red * rgb.red + rgb.green * rgb.green + rgb.blue * rgb.blue) * 0.57735027;

    //float vPic = sqrt(rgb.red * rgb.red + rgb.green * rgb.green + rgb.blue * rgb.blue);

    for (int i = start; i < 256; i++) {
        //float vPal = sqrt(color[i].red * color[i].red + color[i].green * color[i].green + color[i].blue*color[i].blue);
        float vPal = sqrt(color[i].red * color[i].red + color[i].green * color[i].green + color[i].blue*color[i].blue) * 0.57735027;

        float dr = color[i].red-rgb.red;
        float dg = color[i].green-rgb.green;
        float db = color[i].blue-rgb.blue;
        float dv = vPal-vPic;
        //float dist = dr * dr + dg * dg + db * db;
        float dist = dr * dr * 0.3 + dg * dg * 0.59 + db * db * 0.11 + dv * dv * 0.7;

        if (dist < bestDist) {
            bestDist = dist;
            best = i;
        }
    }

    return best;

} // end Palette::findNearestColor

// ramp
//---------------------------------------------------------------------------
void Palette::ramp(RGBColor table [], int startRGB, int r1, int g1, int b1, int endRGB, int r2, int g2, int b2)
{
    float r;
    float g;
    float b;

    r = float(r2 - r1) / float(endRGB - startRGB);
    g = float(g2 - g1) / float(endRGB - startRGB);
    b = float(b2 - b1) / float(endRGB - startRGB);

    for(int i = startRGB; i <= endRGB; i++) {
        table[i].red   = r1 + int(r * (i - startRGB));
        table[i].green = g1 + int(g * (i - startRGB));
        table[i].blue  = b1 + int(b * (i - startRGB));
    }

} // end Palette::ramp

// setEarth
//---------------------------------------------------------------------------
void Palette::setEarth()
{
    //RGBColor c[256];
    //
    //ramp(c,  0,   0,   0,  40,  30,   0,   0, 160); // Deep Water        2 Shallow Water
    //ramp(c, 31,   0,   0, 160,  40,  16, 172, 220); // Sallow Water      2 Very Shallow Water
    //ramp(c, 41,  16, 172, 220,  46, 176, 148, 128); // Very Sallow Water 2 Beach
    //ramp(c, 47, 176, 148, 128,  55,  60, 148,  80); // Beach             2 Grass
    //ramp(c, 56,  60, 148,  80, 100, 128,  92,  64); // Grass             2 Mountain
    //ramp(c,101, 128,  92,  64, 204,  72,  52,  36); // Mountain          2 Mountain
    //ramp(c,205,  72,  52,  36, 225, 200, 200, 200); // Mountain          2 Snow
    //ramp(c,226, 200, 200, 200, 255, 255, 255, 255); // Mountain          2 Snow
    //
    //// Now transfer the temp values into the earth values as
    //// best as possible.
    //earth256.init(256);
    //
    //for (int i = 0; i < earth256.getColorCount(); i++)
    //{
    //	earth256.setColor(i, findNearestColor(c[i]));
    //}

} // end Palette::setEarth

// setFire
//---------------------------------------------------------------------------
void Palette::setFire()
{
    ramp(color,   0,   0,   0,   0,  95, 255, 255, 255); // Black  2 White
    ramp(color,  96, 255, 255, 255, 130, 255, 255,   0); // White  2 Yellow
    ramp(color, 131, 255, 255,   0, 247, 255,   3,   3); // Yellow 2 Red

} // end Palette::setFire

// init
//---------------------------------------------------------------------------
void Palette::init(const std::string& name)
{
    //setFire();
    //return;
    loadACT(name);
    setColors();
    setColorTables();
    setBrightnessAbsolute(brightness);
} // end Palette::init

// initNoColorTables
//---------------------------------------------------------------------------
void Palette::initNoColorTables(const std::string& name)
{
    loadACT(name);
    setColors();
    setBrightnessAbsolute(brightness);
} // end Palette::initNoColorTables

// setBrightnessRelative
//---------------------------------------------------------------------------
void Palette::setBrightnessRelative(float brightness)
{
    Palette::brightness = brightness;
    //	for (int i = 0; i < 256; i++)
    //	{
    //		// Set the colors based on the original colors.
    //		int red = float(originalColor[i].red) * brightness;
    //		if (red > 255) { red = 255; }
    //		else if (red < 0) { red = 0; }
    //
    //		int green = float(originalColor[i].green) * brightness;
    //		if (green > 255) { green = 255; }
    //		else if (green < 0) { green = 0; }
    //
    //		int blue = float(paloriginalColor[i].blue) * brightness;
    //		if (blue > 255) { blue = 255; }
    //		else if (blue < 0) { blue = 0; }
    //
    ////		curRed   = Palette::color[y].red + ((255 - Palette::color[y].red) * (x-128) / 127);
    ////		curGreen = Palette::color[y].green + ((255 - Palette::color[y].green) * (x-128) / 127);
    ////		curBlue  = Palette::color[y].blue + ((255 - Palette::color[y].blue) * (x-128) / 127);
    //
    //		// Save the modified values into the palette.
    //		color[i].red   = red;
    //		color[i].green = green;
    //		color[i].blue  = blue;
    //	}

} // end Palette::setBrightnessRelative

// setBrightnessAbsolute
//---------------------------------------------------------------------------
void Palette::setBrightnessAbsolute(float brightness)
{
    Palette::brightness = brightness;

    for (int i = 0; i < 256; i++) {
        // Set the colors based on the original colors.
        int red = (int) (float(originalColor[i].red) * brightness);
        if (red > 255) {
            red = 255;
        } else if (red < 0) {
            red = 0;
        }

        int green = (int) (float(originalColor[i].green) * brightness);
        if (green > 255) {
            green = 255;
        } else if (green < 0) {
            green = 0;
        }

        int blue = (int) (float(originalColor[i].blue) * brightness);
        if (blue > 255) {
            blue = 255;
        } else if (blue < 0) {
            blue = 0;
        }

        //		curRed   = Palette::color[y].red + ((255 - Palette::color[y].red) * (x-128) / 127);
        //		curGreen = Palette::color[y].green + ((255 - Palette::color[y].green) * (x-128) / 127);
        //		curBlue  = Palette::color[y].blue + ((255 - Palette::color[y].blue) * (x-128) / 127);

        // Save the modified values into the palette.
        color[i].red   = red;
        color[i].green = green;
        color[i].blue  = blue;
    }

} // end Palette::setBrightnessAbsolute
