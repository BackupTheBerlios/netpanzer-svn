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
#include "2D/Surface.hpp"
#include "Util/Exception.hpp"
#include "2D/Color.hpp"
#include "ViewGlobals.hpp"

// createCenterText
//---------------------------------------------------------------------------
// Purpose: Creates a button with the name centered in the absolute horizontal
//          middle of the button.
//---------------------------------------------------------------------------
cButton::cButton(   const iXY &pos,
                    int xSize,
                    const char *nName,
                    const char *nToolTip,
                    ITEM_FUNC nLeftClickFunc)
{
    const unsigned GAP_SPACE = 6;

    int ySize = Surface::getFontHeight() + GAP_SPACE;
    Surface tempTopSurface(xSize, ySize, 3);

    // Find out the horizontal offset to put the button name on the button.
    int xOffset;
    if (nName != 0) xOffset = (xSize-(Surface::getTextLength(nName)))/2;
    else xOffset = xSize/2;

    // Make the unselected button
    tempTopSurface.fill(componentBodyColor);
    drawBorder(tempTopSurface, topLeftBorderColor, bottomRightBorderColor);
    tempTopSurface.bltString(xOffset, GAP_SPACE/2, nName, componentInActiveTextColor);

    // Make the mouse-over button
    tempTopSurface.setFrame(1);
    tempTopSurface.fill(componentBodyColor);
    drawBorder(tempTopSurface, topLeftBorderColor, bottomRightBorderColor);
    tempTopSurface.bltString(xOffset, GAP_SPACE/2, nName, componentFocusTextColor);

    // Make the selected button
    tempTopSurface.setFrame(2);
    tempTopSurface.fill(componentBodyColor);
    drawBorder(tempTopSurface, bottomRightBorderColor, topLeftBorderColor);
    tempTopSurface.bltString(xOffset+1, GAP_SPACE/2+1, nName, componentActiveTextColor);

    // Save the button name.
    setName(nName); assert(name != 0);

    // Save the button tool tip.
    toolTip = SDL_strdup(nToolTip); assert(toolTip != 0);

    // Save the bounds of the button.
    bounds = iRect(pos.x, pos.y, pos.x+tempTopSurface.getWidth(), pos.y+tempTopSurface.getHeight());

    // Save the function associated with the button.
    leftClickFunc = nLeftClickFunc;

    // Reset the button to unselected status.
    tempTopSurface.setFrame(0);

//    topSurface.pack(tempTopSurface);
    topSurface.copy(tempTopSurface);
    topSurface.setAllColorKey(0);

} // end createCenterText

// createBMP
//---------------------------------------------------------------------------
cButton::cButton(   const iXY &pos,
                    const char *imageName,
                    const char *nToolTip,
                    ITEM_FUNC nLeftClickFunc,
                    bool isBordered)
{
    Surface tempSurface;
    tempSurface.loadBMP(imageName);
    
    Surface tempTopSurface(tempSurface.getWidth(), tempSurface.getHeight(), 3);

    tempTopSurface.setFrame(0);
    tempSurface.blt(tempTopSurface, 0, 0);
    if (isBordered) {
        drawBorder(tempTopSurface, Color::darkGray, Color::darkGray);
    }

    tempTopSurface.setFrame(1);
    tempSurface.blt(tempTopSurface, 0, 0);
    if (isBordered) {
        drawBorder(tempTopSurface, Color::red, Color::red);
    }

    tempTopSurface.setFrame(2);
    tempSurface.blt(tempTopSurface, 0, 0);
    if (isBordered) {
        drawBorder(tempTopSurface, Color::darkGray, Color::darkGray);
    }

    tempTopSurface.setFrame(0);

    setName("TIL file");
    toolTip       = SDL_strdup(nToolTip); assert(toolTip != 0);
    bounds        = iRect(pos.x, pos.y, pos.x + tempTopSurface.getWidth(), pos.y + tempTopSurface.getHeight());
    leftClickFunc = nLeftClickFunc;

    topSurface.copy(tempTopSurface);
    topSurface.setAllColorKey(0);
} // end createBMP

// createSurface
//---------------------------------------------------------------------------
cButton::cButton(
    const iXY  &pos,
    Surface    &source,
    const char *nToolTip,
    ITEM_FUNC   nLeftClickFunc,
    bool        isBordered)
{
    Surface tempTopSurface(source.getWidth(), source.getHeight(), 3);

    tempTopSurface.setFrame(0);
    source.setFrame(0);
    source.blt(tempTopSurface, 0, 0);

    tempTopSurface.setFrame(1);
    if (source.getNumFrames() > 1) {
        source.setFrame(1);
        source.blt(tempTopSurface, 0, 0);

        if (isBordered) {
            drawBorder(tempTopSurface, Color::lightGreen, Color::darkGreen);
        }

    } else {
        source.blt(tempTopSurface, 0, 0);

        if (isBordered) {
            drawBorder(tempTopSurface, Color::lightRed, Color::darkRed);
        }
    }

    tempTopSurface.setFrame(2);
    if (source.getNumFrames() > 2) {
        source.setFrame(2);
        source.blt(tempTopSurface, 0, 0);
    } else {
        source.blt(tempTopSurface, 0, 0);

        if (isBordered) {
            drawBorder(tempTopSurface, Color::darkGreen, Color::lightGreen);
        }
    }

    tempTopSurface.setFrame(0);

    setName("Surface Single");
    toolTip       = SDL_strdup(nToolTip); assert(toolTip != 0);
    bounds        = iRect(pos.x, pos.y, pos.x + tempTopSurface.getWidth(), pos.y + tempTopSurface.getHeight());
    leftClickFunc = nLeftClickFunc;

    topSurface.copy(tempTopSurface);
    topSurface.setAllColorKey(0);
} // end createSurface

// createSurfaceSingle
//---------------------------------------------------------------------------
void cButton::createSurfaceSingle(
    iXY pos,
    Surface &source,
    const char *nToolTip,
    ITEM_FUNC nLeftClickFunc)
{
    Surface tempTopSurface(source.getWidth(), source.getHeight(), 3);

    tempTopSurface.setFrame(0);
    source.blt(tempTopSurface, 0, 0);

    tempTopSurface.setFrame(1);
    source.blt(tempTopSurface, 0, 0);
    drawBorder(tempTopSurface, Color::red, Color::darkRed);

    tempTopSurface.setFrame(2);
    source.blt(tempTopSurface, 0, 0);
    drawBorder(tempTopSurface, Color::green, Color::darkGreen);

    tempTopSurface.setFrame(0);

    setName("Surface Single");
    toolTip       = SDL_strdup(nToolTip); assert(toolTip != 0);
    bounds        = iRect(pos.x, pos.y, pos.x + tempTopSurface.getWidth(), pos.y + tempTopSurface.getHeight());
    leftClickFunc = nLeftClickFunc;

//    topSurface.pack(tempTopSurface);
    topSurface.copy(tempTopSurface);
    topSurface.setAllColorKey(0);

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
    name = SDL_strdup(buttonName);
    if (buttonName == 0) throw Exception("ERROR: Unable to allocate button name: %s", buttonName);
} // end SET NAME

void
cButton::drawBorder(Surface &viewArea, Uint32 topLeftColor, Uint32 bottomRightColor)
{
    viewArea.drawHLine(0 , 0, viewArea.getWidth() , topLeftColor);
    viewArea.drawVLine(0 , 0, viewArea.getHeight(), topLeftColor);
    viewArea.drawHLine(0 ,viewArea.getHeight()-1, viewArea.getWidth(), bottomRightColor);
    viewArea.drawVLine(viewArea.getWidth()-1, 0, viewArea.getHeight(), bottomRightColor);
}
