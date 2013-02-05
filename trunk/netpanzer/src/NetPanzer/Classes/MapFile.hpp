/*
Copyright (C) 2004 Hollis Blanchard <hollis@penguinppc.org>
 
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
#ifndef _MAPFILE_HPP
#define _MAPFILE_HPP

#include "Core/CoreTypes.hpp"

class Surface;

class MapFile
{
public:
    typedef uint16_t ElementType;
    enum { InvalidElement = 0xffff };

    ~MapFile()
    {
        if ( data )
        {
            delete[] data;
            data = 0;
        }
        
        if ( thumbnail )
        {
            delete thumbnail;
            thumbnail = 0;
        }
    }

    inline bool hasData()       const { return data != 0; }
    inline bool hasThumbnail()  const { return thumbnail != 0; }
    
    inline uint16_t getWidth()  const { return width; }
    inline uint16_t getHeight() const { return height; }
    
    inline ElementType getValue( size_t offset ) const
    {
        return ( offset < data_length ) ? data[offset] : InvalidElement;
    }
    
    inline ElementType getValue( size_t x, size_t y ) const
    {
        return getValue( (y * getWidth()) + x );
    }
    
    inline unsigned getOutpostCount() const { return outpost_count; }
    
    NPString filename;
    NPString fullfilename;
    
    char netp_id_header[64];
    uint16_t id;
    char name[256];
    char description[1024];
    uint16_t width;
    uint16_t height;
    char tile_set[256];
    
    size_t data_length;
    ElementType * data;
    
    Surface * thumbnail;

    uint16_t thumbnail_width;
    uint16_t thumbnail_height;
    
    unsigned outpost_count;
};

#endif // ** _MAPFILE_HPP
