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
#include "MiniMapView.hpp"
#include "cMouse.hpp"
#include "MiniMapInterface.hpp"
#include "KeyboardInterface.hpp"
#include "cMouse.hpp"
#include "GameConfig.hpp"
#include "Desktop.hpp"

// XXX is this an old version of the MinimapView?

#if 0

MiniMapView miniMapView;

MAP_DRAW_TYPES MiniMapView::mapDrawType;

float MiniMapView::scaleDelta = 400.0f;

static void bCloseMiniMap()
{
	Desktop::setVisibility("MiniMapView", false);
}

static void bMaximizeMiniMap()
{
}

static void bMiniMapOptions()
{
	Desktop::toggleVisibility("MiniMapOptionsView");
}

static void bToggleGrid()
{
}

// MiniMapView
//---------------------------------------------------------------------------
MiniMapView::MiniMapView() : GameTemplateView()
{
	assert(isValidPtr(this));

	setSearchName("MiniMapView");
	setTitle("Mini Map");
	setSubTitle("");

	setVisible(false);

	setDisplayStatusBar(true);
	setBordered(true);
	setAllowResize(true);

} // end MiniMapView::MiniMapView

//---------------------------------------------------------------------------
void MiniMapView::addButtons()
{
	removeAllButtons();

	setTitle(GameConfig::getGameMapName());

	Surface s(DEFAULT_STATUS_BAR_HEIGHT - 2, DEFAULT_STATUS_BAR_HEIGHT - 2, DEFAULT_STATUS_BAR_HEIGHT - 2, 1);

	iRect clientRect(getClientRect());
	iXY pos(clientRect.getSizeX(), clientRect.getSizeY() - DEFAULT_STATUS_BAR_HEIGHT + 1);

	pos.x -= DEFAULT_STATUS_BAR_HEIGHT;
	pos.x -= DEFAULT_STATUS_BAR_HEIGHT;
	s.fill(Color::gray192);
	s.drawLine(s.getPixX() - 2, 0, 0, s.getPixY() - 2, Color::gray64);
	s.drawLine(s.getPixX() - 2, 1, 1, s.getPixY() - 2, Color::gray96);
	s.drawLine(s.getPixX() - 1, 1, 1, s.getPixY() - 1, Color::gray64);
	s.drawRect(0, 0, s.getPixX() - 2, s.getPixY() - 2, Color::gray96);
	s.drawRect(1, 1, s.getPixX() - 1, s.getPixY() - 1, Color::white);
	s.drawRect(1, 1, s.getPixX() - 2, s.getPixY() - 2, Color::black);
	addButtonSurfaceSingle(pos, s, "Close MiniMap", bCloseMiniMap);

	pos.x -= DEFAULT_STATUS_BAR_HEIGHT + 1;
	s.fill(Color::gray192);
	s.drawRect(0, 0, s.getPixX() - 2, s.getPixY() - 2, Color::gray96);
	s.drawRect(1, 1, s.getPixX() - 1, s.getPixY() - 1, Color::white);
	s.drawRect(1, 1, s.getPixX() - 2, s.getPixY() - 2, Color::black);
	addButtonSurfaceSingle(pos, s, "Maximize", bMaximizeMiniMap);

	pos.x -= DEFAULT_STATUS_BAR_HEIGHT + 1;
	pos.x -= DEFAULT_STATUS_BAR_HEIGHT + 1;
	s.fill(Color::gray192);
	s.drawRect(0, 0, s.getPixX() - 2, s.getPixY() - 2, Color::gray96);
	s.drawRect(1, 1, s.getPixX() - 1, s.getPixY() - 1, Color::white);
	s.drawRect(1, 1, s.getPixX() - 2, s.getPixY() - 2, Color::black);
	s.bltStringCenter("O", Color::gray64);
	addButtonSurfaceSingle(pos, s, "Mini Map Options", bMiniMapOptions);

	pos.x -= DEFAULT_STATUS_BAR_HEIGHT;
	s.fill(Color::gray192);
	s.drawRect(0, 0, s.getPixX() - 2, s.getPixY() - 2, Color::gray96);
	s.drawRect(1, 1, s.getPixX() - 1, s.getPixY() - 1, Color::white);
	s.drawRect(1, 1, s.getPixX() - 2, s.getPixY() - 2, Color::black);
	s.drawHLine(3, 3, 10, Color::gray64);
	s.drawHLine(3, 5, 10, Color::gray64);
	s.drawHLine(3, 7, 10, Color::gray64);
	s.drawHLine(3, 9, 10, Color::gray64);
	s.drawVLine(4, 3, 11, Color::gray64);
	s.drawVLine(6, 3, 11, Color::gray64);
	s.drawVLine(8, 3, 11, Color::gray64);
	s.drawVLine(10, 3, 11, Color::gray64);
	addButtonSurfaceSingle(pos, s, "Toggle Grid", bToggleGrid);

	showStatus("This is the status bar.");
}

// init
//---------------------------------------------------------------------------
void MiniMapView::init()
{
	moveTo(iXY(0, 0));

	iXY size(128, 128);

	resize(size);

	mapDrawType = MAP_SOLID;

	//int xOffset = size.x;
	//int yOffset = 0;
	
	iRect clientRect(getClientRect());
	iRect miniMapRect(clientRect.min.x, clientRect.min.y, clientRect.max.x, clientRect.max.y - DEFAULT_STATUS_BAR_HEIGHT);
	MiniMapInterface::setMapScale(miniMapRect.getSize());

	minMapSize = 32;

	// Get the original version of the minimap.
	Surface *miniMap = MiniMapInterface::getMiniMap();
	if (miniMap == 0)
	{
		FUBAR("ERROR: Unable to find minimap surface.");
	}

	miniMapSurface.copy(*miniMap);

	scaleGroupWait = 0.0f;
	needScale      = true;

	addButtons();

} // end MiniMapView::init

// doDraw
//---------------------------------------------------------------------------
void MiniMapView::doDraw(const Surface &viewArea, const Surface &clientArea)
{
	assert(isValidPtr(this));
	assert(viewArea.getDoesExist());
	assert(clientArea.getDoesExist());

	if (!getVisible()) return;

	float dt = TimerInterface::getTimeSlice();

	Surface *miniMap = MiniMapInterface::getMiniMap();
	if (miniMap == 0)
	{
		FUBAR("ERROR: Unable to find minimap surface.");
	}

	// Check to see if this map need to be rescaled.
	iRect clientRect(getClientRect());
	iRect miniMapRect(clientRect.min.x, clientRect.min.y, clientRect.max.x, clientRect.max.y - DEFAULT_STATUS_BAR_HEIGHT);

	if (miniMapRect.getSize() != MiniMapInterface::getMapScale())
	{
		// Place the buttons in the correct place.
		addButtons();
		
		// Update the interface of the scale change.
		MiniMapInterface::setMapScale(miniMapRect.getSize());
		needScale      = true;
		scaleGroupWait = 0.0f;
	}

	if (needScale)
	{
		scaleGroupWait += dt;

		if (scaleGroupWait > 1.0f)
		{
			miniMapSurface.create(getViewRect().getSize(), getViewRect().getSize().x , 1);
			
			//miniMapSurface.scale(getViewRect().getSize());
			iRect r(iXY(0, 0), getViewRect().getSize());

			miniMapSurface.bltScale(*miniMap, r);

			needScale      = false;
			scaleGroupWait = 0.0f;
		}
	}

	iRect r(getViewRect().min, getViewRect().max);

	if (needScale)
	{
		// Draw the slow on the fly scaled map.
		if (mapDrawType == MAP_SOLID) 
		{
			clientArea.bltScale(*miniMap, r);
		}
		// doesn't work...
#if 0 
		else if (mapDrawType == MAP_2575)
		{
			clientArea.bltBlendScale(*miniMap, r, Palette::colorTable2575);
		}
		else if (mapDrawType == MAP_5050)
		{
			clientArea.bltBlendScale(*miniMap, r, Palette::colorTable5050);
		}
#endif
		//else if (mapDrawType == MAP_BLEND_GREEN)
		//{
			//clientArea.bltLookup(iRect(iXY(0, 0), getSize()), Palette::green256.getColorArray());
		//}
		else if (mapDrawType == MAP_BLEND_GRAY)
		{
			clientArea.bltLookup(iRect(iXY(0, 0), getSize()), Palette::gray256.getColorArray());
		}
		else if (mapDrawType == MAP_BLEND_DARK_GRAY)
		{
			clientArea.bltLookup(iRect(iXY(0, 0), getSize()), Palette::darkGray256.getColorArray());
		}
		else if (mapDrawType == MAP_BLACK)
		{
			clientArea.fill(Color::black);
		}
		else if (mapDrawType == MAP_TRANSPARENT)
		{
		}
	} else
	{
		// Draw the fast not on the fly scaled map.
		if (mapDrawType == MAP_SOLID) 
		{
			miniMapSurface.blt(clientArea, 0, 0);
		}
		// XXX not all defined...
#if 0
		else if (mapDrawType == MAP_2575)
		{
			clientArea.blendIn(miniMapSurface, iXY(0, 0), Palette::colorTable2575);
		}
		else if (mapDrawType == MAP_5050)
		{
			clientArea.blendIn(miniMapSurface, iXY(0, 0), Palette::colorTable5050);
		}
#endif
		//else if (mapDrawType == MAP_BLEND_GREEN)
		//{
			//clientArea.bltLookup(iRect(iXY(0, 0), getSize()), Palette::green256.getColorArray());
		//}
		else if (mapDrawType == MAP_BLEND_GRAY)
		{
			clientArea.bltLookup(iRect(iXY(0, 0), getSize()), Palette::gray256.getColorArray());
		}
		else if (mapDrawType == MAP_BLEND_DARK_GRAY)
		{
			clientArea.bltLookup(iRect(iXY(0, 0), getSize()), Palette::darkGray256.getColorArray());
		}
		else if (mapDrawType == MAP_BLACK)
		{
			clientArea.fill(Color::black);
		}
		else if (mapDrawType == MAP_TRANSPARENT)
		{
		}
	}

	// Draw a hairline border.
	viewArea.drawButtonBorder(Color::lightGreen, Color::darkGreen);

	// Draw the units and such on the minimap.
	MiniMapInterface::annotateMiniMap( (Surface &) clientArea );

	// Draw the square which represents the screen.
	updateScreenPosition(clientArea);

	// If the mouse is over the client area, then change the cursor.
	if (getClientRect().contains(getScreenToClientPos(mouse.getScreenPos())) &&
			getActive())
	{
		// Draw a box which show where the area which you click will be located
		drawMouseBox(clientArea);
	}

	View::doDraw(viewArea, clientArea);

} // end doDraw

// updateScreenPosition
//---------------------------------------------------------------------------
// Purpose: Updates the box showing the current location on the world map
//          corresponding to the position on the MiniMapView.
//---------------------------------------------------------------------------
void MiniMapView::updateScreenPosition(const Surface &dest)
{
	assert(isValidPtr(this));
	assert(dest.getDoesExist());

    dest.drawRect(MiniMapInterface::getWorldWindow(), Color::white);

} // end MiniMapView::updateScreenPosition

// setViewWindow
//--------------------------------------------------------------------------
// Purpose: Sets the viewable window on the screen to the position clicked
//          on the map.  pos (position) corresponds to the grid(x,y) position clicked
//          ont he map.  The pos is converted to world coords and the
//          viewable window is repositioned.
//--------------------------------------------------------------------------
void MiniMapView::setViewWindow(const iXY &pos)
{ 
	assert(isValidPtr(this));

	iXY size(getViewRect().getSize());

	if ((pos.x >= 0) && (pos.x < size.x) && (pos.y >= 0) && (pos.y < size.y))
	{
		MiniMapInterface::setWorldWindowPosition( PointXYi( pos.x, pos.y ) );
	} 

} // end MiniMapView::setViewWindow

// drawMouseBox
//--------------------------------------------------------------------------
// Purpose: Replaces the mouse cursor with a box showing where the viewable
//          screen area would be moved to if the mouse was pressed.
//--------------------------------------------------------------------------
void MiniMapView::drawMouseBox(const Surface &dest)
{
	assert(isValidPtr(this));
	assert(dest.getDoesExist());

    iRect r(MiniMapInterface::getWorldWindow());
	iXY   size(r.getSize() / 2);
	iXY   pos(getScreenToClientPos(mouse.getScreenPos()));

	dest.drawRect(pos - size - 1, pos + size - 1, Color::yellow);

} // end MiniMapView::drawMouseBox

// lMouseDown
//--------------------------------------------------------------------------
// Purpose:  Move the current viewable area of the map to the area
//           corresponding to the position of teh mouse on the map.
//--------------------------------------------------------------------------
void MiniMapView::lMouseDown(const iXY &pos)
{
	assert(isValidPtr(this));

	// If the mouse was pressed on the MiniMapView, then move the game win
	// to the click location.
	if (getClientRect().contains(getScreenToClientPos(mouse.getScreenPos())))
	{
		setViewWindow(pos);
	}

	View::lMouseDown(pos);

} // end MiniMapView::lMouseDown

void MiniMapView::rMouseDrag(const iXY &downPos, const iXY &prevPos, const iXY &newPos)
{
	float dt = TimerInterface::getTimeSlice();

	// Let the map go up to the min screen dimension.
	maxMapSize = MIN(SCREEN_XPIX, SCREEN_YPIX);
	//maxMapSize = MIN(640, 480);

	moveTo(min + newPos - prevPos);

	// Check for map blending mode change.
	if (KeyboardInterface::getKeyPressed(SDLK_1))
	{
		MiniMapView::mapDrawType = MAP_SOLID;
	}
	else if (KeyboardInterface::getKeyPressed(SDLK_2))
	{
		MiniMapView::mapDrawType = MAP_2575;
	}
	else if (KeyboardInterface::getKeyPressed(SDLK_3))
	{
		MiniMapView::mapDrawType = MAP_5050;
	}
	else if (KeyboardInterface::getKeyPressed(SDLK_4))
	{
		MiniMapView::mapDrawType = MAP_BLEND_GREEN;
	}
	else if (KeyboardInterface::getKeyPressed(SDLK_5))
	{
		MiniMapView::mapDrawType = MAP_BLEND_GRAY;
	}
	else if (KeyboardInterface::getKeyPressed(SDLK_6))
	{
		MiniMapView::mapDrawType = MAP_BLEND_DARK_GRAY;
	}
	else if (KeyboardInterface::getKeyPressed(SDLK_7))
	{
		MiniMapView::mapDrawType = MAP_BLACK;
	}
	else if (KeyboardInterface::getKeyPressed(SDLK_8))
	{
		MiniMapView::mapDrawType = MAP_TRANSPARENT;
	}

	if (KeyboardInterface::getKeyState(SDLK_NUM_PLUS))
	{
		iXY   destSize(getViewRect().getSize() + scaleDelta * dt);

		//resize(destSize);
		//deltaSize += deltaAmount;
		
		// Check the validity of the X dimension.
		if ((min.x + destSize.x) >= SCREEN_XPIX)
		{
			int xOffset = min.x + destSize.x - SCREEN_XPIX;

			int destXPos = min.x - xOffset;
			
			if (destXPos < 0)
			{
				moveTo(0, min.y);

			} else
			{
				moveTo(destXPos, min.y);
			}
		}

		// Check the validity of the Y dimension.
		if ((min.y + destSize.y) >= SCREEN_YPIX)
		{
			int yOffset = min.y + destSize.y - SCREEN_YPIX;

			int destYPos = min.y - yOffset;
			
			if (destYPos < 0)
			{
				moveTo(min.x, 0);

			} else
			{
				moveTo(min.x, destYPos);
			}
		}
		
		// Resize the x dimension.
		if (destSize.x > getViewRect().getSize().x)
		{
			if (destSize.x > maxMapSize)
			{
				resize(iXY(maxMapSize, getViewRect().getSize().y));

			} else
			{
				resize(iXY(destSize.x, getViewRect().getSize().y));
			}
		}

		// Resize the y dimension.
		if (destSize.y > getViewRect().getSize().y)
		{
			if (destSize.x > maxMapSize)
			{
				resize(iXY(getViewRect().getSize().x, maxMapSize));

			} else
			{
				resize(iXY(getViewRect().getSize().x, destSize.x));
			}
		}

		MiniMapInterface::setMapScale(getViewRect().getSize());

		needScale      = true;
		scaleGroupWait = 0.0f;
	}
	else if (KeyboardInterface::getKeyState(SDLK_NUM_MINUS))
	{
		resize(getViewRect().getSize() - scaleDelta * dt);
		//deltaSize -= deltaAmount;

		if (getViewRect().getSize().y < minMapSize)
		{
			resize(iXY(minMapSize, minMapSize));
		}

		MiniMapInterface::setMapScale(getViewRect().getSize());

		needScale      = true;
		scaleGroupWait = 0.0f;
	}
} // end MiniMapView::rMouseDrag

#endif
