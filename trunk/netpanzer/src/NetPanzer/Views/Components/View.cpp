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


#include "Views/Components/View.hpp"
#include "Views/Components/Label.hpp"
#include "Interfaces/MouseInterface.hpp"
#include "Interfaces/KeyboardInterface.hpp"
#include "Types/fXY.hpp"
#include "ViewGlobals.hpp"
#include "Util/Exception.hpp"
#include "InputEvent.hpp"

#include <algorithm>

// add
//---------------------------------------------------------------------------
void View::add(Component *component)
{
    if ( component )
    {
        components.push_back(component);
    }
} // end View::add

// View
//---------------------------------------------------------------------------
View::View()
{
    reset();
} // end View::View

// ~View
//---------------------------------------------------------------------------
View::~View()
{
    free(searchName);
} // end ~View::View

// reset
//---------------------------------------------------------------------------
// Purpose: Reset all the settings for this window.
//---------------------------------------------------------------------------
void View::reset()
{
    zero();
    status                = 0;
    searchName            = 0;
    focusComponent        = 0;

    setSearchName("donut");

    //scrollBar             = 0;

    removeComponents();

    setAllowMove(true);
    setAlwaysOnBottom(false);

    //setScrollBar(false);
} // end reset

// draw
//---------------------------------------------------------------------------
void View::draw(Surface& surface)
{
    assert(this != 0);

    if ( ! isVisible() )
    {
        return;
    }

    surface.setAbsoluteClip(*this);
    doDraw(surface);
    surface.resetClip();
} // end draw

// activate
//---------------------------------------------------------------------------
// Purpose: Default Implementation - Sets the window status to active.
//---------------------------------------------------------------------------
void View::activate()
{
    if ( isActive() )
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
    if ( ! isActive() )
    {
        return;
    }

    setActive(false);

    doDeactivate();
    
} // end View::deactivate

// doDraw
//---------------------------------------------------------------------------
// Purpose: Default implementation - draws all the componentList of the window.
//---------------------------------------------------------------------------
void View::doDraw( Surface &dest )
{
    // Draw all non-selected components.
    for ( size_t n = 0; n < components.getLastIndex(); n++ )
    {
        if ( components[n] != focusComponent )
        {
            components[n]->draw(dest);
        }
    }

    // Now draw the selected component.
    if (focusComponent != 0)
    {
        focusComponent->draw(dest);
    }
} // end View::doDraw

// doActivate
//---------------------------------------------------------------------------
// Purpose: Default Implementation -
//---------------------------------------------------------------------------
void View::doActivate()
{
    // Tell all the components the mouse entered the view.
    for ( size_t n = 0; n != components.getLastIndex(); n++)
    {
        if ( components[n]->contains(MouseInterface::getMousePosition()) )
        {
            mMouseEvent me(components[n],
                mMouseEvent::MOUSE_EVENT_ENTERED, now(), 0, 
                MouseInterface::getMouseX(), MouseInterface::getMouseY(), 0, false);

            components[n]->actionPerformed(me);

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
    for ( size_t n = 0; n != components.getLastIndex(); n++ )
    {
        mMouseEvent me(components[n],
            mMouseEvent::MOUSE_EVENT_EXITED, now(), 0, 
            MouseInterface::getMouseX(), MouseInterface::getMouseY(), 0, false);

        components[n]->actionPerformed(me);

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

    return pos - getLocation();
} // end getScreenToViewPos

// getClientRect
//---------------------------------------------------------------------------
// Purpose:
//---------------------------------------------------------------------------
iRect View::getClientRect() const
{
    return iRect(0, 0, getWidth(), getHeight());

} // end View::getClientRect

// mouseMove
//---------------------------------------------------------------------------
// Purpse: Changes the current highlighted button and checks for mouseEnter
//         and mouseExit status.
//---------------------------------------------------------------------------
void View::mouseMove(const iXY & prevPos, const iXY &newPos)
{
    // Check all components for a clicked event.
    for ( size_t n = 0; n != components.getLastIndex(); n++)
    {        
        bool containsPrev=components[n]->contains(prevPos);
        bool containsNew=components[n]->contains(newPos);
        if (containsPrev && !containsNew)
        {
            mMouseEvent me(components[n], mMouseEvent::MOUSE_EVENT_EXITED, now(), 0, newPos.x, newPos.y, 0, false);

            components[n]->actionPerformed(me);

            actionPerformed(me);
        }
        else if (!containsPrev && containsNew)
        {
            mMouseEvent me(components[n], mMouseEvent::MOUSE_EVENT_ENTERED, now(), 0, newPos.x, newPos.y, 0, false);

            components[n]->actionPerformed(me);

            actionPerformed(me);
        }
        else if (containsNew && newPos!=prevPos)
        {
            mMouseEvent me(components[n], mMouseEvent::MOUSE_EVENT_MOVED, now(), 0, newPos.x, newPos.y, 0, false);

            components[n]->actionPerformed(me);

            actionPerformed(me);
        }
    }
} // end mouseMove

// lMouseDown
//---------------------------------------------------------------------------
void View::lMouseDown(const iXY &pos)
{
    focusComponent = 0;
//    Desktop::setKeyboardFocusComponent(0);

    // Check all components for a pressed event.
    for ( size_t n = 0; n != components.getLastIndex(); n++)
    {        
        if (components[n]->contains(pos)) {
            mMouseEvent me(components[n], mMouseEvent::MOUSE_EVENT_PRESSED, now(), InputEvent::BUTTON1_MASK, pos.x, pos.y, 0, false);

            components[n]->actionPerformed(me);

            focusComponent = components[n];

            actionPerformed(me);
        }
    }

} // end View::lMouseDown

// lMouseUp
//---------------------------------------------------------------------------
int View::lMouseUp(const iXY &downPos, const iXY &upPos)
{
    // Check all components for a clicked event.
    {    
        for ( size_t n = 0; n != components.getLastIndex(); n++)
        {        
            if (components[n]->contains(downPos) && components[n]->contains(upPos)) {
                mMouseEvent me(components[n], mMouseEvent::MOUSE_EVENT_CLICKED, now(), InputEvent::BUTTON1_MASK, upPos.x, upPos.y, 0, false);

                components[n]->actionPerformed(me);

                actionPerformed(me);
            }
        }
    }

    // Report a mouse release to all components except for a clicked one.
    {    
        for ( size_t n = 0; n != components.getLastIndex(); n++)
        {        
            if (components[n]->contains(upPos)) {
                mMouseEvent me(components[n], mMouseEvent::MOUSE_EVENT_RELEASED, now(), InputEvent::BUTTON1_MASK, upPos.x, upPos.y, 0, false);

                components[n]->actionPerformed(me);

                actionPerformed(me);
            }
        }}

    return 0;
} // end View::lMouseUp

// lMouseDrag
//---------------------------------------------------------------------------
void View::lMouseDrag(const iXY &downPos, const iXY &prevPos, const iXY &newPos)
{
    //lMouseDown(newPos);
    //mouseMove(prevPos, newPos);

    // Check all components for a dragged event.
    for ( size_t n = 0; n != components.getLastIndex(); n++)
    {        
        if (components[n]->contains(newPos) && components[n]->contains(downPos) && newPos!=prevPos) {
            mMouseEvent me(components[n], mMouseEvent::MOUSE_EVENT_DRAGGED, now(), InputEvent::BUTTON1_MASK, newPos.x, newPos.y, 0, false);

            components[n]->actionPerformed(me);

            focusComponent = components[n];

            actionPerformed(me);
        }
    }

} // end View::lMouseDrag

// rMouseDown
//---------------------------------------------------------------------------
void View::rMouseDown(const iXY &pos)
{
    focusComponent = 0;
//    Desktop::setKeyboardFocusComponent(0);

    // Check all components for a pressed event.
    for ( size_t n = 0; n != components.getLastIndex(); n++)
    {        
        if (components[n]->contains(pos)) {
            mMouseEvent me(components[n], mMouseEvent::MOUSE_EVENT_PRESSED, now(), InputEvent::BUTTON2_MASK, pos.x, pos.y, 0, false);

            components[n]->actionPerformed(me);

            focusComponent = components[n];

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
    for ( size_t n = 0; n != components.getLastIndex(); n++)
    {        
        if (components[n]->contains(downPos) && components[n]->contains(upPos)) {
            mMouseEvent me(components[n], mMouseEvent::MOUSE_EVENT_CLICKED, now(), InputEvent::BUTTON2_MASK, upPos.x, upPos.y, 0, false);

            components[n]->actionPerformed(me);

            reportRelease = false;

            actionPerformed(me);
        }
    }

    // If we are on the last component and there was no component containing both
    // mouse press positions, then report a release to an
    if (reportRelease) {
        // Check all components for a release event.
        for ( size_t n = 0; n != components.getLastIndex(); n++)
        {        
            if (components[n]->contains(upPos)) {
                mMouseEvent me(components[n], mMouseEvent::MOUSE_EVENT_RELEASED, now(), InputEvent::BUTTON2_MASK, upPos.x, upPos.y, 0, false);

                components[n]->actionPerformed(me);

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
    for ( size_t n = 0; n != components.getLastIndex(); n++)
    {        
        if (components[n]->contains(newPos)  && components[n]->contains(downPos) && newPos!=prevPos) {
            mMouseEvent me(components[n], mMouseEvent::MOUSE_EVENT_DRAGGED, now(), InputEvent::BUTTON2_MASK, newPos.x, newPos.y, 0, false);

            components[n]->actionPerformed(me);

            focusComponent = components[n];

            actionPerformed(me);
        }
    }

} // end View::rMouseDrag

// mouseEnter
//---------------------------------------------------------------------------
// Purpose: Is triggered when the mouse enters the window.
//---------------------------------------------------------------------------
void View::mouseEnter(const iXY &pos)
{
    // Check all components for a entered event.
    for ( size_t n = 0; n != components.getLastIndex(); n++)
    {        
        if (components[n]->contains(pos)) {
            mMouseEvent me(components[n], mMouseEvent::MOUSE_EVENT_ENTERED, now(), 0, pos.x, pos.y, 0, false);

            components[n]->actionPerformed(me);

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
    for ( size_t n = 0; n != components.getLastIndex(); n++)
    {        
        if (components[n]->contains(pos)) {
            mMouseEvent me(components[n], mMouseEvent::MOUSE_EVENT_EXITED, now(), 0, pos.x, pos.y, 0, false);

            components[n]->actionPerformed(me);

            actionPerformed(me);
        }
    }

} // end View::mouseExit

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

// by default: keep the size and update view position to match new size
void View::onDesktopResized( const iXY& oldResolution, const iXY& newResolution)
{
    translate( (newResolution.x - oldResolution.x)/2,
               (newResolution.y - oldResolution.y)/2);
}

void View::checkArea(iXY viewarea)
{
    if ( getLocationX() < 0 )
        setLocationX(0);
    if ( getLocationY() < 0 )
        setLocationY(0);
                                                               
    if ( getEndX() > viewarea.x)
        setLocationX(viewarea.x - getWidth());
    if ( getEndY() > viewarea.y)
        setLocationY(viewarea.y - getHeight());
}

// RESIZE CLIENT AREA
//---------------------------------------------------------------------------
// Purpose: Resize the client area of the window.  The window area will be
//          resized accordingly.
//---------------------------------------------------------------------------
void View::resizeClientArea(const iXY &size)
{
    resize(size);
} // end View::resizeClientArea

// RESIZE
//---------------------------------------------------------------------------
// Purpose: Resizes the current window to the specified size.
//---------------------------------------------------------------------------
void View::resize(const iXY &size)
{
    setSize(size);
} // end View::resize

// moveTo
//---------------------------------------------------------------------------
// Purpose: Move the window to the specified position, while making sure the
//          window is on the screen.
//---------------------------------------------------------------------------
void View::moveTo(iXY destMin)
{
    setLocation(destMin);
} // end moveTo

// setAlwaysOnBottom
//---------------------------------------------------------------------------
// Purpose: Makes this view always drawn on the bottom or not.
//---------------------------------------------------------------------------
void View::setAlwaysOnBottom(const bool &newStatus)
{
    if (newStatus == true) status |=  STATUS_ALWAYS_ON_BOTTOM;
    else                   status &= ~STATUS_ALWAYS_ON_BOTTOM;
} // setAlwaysOnBottom

// setAllowMove
//---------------------------------------------------------------------------
void View::setAllowMove(const bool &newStatus)
{
    if (newStatus == true) status |=  STATUS_ALLOW_MOVE;
    else                   status &= ~STATUS_ALLOW_MOVE;
} // end setAllowMove

// setActive
//---------------------------------------------------------------------------
// Purpose: Sets this view as an active view or not.
//---------------------------------------------------------------------------
void View::setActive(const bool &newStatus)
{
    if (newStatus == true) status |=  STATUS_ACTIVE;
    else                   status &= ~STATUS_ACTIVE;
} // end setActive
