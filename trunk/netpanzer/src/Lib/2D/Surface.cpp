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


#include <math.h>
#include <ctype.h>
#include <memory>
#include <vector>
#include <string>
#include <algorithm>

#include "Surface.hpp"

#include "Util/Log.hpp"
#include "Util/FileSystem.hpp"
#include "Types/fXY.hpp"
#include "Util/TimerInterface.hpp"
#include "Util/Exception.hpp"
#include "Palette.hpp"
#include "ArrayUtil/PtrArray.hpp"

#include "Color.hpp"

#include "libpng-1.6.5/png.h"

#include "ft2build.h"
#include FT_FREETYPE_H

using std::swap;

#define FONT_HEIGHT 10
#define FONT_WIDTH 8
#define FONT_MAXCHAR 191

// orderCoords
//---------------------------------------------------------------------------
// Purpose: Orders a pair of (x,y) coordinates
//---------------------------------------------------------------------------
template <class T>
inline void orderCoords(T &a, T &b)
{
    if (a > b) swap(a, b);
} // end orderCoords

class BitmapFileHeader
{
public:
    uint16_t    bfType;
    uint32_t   bfSize;
    uint16_t    bfReserved1;
    uint16_t    bfReserved2;
    uint32_t   bfOffBits;

    BitmapFileHeader(filesystem::ReadFile& file);
};

BitmapFileHeader::BitmapFileHeader(filesystem::ReadFile& file)
{
    bfType = file.readULE16();
    bfSize = file.readULE32();
    bfReserved1 = file.readULE16();
    bfReserved2 = file.readULE16();
    bfOffBits = file.readULE32();
}

#define BI_RGB      0L
#define BI_RLE8     1L
#define BI_RLE4     2L

class BitmapInfoHeader
{
public:
    uint32_t  biXY;
    uint32_t  biWidth;
    uint32_t  biHeight;
    uint16_t   biPlanes;
    uint16_t   biBitCount;
    uint32_t  biCompression;
    uint32_t  biXYImage;
    uint32_t  biXPelsPerMeter;
    uint32_t  biYPelsPerMeter;
    uint32_t  biClrUsed;
    uint32_t  biClrImportant;

    BitmapInfoHeader(filesystem::ReadFile& file);
};

BitmapInfoHeader::BitmapInfoHeader(filesystem::ReadFile& file)
{
    biXY = file.readULE32();
    biWidth = file.readULE32();
    biHeight = file.readULE32();
    biPlanes = file.readULE16();
    biBitCount = file.readULE16();
    biCompression = file.readULE32();
    biXYImage = file.readULE32();
    biXPelsPerMeter = file.readULE32();
    biYPelsPerMeter = file.readULE32();
    biClrUsed = file.readULE32();
    biClrImportant = file.readULE32();
}

int Surface::totalSurfaceCount = 0;
int Surface::totalByteCount    = 0;

// Surface
//---------------------------------------------------------------------------

Surface::Surface()
{
    reset();

    totalSurfaceCount++;
    totalByteCount += sizeof(Surface);
} // end Surface::Surface

// Surface
//---------------------------------------------------------------------------
Surface::Surface(unsigned int w, unsigned int h)
{
    reset();

    create( w, h);

    totalSurfaceCount++;
    totalByteCount += sizeof(Surface);
} // end Surface::Surface

// ~Surface
//---------------------------------------------------------------------------
Surface::~Surface()
{
    if ( getDoesExist() )
    {
        free();
    }

    totalSurfaceCount--;
    assert(totalSurfaceCount >= 0);

    totalByteCount -= sizeof(Surface);
    assert(totalByteCount >= 0);
} // end Surface::~Surface

//---------------------------------------------------------------------------
void Surface::free()
{
    if ( mem )
    {
        ::free(mem);

        totalByteCount -= getPitch() * getHeight() * sizeof(PIX);

        assert(totalByteCount >= 0);
    }

    mem     = 0;
    twidth  = 0;
    theight = 0;
    tpitch  = 0;
}

// reset
//---------------------------------------------------------------------------
void Surface::reset()
{
    assert(this != 0);

    twidth      = 0;
    theight     = 0;
    tpitch      = 0;

    mem         = 0;
} // end Surface::reset

// grab
//---------------------------------------------------------------------------
// Purpose: Copies a section from another Surface.
//---------------------------------------------------------------------------
bool Surface::grab(const Surface &source,
                   iRect bounds)
{
    assert(source.getDoesExist());
    assert(this != 0);

    free();

    create(bounds.getWidth(), bounds.getHeight());
    source.blt(*this, -bounds.getLocationX(), -bounds.getLocationY()); // full blit

    return true;
} // end Surface::grab

// blt
//---------------------------------------------------------------------------
// Purpose: Puts the surface onto the destination while performing clipping
//          on the bounds of the object.
//---------------------------------------------------------------------------
void Surface::blt(Surface &dest, int x, int y) const
{
    int source_lines = getHeight();
    int source_cols = getWidth();
    int source_start_y = 0;
    int source_start_x = 0;

    if ( (y >= (int)dest.getHeight()) || (x >= (int)dest.getWidth()) )
    {
        return;
    }

    if ( y < 0 )
    {
        if ( -y >= source_lines )
        {
            return;
        }

        source_lines += y;
        source_start_y -= y;
        y = 0;
    }

    if ( x < 0 )
    {
        if ( -x >= source_cols )
        {
            return;
        }
        source_cols += x;
        source_start_x -= x;
        x = 0;
    }

    int dest_lines = std::min(dest.getHeight() - y, (unsigned)source_lines);
    int dest_cols = std::min(dest.getWidth() - x, (unsigned)source_cols);
    PIX	*sPtr = pixPtr( source_start_x, source_start_y);  // Pointer to source Surface start of memory.
    PIX	*dPtr = dest.pixPtr( x, y); // Pointer to destination Surface start of memory.

    if ( (getPitch() == (unsigned)dest_cols) && (dest.getPitch() == (unsigned)dest_cols) )
    {
        memcpy(dPtr, sPtr, dest_lines * dest_cols * sizeof(PIX));
    }
    else
    {
        do
        {
            memcpy(dPtr, sPtr, dest_cols * sizeof(PIX));
            sPtr += getPitch();
            dPtr += dest.getPitch();
        } while ( --dest_lines );
    }
} // end Surface::blt

// bltTrans
//---------------------------------------------------------------------------
// Purpose: Puts the surface onto the destination using the slowest form of
//          transparency detection (pixel by pixel basis), while performing
//          clipping on the bounds of the object.
//---------------------------------------------------------------------------
void Surface::bltTrans(Surface &dest, int x, int y) const
{
    int source_lines = getHeight();
    int source_cols = getWidth();
    int source_start_y = 0;
    int source_start_x = 0;

//    bool logit = (y < 0) ||(x < 0);
    bool logit = false;

    if ( logit )
    {
        LOGGER.warning("BLIT: ssize: %d,%d$%d dsize: %d,%d$%d pos: %d,%d",
                        getWidth(), getHeight(), getPitch(),
                        dest.getWidth(), dest.getHeight(), dest.getPitch(),
                        x, y );
    }

    if ( (y >= (int)dest.getHeight()) || (x >= (int)dest.getWidth()) )
    {
        if ( logit ) LOGGER.warning("BLIT END: completely out");
        return;
    }

    if ( y < 0 )
    {
        if ( -y >= source_lines )
        {
            if ( logit ) LOGGER.warning("BLIT END: negative lines more than source size");
            return;
        }

        source_lines += y;
        source_start_y -= y;
        y = 0;
    }

    if ( x < 0 )
    {
        if ( -x >= source_cols )
        {
            if ( logit ) LOGGER.warning("BLIT END: negative cols more than source size");
            return;
        }
        source_cols += x;
        source_start_x -= x;
        x = 0;
    }

    unsigned dest_lines = std::min(dest.getHeight() - y, (unsigned)source_lines);
    unsigned dest_cols = std::min(dest.getWidth() - x, (unsigned)source_cols);

    int srcAdjustment  = getPitch()      - dest_cols;
    int destAdjustment = dest.getPitch() - dest_cols;

    if ( logit )
    {
        LOGGER.warning("BLIT DATA: sstart: %d,%d ssize: %d,%d sa: %d dstart: %d,%d dsize: %d,%d da: %d",
                       source_start_x, source_start_y,
                       source_cols, source_lines,
                       srcAdjustment,
                       x, y,
                       dest_cols, dest_lines,
                       destAdjustment
                       );
    }

    PIX	*sPtr = pixPtr( source_start_x, source_start_y);  // Pointer to source Surface start of memory.
    PIX	*dPtr = dest.pixPtr( x, y); // Pointer to destination Surface start of memory.
    
    for (unsigned row = 0; row < dest_lines; row++)
    {
        for (unsigned col = 0; col < dest_cols; col++)
        {
            if (*sPtr != 0)
                *dPtr = *sPtr;
            sPtr++;
            dPtr++;
        }

        sPtr += srcAdjustment;
        dPtr += destAdjustment;
    }

} // end Surface::bltTrans

// bltTransC
//---------------------------------------------------------------------------
// Purpose: Puts the surface onto the destination using the slowest form of
//          transparency detection (pixel by pixel basis), while performing
//          clipping on the bounds of the object. The non-transparent pixels
//          are blitted in the specified color.
//---------------------------------------------------------------------------
void Surface::bltTransColor(Surface &dest, int x, int y, const PIX color) const
{
    int source_lines = getHeight();
    int source_cols = getWidth();
    int source_start_y = 0;
    int source_start_x = 0;

    //    bool logit = (y < 0) ||(x < 0);
    bool logit = false;

    if ( logit )
    {
        LOGGER.warning("BLITX: ssize: %d,%d$%d dsize: %d,%d$%d pos: %d,%d",
                        getWidth(), getHeight(), getPitch(),
                        dest.getWidth(), dest.getHeight(), dest.getPitch(),
                        x, y );
    }

    if ( (y >= (int)dest.getHeight()) || (x >= (int)dest.getWidth()) )
    {
        if ( logit ) LOGGER.warning("BLITX END: completely out");
        return;
    }

    if ( y < 0 )
    {
        if ( -y >= source_lines )
        {
            if ( logit ) LOGGER.warning("BLITX END: negative lines more than source size");
            return;
        }

        source_lines += y;
        source_start_y -= y;
        y = 0;
    }

    if ( x < 0 )
    {
        if ( -x >= source_cols )
        {
            if ( logit ) LOGGER.warning("BLITX END: negative cols more than source size");
            return;
        }
        source_cols += x;
        source_start_x -= x;
        x = 0;
    }

    unsigned dest_lines = std::min(dest.getHeight() - y, (unsigned)source_lines);
    unsigned dest_cols = std::min(dest.getWidth() - x, (unsigned)source_cols);

    int srcAdjustment  = getPitch()      - dest_cols;
    int destAdjustment = dest.getPitch() - dest_cols;

    if ( logit )
    {
        LOGGER.warning("BLITX DATA: sstart: %d,%d ssize: %d,%d sa: %d dstart: %d,%d dsize: %d,%d da: %d",
                       source_start_x, source_start_y,
                       source_cols, source_lines,
                       srcAdjustment,
                       x, y,
                       dest_cols, dest_lines,
                       destAdjustment
                       );
    }

    PIX	*sPtr = pixPtr( source_start_x, source_start_y);  // Pointer to source Surface start of memory.
    PIX	*dPtr = dest.pixPtr( x, y); // Pointer to destination Surface start of memory.

    for (unsigned row = 0; row < dest_lines; row++)
    {
        for (unsigned col = 0; col < dest_cols; col++)
        {
            if (*sPtr != 0)
                *dPtr = color;
            sPtr++;
            dPtr++;
        }

        sPtr += srcAdjustment;
        dPtr += destAdjustment;
    }
} // end Surface::bltTransC

// drawHLine
//---------------------------------------------------------------------------
// Purpose: Draws a horizontal drawLine.
//---------------------------------------------------------------------------
void Surface::drawHLine(int x, int y, int size, const PIX color)
{
    assert(getDoesExist());
    assert(this != 0);

    // Check for trivial rejection
    if (    y < 0
         || size <= 0
         || y >= (int)getHeight()
         || x >= (int)getWidth()
       )
    {
         return;
    }

    if ( x < 0 )
    {
        size += x;
        x = 0;
        if ( size <= 0 )
        {
            return;
        }
    }
    
    if ( (x+size) >= (int)getWidth() ) size = getWidth()-x;
    
    PIX *ptr = pixPtr(x, y);
    
    memset(ptr, color, size * sizeof(PIX));

} // end Surface::drawHLine

// drawVLine
//---------------------------------------------------------------------------
// Purpose: Draws a vertical drawLine.
//---------------------------------------------------------------------------
void Surface::drawVLine(int x, int y, int size, const PIX color)
{
    assert(getDoesExist());
    assert(this != 0);

    // Check for trivial rejection
    if (   x < 0
        || size <= 0
        || x >= (int)getWidth()
        || y >= (int)getHeight()
       )
    {
        return;
    }

    if ( y < 0 )
    {
        size += y;
        y = 0;
        if ( size <= 0 )
        {
            return;
        }
    }

    if ( (y+size) >= (int)getHeight() ) size = getHeight()-y;
    
    PIX	*ptr	= pixPtr(x, y);

    while ( size > 0 )
    {
        *ptr	= color;
        ptr	+= getPitch();
        --size;
    }
} // end Surface::drawVLine

// fill
//---------------------------------------------------------------------------
// Purpose: Fills the Surface will the specified color.
//---------------------------------------------------------------------------
void Surface::fill(const PIX color)
{
    if ( !getWidth() || !getHeight() ) return;

    if ( getWidth() == getPitch() )
    {
        memset(mem, color, getWidth() * getHeight() * sizeof(PIX));
    }
    else
    {
        int n = getHeight();
        PIX* ptr = mem;
        do
        {
            memset(ptr, color, getWidth());
            ptr += getPitch();
        } while (--n > 0);
    }

} // end Surface::fill

// fillRect
//---------------------------------------------------------------------------
// Purpose: Fills the specified rectangle in the calling Surface with the
//          specified color.
//---------------------------------------------------------------------------
void Surface::fillRect(iRect bounds, const PIX color)
{
    assert(getDoesExist());

    if ( !getWidth() || !getHeight() ) return;

    // Check for trivial rejection
    if ( bounds.isEmpty() ) return;
    if (bounds.getLocationX() >= (int)getWidth()) return;
    if (bounds.getLocationY() >= (int)getHeight()) return;

    int x_len = bounds.getWidth();
    int y_len = bounds.getHeight();
    
    // clip
    if (bounds.getLocationX() <  0) { x_len += bounds.getLocationX(); bounds.setLocationX(0); }
    if (bounds.getLocationY() <  0) { y_len += bounds.getLocationY(); bounds.setLocationY(0); }
    if ( (bounds.getLocationX()+x_len) >= (int)getWidth() )  x_len = getWidth() - bounds.getLocationX();
    if ( (bounds.getLocationY()+y_len) > (int)getHeight() )  y_len = getHeight() - bounds.getLocationY();

    if ( ( x_len <= 0 ) || ( y_len <= 0 ) ) return;

    // Set memory to the top-left pixel of the rectangle.
    PIX	*ptr = pixPtr(bounds.getLocationX(), bounds.getLocationY());

    for (int y = 0; y < y_len; y++)
    {
        // Lay the horizontal strip.
        memset(ptr, color, x_len * sizeof(PIX));
        ptr += getPitch();
    }
} // end Surface::fillRect

// drawRect
//---------------------------------------------------------------------------
// Purpose: Draws a rectagle in the specified color on the calling Surface.
//---------------------------------------------------------------------------
void Surface::drawRect(iRect bounds, const PIX color)
{
    assert(getDoesExist());

    if ( !getWidth() || !getHeight() ) return;

    // Check for trivial rejection
    if ( bounds.isEmpty() ) return;
    if (bounds.getLocationX() >= (int)getWidth()) return;
    if (bounds.getLocationY() >= (int)getHeight()) return;

    int x_len = bounds.getWidth();
    int y_len = bounds.getHeight();
    
    // clip
    if (bounds.getLocationX() <  0) { x_len += bounds.getLocationX(); bounds.setLocationX(0); }
    if (bounds.getLocationY() <  0) { y_len += bounds.getLocationY(); bounds.setLocationY(0); }
    if ( (bounds.getLocationX()+x_len) >= (int)getWidth() )  x_len = getWidth() - bounds.getLocationX();
    if ( (bounds.getLocationY()+y_len) > (int)getHeight() )  y_len = getHeight() - bounds.getLocationY();

    if ( ( x_len <= 0 ) || ( y_len <= 0 ) ) return;

    drawHLine(bounds.getLocationX(), bounds.getLocationY(), x_len, color);
    drawHLine(bounds.getLocationX(), bounds.getEndY(),      x_len, color);

    if ( y_len > 2 )
    {
        drawVLine(bounds.getLocationX(), bounds.getLocationY()+1, y_len-2, color);
        drawVLine(bounds.getEndX(),      bounds.getLocationY()+1, y_len-2, color);
    }
} // end Surface::drawRect

// drawLine
//---------------------------------------------------------------------------
// Purpose: Draws a drawLine with any slope.
//---------------------------------------------------------------------------
void Surface::drawLine(int x1, int y1, int x2, int y2, const PIX color)
{
    assert(getDoesExist());
    assert(this != 0);

    // Check for horizontal lines
    if (y1 == y2) {
        orderCoords(x1, x2);
        drawHLine(x1, y1, x2-x1, color);
        return;
    }

    // Check for vertical lines
    if (x1 == x2) {
        orderCoords(y1, y2);
        drawVLine(x1, y1, y2-y1, color);
        return;
    }

    int	y_unit		= 1;
    int	x_unit		= 1;
    int	ydiff		= y2 - y1;
    int	xdiff		= x2 - x1;
    int	errorTerm	= 0;

    // These values are where a pixel is located in the drawLine
    int	xOffset	= x1;
    int	yOffset	= y1;

    // Finds out whether the drawLine moves up or down vertically
    if (ydiff < 0) {
        ydiff  = -ydiff;
        y_unit = -1;   // Move up the screen
    }

    // Finds out whether the drawLine moves left or right horizontally
    if (xdiff < 0) {
        xdiff  = -xdiff;
        x_unit = -1;
    }

    if (xdiff > ydiff) {
        int length = xdiff + 1;
        for (int i = 0; i < length;i++) {
            putPixel(xOffset, yOffset, color);  // Draw the pixel
            xOffset+=x_unit;
            errorTerm += ydiff;
            if (errorTerm > xdiff) {
                errorTerm -= xdiff;
                yOffset+=y_unit;
            }
        }
    }	else {
        int	length	= ydiff + 1;

        for (int i = 0; i < length; i++) {
        putPixel(xOffset, yOffset, color);  // Draw the pixel
            yOffset+=y_unit;
            errorTerm += xdiff;
            if (errorTerm > 0) {
                errorTerm -= ydiff;
                xOffset+=x_unit;
            }
        }
    }
} //end Surface::drawLine

// flipVertical
//---------------------------------------------------------------------------
// Purpose: Goes through all the frames of the surface and flips then
//          vertically.
//---------------------------------------------------------------------------
void Surface::flipVertical()
{
    assert(getDoesExist());

    PIX * newMem = (PIX*)malloc(getPitch() * getHeight());
    PIX * ptr = newMem;

    PIX * bottomPtr = mem + (getPitch() * (getHeight()-1));
    for ( unsigned y = 0; y < getHeight(); y++ )
    {
        memcpy(ptr, bottomPtr, getPitch());
        bottomPtr -= getPitch();
        ptr += getPitch();
    }

    ::free(mem);
    mem = newMem;
} // end Surface::flipVertical

// copy
//---------------------------------------------------------------------------
// Purpose: Copies the specified number of frames from the source Surface to
//          the calling Surface.
//---------------------------------------------------------------------------
void Surface::copy(const Surface &source)
{
    if(!source.getDoesExist())
        return;

    create(source.getWidth(), source.getHeight());
    memcpy(mem, source.mem, getHeight()*getPitch());

} // end Surface::copy

void Surface::bltLookup(const iRect &destRect, const PIX table[])
{
    int source_lines = destRect.getHeight();
    int source_cols = destRect.getWidth();
    int source_start_y = destRect.getLocationY();
    int source_start_x = destRect.getLocationX();

    if ( (source_start_y >= (int)getHeight()) || (source_start_x >= (int)getWidth()) )
    {
        return;
    }

    if ( source_start_y < 0 )
    {
        if ( -source_start_y >= source_lines )
        {
            return;
        }

        source_lines += source_start_y;
        source_start_y = 0;
    }

    if ( source_start_x < 0 )
    {
        if ( -source_start_x >= source_cols )
        {
            return;
        }
        source_cols += source_start_x;
        source_start_x = 0;
    }

    unsigned dest_lines = std::min(getHeight() - source_start_y, (unsigned)source_lines);
    unsigned dest_cols = std::min(getWidth() - source_start_x, (unsigned)source_cols);

    int srcAdjustment  = getPitch()      - dest_cols;

    PIX	*sPtr = pixPtr( source_start_x, source_start_y);  // Pointer to source Surface start of memory.

    for (unsigned row = 0; row < dest_lines; row++)
    {
        for (unsigned col = 0; col < dest_cols; col++)
        {
            *sPtr = table[*sPtr];
            sPtr++;
        }

        sPtr += srcAdjustment;
    }

}

//---------------------------------------------------------------------------
void Surface::bltScale(const Surface &source, const iRect &destRect)
{
    assert(getDoesExist());
    assert(source.getDoesExist());
    assert(this != 0);

    iXY min = destRect.getLocation();
    iXY max = min + destRect.getSize(); // XXX check should have -1 here

    if (min.x >= (int)getWidth()) return;
    if (min.y >= (int)getHeight()) return;

    // Something is overlapping, so we need to verify that both
    // surfaces are valid.
    assert(mem        != 0);
    assert(source.mem != 0);

    size_t pixelsPerRow = max.x-min.x;
    size_t numRows      = max.y-min.y;

    int srcX1 = 0;
    int srcY = 0;

    // FIXME - if inverted bitmap, then flip x1 and y to right or botom edges

    // Compute source deltas
    int xSrcDelta = int((float(source.getWidth()) / float(max.x - min.x)) * 65536.0);
    int ySrcDelta = int((float(source.getHeight()) / float(max.y - min.y)) * 65536.0);

    PIX  *dRow = mem;			// Pointer to the destination Surface

    // CLIP LEFT
    if (min.x < 0) {
        pixelsPerRow += min.x;
        srcX1 -= min.x*xSrcDelta;
    }	else {
        dRow += min.x;
    }

    // CLIP RIGHT
    if (max.x > (int)getWidth()) {
        pixelsPerRow -= max.x - getWidth();
    }

    // CLIP TOP
    if (min.y < 0) {
        numRows += min.y;
        srcY   -= min.y*ySrcDelta;
    }	else {
        dRow += (min.y*(int)getPitch());
    }

    // CLIP BOTTOM
    if (max.y > (int)getHeight()) {
        numRows -= max.y-getHeight();
    }

    // Check to make sure the clipping is working.
    if (pixelsPerRow <= 0) return;
    if (numRows <= 0) return;

    float xdelta = float(source.getWidth()) / float(max.x - min.x);
    for (size_t yCount = 0 ; yCount < numRows ; yCount++)
    {
        const PIX *sRow = source.pixPtr(0, srcY >> 16) + (srcX1 >> 16);

        // XXX: WARNING SLOW CODE
        float sPos = 0;
        for(size_t x=0; x<pixelsPerRow; x++) {
            dRow[x] = sRow[(size_t) sPos];
            sPos += xdelta;
        }

        srcY += ySrcDelta;
        dRow += getPitch();
    }
}

static inline float getRand(float lo, float hi)
{
    return (float(rand()%10000)/10000.0*(hi-lo))+lo;
}

static inline float calcY(float average, float ruggedness, unsigned distance)
{
    return average+getRand(-ruggedness, ruggedness)*float(distance);
}

// getAverageColor
//---------------------------------------------------------------------------
// Purpose: Recalculates the best single color to represent this Surface.
//---------------------------------------------------------------------------
PIX Surface::getAverageColor()
{
    int avgR = 0;
    int avgG = 0;
    int avgB = 0;

    // Go through a single cTile and get all the additive color values.
    for (unsigned int x = 0; x < getWidth(); x++) {
        for (unsigned int y = 0; y < getHeight(); y++) {
            avgR += Palette::color[getPixel(x, y)].r;
            avgG += Palette::color[getPixel(x, y)].g;
            avgB += Palette::color[getPixel(x, y)].b;
        }
    }

    // Divide each individual amount by the number of bytes in the image.
    int numPix = getWidth() * getHeight();

    avgR /= numPix;
    avgG /= numPix;
    avgB /= numPix;

    return Palette::findNearestColor(avgR, avgG, avgB);
} // end Surface::getAverageColor

void Surface::bltRectAlphaColor(Surface& dest, int x, int y, const iRect& rect, const PIX color) const
{
    int source_lines = std::min<int>(rect.getHeight(), getHeight()-rect.getLocationY());
    int source_cols = std::min<int>(rect.getWidth(), getWidth()-rect.getLocationX());
    int source_start_x = rect.getLocationX();
    int source_start_y = rect.getLocationY();

    if ( (y >= (int)dest.getHeight()) || (x >= (int)dest.getWidth()) )
    {
        return;
    }

    if ( y < 0 )
    {
        if ( -y >= source_lines )
        {
            return;
        }

        source_lines += y;
        source_start_y -= y;
        y = 0;
    }

    if ( x < 0 )
    {
        if ( -x >= source_cols )
        {
            return;
        }
        source_cols += x;
        source_start_x -= x;
        x = 0;
    }

    unsigned dest_lines = std::min(dest.getHeight() - y, (unsigned)source_lines);
    unsigned dest_cols = std::min(dest.getWidth() - x, (unsigned)source_cols);

    int srcAdjustment  = getPitch()      - dest_cols;
    int destAdjustment = dest.getPitch() - dest_cols;

    PIX	*sPtr = pixPtr( source_start_x, source_start_y);  // Pointer to source Surface start of memory.
    PIX	*dPtr = dest.pixPtr( x, y); // Pointer to destination Surface start of memory.
    
    uint8_t * cbase = &Palette::fullAlphaTable()[color*(16*256)];

    for (unsigned row = 0; row < dest_lines; row++)
    {
        for (unsigned col = 0; col < dest_cols; col++)
        {
            *dPtr = cbase[(*sPtr * 256) + *dPtr];
            sPtr++;
            dPtr++;
        }

        sPtr += srcAdjustment;
        dPtr += destAdjustment;
    }
}

unsigned int
Surface::getFontHeight()
{
    return 16;
//    return FONT_HEIGHT;
}

int Surface::getTextLength(const char* text)
{
    return FONT_WIDTH * strlen(text);
}

// bltChar8x8
//---------------------------------------------------------------------------
// Purpose: Blits the specied rom character to the screen at the specified
//          location.
//---------------------------------------------------------------------------
void Surface::bltChar8x8(int x, int y, unsigned char character, const PIX color)
{
    if (character > 31) character-=32;
    if (character >= FONT_MAXCHAR)
        return;
//    ascii8x8[character]->bltTransColor(*this, x, y, color);
} // end Surface::bltChar8x8

// bltString
//---------------------------------------------------------------------------
// Purpose: Blits the specified string of text to the screen by making
//          calls to blitChar for each character of the string. Does not
//          handle wrapping.
//---------------------------------------------------------------------------

void Surface::bltString(int x, int y, const char * str, const PIX color)
{
} // end Surface::bltString

// bltStringLen
//---------------------------------------------------------------------------
// Purpose: Blits the specified string of text to the screen by making
//          calls to blitChar for each character of the string. Does not
//          handle wrapping.
//---------------------------------------------------------------------------
void Surface::bltStringLen(int x, int y, const char * str, int len, const PIX color)
{
    for (int index = 0; (index < len) && str[index] != 0; index++) {
        // Don't attempt blank spaces.
        if (str[index] == 32) {
            continue;
        }

        bltChar8x8(x + (index << 3), y, str[index], color);
    }
} // end Surface::bltStringLen

// bltStringShadowed
//---------------------------------------------------------------------------
void Surface::bltStringShadowed(int x, int y, char const *str, const PIX textColor, const PIX shadowColor)
{
    for (int index = 0; str[index] != 0; index++) {
        bltChar8x8(x + (index << 3) + 1, y + 1, str[index], shadowColor);
        bltChar8x8(x + (index << 3),     y,     str[index], textColor);
    }

} // end Surface::bltStringShadowed

// bltStringCenteredInRect
//---------------------------------------------------------------------------
// Purpose: Blits the string centered inside the specified rectangle.
//---------------------------------------------------------------------------
void Surface::bltStringCenteredInRect(const iRect &rect, const char *string, const PIX color)
{
    int length = strlen(string);

    iXY destPos;
    destPos.x = rect.getLocationX() + (rect.getWidth() - (length * FONT_WIDTH)) / 2;
    destPos.y = rect.getLocationY() + (rect.getHeight() - getFontHeight()) / 2;

    for (int i = 0; string[i] != 0; i++) {
        bltChar8x8(destPos.x + (i << 3), destPos.y, string[i], color);
    }

} // end Surface::bltStringCenteredInRect

// create
//---------------------------------------------------------------------------
void
Surface::create(unsigned int w, unsigned int h)
{
    free();

    size_t requestedBytes = w * h;

    if (requestedBytes > 0)
    {
        mem = (PIX *) malloc(requestedBytes);

        if ( ! mem )
            throw Exception("out of memory while allocating surface.");
        
        totalByteCount += requestedBytes;
    }

    twidth = w;
    theight= h;
    tpitch = w;
} // end Surface::create

void Surface::loadBMP(const char *fileName, bool needAlloc)
{
    assert(this != 0);

    if (needAlloc) free();

    filesystem::ReadFile file(fileName);
    if ( file.isOpen() )
    {
        BitmapFileHeader file_header(file);

        if ( file_header.bfType != 0x4d42 ) // file_header.bfType != "BM"
            throw Exception("%s is not a valid 8-bit BMP file", fileName);

        BitmapInfoHeader info_header(file);

        if ( info_header.biBitCount != 8 )
            throw Exception("%s is not a 8-bit BMP file", fileName);

        if ( info_header.biCompression != BI_RGB )
            throw Exception("%s is not a 8-bit UnCompressed BMP file", fileName);


        if (needAlloc)
        {
            LOGGER.warning("Loading '%s' size %dx%d", fileName,info_header.biWidth, info_header.biHeight );
            create(info_header.biWidth, info_header.biHeight);

        } else {
            LOGGER.warning("Loading '%s' size %dx%d preallocated: %dx%d", fileName,info_header.biWidth, info_header.biHeight, getWidth(), getHeight() );
            // Check and make sure the picture will fit
            if (getWidth() < (unsigned long) info_header.biWidth|| getHeight() < (unsigned long) info_header.biHeight )
                throw Exception("Not enough memory to load BMP image %s", fileName);
        }

        file.seek(file_header.bfOffBits);

        if ( (info_header.biWidth % 4) == 0 ) {
            file.read(mem, getWidth() * getHeight(), 1);
        } else {
            int padding = ((info_header.biWidth / 4 + 1) * 4) - info_header.biWidth;

            PIX buffer[10];
            int numRows = getHeight();

            PIX *ptr = mem;

            for (int row = 0; row < numRows; row++)
            {
                file.read(ptr, getWidth(), 1);
                file.read(buffer, padding, 1);
                ptr += getPitch();
            }
        }

        flipVertical();
    }
    else
    {
        throw Exception("Cannot open bmp file '%s'", fileName);
    }
}

// @todo check for returned errors!!!
void Surface::loadPNG(const char* fileName)
{
    filesystem::ReadFile file(fileName);
    if ( file.isOpen() )
    {
        const int flen = file.fileLength();
        if ( flen )
        {
            uint8_t buf[flen];
            memset(buf, 0, flen);
            
            file.read(buf, flen, 1);
            
            png_image pim = {0};
            pim.version = PNG_IMAGE_VERSION;
            png_image_begin_read_from_memory(&pim, buf, flen);
            
            create(pim.width, pim.height);
            if ( mem )
            {
                pim.format |= PNG_FORMAT_FLAG_COLORMAP;
                
                RGBColor p[256];
                memcpy(&p, &Palette::color, sizeof(Palette::color));
                
                // libpng modifies the palette, so make a copy
                png_image_finish_read(&pim, 0, mem, 0, &p);
//                png_image_finish_read(&pim, 0, mem, 0, &Palette::color);
            }
        }
    }
}

void Surface::savePNG(const char* fileName)
{
    std::string fn = filesystem::getRealWriteName(fileName);
    
    png_image pim = {0};
    pim.version = PNG_IMAGE_VERSION;
    
    pim.width = getWidth();
    pim.height = getHeight();
    pim.format = PNG_FORMAT_FLAG_COLOR | PNG_FORMAT_FLAG_COLORMAP | PNG_FORMAT_FLAG_ALPHA;
    pim.colormap_entries = 256;
    
    png_image_write_to_file(&pim, fn.c_str(), false, mem, 0, &Palette::color);
    if ( pim.warning_or_error )
    {
        LOGGER.warning("msg from libpng: '%s'", pim.message);
    }
    
}

// drawBoxCorners
//--------------------------------------------------------------------------
// Purpose: Draws lines in the corners of the surface of the specified length
//          and color.
//--------------------------------------------------------------------------
void Surface::drawBoxCorners(const iRect &rect, unsigned cornerLength, PIX color)
{
    cornerLength = std::min(rect.getWidth(), std::min(rect.getHeight(), cornerLength));
    
    // Draw the rectangle edge-only selection box.
    // Top-left
    drawHLine(rect.getLocationX(), rect.getLocationY(),     cornerLength, color);
    drawVLine(rect.getLocationX(), rect.getLocationY() + 1, cornerLength - 1, color);

    // Top-right
    drawHLine(rect.getEndX() - cornerLength + 1, rect.getLocationY(),     cornerLength,     color);
    drawVLine(rect.getEndX(),                    rect.getLocationY() + 1, cornerLength - 1, color);

    // Bottom-right
    drawHLine(rect.getEndX() - cornerLength + 1, rect.getEndY(),                    cornerLength,     color);
    drawVLine(rect.getEndX(),                    rect.getEndY() - cornerLength + 1, cornerLength - 1, color);

    // Bottom-left
    drawHLine(rect.getLocationX(), rect.getEndY(),                    cornerLength,     color);
    drawVLine(rect.getLocationX(), rect.getEndY() - cornerLength + 1, cornerLength - 1, color);

} // end Surface::drawBoxCorners

void Surface::circle(int cx, int cy, int radius, PIX color)
{
  int d, y, x;

  d = 3 - (2 * radius);
  x = 0;
  y = radius;

  while (y >= x) {
    putPixel(cx + x, cy + y, color);// down right corner
    putPixel(cx + y, cy + x, color);// down right corner
    putPixel(cx - x, cy + y, color);// up left corner
    putPixel(cx - y, cy + x, color);// up left corner
    putPixel(cx + x, cy - y, color);// up right corner
    putPixel(cx + y, cy - x, color);// up right corner
    putPixel(cx - x, cy - y, color);// down left corner
    putPixel(cx - y, cy - x, color);// down left corner

    if (d < 0)
      d = d + (4 * x) + 6;
    else {
      d = d + 4 * (x - y) + 10;
      y--;
    }
    x++;
  }
}

void Surface::FillCircle(int cx, int cy, int radius, PIX color)
{
  int d, y, x;

  d = 3 - (2 * radius);
  x = 0;
  y = radius;

  while (y >= x) {
    drawHLine(cx - x, cy - y, x , color);
    drawHLine(cx - x, cy + y, x, color);
    drawHLine(cx - y, cy - x, y, color);
    drawHLine(cx - y, cy + x, y, color);

    if (d < 0)
      d = d + (4 * x) + 6;
    else {
      d = d + 4 * (x - y) + 10;
      y--;
    }
    x++;
  }
}

void Surface::BltCircle(int cx, int cy, int radius, const PIX table[])
{
  int d, y, x;

  d = 3 - (2 * radius);
  x = 0;
  y = radius;

//  while (y >= x) {
//    bltHLine(cx - x, cy - y,cx+ (x + 1), table);
//    bltHLine(cx - x, cy + y,cx+ (x + 1), table);
//    bltHLine(cx - y, cy - x,cx+ (y + 1), table);
//    bltHLine(cx - y, cy + x,cx+ (y + 1), table);
//
//    if (d < 0)
//      d = d + (4 * x) + 6;
//    else {
//      d = d + 4 * (x - y) + 10;
//      y--;
//    }
//    x++;
//  }
}


void Surface::RoundRect(iRect rect, int radius, PIX color)
{
    int d, y, x;

    d = 3 - (2 * radius);
    x = 0;
    y = radius;

    if ( !getWidth() || !getHeight() ) return;

    // Check for trivial rejection
    if ( rect.isEmpty() ) return;
    else if (rect.getLocationX() >= (int)getWidth()) return;
    else if (rect.getLocationY() >= (int)getHeight()) return;
    
    // Check for clipping
    if (rect.getLocationX() <  0)  rect.setLocationX(0);
    if (rect.getLocationY() <  0)  rect.setLocationY(0);
//    if (rect.max.x >= (int)getWidth())  rect.max.x = getWidth() - 1;
//    if (rect.max.y >= (int)getHeight()) rect.max.y = getHeight() - 1;

    drawHLine(rect.getLocationX()+radius, rect.getLocationY(),        rect.getWidth()  - (radius*2), color);
    drawHLine(rect.getLocationX()+radius, rect.getEndY(),             rect.getWidth()  - (radius*2), color);
    drawVLine(rect.getLocationX(),        rect.getLocationY()+radius, rect.getHeight() - (radius*2), color);
    drawVLine(rect.getEndX(),             rect.getLocationY()+radius, rect.getHeight() - (radius*2), color);

    while (y >= x)
    {
        putPixel( (rect.getLocationX()+radius) - x, (rect.getLocationY()+radius) - y, color);// up left corner
        putPixel( (rect.getLocationX()+radius) - y, (rect.getLocationY()+radius) - x, color);// up left corner

        putPixel( rect.getEndX() - radius + x, (rect.getLocationY()+radius) - y, color);// up right corner
        putPixel( rect.getEndX() - radius + y, (rect.getLocationY()+radius) - x, color);// up right corner

        putPixel( (rect.getLocationX()+radius) - x, rect.getEndY() - radius + y, color);// down left corner
        putPixel( (rect.getLocationX()+radius) - y, rect.getEndY() - radius + x, color);// down left corner

        putPixel( rect.getEndX() - radius + x, rect.getEndY() - radius + y, color);// down right corner
        putPixel( rect.getEndX() - radius + y, rect.getEndY() - radius + x, color);// down right corner

        if (d < 0)
            d = d + (4 * x) + 6;
        else
        {
            d = d + 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

void Surface::FillRoundRect(iRect rect, int radius, PIX color)
{
    if ( !getWidth() || !getHeight() ) return;

    // Check for trivial rejection
    if      (rect.isEmpty())     return;
    else if (rect.getLocationX() >= (int)getWidth()) return;
    else if (rect.getLocationY() >= (int)getHeight()) return;

    int d, y, x;

    d = 3 - (2 * radius);
    x = 0;
    y = radius;

    // Check for clipping
    if (rect.getLocationX() <  0)  rect.setLocationX(0);
    if (rect.getLocationY() <  0)  rect.setLocationY(0);
//    if (rect.max.x >= (int)getWidth())  rect.max.x = getWidth() - 1;
//    if (rect.max.y >= (int)getHeight()) rect.max.y = getHeight() - 1;

    fillRect(iRect( rect.getLocationX(), rect.getLocationY()+radius+1,
                    rect.getWidth(), rect.getHeight() - (radius*2) - 2), color);

    while (y >= x)
    {
        drawHLine((rect.getLocationX()+radius) - y, (rect.getLocationY()+radius) - x, rect.getWidth() - (radius-y) + y - radius, color);//up
        drawHLine((rect.getLocationX()+radius) - y, (rect.getEndY()-radius) + x,      rect.getWidth() - (radius-y) + y - radius, color);//down

        if (d < 0)
        {
            d = d + (4 * x) + 6;
        }
        else
        {
            d = d + 4 * (x - y) + 10;
            if ( y > x )
            {
                drawHLine((rect.getLocationX()+radius) - x, (rect.getLocationY()+radius) - y, rect.getWidth() - (radius-x) + x - radius, color);//up
                drawHLine((rect.getLocationX()+radius) - x, (rect.getEndY()-radius) + y,      rect.getWidth() - (radius-x) + x - radius, color);//down
            }
            y--;
        }

        x++;
    }
}

void Surface::BltRoundRect(iRect rect, int radius, const PIX table[])
{
    if ( !getWidth() || !getHeight() ) return;

    // Check for trivial rejection
    if      (rect.isEmpty())     return;
    else if (rect.getLocationX() >= (int)getWidth()) return;
    else if (rect.getLocationY() >= (int)getHeight()) return;

    int d, y, x;

    d = 3 - (2 * radius);
    x = 0;
    y = radius;

    // Check for clipping
    if (rect.getLocationX() <  0)  rect.setLocationX(0);
    if (rect.getLocationY() <  0)  rect.setLocationY(0);
//    if (rect.max.x >= (int)getWidth())  rect.max.x = getWidth() - 1;
//    if (rect.max.y >= (int)getHeight()) rect.max.y = getHeight() - 1;

    bltLookup(iRect( rect.getLocationX(), rect.getLocationY()+radius+1,
                     rect.getWidth(), rect.getHeight() - (radius*2) - 2), table);


    while (y >= x)
    {
        bltHLine((rect.getLocationX()+radius) - y, (rect.getLocationY()+radius) - x, rect.getWidth() - (radius-y) + y - radius, table);//up
        bltHLine((rect.getLocationX()+radius) - y, (rect.getEndY()-radius) + x,      rect.getWidth() - (radius-y) + y - radius, table);//down

        if (d < 0)
        {
            d = d + (4 * x) + 6;
        }
        else
        {
            d = d + 4 * (x - y) + 10;
            if ( y > x )
            {
                bltHLine((rect.getLocationX()+radius) - x, (rect.getLocationY()+radius) - y, rect.getWidth() - (radius-x) + x - radius, table);//up
                bltHLine((rect.getLocationX()+radius) - x, (rect.getEndY()-radius) + y,      rect.getWidth() - (radius-x) + x - radius, table);//down
            }
            y--;
        }

        x++;
    }
}

void Surface::bltHLine(int x, int y, int size, const PIX table[])
{
    assert(getDoesExist());
    assert(this != 0);

    // Check for trivial rejection
    if (    y < 0
         || y >= (int)getHeight()
         || size <= 0
       )
    {
         return;
    }

    if ( x < 0 )
    {
        size -= x;
        x = 0;
        if ( size <= 0 )
        {
            return;
        }
    }
    
    if ( (x+size) > (int)getWidth() ) size = getWidth()-x;
    
    PIX *ptr = mem + (x * sizeof(PIX)) + y * (int)getPitch();
    
    for(int r=0; r<size; r++)
        ptr[r] = table[ptr[r]];
} // end Surface::drawHLine

