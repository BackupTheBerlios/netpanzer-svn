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

#include "FlagSelectionView.hpp"
#include "GameView.hpp"
#include "GameViewGlobals.hpp"
#include "Util/Exception.hpp"
#include "GameConfig.hpp"

Surface playerFlag;
int     playerFlagSelected = 0;


// FlagSelectionView
//---------------------------------------------------------------------------
FlagSelectionView::FlagSelectionView() : RMouseHackView()
{
    setSearchName("FlagSelectionView");
    setTitle("Flag Selection");
    setSubTitle("");

    setAllowResize(false);
    setAllowMove(false);
    setVisible(false);

    // The thirty is to give more room to the map selectior view.
    moveTo(bodyTextRect.min.x + bodyTextRect.getSizeX() / 2 + 10 + 30,
           bodyTextRect.min.y + 50);

    // Load the flag images.
    //flags.loadAllBMPInDirectory("pics/flags/");

    resizeClientArea(bodyTextRect.getSizeX() / 2 - 10 - 30, 108);

    // Define the scrollBar fot this view.
    //scrollBar = new ScrollBar(HORIZONTAL, 0, 1, 0, flags.getFrameCount());
    //if (scrollBar == 0)
    //{
    //	throw Exception("ERROR: Unable to allocate scrollBar.");
    //}
    //
    //scrollBar->setViewableAmount(getClientRect().getSizeX() / flags.getWidth());
    //
    //add(scrollBar);

    init();

} // end FlagSelectionView::FlagSelectionView

// init
//---------------------------------------------------------------------------
void FlagSelectionView::init()
{
    if (playerFlag.loadAllBMPInDirectory("pics/flags/") <= 0)
        throw Exception("Couldn't find flags for menu in '%s'.",
                        "pics/flags/");
    playerFlag.mapFromPalette("netp");

    iXY flagStartOffset(BORDER_SPACE, BORDER_SPACE * 2 + playerFlag.getHeight());

    int xOffset = playerFlag.getWidth() + 2;
    int yOffset = playerFlag.getHeight() + 2;

    int x = flagStartOffset.x;
    int y = flagStartOffset.y;

    for (unsigned int i = 0; i < playerFlag.getNumFrames(); i++) {
        playerFlag.setFrame(i);
        //playerFlag.drawButtonBorder(Color::white, Color::gray64);

        addButtonSurfaceSingle(iXY(x, y), playerFlag, "", 0);

        x += xOffset;

        if (x > flagStartOffset.x + getClientRect().getSizeX() - BORDER_SPACE - (int)playerFlag.getWidth()) {
            x = flagStartOffset.x;
            y += yOffset;
        }
    }

    playerFlag.setFrame(0);
    if(gameconfig->playerflag.isDefaultValue()) {
        // new player, no flag...
        gameconfig->playerflag=rand()%playerFlag.getNumFrames();
    }
    playerFlagSelected = gameconfig->playerflag;
} // end FlagSelectionView::init

// doDraw
//---------------------------------------------------------------------------
void FlagSelectionView::doDraw(Surface &viewArea, Surface &clientArea)
{
    //iRect r(getViewRect());
    //viewArea.bltLookup(r, Palette::darkGray256.getColorArray());

    char strBuf[256];
    sprintf(strBuf, "Current:");
    int CHAR_XPIX = 8; // XXX hardcoded
    clientArea.bltStringShadowed(
            BORDER_SPACE,
            BORDER_SPACE + (playerFlag.getHeight() - Surface::getFontHeight()) / 2,
            strBuf, windowTextColor, windowTextColorShadow);
    playerFlag.setFrame(playerFlagSelected);
    playerFlag.blt(clientArea, BORDER_SPACE + strlen(strBuf) * CHAR_XPIX + BORDER_SPACE, BORDER_SPACE);

    //char strBuf[256];
    //sprintf(strBuf, "%d", scrollBar->getValue());
    //clientArea.bltStringCenter(strBuf, Color::white);
    //
    //// The -12 is a cheeze hack to take into account the scrollBar minor axis size.
    //int x;
    //int y = (clientArea.getHeight() - flags.getHeight() - 12) / 2;
    //
    //int minFlag = scrollBar->getValue();
    //int maxFlag = scrollBar->getValue() + scrollBar->getViewableAmount();
    //
    //if (maxFlag > flags.getFrameCount() - 1)
    //{
    //	assert(false);
    //	maxFlag = flags.getFrameCount() - 1;
    //}
    //
    ////for (int i = 0; i < flags.getFrameCount(); i++)
    //
    //for (int i = minFlag; i < maxFlag; i++)
    //{
    //	x = flags.getWidth() * (i - minFlag);
    //
    //	flags.setFrame(i);
    //	flags.blt(clientArea, x, y);
    //}

    View::doDraw(viewArea, clientArea);

} // end FlagSelectionView::doDraw

// lMouseUp
//--------------------------------------------------------------------------
int FlagSelectionView::lMouseUp(const iXY &downPos, const iXY &upPos)
{
    View::lMouseUp(downPos, upPos);

    const int buttonsBeforeFlags = 0;

    if (getPrevPressedButton() >= buttonsBeforeFlags && getPrevPressedButton() < (int)playerFlag.getNumFrames() + buttonsBeforeFlags) {
        playerFlagSelected = getPrevPressedButton() - buttonsBeforeFlags;
    }

    return true;
} // end FlagSelectionViewlMouseUp
