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

#include "ArrayUtil/PtrArray.hpp"
#include "Views/Components/View.hpp"
#include "Util/TimeStamp.hpp"
#include "Util/Log.hpp"

class Surface;
class Component;

class Desktop
{
private: // Variables
    static PtrArray<View> views;
    static View      *focus;
    static Component *keyboardFocusComponent;
    static iXY        lMouseDownPos;
    static iXY        rMouseDownPos;
    static iXY        prevMousePos;
    static int        prevButton;
    static View      *lMouseView;
    static View      *rMouseView;
    static View      *prevMouseView;

private: // Functions
    static void isMouseInBox(int mouseX, int mouseY, int x1, int y1, int x2, int y2);

    static unsigned isMouseOverResizeArea(int mouseX, int mouseY);
    static bool     isMouseOverMoveArea  (int mouseX, int mouseY);

    static bool   isMouseInView(int mouseX, int mouseY);
    static View *findViewContaining(iXY p);

public:
    Desktop();

    static void manage(int mouseX, int mouseY, int curButton);
    static void activate(View *view);
    static void toggleVisibility(const char *viewName);
    static void setVisibility(const char *viewName, int isVisible);
    static void setVisibilityNoDoAnything(const char *viewName, int isVisible);
    static void add(View *view, bool autoActivate = true);
    static void remove(View *view);
    static void draw(Surface& surface);

    static int         getViewCount();
    static const char *getViewTitle(int viewNum);
    static const char *getViewSearchName(int viewNum);
    static int         getViewStatus(const char *searchName);
    static void        setActiveView(const char *viewitle);
    static void        setActiveView(View *view);

    static void toggleVisibility(View *view)
    {
        toggleVisibility(view->getSearchName());
    }

    static void setVisibility(View *view, int isVisible)
    {
        setVisibility(view->getSearchName(), isVisible);
    }

    static void setVisibilityAllWindows(int isVisible)
    {
        int viewCount = getViewCount();

        for (int i = 0; i < viewCount; i++) {
            setVisibility(getViewSearchName(i), isVisible);
        }
    }

    static void setVisibilityNoDoAnything(View *view, int isVisible)
    {
        setVisibilityNoDoAnything(view->getSearchName(), isVisible);
    }

    static bool getVisible(const char *searchName)
    {
        if (getViewStatus(searchName) & View::STATUS_VISIBLE) {
            return true;
        }

        return false;
    }

    static View *getView(const char *searchName)
    {
        for( size_t n = 0; n < views.getLastIndex(); n++ )
        {
            if ( ! strcmp(views[n]->searchName, searchName) )
            {
                return views[n];
            }
        }

        return 0;
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

    virtual void doDraw( Surface& dest );
    virtual void rMouseDrag(const iXY &downPos, const iXY &prevPos, const iXY &newPos);
    virtual void doActivate();

}
; // end DesktopView


#endif // end __Desktop_hpp__
