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

#include <stdint.h>

#include "cMouse.hpp"
#include "cButton.hpp"
#include "2D/Surface.hpp"
#include "cInputField.hpp"
#include "Types/iRect.hpp"
#include "Types/iXY.hpp"
#include "Component.hpp"
#include "MouseEvent.hpp"

class PackedSurface;

enum { BLANK, MINI_MAP, IMAGE_TILES };
const int SNAPTO_TOLERANCE = 20;

class cLabel
{
public:
    iXY   pos;
    char *label;
    uint8_t  foreColor;
    uint8_t  backColor;
    bool  isShadowed;
}; // end cLabel

enum DEFAULT_VIEW_BUTTON
{
    CLOSE_VIEW_BUTTON,
    MINMAX_VIEW_BUTTON
};

class View : public iRect
{
    friend class Desktop;
public:
    void add(Component *Component);
    void add(DEFAULT_VIEW_BUTTON button);

public:
    enum
    {
        MAX_COMPONENT_COUNT = 30
    };

    Component *componentList[MAX_COMPONENT_COUNT];
    int        componentsUsedCount;
    Component *focusComponent;

    std::vector<cButton*>     buttons;
    std::vector<cInputField*> inputFields;

    enum
    {
        MAX_WINDOW_CLIENT_XSIZE = 632
    };
    enum
    {
        MAX_WINDOW_CLIENT_YSIZE = 458
    };

    int getPressedButton()
    {
        return pressedButton;
    }
    int getPrevPressedButton()
    {
        return prevPressedButton;
    }
    int getHighlightedButton()
    {
        return highlightedButton;
    }
    int getPrevHighlightedButton()
    {
        return prevHighlightedButton;
    }

protected:
    virtual void     actionPerformed(mMouseEvent )
    {}
    Surface*         getViewArea(Surface& dest);
    virtual Surface* getClientArea(Surface& dest);

    int              pressedButton;
    int              prevPressedButton;
    int              highlightedButton;
    int              prevHighlightedButton;

    int              selectedInputField;
    char            *searchName;
    char            *title;
    char            *subTitle;
    int              status;
    static Surface   pics;
    static Surface   topBorder;
    static Surface   leftBorder;
    static Surface   bottomBorder;
    static Surface   rightBorder;
    static Surface   topLeftCornerLarge;
    static Surface   topLeftCornerSmall;
    static Surface   topRightCornerLarge;
    static Surface   topRightCornerSmall;
    static Surface   bottomLeftCornerLarge;
    static Surface   bottomLeftCornerSmall;
    static Surface   bottomRightCornerLarge;
    static Surface   bottomRightCornerSmall;

    int              numLabels;
    cLabel          *labels;
    char            *statusText;

    enum { MINIMIZE, CLOSE      };
    enum { RESIZE_XMINSIZE = 15 };
    enum { RESIZE_YMINSIZE = 15 };

    // Status items
    enum { STATUS_ACTIVE             = (1U << 0) };
    enum { STATUS_VISIBLE            = (1U << 1) };
    enum { STATUS_ALLOW_RESIZE       = (1U << 2) };
    enum { STATUS_ALLOW_MOVE         = (1U << 3) };
    enum { STATUS_ALWAYS_ON_BOTTOM   = (1U << 4) };
    enum { STATUS_BORDERED           = (1U << 5) };
    enum { STATUS_DISPLAY_STATUS_BAR = (1U << 6) };
    enum { STATUS_SCROLL_BAR         = (1U << 7) };

    // Mouse actions
    enum { MA_RESIZE_TOP    = (1U <<  0) };
    enum { MA_RESIZE_LEFT   = (1U <<  1) };
    enum { MA_RESIZE_BOTTOM = (1U <<  2) };
    enum { MA_RESIZE_RIGHT  = (1U <<  3) };
    enum { MA_MOVE          = (1U <<  4) };
    enum { MA_CLOSE         = (1U <<  5) };
    enum { MA_MINIMIZE      = (1U <<  6) };
    enum { MA_MAXIMIZE      = (1U <<  7) };
    enum { MA_RESTORE       = (1U <<  8) };
    enum { MA_IMAGE_TILES   = (1U <<  9) };
    enum { MA_SCROLL_BAR    = (1U << 10) };
    enum { MA_MOUSE_ENTER   = (1U << 11) };
    enum { MA_MOUSE_EXIT    = (1U << 12) };

    void        reset     ();
    void        activate  ();
    void        deactivate();
    static void loadPics  ();

protected:
    typedef void (*ITEM_FUNC)(void);

    enum { BACK_COLOR = 0 };

    // View Status Functions.
    void setAllowResize     (const bool &newStatus);
    void setDisplayStatusBar(const bool &newStatus);
    void setBordered        (const bool &newStatus);
    void setAllowMove       (const bool &newStatus);
    void setActive          (const bool &newStatus);
    //void setScrollBar       (const bool &newStatus);

    // Scroll bar functions.

    // cLabel Functions.
    void addLabel(const iXY &pos, char *label, const PIX &color);
    void addLabelShadowed(const iXY &pos, char *label, const PIX &foreColor, const PIX &backColor);
    void addLabel(const iXY &pos, char *label, const bool &isShadowed, const PIX &foreColor, const PIX &backColor);
    void drawLabels(Surface &clientArea);

    // cButton Functions.
    void addButtonPackedSurface(const iXY &pos, PackedSurface &source, const char *toolTip, ITEM_FUNC leftClickFunc);
    void addButtonCenterText(const iXY &pos, const int &xSize, const char *nName, const char *nToolTip, ITEM_FUNC nLeftClickFunc);
    void addButtonBMP(const iXY &pos, const char *imageName, const char *toolTip, ITEM_FUNC func, const bool &isBordered);
    inline void addButtonBMP(const iXY &pos, const char *imageName, const char *toolTip, ITEM_FUNC func)
    {
        addButtonBMP(pos, imageName, toolTip, func, false);
    }
    inline void addButtonBMPBordered(const iXY &pos, const char *imageName, const char *toolTip, ITEM_FUNC func)
    {
        addButtonBMP(pos, imageName, toolTip, func, true);
    }
    void addButtonSurface(const iXY &pos, Surface &source, const char *toolTip, ITEM_FUNC func);
    void addButtonSurfaceSingle(const iXY &pos, Surface &source, const char *toolTip, ITEM_FUNC func);
    /*!FIXME!*/ void drawDefinedButtons   (Surface &clientArea);
    void drawHighlightedButton(Surface &clientArea);
    void drawPressedButton(Surface &clientArea);
    void setPressedButton(const int &cButton);
    void setHighlightedButton(const int &cButton);
    int  findButtonContaining(const iXY &pos);

    // SearchName, Title, and SubTitle functions.
    void  setSearchName(const char *searchName);
    void  setTitle(const char *title);
    void  setSubTitle(const char *subTitle);
    void  drawTitle(Surface &windowArea);

    // Input Field Functions
    cInputField* addInputField(const iXY &pos, cInputFieldString *string, const char *excludedCharacters, const bool &isSelected);
    int  findInputFieldContaining(const iXY &pos);
    void drawInputFields(Surface &clientArea);

    /////////////////////////////////
    void draw(Surface& drawon);
    void showStatus(const char *string);
    void drawStatus(Surface &dest);
    void checkResolution(iXY oldResolution, iXY newResolution);
    void checkArea(iXY viewarea);
    void toggleView();
    iXY  getScreenToClientPos(const iXY &pos);
    iXY  getScreenToViewPos(const iXY &pos);
    /////////////////////////////////

    // These options can be modified on a per View type basis
    virtual void drawButtons(Surface &windowArea);
    virtual void drawBorder(Surface &windowArea);
    virtual void doDraw(Surface &windowArea, Surface &clientArea);
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
    //virtual void keyDown(uint8_t keyCode);
    virtual void mouseEnter(const iXY &pos);
    virtual void mouseExit(const iXY &pos);
    //virtual void keyUp();
    void scrollBarMove(const iXY &prevpos, const iXY &newpos);

    void resize(const iXY &size);
    inline void resize(const int &x, const int &y)
    {
        resize(iXY(x, y));
    }

    void resizeClientArea(const iXY &size);
    inline void resizeClientArea(const int &x, const int &y)
    {
        resizeClientArea(iXY(x, y));
    }

    virtual void processEvents();

    int moveAreaHeight;
    int borderSize;
    int snapToTolerance;

    Surface* currentscreen; // HACK

public:
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
    void removeAllButtons()
    {
        buttons.clear();
    }
    void removeComponents()
    {
        // Clear out all the previous component data.
        assert(MAX_COMPONENT_COUNT > 0);
        memset(componentList, 0, sizeof(Component *) * MAX_COMPONENT_COUNT);

        componentsUsedCount = 0;
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
    inline int getResize() const
    {
        return status & STATUS_ALLOW_RESIZE;
    }
    inline int getScrollBar() const
    {
        return status & STATUS_SCROLL_BAR;
    }

    virtual int getMouseActions(const iXY &p) const;

    iRect getClientRect() const;
};

#endif // end __View_hpp__
