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

#include "AreYouSureExitView.hpp"
#include "Desktop.hpp"
#include "Palette.hpp"
#include "ScreenSurface.hpp"
#include "GameManager.hpp"

//---------------------------------------------------------------------------
static void bYES()
{
    GameManager::drawTextCenteredOnScreen("Exiting", Color::white);

    GameManager::exitNetPanzer();
}

//---------------------------------------------------------------------------
static void bNO()
{
    Desktop::setVisibility("AreYouSureExitView", false);
    //Desktop::setVisibility("ResignView", true);
}

// AreYouSureExitView
//---------------------------------------------------------------------------
AreYouSureExitView::AreYouSureExitView() : SpecialButtonView()
{
    setSearchName("AreYouSureExitView");
    setTitle("Exit netPanzer");
    setSubTitle("");

} // end AreYouSureExitView::AreYouSureExitView

// init
//---------------------------------------------------------------------------
void AreYouSureExitView::init()
{
    removeAllButtons();

    setBordered(false);
    setAllowResize(false);
    setDisplayStatusBar(false);

    resize(screen->getPix());
    moveTo(0,0);

    int x = (getClientRect().getSize().x - (141 * 2 + 20)) / 2;
    int y = screen->getPixY()/2 + 30;
    addSpecialButton(	iXY(x, y),
                      "YES",
                      bYES);

    x += 141 + 10;
    addSpecialButton(	iXY(x, y),
                      "NO",
                      bNO);

} // end AreYouSureExitView::init

// doDraw
//---------------------------------------------------------------------------
void AreYouSureExitView::doDraw(Surface &viewArea, Surface &clientArea)
{
    iRect r(min, max);

    viewArea.bltLookup(r, Palette::darkGray256.getColorArray());
    //viewArea.drawButtonBorder(r, Color::lightGreen, Color::darkGreen);

    viewArea.bltStringCenter("Are you sure you wish to exit netPanzer?", Color::white);

    View::doDraw(viewArea, clientArea);
} // end AreYouSureExitView::doDraw

// doActivate
//---------------------------------------------------------------------------
void AreYouSureExitView::doActivate()
{
    init();
    Desktop::setActiveView(this);

} // end AreYouSureExitView::doActivate
