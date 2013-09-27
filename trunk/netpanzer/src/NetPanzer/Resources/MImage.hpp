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
 * Created on September 28, 2013, 12:55 AM
 */

#ifndef MIMAGE_HPP
#define	MIMAGE_HPP


#include "2D/PackedSurface.hpp"
#include "Core/CoreTypes.hpp"
#include "BlendTable.hpp"

class MImage
{
public:
    inline bool isLoaded() const { return loaded; }

    inline void unload()
    {
        image.free();
        loaded = false;
    }
    
    NPString name;
    NPString file;
    bool loaded;

    PackedSurface image;
    unsigned width;
    unsigned height;
    int offset_x;
    int offset_y;
    unsigned frames;
    unsigned fps;
    
private:
    friend class ResourceManager;
    MImage() : loaded(false){}
    ~MImage() {}
    
};

class MImageResource
{
public:
    MImageResource() : ptr(0)
    {
        // @todo use some default image?
    }
    MImageResource(MImage * ptr) : ptr(ptr) {}
    ~MImageResource() {}
    
    inline MImageResource(const MImageResource& other) : ptr(other.ptr) {}
    inline void operator=(const MImageResource& other)
    {
        if ( ptr != other.ptr )
        {
            ptr = other.ptr;
        }
    }
    
    inline void draw(Surface& dest, const unsigned frame, const int x, const int y) const
    {
        ptr->image.bltFrame(dest, frame, x + ptr->offset_x, y + ptr->offset_y);
    }
    
    inline void blend(Surface& dest, const unsigned frame, const int x, const int y, const BlendTableResource& blendTable) const
    {
        ptr->image.bltBlendFrame(dest, frame, x + ptr->offset_x, y + ptr->offset_y, blendTable.data());
    }
    
    inline unsigned getWidth() const { return ptr->width; }
    inline unsigned getHeight() const { return ptr->height; }
    
    inline bool isLoaded() const { return ptr->isLoaded(); }
    
private:
    MImage * ptr;
};

#endif	/* MIMAGE_HPP */

