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
#include "View.hpp"
#include "Desktop.hpp"
#include "MouseInterface.hpp"
#include "KeyboardInterface.hpp"
#include "fXY.hpp"
#include "loadPics.hpp"
#include "ViewGlobals.hpp"
#include "InputEvent.hpp"


Surface View::topBorder;
Surface View::leftBorder;
Surface View::bottomBorder;
Surface View::rightBorder;
Surface View::topLeftCornerLarge;
Surface View::topLeftCornerSmall;
Surface View::topRightCornerLarge;
Surface View::topRightCornerSmall;
Surface View::bottomLeftCornerLarge;
Surface View::bottomLeftCornerSmall;
Surface View::bottomRightCornerLarge;
Surface View::bottomRightCornerSmall;


const int RESIZE_WIDTH = 10;
const int RESIZE_XMIN  = RESIZE_WIDTH;
const int RESIZE_XMAX  = RESIZE_WIDTH * 3;
const int RESIZE_YMIN  = RESIZE_WIDTH;
const int RESIZE_YMAX  = RESIZE_WIDTH * 3;


/////////////////////////////////////////////////////////////////////////////
// Statics.
/////////////////////////////////////////////////////////////////////////////

Surface View::pics;

#define STANDARD_BORDERS
//#define HAIRLINE_BORDERS
//#define GRAPHIC_BORDERS

// add
//---------------------------------------------------------------------------
void View::add(Component *component)
{
	assert(component != 0);
	if (component == 0) { return; }
	
	if (componentsUsedCount < MAX_COMPONENT_COUNT - 1)
	{
		componentList[componentsUsedCount] = component;
		componentList[componentsUsedCount]->setParent(this);
		componentsUsedCount++;
	} else
	{
		FUBAR("ERROR: Trying to add more componentList than pre-allocated, increase the pre-allocated amount.");
	}

} // end View::add

// View
//---------------------------------------------------------------------------
View::View()
{
	reset();
	loadPics();

#ifdef GRAPHIC_BORDERS
	char strBuf[256];

	if (topBorder.getFrameCount() <= 0)
	{
		sprintf(strBuf, "pics\\viewBorders\\btop.til");
		if (!topBorder.loadTIL(strBuf))	FUBAR("ERROR: Unable to load: %s", strBuf);
	}
	if (leftBorder.getFrameCount() <= 0)
	{
		sprintf(strBuf, "pics\\viewBorders\\bleft.til");
		if (!leftBorder.loadTIL(strBuf)) FUBAR("ERROR: Unable to load: %s", strBuf);
	}
	if (bottomBorder.getFrameCount() <= 0)
	{
		sprintf(strBuf, "pics\\viewBorders\\bbottom.til");
		if (!bottomBorder.loadTIL(strBuf))	FUBAR("ERROR: Unable to load: %s", strBuf);
	}
	if (rightBorder.getFrameCount() <= 0)
	{
		sprintf(strBuf, "pics\\viewBorders\\bright.til");
		if (!rightBorder.loadTIL(strBuf)) FUBAR("ERROR: Unable to load: %s", strBuf);
	}
	if (topLeftCornerLarge.getFrameCount() <= 0)
	{
		sprintf(strBuf, "pics\\viewBorders\\bctl1.til");
		if (!topLeftCornerLarge.loadTIL(strBuf)) FUBAR("ERROR: Unable to load: %s", strBuf);
	}
	if (topLeftCornerSmall.getFrameCount() <= 0)
	{
		sprintf(strBuf, "pics\\viewBorders\\bctl2.til");
		if (!topLeftCornerSmall.loadTIL(strBuf)) FUBAR("ERROR: Unable to load: %s", strBuf);
	}

	// Top right corner.
	topRightCornerLarge.copy(topLeftCornerLarge);
	topRightCornerLarge.flipHorizontal();
	
	topRightCornerSmall.copy(topLeftCornerSmall);
	topRightCornerSmall.flipHorizontal();

	// Bottom left corner.
	bottomLeftCornerLarge.copy(topLeftCornerLarge);
	bottomLeftCornerLarge.flipVertical();
	
	bottomLeftCornerSmall.copy(topLeftCornerSmall);
	bottomLeftCornerSmall.flipVertical();

	// Bottom right corner.
	bottomRightCornerLarge.copy(topLeftCornerLarge);
	bottomRightCornerLarge.flipHorizontal();
	bottomRightCornerLarge.flipVertical();
	
	bottomRightCornerSmall.copy(topLeftCornerSmall);
	bottomRightCornerSmall.flipHorizontal();
	bottomRightCornerSmall.flipVertical();
#endif

#ifdef HAIRLINE_BORDERS
	borderSize = 1;
#endif

} // end View::View

// View
//---------------------------------------------------------------------------
View::View(const iXY &pos, const iXY &size, const char *title)
{
	reset();

	// If the button is bordered, then put a close button on it.
	//if (status & STATUS_ISBORDERED)
	//LOG(("About to addButton"));
	//addButton(iXY(getSizeX()-10, getSizeY()-10), "pics\\buttons\\wclose.raw", "Close the current window", View::toggleWindow);
	//LOG(("Past addButton"));

	moveTo(pos);
	resize(size);
	setTitle(title);
	loadPics();

} // end View::View

// ~View
//---------------------------------------------------------------------------
View::~View()
{
	free(title);
	free(subTitle);
	free(labels);

} // end ~View::View

// reset
//---------------------------------------------------------------------------
// Purpose: Reset all the settings for this window.
//---------------------------------------------------------------------------
void View::reset()
{
	min                   =  0;
	max                   =  0;
	status                =  0;
	numLabels             =  0;
	pressedButton         = -1;
	prevPressedButton     = -1;
	highlightedButton     = -1;
	prevHighlightedButton = -1;
	selectedInputField    = -1;
	searchName            = 0;
	title                 = 0;
	subTitle              = 0;
	statusText            = 0;
	focusComponent        = 0;

	setSearchName("donut");
	setTitle("donut");
	setSubTitle("donut");

	buttons               = 0;
	//scrollBar             = 0;
	labels                = 0;
	componentsUsedCount   = 0;
	
	assert(MAX_COMPONENT_COUNT > 0);
	memset(componentList, 0, sizeof(Component *) * MAX_COMPONENT_COUNT);

	moveAreaHeight  = DEFAULT_MOVE_AREA_HEIGHT;
	borderSize      = DEFAULT_BORDER_SIZE;
	snapToTolerance = DEFAULT_SNAP_TOLERANCE;

	setAllowResize(true);
	setAllowMove(true);
	setBordered(true);
	setVisible(false);
	setDisplayStatusBar(false);
	setAlwaysOnBottom(false);

	//setScrollBar(false);
	setAllowAutoResize(false);

} // end reset

// drawBorder
//---------------------------------------------------------------------------
// Purpose: Draws the borders of the window.
//---------------------------------------------------------------------------
void View::drawBorder(const Surface &viewArea)
{
	assert(isValidPtr(this));

#ifdef GRAPHIC_BORDERS
	// Straight edges.
	topBorder.blt(viewArea, topLeftCornerLarge.getPix().x, 2);

	int yOffset = viewArea.getPix().y - bottomBorder.getPix().y - 2;
	bottomBorder.blt(viewArea, bottomLeftCornerSmall.getPix().x, yOffset);

	leftBorder.blt(viewArea, 2, topLeftCornerLarge.getPix().y + topLeftCornerSmall.getPix().y);
	
	int xOffset = viewArea.getPix().x - rightBorder.getPix().x - 2;
	rightBorder.blt(viewArea, xOffset, topRightCornerLarge.getPix().y + topRightCornerSmall.getPix().y);

	// Corners.
	topLeftCornerLarge.blt(viewArea);
	topLeftCornerSmall.blt(viewArea, 0, topLeftCornerLarge.getPix().y);

	xOffset = viewArea.getPix().x - topRightCornerLarge.getPix().x;
	topRightCornerLarge.blt(viewArea, xOffset, 0);
	
	xOffset = viewArea.getPix().x - topRightCornerSmall.getPix().x;
	topRightCornerSmall.blt(viewArea, xOffset, topLeftCornerLarge.getPix().y);

	yOffset = viewArea.getPix().y - bottomLeftCornerLarge.getPix().y;
	bottomLeftCornerLarge.blt(viewArea, 0, yOffset);
	yOffset = viewArea.getPix().y - bottomLeftCornerLarge.getPix().y - bottomLeftCornerSmall.getPix().y;
	bottomLeftCornerSmall.blt(viewArea, 0, yOffset);

	xOffset = viewArea.getPix().x - bottomRightCornerLarge.getPix().x;
	yOffset = viewArea.getPix().y - bottomRightCornerLarge.getPix().y;
	bottomRightCornerLarge.blt(viewArea, xOffset, yOffset);
	xOffset = viewArea.getPix().x - bottomRightCornerSmall.getPix().x;
	yOffset = viewArea.getPix().y - bottomRightCornerSmall.getPix().y - bottomRightCornerLarge.getPix().y;
	bottomRightCornerSmall.blt(viewArea, xOffset, yOffset);
#endif

#ifdef STANDARD_BORDERS
	viewArea.drawWindowsBorder(Color::white, Color::lightGray, Color::gray);
#endif

#ifdef HAIRLINE_BORDERS
	viewArea.drawButtonBorder(Color::lightGreen, Color::darkGreen);
#endif

} // end drawBorder

// drawButtons
//---------------------------------------------------------------------------
// Purpose: Draws the buttons of the window.
//---------------------------------------------------------------------------
void View::drawButtons(const Surface &viewArea)
{
	assert(isValidPtr(this));

	//pics.setFrame(CLOSE);
	//pics.blt(viewArea, iXY(getSizeX()-pics.getPix().x-borderSize-2, borderSize+2));

	//pics.setFrame(MINIMIZE);
	//pics.blt(viewArea, getSizeX()-pics.xSize*2-borderSize*2-2, borderSize+2);

} // end View::drawButtons

// drawTitle
//---------------------------------------------------------------------------
// Purpose: Draws the title of the view and colors the move area background
//          depending on whether the view is active.
//---------------------------------------------------------------------------
void View::drawTitle(const Surface &viewArea)
{
	assert(isValidPtr(this));

	iRect r(borderSize, borderSize, getSizeX() - borderSize, borderSize + moveAreaHeight - 1);

	Surface s;
	s.setTo(viewArea, r);

	s.fill(getActive() ? activeWindowTitleBodyColor : inactiveWindowTitleBodyColor);

	char strBuf[256];
	
	sprintf(strBuf, "%s%s", title, subTitle);

	s.bltStringCenter(strBuf, getActive() ? activeWindowTitleTextColor : inactiveWindowTitleTextColor);

	if (getResize())
	{
		{
		// Bottom-Left resize.
		iXY verticalPos(borderSize, getSizeY() - borderSize - moveAreaHeight + 1);
		iXY horizontalPos(borderSize + moveAreaHeight - 1, getSizeY() - borderSize);

		viewArea.drawLine(verticalPos, horizontalPos, Color::white);
		verticalPos.y++;
		horizontalPos.x--;
		viewArea.drawLine(verticalPos, horizontalPos, Color::gray224);
		verticalPos.y++;
		horizontalPos.x--;
		viewArea.drawLine(verticalPos, horizontalPos, Color::gray192);
		verticalPos.y++;
		horizontalPos.x--;
		viewArea.drawLine(verticalPos, horizontalPos, Color::gray160);
		verticalPos.y++;
		horizontalPos.x--;
		viewArea.drawLine(verticalPos, horizontalPos, Color::gray160);
		verticalPos.y++;
		horizontalPos.x--;
		viewArea.drawLine(verticalPos, horizontalPos, Color::gray160);
		verticalPos.y++;
		horizontalPos.x--;
		viewArea.drawLine(verticalPos, horizontalPos, Color::gray160);
		verticalPos.y++;
		horizontalPos.x--;
		viewArea.drawLine(verticalPos, horizontalPos, Color::gray160);
		verticalPos.y++;
		horizontalPos.x--;
		viewArea.drawLine(verticalPos, horizontalPos, Color::gray160);
		verticalPos.y++;
		horizontalPos.x--;
		viewArea.drawLine(verticalPos, horizontalPos, Color::gray160);
		verticalPos.y++;
		horizontalPos.x--;
		viewArea.drawLine(verticalPos, horizontalPos, Color::gray160);
		verticalPos.y++;
		horizontalPos.x--;
		viewArea.drawLine(verticalPos, horizontalPos, Color::gray160);
		verticalPos.y++;
		horizontalPos.x--;
		}

		// Top-Left resize.
		{
		iXY verticalPos(borderSize, borderSize + moveAreaHeight - 1);
		iXY horizontalPos(borderSize + moveAreaHeight - 1, borderSize);

		viewArea.drawLine(verticalPos, horizontalPos, Color::white);
		verticalPos.y--;
		horizontalPos.x--;
		viewArea.drawLine(verticalPos, horizontalPos, Color::gray224);
		verticalPos.y--;
		horizontalPos.x--;
		viewArea.drawLine(verticalPos, horizontalPos, Color::gray192);
		verticalPos.y--;
		horizontalPos.x--;
		viewArea.drawLine(verticalPos, horizontalPos, Color::gray160);
		verticalPos.y--;
		horizontalPos.x--;
		viewArea.drawLine(verticalPos, horizontalPos, Color::gray160);
		verticalPos.y--;
		horizontalPos.x--;
		viewArea.drawLine(verticalPos, horizontalPos, Color::gray160);
		verticalPos.y--;
		horizontalPos.x--;
		viewArea.drawLine(verticalPos, horizontalPos, Color::gray160);
		verticalPos.y--;
		horizontalPos.x--;
		viewArea.drawLine(verticalPos, horizontalPos, Color::gray160);
		verticalPos.y--;
		horizontalPos.x--;
		viewArea.drawLine(verticalPos, horizontalPos, Color::gray160);
		verticalPos.y--;
		horizontalPos.x--;
		viewArea.drawLine(verticalPos, horizontalPos, Color::gray160);
		verticalPos.y--;
		horizontalPos.x--;
		viewArea.drawLine(verticalPos, horizontalPos, Color::gray160);
		verticalPos.y--;
		horizontalPos.x--;
		viewArea.drawLine(verticalPos, horizontalPos, Color::gray160);
		verticalPos.y--;
		horizontalPos.x--;
		}

		// Top-Right resize.
		{
		iXY verticalPos(getSizeX() - borderSize, borderSize + moveAreaHeight - 1);
		iXY horizontalPos(getSizeX() - borderSize - moveAreaHeight, borderSize - 1);

		viewArea.drawLine(verticalPos, horizontalPos, Color::white);
		verticalPos.y--;
		horizontalPos.x++;
		viewArea.drawLine(verticalPos, horizontalPos, Color::gray224);
		verticalPos.y--;
		horizontalPos.x++;
		viewArea.drawLine(verticalPos, horizontalPos, Color::gray192);
		verticalPos.y--;
		horizontalPos.x++;
		viewArea.drawLine(verticalPos, horizontalPos, Color::gray160);
		verticalPos.y--;
		horizontalPos.x++;
		viewArea.drawLine(verticalPos, horizontalPos, Color::gray160);
		verticalPos.y--;
		horizontalPos.x++;
		viewArea.drawLine(verticalPos, horizontalPos, Color::gray160);
		verticalPos.y--;
		horizontalPos.x++;
		viewArea.drawLine(verticalPos, horizontalPos, Color::gray160);
		verticalPos.y--;
		horizontalPos.x++;
		viewArea.drawLine(verticalPos, horizontalPos, Color::gray160);
		verticalPos.y--;
		horizontalPos.x++;
		viewArea.drawLine(verticalPos, horizontalPos, Color::gray160);
		verticalPos.y--;
		horizontalPos.x++;
		viewArea.drawLine(verticalPos, horizontalPos, Color::gray160);
		verticalPos.y--;
		horizontalPos.x++;
		viewArea.drawLine(verticalPos, horizontalPos, Color::gray160);
		verticalPos.y--;
		horizontalPos.x++;
		viewArea.drawLine(verticalPos, horizontalPos, Color::gray160);
		verticalPos.y--;
		horizontalPos.x++;
		}

		// Bottom-Right resize.
		{
		iXY verticalPos(getSizeX() - borderSize, getSizeY() - borderSize - moveAreaHeight + 1);
		iXY horizontalPos(getSizeX() - borderSize - moveAreaHeight + 1, getSizeY() - borderSize);

		viewArea.drawLine(verticalPos, horizontalPos, Color::white);
		verticalPos.y++;
		horizontalPos.x++;
		viewArea.drawLine(verticalPos, horizontalPos, Color::gray224);
		verticalPos.y++;
		horizontalPos.x++;
		viewArea.drawLine(verticalPos, horizontalPos, Color::gray192);
		verticalPos.y++;
		horizontalPos.x++;
		viewArea.drawLine(verticalPos, horizontalPos, Color::gray160);
		verticalPos.y++;
		horizontalPos.x++;
		viewArea.drawLine(verticalPos, horizontalPos, Color::gray160);
		verticalPos.y++;
		horizontalPos.x++;
		viewArea.drawLine(verticalPos, horizontalPos, Color::gray160);
		verticalPos.y++;
		horizontalPos.x++;
		viewArea.drawLine(verticalPos, horizontalPos, Color::gray160);
		verticalPos.y++;
		horizontalPos.x++;
		viewArea.drawLine(verticalPos, horizontalPos, Color::gray160);
		verticalPos.y++;
		horizontalPos.x++;
		viewArea.drawLine(verticalPos, horizontalPos, Color::gray160);
		verticalPos.y++;
		horizontalPos.x++;
		viewArea.drawLine(verticalPos, horizontalPos, Color::gray160);
		verticalPos.y++;
		horizontalPos.x++;
		viewArea.drawLine(verticalPos, horizontalPos, Color::gray160);
		verticalPos.y++;
		horizontalPos.x++;
		viewArea.drawLine(verticalPos, horizontalPos, Color::gray160);
		verticalPos.y++;
		horizontalPos.x++;
		}
	}

} // end View::drawTitle

// loadPics
//---------------------------------------------------------------------------
void View::loadPics()
{
	// Are we already loaded?  Then bail.
	if (pics.getFrame0() != 0) return;

	//pics.create(iXY(12, 11), 12, 2);
	//pics.extractPCX("pics\\buttons.pcx", 6, 1);

} // end loadPics

// draw
//---------------------------------------------------------------------------
void View::draw()
{
	assert(isValidPtr(this));

	if (getVisible())
	{
		doDraw(getViewArea(), getClientArea());
	}

} // end draw

// activate
//---------------------------------------------------------------------------
// Purpose: Default Implementation - Sets the window status to active.
//---------------------------------------------------------------------------
void View::activate()
{
	if (getActive())
	{
		return;
	}

	// Set the active bit.
	status |= STATUS_ACTIVE;

	doActivate();

} // end View::activate

// deactivate
//---------------------------------------------------------------------------
// Purpose: Sets the window status to non-active.
//---------------------------------------------------------------------------
void View::deactivate()
{
	if (!getActive())
	{
		return;
	}

	setActive(false);

	doDeactivate();
	
	highlightedButton     = -1;
	prevHighlightedButton = -1;
	selectedInputField    = -1;

} // end View::deactivate

// doDraw
//---------------------------------------------------------------------------
// Purpose: Default implementation - draws all the componentList of the window.
//---------------------------------------------------------------------------
void View::doDraw(const Surface &viewArea, const Surface &clientArea)
{
	if (getShowStatus()) 
	{
		drawStatus(clientArea);
	}

	//FIXME//drawButtons(viewArea);
	drawDefinedButtons(clientArea);
	drawInputFields(clientArea);
	drawLabels(clientArea);
	drawHighlightedButton(clientArea);
	drawPressedButton(clientArea);

	// Draw all non-selected components.
	for (int i = 0; i < componentsUsedCount; i++)
	{
		assert(componentList[i] != 0);

		if (componentList[i] != focusComponent)
		{
			componentList[i]->draw(clientArea);
		}
	}

	// Now draw the selected component.
	if (focusComponent != 0)
	{
		focusComponent->draw(clientArea);
	}

	if (getBordered()) 
	{
		drawTitle(viewArea);
		drawBorder(viewArea);
	}

	//drawToolTip(clientArea);

} // end View::doDraw

// doActivate
//---------------------------------------------------------------------------
// Purpose: Default Implementation -
//---------------------------------------------------------------------------
void View::doActivate()
{
	assert(isValidPtr(this));

	// Tell all the components the mouse entered the view.
	for (int i = 0; i < componentsUsedCount; i++)
	{
		assert(componentList[i] != 0);

		if (componentList[i]->contains(mouse.getScreenPos()))
		{
			mMouseEvent me(componentList[i], mMouseEvent::MOUSE_EVENT_ENTERED, now(), 0, mouse.getScreenX(), mouse.getScreenY(), 0, false);

			componentList[i]->actionPerformed(me);

			actionPerformed(me);
		}
	}

} // end View::doActivate

// doDeactivate
//---------------------------------------------------------------------------
// Purpose: Default Implementation -
//---------------------------------------------------------------------------
void View::doDeactivate()
{
	assert(isValidPtr(this));
		
	// Tell all the components the mouse exited the view.
	for (int i = 0; i < componentsUsedCount; i++)
	{
		mMouseEvent me(componentList[i], mMouseEvent::MOUSE_EVENT_EXITED, now(), 0, mouse.getScreenX(), mouse.getScreenY(), 0, false);

		componentList[i]->actionPerformed(me);

		actionPerformed(me);
	}

} // end View::doDeactivate

// getMouseActions
//---------------------------------------------------------------------------
// Purpose: Find the actions associated with the position of the mouse arrow
//          cursor in the window.
//---------------------------------------------------------------------------
int View::getMouseActions(const iXY &pos) const
{
	assert(isValidPtr(this));

	int actions = 0;

	if (!getVisible() || !getActive())
	{
		return actions;
	}

	if (!getBordered())
	{
		return actions;
	}

	if (getResize())
	{
		// Now lets check for a window resize
		iXY minOff;
		iXY maxOff;

		minOff.x = abs(pos.x); maxOff.x = abs(pos.x - getSizeX());
		minOff.y = abs(pos.y); maxOff.y = abs(pos.y - getSizeY());

		// Check the left,  then the right
		if      (minOff.x < RESIZE_XMIN) actions |= MA_RESIZE_LEFT;
		else if (maxOff.x < RESIZE_XMIN) actions |= MA_RESIZE_RIGHT;

		// Check the top, then the bottom
		if      (minOff.y < RESIZE_YMIN) actions |= MA_RESIZE_TOP;
		else if (maxOff.y < RESIZE_YMIN) actions |= MA_RESIZE_BOTTOM;

		// If a left or right was turned on, then give more area for selecting the
		// resize area
		if (actions & (MA_RESIZE_LEFT | MA_RESIZE_RIGHT))
		{
			if      (minOff.y < RESIZE_YMAX) actions |= MA_RESIZE_TOP;
			else if (maxOff.y < RESIZE_YMAX) actions |= MA_RESIZE_BOTTOM;
		}

		if (actions & (MA_RESIZE_TOP | MA_RESIZE_BOTTOM))
		{
			if      (minOff.x < RESIZE_XMAX) actions |= MA_RESIZE_LEFT;
			else if (maxOff.x < RESIZE_XMAX) actions |= MA_RESIZE_RIGHT;
		}
	}

	// Check for moving the window via the move area.
	if (getAllowMove())
	{
		//iRect titleBar(borderSize, borderSize, getSizeX()-borderSize, borderSize + moveAreaHeight);
		iRect titleBar(borderSize, borderSize, getSizeX() - borderSize, borderSize + moveAreaHeight);
		if (titleBar.contains(pos)) return actions |= MA_MOVE;
	}

	// Check for moving the window via the status bar.
	//if (getAllowMove())
	//{
	//	//iRect titleBar(borderSize, borderSize, getSizeX()-borderSize, borderSize + moveAreaHeight);
	//	iRect titleBar(borderSize, getSizeY() - borderSize - DEFAULT_STATUS_BAR_HEIGHT, getSizeX() - borderSize, getSizeY() - borderSize);
	//	if (titleBar.contains(pos)) return actions |= MA_MOVE;
	//}

	return actions;
} // end getMouseActions

// getScreenToClientPos
//---------------------------------------------------------------------------
// Purpose: Finds the position of the mouse in the client area of a window.
//          If this function gets called, we can assume that the mouse is in
//          client area of the window.  IF the window is borderless, then
//          adjust to the window area.
//---------------------------------------------------------------------------
iXY View::getScreenToClientPos(const iXY &pos)
{
	assert(isValidPtr(this));

	if (getBordered())
	{
		return iXY(	pos.x - (min.x + borderSize),
					pos.y - (min.y + borderSize + moveAreaHeight));
	}
	
	return getScreenToViewPos(pos);

} // end View::getScreenToClientPos

// getScreenToViewPos
//---------------------------------------------------------------------------
// Purpose: Finds the position of the mouse in the client area of a window.
//          If this function gets called, we can assume that the mouse is in
//          client area of the window.
//---------------------------------------------------------------------------
iXY View::getScreenToViewPos(const iXY &pos)
{
	assert(isValidPtr(this));

	return iXY(pos.x - min.x, pos.y - min.y);

} // end getScreenToViewPos

// getViewArea
//---------------------------------------------------------------------------
// Purpose: Sets the specified surface to the dimensions of the view.
//---------------------------------------------------------------------------
void View::getViewArea(Surface &dest)
{
	assert(isValidPtr(this));

	iRect rect(min, max);
	
	//iRect rect;
	//checkSnapToEdges(min, max, rect.min, rect.max);
	//dest.setTo(screen, rect);

	dest.setTo(screen, rect);

} // end getViewArea

// checkSnapToEdges
//---------------------------------------------------------------------------
// Purpose: Checks to see if the edges of the window is close enough to the
//          set snap-to tolerance and if so, new values are returned for
//          the view position.
//---------------------------------------------------------------------------
void View::checkSnapToEdges(const iXY &min, const iXY &max, iXY &newMin, iXY &newMax)
{
	const int SNAP_TOLERANCE = 20;

	newMin = min;
	newMax = max;
	
	iXY size(newMax - newMin);

	// Snap To Screen edges!
	if (min.x < SNAP_TOLERANCE)
	{
		newMin.x = 0;
		newMax.x = size.x;
	}
	else if (max.x > SCREEN_XPIX - SNAP_TOLERANCE)
	{
		newMin.x = SCREEN_XPIX - 1 - size.x;
		newMax.x = SCREEN_XPIX - 1;
	}

	if (min.y < SNAP_TOLERANCE)
	{
		newMin.y = 0;
		newMax.y = size.y;
	}
	else if (max.y > SCREEN_YPIX - SNAP_TOLERANCE)
	{
		newMin.y = SCREEN_YPIX - 1 - size.y;
		newMax.y = SCREEN_YPIX - 1;
	}

} // end View::checkSnapToEdges

// getViewArea
//---------------------------------------------------------------------------
// Purpose: Returns a Surface of the view's dimensions.
//---------------------------------------------------------------------------
Surface View::getViewArea()
{
	assert(isValidPtr(this));

	iRect rect(min, max);
	//iRect rect;
	//checkSnapToEdges(min, max, rect.min, rect.max);

	return Surface(screen, rect.min, rect.max, false);

} // end View::getViewArea

// getViewRect
//---------------------------------------------------------------------------
// Purpose: Returns an iRect of the view's dimensions.
//---------------------------------------------------------------------------
iRect View::getViewRect() const
{
	return iRect(0, 0, getSizeX(), getSizeY());

} // end getViewRect

// getClientArea
//---------------------------------------------------------------------------
// Purpose: Sets the destination surface to the specified coodinates within
//          the source surface.  If the window does not have borders, then
//          set the coordinates to the whole window area.
//---------------------------------------------------------------------------
void View::getClientArea(Surface &dest)
{
	assert(isValidPtr(this));

	if (getBordered())
	{
		dest.setTo(	getViewArea(),
					iRect(	borderSize,
							borderSize + moveAreaHeight,
							getSizeX() - borderSize,
							getSizeY() - borderSize));
	} else
	{
		getViewArea(dest);
	}

} // end View::getClientArea

// getClientArea
//---------------------------------------------------------------------------
// Purpose:
//---------------------------------------------------------------------------
Surface View::getClientArea()
{
	if (getBordered())
	{
		return Surface(	getViewArea(),
						iXY(borderSize,
							borderSize + moveAreaHeight),
						iXY(getSizeX() - borderSize,
							getSizeY() - borderSize),
						false);
	}

	return getViewArea();

} // end View::getClientArea

// getClientRect
//---------------------------------------------------------------------------
// Purpose:
//---------------------------------------------------------------------------
iRect View::getClientRect() const
{
	if (getBordered())
	{
		return iRect(	borderSize,
						borderSize + moveAreaHeight,
						getSizeX() - borderSize,
						getSizeY() - borderSize);
	}

	return iRect(0, 0, getSizeX(), getSizeY());

} // end View::getClientRect

// getMouseCursor
//---------------------------------------------------------------------------
// Purpose: Tells you the current state of the mouse cursor.
//---------------------------------------------------------------------------
Surface *View::getMouseCursor(const iXY & /* pos */)
{
	// Draw the mouse pointer last
	return &mouseArrow;

} // end View::getMouseCursor

// mouseMove
//---------------------------------------------------------------------------
// Purpse: Changes the current highlighted button and checks for mouseEnter
//         and mouseExit status.
//---------------------------------------------------------------------------
void View::mouseMove(const iXY & prevPos, const iXY &newPos)
{
	prevHighlightedButton = highlightedButton;
	highlightedButton     = findButtonContaining(newPos);

	// Check all components for a clicked event.
	for (int i = 0; i < componentsUsedCount; i++)
	{
		assert(componentList[i] != 0);
		
		if (componentList[i]->contains(prevPos) && !componentList[i]->contains(newPos))
		{
			mMouseEvent me(componentList[i], mMouseEvent::MOUSE_EVENT_EXITED, now(), 0, newPos.x, newPos.y, 0, false);

			componentList[i]->actionPerformed(me);

			actionPerformed(me);
		}
		else if (!componentList[i]->contains(prevPos) && componentList[i]->contains(newPos))
		{
			mMouseEvent me(componentList[i], mMouseEvent::MOUSE_EVENT_ENTERED, now(), 0, newPos.x, newPos.y, 0, false);

			componentList[i]->actionPerformed(me);

			actionPerformed(me);
		}
		else if (componentList[i]->contains(newPos))
		{
			mMouseEvent me(componentList[i], mMouseEvent::MOUSE_EVENT_MOVED, now(), 0, newPos.x, newPos.y, 0, false);

			componentList[i]->actionPerformed(me);
			
			actionPerformed(me);
		}
	}

/*  // THIS DOES NOT WORK!!!!!!!!!!
	bool flagNewPos  = contains(newPos);
	bool flagPrevPos = contains(prevPos);

	// Check for status of mouseEnter and mouseExit.
	if (flagNewPos && !flagPrevPos)
	{
		mouseEnter(newPos);
	}
	else if (!flagNewPos && flagPrevPos)
	{
		mouseExit(prevPos);
	}
*/
} // end mouseMove

// lMouseDown
//---------------------------------------------------------------------------
void View::lMouseDown(const iXY &pos)
{
	if (pressedButton < 0)
	{
		prevPressedButton = pressedButton;
		pressedButton     = findButtonContaining(pos);
	}

	if (selectedInputField < 0)
	{
		selectedInputField = findInputFieldContaining(pos);
	}
	
	focusComponent = 0;

	// Check all components for a pressed event.
	for (int i = 0; i < componentsUsedCount; i++)
	{
		assert(componentList[i] != 0);
		if (componentList[i]->contains(pos))
		{
			mMouseEvent me(componentList[i], mMouseEvent::MOUSE_EVENT_PRESSED, now(), InputEvent::BUTTON1_MASK, pos.x, pos.y, 0, false);

			componentList[i]->actionPerformed(me);

			focusComponent = componentList[i];

			actionPerformed(me);
		}
	}

} // end View::lMouseDown

// lMouseUp
//---------------------------------------------------------------------------
int View::lMouseUp(const iXY &downPos, const iXY &upPos)
{
	Desktop::resetMouseActionOffset();

	if (pressedButton == findButtonContaining(upPos))
	{
		if (pressedButton >= 0)
		{
				//if (mouse.getCurButton() & 0)
			if (buttons[pressedButton].leftClickFunc != 0) buttons[pressedButton].leftClickFunc();
		  //else
			//if (buttons[pressedButton].rightClickFunc != 0) buttons[pressedButton].rightClickFunc();
		}
	}

	prevPressedButton = pressedButton;
	pressedButton     = -1;

	// Check all components for a clicked event.
	{for (int i = 0; i < componentsUsedCount; i++)
	{
		assert(componentList[i] != 0);
		if (componentList[i]->contains(downPos) && componentList[i]->contains(upPos))
		{
			mMouseEvent me(componentList[i], mMouseEvent::MOUSE_EVENT_CLICKED, now(), InputEvent::BUTTON1_MASK, upPos.x, upPos.y, 0, false);

			componentList[i]->actionPerformed(me);

			actionPerformed(me);
		}
	}}

	// Report a mouse release to all components except for a clicked one.
	{for (int i = 0; i < componentsUsedCount; i++)
	{
		assert(componentList[i] != 0);
		if (!componentList[i]->contains(upPos))
		{
			mMouseEvent me(componentList[i], mMouseEvent::MOUSE_EVENT_RELEASED, now(), InputEvent::BUTTON1_MASK, upPos.x, upPos.y, 0, false);

			componentList[i]->actionPerformed(me);

			actionPerformed(me);
		}
	}}
	
	// I added this so I can tell if a button was pressed when
	// the mouse button is let up.
	return prevPressedButton;

} // end View::lMouseUp

// lMouseDrag
//---------------------------------------------------------------------------
void View::lMouseDrag(const iXY &downPos, const iXY &prevPos, const iXY &newPos)
{
	//lMouseDown(newPos);
	//mouseMove(prevPos, newPos);

	// Check all components for a dragged event.
	for (int i = 0; i < componentsUsedCount; i++)
	{
		assert(componentList[i] != 0);
		if (componentList[i]->contains(newPos) && componentList[i]->contains(downPos))
		{
			mMouseEvent me(componentList[i], mMouseEvent::MOUSE_EVENT_DRAGGED, now(), InputEvent::BUTTON1_MASK, newPos.x, newPos.y, 0, false);

			componentList[i]->actionPerformed(me);

			focusComponent = componentList[i];

			actionPerformed(me);
		}
	}

} // end View::lMouseDrag

// lMouseDouble
//---------------------------------------------------------------------------
void View::lMouseDouble(const iXY &pos)
{
	lMouseDown(pos);

} // end View::lMouseDouble

// rMouseDown
//---------------------------------------------------------------------------
void View::rMouseDown(const iXY &pos)
{
	focusComponent = 0;

	// Check all components for a pressed event.
	for (int i = 0; i < componentsUsedCount; i++)
	{
		assert(componentList[i] != 0);
		if (componentList[i]->contains(pos))
		{
			mMouseEvent me(componentList[i], mMouseEvent::MOUSE_EVENT_PRESSED, now(), InputEvent::BUTTON2_MASK, pos.x, pos.y, 0, false);

			componentList[i]->actionPerformed(me);

			focusComponent = componentList[i];

			actionPerformed(me);
		}
	}

} // end View::rMouseDown

// rMouseUp
//---------------------------------------------------------------------------
void View::rMouseUp(const iXY &downPos, const iXY &upPos)
{
	int reportRelease = true;

	// Check all components for a clicked event.
	for (int i = 0; i < componentsUsedCount; i++)
	{
		assert(componentList[i] != 0);
		if (componentList[i]->contains(downPos) && componentList[i]->contains(upPos))
		{
			mMouseEvent me(componentList[i], mMouseEvent::MOUSE_EVENT_CLICKED, now(), InputEvent::BUTTON2_MASK, upPos.x, upPos.y, 0, false);

			componentList[i]->actionPerformed(me);

			reportRelease = false;

			actionPerformed(me);
		}
	}

	// If we are on the last component and there was no component containing both
	// mouse press positions, then report a release to an
	if (reportRelease)
	{
		// Check all components for a release event.
		for (int i = 0; i < componentsUsedCount; i++)
		{
			assert(componentList[i] != 0);
			if (componentList[i]->contains(upPos))
			{
				mMouseEvent me(componentList[i], mMouseEvent::MOUSE_EVENT_RELEASED, now(), InputEvent::BUTTON2_MASK, upPos.x, upPos.y, 0, false);

				componentList[i]->actionPerformed(me);

				reportRelease = false;

				actionPerformed(me);
			}
		}
	}

} // end View::rMouseUp

// rMouseUp
//---------------------------------------------------------------------------
void View::rMouseUp()
{
} // end View::rMouseUp

// rMouseDrag
//---------------------------------------------------------------------------
void View::rMouseDrag(const iXY &downPos, const iXY &prevPos, const iXY &newPos)
{
	rMouseDown(newPos);

	focusComponent = 0;

	// Check all components for a moved event.
	for (int i = 0; i < componentsUsedCount; i++)
	{
		assert(componentList[i] != 0);
		if (componentList[i]->contains(newPos))
		{
			mMouseEvent me(componentList[i], mMouseEvent::MOUSE_EVENT_DRAGGED, now(), InputEvent::BUTTON2_MASK, newPos.x, newPos.y, 0, false);

			componentList[i]->actionPerformed(me);

			focusComponent = componentList[i];

			actionPerformed(me);
		}
	}

} // end View::rMouseDrag

// rMouseDouble
//---------------------------------------------------------------------------
void View::rMouseDouble(const iXY &pos)
{
	rMouseDown(pos);

} // end View::rMouseDouble

// mouseEnter
//---------------------------------------------------------------------------
// Purpose: Is triggered when the mouse enters the window.
//---------------------------------------------------------------------------
void View::mouseEnter(const iXY &pos)
{
	// Check all components for a entered event.
	for (int i = 0; i < componentsUsedCount; i++)
	{
		assert(componentList[i] != 0);
		if (componentList[i]->contains(pos))
		{
			mMouseEvent me(componentList[i], mMouseEvent::MOUSE_EVENT_ENTERED, now(), 0, pos.x, pos.y, 0, false);

			componentList[i]->actionPerformed(me);

			actionPerformed(me);
		}
	}

} // end View::mouseEnter

// mouseExit
//---------------------------------------------------------------------------
// Purpose: Is triggered when the mouse leaves the window.
//---------------------------------------------------------------------------
void View::mouseExit(const iXY &pos)
{
	assert(isValidPtr(this));

	// Check all components for a exited event.
	for (int i = 0; i < componentsUsedCount; i++)
	{
		assert(componentList[i] != 0);
		if (componentList[i]->contains(pos))
		{
			mMouseEvent me(componentList[i], mMouseEvent::MOUSE_EVENT_EXITED, now(), 0, pos.x, pos.y, 0, false);

			componentList[i]->actionPerformed(me);

			actionPerformed(me);
		}
	}

} // end View::mouseExit

// scrollBarMove
//---------------------------------------------------------------------------
// Purpose: 
//---------------------------------------------------------------------------
void View::scrollBarMove(const iXY &prevPos, const iXY &newPos)
{
	//if (scrollBar != 0)
	//{
	//	scrollBar->actionPerformed(newPos);
	//}

} // end scrollBarMove

// addLabel
//---------------------------------------------------------------------------
// Purpose:
//---------------------------------------------------------------------------
void View::addLabel(const iXY &pos, char *label, const PIX &color)
{
	addLabel(pos, label, false, color, Color::black);

} // end addLabel

// addLabelShadowed
//---------------------------------------------------------------------------
// Purpose:
//---------------------------------------------------------------------------
void View::addLabelShadowed(const iXY &pos, char *label, const PIX &foreColor, 
                              const PIX &backColor)
{
	addLabel(pos, label, true, foreColor, backColor);
} // end addLabelShadowed

// addLabel
//---------------------------------------------------------------------------
// Purpose:
//---------------------------------------------------------------------------
void View::addLabel(const iXY &pos, char *label, const bool &isShadowed, const PIX &foreColor, 
                      const PIX &backColor)
{
	labels = (cLabel *) realloc(labels, (numLabels+1)*sizeof(cLabel));
	if (labels == 0) FUBAR("ERROR: Unable to allocate label.");

	labels[numLabels].label = strdup(label);
	assert(labels[numLabels].label != 0);
	labels[numLabels].foreColor  = foreColor;
	labels[numLabels].backColor  = backColor;
	labels[numLabels].isShadowed = isShadowed;

	labels[numLabels].pos = pos;
	numLabels++;

} // end View::addLabel

// DRAW LABELS
//---------------------------------------------------------------------------
// Purpose:
//---------------------------------------------------------------------------
void View::drawLabels(const Surface &clientArea)
{
	if (!clientArea.getDoesExist())
	{
		FUBAR("ERROR: Client area does not exist.");
	}

	for (int num = 0; num < numLabels; num++)
	{
		if (labels[num].isShadowed)
		{
			clientArea.bltStringShadowed(labels[num].pos.x, labels[num].pos.y, labels[num].label, labels[num].foreColor, labels[num].backColor);
		}
		else clientArea.bltString(labels[num].pos.x, labels[num].pos.y, labels[num].label, labels[num].foreColor);
	}
} // end DRAW LABELS

// drawDefinedButtons
//---------------------------------------------------------------------------
// Purpose:
//---------------------------------------------------------------------------
void View::drawDefinedButtons(const Surface &clientArea)
{
	for (int num = 0; num < buttons.getCount(); num++)
	{
		buttons[num].topSurface.blt(clientArea, iXY(buttons[num].getBounds().min.x, buttons[num].getBounds().min.y));
	}
} // end drawDefinedButtons

// drawInputFields
//---------------------------------------------------------------------------
// Purpose:
//---------------------------------------------------------------------------
void View::drawInputFields(const Surface &clientArea)
{
	for (int num = 0; num < inputFields.getCount(); num++)
	{
		if (num == selectedInputField)
		{
			inputFields[num].drawHighlighted(clientArea);
		} else
		{
			inputFields[num].draw(clientArea);
		}
	}
} // end drawInputFields

// drawHighlightedButton
//---------------------------------------------------------------------------
// Purpose: Draws the correct image frame for the currently highlighted button.
//---------------------------------------------------------------------------
void View::drawHighlightedButton(const Surface &clientArea)
{
	assert(isValidPtr(this));

	if (highlightedButton < 0)
	{
		return;
	}
	else if (buttons[highlightedButton].topSurface.getFrameCount() < 2)
	{
		clientArea.drawRect(iRect(buttons[highlightedButton].getBounds().min.x,
								  buttons[highlightedButton].getBounds().min.y,
								  buttons[highlightedButton].getBounds().max.x,
								  buttons[highlightedButton].getBounds().max.y),
								  Color::red);
		return;
	}

	if (pressedButton == highlightedButton)
	{
		return;
	}

	if (highlightedButton > buttons.getCount())
	{
		FUBAR("ERROR: highlightedButton > butons.getCount()");
	}

	// Change to the highlight button frame.
	buttons[highlightedButton].topSurface.setFrame(1);
	buttons[highlightedButton].topSurface.blt(clientArea, iXY(buttons[highlightedButton].getBounds().min.x, buttons[highlightedButton].getBounds().min.y));
	buttons[highlightedButton].topSurface.setFrame(0);

} // end drawHighlightedButton

void View::addButtonPackedSurface(const iXY &pos, PackedSurface &source, const char *toolTip, ITEM_FUNC leftClickFunc)
{
	buttons.setNum(buttons.getCount() + 1);
	cButton &b = buttons[buttons.getCount() - 1];
	b.createPacked(pos, source, toolTip, leftClickFunc);
}

// addButtonCenterText
//---------------------------------------------------------------------------
// Purpose: Adds a centered type button to the list of buttons.
//---------------------------------------------------------------------------
void View::addButtonCenterText(const iXY &pos, 
                       const int &xSize, 
                       const char *name, 
                       const char *toolTip,
                       ITEM_FUNC leftClickFunc)
{
	// Add a new button to the button list.
	buttons.setNum(buttons.getCount() + 1);
	cButton &b = buttons[buttons.getCount() - 1];
	b.createCenterText(pos, xSize, name, toolTip, leftClickFunc);
} // end addButtonCenterText

// addButtonTIL
//---------------------------------------------------------------------------
// Purpose: Adds a button of a TIL image.
//---------------------------------------------------------------------------
void View::addButtonTIL(const iXY &pos, const char *imageName, const char *toolTip, ITEM_FUNC func, const bool &isBordered)
{
	// Add a new button to the button list.
	buttons.setNum(buttons.getCount() + 1);
	
	cButton &b = buttons[buttons.getCount() - 1];
	
	if (isBordered)
	{
		b.createTILBordered(pos, imageName, toolTip, func);

	} else
	{
		b.createTIL(pos, imageName, toolTip, func);
	}

} // end addButtonTIL

// addButtonSurface
//---------------------------------------------------------------------------
// Purpose: Adds a button of a Surface image.
//---------------------------------------------------------------------------
void View::addButtonSurface(const iXY &pos, Surface &source, const char *toolTip, ITEM_FUNC func)
{
	// Add a new button to the button list.
	buttons.setNum(buttons.getCount() + 1);
	
	cButton &b = buttons[buttons.getCount() - 1];
	
	b.createSurface(pos, source, toolTip, func);

} // end addButtonSurface

// addButtonSurfaceSingle
//---------------------------------------------------------------------------
// Purpose: Adds a button of a Surface image.
//---------------------------------------------------------------------------
void View::addButtonSurfaceSingle(const iXY &pos, Surface &source, const char *toolTip, ITEM_FUNC func)
{
	// Add a new button to the button list.
	buttons.setNum(buttons.getCount() + 1);
	
	cButton &b = buttons[buttons.getCount() - 1];
	
	b.createSurfaceSingle(pos, source, toolTip, func);

} // end addButtonSurfaceSingle

// ADD cButton
//---------------------------------------------------------------------------
// Purpose:
//---------------------------------------------------------------------------
void View::addButton(const iXY &pos, const char *name, const char *toolTip,
											 ITEM_FUNC func)
{
	FUNCF(("View::addButton(pos={%d,%d}, name=%s, toolTip=%s, func=%p)", pos.x, pos.y, name, toolTip, func));
	assert(isValidPtr(this));

	// Grow the list
	buttons.setNum(buttons.getCount()+1);
	cButton &b = buttons[buttons.getCount()-1];
	b.create(pos, name, toolTip, func);
} // end ADD cButton

// addButtonPCX
//---------------------------------------------------------------------------
// Purpose:  Makes a button out of a PCX image.
//---------------------------------------------------------------------------
void View::addButtonPCX(const iXY &pos, 
                       const char *filename, 
                       const char *toolTip,
                       ITEM_FUNC func)
{
	assert(isValidPtr(this));

	// Grow the list.
	buttons.setNum(buttons.getCount()+1);
	cButton &b = buttons[buttons.getCount()-1];

	// Create the button.
	b.createPCX(pos, filename, toolTip, func);
} // end addButtonPCX

//---------------------------------------------------------------------------
void View::addButton(	const iXY &pos, 
						const ANIMATED_BUTTON_TYPE_PCX &topButton, 
						const ANIMATED_BUTTON_TYPE_PCX &bottomButton,
						const char *toolTip,
						ITEM_FUNC leftClickFunc, 
						ITEM_FUNC rightClickFunc)
{
	assert(isValidPtr(this));

	// Grow the list
	buttons.setNum(buttons.getCount()+1);
	cButton &b = buttons[buttons.getCount()-1];
	b.create(pos, topButton, bottomButton, toolTip, leftClickFunc, rightClickFunc);
} // end ADD cButton

// setSearchName
//---------------------------------------------------------------------------
// Purpose: Sets the title of the window.
//---------------------------------------------------------------------------
void View::setSearchName(const char *searchName)
{
	if (View::searchName != 0)
	{
		free(View::searchName);
		View::searchName = 0;
	}

	if (searchName != 0)
	{
		View::searchName = strdup(searchName);
		if (View::searchName == 0)
		{
			FUBAR("ERROR: Unable to allocate searchName: ", searchName);
		}
	}
} // end View::setSearchName

// setTitle
//---------------------------------------------------------------------------
// Purpose: Sets the title of the window.
//---------------------------------------------------------------------------
void View::setTitle(const char *title)
{
	if (View::title != 0)
	{
		free(View::title);
		View::title = 0;
	}

	if (title != 0)
	{
		View::title = strdup(title);
		if (View::title == 0)
		{
			FUBAR("ERROR: Unable to allocate title: ", title);
		}
	}
} // end View::setTitle

// setSubTitle
//---------------------------------------------------------------------------
// Purpose: Sets the subTitle of the window.
//---------------------------------------------------------------------------
void View::setSubTitle(const char *subTitle)
{
	if (View::subTitle != 0)
	{
		free(View::subTitle);
		View::subTitle = 0;
	}

	if (subTitle != 0)
	{
		View::subTitle = strdup(subTitle);
		if (View::subTitle == 0)
		{
			FUBAR("ERROR: Unable to allocate subTitle: ", subTitle);
		}
	}
} // end View::setSubTitle

// showStatus
//---------------------------------------------------------------------------
// Purpose: Sets the status bar text.
//---------------------------------------------------------------------------
void View::showStatus(const char *string)
{
	if (statusText != 0)
	{
		free(statusText);
		statusText = 0;
	}

	if (string != 0)
	{
		statusText = strdup(string);
		if(statusText == 0) { FUBAR("ERROR: statusText == 0"); }
	}

} // end View::showStatus

// drawStatus
//---------------------------------------------------------------------------
void View::drawStatus(const Surface &dest)
{
	// Draw the status bar.
	iRect clientRect(getClientRect());

	iRect r(0,
			clientRect.getSizeY() - DEFAULT_STATUS_BAR_HEIGHT,
			clientRect.getSizeX(),
			clientRect.getSizeY());

	Surface s;
	s.setTo(dest, r);
	s.fill(Color::gray192);

	// Draw the status text.
	if (statusText != 0)
	{
		int pos = (DEFAULT_STATUS_BAR_HEIGHT - CHAR_YPIX) >> 1;

		s.bltString(pos, pos, statusText, Color::black);
	}

} // end View::drawStatus

// findButtonContaining
//---------------------------------------------------------------------------
// Purpose: Finds the button containing the position of the mouse.  The
//          button are bounded relative to the window, so make sure to make
//          the mouse position relative to the window before calling this.
//---------------------------------------------------------------------------
int View::findButtonContaining(const iXY &pos)
{
	assert(isValidPtr(this));

	for (int num = 0; num < buttons.getCount(); num++)
	{
		if (buttons[num].contains(pos))
		{
			//LOG(("pressed button: %u", num));
			return num;
		}
	}
	return -1;
} // end findButtonContaining

// findInputFieldContaining
//---------------------------------------------------------------------------
// Purpose: 
//---------------------------------------------------------------------------
int View::findInputFieldContaining(const iXY &pos)
{
	assert(isValidPtr(this));

	for (int num = 0; num < inputFields.getCount(); num++)
	{
		if (inputFields[num].contains(pos))
		{
			return num;
		}
	}
	return -1;
} // end findInputFieldContaining

// drawPressedButton
//---------------------------------------------------------------------------
// Purpose: Draws the selected button.
//---------------------------------------------------------------------------
void View::drawPressedButton(const Surface &clientArea)
{
	assert(isValidPtr(this));

	if (pressedButton < 0 || buttons[pressedButton].topSurface.getFrameCount() < 2) return;
	if (highlightedButton != pressedButton) return;
	assert(pressedButton < buttons.getCount());

	// Chage to the highlight button frame.
	buttons[pressedButton].topSurface.setFrame(2);
        buttons[pressedButton].topSurface.blt(clientArea, iXY(buttons[pressedButton].getBounds().min.x,
                                                          buttons[pressedButton].getBounds().min.y));
	buttons[pressedButton].topSurface.setFrame(0);
} // drawPressedButton

// DRAW TOOL TIP
//---------------------------------------------------------------------------
// Purpose: Draws the tooltip of the button the mouse is currently over.
//---------------------------------------------------------------------------
void View::drawToolTip(const Surface &dest)
{
	assert(isValidPtr(this));

	iRect rect;
	rect.min.x = 0;
	rect.min.y = getClientRect().max.y - moveAreaHeight + 2;
	rect.max.x = getClientRect().getSizeX()+1;
	rect.max.y = getClientRect().max.y+1;
	int xOffset = 5;

	if (highlightedButton < 0) 
	{
		if (statusText != 0)
		{
			iRect tRect(rect.min.x, rect.min.y, rect.max.x, rect.max.y);
			dest.fillRect(tRect, Color::black);
			dest.drawButtonBorder(tRect, Color::gray32, Color::gray64);
			dest.bltString(rect.min.x+xOffset+1, rect.min.y+2, statusText, Color::black);
			dest.bltString(rect.min.x+xOffset, rect.min.y+1, statusText, Color::white);
		}
		return;
	} else
	{
		if (strlen(buttons[highlightedButton].getToolTip()) == 0)
		{
			return;
		}

		int length = strlen(buttons[highlightedButton].getToolTip()) * CHAR_XPIX;

		// min is this window's min derived from iRect.
		iRect r(buttons[highlightedButton].getBounds());
		iRect r2(r.min.x + min.x, r.max.y + min.y, r.min.x + length + 4 + min.x, r.max.y + CHAR_YPIX + 4 + min.y);

		// Keep the tooltip on the screen as best as possible.
		if (r2.max.x > SCREEN_XPIX)
		{
			int xOffset = r2.max.x - SCREEN_XPIX;
			r2.min.x -= xOffset;
			r2.max.x -= xOffset;
		}

		dest.fillRect(r2, Color::black);
		dest.drawButtonBorder(r2, Color::white, Color::gray64);
		dest.bltString(r2.min.x + 2, r2.min.y + 2, buttons[highlightedButton].getToolTip(), Color::white);
		//dest.bltString(rect.min.x+xOffset, rect.min.y+1, buttons[highlightedButton].getToolTip(), Color::white);
	}
} // end drawToolTip

// drawToolTip
//---------------------------------------------------------------------------
// Purpose: Draws the tooltip of the button the mouse is currently over.
//---------------------------------------------------------------------------
void View::drawToolTip(const Surface &dest, const char *toolTip)
{
	if (strlen(toolTip) == 0)
	{
		return;
	}

	dest.bltString(4, 1, toolTip,      0);
	dest.bltString(3, 0, toolTip, Color::white);
} // end drawToolTip

// centerAbsolute
//---------------------------------------------------------------------------
// Purpose: Centers the window horizontally and vertically on the screen.
//---------------------------------------------------------------------------
void View::centerAbsolute()
{
	iXY oldSize = getSize();

	min.x = (SCREEN_XPIX - oldSize.x) >> 1;
	min.y = (SCREEN_YPIX - oldSize.y) >> 1;
	max.x = min.x + oldSize.x;
	max.y = min.y + oldSize.y;
} // end centerAbsolute

// DRAW TOOL TIP
//---------------------------------------------------------------------------
// Purpose: Draws a tool tip associated with the currently highlighted
//          button.
//---------------------------------------------------------------------------
//void View::drawToolTip()
//{
//	if (highlightedButton < 0) return;
//
//	CHECK(strlen(buttons[highlightedButton].getToolTip()) < 80, "ERROR: Tool tip text too long.");
//
//	char strBuf[80];
//	sprintf(strBuf, "%s", buttons[highlightedButton].getToolTip());
//	bltString(screen, mouse.getScreenPos().x+1, mouse.getScreenPos().y-10+1, strBuf, Color::black);
//	bltString(screen, mouse.getScreenPos().x, mouse.getScreenPos().y-10, strBuf, pal.cBLUE);
//} // end DRAW TOOL TIP

// RESIZE CLIENT AREA
//---------------------------------------------------------------------------
// Purpose: Resize the client area of the window.  The window area will be 
//          resized accordingly.
//---------------------------------------------------------------------------
void View::resizeClientArea(const iXY &size)
{
	// These variables constitue the gap space needed for the window borders and
	// move area.
	int xExtra = borderSize * 2;
	int yExtra = xExtra + moveAreaHeight;

	iXY destSize(size);

	// Add the area for the borders and move area.
	destSize.x += xExtra;
	destSize.y += yExtra;

	// Make sure the window will fit on the screen in the specified size.
	if (destSize.x > SCREEN_XPIX) { FUBAR("ERROR: Trying to size the client area larger than the screen width."); }
	if (destSize.y > SCREEN_YPIX) { FUBAR("ERROR: Trying to size the client area larger than the screen height."); }

	// Move the window accordingly to accomodate for the resize.
	if (min.x + destSize.x > SCREEN_XPIX)
	{
		min.x += (SCREEN_XPIX - min.x + destSize.x);
	}
	if (min.y + destSize.y > SCREEN_YPIX)
	{
		min.y += (SCREEN_YPIX - min.y + destSize.y);
	}

	resize(destSize);

} // end View::resizeClientArea

// RESIZE
//---------------------------------------------------------------------------
// Purpose: Resizes the current window to the specified size.
//---------------------------------------------------------------------------
void View::resize(const iXY &size)
{
	iXY destSize(size);

	if      (destSize.x <= RESIZE_XMINSIZE)
	{
		destSize.y = SCREEN_YPIX - min.y;
	}

	else if (destSize.x >  SCREEN_XPIX - min.x)
	{
		destSize.x = SCREEN_XPIX - min.x;
	}

	if      (destSize.y <= RESIZE_YMINSIZE)
	{
		destSize.y = RESIZE_YMINSIZE;
	}
	else if (destSize.y >  SCREEN_YPIX - min.y)
	{
		destSize.y = SCREEN_YPIX - min.y;
	}

	// Are we already at the desired size?  Then bail.
	if (size == getSize())
	{
		return;
	}

	max = min + destSize;

} // end View::resize

// setPressedButton
//---------------------------------------------------------------------------
void View::setPressedButton(const int &button)
{ 
	if (button >= buttons.getCount())
	{
		FUBAR("ERROR: pressedButton >= numButtons");
	}
	
	prevPressedButton = pressedButton;
	pressedButton     = button;

} // end setPressedButton

// setHighlightedButton
//---------------------------------------------------------------------------
void View::setHighlightedButton(const int &button)
{ 
	if (button >= buttons.getCount())
	{
		FUBAR("ERROR: highlightedButton >= numButtons");
	}

	prevHighlightedButton = highlightedButton;
	highlightedButton     = button;

} // end setHighlightedButton

// checkViewSize
//---------------------------------------------------------------------------
// Purpose: If the window's dimension is too big or too small, then resize 
//          the window accordingly.
//---------------------------------------------------------------------------
void View::checkViewSize(iXY &pix)
{
	if      (pix.x >  SCREEN_XPIX)
	{
		pix.x = SCREEN_XPIX;
	}
	else if (pix.x <= RESIZE_XMINSIZE)
	{
		pix.x = RESIZE_XMINSIZE;
	}
	if      (pix.y >  SCREEN_YPIX)
	{
		pix.y = SCREEN_YPIX;
	}
	else if (pix.y <= RESIZE_YMINSIZE)
	{
		pix.y = RESIZE_YMINSIZE;
	}

} // end View::checkViewSize

// moveTo
//---------------------------------------------------------------------------
// Purpose: Move the window to the specified position, while making sure the 
//          window is on the screen.
//---------------------------------------------------------------------------
void View::moveTo(iXY destMin)
{
	if (getAllowAutoResize())
	{
		if (getSizeX() > SCREEN_XPIX)
		{
			fXY difference;

			difference.x = getSizeX() - SCREEN_XPIX;

			float xRatio = float(difference.x) / float(getSizeX());

			resize(getSizeX() - difference.x, float(getSizeY()) * xRatio);

		} else if (getSizeY() > SCREEN_YPIX)
		{
			fXY difference;

			difference.y = getSizeY() - SCREEN_YPIX;

			float yRatio = float(difference.y) / float(getSizeY());

			resize(float(getSizeY()) * yRatio, getSizeY() - difference.y);
		}
	}

	// Used until I rewrite this to keep the view in the size of the screen.
	int flagFuckMax = 1;

	// Keep the windows within the bounds of the screen
	if (destMin.x > SCREEN_XPIX - getSizeX())
	{
		destMin.x = SCREEN_XPIX - getSizeX();
	}
	if (destMin.x < 0) 
	{
		destMin.x = 0;
		if (destMin.x + getSizeX() - 1 >= SCREEN_XPIX)
		{
			flagFuckMax = 0;
			max.x = SCREEN_XPIX - 1;
			//FUBAR("ERROR: The window is too large for the screen.");
		}
		
	}

	if (destMin.y > SCREEN_YPIX - getSizeY())
	{
		destMin.y = SCREEN_YPIX - getSizeY();
	}
	if (destMin.y < 0)
	{
		destMin.y = 0;
		if (destMin.y + getSizeY() - 1 >= SCREEN_YPIX)
		{
			flagFuckMax = 0;
			max.y = SCREEN_YPIX - 1;

			//FUBAR("ERROR: The window is too large for the screen.");
		}
		
	}

	if (flagFuckMax)
	{
		max += destMin - min;
	}

	min  = destMin;

} // end moveTo

// toggleView
//---------------------------------------------------------------------------
// Purpose: Toggle the visibility of this window.
//---------------------------------------------------------------------------
void View::toggleView()
{
  Desktop::toggleVisibility(searchName);

} // end toggleView

// setAllowResize
//---------------------------------------------------------------------------
// Purpose: Allows this view to be resized or not.
//---------------------------------------------------------------------------
void View::setAllowResize(const bool &newStatus)
{
	if (newStatus == true) status |=  STATUS_ALLOW_RESIZE;
	else                   status &= ~STATUS_ALLOW_RESIZE;
} // end setAllowResize

// setAllowAutoResize
//---------------------------------------------------------------------------
// Purpose: Allows this view to be resized or not.
//---------------------------------------------------------------------------
void View::setAllowAutoResize(const bool &newStatus)
{
	if (newStatus == true) status |=  STATUS_ALLOW_AUTO_RESIZE;
	else                   status &= ~STATUS_ALLOW_AUTO_RESIZE;
} // end setAllowAutoResize

// setDisplayStatusBar
//---------------------------------------------------------------------------
// Purpose: Displays a status bar in the view or not.
//---------------------------------------------------------------------------
void View::setDisplayStatusBar(const bool &newStatus)
{
	if (newStatus == true) status |=  STATUS_DISPLAY_STATUS_BAR;
	else                   status &= ~STATUS_DISPLAY_STATUS_BAR;
} // setDisplayStatusBar

// setAlwaysOnBottom
//---------------------------------------------------------------------------
// Purpose: Makes this view always drawn on the bottom or not.
//---------------------------------------------------------------------------
void View::setAlwaysOnBottom(const bool &newStatus)
{
	if (newStatus == true) status |=  STATUS_ALWAYS_ON_BOTTOM;
	else                   status &= ~STATUS_ALWAYS_ON_BOTTOM;
} // setAlwaysOnBottom

// setBordered
//---------------------------------------------------------------------------
// Purpose: Turns on the borders for this view or not.
//---------------------------------------------------------------------------
void View::setBordered(const bool &newStatus)
{
	if (newStatus == true) status |=  STATUS_BORDERED;
	else                   status &= ~STATUS_BORDERED;
} // end setBordered

// setAllowMove
//---------------------------------------------------------------------------
void View::setAllowMove(const bool &newStatus)
{
	if (newStatus == true) status |=  STATUS_ALLOW_MOVE;
	else                   status &= ~STATUS_ALLOW_MOVE;
} // end setAllowMove

// setVisible
//---------------------------------------------------------------------------
// Purpose: Makes this view visible or not.
//---------------------------------------------------------------------------
void View::setVisible(const bool &newStatus)
{
	if (newStatus == true) status |=  STATUS_VISIBLE;
	else                   status &= ~STATUS_VISIBLE;
} // end setVisible

// setActive
//---------------------------------------------------------------------------
// Purpose: Sets this view as an active view or not.
//---------------------------------------------------------------------------
void View::setActive(const bool &newStatus)
{
	if (newStatus == true) status |=  STATUS_ACTIVE;
	else                   status &= ~STATUS_ACTIVE;
} // end setActive

// processEvents
//---------------------------------------------------------------------------
void View::processEvents(void)
{
	if (selectedInputField >= 0)
	{
		char shit;
		while (KeyboardInterface::getChar(&shit))
		{
			// Check for extended code.
			if (shit == 0)
			{
				if (KeyboardInterface::getChar(&shit))
				{
					inputFields[selectedInputField].addExtendedChar(shit);
				} else
				{
					FUBAR("ERROR: Expecting extended char code.");
				}
			} else
			{
				inputFields[selectedInputField].addChar(shit);
			}
		}
	}
} // end processEvents

// addInputField
//---------------------------------------------------------------------------
void View::addInputField(
						  const iXY &pos, 
						  cInputFieldString *string, 
						  const char *excludedCharacters, 
						  const bool &isSelected)
{
	inputFields.setNum(inputFields.getCount() + 1);
	inputFields[inputFields.getCount() - 1].setPos(pos);
	inputFields[inputFields.getCount() - 1].setInputFieldString(string);
	inputFields[inputFields.getCount() - 1].setExcludedCharacters(excludedCharacters);

	if (isSelected)
	{
		selectedInputField = inputFields.getCount() - 1;
	}

} // end addInputField

//---------------------------------------------------------------------------
void View::add(DEFAULT_VIEW_BUTTON button)
{
	if (button == CLOSE_VIEW_BUTTON)
	{
	}
	else if (button == MINMAX_VIEW_BUTTON)
	{
	} else
	{
		assert(false);
	}
}

//void View::add(Component *Component)
//{
//	assert(isValidPtr(this));
//	assert(isValidPtr(Component));
//
//	// First remove it from the list if we already have it somehow.
//	remove(Component);
//
//	Component **prevLink = &top;
//	
//	while (*prevLink != 0)
//	{
//		prevLink = &(*prevLink)->next;
//	}
//
//	*prevLink    = Component;
//	Component->next = 0;
//
//} // end View::add
//
//// remove
////---------------------------------------------------------------------------
//// Purpose:
////---------------------------------------------------------------------------
//void View::remove(Component *Component)
//{
//	Component **prevLink = &top;
//
//	while (*prevLink != 0)
//	{
//		if (*prevLink == Component)
//		{
//			*prevLink = Component->next;
//			break;
//		}
//
//		prevLink = &(*prevLink)->next;
//	}
//
//} // end View::remove
