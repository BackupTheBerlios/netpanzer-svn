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
//---------------------------------------------------------------------------

#ifndef __Desktop_hpp__
#define __Desktop_hpp__

#include <vector>
#include "Views/Components/View.hpp"
#include "Util/TimeStamp.hpp"
#include "Util/Log.hpp"

class Surface;

class Desktop
{
private: // Variables
    friend class DesktopView;
    enum { RESIZE_NONE,
           RESIZE_TOPLEFT,
           RESIZE_TOPRIGHT,
           RESIZE_BOTTOMLEFT,
           RESIZE_BOTTOMRIGHT };

    static std::vector<View*> views;
    static std::vector<View*> all_views;
    static View      *focus;
    static Component *keyboardFocusComponent;
    static iXY        lMouseDownPos;
    static iXY        rMouseDownPos;
    static iXY        prevMousePos;
    static int        prevButton;
    static View      *lMouseView;
    static View      *rMouseView;
    static View      *mouseView;

private: // Functions
    static View *findViewContaining(iXY p);

public:
    Desktop();

    static void add(View *view);
    static void remove(View *view);

    static View *getView(const char *searchName)
    {
        std::vector<View*>::iterator i;

        for( i = all_views.begin(); i != all_views.end(); i++ )
        {
            View* view = *i;
            if ( strcmp(view->searchName, searchName) == 0 )
            {
                return view;
            }
        }

        return 0;
    }

    static void setVisibility(View *view, int isVisible);
    static void setVisibility(const char *viewName, int isVisible)
    {
        setVisibility( getView(viewName), isVisible );
    }


    static void manage(int mouseX, int mouseY, int curButton);
    static void activate(View *view);
    static void toggleVisibility(const char *viewName);
    static void toggleVisibilityNoDoAnything(const char *viewName);
    static void setVisibilityNoDoAnything(const char *viewName, int isVisible);
    static void draw(Surface& surface);

    static void        setActiveView(const char *viewitle);
    static void        setActiveView(View *view);

    static void toggleVisibility(View *view)
    {
        toggleVisibility(view->getSearchName());
    }

    static void hideAllWindows()
    {
        unsigned int i;
        for ( i = 0; i < views.size(); i++ )
        {
            views[i]->status &= ~View::STATUS_VISIBLE;
            views[i]->doDeactivate();
        }

        views.clear();
    }

    static void toggleVisibilityNoDoAnything(View *view)
    {
        toggleVisibilityNoDoAnything(view->getSearchName());
    }

    static void setVisibilityNoDoAnything(View *view, int isVisible)
    {
        setVisibilityNoDoAnything(view->getSearchName(), isVisible);
    }

    static bool getVisible(const char *searchName)
    {
        View *v = getView(searchName);
        return v && v->isVisible();
    }

    static void checkViewPositions(iXY viewarea);
    static void checkResolution(iXY oldResolution, iXY newResolution);

    static const View *getFocus()
    {
        return focus;
    }

    static void setFocusView(View *view)
    {
        activate(view);
    }

    static void setKeyboardFocusComponent(Component *c)
    {
        keyboardFocusComponent = c;
    }

    static Component * getKeyboardFocusComponent() { return keyboardFocusComponent; }

}
; // end Desktop

class DesktopView : public View
{
public:
    DesktopView();

    virtual void doDraw(Surface &viewArea, Surface &clientArea);
    virtual void rMouseDrag(const iXY &downPos, const iXY &prevPos, const iXY &newPos);
    virtual void doActivate();

}
; // end DesktopView


#endif // end __Desktop_hpp__
