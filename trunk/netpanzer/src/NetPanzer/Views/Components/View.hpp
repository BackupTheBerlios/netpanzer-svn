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

#include "2D/Surface.hpp"
#include "2D/PackedSurface.hpp"
#include "Types/iRect.hpp"
#include "Types/iXY.hpp"
#include "Component.hpp"
#include "MouseEvent.hpp"

#include <algorithm>
#include "ArrayUtil/PtrArray.hpp"

using namespace std;

class View : public iRect
{
private:
    friend class Desktop;
    
    typedef PtrArray<Component> ComponentList;
    
    ComponentList components;
    Component *focusComponent;

    char            *searchName;
    int              status;

    enum { STATUS_ACTIVE             = (1U << 0) };
    enum { STATUS_VISIBLE            = (1U << 1) };
    enum { STATUS_ALLOW_MOVE         = (1U << 2) };
    enum { STATUS_ALWAYS_ON_BOTTOM   = (1U << 3) };
    
    inline void show() {}
    inline void hide() {}
    
public:
    View(){}
    virtual ~View(){}

    inline const char *getSearchName() const { return searchName; }
    
    inline bool isVisible() const { return false; }
    inline bool isActive() const  { return false; }
    inline bool isMovable() const { return false; }
    inline bool isAlwaysOnBottom() const { return false; }

protected:
    void add(Component *Component){}
    
    virtual void     actionPerformed(mMouseEvent ) {}
    virtual void     onDesktopResized( const iXY& oldResolution, const iXY& newResolution){}
    virtual void     onShow() {}
    virtual void     onHide() {}
    
    void        reset     (){}
    void        activate  (){}
    void        deactivate(){}

    // View Status Functions.
    void setAllowMove       (const bool &newStatus){}
    void setActive          (const bool &newStatus){}

    // SearchName, Title, and SubTitle functions.
    void  setSearchName(const char *searchName){}

    /////////////////////////////////
    void draw(Surface& drawon){}
    
    /** Ensures the view is inside the screen, XXX REVIEW */
    void checkArea(iXY viewarea){}
    
    iXY  getScreenToClientPos(const iXY &pos){return iXY(0,0);}
    iXY  getScreenToViewPos(const iXY &pos){return iXY(0,0);}
    /////////////////////////////////

    // These options can be modified on a per View type basis
public:
    virtual void doDraw( Surface& dest ){}
    
protected:
    
    virtual void doActivate(){}
    virtual void doDeactivate(){}
    virtual void mouseMove(const iXY &prevPos, const iXY &newPos){}
    virtual void lMouseDown(const iXY &pos){}
    virtual int  lMouseUp(const iXY &downPos, const iXY &upPos){return 0;}
    virtual void lMouseDrag(const iXY &downPos, const iXY &prevPos, const iXY &newPos){}
    virtual void rMouseDown(const iXY &pos){}
    virtual void rMouseUp(const iXY &downPos, const iXY &upPos){}
    virtual void rMouseUp(){}
    virtual void rMouseDrag(const iXY &downPos, const iXY &prevPos, const iXY &newPos){}

    virtual void mouseEnter(const iXY &pos){}
    virtual void mouseExit(const iXY &pos){}

    void resize(const iXY &size){}
    inline void resize(const int &x, const int &y) {}

    void resizeClientArea(const iXY &size) {}
    inline void resizeClientArea(const int &x, const int &y) {}

public:
    virtual void processEvents() {}

    // Hack city, should be protected???????
    void setAlwaysOnBottom(const bool &newStatus){}

    void moveTo(iXY destMin){}
    inline void moveTo(const int &x, const int &y) {}
    void removeComponents() {}
    void removeComponent(Component *c) {}

    iRect getClientRect() const { return iRect(0,0,0,0); }
};

#endif // end __View_hpp__
