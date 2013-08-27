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

#include "Color.hpp"
#include "Palette.hpp"

#define GEN_COLORS(GENERATOR) \
    GENERATOR(unitAqua) \
    GENERATOR(unitYellow) \
    GENERATOR(unitRed) \
    GENERATOR(unitBlue) \
    GENERATOR(unitDarkBlue) \
    GENERATOR(unitLightGreen) \
    GENERATOR(unitGreen) \
    GENERATOR(unitBlueGray) \
    GENERATOR(unitDarkRed) \
    GENERATOR(unitBlack) \
    GENERATOR(unitDarkGreen) \
    GENERATOR(unitWhite) \
    GENERATOR(unitLightOrange) \
    GENERATOR(unitOrange) \
    GENERATOR(unitGray) \
    GENERATOR(unitDarkGray) \
    GENERATOR(black) \
    GENERATOR(blue) \
    GENERATOR(brown) \
    GENERATOR(cyan) \
    GENERATOR(gray) \
    GENERATOR(green) \
    GENERATOR(magenta) \
    GENERATOR(orange) \
    GENERATOR(pink) \
    GENERATOR(red) \
    GENERATOR(white) \
    GENERATOR(yellow) \
    GENERATOR(tan) \
    GENERATOR(chartreuse) \
    GENERATOR(cobaltGreen) \
    GENERATOR(emeraldGreen) \
    GENERATOR(forestGreen) \
    GENERATOR(darkOliveGreen) \
    GENERATOR(terreVerte) \
    GENERATOR(darkBlue) \
    GENERATOR(darkBrown) \
    GENERATOR(darkCyan) \
    GENERATOR(darkGray) \
    GENERATOR(darkGreen) \
    GENERATOR(darkMagenta) \
    GENERATOR(darkOrange) \
    GENERATOR(darkPink) \
    GENERATOR(darkRed) \
    GENERATOR(darkYellow) \
    GENERATOR(lightBlue) \
    GENERATOR(lightBrown) \
    GENERATOR(lightCyan) \
    GENERATOR(lightGray) \
    GENERATOR(lightGreen) \
    GENERATOR(lightMagenta) \
    GENERATOR(lightOrange) \
    GENERATOR(lightPink) \
    GENERATOR(lightRed) \
    GENERATOR(lightYellow) \
    GENERATOR(gray32) \
    GENERATOR(gray64) \
    GENERATOR(gray96) \
    GENERATOR(gray128) \
    GENERATOR(gray160) \
    GENERATOR(gray192) \
    GENERATOR(gray224)

// Have to merge like this, cant use ##
#define MERGE_CLASS(a) a::
#define GEN_VAR(CNAME) uint8_t MERGE_CLASS(Color)CNAME;

// This generate the uint8_t Color::colorname for all the colors
// Must not have ';' at end
GEN_COLORS(GEN_VAR)

void Color::updateColors()
{
    unitAqua        = Palette::findNearestColor(  0, 204, 255);
    unitYellow      = Palette::findNearestColor(255, 255,   0);
    unitRed         = Palette::findNearestColor(255,   0,   0);
    unitBlue        = Palette::findNearestColor(  3,   3, 255);
    unitDarkBlue    = Palette::findNearestColor(  0,   0, 128);
    unitLightGreen  = Palette::findNearestColor(163, 214, 159);
    unitGreen       = Palette::findNearestColor( 31, 166,  36);
    unitBlueGray    = Palette::findNearestColor( 43,  85, 131);
    unitDarkRed     = Palette::findNearestColor(150,   0,   0);
    unitBlack       = Palette::findNearestColor(  6,   2,   2);
    unitDarkGreen   = Palette::findNearestColor( 26,  88,  36);
    unitWhite       = Palette::findNearestColor(250, 253, 248);
    unitLightOrange = Palette::findNearestColor(246, 210,  99);
    unitOrange      = Palette::findNearestColor(205, 144,   9);
    unitGray        = Palette::findNearestColor(186, 186, 186);
    unitDarkGray    = Palette::findNearestColor( 99, 100, 102);

    // Grays
    black   = Palette::findNearestColor(  0,   0,   0, true);
    gray32  = Palette::findNearestColor( 32,  32,  32);
    gray64  = Palette::findNearestColor( 64,  64,  64);
    gray96  = Palette::findNearestColor( 96,  96,  96);
    gray128 = Palette::findNearestColor(128, 128, 128);
    gray160 = Palette::findNearestColor(160, 160, 160);
    gray192 = Palette::findNearestColor(192, 192, 192);
    gray224 = Palette::findNearestColor(224, 224, 224);
    white   = Palette::findNearestColor(255, 255, 255);

    // Dark Colors !FIXME!
    darkGray    = gray64;
    darkRed     = Palette::findNearestColor(128,   0,   0); // fixed
    darkGreen   = Palette::findNearestColor(  0, 128,   0); // fixed
    darkBlue    = Palette::findNearestColor(  0,   0, 128); // fixed
    darkYellow  = Palette::findNearestColor(255, 168,  18); // fixed
    darkOrange  = Palette::findNearestColor(255, 140,   0); // fixed
    darkMagenta = Palette::findNearestColor(148,   0, 211); // fixed
    darkCyan    = Palette::findNearestColor(  0, 206, 209); // fixed

    // Normal Colors
    gray    = gray128;
    red     = Palette::findNearestColor(255,   0,   0); // fixed
    green   = Palette::findNearestColor(  0, 255,   0); // fixed
    blue    = Palette::findNearestColor(  0,   0, 255); // fixed
    yellow  = Palette::findNearestColor(255, 255,   0); // fixed
    orange  = Palette::findNearestColor(255, 128,   0); // fixed
    magenta = Palette::findNearestColor(255,   0, 255); // fixed
    cyan    = Palette::findNearestColor(  0, 255, 255); // fixed
    //brown   = Palette::findNearestColor(128,  42,  42); // fixed
    brown   = Palette::findNearestColor(200,  80,  80); // new brown is lighter

    //tan                   210 180 140   0.8235 0.7059 0.5490
    tan            = Palette::findNearestColor(210, 180, 140); // fixed
    //chartreuse            127 255   0   0.4980 1.0000 0.0000
    chartreuse     = Palette::findNearestColor(127, 255,   0); // fixed
    //cobalt_green           61 145  64   0.2400 0.5700 0.2500
    cobaltGreen    = Palette::findNearestColor( 61, 145,  64); // fixed
    //emerald_green           0 201  87   0.0000 0.7900 0.3400
    emeraldGreen   = Palette::findNearestColor(  0, 201,  87); // fixed
    //forest_green           34 139  34   0.1333 0.5451 0.1333
    forestGreen    = Palette::findNearestColor( 34, 139,  34); // fixed
    //olive_green_dark       85 107  47   0.3333 0.4196 0.1843
    darkOliveGreen = Palette::findNearestColor( 85, 107,  47); // fixed
    //terre_verte            56  94  15   0.2200 0.3700 0.0600
    terreVerte     = Palette::findNearestColor( 56,  94,  15); // fixed

    // Normal Colors !FIXME!
    lightGray    = gray192;
    lightRed     = Palette::findNearestColor(255, 160, 122); // fixed
    lightGreen   = Palette::findNearestColor( 50, 205,  50); // fixed
    lightBlue    = Palette::findNearestColor(135, 206, 250); // fixed
    lightYellow  = Palette::findNearestColor(255, 255, 224); // fixed
    lightOrange  = Palette::findNearestColor(237, 145,  33); // fixed
    lightMagenta = Palette::findNearestColor(  0, 255, 255);
    lightCyan    = Palette::findNearestColor(224, 255, 255); // fixed
}
