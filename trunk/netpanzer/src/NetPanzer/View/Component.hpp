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
#include <stdint.h>

#include "2D/Color.hpp"
#include "2D/Surface.hpp"
#include "Types/iXY.hpp"
#include "Types/iRect.hpp"
#include "Util/NoCopy.hpp"

class mMouseEvent;

//--------------------------------------------------------------------------
class Component : public NoCopy
{
protected:
    uint8_t    background;
    uint8_t    foreground;
    iXY     size;
    iXY     min;
    Surface surface;
    std::string name;
    bool    enabled;
    bool    visible;

    static int borderSize;

    void reset();

public:
    Component *next;
    Component *prev;

    void *parent;       // Who is my daddy?

    void setParent(void *parent);

    enum
    {
        BOTTOM_ALIGNMENT,
        CENTER_ALIGNMENT,
        LEFT_ALIGNMENT,
        RIGHT_ALIGNMENT,
        TOP_ALIGNMENT,
    };

    Component()
    {
        reset();
    }
    virtual ~Component()
    {}

    // Accessor functions.
    bool contains(int x, int y) const;
    bool            contains(iXY p) const
    {
        return contains(p.x, p.y);
    }
           uint8_t     getBackground() const
    {
        return background;
    }
    void getBounds(iRect &r)
    {
        r.min = min;
        r.max = min + size;
    }
           uint8_t     getForeground() const
    {
        return foreground;
    }
    Surface& getGraphics()
    {
        return surface;
    }
    const std::string& getName() const
    {
        return name;
    }
    const  iXY     &getSize() const
    {
        return size;
    }
    int             getSizeX() const
    {
        return size.x;
    }
    int             getSizeY() const
    {
        return size.y;
    }
           bool     isEnabled() const
    {
        return enabled;
    }
    //const bool &isValid() {}
           bool     isVisible() const
    {
        return visible;
    }

    void setBackground(uint8_t color)
    {}
    void setBounds(const iRect &r)
    {
        min  = r.min;
        size = r.getSize();
    }

    void setEnabled(bool enabled);
    void setForeground(PIX foreground);
    void setLocation(int x, int y);
    void setLocation(const iXY &p)
    {
        setLocation(p.x, p.y);
    }
    void setName(const std::string& name)
    {
        Component::name = name;
    }
    void setSize(int width, int height)
    {}
    void setSize(const iXY &d)
    {
        setSize(d.x, d.y);
    }
    void setVisible(bool b)
    {}

    virtual void draw(Surface &dest) = 0;
    virtual void actionPerformed(const mMouseEvent &me) = 0;
}; // end Component

#endif // end __Component_hpp__

