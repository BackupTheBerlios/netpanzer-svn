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

#include "SpecialButtonView.hpp"
#include "Surface.hpp"
#include "PackedSurface.hpp"
#include "UtilInterface.hpp"
#include "Desktop.hpp"
#include "Exception.hpp"
#include "WorldInputCmdProcessor.hpp"

// addSpecialButton
//---------------------------------------------------------------------------
void SpecialButtonView::addSpecialButton(const iXY &pos, const char *labelString, const char *fileString, ITEM_FUNC function)
{
    char strBuf[256];

    String prefix;

    // Check to see which type of button to load since we have multiple palettes.
    if (Desktop::getVisible("GameView")) {
        prefix = "inGame";
    } else {
        prefix = "inMenus";
    }

    sprintf(strBuf, "pics/backgrounds/menus/buttons/default/pak/%s%s.pak", (const char *) prefix, fileString);

    if (UtilInterface::getFileSize(strBuf) > 0) {
        PackedSurface tempPack;

        tempPack.load(strBuf);

        addButtonPackedSurface(pos, tempPack, "", function);

    } else {
        Surface buttonSurface;
        Surface tempSurface;

        sprintf(strBuf, "pics/backgrounds/menus/buttons/default/%sbutover.bmp", (const char *) prefix);

        // Create.
        tempSurface.loadBMP(strBuf);

        //-------------------
        //tempSurface.fill(0);
        //-------------------

        buttonSurface.create(tempSurface.getPixX(), tempSurface.getPixY(), tempSurface.getPixX(), 3);
        buttonSurface.fillAll(0);

        // Highlight.
        buttonSurface.setFrame(2);
        tempSurface.blt(buttonSurface);

        sprintf(strBuf, "pics/backgrounds/menus/buttons/default/%sbuton.bmp", (const char *) prefix);

        tempSurface.loadBMP(strBuf);

        //-------------------
        //tempSurface.fill(0);
        //-------------------

        buttonSurface.setFrame(1);
        tempSurface.blt(buttonSurface);

        // Unhighlight.
        sprintf(strBuf, "pics/backgrounds/menus/buttons/default/%sbutoff.bmp", (const char *) prefix);

        tempSurface.loadBMP(strBuf);

        //-------------------
        //tempSurface.fill(0);
        //-------------------

        buttonSurface.setFrame(0);
        tempSurface.bltTrans(buttonSurface);

        buttonSurface.setFrame(0);
        //buttonSurface.bltString(13, 5, labelString, Palette::findNearestColor(RGBColor(74, 116, 0)));
        buttonSurface.bltStringShadowed(13, 5, labelString, Color::yellow, Color::black);

        buttonSurface.setFrame(1);
        //buttonSurface.bltString(13, 5, labelString, Palette::findNearestColor(RGBColor(0, 255, 0)));
        buttonSurface.bltStringShadowed(13, 5, labelString, Color::white, Color::black);

        buttonSurface.setFrame(2);
        //buttonSurface.bltString(13, 5, labelString, Palette::findNearestColor(RGBColor(0, 255, 0)));
        buttonSurface.bltStringShadowed(13, 5, labelString, Color::white, Color::black);

        PackedSurface tempPack;

        tempPack.pack(buttonSurface);
        sprintf(strBuf, "pics/backgrounds/menus/buttons/default/pak/%s%s.pak", (const char *) prefix, fileString);
        tempPack.save(strBuf);

        addButtonPackedSurface(pos, tempPack, "", function);
    }

} // end SpecialButtonView::addSpecialButton

// mouseMove
//--------------------------------------------------------------------------
void SpecialButtonView::mouseMove(const iXY & prevPos, const iXY &newPos)
{
    RMouseHackView::mouseMove(prevPos, newPos);

    if (!mouse.getButtonMask() && Desktop::getFocus() != this) {
        Desktop::setFocusView(this);

        COMMAND_PROCESSOR.closeSelectionBox();
    }

} // end SpecialButtonView::mouseMove

// doActivate
//--------------------------------------------------------------------------
void SpecialButtonView::doActivate()
{
} // end SpecialButtonView::doActivate
