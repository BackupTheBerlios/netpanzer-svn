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


#include "stdafx.hpp"
#include "ResignView.hpp"
#include "Desktop.hpp"
#include "AreYouSureResignView.hpp"
#include "AreYouSureExitView.hpp"


//---------------------------------------------------------------------------
static void bResign()
{
	Desktop::setVisibility("ResignView", false);
	Desktop::setVisibility("AreYouSureResignView", true);
}

//---------------------------------------------------------------------------
static void bReturnToGame()
{
	Desktop::setVisibility("ResignView", false);
}

//---------------------------------------------------------------------------
static void bExitNetPanzer()
{
	Desktop::setVisibility("ResignView", false);
	Desktop::setVisibility("AreYouSureExitView", true);
}


// ResignView
//---------------------------------------------------------------------------
ResignView::ResignView() : View()
{
	setSearchName("ResignView");
	setTitle("Resign");
	setSubTitle("");

} // end ResignView::ResignView

// init
//---------------------------------------------------------------------------
void ResignView::init()
{
	removeAllButtons();

	setBordered(false);
	setAllowResize(false);
	setDisplayStatusBar(false);

	moveTo(0, 0);
	resize(SCREEN_PIX);

	iXY buttonSize(150, 15);
	int BUTTON_COUNT = 2;
	int x            = (getClientRect().getSize().x - buttonSize.x) / 2;
	int y;
	int yOffset      = buttonSize.y * 2;
	
	y = SCREEN_YCENTER - 30;
	addButtonCenterText(iXY(x, y), buttonSize.x, "Resign", "Returns to the MainView.", bResign);
	y += yOffset;
	addButtonCenterText(iXY(x, y), buttonSize.x, "Exit netPanzer", "Exits to Windows.", bExitNetPanzer);
	y += yOffset;
	addButtonCenterText(iXY(x, y), buttonSize.x, "Return To Game", "", bReturnToGame);

} // end ResignView::init

// doDraw
//---------------------------------------------------------------------------
void ResignView::doDraw(const Surface &viewArea, const Surface &clientArea)
{
	iXY   pos(0, 0);
	iXY   size(SCREEN_PIX);
	iRect r(pos, pos + size);

	viewArea.bltLookup(r, Palette::darkGray256.getColorArray());
	viewArea.drawButtonBorder(r, Color::lightGreen, Color::darkGreen);

	View::doDraw(viewArea, clientArea);

} // end ResignView::doDraw

// doActivate
//---------------------------------------------------------------------------
void ResignView::doActivate()
{
	init();
	Desktop::setActiveView(this);

} // end ResignView::doActivate
