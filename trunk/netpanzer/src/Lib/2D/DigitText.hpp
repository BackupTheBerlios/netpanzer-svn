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
#ifndef __DigitText_hpp__
#define __DigitText_hpp__

#include "Surface.hpp"
#include "iXY.hpp"
#include "Color.hpp"

enum DIGIT_BLT_MODES
{
    NORMAL_TRANS,
    NO_SPACE_TRANS,
    NORMAL_SOLID,
    NO_SPACE_SOLID,
};

class DigitText
{
private:
    // Storage space for the characters.
    static Surface charactersNormal;
    static Surface charactersNoSpace;

    // Normal character dimensions.
    enum { CHAR_PIXX = 11 };
    enum { CHAR_PIXY = 17 };

    // Dimensions without the spaces.
    enum { CHAR_NOSPACE_PIXX = 5 };
    enum { CHAR_NOSPACE_PIXY = 8 };

    // How are the characters to be blitted?
    static DIGIT_BLT_MODES bltMode;

    // Blits the characters without the spaces in the text.
    static void bltCharNormalTrans(Surface &dest, const iXY &pos, const char &character, const PIX &color);

    // Blits the characters without the spaces in the text.
    static void bltCharNormalSolid(Surface &dest, const iXY &pos, const char &character);

    // Blits the characters without the spaces in the text.
    static void bltCharNoSpaceTrans(Surface &dest, const iXY &pos, const char &character, const PIX &color);

    // Blits the characters without the spaces in the text.
    static void bltCharNoSpaceSolid(Surface &dest, const iXY &pos, const char &character);

    // Blits the string without the spaces in the text.
    static void bltStringNormalTrans(Surface &dest, const iXY &pos, const char *string, const PIX &color);

    // Blits the string without the spaces in the text.
    static void bltStringNormalSolid(Surface &dest, const iXY &pos, const char *string);

    // Blits the string without the spaces in the text.
    static void bltStringNoSpaceTrans(Surface &dest, const iXY &pos, const char *string, const PIX &color);

    // Blits the string without the spaces in the text.
    static void bltStringNoSpaceSolid(Surface &dest, const iXY &pos, const char *string);


public:
    DigitText()
    {}
    ~DigitText()
    {}

    // Loads in the characters for the font.
    static void init(const char *filename);

    // Calls the appropriate string blitting function depending on the current mode.
    static void blt(Surface &dest, const iXY &pos, const char *string, const PIX &color);
    static void blt(Surface &dest, const iXY &pos, const char *string)
    {
        // The color in this really doesn't matter.
        blt(dest, pos, string, Color::black);
    }

    // Sets which type of characters to blt.
    static void setBltMode(const DIGIT_BLT_MODES &mode);
}; // end DigitText

#endif // end __DigitText_hpp__
