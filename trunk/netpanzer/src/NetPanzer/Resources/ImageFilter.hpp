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
 * Created on September 28, 2013, 2:07 AM
 */

#ifndef IMAGEFILTER_HPP
#define	IMAGEFILTER_HPP

#include "2D/Surface.hpp"
#include "Core/CoreTypes.hpp"

class ImageFilter
{
public:
    inline bool isLoaded() const { return loaded; }

    inline void unload()
    {
        delete[] data;
        data = 0;
        loaded = false;
    }
    
    NPString name;
    NPString file;
    bool loaded;

    uint8_t * data;
    
private:
    friend class ResourceManager;
    ImageFilter() : loaded(false), data(0) {}
    ~ImageFilter() {if ( loaded ) unload(); }
    
};

class ImageFilterResource
{
public:
    ImageFilterResource() : ptr(0)
    {
        // @todo use some default image?
    }
    ImageFilterResource(ImageFilter * ptr) : ptr(ptr) {}
    ~ImageFilterResource() {}
    
    inline ImageFilterResource(const ImageFilterResource& other) : ptr(other.ptr) {}
    inline void operator=(const ImageFilterResource& other)
    {
        if ( ptr != other.ptr )
        {
            ptr = other.ptr;
        }
    }
    
    void applyRounded(Surface& dest, const iRect& rect, const int radius) const;
    
    inline bool isLoaded() const { return ptr->isLoaded(); }
    
private:
    ImageFilter* ptr;
};

#endif	/* IMAGEFILTER_HPP */

