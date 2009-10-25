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

std::vector<View*> Desktop::views;
View     *Desktop::focus;
iXY       Desktop::lMouseDownPos;
iXY       Desktop::rMouseDownPos;
iXY       Desktop::prevMousePos;
iXY       Desktop::mouseActionOffset;
int       Desktop::prevButton;
int       Desktop::mouseIsDown;
View     *Desktop::mouseView;
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
    focus        = 0;
    prevButton   = 0;
} // end Desktop constructor

// manage
//--------------------------------------------------------------------------
// Purpose: Manages the window manager.  Check for button presses, actions
//          occur, which window to occur in, etc.
//--------------------------------------------------------------------------
void Desktop::manage(int mouseX, int mouseY, int curButton)
{
    iXY mousePos(mouseX, mouseY);

    MouseInterface::setCursor("default.png");

    prevMouseView = mouseView;

    mouseView = findViewContaining(mousePos);

    // Check for mouseEnter and mouseExit.
    if (mouseView != prevMouseView)
    {
        if (mouseView != 0)
        {
            mouseView->mouseEnter(mouseView->getScreenToClientPos(mousePos));
        }

        if (prevMouseView != 0)
        {
            prevMouseView->mouseExit(prevMouseView->getScreenToClientPos(mousePos));
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

    if (curButton & MouseInterface::left_button_mask)
    {
        // The button is down.  See if it just got pressed, or if it was pressed
        // before and is being dragged.
        if (!(prevButton & MouseInterface::left_button_mask))
        {
            // The mouse button just got pressed. Remember the initial place
            // where it got pressed.
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
        else
        {
            // The mouse was down before and is still down.
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

    if (focus != 0)
    {
        focus->processEvents();
        KeyboardInterface::flushCharBuffer();
    }

} // end manage

// draw
//--------------------------------------------------------------------------
// Purpose:
//--------------------------------------------------------------------------
void Desktop::draw()
{
    std::vector<View*>::reverse_iterator i;
    for(i = views.rbegin(); i != views.rend(); i++)
    {
        (*i)->draw();
    }
} // end draw

// add
//--------------------------------------------------------------------------
// Purpose: Adds a new window to the end of the current window list.
//--------------------------------------------------------------------------
void Desktop::add(View *view, bool autoActivate)
{
    assert(view != 0);
    views.push_back(view);
    if (autoActivate)
    {
        activate(view);
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
    }
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
    if (focus != view)
    {
        if (focus != 0)
        {
            focus->deactivate();
        }

         if (!(view->getAlwaysOnBottom()))
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
    std::vector<View*>::iterator i;
    for(i = views.begin(); i != views.end(); i++) {
        View* view = *i;

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
    std::vector<View*>::iterator i;
    for(i = views.begin(); i != views.end(); i++) {
        View* view = *i;

        if (strcmp(view->searchName, searchName) == 0) {
            if (isVisible) {
                view->status |= View::STATUS_VISIBLE;
                view->doActivate();
                //activate(view);
            } else {
                view->status &= ~View::STATUS_VISIBLE;
                view->doDeactivate();
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

    setDisplayStatusBar(false);
    setVisible(false);

    moveTo(iXY(0, 0));
    resize(iXY(320, 400));

    add( new Label( 0, 0,   "Name", Color::white) );
    add( new Label( 200, 0, "Status", Color::white) );

} // end DesktopView::DesktopView

// doDraw
//---------------------------------------------------------------------------
void DesktopView::doDraw()
{
    fill(Color::black);

    int yOffset = 10;
    char strBuf[256];

    for (int i = 0; i < Desktop::getViewCount(); i++) {
        drawString(0, yOffset, Desktop::getViewSearchName(i), Color::white);

        if (Desktop::getViewStatus(Desktop::getViewSearchName(i)) & STATUS_VISIBLE) {
            sprintf(strBuf, "STATUS_VISIBLE");

        } else {
            sprintf(strBuf, "INVISIBLE");
        }

        drawString(200, yOffset, strBuf, Color::white);

        yOffset += Surface::getFontHeight();
    }

    View::doDraw();

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
