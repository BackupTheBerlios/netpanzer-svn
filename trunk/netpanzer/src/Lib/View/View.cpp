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
#include "ViewGlobals.hpp"
#include "Exception.hpp"
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

// add
//---------------------------------------------------------------------------
void View::add(Component *component)
{
    assert(component != 0);

    if (componentsUsedCount < MAX_COMPONENT_COUNT - 1) {
        componentList[componentsUsedCount] = component;
        componentList[componentsUsedCount]->setParent(this);
        componentsUsedCount++;
    } else {
        throw Exception("ERROR: Trying to add more componentList than pre-allocated, increase the pre-allocated amount.");
    }

} // end View::add

// View
//---------------------------------------------------------------------------
View::View()
    : currentscreen(0)
{
    reset();
    loadPics();
} // end View::View

// View
//---------------------------------------------------------------------------
View::View(const iXY &pos, const iXY &size, const char *title)
{
    reset();

    // If the button is bordered, then put a close button on it.
    //if (status & STATUS_ISBORDERED)
    //LOG(("About to addButton"));
    //addButton(iXY(getSizeX()-10, getSizeY()-10), "pics/buttons/wclose.raw", "Close the current window", View::toggleWindow);
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
    min.zero();
    max.zero();
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
} // end reset

// drawBorder
//---------------------------------------------------------------------------
// Purpose: Draws the borders of the window.
//---------------------------------------------------------------------------
void View::drawBorder(Surface &viewArea)
{
    assert(this != 0);
    
    viewArea.drawWindowsBorder(Color::white, Color::lightGray, Color::gray);
} // end drawBorder

// drawButtons
//---------------------------------------------------------------------------
// Purpose: Draws the buttons of the window.
//---------------------------------------------------------------------------
void View::drawButtons(Surface &viewArea)
{
    assert(this != 0);

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
void View::drawTitle(Surface &viewArea)
{
    assert(this != 0);

    iRect r(borderSize, borderSize, getSizeX() - borderSize, borderSize + moveAreaHeight - 1);

    Surface s;
    s.setTo(viewArea, r);

    s.fill(getActive() ? activeWindowTitleBodyColor : inactiveWindowTitleBodyColor);

    char strBuf[256];

    sprintf(strBuf, "%s%s", title, subTitle);

    s.bltStringCenter(strBuf, getActive() ? activeWindowTitleTextColor : inactiveWindowTitleTextColor);

    if (getResize()) {
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
    //pics.extractPCX("pics/buttons.pcx", 6, 1);

} // end loadPics

// draw
//---------------------------------------------------------------------------
void View::draw(Surface& surface)
{
    assert(this != 0);

    if (!getVisible())
        return;

    currentscreen = &surface; // hack
    Surface* viewarea = getViewArea(surface);
    Surface* clientarea = getClientArea(surface);
    doDraw(*viewarea, *clientarea);
    delete viewarea;
    delete clientarea;
} // end draw

// activate
//---------------------------------------------------------------------------
// Purpose: Default Implementation - Sets the window status to active.
//---------------------------------------------------------------------------
void View::activate()
{
    if (getActive()) {
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
    if (!getActive()) {
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
void View::doDraw(Surface &viewArea, Surface &clientArea)
{
    if (getShowStatus()) {
        drawStatus(clientArea);
    }

    //FIXME//drawButtons(viewArea);
    drawDefinedButtons(clientArea);
    drawInputFields(clientArea);
    drawLabels(clientArea);
    drawHighlightedButton(clientArea);
    drawPressedButton(clientArea);

    // Draw all non-selected components.
    for (int i = 0; i < componentsUsedCount; i++) {
        assert(componentList[i] != 0);

        if (componentList[i] != focusComponent) {
            componentList[i]->draw(clientArea);
        }
    }

    // Now draw the selected component.
    if (focusComponent != 0) {
        focusComponent->draw(clientArea);
    }

    if (getBordered()) {
        drawTitle(viewArea);
        drawBorder(viewArea);
    }
} // end View::doDraw

// doActivate
//---------------------------------------------------------------------------
// Purpose: Default Implementation -
//---------------------------------------------------------------------------
void View::doActivate()
{
    assert(this != 0);

    // Tell all the components the mouse entered the view.
    for (int i = 0; i < componentsUsedCount; i++) {
        assert(componentList[i] != 0);

        if (componentList[i]->contains(mouse.getScreenPos())) {
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
    assert(this != 0);

    // Tell all the components the mouse exited the view.
    for (int i = 0; i < componentsUsedCount; i++) {
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
    assert(this != 0);

    int actions = 0;

    if (!getVisible() || !getActive()) {
        return actions;
    }

    if (!getBordered()) {
        return actions;
    }

    if (getResize()) {
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
        if (actions & (MA_RESIZE_LEFT | MA_RESIZE_RIGHT)) {
            if      (minOff.y < RESIZE_YMAX) actions |= MA_RESIZE_TOP;
            else if (maxOff.y < RESIZE_YMAX) actions |= MA_RESIZE_BOTTOM;
        }

        if (actions & (MA_RESIZE_TOP | MA_RESIZE_BOTTOM)) {
            if      (minOff.x < RESIZE_XMAX) actions |= MA_RESIZE_LEFT;
            else if (maxOff.x < RESIZE_XMAX) actions |= MA_RESIZE_RIGHT;
        }
    }

    // Check for moving the window via the move area.
    if (getAllowMove()) {
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
    assert(this != 0);

    if (getBordered()) {
        return iXY( pos.x - (min.x + borderSize),
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
    assert(this != 0);

    return iXY(pos.x - min.x, pos.y - min.y);
} // end getScreenToViewPos

// getViewArea
//---------------------------------------------------------------------------
// Purpose: Returns a Surface of the view's dimensions.
//---------------------------------------------------------------------------
Surface* View::getViewArea(Surface& dest)
{
    assert(this != 0);

    iRect rect(min, max);
    
    return new Surface(dest, rect.min, rect.max, false);
} // end View::getViewArea

// getClientArea
//---------------------------------------------------------------------------
// Purpose:
//---------------------------------------------------------------------------
Surface* View::getClientArea(Surface& dest)
{
    Surface* viewarea = getViewArea(dest);
    if (getBordered()) {
        Surface* surface = new Surface(*viewarea,
                        iXY(borderSize, borderSize+moveAreaHeight),
                        iXY(getSizeX() - borderSize,
                            getSizeY() - borderSize), false);
        delete viewarea;
        return surface;
    }

    return viewarea;
} // end View::getClientArea

// getClientRect
//---------------------------------------------------------------------------
// Purpose:
//---------------------------------------------------------------------------
iRect View::getClientRect() const
{
    if (getBordered()) {
        return iRect( borderSize,
                      borderSize + moveAreaHeight,
                      getSizeX() - borderSize,
                      getSizeY() - borderSize);
    }

    return iRect(0, 0, getSizeX(), getSizeY());

} // end View::getClientRect

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
    for (int i = 0; i < componentsUsedCount; i++) {
        assert(componentList[i] != 0);

        if (componentList[i]->contains(prevPos) && !componentList[i]->contains(newPos)) {
            mMouseEvent me(componentList[i], mMouseEvent::MOUSE_EVENT_EXITED, now(), 0, newPos.x, newPos.y, 0, false);

            componentList[i]->actionPerformed(me);

            actionPerformed(me);
        } else if (!componentList[i]->contains(prevPos) && componentList[i]->contains(newPos)) {
            mMouseEvent me(componentList[i], mMouseEvent::MOUSE_EVENT_ENTERED, now(), 0, newPos.x, newPos.y, 0, false);

            componentList[i]->actionPerformed(me);

            actionPerformed(me);
        } else if (componentList[i]->contains(newPos)) {
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
    if (pressedButton < 0) {
        prevPressedButton = pressedButton;
        pressedButton     = findButtonContaining(pos);
    }

    if (selectedInputField < 0) {
        selectedInputField = findInputFieldContaining(pos);
    }

    focusComponent = 0;

    // Check all components for a pressed event.
    for (int i = 0; i < componentsUsedCount; i++) {
        assert(componentList[i] != 0);
        if (componentList[i]->contains(pos)) {
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

    if (pressedButton == findButtonContaining(upPos)) {
        if (pressedButton >= 0) {
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
            if (componentList[i]->contains(downPos) && componentList[i]->contains(upPos)) {
                mMouseEvent me(componentList[i], mMouseEvent::MOUSE_EVENT_CLICKED, now(), InputEvent::BUTTON1_MASK, upPos.x, upPos.y, 0, false);

                componentList[i]->actionPerformed(me);

                actionPerformed(me);
            }
        }}

    // Report a mouse release to all components except for a clicked one.
    {for (int i = 0; i < componentsUsedCount; i++)
        {
            assert(componentList[i] != 0);
            if (!componentList[i]->contains(upPos)) {
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
    for (int i = 0; i < componentsUsedCount; i++) {
        assert(componentList[i] != 0);
        if (componentList[i]->contains(newPos) && componentList[i]->contains(downPos)) {
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
    for (int i = 0; i < componentsUsedCount; i++) {
        assert(componentList[i] != 0);
        if (componentList[i]->contains(pos)) {
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
    for (int i = 0; i < componentsUsedCount; i++) {
        assert(componentList[i] != 0);
        if (componentList[i]->contains(downPos) && componentList[i]->contains(upPos)) {
            mMouseEvent me(componentList[i], mMouseEvent::MOUSE_EVENT_CLICKED, now(), InputEvent::BUTTON2_MASK, upPos.x, upPos.y, 0, false);

            componentList[i]->actionPerformed(me);

            reportRelease = false;

            actionPerformed(me);
        }
    }

    // If we are on the last component and there was no component containing both
    // mouse press positions, then report a release to an
    if (reportRelease) {
        // Check all components for a release event.
        for (int i = 0; i < componentsUsedCount; i++) {
            assert(componentList[i] != 0);
            if (componentList[i]->contains(upPos)) {
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
{} // end View::rMouseUp

// rMouseDrag
//---------------------------------------------------------------------------
void View::rMouseDrag(const iXY &downPos, const iXY &prevPos, const iXY &newPos)
{
    rMouseDown(newPos);

    focusComponent = 0;

    // Check all components for a moved event.
    for (int i = 0; i < componentsUsedCount; i++) {
        assert(componentList[i] != 0);
        if (componentList[i]->contains(newPos)) {
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
    for (int i = 0; i < componentsUsedCount; i++) {
        assert(componentList[i] != 0);
        if (componentList[i]->contains(pos)) {
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
    assert(this != 0);

    // Check all components for a exited event.
    for (int i = 0; i < componentsUsedCount; i++) {
        assert(componentList[i] != 0);
        if (componentList[i]->contains(pos)) {
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
    if (labels == 0) throw Exception("ERROR: Unable to allocate label.");

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
void View::drawLabels(Surface &clientArea)
{
    if (!clientArea.getDoesExist()) {
        throw Exception("ERROR: Client area does not exist.");
    }

    for (int num = 0; num < numLabels; num++) {
        if (labels[num].isShadowed) {
            clientArea.bltStringShadowed(labels[num].pos.x, labels[num].pos.y, labels[num].label, labels[num].foreColor, labels[num].backColor);
        } else clientArea.bltString(labels[num].pos.x, labels[num].pos.y, labels[num].label, labels[num].foreColor);
    }
} // end DRAW LABELS

// drawDefinedButtons
//---------------------------------------------------------------------------
// Purpose:
//---------------------------------------------------------------------------
void View::drawDefinedButtons(Surface &clientArea)
{
    for (int num = 0; num < buttons.getCount(); num++) {
        buttons[num].topSurface.blt(clientArea, iXY(buttons[num].getBounds().min.x, buttons[num].getBounds().min.y));
    }
} // end drawDefinedButtons

// drawInputFields
//---------------------------------------------------------------------------
// Purpose:
//---------------------------------------------------------------------------
void View::drawInputFields(Surface &clientArea)
{
    for (int num = 0; num < inputFields.getCount(); num++) {
        if (num == selectedInputField) {
            inputFields[num].drawHighlighted(clientArea);
        } else {
            inputFields[num].draw(clientArea);
        }
    }
} // end drawInputFields

// drawHighlightedButton
//---------------------------------------------------------------------------
// Purpose: Draws the correct image frame for the currently highlighted button.
//---------------------------------------------------------------------------
void View::drawHighlightedButton(Surface &clientArea)
{
    assert(this != 0);

    if (highlightedButton < 0) {
        return;
    } else if (buttons[highlightedButton].topSurface.getFrameCount() < 2) {
        clientArea.drawRect(iRect(buttons[highlightedButton].getBounds().min.x,
                                  buttons[highlightedButton].getBounds().min.y,
                                  buttons[highlightedButton].getBounds().max.x,
                                  buttons[highlightedButton].getBounds().max.y),
                            Color::red);
        return;
    }

    if (pressedButton == highlightedButton) {
        return;
    }

    if (highlightedButton > buttons.getCount()) {
        throw Exception("ERROR: highlightedButton > butons.getCount()");
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

    if (isBordered) {
        b.createTILBordered(pos, imageName, toolTip, func);

    } else {
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

// setSearchName
//---------------------------------------------------------------------------
// Purpose: Sets the title of the window.
//---------------------------------------------------------------------------
void View::setSearchName(const char *searchName)
{
    if (View::searchName != 0) {
        free(View::searchName);
        View::searchName = 0;
    }

    if (searchName != 0) {
        View::searchName = strdup(searchName);
        if (View::searchName == 0) {
            throw Exception("ERROR: Unable to allocate searchName: %s", searchName);
        }
    }
} // end View::setSearchName

// setTitle
//---------------------------------------------------------------------------
// Purpose: Sets the title of the window.
//---------------------------------------------------------------------------
void View::setTitle(const char *title)
{
    if (View::title != 0) {
        free(View::title);
        View::title = 0;
    }

    if (title != 0) {
        View::title = strdup(title);
        if (View::title == 0) {
            throw Exception("ERROR: Unable to allocate title: %s", title);
        }
    }
} // end View::setTitle

// setSubTitle
//---------------------------------------------------------------------------
// Purpose: Sets the subTitle of the window.
//---------------------------------------------------------------------------
void View::setSubTitle(const char *subTitle)
{
    if (View::subTitle != 0) {
        free(View::subTitle);
        View::subTitle = 0;
    }

    if (subTitle != 0) {
        View::subTitle = strdup(subTitle);
        if (View::subTitle == 0) {
            throw Exception("ERROR: Unable to allocate subTitle: %s", subTitle);
        }
    }
} // end View::setSubTitle

// showStatus
//---------------------------------------------------------------------------
// Purpose: Sets the status bar text.
//---------------------------------------------------------------------------
void View::showStatus(const char *string)
{
    if (statusText != 0) {
        free(statusText);
        statusText = 0;
    }

    if (string != 0) {
        statusText = strdup(string);
        if(statusText == 0) {
            throw Exception("ERROR: statusText == 0");
        }
    }

} // end View::showStatus

// drawStatus
//---------------------------------------------------------------------------
void View::drawStatus(Surface &dest)
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
    if (statusText != 0) {
        int pos = (DEFAULT_STATUS_BAR_HEIGHT - Surface::getFontHeight()) >> 1;

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
    assert(this != 0);

    for (int num = 0; num < buttons.getCount(); num++) {
        if (buttons[num].contains(pos)) {
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
    assert(this != 0);

    for (int num = 0; num < inputFields.getCount(); num++) {
        if (inputFields[num].contains(pos)) {
            return num;
        }
    }
    return -1;
} // end findInputFieldContaining

// drawPressedButton
//---------------------------------------------------------------------------
// Purpose: Draws the selected button.
//---------------------------------------------------------------------------
void View::drawPressedButton(Surface &clientArea)
{
    assert(this != 0);

    if (pressedButton < 0 || buttons[pressedButton].topSurface.getFrameCount() < 2) return;
    if (highlightedButton != pressedButton) return;
    assert(pressedButton < buttons.getCount());

    // Chage to the highlight button frame.
    buttons[pressedButton].topSurface.setFrame(2);
    buttons[pressedButton].topSurface.blt(clientArea, iXY(buttons[pressedButton].getBounds().min.x,
                                          buttons[pressedButton].getBounds().min.y));
    buttons[pressedButton].topSurface.setFrame(0);
} // drawPressedButton

// checkResolution
//---------------------------------------------------------------------------
// Purpose: Check position after resolution change
//---------------------------------------------------------------------------
void View::checkResolution(iXY oldResolution, iXY newResolution)
{
    iXY size = getSize();
    
    min.x += (newResolution.x - oldResolution.x)/2;
    min.y += (newResolution.y - oldResolution.y)/2;
    max = min + size;
} // end checkResolution

void View::checkArea(iXY viewarea)
{
    if(min.x < 0)
        moveTo(0, min.y);
    if(min.y < 0)
        moveTo(min.x, 0);
                                                               
    if(max.x > viewarea.x)
        moveTo(viewarea.x - getSize().x, min.y);
    if(max.y > viewarea.y)
        moveTo(min.x, viewarea.y - getSize().y);
}

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

    resize(destSize);
} // end View::resizeClientArea

// RESIZE
//---------------------------------------------------------------------------
// Purpose: Resizes the current window to the specified size.
//---------------------------------------------------------------------------
void View::resize(const iXY &size)
{
    iXY destSize(size);

    max = min + destSize;
} // end View::resize

// setPressedButton
//---------------------------------------------------------------------------
void View::setPressedButton(const int &button)
{
    if (button >= buttons.getCount()) {
        throw Exception("ERROR: pressedButton >= numButtons");
    }

    prevPressedButton = pressedButton;
    pressedButton     = button;
} // end setPressedButton

// setHighlightedButton
//---------------------------------------------------------------------------
void View::setHighlightedButton(const int &button)
{
    if (button >= buttons.getCount()) {
        throw Exception("ERROR: highlightedButton >= numButtons");
    }

    prevHighlightedButton = highlightedButton;
    highlightedButton     = button;
} // end setHighlightedButton

// moveTo
//---------------------------------------------------------------------------
// Purpose: Move the window to the specified position, while making sure the
//          window is on the screen.
//---------------------------------------------------------------------------
void View::moveTo(iXY destMin)
{
    iXY size = getSize();
    min = destMin;
    max = min + size;
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
    if (selectedInputField >= 0) {
        int shit;
        while (KeyboardInterface::getChar(shit)) {
            // Check for extended code.
            if (shit == 0) {
                if (KeyboardInterface::getChar(shit)) {
                    inputFields[selectedInputField].addExtendedChar(shit);
                } else {
                    throw Exception("ERROR: Expecting extended char code.");
                }
            } else {
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

    if (isSelected) {
        selectedInputField = inputFields.getCount() - 1;
    }

} // end addInputField

//---------------------------------------------------------------------------
void View::add(DEFAULT_VIEW_BUTTON button)
{
    if (button == CLOSE_VIEW_BUTTON) {}
    else if (button == MINMAX_VIEW_BUTTON) {}
    else {
        assert(false);
    }
}

