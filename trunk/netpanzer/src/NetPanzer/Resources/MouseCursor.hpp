/*
Copyright (C) 2013 Netpanzer Team. (www.netpanzer.org), Aaron Perez

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
 *
 * Created on September 24, 2013, 8:45 PM
 */

#ifndef MOUSECURSOR_HPP
#define	MOUSECURSOR_HPP

#include "2D/Surface.hpp"
#include "Core/CoreTypes.hpp"

class MouseCursor
{
public:
    inline bool isLoaded() const { return loaded; }

    bool load();

    inline void unload()
    {
        image.free();
        loaded = false;
    }
    
    NPString file;
    bool loaded;

    Surface image;
    int offset_x;
    int offset_y;
    
private:
    friend class ResourceManager;
    MouseCursor() : loaded(false){}
    ~MouseCursor() {}
    
};

class MouseCursorResource
{
public:
    MouseCursorResource() : ptr(0)
    {
        // @todo put the default mouse!
    }
    MouseCursorResource(MouseCursor * mc) : ptr(mc) {}
    ~MouseCursorResource() {}
    
    inline MouseCursorResource(const MouseCursorResource& other) : ptr(other.ptr) {}
    inline void operator=(const MouseCursorResource& other)
    {
        if ( ptr != other.ptr )
        {
            ptr = other.ptr;
        }
    }
    
    inline void draw(Surface& dest, const int x, const int y) const
    {
        ptr->image.bltTrans(dest, x + ptr->offset_x, y + ptr->offset_y);
    }
    
    inline bool isLoaded() const { return ptr->isLoaded(); }
    
private:
    MouseCursor * ptr;
};

#endif	/* MOUSECURSOR_HPP */

