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


#include "FontSystem2D.hpp"
#include "TimerInterface.hpp"

static FontSystem2D theRealZFontSystem2D(iXY(0, 0), "theRealZFontSystem2D", screen, 0.0f, Color::white);
FontSystem2D *const FontSystem2D::zFontSystem2D = &theRealZFontSystem2D;
unsigned FontSystem2D::fontSystemCount = 0;
float FontSystem2D::scrollSpeed = 0.005f;


//---------------------------------------------------------------------------
/*FontSystem2D::FontSystem2D(iXY pos, const char *string, const Surface &dest, float delay, PIX color)
{
} // end FontSystem2D::FontSystem2D
*/

void FontSystem2D::create(iXY pos, const char *string, const Surface &dest, float delay, PIX color)
{
	if (string == 0) { return; }
	//if (dest.getDoesExist == false) { return; }

	reset();
	FontSystem2D::pos          = pos;
	FontSystem2D::delay        = delay;
	FontSystem2D::destSurface  = dest;
	FontSystem2D::string       = strdup(string);
	FontSystem2D::stringLength = strlen(string);

	if (this == zFontSystem2D)
	{
		prev = next = zFontSystem2D;
	}	else
		{
			prev = next = 0;
			insertMe();
		}
}

void FontSystem2D::create(iXY pos, const char *string, const Surface &dest, float delay, const ColorTable &colrorTable)
{
	if (string == 0) { return; }
	//if (dest.getDoesExist == false) { return; }

	reset();
	FontSystem2D::pos          = pos;
	FontSystem2D::delay        = delay;
	FontSystem2D::destSurface  = dest;
	FontSystem2D::string       = strdup(string);
	FontSystem2D::stringLength = strlen(string);

	if (this == zFontSystem2D)
	{
		prev = next = zFontSystem2D;
	}	else
		{
			prev = next = 0;
			insertMe();
		}
}

// reset
//---------------------------------------------------------------------------
void FontSystem2D::reset()
{
	isAlive         = true;
	pos             = iXY(0, 0);
	age             = 0.0f;
	lifetime        = 0.0f;
	delay           = 0.0f;
	groupWait       = 0.0f;
	charCount       = 0;
	string          = 0;
	stringLength    = 0;

} // end FontSystem2D::reset

// ~FontSystem2D
//---------------------------------------------------------------------------
FontSystem2D::~FontSystem2D()
{
	removeMe();

} // end FontSystem2D::~FontSystem2D

// insertMe
//---------------------------------------------------------------------------
// Purpose: Inserts a new system into the list.
//---------------------------------------------------------------------------
void FontSystem2D::insertMe()
{
	// If we're inserting, we should not already be in the list...
	assert(prev == 0);
	assert(next == 0);

	// Insert me into the list
	prev                = zFontSystem2D;
	next                = zFontSystem2D->next;
	zFontSystem2D->next = this;
	next->prev          = this;
	fontSystemCount++;

} // end FontSystem2D::insertMe

// removeMe
//---------------------------------------------------------------------------
// Purpose: Removes the system from the list.
//---------------------------------------------------------------------------
void FontSystem2D::removeMe()
{
	if (string != 0)
	{
		::free(string);
	}

	if (prev != 0) prev->next = next;
	if (next != 0) next->prev = prev;

	prev = next = this;

	fontSystemCount--;

} // end FontSystem2D::removeMe

// simAll
//---------------------------------------------------------------------------
void FontSystem2D::simAll()
{
	// Go through and simulate all the systems.
	FontSystem2D *e = zFontSystem2D->next;
	FontSystem2D *nextPtr;

	while (e != zFontSystem2D)
	{
		nextPtr = e->next;
		e->sim();
		e = nextPtr;
	}

} // end FontSystem2D::simAll

// drawAll
//---------------------------------------------------------------------------
void FontSystem2D::drawAll()
{
	// Go through and draw all the systems.
	FontSystem2D *e = zFontSystem2D->next;
	FontSystem2D *nextPtr;

	while (e != zFontSystem2D)
	{
		nextPtr = e->next;
		e->draw();
		e = nextPtr;
	}

} // end FontSystem2D::drawAll

// removeAll
//---------------------------------------------------------------------------
void FontSystem2D::removeAll()
{
	// Go through and delete all the systems.
	FontSystem2D *e = zFontSystem2D->next;
	FontSystem2D *nextPtr;

	while (e != zFontSystem2D)
	{
		nextPtr = e->next;
		
		delete e;

		e = nextPtr;
	}

} // end FontSystem2D::removeAll

//---------------------------------------------------------------------------
void FontSystem2D::sim()
{
	float dt = TimerInterface::getTimeSlice();
	
	age       += dt;
	groupWait += dt;

	if (charCount < stringLength)	
	{
		charCount = (int) (groupWait / delay);
	}

	if (charCount > stringLength)
	{
		charCount = stringLength;
	}

	//if (groupWait >= delay && charCount < stringLength)
	//{
	//	charCount++;
	//	groupWait = 0.0f;
	//}

	if (!isAlive)
	{
		delete this;
		return;
	}

} // end FontSystem2D::sim()

//---------------------------------------------------------------------------
void FontSystem2D::draw()
{
	if (!destSurface.getDoesExist())
	{
		isAlive = false;
		return;
	}

	char strBuf[256];
	assert(strlen(string) < 256);

	strncpy(strBuf, string, charCount);
	strBuf[charCount] = '\0';


	//destSurface.bltString(pos, strBuf, Color::white);
	destSurface.bltStringVGradient(pos, strBuf, Palette::gradientWhite2Green);

} // end FontSystem2D::draw()
