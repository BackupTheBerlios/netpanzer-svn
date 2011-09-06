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
#include "Views/Components/Component.hpp"

using std::min;
using std::max;

std::vector<View*> Desktop::views;
std::vector<View*> Desktop::all_views;
View     *Desktop::focus = 0;
Component *Desktop::keyboardFocusComponent = 0;
iXY       Desktop::lMouseDownPos;
iXY       Desktop::rMouseDownPos;
iXY       Desktop::prevMousePos;
int       Desktop::prevButton;
View     *Desktop::mouseView;
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
    focus        = 0;
    prevButton   = 0;
} // end Desktop constructor

// add
//--------------------------------------------------------------------------
// Purpose: Adds a new window to the end of the current window list.
//--------------------------------------------------------------------------
void Desktop::add(View *view)
{
    if ( view )
    {
        all_views.push_back(view);
        if ( view->isVisible() )
        {
            views.push_back(view);
        }
    }
} // end add

// remove
//--------------------------------------------------------------------------
// Purpose: Removes a window from the list.
//--------------------------------------------------------------------------
void Desktop::remove(View *view)
{
    if ( view )
    {
        std::remove(views.begin(), views.end(), view);
        std::remove(all_views.begin(), all_views.end(), view);
    }
} // end add

// setVisibility
//--------------------------------------------------------------------------
// Purpose:
//--------------------------------------------------------------------------
void Desktop::setVisibility(View *view, int isVisible)
{
    if ( isVisible && ! view->isVisible() )
    {
        view->status |= View::STATUS_VISIBLE;
        views.push_back(view);
        view->doActivate();
    }
    else if ( ! isVisible && view->isVisible() )
    {
        view->status &= ~View::STATUS_VISIBLE;
        std::remove(views.begin(), views.end(), view);
        view->doDeactivate();
    }
} // end setVisibility


// manage
//--------------------------------------------------------------------------
// Purpose: Manages the window manager.  Check for button presses, actions
//          occur, which window to occur in, etc.
//--------------------------------------------------------------------------
void Desktop::manage(int mouseX, int mouseY, int curButton)
{
    iXY mousePos(mouseX, mouseY);

    MouseInterface::setCursor("default.bmp");

    View* prevMouseView = mouseView;

    mouseView = findViewContaining(mousePos);

    // Check for mouseEnter and mouseExit.
    if (mouseView != prevMouseView)
    {
        if (prevMouseView != 0)
        {
            prevMouseView->mouseExit(prevMouseView->getScreenToClientPos(mousePos));
        }

        if (mouseView != 0)
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

    if (curButton & MouseInterface::left_button_mask) // left button is pressed
    {
        if (!(prevButton & MouseInterface::left_button_mask)) // new press
        {
            keyboardFocusComponent = 0;
            lMouseDownPos = mousePos;
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
        else // continued press
        {
            if (lMouseView != 0)
            {
                lMouseView->lMouseDrag(lMouseView->getScreenToClientPos(lMouseDownPos), lMouseView->getScreenToClientPos(prevMousePos), lMouseView->getScreenToClientPos(mousePos));
            }
        }
    }
    else // left button is not pressed
    {
        if (prevButton & MouseInterface::left_button_mask)
        {
            if (lMouseView != 0)
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

    if (curButton & MouseInterface::right_button_mask) // right button pressed
    {
        if (!(prevButton & MouseInterface::right_button_mask))
        {
            keyboardFocusComponent = 0;
            rMouseDownPos = mousePos;
            rMouseView = mouseView;
            if (rMouseView != 0)
            {
                rMouseView->rMouseDown(rMouseView->getScreenToClientPos(mousePos));
            }
        }
        else
        {
            if (rMouseView != 0)
            {
                rMouseView->rMouseDrag(rMouseView->getScreenToClientPos(rMouseDownPos), rMouseView->getScreenToClientPos(prevMousePos), rMouseView->getScreenToClientPos(mousePos));
            }
        }
    }
    else
    {
        if (prevButton & MouseInterface::right_button_mask)
        {
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

} // end manage

// draw
//--------------------------------------------------------------------------
// Purpose:
//--------------------------------------------------------------------------
void Desktop::draw(Surface& surface)
{
    surface.fill(Color::black); // XXX remove this
    std::vector<View*>::reverse_iterator i;
    for(i = views.rbegin(); i != views.rend(); i++) {
        (*i)->draw(surface);
    }
} // end draw



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

         if (!(view->getAlwaysOnBottom())) {
            for(size_t i = 0; i<views.size(); i++) {
                if(views[i] == view) {
                    for(size_t i2 = i; i2 >= 1; i2--)
                        views[i2] = views[i2-1];
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
    std::vector<View*>::iterator i;
    for(i = views.begin(); i != views.end(); i++) {
        if((*i)->status & View::STATUS_VISIBLE)
            if((*i)->contains(pos))
                return *i;
    }
    return 0;
} // end findViewContaining

// toggleVisibility
//--------------------------------------------------------------------------
// Purpose:
//--------------------------------------------------------------------------
void Desktop::toggleVisibility(const char *searchName)
{
    std::vector<View*>::iterator i;
    for(i = views.begin(); i != views.end(); i++) {
        View* view = *i;
        if (strcmp(view->searchName, searchName) == 0) {
            view->status ^= View::STATUS_VISIBLE;

            if (view->status & View::STATUS_VISIBLE) {
                view->doActivate();
                //activate(view);
            } else {
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
    std::vector<View*>::iterator i;
    for(i = views.begin(); i != views.end(); i++) {
        View* view = *i;

        view->checkArea(viewarea);
    }
} // end Desktop::checkViewPositions

// checkResolution
//--------------------------------------------------------------------------
// Purpose: Makes sure all the view are on the position.
//--------------------------------------------------------------------------
void Desktop::checkResolution(iXY oldResolution, iXY newResolution)
{
    std::vector<View*>::iterator i;
    for(i = views.begin(); i != views.end(); i++) {
        View* view = *i;
        view->checkResolution(oldResolution, newResolution);
    }
} // end Desktop::checkResolution

// toggleVisibilityNoDoAnything
//--------------------------------------------------------------------------
// Purpose: Only changes whether a visibility of the view, no activate or
//          deactivate calls.
//--------------------------------------------------------------------------
void Desktop::toggleVisibilityNoDoAnything(const char *searchName)
{
    std::vector<View*>::iterator i;
    for(i = views.begin(); i != views.end(); i++) {
        View* view = *i;
        if (strcmp(view->searchName, searchName) == 0) {
            view->status ^= View::STATUS_VISIBLE;
            break;
        }
    }
} // end toggleVisibilityNoDoAnything

// setActiveView
//--------------------------------------------------------------------------
// Purpose:
//--------------------------------------------------------------------------
void Desktop::setActiveView(const char *searchName)
{
    std::vector<View*>::iterator i;
    for(i = views.begin(); i != views.end(); i++) {
        View* view = *i;
        if (strcmp(view->searchName, searchName) == 0) {
            // If the view is not active set the view active.
            if (!view->getActive()) {
                activate(view);
            }

            return;
        }
    }

} // end setActiveView

//--------------------------------------------------------------------------
void Desktop::setActiveView(View *v)
{
    std::vector<View*>::iterator i;
    for(i = views.begin(); i != views.end(); i++) {
        View* view = *i;

        if (v == view) {
            // If the view is not active set the view active.
            if (!view->getActive()) {
                activate(view);
            }

            return;
        }
    }

}

// setVisibilityNoDoAnything
//--------------------------------------------------------------------------
// Purpose: Only changes whether a visibility of the view, no activate or
//          deactivate calls.
//--------------------------------------------------------------------------
void Desktop::setVisibilityNoDoAnything(const char *searchName, int isVisible)
{
    std::vector<View*>::iterator i;
    for(i = views.begin(); i != views.end(); i++) {
        View* view = *i;

        if (strcmp(view->searchName, searchName) == 0) {
            if (isVisible) {
                view->status |= View::STATUS_VISIBLE;
            } else {
                view->status &= ~View::STATUS_VISIBLE;
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
    setTitle("View Status");
    setSubTitle(" - F3");

    setAllowResize(false);
    setDisplayStatusBar(false);
    setVisible(false);

    resizeClientArea(iXY(320, 400));
    moveTo(iXY(0, 0));

    add( new Label( 0, 0,   "Name", Color::white) );
    add( new Label( 200, 0, "Status", Color::white) );

} // end DesktopView::DesktopView

// doDraw
//---------------------------------------------------------------------------
void DesktopView::doDraw(Surface &viewArea, Surface &clientArea)
{
    viewArea.fill(Color::black);

    int yOffset = 10;
    char strBuf[256];

    for (unsigned int i = 0; i < Desktop::views.size(); i++)
    {
        clientArea.bltString(0, yOffset, Desktop::views[i]->getSearchName(), Color::white);

        if ( Desktop::views[i]->isVisible() )
        {
            sprintf(strBuf, "STATUS_VISIBLE");

        }
        else
        {
            sprintf(strBuf, "INVISIBLE");
        }

        clientArea.bltString(200, yOffset, strBuf, Color::white);

        yOffset += Surface::getFontHeight();
    }

//    View::doDraw(viewArea, clientArea);

} // end doDraw

// rMouseDrag
//---------------------------------------------------------------------------
void DesktopView::rMouseDrag(const iXY&, const iXY &prevPos, const iXY &newPos)
{
    moveTo(min + newPos - prevPos);
} // end DesktopView::rMouseDrag

// doActivate
//---------------------------------------------------------------------------
void DesktopView::doActivate()
{
    Desktop::setActiveView(this);
} // end DesktopView::doActivate
