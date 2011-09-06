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

#include <string>

#include "2D/Color.hpp"
#include "2D/Surface.hpp"
#include "Types/iXY.hpp"
#include "Types/iRect.hpp"

class mMouseEvent;

//--------------------------------------------------------------------------
class Component
{
protected:
    iXY     size;
    iXY     position;

    void reset();

public:
    void *parent;       // Who is my daddy?

    void setParent(void *parent);

    Component()
    {
        reset();
    }
        
    virtual ~Component()
    {
    }

    void getBounds(iRect &r)
    {
        r.min = position;
        r.max = position + size;
    }
    bool contains(int x, int y) const;
    bool contains(iXY p) const { return contains(p.x, p.y); }

    void setBounds(const iRect &r)
    {
        position  = r.min;
        size = r.getSize();
    }
    
    virtual void setSize(int x, int y)
    {
        size.x=x;
        size.y=y;
    }

    void setLocation(int x, int y);
    void setLocation(const iXY &p) { setLocation(p.x, p.y); }

    virtual void draw(Surface &dest) = 0;
    virtual void actionPerformed(const mMouseEvent &me) = 0;
    virtual void handleKeyboard() {}

private:
    Component(const Component& )
    { }
    void operator=(const Component& )
    { }
}; // end Component

#endif // end __Component_hpp__

