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
#ifndef __ControlView_hpp__
#define __ControlView_hpp__

#include "View.hpp"
#include "Surface.hpp"
#include "MiniMapInterface.hpp"

class ControlView : public View
{
friend class ControlPaletteView;

protected:
	enum ORIENTATION { VERTICAL, HORIZONTAL, };

	static ORIENTATION orientation;
	static int         iconBarMinSize;
	static int         mapNavAidSize;
	static Surface    worldMapSurface;
	static Surface    worldMapClientArea;
	static Surface    zoomMapSurface;
	static int         maxZoomLevel;

public:
	static int         deltaZoom;

public:
	ControlView(const char *title);

	static void initWorldMapSurface()
	{
		Surface *miniMap = MiniMapInterface::getMiniMap();

		assert(miniMap != 0);

		worldMapSurface.copy(*miniMap);
		worldMapSurface.scale(getMapClientArea());
	}

	virtual void doDraw(const Surface &windowArea, const Surface &clientArea);

	static iXY getMapClientArea()
	{
		iXY viewSize;
		int allComponentsButViewAreaYSize = iconBarMinSize * 4 + mapNavAidSize * 3;

		viewSize.y = (SCREEN_YPIX - allComponentsButViewAreaYSize) / 3;

		if (orientation == VERTICAL)
		{
			// Vertical
			viewSize.x = viewSize.y;
		}
		else if (orientation == HORIZONTAL)
		{
			// Horizontal
			viewSize.x = (SCREEN_XPIX - allComponentsButViewAreaYSize) / 3;
		}

		return viewSize;
	}

	static iXY getViewSize()
	{
		iXY viewSize = getMapClientArea();

		viewSize.x += mapNavAidSize + 1;
		viewSize.y += iconBarMinSize + mapNavAidSize + 1;

		return viewSize;
	}

	void drawIconBorder(const Surface &dest);
	void drawZoomMap(const Surface &dest);

	//virtual void lMouseDown(const iXY &pos);
	void rMouseDown(const iXY &pos);
	void rMouseUp(const iXY &downPos, const iXY &upPos);
	void rMouseDrag(const iXY &downPos, const iXY &prevPos, const iXY &newPos);
	void rMouseDouble(const iXY &pos);
	
	//void drawMouseBox(const Surface &dest);
	//void setViewWindow(const iXY &pos);
	//void updateObjectPosition(const Surface &dest);
	void updateScreenPosition(const Surface &dest);

	//virtual void mouseEnter(const iXY &pos);
	//virtual void mouseExit(const iXY &pos);
	//virtual void doActivate();
	//virtual void doDeactivate();

}; // end ControlView

#endif // end __ControlView_hpp__
