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

#include <SDL.h>

#include "PlayerNameView.hpp"
#include "GameViewGlobals.hpp"
#include "GameConfig.hpp"

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

    // XXX ugly
//    if(!playernameview) playernameview = this;
} // end PlayerNameView::PlayerNameView

PlayerNameView::~PlayerNameView()
{
//	playernameview=0;
}

// init
//---------------------------------------------------------------------------
void PlayerNameView::init()
{
    playerName.init(gameconfig->playername.c_str(), INPUT_FIELD_CHARACTERS);
    cInputField* input 
        = addInputField(iXY(BORDER_SPACE, BORDER_SPACE), &playerName, "", true);
    input->setReturnAction(returnPressed);
} // end PlayerNameView::init

// doDraw
//---------------------------------------------------------------------------
void PlayerNameView::doDraw(Surface &viewArea, Surface &clientArea)
{
    //iRect r(getViewRect());
    //viewArea.bltLookup(r, Palette::darkGray256.getColorArray());

    View::doDraw(viewArea, clientArea);
} // end PlayerNameView::doDraw

void PlayerNameView::returnPressed(cInputField* input)
{
    gameconfig->playername = input->getDestString();
}

// sometime we shoudl eleminate all these global vars...
//PlayerNameView* playernameview = 0;
