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
#include <config.h>

#include <math.h>
#include <memory>
#include <vector>
#include <string>
#include <algorithm>
#include "Log.hpp"
#include "Surface.hpp"
#include "FileSystem.hpp"
#include "UtilInterface.hpp"
#include "cGrowList.hpp"
#include "fXY.hpp"
#include "TimerInterface.hpp"
#include "Span.hpp"
#include "Palette.hpp"
#include "Exception.hpp"

using std::swap;
using std::min;
using std::max;

// orderCoords
//---------------------------------------------------------------------------
// Purpose: Orders a pair of (x,y) coordinates
//---------------------------------------------------------------------------
template <class T>
inline void orderCoords(T &a, T &b)
{
    if (a > b) swap(a, b);
} // end orderCoords

// orderCoords
//---------------------------------------------------------------------------
// Purpose: Orders a 2 pairs of (x,y) coordinates, making sure x1 <= x2 and y1 <= y2.
//---------------------------------------------------------------------------
template <class T>
inline void orderCoords(T &x1, T &y1, T &x2, T &y2)
{
    if (x1 > x2) swap(x1, x2);
    if (y1 > y2) swap(y1, y2);
} // end orderCoords

inline void orderCoords(iRect &bounds)
{
    if (bounds.min.x > bounds.max.x) {
        swap(bounds.min.x, bounds.max.x);
    }
    if (bounds.min.y > bounds.max.y) {
        swap(bounds.min.y, bounds.max.y);
    }
} // end orderCoords

#ifdef MSVC
#pragma pack(1)
#endif

struct PcxHeader
{
    uint8_t   manufacturer;
    uint8_t   version;
    uint8_t   encoding;
    uint8_t   bits_per_pixel;
    uint16_t   x,y;                    //upper left of image
    uint16_t   width, height;          //size of the image
    uint16_t   horz_res;               //horizontal resolution
    uint16_t   vert_res;               //vertical resolution
    uint8_t   ega_palette[48];        //who cares?
    uint8_t   reserved;
    uint8_t   num_color_planes;
    uint16_t   bytes_per_line;
    uint16_t   palette_type;
    uint8_t   padding[58];
}
__attribute__((packed));

class SurfaceHeader
{
public:
    uint32_t pixX;
    uint32_t pixY;
    uint32_t offsetX;
    uint32_t offsetY;
    uint32_t frameCount;
    float fps;

}
__attribute__((packed)); // end SurfaceHeader

struct PIC_HEAD
{
    uint32_t xPix;         // Horizontal pixel count.
    uint32_t yPix;         // Vertical pixel count.
    uint32_t frameCount;    // Number of frames.
}
__attribute__((packed));

class BitmapFileHeader
{
public:
    uint16_t    bfType;
    uint32_t   bfSize;
    uint16_t    bfReserved1;
    uint16_t    bfReserved2;
    uint32_t   bfOffBits;
}
__attribute__((packed));

#define BI_RGB      0L
#define BI_RLE8     1L
#define BI_RLE4     2L

class BitmapInfoHeader
{
public:
    uint32_t  biSize;
    uint32_t  biWidth;
    uint32_t  biHeight;
    uint16_t   biPlanes;
    uint16_t   biBitCount;
    uint32_t  biCompression;
    uint32_t  biSizeImage;
    uint32_t  biXPelsPerMeter;
    uint32_t  biYPelsPerMeter;
    uint32_t  biClrUsed;
    uint32_t  biClrImportant;
}
__attribute__((packed));

class RGBQuad
{
public:
    uint8_t    rgbBlue;
    uint8_t    rgbGreen;
    uint8_t    rgbRed;
    uint8_t    rgbReserved;
}
__attribute__((packed));

#ifdef MSVC
#pragma pack()
#endif

Surface ascii8x8;
Surface ascii5x5;

int Surface::totalSurfaceCount = 0;
int Surface::totalByteCount    = 0;

// Surface
//---------------------------------------------------------------------------
Surface::Surface()
{
    assert(this != 0);

    reset();

    totalSurfaceCount++;
    totalByteCount += sizeof(Surface);
} // end Surface::Surface

// Surface
//---------------------------------------------------------------------------
Surface::Surface(const iXY &nPix, int nStride, int nFrameCount)
{
    assert(this != 0);

    reset();
    alloc(nPix, true, nStride, nFrameCount);
    assert(frame0 != 0);

    totalSurfaceCount++;
    totalByteCount += sizeof(Surface);

} // end Surface::Surface

// Surface
//---------------------------------------------------------------------------
Surface::Surface(int xPix, int yPix, int nStride, int nFrameCount)
{
    assert(this != 0);
    reset();

    create(iXY(xPix, yPix), nStride, nFrameCount);

    totalSurfaceCount++;
    totalByteCount += sizeof(Surface);

} // end Surface::Surface

// Surface
//---------------------------------------------------------------------------
Surface::Surface(const Surface &source, const iXY &min, const iXY &max, bool doGrab)
{
    assert(this != 0);
    assert(source.getDoesExist());

    reset();

    if (source.getDoesExist()) doesExist = true;

    if (doGrab) {
        grab(source, iRect(min.x, min.y, max.x, max.y), true, 0);
    }	else {
        setTo(source, iRect(min.x, min.y, max.x, max.y));
    }

    totalSurfaceCount++;
    totalByteCount += sizeof(Surface);

} // end Surface::Surface

// Surface
//---------------------------------------------------------------------------
Surface::Surface(void *nFrame0, const iXY &nPix, int nStride, int nFrameCount)
{
    assert(nFrame0 != 0);
    assert(this != 0);

    reset();
    setTo(nFrame0, nPix, nStride, nFrameCount);

    totalSurfaceCount++;
    totalByteCount += sizeof(Surface);

} // end Surface::Surface

// ~Surface
//---------------------------------------------------------------------------
Surface::~Surface()
{
    if ((doesExist != false) && (myMem != false)) {
        free();
    }

    totalSurfaceCount--;
    totalByteCount -= sizeof(Surface);

    assert(totalByteCount >= 0);
} // end Surface::~Surface

//---------------------------------------------------------------------------
void Surface::free()
{
    if (myMem && frame0 != 0) {
        ::free(frame0);

        totalByteCount -= stride * pix.y * sizeof(PIX) * frameCount;

        assert(totalByteCount >= 0);
    }

    frame0     = 0;
    mem        = 0;
    myMem      = false;
    doesExist  = false;
    frameCount = 0;
}

// reset
//---------------------------------------------------------------------------
void Surface::reset()
{
    assert(this != 0);

    pix.zero();
    stride      = 0;
    center.zero();
    mem         = 0;
    frame0      = 0;
    myMem       = false;
    frameCount  = 0;
    curFrame    = 0;
    fps         = 0;
    offset.zero();
    doesExist   = 0;
    wipeCount   = 0;
} // end Surface::reset

// setOffsetCenter
//---------------------------------------------------------------------------
// Purpose: Set the offset to the center of the image.
//---------------------------------------------------------------------------
void Surface::setOffsetCenter()
{
    assert(getDoesExist());
    assert(this != 0);

    offset = iXY(-center.x, -center.y);

} // end Surface::setOffsetCenter

// alloc
//---------------------------------------------------------------------------
// Purpose: Allocates memory for the surface, while setting the pix.x, pix.y,
//          xCenter, yCenter, and stride.  If successful, true is returned,
//          otherwise false is returned.
//---------------------------------------------------------------------------
bool Surface::alloc(const iXY  &pix,
                    bool gottaHaveIt,
                    int         stride,
                    int         frameCount)
{
    assert(this != 0);

    free();

    Surface::stride = stride ? stride : pix.x;

    uint32_t requestedBytes = uint32_t(Surface::stride) * uint32_t(pix.y) * sizeof(PIX) * uint32_t(frameCount);

    if (requestedBytes > 0) {
        frame0 = (PIX *) malloc(requestedBytes);

        // Add the number of bytes for this surface.
        totalByteCount += requestedBytes;

        if (frame0 == 0) {
            if (gottaHaveIt) {
                throw Exception("out of memory while allocating surface.");
            }

            // Subtract the number of bytes for this surface.
            totalByteCount -= requestedBytes;

            return false;
        }
        myMem = true;
    }

    Surface::mem        = frame0;
    Surface::pix        = pix;
    Surface::stride     = stride;
    Surface::center     = iXY(pix.x >> 1, pix.y >> 1);
    Surface::frameCount = frameCount;
    Surface::doesExist  = true;

    return true;

} // end Surface::alloc

// createNoAlloc
//---------------------------------------------------------------------------
void Surface::createNoAlloc(const iXY &nPix)
{
    free();
    reset();
    Surface::pix = nPix;
    stride       = nPix.x;
    center       = iXY(nPix.x >> 1, nPix.y >> 1);
    frameCount   = 1;
    doesExist    = true;

} // end Surface::createNoAlloc

// resize
//---------------------------------------------------------------------------
// Purpose: Resizes the calling surface to a new length and width, while
//          remaining the same number of frames.
//---------------------------------------------------------------------------
void Surface::resize(const iXY &pix)
{
    assert(getDoesExist());
    assert(this != 0);

    create(pix, pix.x, frameCount);

} // end Surface::resize

// setTo
//---------------------------------------------------------------------------
// Purpose: Maps a Surface's coordinates to an existing surface.  This can
//          save you from having to allocate memory for every single surface.
//          You can just draw everything onto one surface in the given bounds.
//---------------------------------------------------------------------------
void Surface::setTo(const Surface &source, iRect bounds)
{
    assert(source.getDoesExist());
    assert(this != 0);

    free();
    orderCoords(bounds);

    pix        = bounds.getSize();
    stride     = source.stride;
    frame0     = source.pixPtr(bounds.min);
    frameCount = source.getFrameCount();
    mem	       = frame0;
    myMem      = false;
    center     = iXY(source.getPix().x >> 1, source.getPix().y >> 1);
    fps        = source.getFPS();
    doesExist  = source.getDoesExist();

} // end Surface::setTo

// setTo
//---------------------------------------------------------------------------
// Purpose: Maps the calling surface to some specified coordinates of the
//          another Surface.
//---------------------------------------------------------------------------
void Surface::setTo(void *frame0,
                    const iXY &pix,
                    int stride,
                    int frameCount)
{
    assert(frame0 != 0);
    assert(this != 0);

    free();

    Surface::frame0     = (PIX *) frame0;
    mem                 = Surface::frame0;
    Surface::pix        = pix;
    center              = iXY(pix.x >> 1, pix.y >> 1);
    Surface::stride     = stride ? stride : pix.x;
    myMem               = false;
    Surface::frameCount = frameCount;
    fps                 = 0;
    doesExist           = true;

} // end Surface::setTo

// setTo
//---------------------------------------------------------------------------
// Purpose: Maps the calling surface to some specified coordinates of the
//          another Surface.
//---------------------------------------------------------------------------
void Surface::setTo(const Surface &source)
{
    assert(this != 0);
    assert(source.getDoesExist());

    free();

    myMem      = false;
    frame0     = source.getFrame0();
    mem        = frame0;
    pix        = source.getPix();
    center     = source.getCenter();
    stride     = source.getStride();
    frameCount = source.getFrameCount();
    fps        = source.getFPS();
    offset     = source.getOffset();
    doesExist  = source.getDoesExist();

} // end Surface::setTo

// grab
//---------------------------------------------------------------------------
// Purpose: Copies a section from another Surface.
//---------------------------------------------------------------------------
bool Surface::grab(const Surface &source,
                   iRect bounds,
                   bool  gottaHaveIt,
                   int   stride)
{
    assert(source.getDoesExist());
    assert(this != 0);

    free();
    orderCoords(bounds);

    if (!alloc(bounds.getSize(), gottaHaveIt, stride, 1)) {
        return false;
    }

    // We can blit like this because everything will be clipped away for us.
    source.blt(*this, -bounds.min.x, -bounds.min.y);

    return true;

} // end Surface::grab

// blt
//---------------------------------------------------------------------------
// Purpose: Puts the surface onto the destination while performing clipping
//          on the bounds of the object.
//---------------------------------------------------------------------------
void Surface::blt(const Surface &dest, iXY min) const
{
    assert(getDoesExist());
    assert(dest.getDoesExist());
    assert(this != 0);
    assert(mem != 0);
    assert(dest.mem != 0);    

    // Add in the offset factor.
    min += offset;

    // Trivial clipping rejection - no overlap.
    // Also will jump out immediately if either image has zero size.
    if (min.x >= dest.pix.x) return;
    if (min.y >= dest.pix.y) return;

    iXY max;
    max = min + pix;
    if (max.x < 0) return;
    if (max.y < 0) return;

    int	pixelsPerRow = pix.x;
    int	numRows      = pix.y;

    PIX	*sPtr	= mem;      // Pointer to source Surface start of memory.
    PIX	*dPtr	= dest.mem; // Pointer to destination Surface start of memory.

    // Check for partial clip, calculate number of pixels
    // per row to copy, and number of rows to copy.  Adjust
    // sPtr and dPtr.

    // CLIP LEFT
    if (min.x < 0) {
        pixelsPerRow +=  min.x; // This will subtract the neg. x value.
        sPtr         += -min.x; // This will move the sPtr to x = 0, from the neg. x.
    }	else {
        dPtr += min.x;
    }

    // CLIP RIGHT
    // This subtracts only the portion hanging over the right edge of the
    // destination Surface
    if (max.x > dest.pix.x) pixelsPerRow -= max.x - dest.pix.x;

    // CLIP TOP
    if (min.y < 0) {
        numRows += min.y;
        sPtr    -= min.y * stride;
    }	else {
        dPtr += min.y * dest.stride;
    }

    // CLIP BOTTOM
    // This subtracts only the portion hanging over the bottom edge of the
    // destination Surface
    if (max.y > dest.pix.y) numRows -= max.y - dest.pix.y;

    // Now, Check to make sure I actually have something
    // to draw.  I should - because I checked for trivial
    // rejection first.  These asserts just make sure
    // my clipping is working...
    assert(pixelsPerRow > 0);
    assert(numRows > 0);

    // Now blt the sucker!  But first, see if we can do it in one
    // big blt, without doing each row individually...
    if (stride == pixelsPerRow && dest.stride == pixelsPerRow) {
        memcpy(dPtr, sPtr, pixelsPerRow * numRows * sizeof(PIX));
    } else {
        do {
            memcpy(dPtr, sPtr, pixelsPerRow * sizeof(PIX));
            sPtr += stride;
            dPtr += dest.stride;
        } while (--numRows > 0);
    }
} // end Surface::blt

// bltTrans
//---------------------------------------------------------------------------
// Purpose: Puts the surface onto the destination using the slowest form of
//          transparency detection (pixel by pixel basis), while performing
//          clipping on the bounds of the object.
//---------------------------------------------------------------------------
void Surface::bltTrans(const Surface &dest, iXY min) const
{
    assert(getDoesExist());
    assert(dest.getDoesExist());
    assert(this != 0);
    assert(mem != 0);
    assert(dest.mem != 0);    

    // Add in the offset factor.
    min += offset;

    // Trivial clipping rejection - no overlap.
    // Also will jump out immediately if either image has zero size.
    if (min.x >= dest.pix.x) return;
    if (min.y >= dest.pix.y) return;

    iXY max;
    max = min + pix;
    //int	x2 = x1 + pix.x;
    //int	y2 = y1 + pix.y;
    if (max.x < 0) return;
    if (max.y < 0) return;

    int	pixelsPerRow = pix.x;
    int	numRows      = pix.y;

    PIX	*sRow = mem;      // Pointer to source Surface start of memory.
    PIX	*dRow = dest.mem; // Pointer to destination Surface start of memory.

    // Check for partial clip, calculate number of pixels
    // per row to copy, and number of rows to copy.  Adjust
    // sRow and dRow.

    // CLIP LEFT
    if (min.x < 0) {
        pixelsPerRow +=  min.x; // This will subtract the neg. x value.
        sRow         += -min.x; // This will move the sRow to x = 0, from the neg. x.
    } else {
        dRow += min.x;
    }

    // CLIP RIGHT
    // This subtracts only the portion hanging over the right edge of the
    // destination Surface
    if (max.x > dest.pix.x) pixelsPerRow -= max.x - dest.pix.x;

    // CLIP TOP
    if (min.y < 0) {
        numRows += min.y;
        sRow    -= min.y * stride;
    } else {
        dRow += min.y * dest.stride;
    }

    // CLIP BOTTOM
    // This subtracts only the portion hanging over the bottom edge of the
    // destination Surface
    if (max.y > dest.pix.y) numRows -= max.y - dest.pix.y;

    // Now, Check to make sure I actually have something
    // to draw.  I should - because I checked for trivial
    // rejection first.  These asserts just make sure
    // my clipping is working...
    assert(pixelsPerRow > 0);
    assert(numRows > 0);

    int srcAdjustment  = stride      - pixelsPerRow;
    int destAdjustment = dest.stride - pixelsPerRow;
    for (int row = 0; row < numRows; row++) {
        for (int col = 0; col < pixelsPerRow; col++) {
            if (*sRow != 0)
                *dRow = *sRow;
            sRow++;
            dRow++;
        }

        sRow += srcAdjustment;
        dRow += destAdjustment;
    }

} // end Surface::bltTrans

// bltTransC
//---------------------------------------------------------------------------
// Purpose: Puts the surface onto the destination using the slowest form of
//          transparency detection (pixel by pixel basis), while performing
//          clipping on the bounds of the object. The non-transparent pixels
//          are blitted in the specified color.
//---------------------------------------------------------------------------
void Surface::bltTransColor(const Surface &dest, iXY min, const uint8_t &color) const
{
    assert(getDoesExist());
    assert(dest.getDoesExist());
    assert(this != 0);
    assert(mem != 0);
    assert(dest.mem != 0);    

    // Add in the offset factor.
    min += offset;

    // Trivial clipping rejection - no overlap.
    // Also will jump out immediately if either image has zero size.
    if (min.x >= dest.pix.x) return;
    if (min.y >= dest.pix.y) return;

    iXY max;
    max = min + pix;
    if (max.x < 0) return;
    if (max.y < 0) return;

    int	pixelsPerRow = pix.x;
    int	numRows      = pix.y;

    PIX	*sPtr	= mem;      // Pointer to source Surface start of memory.
    PIX	*dPtr	= dest.mem; // Pointer to destination Surface start of memory.

    // Check for partial clip, calculate number of pixels
    // per row to copy, and number of rows to copy.  Adjust
    // sPtr and dPtr.

    // CLIP LEFT
    if (min.x < 0) {
        pixelsPerRow +=  min.x; // This will subtract the neg. x value.
        sPtr         += -min.x; // This will move the sPtr to x = 0, from the neg. x.
    }	else {
        dPtr += min.x;
    }

    // CLIP RIGHT
    // This subtracts only the portion hanging over the right edge of the
    // destination Surface
    if (max.x > dest.pix.x) pixelsPerRow -= max.x - dest.pix.x;

    // CLIP TOP
    if (min.y < 0) {
        numRows += min.y;
        sPtr    -= min.y * stride;
    }	else {
        dPtr += min.y * dest.stride;
    }

    // CLIP BOTTOM
    // This subtracts only the portion hanging over the bottom edge of the
    // destination Surface
    if (max.y > dest.pix.y) numRows -= max.y - dest.pix.y;

    // Now, Check to make sure I actually have something
    // to draw.  I should - because I checked for trivial
    // rejection first.  These asserts just make sure
    // my clipping is working...
    assert(pixelsPerRow > 0);
    assert(numRows > 0);

    int srcAdjustment  = stride      - pixelsPerRow;
    int destAdjustment = dest.stride - pixelsPerRow;

    for (int row = 0; row < numRows; row++) {
        for (int col = 0; col < pixelsPerRow; col++) {
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
void Surface::drawHLine(int x1, int y, int x2, const PIX &color) const
{
    assert(getDoesExist());
    assert(this != 0);

    // Check for trivial rejection
    if (y < 0 || x2 <= 0 || y >= pix.y || x1 >= pix.x) return;

    assert(mem != 0);
    if (mem == 0) return;

    orderCoords(x1, x2);

    unsigned length = x2 - x1;
    PIX *ptr = mem + y * stride;

    // CLIP LEFT
    if (x1 < 0) {
        length += x1;
    }	else {
        ptr += x1;
    }

    // CLIP RIGHT
    if (x2 >= pix.x) length -= (x2 - pix.x);

    memset(ptr, color, length * sizeof(PIX));

} // end Surface::drawHLine

// drawVLine
//---------------------------------------------------------------------------
// Purpose: Draws a vertical drawLine.
//---------------------------------------------------------------------------
void Surface::drawVLine(int x, int y1, int y2, const PIX &color) const
{
    assert(getDoesExist());
    assert(this != 0);

    // Check for trivial rejection
    if (x < 0 || y2 <= 0 || x >= pix.x || y1 >= pix.y) return;

    assert(mem != 0);
    if (mem == 0) return;

    orderCoords(y1, y2);

    // CLIP TOP
    if (y1 < 0) y1 = 0;

    // CLIP BOTTOM
    if (y2 >= pix.y) y2 = pix.y-1;

    PIX	*ptr	= mem+y1*stride+x;

    int	width	= y2 - y1;

    while(width > 0) {
        *ptr	= color;
        ptr	+= stride;
        width--;
    }
} // end Surface::drawVLine

// fillAll
//---------------------------------------------------------------------------
// Purpose: Filles all the frames with the specified color.
//---------------------------------------------------------------------------
void Surface::fillAll(const PIX &color)
{
    for (int i = 0; i < frameCount; i++) {
        setFrame(i);
        fill(color);
    }

} // end Surface::fillAll

// fill
//---------------------------------------------------------------------------
// Purpose: Fills the Surface will the specified color.
//---------------------------------------------------------------------------
void Surface::fill(const PIX &color) const
{
    assert(getDoesExist());
    assert(this != 0);

    if (pix.x == 0 || pix.y == 0) return;

    if (pix.x == stride) {
        memset(mem, color, pix.x * pix.y * sizeof(PIX));
    }	else {
        int	n = pix.y;
        PIX *ptr = mem;
        do {
            memset(ptr, color, pix.x);
            ptr += stride;
        } while (--n > 0);
    }

} // end Surface::fill

// fillRect
//---------------------------------------------------------------------------
// Purpose: Fills the specified rectangle in the calling Surface with the
//          specified color.
//---------------------------------------------------------------------------
void Surface::fillRect(iRect bounds, const PIX &color) const
{
    assert(getDoesExist());
    assert(this != 0);

    if (pix.isZero()) return;

    orderCoords(bounds);

    // Check for trivial rejection
    //if (bounds.max < 0 || bounds.min >= pix) return;
    if (bounds.max.x <  0)     return;
    if (bounds.max.y <  0)     return;
    if (bounds.min.x >= pix.x) return;
    if (bounds.min.y >= pix.y) return;

    // Check for clipping
    if (bounds.min.x <  0)     bounds.min.x = 0;
    if (bounds.min.y <  0)     bounds.min.y = 0;
    if (bounds.max.x > pix.x) bounds.max.x = pix.x;
    if (bounds.max.y > pix.y) bounds.max.y = pix.y;

    iXY diff;
    diff = (bounds.max - bounds.min);

    // Set memory to the top-left pixel of the rectangle.
    PIX	*ptr = mem + bounds.min.y * stride + bounds.min.x;

    for (int y = 0; y < diff.y; y++) {
        // Lay the horizontal strip.
        memset(ptr, color, diff.x * sizeof(PIX));
        ptr += stride;
    }
} // end Surface::fillRect

// drawRect
//---------------------------------------------------------------------------
// Purpose: Draws a rectagle in the specified color on the calling Surface.
//---------------------------------------------------------------------------
void Surface::drawRect(iRect bounds, const PIX &color) const
{
    assert(getDoesExist());
    assert(this != 0);

    if (pix.isZero()) return;

    orderCoords(bounds);

    // Check for trivial rejection
    if      (bounds.max.x <  0)     return;
    else if (bounds.max.y <  0)     return;
    else if (bounds.min.x >= pix.x) return;
    else if (bounds.min.y >= pix.y) return;

    // Check for clipping
    if (bounds.min.x <  0)     bounds.min.x = 0;
    if (bounds.min.y <  0)     bounds.min.y = 0;
    if (bounds.max.x >= pix.x) bounds.max.x = pix.x - 1;
    if (bounds.max.y >= pix.y) bounds.max.y = pix.y - 1;

    drawHLine(bounds.min.x, bounds.min.y, bounds.max.x,   color);
    drawHLine(bounds.min.x, bounds.max.y, bounds.max.x+1, color);
    drawVLine(bounds.min.x, bounds.min.y, bounds.max.y,   color);
    drawVLine(bounds.max.x, bounds.min.y, bounds.max.y,   color);
} // end Surface::drawRect

// drawLine
//---------------------------------------------------------------------------
// Purpose: Draws a drawLine with any slope.
//---------------------------------------------------------------------------
void Surface::drawLine(int x1, int y1, int x2, int y2, const PIX &color) const
{
    assert(getDoesExist());
    assert(this != 0);

    // Check for horizontal lines
    if (y1 == y2) {
        orderCoords(x1, x2);
        drawHLine(x1, y1, x2 + 1, color);
        return;
    }

    // Check for vertical lines
    if (x1 == x2) {
        orderCoords(y1, y2);
        drawVLine(x1, y1, y2, color);
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

// flipHorizontal
//---------------------------------------------------------------------------
// Purpose: Goes through all the frames of the surface and mirrors them
//          horizontally.
//---------------------------------------------------------------------------
void Surface::flipHorizontal()
{
    assert(getDoesExist());
    assert(this != 0);

    Surface tempSurface(pix, stride, 1);

    for (int frameNum = 0; frameNum < frameCount; frameNum++) {

        setFrame(frameNum);
        blt(tempSurface, 0, 0);

        for (int x = 0; x < pix.x; x++) {
            int offset = pix.x-1-x;

            for (int y = 0; y < pix.y; y++)
                tempSurface.putPixel(x, y, getPixel(offset, y));
        }
        tempSurface.blt(*this);
    }
} // end Surface::flipHorizontal

// flipVertical
//---------------------------------------------------------------------------
// Purpose: Goes through all the frames of the surface and flips then
//          vertically.
//---------------------------------------------------------------------------
void Surface::flipVertical()
{
    assert(getDoesExist());
    assert(this != 0);

    Surface tempSurface(pix, stride, 1);

    for (int frameNum = 0; frameNum < frameCount; frameNum++) {
        // This sets the mem pointer of the source Surface
        setFrame(frameNum);

        PIX *sPtr = mem + pix.x * pix.y - pix.x;
        PIX *dPtr = tempSurface.mem;

        for (int y = 0; y < pix.y; y++) {
            memcpy(dPtr, sPtr, pix.x * sizeof(PIX));
            sPtr -= pix.x;
            dPtr += pix.x;
        }
        tempSurface.blt(*this);
    }
} // end Surface::flipVertical

// copy
//---------------------------------------------------------------------------
// Purpose: Copies the specified number of frames from the source Surface to
//          the calling Surface.
//---------------------------------------------------------------------------
void Surface::copy(Surface &source)
{
    assert(source.getDoesExist());
    assert(this != 0);

    // Create a Surface the surface the same size as the source.
    create(source);

    for (int frameNum = 0; frameNum < source.getFrameCount(); frameNum++) {
        // Set the source Surface frame.
        setFrame(frameNum);
        source.setFrame(frameNum);

        // Blit the source frame to the calling frame
        source.blt(*this);
    }
} // end Surface::copy

// rotate
//---------------------------------------------------------------------------
// Purpose: Rotates a surface.  Accepts 0..360 integers for the degrees.
//---------------------------------------------------------------------------
void Surface::rotate(int angle)
{
    assert(this != 0);

    for (int i = 0; i < frameCount; i++) {
        setFrame(i);

        if (angle % 360 != 0) {
            Surface tempSurface(pix.x, pix.y, stride, 1);

            float angleRadians = -float(angle) / float(180.0 / M_PI);
            float cosAngle     = cos(angleRadians);
            float sinAngle     = sin(angleRadians);

            int index   = 0;

            for (int y = -center.y; y < center.y; y++) {
                for (int x = -center.x; x < center.x; x++) {
                    int xSource = int((x * cosAngle - y * sinAngle) + center.x);
                    int ySource = int((y * cosAngle + x * sinAngle) + center.y);

                    if ((xSource >= 0) && (xSource < pix.x) && (ySource >= 0) && (ySource < pix.y)) {
                        tempSurface.putPixel(index % pix.x, index / pix.y, getPixel(xSource, ySource));
                    } else {
                        // Set the pixel transparent
                        tempSurface.putPixel(index % pix.x, index / pix.y, 0);
                    }
                    index++;
                }
            }

            tempSurface.blt(*this, 0, 0);
        }
    }
} // end ROTATE

// scale
//---------------------------------------------------------------------------
// Purpose: Scales all the frames of the surface from the current size to the
//          requested size.
//---------------------------------------------------------------------------
void Surface::scale(const iXY &pix)
{
    assert(getDoesExist());
    assert(this != 0);

    if (pix.x <= 0) {
        return;
    }
    if (pix.y <= 0) {
        return;
    }
    if (pix.x == this->pix.x && pix.y == this->pix.y) {
        return;
    }

    // Create a temporary surface to scale the image onto.
    Surface tempSurface(pix, pix.x, frameCount);

    iRect r(0, 0, pix.x, pix.y);

    // Go through all the frames of the surface.
    int frame;
    for (frame = 0; frame < tempSurface.getFrameCount(); frame++) {
        tempSurface.setFrame(frame);
        Surface::setFrame(frame);

        tempSurface.bltScale(*this, r);
    }

    // Resize the calling surface, then copy all the scaled images on it.
    Surface::resize(pix);

    for (frame = 0; frame < tempSurface.getFrameCount(); frame++) {
        Surface::setFrame(frame);
        tempSurface.setFrame(frame);

        tempSurface.blt(*this);
    }

    /* OLD VERSION 6.29.1998
    	// Find out the number of pix to step in the image.
    	fXY stepPix;
    	stepPix.x = float(Surface::pix.x) / float(pix.x);
    	stepPix.y = float(Surface::pix.y) / float(pix.y);
     
    	// Create a temporary surface to scale the image onto.
    	Surface tempSurface(pix, pix.x, frameCount);
     
    	// build a table the first time you go through, then scaling all
    	// additional frames can be done by getting the values from the table.
     
    	fXY curPix;
    	curPix = 0.0;
     
    	// Go through all the frames of the surface.
    	for (int frame = 0; frame < frameCount; frame++)
    	{
    		//LOG(("frame: %d; frameCount: %d", frame, frameCount));
    		tempSurface.setFrame(frame);
    		this->setFrame(frame);
     
    		for (int x = 0; x < pix.x; x++)
    		{
    			for (int y = 0; y < pix.y; y++)
    			{
    				tempSurface.putPixel(x, y, this->getPixel(int(curPix.x), int(curPix.y)));
    				curPix.y += stepPix.y;
    			}
    			curPix.y = 0.0;
    			curPix.x += stepPix.x;
    		}
    		curPix.x = 0.0;
    	}
     
    	// Resize the calling surface, then map all the scaled images on it.
    	this->resize(pix);
     
    	assert(frameCount == tempSurface.frameCount);
     
    	for (frame = 0; frame < frameCount; frame++)
    	{
    		//LOG(("frame: %d; frameCount: %d", frame, frameCount));
    		this->setFrame(frame);
    		tempSurface.setFrame(frame);
    		tempSurface.blt(*this);
    	}
    */
} // end Surface::scale

// verticalWave3DAll
//---------------------------------------------------------------------------
void Surface::verticalWave3DAll(int numWaves, float percent)
{
    assert(getDoesExist());
    assert(this != 0);

    float curOffset  = 0;
    float offsetStep = 100.0 / float(frameCount);

    for (int curFrame = 0; curFrame < frameCount; curFrame++) {
        setFrame(curFrame);
        verticalWave3D(numWaves, percent, (int) curOffset);
        curOffset += offsetStep;
    }
} // end Surface::verticalWave3DAll

// verticalWave3D
//---------------------------------------------------------------------------
void Surface::verticalWave3D(int numWaves, float percent, int offset)
{
    assert(getDoesExist());
    assert(this != 0);

    Surface temp;
    temp.create(pix.x, pix.y, pix.x, 1);

    float angleRadians = (offset * 3.6) / (180.0 / M_PI);
    float angleStep    = ((M_PI * 2) * numWaves) / pix.y;
    float amplitude    = (percent * pix.y / 2) / 100.0;

    blt(temp, 0, 0);

    int y = 0;
    for (int offset1 = 0; offset1 < pix.x * pix.y; offset1 += pix.x) {
        int offset2 = max(min(int(((cos(angleRadians) * amplitude + y))), pix.y - 1), 0) * pix.x;
        y++;
        memcpy((mem + offset1), (temp.mem + offset2), pix.x);

        angleRadians += angleStep;
    }
} // end Surface::VERTICAL WAVE 3D

// horizontalWave3DAll
//---------------------------------------------------------------------------
void Surface::horizontalWave3DAll(int numWaves, float percent)
{
    assert(getDoesExist());
    assert(this != 0);

    float curOffset  = 0;
    float offsetStep = 100.0 / float(frameCount);

    for (int curFrame = 0; curFrame < frameCount; curFrame++) {
        setFrame(curFrame);
        horizontalWave3D(numWaves, percent, (int) curOffset);
        curOffset += offsetStep;
    }
} // end Surface::horizontalWave3DAll

// horizontalWave3D
//---------------------------------------------------------------------------
void Surface::horizontalWave3D(int numWaves, float percent, int offset)
{
    assert(getDoesExist());
    assert(this != 0);

    Surface temp;
    temp.create(pix.x, pix.y, pix.x, 1);

    float angleRadians = (offset*3.6) / (180.0 / M_PI);
    float angleStep    = ((M_PI * 2) * numWaves) / pix.y;
    float amplitude    = (percent * pix.y / 2) / 100.0;

    blt(temp, 0, 0);

    for (int x = 0; x < pix.x; x++) {
        int xOffset = max(min((int)((cos(angleRadians) * amplitude + x)), pix.x - 1), 0);
        for (int yOffset = 0; yOffset < pix.x * pix.y; yOffset += pix.x) {
            mem[x + yOffset] = temp.mem[xOffset + yOffset];
            angleRadians += angleStep;
        }
    }
} // end Surface::horizontalWave3D

// rippleAll
//---------------------------------------------------------------------------
void Surface::rippleAll(int numWaves, float percent)
{
    assert(getDoesExist());
    assert(this != 0);

    float curOffset  = 0;
    float offsetStep = 100.0 / float(frameCount);

    for (int curFrame = 0; curFrame < frameCount; curFrame++) {
        setFrame(curFrame);
        ripple(numWaves, percent, (int) curOffset);
        curOffset += offsetStep;
    }
} // end Surface::rippleAll

// ripple
//---------------------------------------------------------------------------
void Surface::ripple(int numWaves, float percent, int offset)
{
    assert(getDoesExist());
    assert(this != 0);

    Surface temp(pix.x, pix.y, pix.x, 1);

    unsigned index     = 0;

    float piTimes2     = float(M_PI * 2.0);
    float angleRadians = (piTimes2 * percent) / 100.0;
    float maxDist      = sqrt(pix.x * pix.x + pix.y * pix.y);
    float scale        = (piTimes2 * numWaves) / maxDist;

    offset = (int) ((offset * piTimes2) / 100.0);

    blt(temp, 0, 0);

    for (int y = 0; y < pix.y; y++) {
        for (int x = 0; x < pix.x; x++) {
            float a  = sin(sqrt(x * x + y * y) * scale + offset) * angleRadians;
            float ca = cos(a);
            float sa = sin(a);

            int xs = (int)(x * ca - y * sa) + x - center.x;
            int ys = (int)(y * ca + x * sa) + y - center.y;

            if (xs >= 0 && xs < x - pix.x && ys >= 0 && ys < y - pix.y)
                mem[index++] = temp.mem[(x - pix.x) * ys + xs];
            else mem[index++] = Color::black;
        }
    }
} // end Surface::ripple

// horizontalWaveAll
//---------------------------------------------------------------------------
void Surface::horizontalWaveAll(int numWaves, float percent)
{
    assert(getDoesExist());
    assert(this != 0);

    float curOffset  = 0;
    float offsetStep = 100.0 / float(frameCount);

    for (int frame = 0; frame < frameCount; frame++) {
        setFrame(frame);
        horizontalWave(numWaves, percent, (int) curOffset);
        curOffset += offsetStep;
    }
} // end Surface::horizontalWaveAll

// horizontalWave
//---------------------------------------------------------------------------
void Surface::horizontalWave(int numWaves, float percent, int offset)
{
    assert(getDoesExist());
    assert(this != 0);

    Surface temp(pix.x, pix.y, pix.x, 1);
    blt(temp, 0, 0);

    float piTimes2      = float(M_PI * 2.0);
    float waveFrequency = (numWaves * piTimes2) / pix.y;
    float waveOffset    = (offset * numWaves * piTimes2) / 100.0;
    float radius        = (pix.x * percent) / 100.0;

    int index = 0;

    for (int y = 0; y < pix.y; y++) {
        int xOffset = int(round(sin(y * waveFrequency + waveOffset) * radius));
        for (int x = 0; x < pix.x; x++) {
            if (xOffset >= 0 && xOffset < pix.x)
                mem[index++] = temp.mem[xOffset + pix.x * y];
            else mem[index++] = Color::black;
            xOffset++;
        }
    }
} // end Surface::horizontalWave

// verticalWaveAll
//---------------------------------------------------------------------------
void Surface::verticalWaveAll(int numWaves, float percent)
{
    assert(getDoesExist());
    assert(this != 0);

    float curOffset  = 0;
    float offsetStep = 100.0 / float(frameCount);

    for (int frame = 0; frame < frameCount; frame++) {
        setFrame(frame);
        verticalWave(numWaves, percent, (int) curOffset);
        curOffset += offsetStep;
    }
} // end Surface::verticalWaveAll

// verticalWave
//---------------------------------------------------------------------------
void Surface::verticalWave(int numWaves, float percent, int offset)
{
    assert(getDoesExist());
    assert(this != 0);

    Surface temp(pix.x, pix.y, pix.x, 1);
    blt(temp, 0, 0);

    float piTimes2      = float(M_PI * 2.0);
    float waveFrequency = (numWaves * piTimes2) / pix.y;
    float waveOffset    = (offset * numWaves * piTimes2) / 100.0;
    float radius        = (pix.x * percent) / 100.0;

    for (int x = 0; x < pix.x; x++) {
        int yOffset = int(round(sin(x * waveFrequency + waveOffset) * radius));

        for (int y = 0; y < pix.y; y++) {
            if (yOffset >= 0 && yOffset < pix.y) {
                mem[pix.x * y + x] = temp.mem[pix.x * yOffset + x];
            } else {
                mem[pix.x * y + x] = Color::black;
            }
            yOffset++;
        }
    }
} // end Surface::verticalWave

// drawButtonBorder
//---------------------------------------------------------------------------
void Surface::drawButtonBorder(iRect bounds, PIX topLeftColor, PIX bottomRightColor) const
{
    assert(getDoesExist());
    assert(this != 0);

    orderCoords(bounds);

    // Top.
    drawLine(bounds.min.x,    bounds.min.y, -1+bounds.max.x,    bounds.min.y, topLeftColor);

    // Left.
    drawLine(bounds.min.x,    bounds.min.y,    bounds.min.x, -1+bounds.max.y, topLeftColor);

    // Right.
    drawLine(-1+bounds.max.x,    bounds.min.y, -1+bounds.max.x, -1+bounds.max.y, bottomRightColor);

    // Bottom.
    drawLine(bounds.min.x, -1+bounds.max.y, -1+bounds.max.x, -1+bounds.max.y, bottomRightColor);

} // end Surface::drawButtonBorder

// blendIn
//---------------------------------------------------------------------------
// Purpose - Blend the specified surface into the calling surface.  The upper-left
//           corner is min.
//---------------------------------------------------------------------------
void Surface::blendIn(const Surface &source, iXY min, ColorTable &colorTable) const
{
    assert(getDoesExist());
    assert(source.getDoesExist());
    assert(this != 0);
    assert(colorTable.getColorCount() == 256 * 256);

    min += source.offset;

    if (min.x >= pix.x) return;
    if (min.y >= pix.y) return;

    iXY max;
    max = min + source.pix;
    if (max.x <= 0) return;
    if (max.y <= 0) return;

    // Something is overlapping, so we need to verify that both
    // surfaces are valid.
    assert(mem        != 0);
    assert(source.mem != 0);

    int        pixelsPerRow = source.pix.x;
    int        numRows      = source.pix.y;
    const PIX *sRow         = source.mem;		// Pointer to the source Surface
    PIX       *dRow         = mem;			// Pointer to the destination Surface

    // CLIP LEFT
    if (min.x < 0) {
        pixelsPerRow += min.x;
        sRow         -= min.x;
    }	else {
        dRow += min.x;
    }

    // CLIP RIGHT
    if (max.x > pix.x) {
        pixelsPerRow -= max.x - pix.x;
    }

    // CLIP TOP
    if (min.y < 0) {
        numRows += min.y;
        sRow    -= (min.y*source.stride);
    }	else {
        dRow += (min.y*stride);
    }

    // CLIP BOTTOM
    if (max.y > pix.y) {
        numRows -= max.y-pix.y;
    }

    // Check to make sure the clipping is working.
    if (signed(pixelsPerRow) <= 0) return;
    if (signed(numRows) <= 0) return;

    const uint8_t *table = colorTable.getColorArray();

    for (int yCount = 0 ; yCount < numRows ; yCount++) {
        bltBlendSpan(dRow, sRow, pixelsPerRow, table);

        sRow += source.stride;
        dRow += stride;
    }

} // end Surface::blendIn

// bltBlendScale
//---------------------------------------------------------------------------
// Purpose - Blend the specified surface into the calling surface.  The upper-left
//           corner is min.
//---------------------------------------------------------------------------
void Surface::bltBlendScale(const Surface &source, const iRect &destRect, ColorTable &colorTable) const
{
    assert(getDoesExist());
    assert(source.getDoesExist());
    assert(this != 0);
    assert(colorTable.getColorCount() == 256 * 256);

    iXY min = destRect.min + source.offset;
    iXY max = destRect.max + source.offset;

    if (min.x >= pix.x) return;
    if (min.y >= pix.y) return;

    // Something is overlapping, so we need to verify that both
    // surfaces are valid.
    assert(mem        != 0);
    assert(source.mem != 0);

    size_t pixelsPerRow = max.x-min.x;
    int numRows      = max.y-min.y;

    int srcX1 = 0;
    int srcY = 0;

    // FIXME - if inverted bitmap, then flip x1 and y to right or botom edges

    // Compute source deltas
    int xSrcDelta = int((float(source.pix.x) / float(max.x - min.x)) * 65536.0);
    int ySrcDelta = int((float(source.pix.y) / float(max.y - min.y)) * 65536.0);

    PIX  *dRow = mem;			// Pointer to the destination Surface

    // CLIP LEFT
    if (min.x < 0) {
        pixelsPerRow += min.x;
        srcX1 -= min.x*xSrcDelta;
    }	else {
        dRow += min.x;
    }

    // CLIP RIGHT
    if (max.x > pix.x) {
        pixelsPerRow -= max.x - pix.x;
    }

    // CLIP TOP
    if (min.y < 0) {
        numRows += min.y;
        srcY   -= min.y*ySrcDelta;
    }	else {
        dRow += (min.y*stride);
    }

    // CLIP BOTTOM
    if (max.y > pix.y) {
        numRows -= max.y-pix.y;
    }

    // Check to make sure the clipping is working.
    if (pixelsPerRow <= 0) return;
    if (numRows <= 0) return;

    const uint8_t *table = colorTable.getColorArray();
#if 0
    int stepAndDecCount = (xSrcDelta << 16) | 0xffff;
    int stepWholePart = xSrcDelta >> 16;
    int srcX1FracWithCount = (srcX1 << 16) | pixelsPerRow;
#endif

    float xdelta = float(source.pix.x) / float(max.x - min.x);
    for (int yCount = 0 ; yCount < numRows ; yCount++) {
        const PIX *sRow = source.rowPtr(srcY >> 16) + (srcX1 >> 16);

        // bltBlendScaleSpan(dRow, sRow, srcX1FracWithCount, stepAndDecCount, stepWholePart, table);

        // XXX: WARNING SLOW CODE
        float sPos = 0;
        for(size_t x=0; x<pixelsPerRow; x++) {
            dRow[x] = table[ (dRow[x]<<8) + sRow[(size_t) sPos]];
            //dRow[x] = table[sRow[(size_t) sPos]];
            sPos += xdelta;
        }

        srcY += ySrcDelta;
        dRow += stride;
    }

} // end Surface::bltBlendScale

void Surface::bltLookup(const iRect &destRect, const PIX table[]) const
{
    assert(getDoesExist());
    assert(this != 0);
    assert(mem != 0);

    iXY min = destRect.min + offset;
    if (min.x >= pix.x) return;
    if (min.y >= pix.y) return;

    iXY max = destRect.max + offset;
    if (max.x < 0) return;
    if (max.y < 0) return;

    // Clip destination rectangle
    if (min.x < 0) min.x = 0;
    if (min.y < 0) min.y = 0;
    if (max.x >= pix.x) max.x = pix.x;
    if (max.y >= pix.y) max.y = pix.y;

    size_t pixelsPerRow = max.x - min.x;
    size_t numRows      = max.y - min.y;

    PIX *dRow = mem + min.y*stride + min.x;

    for (size_t yCount = 0 ; yCount < numRows ; yCount++) {
        for(size_t x=0; x<pixelsPerRow; x++)
            dRow[x] = table[dRow[x]];

        dRow += stride;
    }
}

//---------------------------------------------------------------------------
void Surface::bltScale(const Surface &source, const iRect &destRect) const
{
    assert(getDoesExist());
    assert(source.getDoesExist());
    assert(this != 0);

    iXY min = destRect.min + source.offset;
    iXY max = destRect.max + source.offset;

    if (min.x >= pix.x) return;
    if (min.y >= pix.y) return;

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
    int xSrcDelta = int((float(source.pix.x) / float(max.x - min.x)) * 65536.0);
    int ySrcDelta = int((float(source.pix.y) / float(max.y - min.y)) * 65536.0);

    PIX  *dRow = mem;			// Pointer to the destination Surface

    // CLIP LEFT
    if (min.x < 0) {
        pixelsPerRow += min.x;
        srcX1 -= min.x*xSrcDelta;
    }	else {
        dRow += min.x;
    }

    // CLIP RIGHT
    if (max.x > pix.x) {
        pixelsPerRow -= max.x - pix.x;
    }

    // CLIP TOP
    if (min.y < 0) {
        numRows += min.y;
        srcY   -= min.y*ySrcDelta;
    }	else {
        dRow += (min.y*stride);
    }

    // CLIP BOTTOM
    if (max.y > pix.y) {
        numRows -= max.y-pix.y;
    }

    // Check to make sure the clipping is working.
    if (pixelsPerRow <= 0) return;
    if (numRows <= 0) return;

#if 0
    int stepAndDecCount = (xSrcDelta << 16) | 0xffff;
    int stepWholePart = xSrcDelta >> 16;
    int srcX1FracWithCount = (srcX1 << 16) | pixelsPerRow;
#endif

    float xdelta = float(source.pix.x) / float(max.x - min.x);
    for (size_t yCount = 0 ; yCount < numRows ; yCount++) {
        const PIX *sRow = source.rowPtr(srcY >> 16) + (srcX1 >> 16);

#if 0
        bltScaleSpan(dRow, sRow, srcX1FracWithCount, stepAndDecCount, stepWholePart);
#else
        // XXX: WARNING SLOW CODE
        float sPos = 0;
        for(size_t x=0; x<pixelsPerRow; x++) {
            dRow[x] = sRow[(size_t) sPos];
            sPos += xdelta;
        }
#endif

        srcY += ySrcDelta;
        dRow += stride;
    }
}

// blendRect
//---------------------------------------------------------------------------
// Purpose - Blend the specified iRect using one of the specific palette tables.
//---------------------------------------------------------------------------
void Surface::blendRect(iRect bounds, ColorTable &colorTable) const
{
    assert(getDoesExist());
    assert(this != 0);
    assert(Palette::brightness256.getColorCount() == 256);

    if (pix.isZero()) return;

    orderCoords(bounds);

    // Check for trivial rejection
    //if (bounds.max < 0 || bounds.min >= pix) return;
    if (bounds.max.x <  0)     return;
    if (bounds.max.y <  0)     return;
    if (bounds.min.x >= pix.x) return;
    if (bounds.min.y >= pix.y) return;

    assert(mem != 0);
    if (mem == 0) return;

    // Check for clipping
    if (bounds.min.x <  0)     bounds.min.x = 0;
    if (bounds.min.y <  0)     bounds.min.y = 0;
    if (bounds.max.x >= pix.x) bounds.max.x = pix.x - 1;
    if (bounds.max.y >= pix.y) bounds.max.y = pix.y - 1;

    iXY diff;
    diff = bounds.max - bounds.min;

    // Set memory to the top-left pixel of the rectangle.
    PIX	*ptr = mem + bounds.min.y * stride + bounds.min.x;

    int index;
    int shiftFactor;
    int minusAmount;

    if      (colorTable.getColorCount() == 16) {
        shiftFactor = 4;
        minusAmount = 16;
    } else if (colorTable.getColorCount() == 32) {
        shiftFactor = 3;
        minusAmount = 8;
    } else if (colorTable.getColorCount() == 64) {
        shiftFactor = 2;
        minusAmount = 4;
    } else if (colorTable.getColorCount() == 256) {
        shiftFactor = 0;
        minusAmount = 0;
    } else {
        throw Exception("ERROR: Invalid number of colors for color array.");
    }

    for (int y = 0; y < diff.y; y++) {
        for (int x = 0; x < diff.x; x++) {
            index = (int) ((float(Palette::brightness256[*ptr]) / 100.0f) *
                           colorTable.getColorCount());
            assert(index < colorTable.getColorCount());
            *ptr = colorTable[index];
            ptr++;
        }
        ptr += stride-bounds.getSize().x;
    }
} // end Surface::blendRect

// shrinkWrap
//---------------------------------------------------------------------------
void Surface::shrinkWrap()
{
    assert(getDoesExist());
    assert(this != 0);

    // Start the bounds values in the center of the surface.
    iRect bounds;
    bounds.min.x = center.x;
    bounds.max.x = center.x;
    bounds.min.y = center.y;
    bounds.max.y = center.y;

    int num;
    for (num = 0; num < frameCount; num++) {
        setFrame(num);
        //LOG(("curFrame:  %d", curFrame));
        //LOG(("frameCount: %d", frameCount));
        //LOG(("pix.x:     %d", pix.x));
        //LOG(("pix.y:     %d", pix.y));

        // Check the x bounds.
        for (int y = 0; y < pix.y; y++) {
            for (int x = 0; x < pix.x; x++) {
                if (getPixel(x, y) != 0) {
                    if (x < bounds.min.x) {
                        bounds.min.x = x;
                    }
                    if (x > bounds.max.x) {
                        bounds.max.x = x;
                    }
                }
            }
        }

        // Check the y bounds.
        for (int x = 0; x < pix.x; x++) {
            for (int y = 0; y < pix.y; y++) {
                if (getPixel(x, y) != 0) {
                    if (y < bounds.min.y) {
                        bounds.min.y = y;
                    }
                    if (y > bounds.max.y) {
                        bounds.max.y = y;
                    }
                }
            }
        }
    }

    int xDiff = bounds.max.x-bounds.min.x;
    int yDiff = bounds.max.y-bounds.min.y;

    //LOG(("bounds.min.x: %d", bounds.min.x));
    //LOG(("bounds.max.x: %d", bounds.max.x));
    //LOG(("bounds.min.y: %d", bounds.min.y));
    //LOG(("bounds.max.y: %d", bounds.max.y));
    //LOG(("xDiff:        %d", xDiff));
    //LOG(("yDiff:        %d", yDiff));

    // Create a temporary surface to draw all the cropped frames onto.
    Surface tempSurface;
    tempSurface.create(xDiff, yDiff, xDiff, frameCount);

    // Crop the surface frames onto the temp surface.
    for (num = 0; num < frameCount; num++) {
        setFrame(num);
        tempSurface.setFrame(num);
        tempSurface.fill(0);
        blt(tempSurface, -bounds.min.x, -bounds.min.y);
    }

    resize(xDiff, yDiff);

    copy(tempSurface);

} // end Surface::shrinkWrap

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
    for (int x = 0; x < pix.x; x++) {
        for (int y = 0; y < pix.y; y++) {
            avgR += Palette::color[getPixel(x, y)].red;
            avgG += Palette::color[getPixel(x, y)].green;
            avgB += Palette::color[getPixel(x, y)].blue;
        }
    }

    // Divide each individual amount by the number of bytes in the image.
    int numPix = getArea();

    avgR /= numPix;
    avgG /= numPix;
    avgB /= numPix;

    return Palette::findNearestColor(RGBColor(avgR, avgG, avgB));
} // end Surface::getAverageColor

// loadTIL
//---------------------------------------------------------------------------
void Surface::loadTIL(const char* filename)
{
    std::auto_ptr<ReadFile> file(FileSystem::openRead(filename));

    FletchTileHeader fletchTileHeader;
    file->read(&fletchTileHeader, sizeof(FletchTileHeader), 1);

    if (frame0 == 0 || mem == 0 || pix.x != int(fletchTileHeader.xSize) || pix.y != int(fletchTileHeader.ySize)) {
        create(fletchTileHeader.xSize, fletchTileHeader.ySize, fletchTileHeader.xSize, 1);
    }

    int numBytes = pix.x * pix.y * sizeof(uint8_t);

    if (numBytes <= 0) return;
    if (mem == 0) {
        throw Exception("ERROR: This should not happen.");
    }

    if(file->read(mem, numBytes, 1) != 1)
        throw Exception("Error reading .TIL '%s'", filename);
} // end Surface::loadTIL

// setBrightness
void Surface::setBrightness(int percent)
{
    assert(percent >= 0 || percent <= 100);

    for (int x = 0; x < pix.x; x++) {
        for (int y = 0; y < pix.y; y++) {
            putPixel(x, y, Palette::colorTableDarkenALittle[(getPixel(x, y) * 100) + percent]);
        }
    }

} // end Surface::setBrightness

// bltBrightness
void Surface::bltBrightness(const Surface &dest, const iXY &pos, const float &percent)
{
    assert(getDoesExist());
    assert(dest.getDoesExist());
    assert(this != 0);
    assert(Palette::colorTableBrighten.getColorCount() == 256 * 256);
    assert(Palette::brightness256.getColorCount() == 256);

    iXY min(pos);

    min += offset;

    if (min.x >= dest.pix.x) return;
    if (min.y >= dest.pix.y) return;

    iXY max(min + pix);
    if (max.x <= 0) return;
    if (max.y <= 0) return;

    // Something is overlapping, so we need to verify that both
    // surfaces are valid.
    assert(mem      != 0);
    assert(dest.mem != 0);

    int        pixelsPerRow = pix.x;
    int        numRows      = pix.y;
    const PIX *sRow         = mem;        // Pointer to the this (source) Surface
    PIX       *dRow         = dest.mem;   // Pointer to the dest Surface

    // CLIP LEFT
    if (min.x < 0) {
        pixelsPerRow += min.x;
        sRow         -= min.x;
    }	else {
        dRow += min.x;
    }

    // CLIP RIGHT
    if (max.x > dest.pix.x) {
        pixelsPerRow -= max.x - dest.pix.x;
    }

    // CLIP TOP
    if (min.y < 0) {
        numRows += min.y;
        sRow    -= (min.y * stride);
    }	else {
        dRow += (min.y * dest.stride);
    }

    // CLIP BOTTOM
    if (max.y > dest.pix.y) {
        numRows -= max.y - dest.pix.y;
    }

    // Check to make sure the clipping is working.
    //if (signed(pixelsPerRow) <= 0) return;
    //if (signed(numRows) <= 0) return;
    assert(signed(pixelsPerRow) > 0);
    assert(signed(numRows) > 0);

    PIX *d;
    int xCount;

    for (int yCount = 0 ; yCount < numRows ; yCount++) {
        const PIX *s = sRow;
        d            = dRow;
        xCount       = pixelsPerRow;

        while (xCount > 0) {
            //*d = Palette::colorTableBrighten[((*d) * 256) + (Palette::brightness256[(*s)] * percent)];
            *d = Palette::colorTableBrighten[
                     (int) (((*d) << 8) + (Palette::brightness256[(*s)] * percent))];

            xCount--;
            s++;
            d++;
        }

        sRow += stride;
        dRow += dest.stride;
    }

} // end Surface::bltBrightness

static uint8_t quickHack[65536];

static void bltLightDarkSpan(int n, PIX *d, const uint8_t *i, const PIX *s)
{
    static int once = 0;

    if (!once) {
        for (int i = 0 ; i < 65536 ; ++i) {
            quickHack[i] = Palette::colorTableLightDark[i];
        }
        once = 1;
    }

    // Quick bail if no pixels in span

    if (n < 1) return;

    // Align to 4-byte dest
#if 0
    while (int(d) & 3)
    {
        //assert(((*s << 8) + *i) < 256 * 256);
        *d = Palette::colorTableLightDark[(*s << 8) + *i];

        n--;
        i++;
        s++;
        d++;

        if (n < 1) return;
    }

    if (n > 3) {
        // XXX no msvc assembler
#ifdef MSVC
        _asm {

            // load vars into regs
            push ebp
            mov ebx, i
            mov esi, s
            mov edi, d
            mov ebp, n

            xor ecx, ecx
    quad:

            mov cl, [ebx + 2]
            add ebx, 4

                mov ch, [esi + 2]
                add edi, 4

                    mov al, quickHack[ecx]
                    mov cl, [ebx - 4 + 3]

                    mov ch, [esi + 3]
                    add esi, 4

                        mov ah, quickHack[ecx]

                        shl eax, 16
                        mov cl, [ebx - 4 + 0]

                        mov ch, [esi - 4 + 0]
                        sub ebp, 4

                        mov al, quickHack[ecx]
                        mov cl, [ebx - 4 + 1]

                        mov ch, [esi - 4 + 1]

                        mov ah, quickHack[ecx]

                        // decrement counter and advance pointers
                        mov [edi-4], eax
                        ja quad

                        // restore vars for the c code that cleans up
                        mov eax, ebp
                        pop ebp
                        mov i, ebx
                        mov s, esi
                        mov d, edi
                        mov n, eax
                    }
#endif

                }
#endif
    while (n > 0) {
        //assert(((*s << 8) + *i) < 256 * 256);
        *d = Palette::colorTableLightDark[(*s << 8) + *i];

        n--;
        i++;
        s++;
        d++;
    }

}

void Surface::bltLightDark(const Surface &source, const Surface &lightTable) const
{
    // Make sure all 3 surfaces have the same dimension.
    assert(lightTable.getPix().x  == pix.x && source.getPix().x  == pix.x);
    assert(lightTable.getPix().y  == pix.y && source.getPix().y  == pix.y);
    //assert(lightTable.getStride() == stride && source.getStride() == stride);
    assert(Palette::colorTableLightDark.getColorCount() == 256 * 256);

    int numRows = pix.y;

    uint8_t const *rowI = lightTable[0];
    PIX  const *rowS = source.mem;
    PIX        *rowD = mem;

    while (numRows > 0) {
        //int xCount = pix.x;
        //uint8_t const *i = rowI;
        //PIX  const *s = rowS;
        //PIX        *d = rowD;

        //while (xCount > 0)
        //{
        //	//assert(((*s << 8) + *i) < 256 * 256);
        //	*d = Palette::colorTableLightDark[(*s << 8) + *i];
        //
        //	xCount--;
        //	i++;
        //	s++;
        //	d++;
        //}
        bltLightDarkSpan(pix.x, rowD, rowI, rowS);

        numRows--;
        rowI += lightTable.getStride();
        rowS += source.getStride();
        rowD += stride;
    }
}

void Surface::setToBrightnessIndexes128()
{
    for (int x = 0; x < pix.x; x++) {
        for (int y = 0; y < pix.y; y++) {
            putPixel(x, y, (int) (Palette::brightness256[getPixel(x, y)] *
                                  (128.0/100.0)));
        }
    }
}

// bltAdd
//---------------------------------------------------------------------------
// Purpose:
//---------------------------------------------------------------------------

void Surface::bltAdd(const Surface &dest, iXY min) const
{
    assert(getDoesExist());
    assert(dest.getDoesExist());
    assert(this != 0);

    static uint8_t saturateTable[512];

    static int once = 0;
    if (!once) {
        int i;
        for (i = 0 ; i < 256 ; ++i) saturateTable[i] = i;
        for (i = 256 ; i < 512 ; ++i) saturateTable[i] = 255;
        once = 1;
    }


    // Add in the offset factor.
    min += offset;

    // Trivial clipping rejection - no overlap.
    // Also will jump out immediately if either image has zero size.
    if (min.x >= dest.pix.x) return;
    if (min.y >= dest.pix.y) return;

    iXY max;
    max = min + pix;
    //int	x2 = x1 + pix.x;
    //int	y2 = y1 + pix.y;
    if (max.x <= 0) return;
    if (max.y <= 0) return;

    // Something is overlapping, so we need to verify that both
    // surfaces are valid.
    assert(mem != 0);
    assert(dest.mem != 0);
    if (mem == 0 || dest.mem == 0) return;

    int	pixelsPerRow = pix.x;
    int	numRows      = pix.y;

    PIX	*sPtr	= mem;      // Pointer to source Surface start of memory.
    PIX	*dPtr	= dest.mem; // Pointer to destination Surface start of memory.

    // Check for partial clip, calculate number of pixels
    // per row to copy, and number of rows to copy.  Adjust
    // sPtr and dPtr.

    // CLIP LEFT
    if (min.x < 0) {
        pixelsPerRow +=  min.x; // This will subtract the neg. x value.
        sPtr         += -min.x; // This will move the sPtr to x = 0, from the neg. x.
    }	else {
        dPtr += min.x;
    }

    // CLIP RIGHT
    // This subtracts only the portion hanging over the right edge of the
    // destination Surface
    if (max.x > dest.pix.x) pixelsPerRow -= max.x - dest.pix.x;

    // CLIP TOP
    if (min.y < 0) {
        numRows += min.y;
        sPtr    -= min.y * stride;
    }	else {
        dPtr += min.y * dest.stride;
    }

    // CLIP BOTTOM
    // This subtracts only the portion hanging over the bottom edge of the
    // destination Surface
    if (max.y > dest.pix.y) numRows -= max.y - dest.pix.y;

    // Now, Check to make sure I actually have something
    // to draw.  I should - because I checked for trivial
    // rejection first.  These asserts just make sure
    // my clipping is working...
    if (signed(pixelsPerRow) <= 0) return;
    if (signed(numRows) <= 0) return;
    assert(signed(pixelsPerRow) > 0);
    assert(signed(numRows) > 0);

    // Now blt the sucker!  But first, see if we can do it in one
    // big blt, without doing each row individually...
    if (stride == pixelsPerRow && dest.stride == pixelsPerRow) {
        pixelsPerRow *= numRows;
        numRows = 1;
    }

    do {

        // Align to 4-byte dest boundary

        PIX *d = dPtr;
        PIX *s = sPtr;
        int n = pixelsPerRow;

        // Cleanup
        while (n > 0) {
            *d = saturateTable[int(*d) + int(*s)];
            ++d;
            ++s;
            --n;
        }

        sPtr += stride;
        dPtr += dest.stride;
    } while (--numRows > 0);


} // end bltAdd

// loadAllTILInDirectory
//---------------------------------------------------------------------------
int Surface::loadAllTILInDirectory(const char *path)
{
    char** list = FileSystem::enumerateFiles(path);
    
    std::vector<std::string> filenames;
    Surface tempSurface;
    iXY maxSize(0, 0);
    for(char** file = list; *file != 0; file++) {
        std::string name = path;
        name += *file;
        if(name.find(".til") != std::string::npos) {
            filenames.push_back(name);

            // Get the max image size.
            tempSurface.loadTIL(name.c_str());
            if (maxSize.x < tempSurface.getPix().x) {
                maxSize.x = tempSurface.getPix().x;
            }
            if (maxSize.y < tempSurface.getPix().y) {
                maxSize.y = tempSurface.getPix().y;
            }
        }
    }

    FileSystem::freeList(list);

    std::sort(filenames.begin(), filenames.end());

    // Create the Surface to have the size of the largest image in the
    // diRectory.  All other images will be centered based off the
    // largest size.
    create(maxSize, maxSize.x, filenames.size());

    // Now load in the sorted TIL names.
    for (size_t i = 0; i < filenames.size(); i++) {
        setFrame(i);

        tempSurface.loadTIL(filenames[i].c_str());
        iXY myOffset;
        myOffset = maxSize - tempSurface.getPix();

        fill(Color::black);
        tempSurface.blt(*this, myOffset);
    }

    return 1;
} // end loadAllTILInDirectory

// loadAllBMPInDirectory
//---------------------------------------------------------------------------
int Surface::loadAllBMPInDirectory(const char *path)
{
    char** list = FileSystem::enumerateFiles(path);
    
    std::vector<std::string> filenames;
    Surface tempSurface;
    iXY maxSize(0, 0);
    for(char** file = list; *file != 0; file++) {
        std::string name = path;
        name += *file;
        if(name.find(".bmp") != std::string::npos) {
            filenames.push_back(name);

            // Get the max image size.
            tempSurface.loadBMP(name.c_str());
            if (maxSize.x < tempSurface.getPix().x) {
                maxSize.x = tempSurface.getPix().x;
            }
            if (maxSize.y < tempSurface.getPix().y) {
                maxSize.y = tempSurface.getPix().y;
            }
        }
    }

    FileSystem::freeList(list);

    std::sort(filenames.begin(), filenames.end());

    // Create the Surface to have the size of the largest image in the
    // diRectory.  All other images will be centered based off the
    // largest size.
    create(maxSize, maxSize.x, filenames.size());

    // Now load in the sorted TIL names.
    for (size_t i = 0; i < filenames.size(); i++) {
        setFrame(i);

        tempSurface.loadBMP(filenames[i].c_str());
        iXY myOffset;
        myOffset = maxSize - tempSurface.getPix();

        fill(Color::black);
        tempSurface.blt(*this, myOffset);
    }

    return 1;
} // end loadAllBMPInDirectory

// initFont
//---------------------------------------------------------------------------
// Purpose: Load all the characters into a surface of 128 frames.  Then the
//          characters can be accesed by changing the frame appropriately.
//---------------------------------------------------------------------------
void initFont()
{
    {
        // Make room for the 128 characters.
        ascii8x8.create(8, 8, 8, 128);

        // NOTE: Make sure the file size is 128 characters.
        char charfilename[] = "pics/chars8x8.raw";

        std::auto_ptr<ReadFile> file (FileSystem::openRead(charfilename));

        for (int y = 0; y < ascii8x8.getPix().y; y++) {
            for (int curChar = 0; curChar < ascii8x8.getFrameCount(); curChar++) {
                ascii8x8.setFrame(curChar);
                int yOffset = y * ascii8x8.getPix().x;

                if (file->read(ascii8x8.getMem() + yOffset, ascii8x8.getPix().x, 1) != 1) {
                    throw Exception("Error while reading font '%s'.",
                                    charfilename);
                }
            }
        }
    }

    {
        // Make room for the 128 characters.
        ascii5x5.create(5, 5, 5, 128);

        // NOTE: Make sure the file size is 128 characters.
        char charfilename[] = "pics/chars5x5.raw";
        
        std::auto_ptr<ReadFile> file (FileSystem::openRead(charfilename));
        
        for (int y = 0; y < ascii5x5.getPix().y; y++) {
            for (int curChar = 0; curChar < ascii5x5.getFrameCount(); curChar++) {
                ascii5x5.setFrame(curChar);
                int yOffset = y * ascii5x5.getPix().x;

                if (file->read(ascii5x5.getMem() + yOffset, ascii5x5.getPix().x, 1) != 1) {
                    throw Exception("error loading font '%s'.", charfilename);
                }
            }
        }
    }

} // Surface::initFont

int Surface::getFontHeight()
{
    return ascii8x8.getPixY();
}

int Surface::getTextLength(const char* text)
{
    return ascii8x8.getPixX() * strlen(text);
}

// bltChar8x8
//---------------------------------------------------------------------------
// Purpose: Blits the specied rom character to the screen at the specified
//          location.
//---------------------------------------------------------------------------
void Surface::bltChar8x8(const iXY &pos, const char &character, const PIX &color) const
{
#ifdef _DEBUG
    if (character > ascii8x8.getFrameCount()) {
        assert(false);
    }
#endif

    ascii8x8.setFrame(character);
    ascii8x8.bltTransColor(*this, pos, color);

} // end Surface::bltChar8x8

// bltChar5x5
//---------------------------------------------------------------------------
void Surface::bltChar5x5(const iXY &pos, const char &character, const PIX &color) const
{
#ifdef _DEBUG
    if (character > ascii5x5.getFrameCount()) {
        assert(false);
    }
#endif

    ascii5x5.setFrame(character);
    ascii5x5.bltTransColor(*this, pos, color);

} // end Surface::bltChar8x8

// bltString
//---------------------------------------------------------------------------
// Purpose: Blits the specified string of text to the screen by making
//          calls to blitChar for each character of the string. Does not
//          handle wrapping.
//---------------------------------------------------------------------------
void Surface::bltString(const iXY &pos, const char *string, const uint8_t &color) const
{
    for (int index = 0; string[index] != 0; index++) {
        // Don't attempt blank spaces.
        if (string[index] == 32) {
            continue;
        }

        bltChar8x8(iXY(pos.x + (index << 3), pos.y), string[index], color);
    }

} // end Surface::bltString

// bltString5x5
//---------------------------------------------------------------------------
// Purpose: Blits the specified string of text to the screen by making
//          calls to blitChar for each character of the string. Does not
//          handle wrapping.
//---------------------------------------------------------------------------
void Surface::bltString5x5(const iXY &pos, const char *string, const uint8_t &color) const
{
    for (int index = 0; string[index] != 0; index++) {
        bltChar5x5(iXY(pos.x + (index * 5), pos.y), string[index], color);
    }

} // end Surface::bltString5x5

// bltStringShadowed
//---------------------------------------------------------------------------
void Surface::bltStringShadowed(const iXY &pos, char const *string, const uint8_t &textColor, const uint8_t &shadowColor) const
{
    for (int index = 0; string[index] != 0; index++) {
        bltChar8x8(pos.x + (index << 3) + 1, pos.y + 1, string[index], shadowColor);
        bltChar8x8(pos.x + (index << 3),     pos.y,     string[index], textColor);
    }

} // end Surface::bltStringShadowed

// bltStringCenter
//---------------------------------------------------------------------------
// Purpose: Blits a string of text and centers it horizontally and vertically
//          on the screen. Does not handle wrapping.
//---------------------------------------------------------------------------
void Surface::bltStringCenter(const char *string, PIX color) const
{
    iXY pos;
    pos.x = (getPix().x - (strlen(string) * ascii8x8.getPixX())) / 2;
    pos.y = (getPix().y - getFontHeight()) / 2;

    bltString(pos, string, color);

} // end Surface::bltStringCenter

// bltStringShadowedCenter
//---------------------------------------------------------------------------
// Purpose: Blits a string of text and centers it horizontally and vertically
//          on the screen. Does not handle wrapping.
//---------------------------------------------------------------------------
void Surface::bltStringShadowedCenter(const char *string, PIX foreground, PIX background) const
{
    iXY pos;
    pos.x = (getPix().x - (strlen(string) * ascii8x8.getPixX())) / 2;
    pos.y = (getPix().y - getFontHeight()) / 2;

    bltStringShadowed(pos, string, foreground, background);

} // end Surface::bltStringShadowedCenter

// bltStringCenteredAtPoint
//---------------------------------------------------------------------------
// Purpose: Blits the string centered at the specified point.
//---------------------------------------------------------------------------
void Surface::bltStringCenteredAtPoint(const iXY &pos, const char *string, const PIX &color) const
{
    iXY destPos;
    destPos.x = pos.x - (strlen(string) * ascii8x8.getPixX()) / 2;
    destPos.y = pos.y - getFontHeight() / 2;

    for (int i = 0; string[i] != 0; i++) {
        bltChar8x8(destPos.x + (i << 3), destPos.y, string[i], color);
    }
}

// bltStringCenteredInRect
//---------------------------------------------------------------------------
// Purpose: Blits the string centered inside the specified rectangle.
//---------------------------------------------------------------------------
void Surface::bltStringCenteredInRect(const iRect &rect, const char *string, const PIX &color) const
{
    int length = strlen(string);

    iXY destPos;
    destPos.x = rect.min.x + (rect.getSizeX() - (length * ascii8x8.getPixX())) / 2;
    destPos.y = rect.min.y + (rect.getSizeY() - getFontHeight()) / 2;

    for (int i = 0; string[i] != 0; i++) {
        bltChar8x8(destPos.x + (i << 3), destPos.y, string[i], color);
    }

} // end Surface::bltStringCenteredInRect

// Blit a string of text with a vertical gradient.
//---------------------------------------------------------------------------
void Surface::bltStringVGradient(const iXY &pos, const char *string, ColorTable &colorTable) const
{
    for (int index = 0; string[index] != 0; index++) {
        bltChar8x8VGradient(iXY(pos.x + (index << 3), pos.y), string[index], colorTable);
    }
}

//---------------------------------------------------------------------------
void Surface::bltChar8x8VGradient(const iXY &pos, const char &character,
                                  ColorTable &colorTable) const
{
#ifdef _DEBUG
    if (character > ascii8x8.getFrameCount()) {
        assert(false);
    }
#endif
    ascii8x8.setFrame(character);
    ascii8x8.bltTransVGradient(*this, pos, colorTable);
}

//---------------------------------------------------------------------------
void Surface::bltTransVGradient(const Surface &dest, iXY min, ColorTable &colorTable) const
{
    assert(getDoesExist());
    assert(dest.getDoesExist());
    assert(this != 0);
    assert(colorTable.getColorCount() > 0);

    // Add in the offset factor.
    min += offset;

    // Trivial clipping rejection - no overlap.
    // Also will jump out immediately if either image has zero size.
    if (min.x >= dest.pix.x) return;
    if (min.y >= dest.pix.y) return;

    iXY max;
    max = min + pix;
    //int	x2 = x1 + pix.x;
    //int	y2 = y1 + pix.y;
    if (max.x <= 0) return;
    if (max.y <= 0) return;

    // Something is overlapping, so we need to verify that both
    // surfaces are valid.
    assert(mem != 0);
    assert(dest.mem != 0);
    if (mem == 0 || dest.mem == 0) return;

    int	pixelsPerRow = pix.x;
    int	numRows      = pix.y;

    PIX	*sPtr	= mem;      // Pointer to source Surface start of memory.
    PIX	*dPtr	= dest.mem; // Pointer to destination Surface start of memory.

    // Check for partial clip, calculate number of pixels
    // per row to copy, and number of rows to copy.  Adjust
    // sPtr and dPtr.

    // CLIP LEFT
    if (min.x < 0) {
        pixelsPerRow +=  min.x; // This will subtract the neg. x value.
        sPtr         += -min.x; // This will move the sPtr to x = 0, from the neg. x.
    }	else {
        dPtr += min.x;
    }

    // CLIP RIGHT
    // This subtracts only the portion hanging over the right edge of the
    // destination Surface
    if (max.x > dest.pix.x) pixelsPerRow -= max.x - dest.pix.x;

    // CLIP TOP
    if (min.y < 0) {
        numRows += min.y;
        sPtr    -= min.y * stride;
    }	else {
        dPtr += min.y * dest.stride;
    }

    // CLIP BOTTOM
    // This subtracts only the portion hanging over the bottom edge of the
    // destination Surface
    if (max.y > dest.pix.y) numRows -= max.y - dest.pix.y;

    // Now, Check to make sure I actually have something
    // to draw.  I should - because I checked for trivial
    // rejection first.  These asserts just make sure
    // my clipping is working...
    if (signed(pixelsPerRow) <= 0) return;
    if (signed(numRows) <= 0) return;
    assert(signed(pixelsPerRow) > 0);
    assert(signed(numRows) > 0);

    int srcAdjustment  = stride      - pixelsPerRow;
    int destAdjustment = dest.stride - pixelsPerRow;

    for (int row = 0; row < numRows; row++) {
        int index = (int)
                    (float(row) / float(numRows) *(colorTable.getColorCount() - 1));
        int color = colorTable[index];

        for (int col = 0; col < pixelsPerRow; col++) {
            if (*sPtr != 0)
                *dPtr = color;
            sPtr++;
            dPtr++;
        }

        sPtr += srcAdjustment;
        dPtr += destAdjustment;
    }
}

// create
//---------------------------------------------------------------------------
void Surface::create(iXY nPix ,int nStride, int nFrameCount)
{
    reset();
    alloc(nPix, true, nStride, nFrameCount);
} // end Surface::create

// nextFrame
//
// Move to the next frame based off time and the fps.  Returns a 0 if it
// was the last frame and 1 otherwise.
//---------------------------------------------------------------------------
int Surface::nextFrame()
{
    curFrame += TimerInterface::getTimeSlice() * fps;

    if (curFrame >= frameCount) {
        curFrame = 0.0;
        return 0;
    }

    setFrame(curFrame);
    return 1;
}

void Surface::loadBMP(const char *fileName, bool needAlloc /* = true */,
                      void *returnPalette /* = 0 */)
{
    BitmapFileHeader file_header;
    BitmapInfoHeader info_header;

    assert(this != 0);

    if (needAlloc) free();

    std::auto_ptr<ReadFile> file (FileSystem::openRead(fileName));

    if(file->read( &file_header, sizeof(BitmapFileHeader), 1) != 1)
        throw Exception("Error reading .bmp file %s", fileName);

    if ( file_header.bfType != 0x4d42 ) // file_header.bfType != "BM"
        throw Exception("%s is not a valid 8-bit BMP file", fileName);

    if(file->read(&info_header, sizeof(BitmapInfoHeader), 1) != 1)
        throw Exception("Error reading .bmp file %s", fileName);

    if ( info_header.biBitCount != 8 )
        throw Exception("%s is not a 8-bit BMP file", fileName);

    if ( info_header.biCompression != BI_RGB )
        throw Exception("%s is not a 8-bit UnCompressed BMP file", fileName);

    if (needAlloc) {

        if (!alloc(info_header.biWidth, info_header.biHeight , false, info_header.biWidth, 1) ) {
            throw Exception("Not enough memory to load BMP image %s", fileName);
        }

    } else {
        // Check and make sure the picture will fit
        if (pix.x < (long) info_header.biWidth|| pix.y < (long) info_header.biHeight )
            throw Exception("Not enough memory to load BMP image %s", fileName);
    }

    file->seek(file_header.bfOffBits);

    if ( (info_header.biWidth % 4) == 0 ) {
        if (file->read(mem, pix.x * pix.y, 1) != 1)
            throw Exception("error while reading bmp image %s", fileName);
    } else {
        int padding = ((info_header.biWidth / 4 + 1) * 4) - info_header.biWidth;

        PIX buffer[10];
        int numRows = pix.y;

        //PIX *sPtr = mem;

        for (int row = 0; row < numRows; row++) {
            if(file->read(mem, pix.x, 1) != 1 ||
               file->read(buffer, padding, 1) != 1)
                throw Exception("error reading file %s.", fileName);
            mem += stride;
        }
    }

    flipVertical();
}

// drawWindowsBorder
//--------------------------------------------------------------------------
void Surface::drawWindowsBorder(iRect rect, PIX light, PIX medium, PIX dark) const
{
    orderCoords(rect);

    // Draw the world view box.
    //drawLookupBorder(Palette::darkGray256.getColorArray());
    drawRect(Color::darkGray);
    return;

    // Draw the border of the window
    drawLine(rect.min.x + 0, rect.min.y + 0, rect.max.x,      rect.min.y ,    light); //N
    drawLine(rect.min.x + 2, rect.max.y - 3, rect.max.x - 2,  rect.max.y - 3, light); //S
    drawLine(rect.max.x - 2, rect.max.y - 2, rect.max.x - 2,  rect.min.y + 2, light); //E
    drawLine(rect.min.x + 0, rect.max.y,     rect.min.x,      rect.min.y + 0, light); //W

    drawLine(rect.min.x + 1, rect.min.y + 1, rect.max.x - 1, rect.min.y + 1, medium); //N
    drawLine(rect.min.x + 1, rect.max.y - 2, rect.max.x - 2, rect.max.y - 2, medium); //S
    //drawLine(rect.max.x - 2, rect.max.y - 1, rect.max.x - 2, rect.min.y + 1, medium); //E
    drawLine(rect.min.x + 1, rect.max.y,     rect.min.x + 1, rect.min.y + 1, medium); //W

    drawLine(rect.min.x + 2, rect.min.y + 2, rect.max.x - 4, rect.min.y + 2, dark); //N
    drawLine(rect.min.x + 0, rect.max.y - 1, rect.max.x,     rect.max.y - 1, dark); //S
    drawLine(rect.max.x - 1, rect.max.y,     rect.max.x - 1, rect.min.y + 0, dark); //E
    drawLine(rect.min.x + 2, rect.max.y - 3, rect.min.x + 2, rect.min.y + 2, dark); //W
} // end Surface::drawWindowsBorder

// bltStringInBox
//--------------------------------------------------------------------------
void Surface::bltStringInBox(const iRect &rect, const char *string, PIX color, int gapSpace, bool drawBox) const
{
    if (drawBox) {
        drawRect(rect, Color::yellow);
    }

    iXY pos(rect.min);

    int  totalLength = strlen(string);
    int  length      = 0;
    int  done        = 0;

    while (length < totalLength - 1) {
        while (string[length] == '\n') {
            pos.x = rect.min.x;
            pos.y += gapSpace;
            length++;
        }

        // Remove any spaces.
        while (string[length] == ' ') {
            pos.x += ascii8x8.getPixX();
            length++;
        }

        char strBuf[256];
        memset(strBuf, 0, sizeof(strBuf));
        int strBufLength = 0;

        while (!isspace(string[length + strBufLength]) && string[length + strBufLength] != '\n') {
            if (string[length + strBufLength] == '\0') {
                done = true;
                break;
            }

            assert(strBufLength < 256);
            strBuf[strBufLength] = string[length + strBufLength];
            strBufLength++;
        }

        strBuf[strBufLength] = '\0';

        if ((int) (pos.x + strlen(strBuf) * ascii8x8.getPixX()) > rect.max.x) {
            pos.x = rect.min.x;
            pos.y += gapSpace;
        }

        bltString(pos, strBuf, color);

        if (done) {
            return;
        }

        pos.x += strlen(strBuf) * ascii8x8.getPixX();

        length += strBufLength;
    }

} // end Surface::bltStringInBox

// mapFromPalette
//--------------------------------------------------------------------------
// Purpose: Maps this image from the specified palette to the current palette.
//--------------------------------------------------------------------------
void Surface::mapFromPalette(const char* oldPalette)
{
    // Load the source palette.
    uint8_t     bestFitArray[256];
    RGBColor sourceColor[256];

    std::auto_ptr<ReadFile> file (FileSystem::openRead(oldPalette));

    for (int i = 0; i < 256; i++) {
        if(file->read(&sourceColor[i], 3, 1) != 1) {
            throw Exception("Error while loading palette '%s'.", oldPalette);
        }
    }

    for (int i = 0; i < 256; i++) {
        bestFitArray[i] = Palette::findNearestColor(sourceColor[i]);
    }

    for (size_t x = 0; x < (size_t) (pix.x * pix.y * frameCount); x++) {
        frame0[x] = bestFitArray[frame0[x]];
    }
} // end Surface::mapFromPalette

// drawBoxCorners
//--------------------------------------------------------------------------
// Purpose: Draws lines in the corners of the surface of the specified length
//          and color.
//--------------------------------------------------------------------------
void Surface::drawBoxCorners(const iRect &rect, int cornerLength, PIX color) const
{
    // Make sure the corner lines are not longer than the rect.
    if (rect.getSizeX() < cornerLength) {
        cornerLength -= cornerLength - rect.getSizeX();
    }

    if (rect.getSizeY() < cornerLength) {
        cornerLength -= cornerLength - rect.getSizeY();
    }

    // Draw the rectangle edge-only selection box.
    // Top-left
    drawHLine(rect.min.x, rect.min.y, rect.min.x + cornerLength, color);
    drawVLine(rect.min.x, rect.min.y, rect.min.y + cornerLength, color);

    // Top-right
    drawHLine(rect.max.x - 1, rect.min.y, rect.max.x - cornerLength, color);
    drawVLine(rect.max.x - 1, rect.min.y, rect.min.y + cornerLength, color);

    // Bottom-right
    drawHLine(rect.max.x, rect.max.y - 1, rect.max.x - cornerLength, color);
    drawVLine(rect.max.x - 1, rect.max.y - 1, rect.max.y - cornerLength, color);

    // Bottom-left
    drawHLine(rect.min.x, rect.max.y - 1, rect.min.x + cornerLength, color);
    drawVLine(rect.min.x, rect.max.y - 1, rect.max.y - cornerLength, color);

} // end Surface::drawBoxCorners

// drawLookupBorder
//--------------------------------------------------------------------------
// Purpose: Colormaps the edges of the surface with the specified table.
//--------------------------------------------------------------------------
void Surface::drawLookupBorder(const PIX table[]) const
{
    iRect r;

    // Top
    r = iRect(0, 0, pix.x - 1, 1);
    bltLookup(r, table);

    // Bottom
    r = iRect(0, pix.y - 1, pix.x, pix.y);
    bltLookup(r, table);

    // Left
    r = iRect(0, 0, 1, pix.y - 1);
    bltLookup(r, table);

    // Right
    r = iRect(pix.x - 1, 0, pix.x, pix.y - 1);
    bltLookup(r, table);
} // end Surface::drawLookupBorder

