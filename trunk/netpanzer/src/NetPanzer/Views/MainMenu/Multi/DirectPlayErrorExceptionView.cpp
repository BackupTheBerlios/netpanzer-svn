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

#include "DirectPlayErrorExceptionView.hpp"
#include "Desktop.hpp"
#include "GameManager.hpp"


//---------------------------------------------------------------------------
static void bOK()
{
	Desktop::setVisibility("DirectPlayErrorExceptionView", false);
	Desktop::setVisibility("GetSessionView", true);
}

// DirectPlayErrorExceptionView
//---------------------------------------------------------------------------
DirectPlayErrorExceptionView::DirectPlayErrorExceptionView() : View()
{
	setSearchName("DirectPlayErrorExceptionView");
	setTitle("DirectPlayErrorException");
	setSubTitle("");

} // end DirectPlayErrorExceptionView::DirectPlayErrorExceptionView

// init
//---------------------------------------------------------------------------
void DirectPlayErrorExceptionView::init()
{
	removeAllButtons();

	setBordered(false);
	setAllowResize(false);
	setDisplayStatusBar(false);

	moveTo(0, 0);
	resize(SCREEN_PIX);

	iXY buttonSize(70, 15);
	int BUTTON_COUNT = 2;
	int x            = (getClientRect().getSize().x - (buttonSize.x * 2 + 12)) / 2;
	int y;
	int yOffset      = buttonSize.y * 2;
	
	y = SCREEN_YCENTER + 30;
	addButtonCenterText(iXY(x, y), buttonSize.x, "OK", "", bOK);

} // end DirectPlayErrorExceptionView::init

// doDraw
//---------------------------------------------------------------------------
void DirectPlayErrorExceptionView::doDraw(const Surface &viewArea, const Surface &clientArea)
{
	iXY   pos(0, 0);
	iXY   size(SCREEN_PIX);
	iRect r(pos, pos + size);

	viewArea.bltLookup(r, Palette::darkGray256.getColorArray());
	viewArea.drawButtonBorder(r, Color::lightGreen, Color::darkGreen);

     //WE NEED TO TELL THEM THAT A WEIRD DPLAY ERROR OCCURRED
     //AND BUMP THEM OUT TO HOST OR JOIN MENU
	viewArea.bltStringCenter("A weird DirectPlay error just occurred, returning you to the Host/Join View.", Color::white);

	View::doDraw(viewArea, clientArea);

} // end DirectPlayErrorExceptionView::doDraw

// doActivate
//---------------------------------------------------------------------------
void DirectPlayErrorExceptionView::doActivate()
{
	init();
	Desktop::setActiveView(this);

} // end DirectPlayErrorExceptionView::doActivate
