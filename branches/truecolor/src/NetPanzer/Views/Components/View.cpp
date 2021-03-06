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


#include "2D/Color.hpp"

#include "Views/Components/View.hpp"
#include "Views/Components/Desktop.hpp"
#include "Views/Components/Label.hpp"
#include "Interfaces/MouseInterface.hpp"
#include "Interfaces/KeyboardInterface.hpp"
#include "Types/fXY.hpp"
#include "ViewGlobals.hpp"
#include "Util/Exception.hpp"
#include "InputEvent.hpp"
#include "Interfaces/GameConfig.hpp"
#include "Views/GameViewGlobals.hpp"
#include "2D/Palette.hpp"
#include "Classes/ScreenSurface.hpp"

const int RESIZE_WIDTH = 10;
const int RESIZE_XMIN  = RESIZE_WIDTH;
const int RESIZE_XMAX  = RESIZE_WIDTH * 3;
const int RESIZE_YMIN  = RESIZE_WIDTH;
const int RESIZE_YMAX  = RESIZE_WIDTH * 3;

// add
//---------------------------------------------------------------------------
void View::add(Component *component)
{
    if ( component ) {
        components.push_back(component);
        component->setParent(this);
    }
} // end View::add

// View
//---------------------------------------------------------------------------
View::View()
{
    reset();
} // end View::View

// View
//---------------------------------------------------------------------------
View::View(const iXY &pos, const iXY &size, const char *title)
{
    reset();
    moveTo(pos);
    resize(size);
    setTitle(title);

} // end View::View

// ~View
//---------------------------------------------------------------------------
View::~View()
{
    std::vector<cInputField*>::iterator f;
    for(f = inputFields.begin(); f != inputFields.end(); f++) {
        delete *f;
    }
    
    free(title);
    free(subTitle);
    free(searchName);
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
    selectedInputField    = -1;
    searchName            = 0;
    title                 = 0;
    subTitle              = 0;
    statusText            = 0;
    focusComponent        = 0;

    setSearchName("donut");
    setTitle("donut");
    setSubTitle("donut");

    removeComponents();

    moveAreaHeight  = DEFAULT_MOVE_AREA_HEIGHT;
    borderSize      = DEFAULT_BORDER_SIZE;
    snapToTolerance = DEFAULT_SNAP_TOLERANCE;

    setAllowMove(true);
    setBordered(true);
    setVisible(false);
    setDisplayStatusBar(false);
    setAlwaysOnBottom(false);
} // end reset

// drawBorder
//---------------------------------------------------------------------------
// Purpose: Draws the borders of the window.
//---------------------------------------------------------------------------
void View::drawBorder()
{
    assert(this != 0);
    screen->drawRect(iRect(min, max), Color::darkGray);
} // end drawBorder

// drawTitle
//---------------------------------------------------------------------------
// Purpose: Draws the title of the view and colors the move area background
//          depending on whether the view is active.
//---------------------------------------------------------------------------
void View::drawTitle()
{
    assert(this != 0);

    iRect r(borderSize, borderSize, getSizeX() - borderSize, borderSize + moveAreaHeight - 1);
    r.translate(min);

    screen->fillRect(r, getActive() ? activeWindowTitleBodyColor : inactiveWindowTitleBodyColor);

    char strBuf[256];

    sprintf(strBuf, "%s%s", title, subTitle);

    screen->bltStringCenteredInRect(r, strBuf, getActive() ? activeWindowTitleTextColor : inactiveWindowTitleTextColor);

} // end View::drawTitle

// draw
//---------------------------------------------------------------------------
void View::draw()
{
    if (!getVisible())
        return;

    doDraw();
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

    selectedInputField    = -1;

} // end View::deactivate

// doDraw
//---------------------------------------------------------------------------
// Purpose: Default implementation - draws all the componentList of the window.
//---------------------------------------------------------------------------
void View::doDraw()
{
    if (getShowStatus()) {
        drawStatus();
    }

    drawInputFields();

    // Draw all non-selected components.
    ComponentsIterator i;
    for ( i=components.begin(); i != components.end(); ++i)
    {
        if (*i != focusComponent)
        {
            (*i)->draw(clientRect.min.x, clientRect.min.y, *screen);
        }
    }

    // Now draw the selected component.
    if (focusComponent != 0)
    {
        focusComponent->draw(clientRect.min.x, clientRect.min.y, *screen);
    }

    if (getBordered())
    {
        drawTitle();
        drawBorder();
    }
} // end View::doDraw

// doActivate
//---------------------------------------------------------------------------
// Purpose: Default Implementation -
//---------------------------------------------------------------------------
void View::doActivate()
{
    // Tell all the components the mouse entered the view.
    ComponentsIterator i;
    for ( i=components.begin(); i != components.end(); ++i)
    {
        if ((*i)->contains(MouseInterface::getMousePosition()))
        {
            mMouseEvent me((*i),
                mMouseEvent::MOUSE_EVENT_ENTERED, now(), 0, 
                MouseInterface::getMouseX(), MouseInterface::getMouseY(), 0, false);

            (*i)->actionPerformed(me);

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
    ComponentsIterator i;
    for ( i=components.begin(); i != components.end(); i++)
    {
        mMouseEvent me((*i),
            mMouseEvent::MOUSE_EVENT_EXITED, now(), 0, 
            MouseInterface::getMouseX(), MouseInterface::getMouseY(), 0, false);

        (*i)->actionPerformed(me);

        actionPerformed(me);
    }
} // end View::doDeactivate

// getScreenToClientPos
//---------------------------------------------------------------------------
// Purpose: Finds the position of the mouse in the client area of a window.
//          If this function gets called, we can assume that the mouse is in
//          client area of the window.  IF the window is borderless, then
//          adjust to the window area.
//---------------------------------------------------------------------------
iXY View::getScreenToClientPos(const iXY &pos)
{
    return pos-clientRect.min;
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

    return pos-min;
} // end getScreenToViewPos

// getClientRect
//---------------------------------------------------------------------------
// Purpose:
//---------------------------------------------------------------------------
iRect View::getClientRect() const
{
    return clientRect;
} // end View::getClientRect

// mouseMove
//---------------------------------------------------------------------------
// Purpse: Changes the current highlighted button and checks for mouseEnter
//         and mouseExit status.
//---------------------------------------------------------------------------
void View::mouseMove(const iXY & prevPos, const iXY &newPos)
{
    // Check all components for a clicked event.
    ComponentsIterator i;
    for ( i=components.begin(); i != components.end(); i++)
    {        
        bool containsPrev=(*i)->contains(prevPos);
        bool containsNew=(*i)->contains(newPos);
        if (containsPrev && !containsNew) {
            mMouseEvent me((*i), mMouseEvent::MOUSE_EVENT_EXITED, now(), 0, newPos.x, newPos.y, 0, false);

            (*i)->actionPerformed(me);

            actionPerformed(me);
        } else if (!containsPrev && containsNew) {
            mMouseEvent me((*i), mMouseEvent::MOUSE_EVENT_ENTERED, now(), 0, newPos.x, newPos.y, 0, false);

            (*i)->actionPerformed(me);

            actionPerformed(me);
        } else if (containsNew && newPos!=prevPos) {
            mMouseEvent me((*i), mMouseEvent::MOUSE_EVENT_MOVED, now(), 0, newPos.x, newPos.y, 0, false);

            (*i)->actionPerformed(me);

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
    if (selectedInputField < 0) {
        selectedInputField = findInputFieldContaining(pos);
    }

    focusComponent = 0;

	// Check all components for a pressed event.
    ComponentsIterator i;
    for ( i=components.begin(); i != components.end(); i++)
    {        
        if ((*i)->contains(pos)) {
            mMouseEvent me((*i), mMouseEvent::MOUSE_EVENT_PRESSED, now(), InputEvent::BUTTON1_MASK, pos.x, pos.y, 0, false);

            (*i)->actionPerformed(me);

            focusComponent = (*i);

            actionPerformed(me);
        }
    }

} // end View::lMouseDown

// lMouseUp
//---------------------------------------------------------------------------
int View::lMouseUp(const iXY &downPos, const iXY &upPos)
{
    Desktop::resetMouseActionOffset();

    // Check all components for a clicked event.
    {    
        ComponentsIterator i;
        for ( i=components.begin(); i != components.end(); i++)
        {        
            if ((*i)->contains(downPos) && (*i)->contains(upPos)) {
                mMouseEvent me((*i), mMouseEvent::MOUSE_EVENT_CLICKED, now(), InputEvent::BUTTON1_MASK, upPos.x, upPos.y, 0, false);

                (*i)->actionPerformed(me);

                actionPerformed(me);
            }
        }}

    // Report a mouse release to all components except for a clicked one.
    {    
        ComponentsIterator i;
        for ( i=components.begin(); i != components.end(); i++)
        {        
            if ((*i)->contains(upPos)) {
                mMouseEvent me((*i), mMouseEvent::MOUSE_EVENT_RELEASED, now(), InputEvent::BUTTON1_MASK, upPos.x, upPos.y, 0, false);

                (*i)->actionPerformed(me);

                actionPerformed(me);
            }
        }}

    // I added this so I can tell if a button was pressed when
    // the mouse button is let up.
    return -1;

} // end View::lMouseUp

// lMouseDrag
//---------------------------------------------------------------------------
void View::lMouseDrag(const iXY &downPos, const iXY &prevPos, const iXY &newPos)
{
    //lMouseDown(newPos);
    //mouseMove(prevPos, newPos);

    // Check all components for a dragged event.
    ComponentsIterator i;
    for ( i=components.begin(); i != components.end(); i++)
    {        
        if ((*i)->contains(newPos) && (*i)->contains(downPos) && newPos!=prevPos) {
            mMouseEvent me((*i), mMouseEvent::MOUSE_EVENT_DRAGGED, now(), InputEvent::BUTTON1_MASK, newPos.x, newPos.y, 0, false);

            (*i)->actionPerformed(me);

            focusComponent = (*i);

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
    ComponentsIterator i;
    for ( i=components.begin(); i != components.end(); i++)
    {        
        if ((*i)->contains(pos)) {
            mMouseEvent me((*i), mMouseEvent::MOUSE_EVENT_PRESSED, now(), InputEvent::BUTTON2_MASK, pos.x, pos.y, 0, false);

            (*i)->actionPerformed(me);

            focusComponent = (*i);

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
    ComponentsIterator i;
    for ( i=components.begin(); i != components.end(); i++)
    {        
        if ((*i)->contains(downPos) && (*i)->contains(upPos)) {
            mMouseEvent me((*i), mMouseEvent::MOUSE_EVENT_CLICKED, now(), InputEvent::BUTTON2_MASK, upPos.x, upPos.y, 0, false);

            (*i)->actionPerformed(me);

            reportRelease = false;

            actionPerformed(me);
        }
    }

    // If we are on the last component and there was no component containing both
    // mouse press positions, then report a release to an
    if (reportRelease) {
        // Check all components for a release event.
        ComponentsIterator i;
        for ( i=components.begin(); i != components.end(); i++)
        {        
            if ((*i)->contains(upPos)) {
                mMouseEvent me((*i), mMouseEvent::MOUSE_EVENT_RELEASED, now(), InputEvent::BUTTON2_MASK, upPos.x, upPos.y, 0, false);

                (*i)->actionPerformed(me);

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
    ComponentsIterator i;
    for ( i=components.begin(); i != components.end(); i++)
    {        
        if ((*i)->contains(newPos)  && (*i)->contains(downPos) && newPos!=prevPos) {
            mMouseEvent me((*i), mMouseEvent::MOUSE_EVENT_DRAGGED, now(), InputEvent::BUTTON2_MASK, newPos.x, newPos.y, 0, false);

            (*i)->actionPerformed(me);

            focusComponent = (*i);

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
    ComponentsIterator i;
    for ( i=components.begin(); i != components.end(); i++)
    {        
        if ((*i)->contains(pos)) {
            mMouseEvent me((*i), mMouseEvent::MOUSE_EVENT_ENTERED, now(), 0, pos.x, pos.y, 0, false);

            (*i)->actionPerformed(me);

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
    ComponentsIterator i;
    for ( i=components.begin(); i != components.end(); i++)
    {        
        if ((*i)->contains(pos)) {
            mMouseEvent me((*i), mMouseEvent::MOUSE_EVENT_EXITED, now(), 0, pos.x, pos.y, 0, false);

            (*i)->actionPerformed(me);

            actionPerformed(me);
        }
    }

} // end View::mouseExit

// drawInputFields
//---------------------------------------------------------------------------
// Purpose:
//---------------------------------------------------------------------------
void View::drawInputFields()
{
    for (size_t num = 0; num < inputFields.size(); num++) {
        if (num == (size_t) selectedInputField) {
            inputFields[num]->drawHighlighted();
        } else {
            inputFields[num]->draw();
        }
        inputFields[num]->inputFieldSurface.blt(*screen,
                            clientRect.min.x + inputFields[num]->bounds.min.x,
                            clientRect.min.y + inputFields[num]->bounds.min.y);
    }
} // end drawInputFields

// setSearchName
//---------------------------------------------------------------------------
// Purpose: Sets the title of the window.
//---------------------------------------------------------------------------
void View::setSearchName(const char *searchName)
{
    free(View::searchName);
    View::searchName = 0;

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
void View::drawStatus()
{
    iRect r( min.x, max.y - DEFAULT_STATUS_BAR_HEIGHT, max.x, max.y);

    screen->fillRect(r, Color::gray192);

    // Draw the status text.
    if (statusText != 0) {
        int pos = (DEFAULT_STATUS_BAR_HEIGHT - Surface::getFontHeight()) >> 1;
        screen->bltString(r.min.x + pos, r.min.y + pos, statusText, Color::black);
    }
} // end View::drawStatus

// findInputFieldContaining
//---------------------------------------------------------------------------
// Purpose:
//---------------------------------------------------------------------------
int View::findInputFieldContaining(const iXY &pos)
{
    assert(this != 0);

    for(size_t num = 0; num < inputFields.size(); num++) {
        if (inputFields[num]->contains(pos)) {
            return num;
        }
    }
    return -1;
} // end findInputFieldContaining

// checkResolution
//---------------------------------------------------------------------------
// Purpose: Check position after resolution change
//---------------------------------------------------------------------------
void View::checkResolution(iXY oldResolution, iXY newResolution)
{
//        iXY size = getSize();
//
//        min.x += (newResolution.x - oldResolution.x)/2;
//        min.y += (newResolution.y - oldResolution.y)/2;
//        max = min + size;
    iXY newpos = min + ((newResolution-oldResolution) / 2);
    //newpos -= getSize() / 2;
    moveTo(newpos);
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

// RESIZE
//---------------------------------------------------------------------------
// Purpose: Resizes the current window to the specified size.
//---------------------------------------------------------------------------
void View::resize(const iXY &size)
{
    iXY destSize(size);
    clientRect.min = min;
    clientRect.max = min + size;

    if ( getBordered() )
    {
        clientRect.min.x += borderSize;
        clientRect.min.y += borderSize + moveAreaHeight;
        clientRect.max.x += borderSize;
        clientRect.max.y += borderSize + moveAreaHeight;
        destSize.x += borderSize * 2;
        destSize.y += (borderSize * 2) + moveAreaHeight;
    }

    if ( getShowStatus() )
    {
        destSize.y += DEFAULT_STATUS_BAR_HEIGHT;
    }

    max = min + destSize;
} // end View::resize

// moveTo
//---------------------------------------------------------------------------
// Purpose: Move the window to the specified position, while making sure the
//          window is on the screen.
//---------------------------------------------------------------------------
void View::moveTo(iXY destMin)
{
    iXY diff = destMin - min;
    min += diff;
    max += diff;
    clientRect.min += diff;
    clientRect.max += diff;

} // end moveTo

// toggleView
//---------------------------------------------------------------------------
// Purpose: Toggle the visibility of this window.
//---------------------------------------------------------------------------
void View::toggleView()
{
    Desktop::toggleVisibility(searchName);
} // end toggleView

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
                    inputFields[selectedInputField]->addExtendedChar(shit);
                } else {
                    throw Exception("ERROR: Expecting extended char code.");
                }
            } else {
                inputFields[selectedInputField]->addChar(shit);
            }
        }
    }
} // end processEvents

// addInputField
//---------------------------------------------------------------------------
cInputField* View::addInputField(
    const iXY &pos,
    cInputFieldString *string,
    const char *excludedCharacters,
    const bool &isSelected)
{
    cInputField* inputfield = new cInputField;

    inputfield->setPos(pos);
    inputfield->setInputFieldString(string);
    inputfield->setExcludedCharacters(excludedCharacters);

    inputFields.push_back(inputfield);

    if (isSelected) {
        selectedInputField = inputFields.size() - 1;
    }

    return inputfield;
} // end addInputField

void
View::drawString(int x, int y, const char * str, const IntColor color)
{
    x += clientRect.min.x;
    y += clientRect.min.y;
    screen->bltString(x, y, str, color);
}

void
View::drawStringCenter(const char* string, IntColor color)
{
    screen->bltStringCenteredInRect(clientRect, string, color);
}

void
View::drawStringInBox( const iRect &rect, const char *string, IntColor color, int gapSpace, bool drawBox)
{
    iRect r(rect);
    r.translate(clientRect.min);
    screen->bltStringInBox(r, string, color, gapSpace, drawBox);
}

void
View::drawStringShadowed(int x, int y, const char *str, const IntColor textColor, const IntColor shadowColor)
{
    x += clientRect.min.x;
    y += clientRect.min.y;
    screen->bltStringShadowed( x, y, str, textColor, shadowColor);
}

void
View::drawImage( Surface &s, int x, int y)
{
    s.blt(*screen, clientRect.min.x + x, clientRect.min.y + y );
}

void
View::drawImageTrans( Surface &s, int x, int y)
{
    s.bltTrans(*screen, clientRect.min.x + x, clientRect.min.y + y );
}

void
View::drawTransRect(const iRect &destRect)
{
    iRect r(destRect);
    r.translate(clientRect.min);
    screen->bltLookup(r);
}

void
View::drawButtonBorder(iRect bounds, IntColor topLeftColor, IntColor bottomRightColor)
{
    bounds.translate(clientRect.min);
    screen->drawButtonBorder(bounds, topLeftColor, bottomRightColor);
}

void
View::fill(const IntColor color)
{
    screen->fillRect(clientRect, color);
}

void
View::drawRect(iRect bounds, const IntColor color)
{
    bounds.translate(clientRect.min);
    screen->drawRect(bounds, color);
}

void
View::fillRect(iRect bounds, const IntColor color)
{
    bounds.translate(clientRect.min);
    screen->fillRect(bounds, color);
}

void
View::drawViewBackground()
{
    if (gameconfig->viewdrawbackgroundmode == VIEW_BACKGROUND_DARK_GRAY_BLEND)
    {
        screen->bltLookup(*this);
    }
    else if (gameconfig->viewdrawbackgroundmode == VIEW_BACKGROUND_LIGHT_GRAY_BLEND)
    {
        screen->bltLookup(*this);
    }
    else if (gameconfig->viewdrawbackgroundmode == VIEW_BACKGROUND_SOLID_BLACK)
    {
        screen->fillRect(*this, Color::black);
    }
    else if (gameconfig->viewdrawbackgroundmode == VIEW_BACKGROUND_TRANSPARENT)
    {
        // nothing, so transparent
    }

}
