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
#include "PlayerNameView.hpp"
#include "GameViewGlobals.hpp"
#include "GameConfig.hpp"

cInputFieldString PlayerNameView::playerName;


// PlayerNameView
//---------------------------------------------------------------------------
PlayerNameView::PlayerNameView() : View()
{
    setSearchName("PlayerNameView");
    setTitle("Player Name");
    setSubTitle("");

    setAllowResize(false);
    setAllowMove(false);
    setVisible(false);

    moveTo(bodyTextRect.min);

    // The plus 8 for x and 4 for y are what I put in input field.  Add function to find out,
    // inpit field dimension.
    int CHAR_XPIX = 8; // XXX hardcoded :-/
    resizeClientArea(INPUT_FIELD_CHARACTERS * CHAR_XPIX + 8 + BORDER_SPACE * 2, Surface::getFontHeight() + 4 + BORDER_SPACE * 2);

    init();

} // end PlayerNameView::PlayerNameView

// init
//---------------------------------------------------------------------------
void PlayerNameView::init()
{
    playerName.init("Player", INPUT_FIELD_CHARACTERS);
    addInputField(iXY(BORDER_SPACE, BORDER_SPACE), &playerName, "", true);

} // end PlayerNameView::init

// doDraw
//---------------------------------------------------------------------------
void PlayerNameView::doDraw(const Surface &viewArea, const Surface &clientArea)
{
    //iRect r(getViewRect());
    //viewArea.bltLookup(r, Palette::darkGray256.getColorArray());

    View::doDraw(viewArea, clientArea);

} // end PlayerNameView::doDraw
