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


#include <algorithm>
#include "Views/Components/Desktop.hpp"
#include "Interfaces/KeyboardInterface.hpp"
#include "Util/TimerInterface.hpp"
#include "Interfaces/MouseInterface.hpp"

#include "Views/Components/Label.hpp"

using std::min;
using std::max;

PtrArray<View> Desktop::views;
View     *Desktop::focus;
Component *Desktop::keyboardFocusComponent = 0;
iXY       Desktop::lMouseDownPos;
iXY       Desktop::rMouseDownPos;
iXY       Desktop::prevMousePos;
int       Desktop::prevButton;
View     *Desktop::prevMouseView;
View     *Desktop::lMouseView;
View     *Desktop::rMouseView;


/////////////////////////////////////////////////////////////////////////////
// Class definitions.
/////////////////////////////////////////////////////////////////////////////

// Desktop constructor
//--------------------------------------------------------------------------
// Purpose:
//--------------------------------------------------------------------------
Desktop::Desktop()
{
    focus         = 0;
    prevButton    = 0;
    prevMouseView = 0;
} // end Desktop constructor

// manage
//--------------------------------------------------------------------------
// Purpose: Manages the window manager.  Check for button presses, actions
//          occur, which window to occur in, etc.
//--------------------------------------------------------------------------
void Desktop::manage(int mouseX, int mouseY, int curButton)
{
    iXY mousePos(mouseX, mouseY);
    View * mouseView = findViewContaining(mousePos);

    MouseInterface::setCursor("default.bmp");

    // Check for mouseEnter and mouseExit.
    if (mouseView != prevMouseView)
    {
        if ( prevMouseView )
        {
            prevMouseView->mouseExit(prevMouseView->getScreenToClientPos(mousePos));
        }
        
        if ( mouseView )
        {
            mouseView->mouseEnter(mouseView->getScreenToClientPos(mousePos));
        }
    }

    if (mouseView != 0)
    {
        mouseView->mouseMove(mouseView->getScreenToClientPos(prevMousePos), mouseView->getScreenToClientPos(mousePos));
    }

    //////////////////////////////////////////////////////////////////////////
    //
    // handle left mouse button
    //
    //////////////////////////////////////////////////////////////////////////

    if ( curButton & MouseInterface::left_button_mask )
    {
        // The button is down.  See if it just got pressed, or if it was pressed
        // before and is being dragged.
        if (!(prevButton & MouseInterface::left_button_mask))
        {
            // The mouse button just got pressed. Remember the initial place
            // where it got pressed.
            lMouseDownPos = mousePos;

            if ( ! lMouseView )
            {
                lMouseView = mouseView;
                if (lMouseView != 0)
                {
                    lMouseView->lMouseDown(lMouseView->getScreenToClientPos(mousePos));
                    if (focus != lMouseView)
                    {
                        activate(lMouseView);
                    }
                }
            }
        }
        else
        {
            if (lMouseView != 0)
            {
                lMouseView->lMouseDrag(lMouseView->getScreenToClientPos(lMouseDownPos), lMouseView->getScreenToClientPos(prevMousePos), lMouseView->getScreenToClientPos(mousePos));
            }
        }
    }
    else
    {
        // The mouse button is up.
        //See if it just got released, or if it was up before.
        if (prevButton & MouseInterface::left_button_mask)
        {
            // The mouse button just got released.  If it was on top of a window
            // before, then tell the window that the button was released
            if ( lMouseView )
            {
                lMouseView->lMouseUp(lMouseView->getScreenToClientPos(lMouseDownPos), lMouseView->getScreenToClientPos(mousePos));
            }
        }

        lMouseView = 0;
    }

    //////////////////////////////////////////////////////////////////////////
    //
    // Handle right mouse button.
    //
    //////////////////////////////////////////////////////////////////////////

    if (curButton & MouseInterface::right_button_mask)
    {
        // The button is down.  See if it just got pressed, or if it was pressed
        // before and is being dragged.
        if (!(prevButton & MouseInterface::right_button_mask))
        {
            // The mouse button just got pressed. Remember the initial place
            // where it got pressed.
            rMouseDownPos = mousePos;

            rMouseView = mouseView;
            if (rMouseView != 0)
            {
                // No standard mouse actions.  Tell the window that the mouse
                // just got pressed.
                rMouseView->rMouseDown(rMouseView->getScreenToClientPos(mousePos));
            }
        }
        else
        {
            // The mouse was down before and is still down.

            if (rMouseView != 0)
            {
                rMouseView->rMouseDrag(rMouseView->getScreenToClientPos(rMouseDownPos), rMouseView->getScreenToClientPos(prevMousePos), rMouseView->getScreenToClientPos(mousePos));
            }
        }
    }
    else
    {
        // The mouse button is up.
        //See if it just got released, or if it was up before.
        if (prevButton & MouseInterface::right_button_mask)
        {
            // The mouse button just got released.  If it was on top of a window
            // before, then tell the window that the button was released
            if (rMouseView != 0)
            {
                rMouseView->rMouseUp(rMouseView->getScreenToClientPos(rMouseDownPos), rMouseView->getScreenToClientPos(mousePos));
            }
        }
        rMouseView = 0;
    }

    prevButton   = curButton;
    prevMousePos = mousePos;

    if ( keyboardFocusComponent )
    {
        keyboardFocusComponent->handleKeyboard();
        KeyboardInterface::flushCharBuffer();
    }
    else if (focus != 0)
    {
        focus->processEvents();
        KeyboardInterface::flushCharBuffer();
    }
    
    prevMouseView = mouseView;

} // end manage

// draw
//--------------------------------------------------------------------------
// Purpose:
//--------------------------------------------------------------------------
void Desktop::draw(Surface& surface)
{
    for ( size_t n = views.getLastIndex()-1; n < views.getLastIndex(); n-- )
    {
        views[n]->draw(surface);
    }
} // end draw

// add
//--------------------------------------------------------------------------
// Purpose: Adds a new window to the end of the current window list.
//--------------------------------------------------------------------------
void Desktop::add(View *view, bool autoActivate)
{
    views.push_back(view);
    if (autoActivate)
        activate(view);
} // end add

// remove
//--------------------------------------------------------------------------
// Purpose: Removes a window from the list.
//--------------------------------------------------------------------------
void Desktop::remove(View *view)
{
    for ( size_t n = 0; n < views.getLastIndex(); n++ )
    {
        if ( views[n] == view )
        {
            LOGGER.warning("Erasing view '%s'", view->searchName);
            views.stableErase(n);
            break;
        }
    }
    // XXX shall delete it?
} // end add


// activate
//--------------------------------------------------------------------------
// Purpose:
//---------------------------------------------------------------------------
void Desktop::activate(View *view)
{
    assert(view != 0);

    // If the top window equals the window to activate, then nothing needs to
    // be done.
    if (focus != view) {
        if (focus != 0)	{
            focus->deactivate();
        }

         if ( ! view->isAlwaysOnBottom() )
         {
            for(size_t i = 0; i<views.size(); i++)
            {
                if(views[i] == view)
                {
                    for(size_t i2 = i; i2 >= 1; i2--)
                    {
                        views[i2] = views[i2-1];
                    }
                    views[0] = view;
                    break;
                }
            }
        }
        focus = view;
        view->activate();
    }
} // end activate

// findViewContaining
//--------------------------------------------------------------------------
View *Desktop::findViewContaining(iXY pos)
{
    for ( size_t n = 0; n < views.getLastIndex(); n++ )
    {
        View* view = views[n];
        if( view->isVisible()  && view->contains(pos) )
        {
            return view;
        }
    }
    return 0;
} // end findViewContaining

// toggleVisibility
//--------------------------------------------------------------------------
// Purpose:
//--------------------------------------------------------------------------
void Desktop::toggleVisibility(const char *searchName)
{
    for ( size_t n = 0; n < views.getLastIndex(); n++ )
    {
        View* view = views[n];
        if (strcmp(view->searchName, searchName) == 0)
        {
            bool newVis = ! view->isVisible();

            if ( newVis )
            {
                view->show();
                view->onShow();
                view->doActivate();
            }
            else
            {
                view->hide();
                view->onHide();
                view->doDeactivate();
            }
            break;
        }
    }
} // end toggleVisibility

// checkViewPositions
//--------------------------------------------------------------------------
// Purpose: Makes sure all the views are on the screen.
//--------------------------------------------------------------------------
void Desktop::checkViewPositions(iXY viewarea)
{
    for ( size_t n = 0; n < views.getLastIndex(); n++ )
    {
        views[n]->checkArea(viewarea);
    }
} // end Desktop::checkViewPositions

// checkResolution
//--------------------------------------------------------------------------
// Purpose: Makes sure all the view are on the position.
//--------------------------------------------------------------------------
void Desktop::checkResolution(iXY oldResolution, iXY newResolution)
{
    for ( size_t n = 0; n < views.getLastIndex(); n++ )
    {
        views[n]->onDesktopResized(oldResolution, newResolution);
    }
} // end Desktop::checkResolution

// setActiveView
//--------------------------------------------------------------------------
// Purpose:
//--------------------------------------------------------------------------
void Desktop::setActiveView(const char *searchName)
{
    for ( size_t n = 0; n < views.getLastIndex(); n++ )
    {
        View* view = views[n];
        if (strcmp(view->searchName, searchName) == 0)
        {
            // If the view is not active set the view active.
            if ( ! view->isActive() )
            {
                activate(view);
            }

            return;
        }
    }

} // end setActiveView

//--------------------------------------------------------------------------
void Desktop::setActiveView(View *v)
{
    for ( size_t n = 0; n < views.getLastIndex(); n++ )
    {
        View* view = views[n];

        if (v == view) {
            // If the view is not active set the view active.
            if ( ! view->isActive() )
            {
                activate(view);
            }

            return;
        }
    }

}

// getViewCount
//--------------------------------------------------------------------------
// Purpose: Returns the number of windows in the window manager.
//--------------------------------------------------------------------------
int Desktop::getViewCount()
{
    return views.size();
} // end getViewCount

// getViewSearchName
//--------------------------------------------------------------------------
// Purpose: Returns the searchName of the specified window.
//--------------------------------------------------------------------------
const char *Desktop::getViewSearchName(int winNum)
{
    assert(winNum < getViewCount());

    // Returnthe requested windows searchName.
    return views[winNum]->searchName;
} // end getViewSearchName

// getViewStatus
//--------------------------------------------------------------------------
// Purpose: Returns the status of the specified window.
//--------------------------------------------------------------------------
int Desktop::getViewStatus(const char *searchName)
{
    for ( size_t n = 0; n < views.getLastIndex(); n++ )
    {
        View* view = views[n];

        if (strcmp(view->searchName, searchName) == 0) {
            return view->status;
        }
    }

    return 0;
} // end getViewStatus

// setVisibility
//--------------------------------------------------------------------------
// Purpose:
//--------------------------------------------------------------------------
void Desktop::setVisibility(const char *searchName, int isVisible)
{
    for ( size_t n = 0; n < views.getLastIndex(); n++ )
    {
        View* view = views[n];

        if (strcmp(view->searchName, searchName) == 0)
        {
            if ( isVisible != view->isVisible() )
            {
                if (isVisible)
                {
                    view->show();
                    view->onShow();
                    view->doActivate();
                }
                else
                {
                    view->hide();
                    view->onHide();
                    view->doDeactivate();
                }
            }
            break;
        }
    }
} // end setVisibility

// setVisibilityNoDoAnything
//--------------------------------------------------------------------------
// Purpose: Only changes whether a visibility of the view, no activate or
//          deactivate calls.
//--------------------------------------------------------------------------
void Desktop::setVisibilityNoDoAnything(const char *searchName, int isVisible)
{
    for ( size_t n = 0; n < views.getLastIndex(); n++ )
    {
        View* view = views[n];

        if (strcmp(view->searchName, searchName) == 0)
        {
            if ( isVisible )
            {
                view->show();
            }
            else
            {
                view->hide();
            }
            
            break;
        }
    }
} // end setVisibilityNoDoAnything

// DesktopView
//---------------------------------------------------------------------------
DesktopView::DesktopView() : View()
{
    setSearchName("DesktopView");

    resizeClientArea(iXY(800, 600));
    moveTo(iXY(0, 0));

    add( new Label( 0, 0,   "Name", Color::white) );
    add( new Label( 200, 0, "Status", Color::white) );

} // end DesktopView::DesktopView

// doDraw
//---------------------------------------------------------------------------
void DesktopView::doDraw( Surface &dest )
{
    dest.fill(Color::black);

    int yOffset = 10;
    char strBuf[256];

    for (int i = 0; i < Desktop::getViewCount(); i++) {
        dest.bltString(0, yOffset, Desktop::getViewSearchName(i), Color::white);

        if ( Desktop::getVisible(Desktop::getViewSearchName(i)) )
        {
            sprintf(strBuf, "STATUS_VISIBLE");
        }
        else
        {
            sprintf(strBuf, "INVISIBLE");
        }

        dest.bltString(200, yOffset, strBuf, Color::white);

        yOffset += Surface::getFontHeight();
    }

    View::doDraw( dest );

} // end doDraw

// rMouseDrag
//---------------------------------------------------------------------------
void DesktopView::rMouseDrag(const iXY&, const iXY &prevPos, const iXY &newPos)
{
    moveTo(getLocation() + newPos - prevPos);
} // end DesktopView::rMouseDrag

// doActivate
//---------------------------------------------------------------------------
void DesktopView::doActivate()
{
    Desktop::setActiveView(this);
} // end DesktopView::doActivate
