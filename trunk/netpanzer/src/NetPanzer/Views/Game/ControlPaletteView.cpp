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

#include "Log.hpp"
#include "ControlPaletteView.hpp"
#include "cMouse.hpp"
#include "MiniMapInterface.hpp"
#include "KeyboardInterface.hpp"
#include "cMouse.hpp"


//int ControlPaletteView::drawMethod  = Palette::DRAW_SOLID;


// ControlPaletteView
//---------------------------------------------------------------------------
ControlPaletteView::ControlPaletteView() : View()
{
	assert(this != 0);

	setSearchName("ControlPaletteView");
	setTitle("Control Palette");
	setSubTitle("");

	setAllowResize(false);
	setDisplayStatusBar(false);
	setVisible(true);
	setBordered(false);

	moveTo(iXY(0, 0));
	
	resize(iXY(141, 15));

	int x = 0;
	int y = 0;

	int xOffset = 15;

	// Add the option buttons.
	addButtonCenterText(iXY(x, y), 15, "0", "", 0);
	x += xOffset;
	addButtonCenterText(iXY(x, y), 15, "1", "", 0);
	x += xOffset;
	addButtonCenterText(iXY(x, y), 15, "2", "", 0);
	x += xOffset;
	addButtonCenterText(iXY(x, y), 15, "3", "", 0);

	init();
	ControlView::initWorldMapSurface();

} // end ControlPaletteView::ControlPaletteView

// doDraw
//---------------------------------------------------------------------------
void ControlPaletteView::doDraw(const Surface &viewArea, const Surface &clientArea)
{
	FUNC("ControlPaletteView::doDraw");
	assert(this != 0);
	assert(viewArea.getDoesExist());
	assert(clientArea.getDoesExist());

	viewArea.fill(Color::blue);

	View::doDraw(viewArea, clientArea);

} // end ControlPaletteView::doDraw
