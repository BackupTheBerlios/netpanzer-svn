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

#ifdef WIN32
#include <io.h>
#endif

#include "FindFirst.hpp"
#include "Surface.hpp"
#include "UtilInterface.hpp"
#include "cGrowList.hpp"
#include "fXY.hpp"
#include "gapp.hpp"
#include "TimerInterface.hpp"
#include "Span.hpp"
#include "Exception.hpp"

#ifdef MSVC
#pragma pack(1)
#endif

struct PcxHeader
 {
   BYTE   manufacturer;
   BYTE   version;
   BYTE   encoding;
   BYTE   bits_per_pixel;
   WORD   x,y;                    //upper left of image
   WORD   width, height;          //size of the image
   WORD   horz_res;               //horizontal resolution
   WORD   vert_res;               //vertical resolution
   BYTE   ega_palette[48];        //who cares?
   BYTE   reserved;
   BYTE   num_color_planes;
   WORD   bytes_per_line;
   WORD   palette_type;
   BYTE   padding[58];
 } __attribute__((packed));

struct TGAHeader
{
	BYTE imageIDLength;
	PIX colorMapType;
	BYTE imageType;
	WORD colorMapFirstIndex;
	WORD colorMapLength;
	PIX colorMapBitsPerEntry;
	WORD xOrigin;
	WORD yOrigin;
	WORD xPix;
	WORD yPix;
	BYTE bitsPerPixel;
	BYTE imageDescriptor;
} __attribute__((packed)); // end cTGA

class SurfaceHeader
{
public:
	DWORD pixX;
	DWORD pixY;
	DWORD offsetX;
	DWORD offsetY;
	DWORD frameCount;
	float fps;

} __attribute__((packed)); // end SurfaceHeader

struct PIC_HEAD
{
	DWORD xPix;         // Horizontal pixel count.
	DWORD yPix;         // Vertical pixel count.
	DWORD frameCount;    // Number of frames.
} __attribute__((packed));

class BitmapFileHeader
  {
   public:
   WORD    bfType;
   DWORD   bfSize;
   WORD    bfReserved1;
   WORD    bfReserved2;
   DWORD   bfOffBits;
  } __attribute__((packed));

#define BI_RGB      0L
#define BI_RLE8     1L
#define BI_RLE4     2L

class BitmapInfoHeader
 {
  public:
  DWORD  biSize;
  DWORD  biWidth;
  DWORD  biHeight;
  WORD   biPlanes;
  WORD   biBitCount;
  DWORD  biCompression;
  DWORD  biSizeImage;
  DWORD  biXPelsPerMeter;
  DWORD  biYPelsPerMeter;
  DWORD  biClrUsed;
  DWORD  biClrImportant;
 } __attribute__((packed));

class RGBQuad 
 {
  public:
  BYTE    rgbBlue;
  BYTE    rgbGreen;
  BYTE    rgbRed;
  BYTE    rgbReserved;
 } __attribute__((packed));

#ifdef MSVC
#pragma pack()
#endif

PIX Surface::transPix = 0;
bool Surface::screenLocked = true;


Surface screen;
Surface ascii8x8;
Surface ascii5x5;

int Surface::totalSurfaceCount = 0;
int Surface::totalByteCount    = 0;

// Surface
//---------------------------------------------------------------------------
Surface::Surface()
{ 
	assert(isValidPtr(this));

	reset();

	totalSurfaceCount++;
	totalByteCount += sizeof(Surface);

} // end Surface::Surface

//---------------------------------------------------------------------------
Surface::Surface(bool nMyMem)
{
	assert(isValidPtr(this));

	reset();
	myMem     = nMyMem;
	doesExist = true;

	totalSurfaceCount++;
	totalByteCount += sizeof(Surface);

} // end Surface::Surface

// Surface
//---------------------------------------------------------------------------
Surface::Surface(const iXY &nPix, int nStride, int nFrameCount)
{
	assert(isValidPtr(this));

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
	assert(isValidPtr(this));
	reset();

	create(iXY(xPix, yPix), nStride, nFrameCount);

	totalSurfaceCount++;
	totalByteCount += sizeof(Surface);

} // end Surface::Surface

// Surface
//---------------------------------------------------------------------------
Surface::Surface(const Surface &source, const iXY &min, const iXY &max, bool doGrab)
{
	assert(isValidPtr(this));
	assert(source.getDoesExist());

	reset();
	
	if (source.getDoesExist()) doesExist = true;

	if (doGrab)
	{
		grab(source, iRect(min.x, min.y, max.x, max.y), true, 0);
	}	else
		{
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
	assert(isValidPtr(this));

	reset();
	setTo(nFrame0, nPix, nStride, nFrameCount);

	totalSurfaceCount++;
	totalByteCount += sizeof(Surface);

} // end Surface::Surface

// ~Surface
//---------------------------------------------------------------------------
Surface::~Surface() 
{ 
	if ((doesExist != false) && (myMem != false))
	{
		free(); 
	}

	totalSurfaceCount--;
	totalByteCount -= sizeof(Surface);

	assert(totalByteCount >= 0);

} // end Surface::~Surface

//---------------------------------------------------------------------------
void Surface::free()
{
	if (myMem && frame0 != 0)
	{
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
	assert(isValidPtr(this));

	pix	        = 0;
	stride      = 0;
	center      = 0;
	mem         = 0;
	frame0      = 0;
	myMem       = false;
	frameCount  = 0;
	curFrame    = 0;
	fps         = 0;
	offset      = 0;
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
	assert(isValidPtr(this));

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
	assert(isValidPtr(this));

	free();

	Surface::stride = stride ? stride : pix.x;

	DWORD requestedBytes = DWORD(Surface::stride) * DWORD(pix.y) * sizeof(PIX) * DWORD(frameCount);

	if (requestedBytes > 0)
	{
		frame0 = (PIX *) malloc(requestedBytes);
		
		// Add the number of bytes for this surface.
		totalByteCount += requestedBytes;

		if (frame0 == 0)
		{
			if (gottaHaveIt)
			{
				outOfMem(requestedBytes);
				assert(false);
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
	assert(isValidPtr(this));

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
	assert(isValidPtr(this));

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
	assert(isValidPtr(this));

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
	assert(isValidPtr(this));
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
	assert(isValidPtr(this));

	free();
	orderCoords(bounds);
	
	if (!alloc(bounds.getSize(), gottaHaveIt, stride, 1))
	{
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
	assert(screenLocked);
	assert(getDoesExist());
	assert(dest.getDoesExist());
	assert(isValidPtr(this));

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
	if (min.x < 0)
	{
		pixelsPerRow +=  min.x; // This will subtract the neg. x value.
		sPtr         += -min.x; // This will move the sPtr to x = 0, from the neg. x.
	}	else
		{
		dPtr += min.x;
		}

	// CLIP RIGHT
	// This subtracts only the portion hanging over the right edge of the
	// destination Surface
	if (max.x > dest.pix.x) pixelsPerRow -= max.x - dest.pix.x;

	// CLIP TOP
	if (min.y < 0)
	{
		numRows += min.y;
		sPtr    -= min.y * stride;
	}	else
		{
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
	if (stride == pixelsPerRow && dest.stride == pixelsPerRow)
	{
		memcpy(dPtr, sPtr, pixelsPerRow * numRows * sizeof(PIX));
	} else {
		do
		{
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
	assert(screenLocked);
	assert(getDoesExist());
	assert(dest.getDoesExist());
	assert(isValidPtr(this));

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

	PIX	*sRow = mem;      // Pointer to source Surface start of memory.
	PIX	*dRow = dest.mem; // Pointer to destination Surface start of memory.

	// Check for partial clip, calculate number of pixels
	// per row to copy, and number of rows to copy.  Adjust
	// sRow and dRow.

	// CLIP LEFT
	if (min.x < 0)
	{
		pixelsPerRow +=  min.x; // This will subtract the neg. x value.
		sRow         += -min.x; // This will move the sRow to x = 0, from the neg. x.
	} 
	else
	{
		dRow += min.x;
	}

	// CLIP RIGHT
	// This subtracts only the portion hanging over the right edge of the
	// destination Surface
	if (max.x > dest.pix.x) pixelsPerRow -= max.x - dest.pix.x;

	// CLIP TOP
	if (min.y < 0)
	{
		numRows += min.y;
		sRow    -= min.y * stride;
	}
	else
	{
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
	if (signed(pixelsPerRow) <= 0) return;
	if (signed(numRows) <= 0) return;
	assert(signed(pixelsPerRow) > 0);
	assert(signed(numRows) > 0);

#ifndef MSVC
	int srcAdjustment  = stride      - pixelsPerRow;
	int destAdjustment = dest.stride - pixelsPerRow;
#endif
	for (int row = 0; row < numRows; row++)
	{
		// XXX we don't use the assembler stuff when not on msvc
#ifdef MSVC
		bltTransSpan(dRow, sRow, pixelsPerRow);

		sRow += stride;
		dRow += dest.stride;
#else
		for (int col = 0; col < pixelsPerRow; col++)
		{
			if (*sRow != 0) 
				*dRow = *sRow;
			sRow++;
			dRow++;
		}
		
		sRow += srcAdjustment;
		dRow += destAdjustment;
#endif
	}

} // end Surface::bltTrans

// bltTransC
//---------------------------------------------------------------------------
// Purpose: Puts the surface onto the destination using the slowest form of
//          transparency detection (pixel by pixel basis), while performing
//          clipping on the bounds of the object. The non-transparent pixels
//          are blitted in the specified color.
//---------------------------------------------------------------------------
void Surface::bltTransColor(const Surface &dest, iXY min, const BYTE &color) const
{
	assert(screenLocked);
	assert(getDoesExist());
	assert(dest.getDoesExist());
	assert(isValidPtr(this));

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
	if (min.x < 0)
	{
		pixelsPerRow +=  min.x; // This will subtract the neg. x value.
		sPtr         += -min.x; // This will move the sPtr to x = 0, from the neg. x.
	}	else
		{
		dPtr += min.x;
		}

	// CLIP RIGHT
	// This subtracts only the portion hanging over the right edge of the
	// destination Surface
	if (max.x > dest.pix.x) pixelsPerRow -= max.x - dest.pix.x;

	// CLIP TOP
	if (min.y < 0)
	{
		numRows += min.y;
		sPtr    -= min.y * stride;
	}	else
		{
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

	for (int row = 0; row < numRows; row++)
	{
		for (int col = 0; col < pixelsPerRow; col++)
		{
			if (*sPtr != transPix) *dPtr = color;
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
	assert(screenLocked);
	assert(getDoesExist());
	assert(isValidPtr(this));

	// Check for trivial rejection
	if (y < 0 || x2 <= 0 || y >= pix.y || x1 >= pix.x) return;

	assert(mem != 0);
	if (mem == 0) return;

	orderCoords(x1, x2);

	unsigned length = x2 - x1;
	PIX *ptr = mem + y * stride;

	// CLIP LEFT
	if (x1 < 0)
	{
		length += x1;
	}	else
		{
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
	assert(screenLocked);
	assert(getDoesExist());
	assert(isValidPtr(this));

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

	while(width > 0)
	{
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
	for (int i = 0; i < frameCount; i++)
	{
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
	assert(screenLocked);
	assert(getDoesExist());
	assert(isValidPtr(this));

	if (pix.x == 0 || pix.y == 0) return;

	if (pix.x == stride)
	{
		memset(mem, color, pix.x * pix.y * sizeof(PIX));
	}	else
		{
			int	n = pix.y;
			PIX *ptr = mem;
			do
			{
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
	assert(screenLocked);
	assert(getDoesExist());
	assert(isValidPtr(this));

	if (pix == 0) return;

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
	if (bounds.max.x >= pix.x) bounds.max.x = pix.x - 1;
	if (bounds.max.y >= pix.y) bounds.max.y = pix.y - 1;

	iXY diff;
	diff = (bounds.max - bounds.min);

	// Set memory to the top-left pixel of the rectangle.
	PIX	*ptr = mem + bounds.min.y * stride + bounds.min.x;

	for (int y = 0; y < diff.y; y++)
	{
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
	assert(screenLocked);
	assert(getDoesExist());
	assert(isValidPtr(this));

	if (pix == 0) return;

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
	assert(screenLocked);
	assert(getDoesExist());
	assert(isValidPtr(this));

	// Check for horizontal lines
	if (y1 == y2)
	{
		orderCoords(x1, x2);
		drawHLine(x1, y1, x2 + 1, color);
		return;
	}

	// Check for vertical lines
	if (x1 == x2)
	{
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
	if (ydiff < 0)
	{
		ydiff  = -ydiff;
		y_unit = -1;   // Move up the screen
	}

	// Finds out whether the drawLine moves left or right horizontally
	if (xdiff < 0)
	{
		xdiff  = -xdiff;
		x_unit = -1;
	}

	if (xdiff > ydiff)
	{
		int length = xdiff + 1;
		for (int i = 0; i < length;i++)
		{
			putPixel(xOffset, yOffset, color);  // Draw the pixel
			xOffset+=x_unit;
			errorTerm += ydiff;
			if (errorTerm > xdiff)
			{
				errorTerm -= xdiff;
				yOffset+=y_unit;
			}
		}
	}	else
		{
		int	length	= ydiff + 1;

		for (int i = 0; i < length; i++)
		{
			putPixel(xOffset, yOffset, color);  // Draw the pixel
			yOffset+=y_unit;
			errorTerm += xdiff;
			if (errorTerm > 0)
			{
				errorTerm -= ydiff;
				xOffset+=x_unit;
			}
		}
	}
} //end Surface::drawLine

// outOfMem
//---------------------------------------------------------------------------
// Purpose: Displays an out of memory error.
//---------------------------------------------------------------------------
void Surface::outOfMem(size_t requestedBytes)
{
	assert(false);
	
	fprintf(stderr, "ERROR: Surface out of memory.");
	//MessageBox(gapp.hwndApp, "ERROR: Surface out of memory.", "Error", MB_OK);

	exit(1);

} // end Surface::outOfMem

// extractPCX
//---------------------------------------------------------------------------
// Purpose: Extracts PCX images from the specified PCX file.  Given the number
//          of columnsand gapspace of each frame, the number of rows can be
//          automatically calculated.  Gapspace corresponds only to the border
//          pixel width to the right and bottom of the first image.  Therefore,
//          all images but the top row and left-most row are surrounded by the
//          pixel width defined by gapspace.
//---------------------------------------------------------------------------
void Surface::extractPCX(const char *filename, int nCols, int gapSpace)
{
	assert(getDoesExist());
	assert(isValidPtr(this));

	Surface pcx;
	pcx.loadPCX(filename);

	// We are going to extract accross columns, then rows.
	int	col	 = 0;
	int	xPos = 0;
	int	yPos = 0;

	for (int frameNum = 0 ; frameNum < frameCount ; ++frameNum)
	{
		setFrame(frameNum);

		pcx.blt(*this, xPos, yPos);

		if (++col >= nCols)
		{
			col  = 0;
			xPos = 0;
			yPos -= pix.y + gapSpace;
		}	else
			{
				xPos -= pix.x + gapSpace;
			}
	}

	// Reset back to the first frame
	setFrame(0);
} // end Surface::extractPCX

#ifdef MSVC
#pragma pack (1)
#endif
struct PCX_HEADER
{
	char   manufacturer;
	char   version;
	char   encoding;
	char   bitsPerPixel;
	short  xPos;
	short  yPos;
	short  width;
	short  height;
	short  hres;
	short  vres;
	char   egaPalette[48];
	char   reserved;
	char   numColorPlanes;
	short  bytesPerLine;
	short  paletteType;
	char   padding[58];
} __attribute__((packed));
#ifdef MSVC
#pragma pack ()
#endif

// loadPCX
//---------------------------------------------------------------------------
// Purpose: Loads in a single PCX from the specified filename.  Memory for
//          this image will automatically allocated unless otherwise
//          specified.  The PCX palette can be set by sending a palette var
//          into the palette field.
//---------------------------------------------------------------------------
void Surface::loadPCX(const char *fileName, bool needAlloc /* = true */,
	                  void *returnPalette /* = 0 */)
{
	assert(isValidPtr(this));

	if (needAlloc) free();

	// Open the PCX file
	FILE *fp = fopen(fileName,"rb");
	if (fp == 0)
		throw Exception("Unable to open %s", fileName);

	// Read in the header
	PCX_HEADER header;
	fread(&header, sizeof(PCX_HEADER), 1, fp);

	if (ferror(fp))
		throw Exception("Error reading .pcx file %s", fileName);

	if (header.manufacturer != 0x0a || header.version != 5)
		throw Exception("%s is not a valid 8-bit PCX file", fileName);

	if (needAlloc)
	{
		// Allocate room for the PCX, check to see if we need the palette.
		if (!alloc(header.width-header.xPos+1, header.height-header.yPos+1, false, header.width-header.xPos+1, 1))
		{
			throw Exception("Not enough memory to load PCX image %s", fileName);
		}
		}	else
			{
				// Check and make sure the picture will fit
				if (pix.x < header.width || pix.y < header.height)
					throw Exception("Not enough memory to load PCX image %s",
									fileName);
			}
		if (returnPalette != 0)
		{
			// Move to the end of the file and back up to the start of the palette
			fseek(fp, -768L, SEEK_END);

			((Palette *) returnPalette)->read(fp);
		}

		fseek(fp, 128L, SEEK_SET);

	// Decode the picture
	PIX *d = mem;
	DWORD size = DWORD(pix.x) * DWORD(pix.y);
	long unsigned index = 0;
	while (index < size)
	{
		int data = fgetc(fp);				// If it's a run of bytes field.
		if (data < 0) break;
		if ((data & 0xc0) == 0xc0)
		{						// And off the high bits.
			int runLength = data & 0x3f;
			data = fgetc(fp);
			if (data < 0) break;
			while (runLength--) d[index++] = data;	// Run the byte (data).
		}	else
			{
				d[index++] = data;		// Else just store it.
			}
	}
	if (ferror(fp)) 
		throw Exception("Error reading .pcx file %s", fileName);
	fclose(fp);
} // end Surface::loadPCX

// flipHorizontal
//---------------------------------------------------------------------------
// Purpose: Goes through all the frames of the surface and mirrors them
//          horizontally.
//---------------------------------------------------------------------------
void Surface::flipHorizontal()
{
	assert(getDoesExist());
	assert(isValidPtr(this));

	Surface tempSurface(pix, stride, 1);

	for (int frameNum = 0; frameNum < frameCount; frameNum++)
	{

		setFrame(frameNum);
		blt(tempSurface, 0, 0);

		for (int x = 0; x < pix.x; x++)
		{
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
	assert(isValidPtr(this));

	Surface tempSurface(pix, stride, 1);

	for (int frameNum = 0; frameNum < frameCount; frameNum++)
	{
		// This sets the mem pointer of the source Surface
		setFrame(frameNum);

		PIX *sPtr = mem + pix.x * pix.y - pix.x;
		PIX *dPtr = tempSurface.mem;

		for (int y = 0; y < pix.y; y++)
		{
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
	assert(isValidPtr(this));

	// Create a Surface the surface the same size as the source.
	create(source);

	for (int frameNum = 0; frameNum < source.getFrameCount(); frameNum++)
	{
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

	for (int i = 0; i < frameCount; i++)
	{
		setFrame(i);

		if (angle % 360 != 0)
		{
			Surface tempSurface(pix.x, pix.y, stride, 1);

			float angleRadians = -float(angle) / float(180.0 / M_PI);
			float cosAngle     = cos(angleRadians);
			float sinAngle     = sin(angleRadians);

			int index   = 0;

			for (int y = -center.y; y < center.y; y++)
			{
				for (int x = -center.x; x < center.x; x++)
				{
					int xSource = int((x * cosAngle - y * sinAngle) + center.x);
					int ySource = int((y * cosAngle + x * sinAngle) + center.y);

					if ((xSource >= 0) && (xSource < pix.x) && (ySource >= 0) && (ySource < pix.y))
					{
						tempSurface.putPixel(index % pix.x, index / pix.y, getPixel(xSource, ySource));
					} else
					{
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

// loadRaw
//---------------------------------------------------------------------------
// Purpose: Loads a ".raw" file, which is not actually a raw because it
//          has dimension information in the header, so I need to rename
//          eventually.
//---------------------------------------------------------------------------
void Surface::loadRAW(const char *filename, bool needAlloc /* = true */)
{
	assert(isValidPtr(this));

	if (needAlloc) free();

	// Open the TIL file
	FILE *fp = fopen(filename, "rb");
	if (fp == 0)
		throw Exception("ERROR: Unable to load raw %s", filename);

	// Read in the header
	PIC_HEAD head;

	fread(&head, sizeof(PIC_HEAD), 1, fp);

	//LOG(("loadRAW -> picName: %s", fileName));
	//LOG(("head.xPix:          %u", head.xPix));
	//LOG(("head.yPix:          %u", head.yPix));
	//LOG(("head.frameCount:     %u", head.frameCount));
	//LOG(("head.numBytes:      %u", head.xPix*head.yPix*head.frameCount));

	if (ferror(fp))
	{
		////MessageBox(gapp.hwndApp, "Error reading .raw file.", "Shit", MB_OK);
		throw Exception("Error reading .pcx file %s", filename);
	}

	if (needAlloc)
	{
		create(head.xPix, head.yPix, head.xPix, head.frameCount);
	}	else
		{
			// Check and make sure the picture will fit
			if (pix.x < signed(head.xPix) || pix.y < signed(head.yPix) || frameCount < signed(head.frameCount))
			{
				////MessageBox(gapp.hwndApp, "During loadTil, you chose to not allocate the memory for the image, but the was not enough already memory already allocated.", "Shit", MB_OK);
				throw Exception("error during loadTil");
			}
		}

	if (ferror(fp)) 
	{
		throw Exception("Error reading file %s", filename);
	}

	assert(frame0 != 0);
	assert(mem    != 0);
	assert(unsigned(pix.x)  == head.xPix);
	assert(unsigned(pix.y)  == head.yPix);

	// Read in the raw data of the tiles.
	DWORD numBytes = pix.x*pix.y*frameCount;
	fread(mem, numBytes, 1, fp);

	if (ferror(fp))
	{
		throw Exception("Error reading file %s", filename);
	}

	fclose(fp);
} // end Surface::loadRaw

// shade
//---------------------------------------------------------------------------
// Purpose: Shades every other pixel which gies the surface a mesh look.  The
//          default shade color is BLACK.  It can be specified otherwise.
//---------------------------------------------------------------------------
void Surface::shade(iRect bounds, PIX color /* = cBLACK */) const
{
	assert(screenLocked);
	assert(getDoesExist());
	assert(isValidPtr(this));

	assert(bounds.min >= 0);
	assert(bounds.max <= pix);

	//for(int x = bounds.min.x; x < bounds.max.x; x += 2)
	//	for(int y = bounds.min.y; y < bounds.max.y; y++)
	//	{
	//		if (y & 2 == 0) mem[y * screen.pix.x + x + 1] = color;
	//		else mem[y * screen.pix.x + x] = color;
	//	}
    int x, y, which;
    for (y = bounds.min.y; y < bounds.max.y; y++)
    {
      which = (y % 2);
      for (x = bounds.min.x; x < bounds.max.x; x += 2)
          {
            if (which)
                mem[y * screen.pix.x + x + 1] = color;
            else
                mem[y * screen.pix.x + x] = color;
          }
    }

} // end Surface::shade

// scale
//---------------------------------------------------------------------------
// Purpose: Scales all the frames of the surface from the current size to the
//          requested size.
//---------------------------------------------------------------------------
void Surface::scale(const iXY &pix)
{
	assert(getDoesExist());
	assert(isValidPtr(this));

	if (pix.x <= 0) { return; }
	if (pix.y <= 0) { return; }
	if (pix.x == this->pix.x && pix.y == this->pix.y) { return; }

	// Create a temporary surface to scale the image onto.
	Surface tempSurface(pix, pix.x, frameCount);

	iRect r(0, 0, pix.x, pix.y);

	// Go through all the frames of the surface.
	int frame;
	for (frame = 0; frame < tempSurface.getFrameCount(); frame++)
	{
		tempSurface.setFrame(frame);
		Surface::setFrame(frame);

		tempSurface.bltScale(*this, r);
	}

	// Resize the calling surface, then copy all the scaled images on it.
	Surface::resize(pix);

	for (frame = 0; frame < tempSurface.getFrameCount(); frame++)
	{
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
	assert(isValidPtr(this));

	float curOffset  = 0;
	float offsetStep = 100.0 / float(frameCount);

	for (int curFrame = 0; curFrame < frameCount; curFrame++)
	{
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
	assert(isValidPtr(this));

	Surface temp;
	temp.create(pix.x, pix.y, pix.x, 1);

	float angleRadians = (offset * 3.6) / (180.0 / PI);
	float angleStep    = ((PI * 2) * numWaves) / pix.y;
	float amplitude    = (percent * pix.y / 2) / 100.0;

	blt(temp, 0, 0);

	int y = 0;
	for (int offset1 = 0; offset1 < pix.x * pix.y; offset1 += pix.x)
	{
		int offset2 = MAX(MIN(int(((cos(angleRadians) * amplitude + y))), pix.y - 1), 0) * pix.x;
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
	assert(isValidPtr(this));

	float curOffset  = 0;
	float offsetStep = 100.0 / float(frameCount);

	for (int curFrame = 0; curFrame < frameCount; curFrame++)
	{
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
	assert(isValidPtr(this));

	Surface temp;
	temp.create(pix.x, pix.y, pix.x, 1);

	float angleRadians = (offset*3.6) / (180.0 / PI);
	float angleStep    = ((PI * 2) * numWaves) / pix.y;
	float amplitude    = (percent * pix.y / 2) / 100.0;

	blt(temp, 0, 0);

	for (int x = 0; x < pix.x; x++)
	{
		int xOffset = MAX(MIN((int)((cos(angleRadians) * amplitude + x)), pix.x - 1), 0);
		for (int yOffset = 0; yOffset < pix.x * pix.y; yOffset += pix.x)
		{
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
	assert(isValidPtr(this));

	float curOffset  = 0;
	float offsetStep = 100.0 / float(frameCount);

	for (int curFrame = 0; curFrame < frameCount; curFrame++)
	{
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
	assert(isValidPtr(this));

	Surface temp(pix.x, pix.y, pix.x, 1);

	unsigned index     = 0;

	float piTimes2     = float(PI * 2.0);
	float angleRadians = (piTimes2 * percent) / 100.0;
	float maxDist      = sqrt(pix.x * pix.x + pix.y * pix.y);
	float scale        = (piTimes2 * numWaves) / maxDist;

	offset = (int) ((offset * piTimes2) / 100.0);

	blt(temp, 0, 0);

	for (int y = 0; y < pix.y; y++)
	{
		for (int x = 0; x < pix.x; x++)
		{
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
	assert(isValidPtr(this));

	float curOffset  = 0;
	float offsetStep = 100.0 / float(frameCount);

	for (int frame = 0; frame < frameCount; frame++)
	{
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
	assert(isValidPtr(this));

	Surface temp(pix.x, pix.y, pix.x, 1);
	blt(temp, 0, 0);

	float piTimes2      = float(PI * 2.0);
	float waveFrequency = (numWaves * piTimes2) / pix.y;
	float waveOffset    = (offset * numWaves * piTimes2) / 100.0;
	float radius        = (pix.x * percent) / 100.0;

	int index = 0;

	for (int y = 0; y < pix.y; y++)
	{
		int xOffset = int(round(sin(y * waveFrequency + waveOffset) * radius));
		for (int x = 0; x < pix.x; x++)
		{
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
	assert(isValidPtr(this));

	float curOffset  = 0;
	float offsetStep = 100.0 / float(frameCount);

	for (int frame = 0; frame < frameCount; frame++)
	{
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
	assert(isValidPtr(this));

	Surface temp(pix.x, pix.y, pix.x, 1);
	blt(temp, 0, 0);

	float piTimes2      = float(PI * 2.0);
	float waveFrequency = (numWaves * piTimes2) / pix.y;
	float waveOffset    = (offset * numWaves * piTimes2) / 100.0;
	float radius        = (pix.x * percent) / 100.0;

	for (int x = 0; x < pix.x; x++)
	{
		int yOffset = int(round(sin(x * waveFrequency + waveOffset) * radius));

		for (int y = 0; y < pix.y; y++)
		{
			if (yOffset >= 0 && yOffset < pix.y)
			{
				mem[pix.x * y + x] = temp.mem[pix.x * yOffset + x];
			} else
			{
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
	assert(screenLocked);
	assert(getDoesExist());
	assert(isValidPtr(this));

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
	assert(screenLocked);
	assert(getDoesExist());
	assert(source.getDoesExist());
	assert(isValidPtr(this));
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
	if (min.x < 0)
	{
		pixelsPerRow += min.x;
		sRow         -= min.x;
	}	else
		{
			dRow += min.x;
		}

	// CLIP RIGHT
	if (max.x > pix.x) { pixelsPerRow -= max.x - pix.x; }

	// CLIP TOP
	if (min.y < 0)
	{
		numRows += min.y;
		sRow    -= (min.y*source.stride);
	}	else
		{
			dRow += (min.y*stride);
		}

	// CLIP BOTTOM
	if (max.y > pix.y) { numRows -= max.y-pix.y; }

	// Check to make sure the clipping is working.
	if (signed(pixelsPerRow) <= 0) return;
	if (signed(numRows) <= 0) return;

	const BYTE *table = colorTable.getColorArray();

	for (int yCount = 0 ; yCount < numRows ; yCount++)
	{
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
	assert(screenLocked);
	assert(getDoesExist());
	assert(source.getDoesExist());
	assert(isValidPtr(this));
	assert(colorTable.getColorCount() == 256 * 256);

	iXY min = destRect.min + source.offset;
	iXY max = destRect.max + source.offset;

	if (min.x >= pix.x) return;
	if (min.y >= pix.y) return;

	// Something is overlapping, so we need to verify that both
	// surfaces are valid.
	assert(mem        != 0);
	assert(source.mem != 0);

	int pixelsPerRow = max.x-min.x;
	int numRows      = max.y-min.y;

	int srcX1 = 0;
	int srcY = 0;

	// FIXME - if inverted bitmap, then flip x1 and y to right or botom edges

	// Compute source deltas
	int xSrcDelta = int((float(source.pix.x) / float(max.x - min.x)) * 65536.0);
	int ySrcDelta = int((float(source.pix.y) / float(max.y - min.y)) * 65536.0);

	PIX  *dRow = mem;			// Pointer to the destination Surface
	
	// CLIP LEFT
	if (min.x < 0)
	{
		pixelsPerRow += min.x;
		srcX1 -= min.x*xSrcDelta;
	}	else
		{
			dRow += min.x;
		}

	// CLIP RIGHT
	if (max.x > pix.x) { pixelsPerRow -= max.x - pix.x; }

	// CLIP TOP
	if (min.y < 0)
	{
		numRows += min.y;
		srcY   -= min.y*ySrcDelta;
	}	else
		{
			dRow += (min.y*stride);
		}

	// CLIP BOTTOM
	if (max.y > pix.y) { numRows -= max.y-pix.y; }

	// Check to make sure the clipping is working.
	if (pixelsPerRow <= 0) return;
	if (numRows <= 0) return;

	const BYTE *table = colorTable.getColorArray();
	int stepAndDecCount = (xSrcDelta << 16) | 0xffff;
	int stepWholePart = xSrcDelta >> 16;
	int srcX1FracWithCount = (srcX1 << 16) | pixelsPerRow;

	for (int yCount = 0 ; yCount < numRows ; yCount++)
	{
		const PIX *sRow = source.rowPtr(srcY >> 16) + (srcX1 >> 16);

		bltBlendScaleSpan(dRow, sRow, srcX1FracWithCount, stepAndDecCount, stepWholePart, table);

		srcY += ySrcDelta;
		dRow += stride;
	}

} // end Surface::bltBlendScale

void Surface::bltLookup(const iRect &destRect, const PIX table[]) const
{
	assert(screenLocked);
	assert(getDoesExist());
	assert(isValidPtr(this));

	iXY min = destRect.min + offset;

	if (min.x >= pix.x) return;
	if (min.y >= pix.y) return;
	
	iXY max = destRect.max + offset;

	if (max.x <= 0) return;
	if (max.y <= 0) return;
	
	// Clip destination rectangle

	if (min.x < 0) min.x = 0;
	if (min.y < 0) min.y = 0;
	if (max.x > pix.x) max.x = pix.x;
	if (max.y > pix.y) max.y = pix.y;
	
	assert(mem != 0);

	int pixelsPerRow = max.x - min.x;
	int numRows      = max.y - min.y;

	PIX *dRow = mem + min.y*stride + min.x;

	for (int yCount = 0 ; yCount < numRows ; yCount++)
	{
		bltLookupSpan(dRow, pixelsPerRow, table);

		dRow += stride;
	}
}

//---------------------------------------------------------------------------
void Surface::bltScale(const Surface &source, const iRect &destRect) const
{
	assert(screenLocked);
	assert(getDoesExist());
	assert(source.getDoesExist());
	assert(isValidPtr(this));

	iXY min = destRect.min + source.offset;
	iXY max = destRect.max + source.offset;

	if (min.x >= pix.x) return;
	if (min.y >= pix.y) return;

	// Something is overlapping, so we need to verify that both
	// surfaces are valid.
	assert(mem        != 0);
	assert(source.mem != 0);

	int pixelsPerRow = max.x-min.x;
	int numRows      = max.y-min.y;

	int srcX1 = 0;
	int srcY = 0;

	// FIXME - if inverted bitmap, then flip x1 and y to right or botom edges

	// Compute source deltas
	int xSrcDelta = int((float(source.pix.x) / float(max.x - min.x)) * 65536.0);
	int ySrcDelta = int((float(source.pix.y) / float(max.y - min.y)) * 65536.0);

	PIX  *dRow = mem;			// Pointer to the destination Surface
	
	// CLIP LEFT
	if (min.x < 0)
	{
		pixelsPerRow += min.x;
		srcX1 -= min.x*xSrcDelta;
	}	else
		{
			dRow += min.x;
		}

	// CLIP RIGHT
	if (max.x > pix.x) { pixelsPerRow -= max.x - pix.x; }

	// CLIP TOP
	if (min.y < 0)
	{
		numRows += min.y;
		srcY   -= min.y*ySrcDelta;
	}	else
		{
			dRow += (min.y*stride);
		}

	// CLIP BOTTOM
	if (max.y > pix.y) { numRows -= max.y-pix.y; }

	// Check to make sure the clipping is working.
	if (pixelsPerRow <= 0) return;
	if (numRows <= 0) return;

	int stepAndDecCount = (xSrcDelta << 16) | 0xffff;
	int stepWholePart = xSrcDelta >> 16;
	int srcX1FracWithCount = (srcX1 << 16) | pixelsPerRow;

#if 0
	printf("Source: %d %d\n", source.pix.x, source.pix.y);
	printf("DestRect: %d %d - %d %d\n", destRect.min.x, destRect.min.y,
										destRect.max.x, destRect.max.y);
	printf("ScaledVals: %d %d - %d %d %d\n", srcX1, xSrcDelta,
			srcX1FracWithCount, stepAndDecCount, stepWholePart);
#endif
	float xdelta = float(source.pix.x) / float(max.x - min.x);
	for (int yCount = 0 ; yCount < numRows ; yCount++)
	{
		const PIX *sRow = source.rowPtr(srcY >> 16) + (srcX1 >> 16);

		/*
		bltScaleSpan(dRow, sRow, srcX1FracWithCount, stepAndDecCount, stepWholePart);
		*/
		// XXX: WARNING SLOW CODE
		float sPos = 0;
		for(int x=0; x<pixelsPerRow; x++) {
			dRow[x] = sRow[(size_t) sPos];
			sPos += xdelta;
		}

		srcY += ySrcDelta;
		dRow += stride;
	}
}

void Surface::bltScaleTrans(const Surface &source, const iRect &destRect) const {

//00401151   mov         dword ptr [edi],edx
//00401153   add         edi,4
//main_loop_entrance:
//00401156   mov         edx,7EFEFEFFh
//0040115B   mov         eax,dword ptr [ecx]
//0040115D   add         edx,eax
//0040115F   xor         eax,0FFh
//00401162   xor         eax,edx
//00401164   mov         edx,dword ptr [ecx]
//00401166   add         ecx,4
//00401169   test        eax,81010100h
//0040116E   je          main_loop(0x00401151)

}

// blendRect
//---------------------------------------------------------------------------
// Purpose - Blend the specified iRect using one of the specific palette tables.
//---------------------------------------------------------------------------
void Surface::blendRect(iRect bounds, ColorTable &colorTable) const
{
	assert(screenLocked);
	assert(getDoesExist());
	assert(isValidPtr(this));
	assert(Palette::brightness256.getColorCount() == 256);

	if (pix == 0) return;

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
	
	if      (colorTable.getColorCount() == 16)
	{
		shiftFactor = 4;
		minusAmount = 16;
	}
	else if (colorTable.getColorCount() == 32)
	{
		shiftFactor = 3;
		minusAmount = 8;
	}
	else if (colorTable.getColorCount() == 64)
	{
		shiftFactor = 2;
		minusAmount = 4;
	}
	else if (colorTable.getColorCount() == 256)
	{
		shiftFactor = 0;
		minusAmount = 0;
	}
	else
	{
		throw Exception("ERROR: Invalid number of colors for color array.");
	}

	for (int y = 0; y < diff.y; y++)
	{
		for (int x = 0; x < diff.x; x++)
		{
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
	assert(isValidPtr(this));

	// Start the bounds values in the center of the surface.
	iRect bounds;
	bounds.min.x = center.x;
	bounds.max.x = center.x;
	bounds.min.y = center.y;
	bounds.max.y = center.y;

	int num;
	for (num = 0; num < frameCount; num++)
	{
		setFrame(num);
		//LOG(("curFrame:  %d", curFrame));
		//LOG(("frameCount: %d", frameCount));
		//LOG(("pix.x:     %d", pix.x));
		//LOG(("pix.y:     %d", pix.y));

		// Check the x bounds.                       
		for (int y = 0; y < pix.y; y++)
		{
			for (int x = 0; x < pix.x; x++)
			{
				if (getPixel(x, y) != transPix)
				{
					if (x < bounds.min.x)
					{
						bounds.min.x = x;
					}
					if (x > bounds.max.x)
					{
						bounds.max.x = x;
					}
				}
			}
		}

		// Check the y bounds.
		for (int x = 0; x < pix.x; x++)
		{
			for (int y = 0; y < pix.y; y++)
			{
				if (getPixel(x, y) != transPix)
				{
					if (y < bounds.min.y)
					{
						bounds.min.y = y;
					}
					if (y > bounds.max.y)
					{
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
	for (num = 0; num < frameCount; num++)
	{
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

// frac
//---------------------------------------------------------------------------
// Purpose: Recursively creates a random fractal image.
//---------------------------------------------------------------------------
void frac(int *matrix, int stride, int x1, int y1, int x2, int y2, float ruggedness)
{
	assert(matrix != 0);

	int xm, zm;

	assert(x1 <= x2);
	assert(y1 <= y2);
	assert(x1 >= 0);
	assert(y1 >= 0);

	xm = (x1+x2+1)/2;
	zm = (y1+y2+1)/2;

	float tl = matrix[y1 * stride + x1];
	float bl = matrix[y2 * stride + x1];
	float tr = matrix[y1 * stride + x2];
	float br = matrix[y2 * stride + x2];

	// If the rectangle is at least 3 cells tall, check
	// if the middle left cell has been filled in, and if
	// not, then stick in a value
	if (zm < y2)
	{

		// See if the middle left cell has been set
		if (matrix[zm * stride + x1] == -10000)
		{
			// Not filled in - generate a random cell value
			matrix[zm * stride + x1] = (int) (calcY((tl+bl)/2.0, ruggedness,
						y2-y1));
		}

		// Fill in the middle right cell.  We know it hasn't
		// been set because of the order in which we fill in
		// the smaller squares
		if (matrix[zm * stride + x2] != -10000.0) {
			throw Exception("1. matrix[%u, %u] != -10000 - value: %d",
							zm, x2, matrix[zm * stride + x2]);
		}
		if (x1 < x2)
			matrix[zm * stride + x2] = (int) (calcY((tr+br)/2.0, ruggedness,
						y2-y1));
	}

	// Do the same for the top-middle and bottom-middle cells
	if (xm < x2)
	{
		if (matrix[y1 * stride + xm] == -10000.0)
			matrix[y1 * stride + xm] = (int) (calcY((tl+tr)/2.0, ruggedness,
						x2-x1));
		if (matrix[y2 * stride + xm] != -10000.0) 
			throw Exception("2. matrix[%u, %u] != -10000 - value: %d",
							y2, xm, matrix[y2 * stride + xm]);
		if (y1 < y2)
			matrix[y2 * stride + xm] = (int) (calcY((bl+tr)/2.0, ruggedness,
						x2-x1));
	}

	// Fill in the middle cell value.  If our square is at
	// least 3x3, then this cell should have not yet been set
	if (xm < x2 && zm < y2)
	{
		if (matrix[zm * stride + xm] != -10000.0) {
			throw Exception("3. matrix[%u, %u] != -10000 - value: %d", zm, xm, matrix[zm * stride + xm]);
		}
		matrix[zm * stride + xm] = (int) (calcY((tl+tr+bl+br)/4.0, ruggedness,
					(x2-x1)+(y2-y1)));
	}

	// Now recursivly do the smaller squares
	if (xm < x2 || zm < y2)
	{
		frac(matrix, stride, x1, y1, xm, zm, ruggedness);
		frac(matrix, stride, x1, zm, xm, y2, ruggedness);
		frac(matrix, stride, xm, y1, x2, zm, ruggedness);
		frac(matrix, stride, xm, zm, x2, y2, ruggedness);
	}

	// Since we've finished this square, every cell in the square
	// should have been filled in.
	for (int x = x1; x < x2 ; ++x) {
		for (int z = y1; z < y2 ; ++z) {
			if (matrix[z * stride + x] == -10000.0) {
				if (matrix[z * stride + x] != -10000.0) {
					throw Exception("4. matrix[%u, %u] != -10000 - value: %d", z, x, matrix[z * stride + x]);
				}
			}
		}
	}
} // end Surface::frac

// createFractal
//---------------------------------------------------------------------------
// Purpose: Create a fractal image onto the Surface.
//---------------------------------------------------------------------------
void Surface::createFractal(const float &minY, const float &maxY, const float &ruggedness)
{
	assert(getDoesExist());
	assert(isValidPtr(this));

	// Create some temp buffer to generate the values in.
	int *tempBuf = (int *) malloc(stride * pix.y * sizeof(int));
	if (tempBuf == 0) { throw Exception("ERROR: Unable to allocate temp fractal surface.");
	}

	// Initialize everything to some bogus value.
	for (int x = 0; x < stride; x++)
		for (int y = 0; y < pix.y; y++)
			tempBuf[y * stride + x] = -10000;

	//for (int num = 0; num < stride*pix.y; num++)
	//	LOG(("%d tempBuf: %d", num, tempBuf[num]));
	//throw Exception("!EXIT!");;

	// Initialize the corners to some value.
	tempBuf[0]                           = (int) getRand(minY, maxY);
	tempBuf[stride-1]                    = (int) getRand(minY, maxY);
	tempBuf[pix.y-1 * stride-1]          = (int) getRand(minY, maxY);
	tempBuf[pix.y-1 * stride + stride-1] = (int) getRand(minY, maxY);

	// Recursively generate the fractal surface
	frac(tempBuf, stride, 0, 0, stride-1, pix.y-1, ruggedness);

	int yMin = tempBuf[0];
	int yMax = tempBuf[0];

	for (int x = 0; x < stride; x++)
	{
		for (int y = 0; y < pix.y; y++)
		{
			//if (tempBuf[y * stride + x] < 0)   tempBuf[y * stride + x] = 0;//throw Exception("tempBuf[y * stride + x] < 0,   value: %d", tempBuf[y * stride + x]);
			//if (tempBuf[y * stride + x] > 255) tempBuf[y * stride + x] = 255;//throw Exception("tempBuf[y * stride + x] > 255, value: %d", tempBuf[y * stride + x]);
			//LOG(("tempBuf[y * stride + x]: %d", tempBuf[y * stride + x]));
			if (tempBuf[y * stride + x] < yMin) yMin = tempBuf[y * stride + x];
			if (tempBuf[y * stride + x] > yMax) yMax = tempBuf[y * stride + x];
		}
	}

	int   difference = yMax - yMin;
	float ratio      = 255.0/float(difference);

	for (int x = 0; x < stride; x++)
	{
		for (int y = 0; y < pix.y; y++)
		{
			// Push all the min values to sea level (0) if the min is less than 0.
			if (yMin < 0)
			{
				tempBuf[y * stride + x] -= yMin;
			}
			// Move the land to be within the range [0, 255].
			int value = (int) (tempBuf[y * stride + x]*ratio);
			assert(value < 256);
			tempBuf[y * stride + x] = value;
			//LOG(("tempBuf[y * stride + x], value: %d", tempBuf[y * stride + x]));
		}
	}

	fill(Color::black);
	//for (int y = 0; y < pix.y; y++)
	//{
		memcpy(frame0, tempBuf, stride*pix.y);
	//}

	smooth();
	smooth();
	smooth();

} // end Surface::createFractal


#if _MSC_VER > 1000
 #pragma optimize( "", off )
#endif
void Surface::fire(int *dest, int xSize, int ySize)
{

	// XXX no msvc assembler
#ifdef MSVC
	_asm {

		mov edi, [dest]
		mov ecx, [xSize]
		mov ebx, [ySize]

		add  edi,4
		lea  esi,[ecx*4]
		imul ecx,ebx
		sub  ecx,ebx
		sub  ecx,ebx
		sub  ecx,2
	fl1:
		mov  ebx,[edi]
		mov  edx,[edi+esi-4]
		lea  eax,[ebx + edx]
		mov  ebx,[edi+esi+4]
		mov  edx,[edi+esi*2]
		add  eax,ebx
		add  eax,edx
		add  edi,4
		shr  eax,2
		jz   fl2
		dec  eax
	fl2:
		dec  ecx
		mov  [edi+esi-4], eax
		jnz  fl1
	}
#endif
	
}

// smooth
//---------------------------------------------------------------------------
// Purpose: Makes every pixel the average of its four neighbors oolor.
//          This is NOT complete, needs to be finished.
//---------------------------------------------------------------------------
void Surface::smooth()
{
	assert(getDoesExist());
	assert(isValidPtr(this));

	const int XP = pix.x;
	const int ZP = pix.y;

	// First erode the edges
	putPixel( 0,  0, (getPixel(   0,    1)+getPixel(   1,    1)+getPixel( 1,    0))/3); // NW
	putPixel( 0, ZP, (getPixel(   0, ZP-1)+getPixel(   1, ZP-1)+getPixel( 1,   ZP))/3); // SW
	putPixel(XP,  0, (getPixel(XP-1,    0)+getPixel(XP-1,    1)+getPixel(XP,    0))/3); // NE
	putPixel(XP, ZP, (getPixel(XP-1, ZP-1)+getPixel(XP-1,   ZP)+getPixel(XP, ZP-1))/3); // SE

	// NORTH EDGE
	for (int x = 1; x < pix.x; x++)
		putPixel(x, 0, (getPixel(x-1, 1)+getPixel(x, 1)+getPixel(x+1, 1))/3);

	// SOUTH EDGE
	for (int x = 1; x < pix.x; x++)
		putPixel(x, ZP, (getPixel(x-1, ZP-1)+getPixel(x, ZP-1)+getPixel(x+1, ZP-1))/3);

	// WEST EDGE
	for (int y = 1; y < pix.y; y++)
		putPixel(0, y, (getPixel(1, y-1)+getPixel(1, y)+getPixel(1, y+1))/3);

	// EAST EDGE
	for (int y = 1; y < pix.y; y++)
		putPixel(XP, y, (getPixel(XP, y-1)+getPixel(XP, y)+getPixel(XP, y+1))/3);

	// Then erode the inner area
	for (int x = 1; x < pix.x; x++)
		for (int y = 1; y < pix.y; y++)
			putPixel(x, y, (getPixel(x-1, y)+getPixel(x+1, y)+getPixel(x, y-1)+getPixel(x, y+1))/4);
} // end smooth

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
	for (int x = 0; x < pix.x; x++)
	{
		for (int y = 0; y < pix.y; y++)
		{
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

// saveRAW
//---------------------------------------------------------------------------
// Purpose: Save a RAW file of the first frame of the Surface into the 
//          specified file handle.
//---------------------------------------------------------------------------
void Surface::saveRAW(FILE *fp) const
{
	if (fp == 0) return;

	int numBytes = pix.x * pix.y * sizeof(PIX);
	
	if (numBytes <= 0) return;
	if (frame0 == 0) return;

	fwrite(frame0, numBytes, 1, fp);

} // end Surface::saveRAW

// saveRAW
//---------------------------------------------------------------------------
// Purpose: Save a RAW file of the first frame of the Surface into the
//          specified filename.  Returns 0 on error, 1 on success.
//---------------------------------------------------------------------------
int Surface::saveRAW(const char *filename) const
{
	FILE *fp = fopen(filename, "wb");
	if (fp == 0) return 0;

	saveRAW(fp);

	fclose(fp);
	
	return 1;

} // end Surface::saveRAW

// loadTIL
//---------------------------------------------------------------------------
int Surface::loadTIL(String filename)
{
	FILE *fp = fopen((const char *) filename, "rb");
	
	if (fp == 0)
	{
		return 0;
	}

	loadTIL(fp);

	fclose(fp);
	
	return 1;

} // end Surface::loadTIL

// loadTIL
//---------------------------------------------------------------------------
void Surface::loadTIL(FILE *fp)
{
	if (fp == 0) return;

	FletchTileHeader fletchTileHeader;
	fread(&fletchTileHeader, sizeof(FletchTileHeader), 1, fp);

	if (frame0 == 0 || mem == 0 || pix.x != int(fletchTileHeader.xSize) || pix.y != int(fletchTileHeader.ySize))
	{
		create(fletchTileHeader.xSize, fletchTileHeader.ySize, fletchTileHeader.xSize, 1);
	}

	int numBytes = pix.x * pix.y * sizeof(BYTE);
	
	if (numBytes <= 0) return;
	if (mem == 0) {
		throw Exception("ERROR: This should not happen.");
	}

	fread(mem, numBytes, 1, fp);

} // end Surface::loadTIL

// saveTIL
void Surface::saveAllTIL(const char *path)
{
	char strBuf[256];
	int  num = 0;

	for (int i = 0; i < frameCount; i++)
	{
		sprintf(strBuf, "%s/dump%04d.til", path, num);
		setFrame(num);

		FILE *fp = fopen(strBuf, "wb");
		if (fp == 0) { continue; }

		saveTIL(fp);

		fclose(fp);
		
		num++;
	}
} // end Surface::saveTIL

// saveTIL
int Surface::saveTIL(const char *filename)
{
	FILE *fp = fopen(filename, "wb");
	if (fp == 0) return 0;

	saveTIL(fp);

	fclose(fp);
	
	return 1;

} // end Surface::saveTIL

// saveTIL
void Surface::saveTIL(FILE *fp)
{
	if (fp == 0) return;

	FletchTileHeader fletchTileHeader;

	fletchTileHeader.minVer    = 0;
	fletchTileHeader.majVer    = 0;
	fletchTileHeader.headSize  = sizeof(FletchTileHeader);
	fletchTileHeader.xSize     = pix.x;
	fletchTileHeader.ySize     = pix.y;
	fletchTileHeader.frameCount = frameCount;
	
	// Add functions for the average color values.
	fletchTileHeader.avgR      = 0;
	fletchTileHeader.avgG      = 0;
	fletchTileHeader.avgB      = 0;
	fletchTileHeader.avgIndex  = getAverageColor();

	fwrite(&fletchTileHeader, sizeof(FletchTileHeader), 1, fp);

	int numBytes = pix.x * pix.y * sizeof(BYTE);
	
	if (numBytes <= 0) {
		throw Exception("ERROR: Trying to write surface when (numBytes <= 0).");
	}
	if (mem == 0) {
		throw Exception("ERROR: Trying to write surface when (mem == 0).");
	}

	fwrite(mem, numBytes, 1, fp);

} // end Surface::saveTIL

// loadRAW
int Surface::loadRAW(const char *filename, const iXY pix)
{
	FILE *fp = fopen(filename, "rb");
	if (fp == 0) return 0;

	loadRAW(fp, pix);

	fclose(fp);
	
	return 1;

} // end Surface::loadRAW

// loadRAW
void Surface::loadRAW(FILE *fp, const iXY pix)
{
	if (fp == 0) return;
	if (pix.x <= 0) return;
	if (pix.y <= 0) return;

	if (frame0 == 0 || mem == 0 || this->pix.x != pix.x || this->pix.y != pix.y)
	{
		create(pix.x, pix.y, pix.x, 1);
	}

	int numBytes = pix.x * pix.y * sizeof(BYTE);
	
	if (numBytes <= 0) return;
	if (mem == 0) { 
		throw Exception("ERROR: This should not happen.");
	}

	fread(mem, numBytes, 1, fp);

} // end Surface::loadRAW

// setBrightness
void Surface::setBrightness(int percent)
{
	assert(percent >= 0 || percent <= 100);

	for (int x = 0; x < pix.x; x++)
	{
		for (int y = 0; y < pix.y; y++)
		{
			putPixel(x, y, Palette::colorTableDarkenALittle[(getPixel(x, y) * 100) + percent]);
		}
	}

} // end Surface::setBrightness

// bltBrightness
void Surface::bltBrightness(const Surface &dest, const iXY &pos, const float &percent)
{
	assert(screenLocked);
	assert(getDoesExist());
	assert(dest.getDoesExist());
	assert(isValidPtr(this));
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
	if (min.x < 0)
	{
		pixelsPerRow += min.x;
		sRow         -= min.x;
	}	else
		{
			dRow += min.x;
		}

	// CLIP RIGHT
	if (max.x > dest.pix.x) { pixelsPerRow -= max.x - dest.pix.x; }

	// CLIP TOP
	if (min.y < 0)
	{
		numRows += min.y;
		sRow    -= (min.y * stride);
	}	else
		{				    
			dRow += (min.y * dest.stride);
		}

	// CLIP BOTTOM
	if (max.y > dest.pix.y) { numRows -= max.y - dest.pix.y; }

	// Check to make sure the clipping is working.
	//if (signed(pixelsPerRow) <= 0) return;
	//if (signed(numRows) <= 0) return;
	assert(signed(pixelsPerRow) > 0);
	assert(signed(numRows) > 0);

	PIX *d;
	int xCount;

	for (int yCount = 0 ; yCount < numRows ; yCount++)
	{
		const PIX *s = sRow;
		d            = dRow;
		xCount       = pixelsPerRow;

		while (xCount > 0)
		{
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

/*
// bltBrightness
void Surface::bltBrightness(const Surface &dest, const iXY pos, ColorTable &colorTable)
{
	assert(getDoesExist());
	assert(dest.getDoesExist());
	assert(isValidPtr(this));
	assert(colorTable.getColorCount() == 256 * 100);

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
	if (min.x < 0)
	{
		pixelsPerRow += min.x;
		sRow         -= min.x;
	}	else
		{
			dRow += min.x;
		}

	// CLIP RIGHT
	if (max.x > dest.pix.x) { pixelsPerRow -= max.x - dest.pix.x; }

	// CLIP TOP
	if (min.y < 0)
	{
		numRows += min.y;
		sRow    -= (min.y * stride);
	}	else
		{				    
			dRow += (min.y * dest.stride);
		}

	// CLIP BOTTOM
	if (max.y > dest.pix.y) { numRows -= max.y - dest.pix.y; }

	// Check to make sure the clipping is working.
	//if (signed(pixelsPerRow) <= 0) return;
	//if (signed(numRows) <= 0) return;
	assert(signed(pixelsPerRow) > 0);
	assert(signed(numRows) > 0);

	PIX *d;
	int xCount;
	int sc;
	int dc;

	for (int yCount = 0 ; yCount < numRows ; yCount++)
	{
		const PIX *s = sRow;
		d            = dRow;
		xCount       = pixelsPerRow;

		while (xCount > 0)
		{
			sc = *s;
			dc = *d;

			assert(dc + ((Palette::brightness256[sc]) << 8) < 256 * 100);

			*d = colorTable[dc + ((Palette::brightness256[sc]) << 8)];
      
			xCount--;
			s++;
			d++;
		}

		sRow += stride;
		dRow += dest.stride;
	}
} // end bltBrightness*/

static BYTE quickHack[65536];

static void bltLightDarkSpan(int n, PIX *d, const BYTE *i, const PIX *s)
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

	while (n > 0)
	{
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

	BYTE const *rowI = lightTable[0];
	PIX  const *rowS = source.mem;
	PIX        *rowD = mem;

	while (numRows > 0)
	{
		//int xCount = pix.x;
		//BYTE const *i = rowI;
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
	for (int x = 0; x < pix.x; x++)
	{
		for (int y = 0; y < pix.y; y++)
		{
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

	assert(screenLocked);
	assert(getDoesExist());
	assert(dest.getDoesExist());
	assert(isValidPtr(this));

	static BYTE saturateTable[512];

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
	if (min.x < 0)
	{
		pixelsPerRow +=  min.x; // This will subtract the neg. x value.
		sPtr         += -min.x; // This will move the sPtr to x = 0, from the neg. x.
	}	else
		{
		dPtr += min.x;
		}

	// CLIP RIGHT
	// This subtracts only the portion hanging over the right edge of the
	// destination Surface
	if (max.x > dest.pix.x) pixelsPerRow -= max.x - dest.pix.x;

	// CLIP TOP
	if (min.y < 0)
	{
		numRows += min.y;
		sPtr    -= min.y * stride;
	}	else
		{
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
	if (stride == pixelsPerRow && dest.stride == pixelsPerRow)
	{
		pixelsPerRow *= numRows;
		numRows = 1;
	}

	do
	{
		
		// Align to 4-byte dest boundary

		PIX *d = dPtr;
		PIX *s = sPtr;
		int n = pixelsPerRow;
		while (n > 0 && int(d) & 3) {
			*d = saturateTable[int(*d) + int(*s)];
			++d;
			++s;
			--n;
		}
		
		// Do 4-byte chunks

		if (0 && n > 3) {
			// XXX no msvc assembler
#ifdef MSVC
			_asm {

				// load vars into regs

				push ebp
				mov esi, sPtr
				mov edi, dPtr
				mov ebp, n

				xor ebx, ebx
				xor ecx, ecx
			quad:

				mov bl, [esi+2]
				mov cl, [edi+2]

				add esi, 4
				mov al, saturateTable[ebx + ecx]

				mov bl, [esi+3]
				mov cl, [edi+3]

				add edi, 4
				mov ah, saturateTable[ebx + ecx]

				shl eax, 16
				sub ebp, 4

				mov bl, [esi+0]
				mov cl, [edi+0]

				mov al, saturateTable[ebx + ecx]

				mov bl, [esi+1]
				mov cl, [edi+1]

				mov ah, saturateTable[ebx + ecx]

				mov [edi], eax
				ja quad

				// restore vars for the c code that cleans up
				mov eax, ebp
				pop ebp
				mov sPtr, esi
				mov dPtr, edi
				mov n, ebp

			}
#endif
		}
		
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
	char strBuf[256];
	char pathWild[256];

	sprintf(pathWild, "%s*.til", path);

	int imageCount = UtilInterface::getNumFilesInDirectory(pathWild);
	if (imageCount <= 0)
	{
		return 0;
	}

	struct _finddata_t myFile;
	int* hFile;

	_findfirst(pathWild, &myFile);
	
	// Find the dimensions of the first file.
	Surface tempSurface;

	cGrowList <Filename> filenames;
	filenames.setNum(imageCount);

	int curFilename = 0;
	iXY maxSize(0, 0);

    if ((hFile = _findfirst(pathWild, &myFile)) != ((int*) -1))
	{
		do
		{
			sprintf(strBuf, "%s%s", path, myFile.name);
			filenames[curFilename].setName(strBuf);
			curFilename++;

			// Get the max image size.
			if (!tempSurface.loadTIL(strBuf))
			{
				throw Exception("ERROR: This should not happen!");
			}
			if (maxSize.x < tempSurface.getPix().x)
			{
				maxSize.x = tempSurface.getPix().x;
			}
			if (maxSize.y < tempSurface.getPix().y)
			{
				maxSize.y = tempSurface.getPix().y;
			}

		} while (_findnext(hFile, &myFile) == 0);
	}
	_findclose(hFile);

	filenames.sort(FilenameSortFunction);

	// Create the Surface to have the size of the largest image in the
	// diRectory.  All other images will be centered based off the
	// largest size.
	create(maxSize, maxSize.x, imageCount);

	// Now load in the sorted TIL names.
	for (int i = 0; i < imageCount; i++)
	{
		setFrame(i);

		if (!tempSurface.loadTIL(filenames[i].name))
		{
			return 0;
		} else
		{
			iXY myOffset;
			myOffset = maxSize - tempSurface.getPix();

			fill(Color::black);
			tempSurface.blt(*this, myOffset);
		}
	}

	return 1;
} // end loadAllTILInDirectory

// loadAllPCXInDirectory
//---------------------------------------------------------------------------
int Surface::loadAllPCXInDirectory(const char *path)
{
	char strBuf[256];
	char pathWild[256];

	sprintf(pathWild, "%s*.pcx", path);

	int imageCount = UtilInterface::getNumFilesInDirectory(pathWild);
	if (imageCount <= 0)
	{
		return 0;
	}

	struct _finddata_t myFile;
	int* hFile;

	_findfirst(pathWild, &myFile);
	
	// Find the dimensions of the first file.
	Surface tempSurface;

	cGrowList <Filename> filenames;
	filenames.setNum(imageCount);

	int curFilename = 0;
	iXY maxSize(0, 0);

    if ((hFile = _findfirst(pathWild, &myFile)) != ((int*) -1))
	{
		do
		{
			sprintf(strBuf, "%s%s", path, myFile.name);
			filenames[curFilename].setName(strBuf);
			curFilename++;

			// Get the max image size.
			tempSurface.loadPCX(strBuf);

			if (maxSize.x < tempSurface.getPix().x)
			{
				maxSize.x = tempSurface.getPix().x;
			}
			if (maxSize.y < tempSurface.getPix().y)
			{
				maxSize.y = tempSurface.getPix().y;
			}

		} while (_findnext(hFile, &myFile) == 0);
	}
	_findclose(hFile);

	filenames.sort(FilenameSortFunction);

	// Create the Surface to have the size of the largest image in the
	// diRectory.  All other images will be centered based off the
	// largest size.
	create(maxSize, maxSize.x, imageCount);

	// Now load in the sorted TIL names.
	for (int i = 0; i < imageCount; i++)
	{
		setFrame(i);

		tempSurface.loadPCX(filenames[i].name);

		iXY myOffset;
		myOffset = maxSize - tempSurface.getPix();

		fill(Color::black);
		tempSurface.blt(*this, myOffset);
	}

	return 1;
} // end loadAllPCXInDirectory

// loadAllBMPInDirectory
//---------------------------------------------------------------------------
int Surface::loadAllBMPInDirectory(const char *path)
{
	char strBuf[256];
	char pathWild[256];

	sprintf(pathWild, "%s*.bmp", path);

	int imageCount = UtilInterface::getNumFilesInDirectory(pathWild);
	if (imageCount <= 0)
	{
		return 0;
	}

	struct _finddata_t myFile;
	int* hFile;

	_findfirst(pathWild, &myFile);
	
	// Find the dimensions of the first file.
	Surface tempSurface;

	cGrowList <Filename> filenames;
	filenames.setNum(imageCount);

	int curFilename = 0;
	iXY maxSize(0, 0);

    if ((hFile = _findfirst(pathWild, &myFile)) != ((int*) -1))
	{
		do
		{
			sprintf(strBuf, "%s%s", path, myFile.name);
			filenames[curFilename].setName(strBuf);
			curFilename++;

			// Get the max image size.
			tempSurface.loadBMP(strBuf);

			if (maxSize.x < tempSurface.getPix().x)
			{
				maxSize.x = tempSurface.getPix().x;
			}
			if (maxSize.y < tempSurface.getPix().y)
			{
				maxSize.y = tempSurface.getPix().y;
			}

		} while (_findnext(hFile, &myFile) == 0);
	}
	_findclose(hFile);

	filenames.sort(FilenameSortFunction);

	// Create the Surface to have the size of the largest image in the
	// diRectory.  All other images will be centered based off the
	// largest size.
	create(maxSize, maxSize.x, imageCount);

	// Now load in the sorted TIL names.
	for (int i = 0; i < imageCount; i++)
	{
		setFrame(i);

		tempSurface.loadBMP(filenames[i].name);

		iXY myOffset;
		myOffset = maxSize - tempSurface.getPix();

		fill(Color::black);
		tempSurface.blt(*this, myOffset);
	}

	return 1;
} // end loadAllBMPInDirectory

// loadAllRAWInDirectory
//---------------------------------------------------------------------------
int Surface::loadAllRAWInDirectory(const char *path, const iXY &pix)
{
	char strBuf[256];
	char pathWild[256];

	sprintf(pathWild, "%s*.raw", path);

	int imageCount = UtilInterface::getNumFilesInDirectory(pathWild);
	if (imageCount <= 0)
	{
		return 0;
	}

	struct _finddata_t myFile;
	int* hFile;

	_findfirst(pathWild, &myFile);
	
	create(pix.x, pix.y, pix.x, imageCount);

	cGrowList <Filename> filenames;
	filenames.setNum(imageCount);

	int curFilename = 0;

	// Load up all the frames of the smoke.
    if ((hFile = _findfirst(pathWild, &myFile)) != ((int*) -1))
	{
		do
		{
			sprintf(strBuf, "%s%s", path, myFile.name);
			filenames[curFilename].setName(strBuf);
			curFilename++;
		} while (_findnext(hFile, &myFile) == 0);
	}
	_findclose(hFile);

	filenames.sort(FilenameSortFunction);

	// Now load in the sorted RAW names.
	for (int i = 0; i < imageCount; i++)
	{
		setFrame(i);

		if (!loadRAW(filenames[i].name, pix))
		{
			return 0;
		}
	}

	return 1;
} // end loadAllRAWInDirectory

// explode
void Surface::explode(const double &time)
{
    //Duplicate image
    //int[] sourcePixels=pixels;
    //pixels = new int[width * height];
	Surface tempSurface;
	tempSurface.create(pix, pix.x, 1);
	this->blt(tempSurface);
	this->fill(Color::black);

    //int[] zBuffer = new int[width * height];
	int *zBuffer = new int[getArea()];
	if (zBuffer == 0)
	{
		throw Exception("Unable to allocate zBuffer.");
	}
    
	//Random rnd=new Random();
    //Make sure the same sequence of random numbers are returned for each call to explode
    //rnd.setSeed(0);
	srand(0);

    int index = 0;
    
	//This is the y value added because of gravity
    double yOffset = ((time + 1.0f) * (time * 1.0f)) / 60.0f;

    for (int y = -center.y; y < center.y; y++)
	{
    	for (int x = -center.x; x < center.x; x++)
    	{
    		//int c = sourcePixels[index];
			int c = tempSurface.getPixel(x, y);
    		
			//if ((c&0xff000000) != 0)
			if (c != 0)
    		{
    			//Calculate distance from center, or the ‘bomb’
    			//double d = Math.sqrt(x*x+y*y);
    			double d = sqrt(x * x + y * y);

    			//Pixel's z, or depth, value
    			//double z=(rnd.nextDouble()+1)*time/d;
    			double z = (float(rand() % RAND_MAX) / float(rand() % RAND_MAX) + 1) * time / d;

    			//Calculate x and y destination, add perspective
    			int xd = (int)(x * z + x) + center.x;
    			int yd = (int)((y + yOffset) * z + y) + center.y;

                //Calculate the pixels offset in the pixels array
    			int offset = xd + yd * pix.x;
    			
				if (xd >= 0 && xd < pix.x && yd >= 0 && yd < pix.y)
				{
                    
					//Check to see if another pixel blocks this
    				if (zBuffer[offset] <= z)
    				{
    					//The pixel is unblocked, plot and update z, or depth, value
    					//pixels[offset]  = c;
						this->putPixel(xd, yd, c);

    					//zBuffer[offset] = (int)z;
						zBuffer[offset] = (int)z;
    				}
				}
    		}
			index++;
	   }
	}
	delete [] zBuffer;
} // end explode

/*
//---------------------------------------------------------------------------
int Surface::loadTGA(const char *filename)
{
	FILE *fp = fopen(filename, "rb");
	if (fp == 0) return 0;

	loadTGA(fp);

	fclose(fp);
	
	return 1;
} // end loadTGA

class cPixel24Bit
{
	BYTE r;
	BYTE g
	BYTE b;
	BYTE v;
}; // end cPixel24Bit

class Surface24Bit
{
	loadTGA();
};

//---------------------------------------------------------------------------
int loadTGA(FILE *file, cPixel24Bit *dest)
{

	assert(isValidPtr(&this));

	// Read in the TGA header.
	cTGAHeader TGAheader;
	fread(&TGAheader, sizeof(TGAheader), 1, fp);

	BYTE        *buffer;

	// Read in the header
	fread(&TGAheader, sizeof(TGAheader), 1, fp);

	// Check for uncompressed/true-color.
	if (TGAheader.imageType != UNCOMPRESSEDtrueCOLOR) 
	{
		throw Exception("ERROR: TGAheader.imageType != UNCOMPRESSEDtrueCOLOR");
	}

	// Check for 24-bits per pixel.
	if (TGAheader.bitsPerPixel != 24) 
	{
		throw Exception("ERROR: TGAheader.bitsPerPixel != 24");
	}

	// Mark the starting point of the image data
	long dataStartPos = ftell(f);

	// Set the size
	iXY size(TGAheader.width, TGAheader.height);

	int xGoofy, yGoofy;

	switch (header.imageDescriptor) 
	{
		case ORIGIN_BOTTOMLEFT:  xGoofy = 0; yGoofy = 1; break;
		case ORIGIN_BOTTOMRIGHT: xGoofy = 1; yGoofy = 1; break;
		case ORIGIN_TOPLEFT:     xGoofy = 0; yGoofy = 0; break;
		case ORIGIN_TOPRIGHT:    xGoofy = 1; yGoofy = 0; break;
		default: return 0;
	}

	if (dest != 0)
	{
		delete [] dest;
	}

	dest  = new SPixel [size.x * size.y * sizeof(cPixel24Bit)];

	// Read in image
	buffer = (BYTE *) malloc(size.x * 3);

	// Top-down or bottom-up?
	int yStart, yEnd, yStep;

	if (yGoofy) 
	{ 
		yStart = ys-1; 
		yEnd = -1; 
		yStep = -1; 
	} else
	{
		yStart = 0; 
		yEnd = ys; 
		yStep = +1; 
	}

	// Left->Right or Right->Left?
	int xStart, xEnd, xStep;

	if (xGoofy)
	{ 
		xStart = xs-1; 
		xEnd = -1; 
		xStep = -1; 
	} else 
	{ 
		xStart = 0; 
		xEnd = xs; 
		xStep = +1; 
	}

	for (int y = yStart; y != yEnd; y += yStep)
	{
		dest = bitmap + (y * xs);

		fread(buffer, xs * 3, 1, f);

		for (int x = xStart; x != xEnd; x += xStep)
		{
			dest->r = buffer[(x * 3) + 2];
			dest->g = buffer[(x * 3) + 1];
			dest->b = buffer[(x * 3) + 0];
			dest->v = 0;

			++dest;
		}
	}

	free(buffer);

	cornerX1 = 0;
	cornerY1 = 0;
	cornerX2 = xs - 1;
	cornerY2 = ys - 1;

	// Return success!
	return(1);
} // loadTGA
*/

// Surface::saveRAW
//---------------------------------------------------------------------------
// Purpose: Dumps an incrementally numbered RAW file of the Surface.
//---------------------------------------------------------------------------
bool Surface::saveRAW()
{
	char strBuf[256];
	int  num = 0;

	// Incrementally create screen shots.
	for(;;)
	{
		sprintf(strBuf, "shot%04d.raw", num);

		if (UtilInterface::getFileSize(strBuf) <= 0)
		{
			if (!saveRAW(strBuf))
			{
				return false;
				//sprintf(progressBuf, "Error saving screen shot: %d.raw", num);
				//progressView->scrollAndUpdate(progressBuf);
				//break;
			}

			//sprintf(progressBuf, "Screen shot saved(%d, %d): %d.raw", pix.x, pix.y, num);
			//progressView->scrollAndUpdate(progressBuf);
			//break;
			return true;
		} else num++;
	}
	return true;
} // end Surface::saveRAW

// convertBW
//--------------------------------------------------------------------------
// Purpose: Converts the image to the best black and white representation it 
//          can.
//--------------------------------------------------------------------------
void Surface::convertBW()
{
	assert(Palette::gray256.getColorCount() == 256);

	for (int x = 0; x < pix.x; x++)
	{
		for (int y = 0; y < pix.y; y++)
		{
			putPixel(x, y, Palette::gray256[getPixel(x, y)]);
		}
	}
} // end Surface::convertBW

//--------------------------------------------------------------------------
void Surface::drawPalette()
{
	iXY  pos;
	BYTE color = 0;

	iXY dimension(SCREEN_XPIX / 16, SCREEN_YPIX / 16);

	for (int y = 0; y < SCREEN_YPIX; y += dimension.y)
	{
		for (int x = 0; x < SCREEN_XPIX; x += dimension.x)
		{
			fillRect(x, y, x + dimension.x, y + dimension.y, color);
			color++;
		}
	}
}

//--------------------------------------------------------------------------
void fireByte(BYTE *dest, int xSize, int ySize)
{

	// XXX no msvc assembler
#ifdef MSVC	
	_asm {

		mov edi, [dest]
		mov ecx, [xSize]
		mov ebx, [ySize]

		inc  edi
		mov  esi,ecx
		imul ecx,ebx
		sub  ecx,ebx
		sub  ecx,ebx
		sub  ecx,2
		xor  ebx,ebx
		xor  edx,edx
	fl1:
		mov  bl,[edi]
		mov  dl,[edi+esi-1]
		lea  eax, [ebx + edx]
		mov  bl,[edi+esi+1]
		mov  dl,[edi+esi*2]
		add  eax,ebx
		add  eax,edx
		inc  edi
		shr  eax,2
		jz   fl2
		dec  al
	fl2:
		dec  ecx
		mov  [edi+esi-1], al
		jnz  fl1
	}
#endif

}

//--------------------------------------------------------------------------
void Surface::erode()
{
	fireByte(frame0, pix.x, pix.y);
}

//--------------------------------------------------------------------------
void Surface::setToColorTable(const ColorTable &source)
{
	//assert(source.getColorCount() == 256);

	for (int x = 0; x < pix.x; x++)
	{
		for (int y = 0; y < pix.y; y++)
		{
			putPixel(x, y, source[getPixel(x, y)]);
		}
	}
}


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

	FILE *fp = fopen(charfilename, "rb");
	if (fp == 0)
		throw Exception("couldn't load font '%s'.", charfilename);

	for (int y = 0; y < ascii8x8.getPix().y; y++)
	{
		for (int curChar = 0; curChar < ascii8x8.getFrameCount(); curChar++)
		{
			ascii8x8.setFrame(curChar);
			int yOffset = y * ascii8x8.getPix().x;

			fread(ascii8x8.getMem() + yOffset, ascii8x8.getPix().x, 1, fp);
		}
	}

	fclose(fp);
	}

	{
	// Make room for the 128 characters.
	ascii5x5.create(5, 5, 5, 128);

	// NOTE: Make sure the file size is 128 characters.
	char charfilename[] = "pics/chars5x5.raw";

	FILE *fp = fopen(charfilename, "rb");
	if (fp == 0)
		throw Exception("couldn't load font '%s'.", charfilename);

	for (int y = 0; y < ascii5x5.getPix().y; y++)
	{
		for (int curChar = 0; curChar < ascii5x5.getFrameCount(); curChar++)
		{
			ascii5x5.setFrame(curChar);
			int yOffset = y * ascii5x5.getPix().x;

			fread(ascii5x5.getMem() + yOffset, ascii5x5.getPix().x, 1, fp);
		}
	}
	fclose(fp);
	}

} // Surface::initFont

// bltChar8x8
//---------------------------------------------------------------------------
// Purpose: Blits the specied rom character to the screen at the specified
//          location.
//---------------------------------------------------------------------------
void Surface::bltChar8x8(const iXY &pos, const char &character, const PIX &color) const
{
	assert(screenLocked);

#ifdef _DEBUG
	if (character > ascii8x8.getFrameCount())
	{
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
	assert(screenLocked);

#ifdef _DEBUG
	if (character > ascii5x5.getFrameCount())
	{
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
void Surface::bltString(const iXY &pos, const char *string, const BYTE &color) const
{
	assert(screenLocked);

	for (int index = 0; string[index] != 0; index++)
	{
		// Don't attempt blank spaces.
		if (string[index] == 32) { continue; }

		bltChar8x8(iXY(pos.x + (index << 3), pos.y), string[index], color);
	}

} // end Surface::bltString

// bltString5x5
//---------------------------------------------------------------------------
// Purpose: Blits the specified string of text to the screen by making
//          calls to blitChar for each character of the string. Does not
//          handle wrapping.
//---------------------------------------------------------------------------
void Surface::bltString5x5(const iXY &pos, const char *string, const BYTE &color) const
{
	assert(screenLocked);

	for (int index = 0; string[index] != 0; index++)
	{
		bltChar5x5(iXY(pos.x + (index * 5), pos.y), string[index], color);
	}

} // end Surface::bltString5x5

// bltStringShadowed
//---------------------------------------------------------------------------
void Surface::bltStringShadowed(const iXY &pos, char const *string, const BYTE &textColor, const BYTE &shadowColor) const
{
	assert(screenLocked);

	for (int index = 0; string[index] != 0; index++)
	{
		bltChar8x8(pos.x + (index << 3) + 1, pos.y + 1, string[index], shadowColor);
		bltChar8x8(pos.x + (index << 3),     pos.y,     string[index], textColor);
	}

} // end Surface::bltStringShadowed

// bltStringSafe
//---------------------------------------------------------------------------
// Purpose: Blit a string of text and makes sure to lock and unlock the DDraw 
//          buffer.  This should be used when you are wanting to display some
//          text outside the make renderloop. Does not handle wrapping.
//---------------------------------------------------------------------------
void Surface::bltStringSafe(const iXY &pos, const char *string, PIX color) const
{
	assert(screenLocked);

} // end Surface::bltStringSafe

// bltStringCenter
//---------------------------------------------------------------------------
// Purpose: Blits a string of text and centers it horizontally and vertically
//          on the screen. Does not handle wrapping.
//---------------------------------------------------------------------------
void Surface::bltStringCenter(const char *string, PIX color) const
{
	assert(screenLocked);

	iXY pos;
	pos.x = (getPix().x - (strlen(string) * CHAR_XPIX)) / 2;
	pos.y = (getPix().y - CHAR_YPIX) / 2;

	bltString(pos, string, color);

} // end Surface::bltStringCenter

// bltStringShadowedCenter
//---------------------------------------------------------------------------
// Purpose: Blits a string of text and centers it horizontally and vertically
//          on the screen. Does not handle wrapping.
//---------------------------------------------------------------------------
void Surface::bltStringShadowedCenter(const char *string, PIX foreground, PIX background) const
{
	assert(screenLocked);

	iXY pos;
	pos.x = (getPix().x - (strlen(string) * CHAR_XPIX)) / 2;
	pos.y = (getPix().y - CHAR_YPIX) / 2;

	bltStringShadowed(pos, string, foreground, background);

} // end Surface::bltStringShadowedCenter

// bltStringCenteredAtPoint
//---------------------------------------------------------------------------
// Purpose: Blits the string centered at the specified point.
//---------------------------------------------------------------------------
void Surface::bltStringCenteredAtPoint(const iXY &pos, const char *string, const PIX &color) const
{
	assert(screenLocked);

	iXY destPos;
	destPos.x = pos.x - (strlen(string) * CHAR_XPIX) / 2;
	destPos.y = pos.y - CHAR_YPIX / 2;
	
	for (int i = 0; string[i] != 0; i++)
	{
		bltChar8x8(destPos.x + (i << 3), destPos.y, string[i], color);
	}
}

// bltStringCenteredInRect
//---------------------------------------------------------------------------
// Purpose: Blits the string centered inside the specified rectangle.
//---------------------------------------------------------------------------
void Surface::bltStringCenteredInRect(const iRect &rect, const char *string, const PIX &color) const
{
	assert(screenLocked);

	int length = strlen(string);

	iXY destPos;
	destPos.x = rect.min.x + (rect.getSizeX() - (length * CHAR_XPIX)) / 2;
	destPos.y = rect.min.y + (rect.getSizeY() - CHAR_YPIX) / 2;
	
	for (int i = 0; string[i] != 0; i++)
	{
		bltChar8x8(destPos.x + (i << 3), destPos.y, string[i], color);
	}

} // end Surface::bltStringCenteredInRect

// Blit a string of text with a vertical gradient.
//---------------------------------------------------------------------------
void Surface::bltStringVGradient(const iXY &pos, const char *string, ColorTable &colorTable) const
{
	assert(screenLocked);

	for (int index = 0; string[index] != 0; index++)
	{
		bltChar8x8VGradient(iXY(pos.x + (index << 3), pos.y), string[index], colorTable);
	}
}

// Blit a string of text with a horizontal gradient.
//---------------------------------------------------------------------------
void Surface::bltStringHGradient(const iXY &pos, const char *string, ColorTable &colorTable) const
{
	assert(screenLocked);

	int xSize = strlen(string) * CHAR_XPIX;

	Surface tempSurface(xSize, CHAR_YPIX, xSize, 1);

	tempSurface.bltString(iXY(0, 0), string, Color::white);

	tempSurface.bltTransHGradient(*this, pos, colorTable);
}

//---------------------------------------------------------------------------
void Surface::bltChar8x8VGradient(const iXY &pos, const char &character, ColorTable &colorTable) const
{
	assert(screenLocked);

#ifdef _DEBUG
	if (character > ascii8x8.getFrameCount())
	{
		assert(false);
	}
#endif

	ascii8x8.setFrame(character);
	ascii8x8.bltTransVGradient(*this, pos, colorTable);
}

//---------------------------------------------------------------------------
void Surface::bltTransVGradient(const Surface &dest, iXY min, ColorTable &colorTable) const
{
	assert(screenLocked);
	assert(getDoesExist());
	assert(dest.getDoesExist());
	assert(isValidPtr(this));
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
	if (min.x < 0)
	{
		pixelsPerRow +=  min.x; // This will subtract the neg. x value.
		sPtr         += -min.x; // This will move the sPtr to x = 0, from the neg. x.
	}	else
		{
		dPtr += min.x;
		}

	// CLIP RIGHT
	// This subtracts only the portion hanging over the right edge of the
	// destination Surface
	if (max.x > dest.pix.x) pixelsPerRow -= max.x - dest.pix.x;

	// CLIP TOP
	if (min.y < 0)
	{
		numRows += min.y;
		sPtr    -= min.y * stride;
	}	else
		{
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

	for (int row = 0; row < numRows; row++)
	{
		int index = (int) 
			(float(row) / float(numRows) *(colorTable.getColorCount() - 1));
		int color = colorTable[index];

		for (int col = 0; col < pixelsPerRow; col++)
		{
			if (*sPtr != transPix) *dPtr = color;
			sPtr++;
			dPtr++;
		}

		sPtr += srcAdjustment;
		dPtr += destAdjustment;
	}
}

//---------------------------------------------------------------------------
void Surface::bltTransHGradient(const Surface &dest, const iXY &pos, ColorTable &colorTable) const
{
	assert(screenLocked);

}

//---------------------------------------------------------------------------
int Surface::savePCX( Palette &pal )
{
        char strBuf[256];
        int  num = 0;

        // Incrementally create screen shots.
        for(;;)
        {
                sprintf(strBuf, "shot%04d.pcx", num);

                if (UtilInterface::getFileSize(strBuf) <= 0)
                {
              if (!savePCX(strBuf, pal))
                        {
                     return false;
                        }
                        return true;
                } else num++;
        }
        return true;

 }

//---------------------------------------------------------------------------
inline long pcx_putc( unsigned long c, unsigned long n, FILE *fp )
{
        if ( (n > 1) || ( (c & 0xc0) == 0xc0 ) )
        {  while ( n > 0x3f )
              if ( pcx_putc( c, 0x3f, fp ) )
                 return ( 1 );
              else n -= 0x3f;

           if ( !n )
              return (0);

           if ( fputc( 0xc0 | n, fp ) == EOF )
              return ( 1 );
        };

        if ( fputc( c, fp ) == EOF )
           return ( 1 );

        return (0);
}

//---------------------------------------------------------------------------
inline long pcx_xputc( long c, FILE *fp )
{
        long i;
        static long csave = -1, n = -1;

        if ( c == -1 )
        {  if ( csave != -1 )
              if ( pcx_putc( csave, n, fp ) )
                 return ( 1 );      
           csave = n = -1;
           return (0);
        };

        if ( c == csave )
        {  n++;
           return (0);
        };

        if ( csave != -1 )
        {  
		   // XXX is this = intended here? (I added the braces to avoid the
		   // warning now...)
		   if ( (i = pcx_putc( csave, n, fp )) )
              return i;
           csave = n = -1;
        };

        csave = c;
        n = 1;
        return (0);
}

//---------------------------------------------------------------------------
void Surface::savePCX(FILE *fp, Palette &pal )
{
	PcxHeader header;

	header.manufacturer = 10;
	header.version = 5;
	header.encoding = 1;
	header.bits_per_pixel = 8;
	header.x = 0;
	header.y = 0;                        //upper left of image
	header.width = getPix().x - 1;
	header.height = getPix().y - 1;      //size of the image
	header.horz_res = 150;               //horizontal resolution
	header.vert_res = 150;               //vertical resolution
	header.reserved = 0;
	header.num_color_planes = 1;
	header.bytes_per_line = getPix().x;
	header.palette_type = 1; 

	memset( header.padding, 0, 58 );
    fwrite( &header, 1, sizeof( PcxHeader ), fp );

	//int h_res = header.bytes_per_line;
	//int v_res = (header.height - header.x) + 2;

	int h_res = getPix().x;
	int v_res = getPix().y;

	PIX *buff_ptr = mem;

	for ( int y = 0; y < v_res; y++ )
	{
		for ( int x = 0; x < h_res; x++ )
		{  
			if ( pcx_xputc( *buff_ptr++, fp ) )
			return; 
		} 
	 pcx_xputc( -1, fp );
    }

	pcx_xputc( -1, fp );

	unsigned char c = 12; // indicates VGA pallete

	fwrite ( &c, 1, 1, fp );

	//now load the palette into the vga registers
	for(int index = 0; index < 256; index++)
	{
		RGBColor color;

		color = pal[ index ];
    
		c = color.red;
		fwrite ( &c, 1, 1, fp );

		c = color.green;
		fwrite ( &c, 1, 1, fp );

		c = color.blue;
		fwrite ( &c, 1, 1, fp );

	}//end for index that loads the palette

}

//---------------------------------------------------------------------------
int Surface::savePCX( const char *filename, Palette &pal ) 
{
	FILE *fp = fopen(filename, "wb");

	if (fp == 0) return 0;

	savePCX(fp, pal);

	fclose(fp);

	return 1;
}

// convertTransColor
//---------------------------------------------------------------------------
void Surface::convertTransColor(const PIX &color)
{
	for (int i = 0; i < frameCount; i++)
	{
		setFrame(i);
		
		for (int x = 0; x < pix.x; x++)
		{
			for (int y = 0; y < pix.y; y++)
			{
				if (getPixel(x, y) != 0)
				{
					putPixel(x, y, color);
				}
			}
		}
	}
} // end Surface::convertTransColor

// saveSUR
//---------------------------------------------------------------------------
int  Surface::saveSUR(const char *filename)
{
	FILE *fp = fopen(filename, "wb");
	if (fp == 0)	{ return 0; }

	saveSUR(fp);

	fclose(fp);

	return 1;

} // end Surface::saveSUR

// saveSUR
//---------------------------------------------------------------------------
void Surface::saveSUR(FILE *fp)
{
	SurfaceHeader surfaceHeader;

	surfaceHeader.pixX		 = pix.x;
	surfaceHeader.pixY		 = pix.y;
	surfaceHeader.offsetX	 = offset.x;
	surfaceHeader.offsetY	 = offset.y;
	surfaceHeader.frameCount = frameCount;
	surfaceHeader.fps        = fps;

	fwrite(&surfaceHeader, sizeof(SurfaceHeader), 1, fp);

	// Write the actual data...
	for (int i = 0; i < frameCount; i++)
	{
		setFrame(i);

		fwrite(mem, sizeof(PIX), pix.x * pix.y, fp);
	}

} // end Surface::saveSUR

// loadSUR
//---------------------------------------------------------------------------
int  Surface::loadSUR(const char *filename)
{
	FILE *fp = fopen(filename, "rb");
	if (fp == 0)	{ return 0; }

	loadSUR(fp);

	fclose(fp);

	return 1;

} // end Surface::loadSUR

// loadSUR
//---------------------------------------------------------------------------
void Surface::loadSUR(FILE *fp)
{
	SurfaceHeader surfaceHeader;

	fread(&surfaceHeader, sizeof(SurfaceHeader), 1, fp);

	create(surfaceHeader.pixX, surfaceHeader.pixY, surfaceHeader.pixX, surfaceHeader.frameCount);

	setOffset(iXY(surfaceHeader.offsetX, surfaceHeader.offsetY));
	setFPS((int) surfaceHeader.fps);

	// Write the actual data...
	for (int i = 0; i < frameCount; i++)
	{
		setFrame(i);

		fread(mem, sizeof(PIX), pix.x * pix.y, fp);
	}

} // end Surface::loadSUR

// lock
//---------------------------------------------------------------------------
void Surface::lock(PIX *memBuf)
{
	screenLocked = true;
	frame0 = mem = memBuf;
	doesExist    = true;
	

} // end Surface::lock

// unlock
//---------------------------------------------------------------------------
void Surface::unlock()
{
	screenLocked = true;
	frame0 = mem = 0;
	doesExist    = false;


} // end Surface::unlock

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
	
	if (curFrame >= frameCount)
	{
		curFrame = 0.0;
		return 0;
	}
	
	setFrame(curFrame);
	return 1;
}

// drawStatic
//---------------------------------------------------------------------------
// Purpose: Draws static (like a t.v. station at 3AM) on the surface.
//---------------------------------------------------------------------------
void Surface::drawStatic()
{
	for (int x = 0; x < pix.x; x++)
	{
		for (int y = 0; y < pix.y; y++)
		{
			putPixel(x, y, rand() % 256);
		}
	}

	convertBW();

} // end Surface::drawStatic

void Surface::loadBMP(const char *fileName, bool needAlloc /* = true */,
	                  void *returnPalette /* = 0 */)
 {
	BitmapFileHeader file_header;
    BitmapInfoHeader info_header;

    assert(isValidPtr(this));

	if (needAlloc) free();

	FILE *fp = fopen(fileName,"rb");
	if (fp == 0) {
		throw Exception("Unable to open %s", fileName);
	}

    fread( &file_header, sizeof(BitmapFileHeader), 1, fp ); 
 
	if (ferror(fp)) {
		throw Exception("Error reading .bmp file %s", fileName);
	}
    
    if ( file_header.bfType != 0x4d42 ) // file_header.bfType != "BM"
     {
      throw Exception("%s is not a valid 8-bit BMP file", fileName);
     }
  
    fread( &info_header, sizeof(BitmapInfoHeader), 1, fp ); 
 
 	if (ferror(fp)) { throw Exception("Error reading .bmp file %s", fileName); }
  
    if ( info_header.biBitCount != 8 )
     {
      throw Exception("%s is not a 8-bit BMP file", fileName);     
     }
    
    if ( info_header.biCompression != BI_RGB )
     {
      throw Exception("%s is not a 8-bit UnCompressed BMP file", fileName);     
     }

	if (needAlloc)
	 {
      
      if (!alloc(info_header.biWidth, info_header.biHeight , false, info_header.biWidth, 1) )
	   {
	    throw Exception("Not enough memory to load BMP image %s", fileName);
	   }
		
     }
    else
	 {
	  // Check and make sure the picture will fit
	  if (pix.x < (long) info_header.biWidth|| pix.y < (long) info_header.biHeight )
	    throw Exception("Not enough memory to load BMP image %s", fileName);
	 }
   
   fseek(fp, file_header.bfOffBits, SEEK_SET);
  
      
    if ( (info_header.biWidth % 4) == 0 )
     { 
      fread(mem, pix.x * pix.y, 1, fp);  
     }
    else
     { 
      int padding = ((info_header.biWidth / 4 + 1) * 4) - info_header.biWidth; 

	  PIX buffer[10];
      int numRows = pix.y;

      //PIX *sPtr = mem;

      for (int row = 0; row < numRows; row++)
       {
        fread( mem, pix.x, 1, fp );
        fread( buffer, padding, 1, fp);
	    mem += stride;
	   } 
     }

   if (ferror(fp)) { throw Exception("Error reading .bmp file %s", fileName); }
	fclose(fp);
   
   flipVertical();  
  }



void Surface::saveBMP(FILE *fp, Palette &pal )
 {
  BitmapFileHeader file_header;
  BitmapInfoHeader info_header;
  RGBQuad palette[256];

  file_header.bfType = 0x4D42;
  file_header.bfOffBits = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader) 
                           + (sizeof(RGBQuad) * 256);
  file_header.bfSize = file_header.bfOffBits + (pix.x * pix.y);
  file_header.bfReserved1 = 0;
  file_header.bfReserved2 = 0;

  info_header.biSize = sizeof(BitmapInfoHeader);
  info_header.biWidth = pix.x;
  info_header.biHeight = pix.y;
  info_header.biPlanes = 1;
  info_header.biBitCount = 8;
  info_header.biCompression = BI_RGB;
  info_header.biSizeImage= pix.x * pix.y;
  info_header.biXPelsPerMeter = 0;
  info_header.biYPelsPerMeter = 0;
  info_header.biClrUsed = 256;
  info_header.biClrImportant = 256;

  fwrite( &file_header, sizeof(BitmapFileHeader), 1, fp ); 
  fwrite( &info_header, sizeof(BitmapInfoHeader), 1, fp ); 
  
  for(int index = 0; index < 256; index++)
   {
	RGBColor color;

	color = pal[ index ];
    
	palette[index].rgbRed = color.red;
	palette[index].rgbGreen = color.green;
	palette[index].rgbBlue = color.blue;
    palette[index].rgbReserved = 0;
   }//end for index that loads the palette

  fwrite( palette, sizeof(RGBQuad), 256, fp );
 
  flipVertical();

    if ( (info_header.biWidth % 4) == 0 )
     { 
      fwrite( mem, info_header.biSizeImage, 1, fp );
     }
    else
     { 
      int padding = ((info_header.biWidth / 4 + 1) * 4) - info_header.biWidth; 

	  PIX buffer[10];
      int numRows = pix.y;

      //PIX *sPtr = mem;

      for (int row = 0; row < numRows; row++)
       {
        fwrite( mem, pix.x, 1, fp );
        fwrite( buffer, padding, 1, fp);
	    mem += stride;
	   } 
     }


  flipVertical();
 }

// saveBMP
//--------------------------------------------------------------------------
int Surface::saveBMP(Palette &pal)
{
	char strBuf[256];
	int  count = 0;

	// Incrementally create screen shots.
	for(;;)
	{
		sprintf(strBuf, "shot%04d.bmp", count);

		if (UtilInterface::getFileSize(strBuf) <= 0)
		{
			if (!saveBMP(strBuf, pal))
			{
				return false;
			}

			return true;
		} 
		else
		{
			count++;
		}

	}
	return true;

} // end Surface::saveBMP
 
// saveBMP
//--------------------------------------------------------------------------
int Surface::saveBMP( const char *filename, Palette &pal )
{
	FILE *fp = fopen(filename, "wb");

	if (fp == 0) return 0;

	saveBMP(fp, pal);

	fclose(fp);

	return 1;

} // end Surface::saveBMP

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

	// Draw the border of the window
	//drawLine(0,         0,         pix.x,   0,         light); //N
	//drawLine(2,         pix.y - 3, pix.x-2, pix.y - 3, light); //S
	//drawLine(pix.x - 3, pix.y - 2, pix.x-3, 2,         light); //E
	//drawLine(0,         pix.y,     0,       0,         light); //W
	//
	//drawLine(1,         1,         pix.x - 1, 1,         medium); //N
	//drawLine(1,         pix.y - 2, pix.x - 2, pix.y - 2, medium); //S
	//drawLine(pix.x - 2, pix.y - 1, pix.x - 2, 1,         medium); //E
	//drawLine(1,         pix.y,     1,         1,         medium); //W
	//
	//drawLine(2,         2,         pix.x - 4, 2,         dark); //N
	//drawLine(0,         pix.y - 1, pix.x,     pix.y - 1, dark); //S
	//drawLine(pix.x - 1, pix.y,     pix.x - 1, 0,         dark); //E
	//drawLine(2,         pix.y - 3, 2,         2,         dark); //W

} // end Surface::drawWindowsBorder

// bltStringInBox
//--------------------------------------------------------------------------
void Surface::bltStringInBox(const iRect &rect, const char *string, PIX color, int gapSpace, bool drawBox) const
{
	if (drawBox)
	{
		drawRect(rect, Color::yellow);
	}

	iXY pos(rect.min);

	int  totalLength = strlen(string);
	int  length      = 0;
	int  done        = 0;

	while (length < totalLength - 1)
	{
		while (string[length] == '\n')
		{
			pos.x = rect.min.x;
			pos.y += gapSpace;
			length++;
		}

		// Remove any spaces.
		while (string[length] == ' ')
		{
			pos.x += CHAR_XPIX;
			length++;
		}

		char strBuf[256];
		memset(strBuf, 0, sizeof(strBuf));
		int strBufLength = 0;

		while (!isspace(string[length + strBufLength]) && string[length + strBufLength] != '\n')
		{
			if (string[length + strBufLength] == '\0')
			{
				done = true;
				break;
			}

			assert(strBufLength < 256);
			strBuf[strBufLength] = string[length + strBufLength];
			strBufLength++;
		}
		
		strBuf[strBufLength] = '\0';

		if ((int) (pos.x + strlen(strBuf) * CHAR_XPIX) > rect.max.x)
		{
			pos.x = rect.min.x;
			pos.y += gapSpace;
		}

		bltString(pos, strBuf, color);

		if (done)
		{
			return;
		}

		pos.x += strlen(strBuf) * CHAR_XPIX;

		length += strBufLength;
	}

} // end Surface::bltStringInBox

// mapFromPalette
//--------------------------------------------------------------------------
// Purpose: Maps this image from the specified palette to the current palette.
//--------------------------------------------------------------------------
void Surface::mapFromPalette(String oldPalette)
{
	// Load the source palette.
	BYTE     bestFitArray[256];
	RGBColor sourceColor[256];

	FILE *fp;

	if ((fp = fopen(oldPalette, "rb")) == 0)
	{
		throw Exception("Unable to open palette file: %s", (const char *) oldPalette);
	}

	{for (int i = 0; i < 256; i++)
	{
	    fread(&sourceColor[i], 3, 1, fp);
	}}

	{for (int i = 0; i < 256; i++)
	{
		bestFitArray[i] = Palette::findNearestColor(sourceColor[i]);
	}}

	for (int x = 0; x < pix.x; x++)
	{
		for (int y = 0; y < pix.y; y++)
		{
			putPixel(x, y, bestFitArray[getPixel(x, y)]);
		}
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
	if (rect.getSizeX() < cornerLength)
	{
		cornerLength -= cornerLength - rect.getSizeX();
	}

	if (rect.getSizeY() < cornerLength)
	{
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
