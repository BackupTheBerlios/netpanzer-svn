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

#include "Exception.hpp"
#include "FileSystem.hpp"
#include "DigitText.hpp"

Surface         DigitText::charactersNormal;
Surface         DigitText::charactersNoSpace;
DIGIT_BLT_MODES DigitText::bltMode = NORMAL_TRANS;

// init
//--------------------------------------------------------------------------
// Purpose: Load all the characters into a surface of 128 frames.  Then the
//          characters can be accessed by changing the frame appropriately.
//--------------------------------------------------------------------------
void DigitText::init(const char *filename)
{
	// Make room for the 128 characters.
	charactersNormal.create(11, 17, 11, 128);

	// NOTE: Make sure the file size is 128 characters.
	char charfilename[] = "pics/chars11x17digit.raw";

	ReadFile* file = FileSystem::openRead(charfilename);

	for (int y = 0; y < charactersNormal.getPixY(); y++)
	{
		int yOffset = y * charactersNormal.getPixX();

		for (int curChar = 0; curChar < charactersNormal.getFrameCount(); curChar++)
		{
			charactersNormal.setFrame(curChar);
			if(!file->read(charactersNormal.mem + yOffset,
						charactersNormal.getPixX(), 1) != 1) {
				delete file;
				throw Exception("Error while loading font file '%s'.",
								charfilename);
			}
		}
	}

	delete file;
} // DigitText::initFont

//--------------------------------------------------------------------------
void DigitText::blt(const Surface &dest, const iXY &pos, const char *string, const PIX &color)
{
	if (bltMode == NORMAL_TRANS)
	{
		bltStringNormalTrans(dest, pos, string, color);
	}
	else if (bltMode == NO_SPACE_TRANS)
	{
		bltStringNoSpaceTrans(dest, pos, string, color);
	}
	else if (bltMode == NORMAL_SOLID)
	{
		bltStringNormalSolid(dest, pos, string);
	}
	else if (bltMode == NO_SPACE_SOLID)
	{
		bltStringNoSpaceSolid(dest, pos, string);
	}

} // DigitText::blt

// Blits the characters without the spaces in the text.
//--------------------------------------------------------------------------
void DigitText::bltCharNormalTrans(const Surface &dest, const iXY &pos, const char &character, const PIX &color)
{
#ifdef _DEBUG
	if (character > charactersNormal.getFrameCount())
	{
		assert(false);
	}
#endif

	charactersNormal.setFrame(character);
	charactersNormal.bltTransColor(dest, pos, color);

}

// Blits the characters without the spaces in the text.
//--------------------------------------------------------------------------
void DigitText::bltCharNormalSolid(const Surface &dest, const iXY &pos, const char &character)
{
#ifdef _DEBUG
	if (character > charactersNormal.getFrameCount())
	{
		assert(false);
	}
#endif

	charactersNormal.setFrame(character);
	charactersNormal.blt(dest, pos);

}

// Blits the characters without the spaces in the text.
//--------------------------------------------------------------------------
void DigitText::bltCharNoSpaceTrans(const Surface &dest, const iXY &pos, const char &character, const PIX &color)
{
#ifdef _DEBUG
	if (character > charactersNormal.getFrameCount())
	{
		assert(false);
	}
#endif

	charactersNoSpace.setFrame(character);
	charactersNoSpace.bltTransColor(dest, pos, color);

}

// Blits the characters without the spaces in the text.
//--------------------------------------------------------------------------
void DigitText::bltCharNoSpaceSolid(const Surface &dest, const iXY &pos, const char &character)
{
#ifdef _DEBUG
	if (character > charactersNormal.getFrameCount())
	{
		assert(false);
	}
#endif

	charactersNoSpace.setFrame(character);
	charactersNoSpace.blt(dest, pos);

} // end DigitText::bltCharNoSpaceSolid

// Blits the string without the spaces in the text.
//--------------------------------------------------------------------------
void DigitText::bltStringNormalTrans(const Surface &dest, const iXY &pos, const char *string, const PIX &color)
{
	for (int index = 0; string[index] != 0; index++)
	{
		bltCharNormalTrans(dest, iXY(pos.x + (index * charactersNormal.getPixX()), pos.y), string[index], color);
	}
}

// Blits the string without the spaces in the text.
//--------------------------------------------------------------------------
void DigitText::bltStringNormalSolid(const Surface &dest, const iXY &pos, const char *string)
{
	for (int index = 0; string[index] != 0; index++)
	{
		bltCharNormalSolid(dest, iXY(pos.x + (index * charactersNormal.getPixX()), pos.y), string[index]);
	}
}

// Blits the string without the spaces in the text.
//--------------------------------------------------------------------------
void DigitText::bltStringNoSpaceTrans(const Surface &dest, const iXY &pos, const char *string, const PIX &color)
{
	for (int index = 0; string[index] != 0; index++)
	{
		bltCharNoSpaceTrans(dest, iXY(pos.x + (index * charactersNormal.getPixX()), pos.y), string[index], color);
	}
}

// Blits the string without the spaces in the text.
//--------------------------------------------------------------------------
void DigitText::bltStringNoSpaceSolid(const Surface &dest, const iXY &pos, const char *string)
{
	for (int index = 0; string[index] != 0; index++)
	{
		bltCharNoSpaceSolid(dest, iXY(pos.x + (index * charactersNormal.getPixX()), pos.y), string[index]);
	}
}

// Sets which type of characters to blt.
//--------------------------------------------------------------------------
void DigitText::setBltMode(const DIGIT_BLT_MODES &mode)
{
	bltMode = mode;

} // end DigitText::setBltMode
