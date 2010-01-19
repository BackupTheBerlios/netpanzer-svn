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
#ifndef __Color_hpp__
#define __Color_hpp__

#include "SDL.h"
#include "Core/CoreTypes.hpp"

//---------------------------------------------------------------------------

class NamedColors
{
public:
	enum
	{
		black = 0,
		gray32,
		gray64,
		gray96,
		gray128,
		gray160,
		gray192,
		gray224,
		white,

		blue,
		brown,
		cyan,
		gray,
		green,
		magenta,
		orange,
		pink,
		red,
		yellow,

		darkBlue,
		darkBrown,
		darkCyan,
		darkGray,
		darkGreen,
		darkMagenta,
		darkOrange,
		darkPink,
		darkRed,
		darkYellow,

		lightBlue,
		lightBrown,
		lightCyan,
		lightGray,
		lightGreen,
		lightMagenta,
		lightOrange,
		lightPink,
		lightRed,
		lightYellow,

		tan,
		chartreuse,
		cobaltGreen,
		emeraldGreen,
		forestGreen,
		darkOliveGreen,
		terreVerte,

		unitAqua,
		unitYellow,
		unitRed,
		unitBlue,
		unitDarkBlue,
		unitLightGreen,
		unitGreen,
		unitBlueGray,
		unitDarkRed,
		unitBlack,
		unitDarkGreen,
		unitWhite,
		unitLightOrange,
		unitOrange,
		unitGray,
		unitDarkGray
	};
	static SDL_Color colors[];
};

class Color
{
public:
    // Color for netPanzer players.
    static IntColor unitAqua;
    static IntColor unitYellow;
    static IntColor unitRed;
    static IntColor unitBlue;
    static IntColor unitDarkBlue;
    static IntColor unitLightGreen;
    static IntColor unitGreen;
    static IntColor unitBlueGray;
    static IntColor unitDarkRed;
    static IntColor unitBlack;
    static IntColor unitDarkGreen;
    static IntColor unitWhite;
    static IntColor unitLightOrange;
    static IntColor unitOrange;
    static IntColor unitGray;
    static IntColor unitDarkGray;

    // Normal Colors.
    static IntColor black;
    static IntColor blue;
    static IntColor brown;
    static IntColor cyan;
    static IntColor gray;
    static IntColor green;
    static IntColor magenta;
    static IntColor orange;
    static IntColor pink;
    static IntColor red;
    static IntColor white;
    static IntColor yellow;

    // Color specifically for netPanzer.
    static IntColor tan;
    static IntColor chartreuse;
    static IntColor cobaltGreen;
    static IntColor emeraldGreen;
    static IntColor forestGreen;
    static IntColor darkOliveGreen;
    static IntColor terreVerte;

    // Dark Colors.
    static IntColor darkBlue;
    static IntColor darkBrown;
    static IntColor darkCyan;
    static IntColor darkGray;
    static IntColor darkGreen;
    static IntColor darkMagenta;
    static IntColor darkOrange;
    static IntColor darkPink;
    static IntColor darkRed;
    static IntColor darkYellow;

    // Light Colors.
    static IntColor lightBlue;
    static IntColor lightBrown;
    static IntColor lightCyan;
    static IntColor lightGray;
    static IntColor lightGreen;
    static IntColor lightMagenta;
    static IntColor lightOrange;
    static IntColor lightPink;
    static IntColor lightRed;
    static IntColor lightYellow;

    // Gray scales.
    static IntColor gray32;
    static IntColor gray64;
    static IntColor gray96;
    static IntColor gray128;
    static IntColor gray160;
    static IntColor gray192;
    static IntColor gray224;
private:
    friend class ScriptManager;
    static void registerScript(const char* table_name);
};

#endif // end __Color_hpp__

