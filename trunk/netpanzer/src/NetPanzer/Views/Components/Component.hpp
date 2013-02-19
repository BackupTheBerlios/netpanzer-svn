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
#include "Types/iXY.hpp"
#include "Types/iRect.hpp"

class mMouseEvent;

//--------------------------------------------------------------------------
class Component
{
protected:
    iXY     position;
    iXY     size;
    Surface surface;
    bool    dirty;

    static int borderSize;

    void reset();

public:
    Component() : clickAction(0)
    {
        reset();
    }

    virtual ~Component()
    {
    }

    // Accessor functions.
    void getBounds(iRect &r)
    {
        r.setLocation(position);
        r.setSize(size);
    }
    bool contains(const int x, const int y) const;
    bool contains(const iXY& p) const { return contains(p.x, p.y); }

    void setBounds(const iRect &r)
    {
        position  = r.getLocation();
        size = r.getSize();
        //surface.create(r.getSizeX(), r.getSizeY(), 1);
        dirty = true;
    }

    virtual void setSize(const int x, const int y)
    {
        size.x=x;
        size.y=y;
        surface.create(x,y);
        dirty=true;
    }

    virtual void setLocation(const int x, const int y);
    void setLocation(const iXY &p)
    {
        setLocation(p.x, p.y);
    }

    virtual void draw(Surface &dest)
    {
        if ( dirty )
            render();

        surface.bltTrans(dest, position.x, position.y ); // blit full
    }

    virtual void render() = 0;
    virtual void actionPerformed(const mMouseEvent &me) = 0;
    virtual void handleKeyboard() {}
    
    inline void setClickAction(int ca) { clickAction = ca; }
    inline int getClickAction() const { return clickAction; }

private:
    Component(const Component&);
    void operator=(const Component&);
    
    int clickAction;
}; // end Component

#endif // end __Component_hpp__

