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
 * Created on November 16, 2013, 9:41 AM
 */

#ifndef TILESET_HPP
#define	TILESET_HPP

#include "2D/Surface.hpp"
#include "Core/CoreTypes.hpp"

class Tileset
{
public:
    inline bool isLoaded() const { return loaded; }

    bool load();

    inline void unload()
    {
        delete[] move_value;
        delete[] avg_color;
        move_value = avg_color = 0;
        loaded = false;
    }
    
    NPString file;
    bool loaded;

    NPString idheader;
    unsigned tile_count;
    unsigned tile_width;
    unsigned tile_height;
    
    uint8_t* move_value;
    uint8_t* avg_color;
    
    
private:
    friend class ResourceManager;
    Tileset() : loaded(false) {}
    ~Tileset() {}

};

class TilesetResource
{
public:
    TilesetResource() : ptr(0)
    {
        // @todo use some default image?
    }
    
    TilesetResource(Tileset * ptr) : ptr(ptr) {}
    ~TilesetResource() {}
    
    inline TilesetResource(const TilesetResource& other) : ptr(other.ptr) {}
    inline void operator=(const TilesetResource& other)
    {
        if ( ptr != other.ptr )
        {
            ptr = other.ptr;
        }
    }
    
    inline bool isLoaded() const { return ptr->isLoaded(); }
    
private:
    Tileset* ptr;
};

#endif	/* TILESET_HPP */

