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


#include "AreYouSureExitView.hpp"
#include "Views/Components/Desktop.hpp"
#include "Views/Components/Button.hpp"
#include "2D/Palette.hpp"
#include "Classes/ScreenSurface.hpp"
#include "Interfaces/GameManager.hpp"

#include "Actions/Action.hpp"


class ExitNetPanzerAction : public Action
{
public:
    ExitNetPanzerAction() : Action(false) {}
    void execute()
    {
        GameManager::drawTextCenteredOnScreen("Exiting", Color::white);
        GameManager::exitNetPanzer();
    }
};

class HideView : public Action
{
public:
    View * view;
    HideView(View * view) : Action(false), view(view) {}
    void execute()
    {
        Desktop::setVisibility(view->getSearchName(), false);
    }
};

// AreYouSureExitView
//---------------------------------------------------------------------------
AreYouSureExitView::AreYouSureExitView() : SpecialButtonView()
{
    setSearchName("AreYouSureExitView");
    setTitle("Exit netPanzer");
    setSubTitle("");
    loaded = false;
	} // end AreYouSureExitView::AreYouSureExitView

// init
//---------------------------------------------------------------------------
void AreYouSureExitView::init()
{
    setBordered(false);
    setAllowResize(false);
    setDisplayStatusBar(false);

    moveTo(iXY(0, 0));
    resize(iXY(800, 600));

    int x = (getClientRect().getSize().x - (141 * 2 + 20)) / 2;
    int y = getClientRect().getSize().y/2 + 30;
    add( Button::createTextButton( "YES", iXY(x, y), 137, new ExitNetPanzerAction()));
    x += 141 + 10;
    add( Button::createTextButton( "NO", iXY(x, y), 137, new HideView(this)));
    loaded = true;
} // end AreYouSureExitView::init

// doDraw
//---------------------------------------------------------------------------
void AreYouSureExitView::doDraw(Surface &viewArea, Surface &clientArea)
{
    viewArea.bltLookup(getClientRect(), Palette::darkGray256.getColorArray());
    //viewArea.drawButtonBorder(r, Color::lightGreen, Color::darkGreen);

    viewArea.bltStringCenter("Are you sure you wish to exit netPanzer?", Color::white);

    View::doDraw(viewArea, clientArea);
} // end AreYouSureExitView::doDraw

// doActivate
//---------------------------------------------------------------------------
void AreYouSureExitView::doActivate()
{
    if ( ! loaded )
    {
	init();
    }
    Desktop::setActiveView(this);

} // end AreYouSureExitView::doActivate
