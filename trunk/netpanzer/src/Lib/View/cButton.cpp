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
#include "Exception.hpp"
#include "Color.hpp"
#include "ViewGlobals.hpp"


void cButton::createPacked(const iXY &pos, PackedSurface &source, const char *toolTip, ITEM_FUNC leftClickFunc)
{
    Surface tempTopSurface(source.getPixX(), source.getPixY(), source.getPixX(), source.getFrameCount());

    for (int i = 0; i < tempTopSurface.getFrameCount(); i++) {
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
    tempSurface.loadTIL(imageName);

    tempTopSurface.create(tempSurface.getPix(), tempSurface.getPix().x, 3);

    tempTopSurface.setFrame(0);
    tempSurface.blt(tempTopSurface);
    if (isBordered) {
        tempTopSurface.drawButtonBorder(Color::darkGray, Color::darkGray);
    }

    tempTopSurface.setFrame(1);
    tempSurface.blt(tempTopSurface);
    if (isBordered) {
        tempTopSurface.drawButtonBorder(Color::red, Color::red);
    }

    tempTopSurface.setFrame(2);
    tempSurface.blt(tempTopSurface);
    if (isBordered) {
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
    if (source.getFrameCount() > 1) {
        source.setFrame(1);
        source.blt(tempTopSurface);

        if (isBordered) {
            tempTopSurface.drawButtonBorder(Color::lightGreen, Color::darkGreen);
        }

    } else {
        source.blt(tempTopSurface);

        if (isBordered) {
            tempTopSurface.drawButtonBorder(Color::lightRed, Color::darkRed);
        }
    }

    tempTopSurface.setFrame(2);
    if (source.getFrameCount() > 2) {
        source.setFrame(2);
        source.blt(tempTopSurface);
    } else {
        source.blt(tempTopSurface);

        if (isBordered) {
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
    if (buttonName == 0) throw Exception("ERROR: Unable to allocate button name: %s", buttonName);
} // end SET NAME

