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
 * Created on September 28, 2013, 12:15 AM
 */

#ifndef IMAGE_HPP
#define	IMAGE_HPP

#include "2D/Surface.hpp"
#include "Core/CoreTypes.hpp"

class Image
{
public:
    inline bool isLoaded() const { return loaded; }

    bool load();
    
    inline void unload()
    {
        image.free();
        loaded = false;
    }
    
    NPString name;
    NPString file;
    bool loaded;

    Surface image;
    unsigned width;
    unsigned height;
    
private:
    friend class ResourceManager;
    Image() : loaded(false){}
    ~Image() {}
    
};

class ImageResource
{
public:
    ImageResource() : ptr(0)
    {
        // @todo use some default image?
    }
    ImageResource(Image * ptr) : ptr(ptr) {}
    ~ImageResource() {}
    
    inline ImageResource(const ImageResource& other) : ptr(other.ptr) {}
    inline void operator=(const ImageResource& other)
    {
        if ( ptr != other.ptr )
        {
            ptr = other.ptr;
        }
    }
    
    inline void draw(Surface& dest, const int x, const int y) const
    {
        ptr->image.blt(dest, x, y); // @todo decide: use blt or bltTrans?
    }
    
    inline unsigned getWidth() const { return ptr->width; }
    inline unsigned getHeight() const { return ptr->height; }
    
    inline bool isLoaded() const { return ptr->isLoaded(); }
    
private:
    Image * ptr;
};

#endif	/* IMAGE_HPP */

