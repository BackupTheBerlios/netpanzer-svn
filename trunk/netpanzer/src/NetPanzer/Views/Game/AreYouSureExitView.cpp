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
#include "Views/Components/Desktop.hpp"
#include "2D/Palette.hpp"
#include "Classes/ScreenSurface.hpp"
#include "Interfaces/GameManager.hpp"
#include "Views/Components/Button.hpp"

enum
{
    BTN_YES,
    BTN_NO
};

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
    setBordered(false);
    setAllowResize(false);
    setDisplayStatusBar(false);

    resize(screen->getWidth(), screen->getHeight());
    moveTo(0,0);

    int x = (getClientRect().getSize().x - (141 * 2 + 20)) / 2;
    int y = screen->getHeight()/2 + 30;

    iXY pos(x,y);
    add( Button::createSpecialButton("yes","Yes", pos, BTN_YES));

    pos.x += 141 + 10;
    add( Button::createSpecialButton("no","No", pos, BTN_NO));

} // end AreYouSureExitView::init

// doDraw
//---------------------------------------------------------------------------
void AreYouSureExitView::doDraw()
{
    drawTransRect(*this, Palette::darkGray256.getColorArray());
    //viewArea.drawButtonBorder(r, Color::lightGreen, Color::darkGreen);

    drawStringCenter("Are you sure you wish to exit netPanzer?", Color::white);

    View::doDraw();
} // end AreYouSureExitView::doDraw

// doActivate
//---------------------------------------------------------------------------
void AreYouSureExitView::doActivate()
{
    init();
    Desktop::setActiveView(this);

} // end AreYouSureExitView::doActivate

void
AreYouSureExitView::onComponentClicked(Component* c)
{
    switch ( c->getCustomCode() )
    {
        case BTN_YES:
            GameManager::drawTextCenteredOnScreen("Exiting", Color::white);
            GameManager::exitNetPanzer();
            break;

        case BTN_NO:
            Desktop::setVisibility("AreYouSureExitView", false);
            break;
    }
}
