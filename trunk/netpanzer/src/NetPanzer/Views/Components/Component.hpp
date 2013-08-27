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

#ifndef __Component_hpp__
#define __Component_hpp__

#include "2D/Surface.hpp"
#include "Types/iRect.hpp"
#include "2D/ComponentEvents.hpp"

class mMouseEvent;
namespace GameInput { class InputState; }

//--------------------------------------------------------------------------
class Component
{
protected:
    iRect   rect;

    static int borderSize;

    ComponentEvents * events;

    void reset() { rect.zero(); }

public:
    Component() : events(0)
    {
        reset();
    }

    virtual ~Component()
    {
    }

    void setComponentEvents(ComponentEvents *ce) { events = ce; }

    // Accessor functions.
    inline void getBounds(iRect &r) { r = rect; }
    
    inline bool contains(const int x, const int y) const { return rect.contains(x, y); }
    inline bool contains(const iXY& p) const { return rect.contains(p); }

    virtual void setSize(const int x, const int y) { rect.setSize(x, y); }

    inline int      getLocationX() const { return rect.getLocationX(); }
    inline int      getLocationY() const { return rect.getLocationY(); }
    inline unsigned getWidth()     const { return rect.getWidth(); }
    inline unsigned getHeight()    const { return rect.getHeight(); }

    virtual void setLocation(const int x, const int y) { rect.setLocation(x, y); }
    void setLocation(const iXY &p) { setLocation(p.x, p.y); }

    virtual void draw(Surface &dest) = 0;

    virtual void actionPerformed(const mMouseEvent &me) {}
    virtual void handleKeyboard() {}

    virtual void onSelectStart() {}
    virtual void onSelectStop() {}

    virtual void onHoverStart() {}
    virtual void onHoverStop() {}
    
    virtual void onFocusStart() {}
    virtual void onFocusStop() {}
    virtual void handleInput( GameInput::InputState * input ) { (void)input; }
    
    virtual void onPointerMove(int x, int y) {}

    virtual void onMousePress(int button) {}
    virtual void onMouseRelease(int button) {}
    virtual void onMouseClick(int button) {} // @todo do we want this event?

    virtual void onMouseMove(int x, int y) {}
    virtual void onMouseDrag(int x, int y, int button) {}

private:
    Component(const Component&);
    void operator=(const Component&);
}; // end Component

#endif // end __Component_hpp__

