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
 */

#include "ImageFilter.hpp"
#include "json/json.h"
#include "ResourceManager.hpp"
#include "Util/FileSystem.hpp"

template<> ImageFilter * ResourceManager::resourceFromJSon<ImageFilter>(const Json::Value& node)
{
    if ( node.isMember("file") )
    {
        ImageFilter * b = new ImageFilter();
        b->file   = node["file"].asString();
        return b;
    }
    
    return 0;
}

bool ImageFilter::load()
{
    filesystem::ReadFile f(file);
    if ( f.isOpen() )
    {
        const unsigned imagefiltersize = 256;
        uint8_t * buffer = new uint8_t[imagefiltersize];
        f.read(buffer, imagefiltersize, 1);
        data = buffer;
        loaded = true;
    }
    return loaded;
}

// @todo full check on sizing
void ImageFilterResource::applyRounded(Surface& dest, const iRect& rect, const int radius) const
{
//    dest.BltRoundRect(rect, radius, data());
    int w = dest.getWidth();
    int h = dest.getHeight();
    
    
    if ( !w || !h ) return;
    
    // Check for trivial rejection
    if      (rect.isEmpty())     return;
    else if (rect.getLocationX() >= w) return;
    else if (rect.getLocationY() >= h) return;

    

    // Check for clipping
    const int destx = rect.getLocationX() <  0 ? 0 : rect.getLocationX();
    const int desty = rect.getLocationY() <  0 ? 0 : rect.getLocationY();
//    if (rect.max.x >= (int)getWidth())  rect.max.x = getWidth() - 1;
//    if (rect.max.y >= (int)getHeight()) rect.max.y = getHeight() - 1;

    int source_lines = rect.getHeight() - (radius*2) - 2;
    int source_cols = rect.getWidth();
    int source_start_y = desty + radius + 1;
    int source_start_x = destx;

    unsigned dest_lines = std::min(h - source_start_y, source_lines);
    unsigned dest_cols = std::min(w - source_start_x, source_cols);

    int srcAdjustment  = dest.getPitch()      - dest_cols;

    PIX	*sPtr = dest.pixPtr( source_start_x, source_start_y);  // Pointer to source Surface start of memory.
    const uint8_t * table = ptr->data;

    for (unsigned row = 0; row < dest_lines; row++)
    {
        for (unsigned col = 0; col < dest_cols; col++)
        {
            *sPtr = table[*sPtr];
            sPtr++;
        }

        sPtr += srcAdjustment;
    }
    
//    bltLookup(iRect( rect.getLocationX(), rect.getLocationY()+radius+1,
//                     rect.getWidth(), rect.getHeight() - (radius*2) - 2), table);
//
    int d, y, x;

    d = 3 - (2 * radius);
    x = 0;
    y = radius;

    while (y >= x)
    {
//        bltHLine((rect.getLocationX()+radius) - y, (rect.getLocationY()+radius) - x, rect.getWidth() - (radius-y) + y - radius, table);//up
        int dx = destx + radius - y;
        int dy = desty + radius - x;
        int dn = dest_cols - (radius-y) + y - radius;
        sPtr = dest.pixPtr(dx, dy);
        for(int r=0; r<dn; r++) sPtr[r] = table[sPtr[r]];
        
//        bltHLine((rect.getLocationX()+radius) - y, (rect.getEndY()-radius) + x,      rect.getWidth() - (radius-y) + y - radius, table);//down
//        int dx = destx + radius - y;
        dy = desty + (rect.getHeight()-1) - radius + x; // maybe should do too -1
//        int dn = dest_cols - (radius-y) + y - radius;
        sPtr = dest.pixPtr(dx, dy);
        for(int r=0; r<dn; r++) sPtr[r] = table[sPtr[r]];

        if (d < 0)
        {
            d = d + (4 * x) + 6;
        }
        else
        {
            d = d + 4 * (x - y) + 10;
            if ( y > x )
            {
//                bltHLine((rect.getLocationX()+radius) - x, (rect.getLocationY()+radius) - y, rect.getWidth() - (radius-x) + x - radius, table);//up
                dx = destx + radius - x;
                dy = desty + radius - y;
                dn = dest_cols - (radius-x) + x - radius;
                sPtr = dest.pixPtr(dx, dy);
                for(int r=0; r<dn; r++) sPtr[r] = table[sPtr[r]];
                
//                bltHLine((rect.getLocationX()+radius) - x, (rect.getEndY()-radius) + y,      rect.getWidth() - (radius-x) + x - radius, table);//down
//                dx = destx + radius - x;
                dy = desty + (rect.getHeight()-1) - radius + y;
//                dn = dest_cols - (radius-x) + x - radius;
                sPtr = dest.pixPtr(dx, dy);
                for(int r=0; r<dn; r++) sPtr[r] = table[sPtr[r]];
            }
            y--;
        }

        x++;
    }

}
