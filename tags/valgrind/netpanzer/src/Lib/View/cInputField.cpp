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
#include "cInputField.hpp"
#include "TimerInterface.hpp"


////////////////////////////////////////////////////////////////////////////
// cInputFieldString definitions.
////////////////////////////////////////////////////////////////////////////

// init
//--------------------------------------------------------------------------
void cInputFieldString::init(const char *string, int maxCharCount)
{
	cInputFieldString::maxCharCount = maxCharCount;
	assert(maxCharCount > 0);

	cInputFieldString::string = new char [maxCharCount + 1];
	if (this->string == 0)
	{
		throw Exception("ERROR: Unable to allocate cInputFieldString string.");
	}

	if (strlen(string) > 0 && strlen(string) < (size_t) maxCharCount)
	{
		strcpy(cInputFieldString::string, string);
	} else
	{
		cInputFieldString::string[0] = '\0';
	}

} // end init

// setString
void cInputFieldString::setString(const char *string)
{
	strncpy(this->string, string, maxCharCount);

} // end setString

// reset
//--------------------------------------------------------------------------
void cInputFieldString::reset()
{
	string       = 0;
	maxCharCount = 0;
} // end reset


////////////////////////////////////////////////////////////////////////////
// cInputField definitions.
////////////////////////////////////////////////////////////////////////////

// reset
//--------------------------------------------------------------------------
void cInputField::reset()
{
	cursorPos          = 0;
	pos                = iXY(0, 0);
	maxCharCount       = 0;
	destString         = 0;
	excludedCharacters = 0;
} // end reset

// setPos
//--------------------------------------------------------------------------
void cInputField::setPos(iXY pos)
{ 
	this->pos  = pos;
	bounds.min = pos;
} // end setPos

// setInputFieldString
// Purpose: Make sure you call setPos before you call this so the 
//          bounds get set correctly.
//--------------------------------------------------------------------------
void cInputField::setInputFieldString(cInputFieldString *string)
{
	//assert(string->string != 0);
	assert(string->maxCharCount > 0);

	this->destString   = string->string;
	this->maxCharCount = string->maxCharCount;

	iXY size;
	size.x = maxCharCount * CHAR_XPIX + 8;
	size.y = CHAR_YPIX + 4;

	bounds.max = bounds.min + size;
	
	inputFieldSurface.create(size, size.x, 1);
	inputFieldSurface.fill(Color::black);

} // end setInputFieldString

// setExcludedCharacters
//--------------------------------------------------------------------------
void cInputField::setExcludedCharacters(const char *excludedCharacters)
{
	this->excludedCharacters = new char [strlen(excludedCharacters) + 1];
	if (this->excludedCharacters == 0)
	{
		throw Exception("ERROR: Unable to allocate cInputField excludedCharacters.");
	}

	strcpy(this->excludedCharacters, excludedCharacters);
} // setExcludedCharacters

// addChar
//--------------------------------------------------------------------------
void cInputField::addChar(int newChar)
{
	// Check if the character should be excluded.
	if (strchr(excludedCharacters, newChar) == 0)
	{
		// Add the character.
		destString[cursorPos] = newChar;

		int length = strlen(destString) + 1;
		if (length <= maxCharCount)
		{
			destString[length] = '\0';
		} else
		{
			destString[maxCharCount] = '\0';
		}

		assert(cursorPos < maxCharCount);
		
		if (cursorPos < maxCharCount - 1)
		{
			cursorPos++;
		} 
	}
} // addChar

// addExtendedChar
//--------------------------------------------------------------------------
void cInputField::addExtendedChar(int newExtendedChar)
{
	// Process the extendedChar accordingly.
	switch (newExtendedChar)
	{
		case SDLK_RETURN:
		{
		} break;
		
		case SDLK_HOME:
		{
			cursorPos = 0;
		} break;
		
		case SDLK_LEFT:
		{
			if(--cursorPos < 0)
			{
				cursorPos = 0;
			}
		} break;
		
		case SDLK_RIGHT:
		{
			int length = strlen(destString);
			if(++cursorPos > length)
			{
				if (cursorPos > maxCharCount)
				{
					cursorPos = maxCharCount - 1;
				} else
				{
					cursorPos = length;
				}
			}
		} break;
		
		case SDLK_END:
		{
			cursorPos = strlen(destString);

			if (cursorPos > maxCharCount)
			{
				cursorPos = maxCharCount - 1;
			}
		} break;
		
		case SDLK_INSERT:
		{
		} break;

		case SDLK_DELETE:
		{
			if (cursorPos == strlen(destString)) { break; }

			memcpy(destString + cursorPos, destString + cursorPos + 1, strlen(destString + cursorPos + 1) + 1);
		} break;
		
		case SDLK_BACKSPACE:
		{
			if (cursorPos >= 1)
			{
				int byteCount = strlen(destString + cursorPos);
				
				// Only do this if we are not at the end of the string.
				if (byteCount > 0)
				{
					memcpy(destString + cursorPos - 1, destString + cursorPos, byteCount);
				}

				cursorPos--;
				destString[cursorPos + byteCount] = '\0';
			}
			
		} break;

		default: break;
	}

} // addExtendedChar

// draw
//--------------------------------------------------------------------------
void cInputField::draw(const Surface &dest)
{
	checkCursor();

	inputFieldSurface.fill(Color::black);
	inputFieldSurface.drawButtonBorder(Color::white, Color::gray64);
	inputFieldSurface.bltString(4, 2, destString, Color::white);
	inputFieldSurface.blt(dest, pos);
} // draw

// drawHighlighted
//--------------------------------------------------------------------------
void cInputField::drawHighlighted(const Surface &dest)
{
	checkCursor();

	inputFieldSurface.fill(Color::black);
	inputFieldSurface.drawButtonBorder(Color::darkGray, Color::white);
	inputFieldSurface.bltStringShadowed(4, 2, destString, Color::white, Color::black);

	static float timeForBlink = 0.0f;
	if ((timeForBlink += TimerInterface::getTimeSlice()) > 0.25f)
	{
		if (timeForBlink > 0.50f)
		{
			timeForBlink = 0.0f;
		}
	} else
	{
		if (cursorPos >= maxCharCount)
		{
			inputFieldSurface.bltString(((cursorPos - 1) * CHAR_XPIX) + 4, 2, "_", Color::red);
		} else
		{
			inputFieldSurface.bltString(cursorPos * CHAR_XPIX + 4, 2, "_", Color::red);
		}
	}

	inputFieldSurface.blt(dest, pos);
} // drawHighlighted

// checkCursor
//--------------------------------------------------------------------------
// Purpose: Make sure the cursor stays at the end of the string if the cursor
//          is past the string.
//--------------------------------------------------------------------------
void cInputField::checkCursor()
{
	if (cursorPos > int(strlen(destString)))
	{
		cursorPos = strlen(destString);
	}
} // end checkCursor
