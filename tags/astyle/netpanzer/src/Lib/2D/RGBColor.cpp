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
#include "RGBColor.hpp"


RGBColor::RGBColor(BYTE red, BYTE green, BYTE blue)
{
    RGBColor::red   = red;
    RGBColor::green = green;
    RGBColor::blue  = blue;
} // end RGBColor::RGBColor

// getBrightnessInt
//--------------------------------------------------------------------------
// Purpose: Returns an int between [0...255]
//--------------------------------------------------------------------------
int RGBColor::getBrightnessInt()
{
    return (int) (255.0f / 768.0f * float(red + blue + green));
}

// getBrightnessFloat
//--------------------------------------------------------------------------
// Purpose: Returns an int between [+0.0...+1.0]
//--------------------------------------------------------------------------
int RGBColor::getBrightnessFloat()
{
    return (int) (float(red + blue + green) / 768.0f);
}
