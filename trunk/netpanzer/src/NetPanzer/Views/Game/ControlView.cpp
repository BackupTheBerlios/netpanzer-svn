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
#include "ControlView.hpp"
#include "cMouse.hpp"
#include "MiniMapInterface.hpp"
#include "KeyboardInterface.hpp"
#include "cMouse.hpp"


ControlView::ORIENTATION ControlView::orientation = VERTICAL;

int         ControlView::mapNavAidSize  = 12;
int         ControlView::iconBarMinSize = 15;
int         ControlView::maxZoomLevel   = 0;

Surface    ControlView::worldMapSurface;
Surface    ControlView::worldMapClientArea;
Surface    ControlView::zoomMapSurface;

int         ControlView::deltaZoom = 0;


static void increaseZoom()
{
	ControlView::deltaZoom++;
}

static void decreaseZoom()
{
	ControlView::deltaZoom--;
}

// ControlView constructor
//---------------------------------------------------------------------------
ControlView::ControlView(const char *title) : View()
{
	setSearchName("ControlView");
	setTitle("Control");
	setSubTitle("");

	assert(this != 0);

	setTitle(title);
	setAllowResize(false);
	setDisplayStatusBar(false);
	setVisible(true);
	setBordered(false);

	moveTo(iXY(0, 0));
	
	iXY size(getViewSize());
	resize(size);

	int x = 1;
	int y = size.y - 14;

	int xOffset = 15;

	// Add the option buttons.
	addButtonCenterText(iXY(x, y), 15, "-", "", decreaseZoom);
	x += xOffset;
	addButtonCenterText(iXY(x, y), 15, "+", "", increaseZoom);
	x += xOffset;
	addButtonCenterText(iXY(x, y), 15, "2", "", 0);
	x += xOffset;
	addButtonCenterText(iXY(x, y), 15, "3", "", 0);

} // end ControlView constructor

// doDraw
//---------------------------------------------------------------------------
void ControlView::doDraw(const Surface &viewArea, const Surface &clientArea)
{
	assert(this != 0);
	assert(viewArea.getDoesExist());
	assert(clientArea.getDoesExist());

	viewArea.fill(Color::red);

	iXY caSize(getClientRect().getSize());

	iRect r(iXY(0, 0), caSize - 1);
	clientArea.drawButtonBorder(r, Color::gray64, Color::white);

	drawIconBorder(clientArea);
	drawZoomMap(clientArea);

	View::doDraw(viewArea, clientArea);

} // end doDraw

void ControlView::drawIconBorder(const Surface &dest)
{
	iXY crSize(getClientRect().getSize());

	iRect r(iXY(0, (crSize.y - iconBarMinSize)), crSize - 1);

	dest.drawButtonBorder(r, Color::gray64, Color::white);
}

void ControlView::drawZoomMap(const Surface &dest)
{
    iRect world_win;
    world_win = MiniMapInterface::getWorldWindow();

	// Offset the map drawing area from the sector markers.
	iXY pos(mapNavAidSize, mapNavAidSize);

	iRect r(pos, pos + ControlView::getMapClientArea());

	zoomMapSurface.setTo(dest, r);

	Surface *miniMap = MiniMapInterface::getMiniMap();

	iXY drawPos(-world_win.min.x, -world_win.min.y);

	if (miniMap != 0)
	{
		iXY mcArea = ControlView::getMapClientArea();

		// Make sure the map stays totally on the clientArea.
		if (drawPos.x < -miniMap->getPix().x + mcArea.x)
		{
			drawPos.x = -miniMap->getPix().x + mcArea.x;
		}
		else if (drawPos.y < -miniMap->getPix().y + mcArea.y)
		{
			drawPos.y = -miniMap->getPix().y + mcArea.y;
		}

		//void bltBlendScale(const Surface &source, const iRect &destRect, cColorTable &colorTable) const;
		iRect destRect(drawPos, drawPos + miniMap->getPix() + deltaZoom);

		zoomMapSurface.bltScale(*miniMap, destRect);
		//miniMap->blt(zoomMapSurface, drawPos);

	} else
	{
		assert(false);
	}
}

//---------------------------------------------------------------------------
void ControlView::rMouseDown(const iXY &pos)
{
	View::lMouseDown(pos);
}

//---------------------------------------------------------------------------
void ControlView::rMouseUp(const iXY &downPos, const iXY &upPos)
{
	View::lMouseUp(downPos, upPos);
}

//---------------------------------------------------------------------------
void ControlView::rMouseDrag(const iXY &downPos, const iXY &prevPos, const iXY &newPos)
{
	View::lMouseUp(downPos, newPos);
	View::lMouseDown(newPos);
}

//---------------------------------------------------------------------------
void ControlView::rMouseDouble(const iXY &pos)
{
}
