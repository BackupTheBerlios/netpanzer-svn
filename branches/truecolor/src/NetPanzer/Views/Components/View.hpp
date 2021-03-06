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
#ifndef __View_hpp__
#define __View_hpp__

class Surface;
#include "cInputField.hpp"
#include "Types/iRect.hpp"
#include "Types/iXY.hpp"
#include "Component.hpp"
#include "MouseEvent.hpp"

#include <list>

using namespace std;

class View : public iRect
{
    friend class Desktop;
public:
    void add(Component *Component);

    typedef list<Component *> ComponentList;
    typedef ComponentList::iterator ComponentsIterator;
    
    ComponentList components;
    
    Component *focusComponent;

    std::vector<cInputField*> inputFields;

    enum
    {
        MAX_WINDOW_CLIENT_XSIZE = 632
    };
    enum
    {
        MAX_WINDOW_CLIENT_YSIZE = 458
    };

    // Hack city, should be protected???????
    void setAlwaysOnBottom(const bool &newStatus);
    void setVisible(const bool &newStatus);

    View();
    View(const iXY &pos, const iXY &size, const char *title);
    virtual ~View();

    enum { DEFAULT_MOVE_AREA_HEIGHT  = 12 };
    enum { DEFAULT_STATUS_BAR_HEIGHT = 16 };
    enum { DEFAULT_BORDER_SIZE       =  3 };
    enum { DEFAULT_SNAP_TOLERANCE    = 20 };

    void moveTo(iXY destMin);
    inline void moveTo(const int &x, const int &y)
    {
        moveTo(iXY(x, y));
    }

    void removeComponents()
    {
        while ( ! components.empty() )
        {
            delete components.back();
            components.pop_back();
        }

        focusComponent      = 0;
    }

    // Accessor Functions.
    inline const char *getSearchName() const
    {
        return searchName;
    }
    inline const char *getTitle() const
    {
        return title;
    }
    inline const char *getSubTitle() const
    {
        return subTitle;
    }

    inline int getActive() const
    {
        return status & STATUS_ACTIVE;
    }
    inline int getVisible() const
    {
        return status & STATUS_VISIBLE;
    }
    inline int getAllowMove() const
    {
        return status & STATUS_ALLOW_MOVE;
    }
    inline int getBordered() const
    {
        return status & STATUS_BORDERED;
    }
    inline int getAlwaysOnBottom() const
    {
        return status & STATUS_ALWAYS_ON_BOTTOM;
    }
    inline int getShowStatus() const
    {
        return status & STATUS_DISPLAY_STATUS_BAR;
    }

    iRect getClientRect() const;

    void drawString(int x, int y, const char * str, const IntColor color);
    void drawStringCenter(const char *string, IntColor color);
    void drawStringInBox( const iRect &rect, const char *string, IntColor color, int gapSpace = 14, bool drawBox = false);
    void drawStringShadowed(int x, int y, const char *str, const IntColor textColor, const IntColor shadowColor);
    void drawImage( Surface &s, int x, int y);
    void drawImageTrans( Surface &s, int x, int y);
    void drawTransRect(const iRect &destRect);
    void drawButtonBorder(iRect bounds, IntColor topLeftColor, IntColor bottomRightColor);
    void drawRect(iRect bounds, const IntColor color);
    void drawViewBackground();
    void fill(const IntColor color);
    void fillRect(iRect bounds, const IntColor color);

    virtual void onComponentClicked(Component *c) { (void)c; }

    // View Status Functions.
    void setDisplayStatusBar(const bool &newStatus);
    void setBordered        (const bool &newStatus);
    void setAllowMove       (const bool &newStatus);
    void setActive          (const bool &newStatus);

    // SearchName, Title, and SubTitle functions.
    void  setSearchName(const char *searchName);
    void  setTitle(const char *title);
    void  setSubTitle(const char *subTitle);

    void showStatus(const char *string);

    void resize(const iXY &size);
    inline void resize(const int &x, const int &y)
    {
        resize(iXY(x, y));
    }

    
protected:
    virtual void     actionPerformed(mMouseEvent )
    {}

    int              selectedInputField;
    char            *searchName;
    char            *title;
    char            *subTitle;
    int              status;

    char            *statusText;

    // Status items
    enum { STATUS_ACTIVE             = (1U << 0) };
    enum { STATUS_VISIBLE            = (1U << 1) };
    enum { STATUS_ALLOW_MOVE         = (1U << 2) };
    enum { STATUS_ALWAYS_ON_BOTTOM   = (1U << 3) };
    enum { STATUS_BORDERED           = (1U << 4) };
    enum { STATUS_DISPLAY_STATUS_BAR = (1U << 5) };

    void        reset     ();
    void        activate  ();
    void        deactivate();

    void  drawTitle();

    // Input Field Functions
    cInputField* addInputField(const iXY &pos, cInputFieldString *string, const char *excludedCharacters, const bool &isSelected);
    int  findInputFieldContaining(const iXY &pos);
    void drawInputFields();

    /////////////////////////////////
    void draw();
    void drawStatus();
    virtual void checkResolution(iXY oldResolution, iXY newResolution);
    void checkArea(iXY viewarea);
    void toggleView();
    iXY  getScreenToClientPos(const iXY &pos);
    iXY  getScreenToViewPos(const iXY &pos);
    /////////////////////////////////

    // These options can be modified on a per View type basis
    virtual void drawBorder();
    virtual void doDraw();
    virtual void doActivate();
    virtual void doDeactivate();
    virtual void mouseMove(const iXY &prevPos, const iXY &newPos);
    virtual void lMouseDown(const iXY &pos);
    virtual int  lMouseUp(const iXY &downPos, const iXY &upPos);
    virtual void lMouseDrag(const iXY &downPos, const iXY &prevPos, const iXY &newPos);
    virtual void lMouseDouble(const iXY &pos);
    virtual void rMouseDown(const iXY &pos);
    virtual void rMouseUp(const iXY &downPos, const iXY &upPos);
    virtual void rMouseUp();
    virtual void rMouseDrag(const iXY &downPos, const iXY &prevPos, const iXY &newPos);
    virtual void rMouseDouble(const iXY &pos);
    //virtual void keyDown(Uint8 keyCode);
    virtual void mouseEnter(const iXY &pos);
    virtual void mouseExit(const iXY &pos);
    //virtual void keyUp();

    virtual void processEvents();

    int moveAreaHeight;
    int borderSize;
    int snapToTolerance;

    iRect clientRect;
};

#endif // end __View_hpp__
