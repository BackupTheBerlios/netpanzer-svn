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
#include "cButton.hpp"
#include "Surface.hpp"
#include "ViewGlobals.hpp"


void cButton::createPacked(const iXY &pos, PackedSurface &source, const char *toolTip, ITEM_FUNC leftClickFunc)
{
	Surface tempTopSurface(source.getPixX(), source.getPixY(), source.getPixX(), source.getFrameCount());

	for (int i = 0; i < tempTopSurface.getFrameCount(); i++)
	{
		tempTopSurface.setFrame(i);
		tempTopSurface.fill(0);
		source.setFrame(i);
		source.blt(tempTopSurface, 0, 0);
	}

	topSurface.pack(tempTopSurface);

	cButton::toolTip       = strdup(toolTip); assert(toolTip != 0);
	cButton::bounds        = iRect(pos.x, pos.y, pos.x + tempTopSurface.getPix().x, pos.y + tempTopSurface.getPix().y);
	cButton::leftClickFunc = leftClickFunc;
}

// CREATE
//---------------------------------------------------------------------------
void cButton::create(iXY pos, const char *nName, const char *nToolTip,
										ITEM_FUNC nLeftClickFunc)
{
	Surface tempTopSurface;

	const unsigned GAP_SPACE = 6;
	int xSize = strlen(nName) * CHAR_XPIX + GAP_SPACE + 1;
	int ySize = CHAR_YPIX + GAP_SPACE;
	tempTopSurface.create(iXY(xSize, ySize), xSize, 3);

	// Make the unselected button
	tempTopSurface.fill(componentBodyColor);
	tempTopSurface.drawButtonBorder(iRect(0, 0, xSize, ySize), topLeftBorderColor, bottomRightBorderColor);
	tempTopSurface.bltString(GAP_SPACE / 2 + 1, GAP_SPACE / 2, nName, componentInActiveTextColor);

	// Make the mouse-over button
	tempTopSurface.setFrame(1);
	tempTopSurface.fill(componentBodyColor);
	tempTopSurface.drawButtonBorder(iRect(0, 0, xSize, ySize), topLeftBorderColor, bottomRightBorderColor);
	tempTopSurface.bltString(GAP_SPACE / 2 + 1, GAP_SPACE / 2, nName, componentFocusTextColor);

	// Make the selected button
	tempTopSurface.setFrame(2);
	tempTopSurface.fill(componentBodyColor);
	tempTopSurface.drawButtonBorder(iRect(0, 0, xSize, ySize), bottomRightBorderColor, topLeftBorderColor);
	tempTopSurface.bltString(GAP_SPACE / 2 + 2, GAP_SPACE / 2 + 1, nName, componentActiveTextColor);

	setName(nName); assert(name != 0);
	toolTip       = strdup(nToolTip); assert(toolTip != 0);
	bounds        = iRect(pos.x, pos.y, pos.x + tempTopSurface.getPix().x, pos.y + tempTopSurface.getPix().y);
	leftClickFunc = nLeftClickFunc;

	tempTopSurface.setFrame(0);

	topSurface.pack(tempTopSurface);

} // end CREATE

// createCenterText
//---------------------------------------------------------------------------
// Purpose: Creates a button with the name centered in the absolute horizontal
//          middle of the button.
//---------------------------------------------------------------------------
void cButton::createCenterText(iXY pos, 
                              int xSize, 
                              const char *nName, 
                              const char *nToolTip,
                              ITEM_FUNC nLeftClickFunc)
{
	Surface tempTopSurface;

	const unsigned GAP_SPACE = 6;

	int ySize = CHAR_YPIX+GAP_SPACE;
	tempTopSurface.create(iXY(xSize, ySize), xSize, 3);

	// Find out the horizontal offset to put the button name on the button.
	int xOffset;
	if (nName != 0) xOffset = (xSize-(strlen(nName)*CHAR_XPIX))/2;
	else xOffset = xSize/2;

	// Make the unselected button
	tempTopSurface.fill(componentBodyColor);
	tempTopSurface.drawButtonBorder(topLeftBorderColor, bottomRightBorderColor);
	tempTopSurface.bltString(xOffset, GAP_SPACE/2, nName, componentInActiveTextColor);

	// Make the mouse-over button
	tempTopSurface.setFrame(1);
	tempTopSurface.fill(componentBodyColor);
	tempTopSurface.drawButtonBorder(topLeftBorderColor, bottomRightBorderColor);
	tempTopSurface.bltString(xOffset, GAP_SPACE/2, nName, componentFocusTextColor);

	// Make the selected button
	tempTopSurface.setFrame(2);
	tempTopSurface.fill(componentBodyColor);
	tempTopSurface.drawButtonBorder(bottomRightBorderColor, topLeftBorderColor);
	tempTopSurface.bltString(xOffset+1, GAP_SPACE/2+1, nName, componentActiveTextColor);

	// Save the button name.
	setName(nName); assert(name != 0);

	// Save the button tool tip.
	toolTip = strdup(nToolTip); assert(toolTip != 0);
	
	// Save the bounds of the button.
	bounds = iRect(pos.x, pos.y, pos.x+tempTopSurface.getPix().x, pos.y+tempTopSurface.getPix().y);
	
	// Save the function associated with the button.
	leftClickFunc = nLeftClickFunc;

	// Reset the button to unselected status.
	tempTopSurface.setFrame(0);

	topSurface.pack(tempTopSurface);

} // end createCenterText

// createTIL
//---------------------------------------------------------------------------
void cButton::createTIL(iXY pos, 
					const char *imageName, 
                    const char *nToolTip, 
					ITEM_FUNC nLeftClickFunc,
					bool isBordered)
{
	Surface tempTopSurface;

	Surface tempSurface;
	if(!tempSurface.loadTIL(imageName))
	{
		FUBAR("ERROR: Unable to open button TIL file: %s", imageName);
	}

	tempTopSurface.create(tempSurface.getPix(), tempSurface.getPix().x, 3);

	tempTopSurface.setFrame(0);
	tempSurface.blt(tempTopSurface);
	if (isBordered)
	{
		tempTopSurface.drawButtonBorder(Color::darkGray, Color::darkGray);
	}

	tempTopSurface.setFrame(1);
	tempSurface.blt(tempTopSurface);
	if (isBordered)
	{
		tempTopSurface.drawButtonBorder(Color::red, Color::red);
	}
	
	tempTopSurface.setFrame(2);
	tempSurface.blt(tempTopSurface);
	if (isBordered)
	{
		tempTopSurface.drawButtonBorder(Color::darkGray, Color::darkGray);
	}

	tempTopSurface.setFrame(0);

	setName("TIL file");
	toolTip       = strdup(nToolTip); assert(toolTip != 0);
	bounds        = iRect(pos.x, pos.y, pos.x + tempTopSurface.getPix().x, pos.y + tempTopSurface.getPix().y);
	leftClickFunc = nLeftClickFunc;

	topSurface.pack(tempTopSurface);

} // end createTIL

// createSurface
//---------------------------------------------------------------------------
void cButton::createSurface(
							iXY         pos, 
							Surface    &source, 
							const char *nToolTip, 
							ITEM_FUNC   nLeftClickFunc,
							bool        isBordered)
{
	Surface tempTopSurface;

	tempTopSurface.create(source.getPix(), source.getPix().x, 3);

	tempTopSurface.setFrame(0);
	source.setFrame(0);
	source.blt(tempTopSurface);

	tempTopSurface.setFrame(1);
	if (source.getFrameCount() > 1)
	{
		source.setFrame(1);
		source.blt(tempTopSurface);
		
		if (isBordered)
		{
			tempTopSurface.drawButtonBorder(Color::lightGreen, Color::darkGreen);
		}

	} else
	{
		source.blt(tempTopSurface);
	
		if (isBordered)
		{
			tempTopSurface.drawButtonBorder(Color::lightRed, Color::darkRed);
		}
	}
	
	tempTopSurface.setFrame(2);
	if (source.getFrameCount() > 2)
	{
		source.setFrame(2);
		source.blt(tempTopSurface);
	} else
	{
		source.blt(tempTopSurface);

		if (isBordered)
		{
			tempTopSurface.drawButtonBorder(Color::darkGreen, Color::lightGreen);
		}
	}

	tempTopSurface.setFrame(0);

	setName("Surface Single");
	toolTip       = strdup(nToolTip); assert(toolTip != 0);
	bounds        = iRect(pos.x, pos.y, pos.x + tempTopSurface.getPix().x, pos.y + tempTopSurface.getPix().y);
	leftClickFunc = nLeftClickFunc;

	topSurface.pack(tempTopSurface);

} // end createSurface

// createSurfaceSingle
//---------------------------------------------------------------------------
void cButton::createSurfaceSingle(
							iXY pos, 
							Surface &source, 
							const char *nToolTip, 
							ITEM_FUNC nLeftClickFunc)
{
	Surface tempTopSurface;

	tempTopSurface.create(source.getPix(), source.getPix().x, 3);

	tempTopSurface.setFrame(0);
	source.blt(tempTopSurface);
	//tempTopSurface.drawButtonBorder(Color::white, Color::gray128);

	tempTopSurface.setFrame(1);
	source.blt(tempTopSurface);
	tempTopSurface.drawButtonBorder(Color::red, Color::darkRed);
	
	tempTopSurface.setFrame(2);
	source.blt(tempTopSurface);
	tempTopSurface.drawButtonBorder(Color::green, Color::darkGreen);

	tempTopSurface.setFrame(0);

	setName("Surface Single");
	toolTip       = strdup(nToolTip); assert(toolTip != 0);
	bounds        = iRect(pos.x, pos.y, pos.x + tempTopSurface.getPix().x, pos.y + tempTopSurface.getPix().y);
	leftClickFunc = nLeftClickFunc;

	topSurface.pack(tempTopSurface);

} // end createSurfaceSingle

// create
//---------------------------------------------------------------------------
void cButton::create(iXY pos, 
                    const char *imageName0, 
                    const char *imageName1,
                    const char *imageName2, 
					const char *nName,
                    const char *nToolTip, 
                    ITEM_FUNC nLeftClickFunc)
{
	Surface tempTopSurface;

	tempTopSurface.create(iXY(32, 32), 32, 3);
	tempTopSurface.setFrame(0);
	tempTopSurface.loadRAW(imageName0, false);
	tempTopSurface.setFrame(1);
	tempTopSurface.loadRAW(imageName1, false);
	tempTopSurface.setFrame(2);
	tempTopSurface.loadRAW(imageName2, false);
	setName(nName); assert(name != 0);
	toolTip       = strdup(nToolTip); assert(toolTip != 0);
	bounds        = iRect(pos.x, pos.y, pos.x+tempTopSurface.getPix().x, pos.y+tempTopSurface.getPix().y);
	leftClickFunc = nLeftClickFunc;

	topSurface.pack(tempTopSurface);

} // end create

// create
//---------------------------------------------------------------------------
// Purpose: Adds a button which has 2 animated surfaces as the button.
//---------------------------------------------------------------------------
void cButton::create(iXY pos, ANIMATED_BUTTON_TYPE_PCX topButton, 
                    ANIMATED_BUTTON_TYPE_PCX bottomButton,
							      const char *nToolTip, ITEM_FUNC nLeftClickFunc, 
                    ITEM_FUNC nRightClickFunc)
{
	Surface tempTopSurface;

	tempTopSurface.create(iXY(topButton.size.x, topButton.size.y), topButton.size.x, topButton.numFrames);
	tempTopSurface.extractPCX(topButton.filename, topButton.numColumns, topButton.gapSpace);
	tempTopSurface.scale(iXY(topButton.scale.x, topButton.scale.y));
	tempTopSurface.setFPS((int) topButton.fps);
/*
	bottomSurface.create(iXY(bottomButton.size.x, bottomButton.size.y), bottomButton.size.x, bottomButton.numFrames);
	bottomSurface.extractPCX(bottomButton.filename, bottomButton.numColumns, bottomButton.gapSpace);
	bottomSurface.scale(iXY(bottomButton.scale.x, bottomButton.scale.y));
	bottomSurface.setFPS(bottomButton.fps);

	// Make sure the button pics are the same size.
	assert(tempTopSurface.getPix().x == bottomSurface.getPix().x &&
	tempTopSurface.getPix().y == bottomSurface.getPix().y);
*/
	toolTip = strdup(nToolTip); assert(toolTip != 0);
	bounds  = iRect(pos.x, pos.y, pos.x+tempTopSurface.getPix().x, pos.y+tempTopSurface.getPix().y);
	
	leftClickFunc  = nLeftClickFunc;
	rightClickFunc = nRightClickFunc;

	topSurface.pack(tempTopSurface);

} // end create

// RESET
//---------------------------------------------------------------------------
void cButton::reset()
{
	toolTip        = 0;
	name           = 0;
	isSelected     = false;
	leftClickFunc  = 0;
	rightClickFunc = 0;
} // end RESET

// SET NAME
//---------------------------------------------------------------------------
void cButton::setName(const char *buttonName)
{
	name = strdup(buttonName);
	if (buttonName == 0) FUBAR("ERROR: Unable to allocate button name: %s", buttonName);
} // end SET NAME

// createPCX
//---------------------------------------------------------------------------
void cButton::createPCX(iXY pos, 
                       const char *filename, 
                       const char *nToolTip,
                       ITEM_FUNC nLeftClickFunc)
{
	//SURFACE tempSurface;
	//tempSurface.loadPCX(filename);
	//tempTopSurface.loadPCX(filename);
	
	//iXY nSize(tempTopSurface.getPix());

	Surface tempTopSurface;

	tempTopSurface.create(22, 22, 22, 3);

	// Make the unselected button
	tempTopSurface.setFrame(0);
	tempTopSurface.loadPCX(filename, false, 0);
	tempTopSurface.drawButtonBorder(Color::white, Color::gray64);

	// Make the mouse-over button
	tempTopSurface.setFrame(1);
	tempTopSurface.loadPCX(filename, false, 0);
	tempTopSurface.drawButtonBorder(Color::red, Color::red);

	// Make the selected button
	tempTopSurface.setFrame(2);
	tempTopSurface.loadPCX(filename, false, 0);
	tempTopSurface.drawButtonBorder(Color::gray64, Color::white);

	setName(filename); assert(name != 0);
	toolTip       = strdup(nToolTip); assert(toolTip != 0);
	bounds        = iRect(pos.x, pos.y, pos.x+tempTopSurface.getPix().x, pos.y+tempTopSurface.getPix().y);
	leftClickFunc = nLeftClickFunc;

	tempTopSurface.setFrame(0);

	topSurface.pack(tempTopSurface);

} // end createPCX
