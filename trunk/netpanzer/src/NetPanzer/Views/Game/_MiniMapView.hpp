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


#ifndef __MiniMapView_hpp__
#define __MiniMapView_hpp__


#if _MSC_VER > 1000
	#pragma once
#endif


#include "View.hpp"
#include "Surface.hpp"
#include "GameTemplateView.hpp"


enum MAP_DRAW_TYPES
{
	MAP_SOLID,
    MAP_2575,
    MAP_5050,
    MAP_BLEND_GRAY,
    MAP_BLEND_DARK_GRAY,
    MAP_BLEND_GREEN,
    MAP_BLACK,
    MAP_TRANSPARENT,
};

//---------------------------------------------------------------------------
class MiniMapView : public GameTemplateView
{
private:
	 int     deltaSize;
	 int     minMapSize;
	 int     maxMapSize;
	 Surface miniMapSurface;
	 bool    needScale;
	 float   scaleGroupWait;

	void addButtons();

public:
	MiniMapView();

	void init();
	
	virtual void doDraw(const Surface &windowArea, const Surface &clientArea);
	virtual void lMouseDown(const iXY &pos);
	virtual void rMouseDrag(const iXY &downPos, const iXY &prevPos, const iXY &newPos);
	
	void drawMouseBox(const Surface &dest);
	void setViewWindow(const iXY &pos);
	void updateScreenPosition(const Surface &dest);

	static float          scaleDelta;
	static MAP_DRAW_TYPES mapDrawType;

}; // end MiniMapView

extern MiniMapView miniMapView;

#endif // end __MiniMapView_hpp__
