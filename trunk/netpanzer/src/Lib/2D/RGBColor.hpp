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
#ifndef __RGBColor_hpp__
#define __RGBColor_hpp__

#include "codewiz.hpp"

//--------------------------------------------------------------------------
class RGBColor
{
public:
	BYTE red;   // 0-FF
	BYTE green; // 0-FF
	BYTE blue;  // 0-FF

	RGBColor() {}

	RGBColor(BYTE red, BYTE green, BYTE blue);
	
	// Returns an int between [0...255]
	int getBrightnessInt();

	// Returns an int between [+0.0...+1.0]
	int getBrightnessFloat();

	bool operator == (const RGBColor& other) const
	{
		return red == other.red &&
			   green == other.green &&
			   blue == other.blue;
	}
	bool operator != (const RGBColor& other) const
	{ return ! (*this == other); }
};

#endif // end __RGBColor_hpp__
