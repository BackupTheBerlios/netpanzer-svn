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

#include "Util/Log.hpp"
#include "Util/FileSystem.hpp"
#include "Util/UtilInterface.hpp"
#include "Types/fXY.hpp"
#include "Util/TimerInterface.hpp"
#include "Util/Exception.hpp"
#include "Palette.hpp"
#include "Surface.hpp"
extern "C"
{
    SDL_Surface *IMG_LoadPNG_RW(SDL_RWops *src);
}

using std::swap;
using std::min;
using std::max;

extern char staticFont[8192];
#define FONT_HEIGHT 8
#define FONT_WIDTH 8
#define FONT_MAXCHAR 127

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

int Surface::totalSurfaceCount = 0;
int Surface::totalByteCount    = 0;
SDL_Surface * Surface::blackScreen = 0;

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
Surface::Surface(unsigned int w, unsigned int h, unsigned int nframes)
{
    reset();
    
    alloc( w, h, nframes, 32);

    totalSurfaceCount++;
    totalByteCount += sizeof(Surface);
} // end Surface::Surface

// ~Surface
//---------------------------------------------------------------------------
Surface::~Surface()
{
    freeFrames();

    totalSurfaceCount--;
    assert(totalSurfaceCount >= 0);

    totalByteCount -= sizeof(Surface);
    assert(totalByteCount >= 0);
} // end Surface::~Surface

void
Surface::freeFrames()
{
	for ( unsigned int n = 0; n < frames.size(); ++n )
	{
		SDL_FreeSurface(frames[n]);
	}
	frames.clear();
	cur_frame = 0;
	mem = 0;
}

// reset
//---------------------------------------------------------------------------
void Surface::reset()
{
    twidth      = 0;
    theight     = 0;
    tpitch      = 0;
    
    mem         = 0;
    cur_frame   = 0;
    curFrame    = 0;
    fps         = 0;
    offset.zero();
} // end Surface::reset

// setOffsetCenter
//---------------------------------------------------------------------------
// Purpose: Set the offset to the center of the image.
//---------------------------------------------------------------------------
void Surface::setOffsetCenter()
{
    assert(getDoesExist());
    assert(this != 0);

    offset = iXY(-(getWidth()>>1), -(getHeight()>>1));

} // end Surface::setOffsetCenter

// alloc
//---------------------------------------------------------------------------
// Purpose: Allocates memory for the surface, while setting the pix.x, pix.y,
//          xCenter, yCenter, and stride.  If successful, true is returned,
//          otherwise false is returned.
//---------------------------------------------------------------------------
void
Surface::alloc(unsigned int w, unsigned int h, int nframes, int bpp)
{
    assert(this != 0);

    freeFrames();


    frames.resize(nframes);
    for ( int n = 0; n < nframes; ++n )
    {
        frames[n] = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, bpp, 0, 0, 0, 0);
        if ( bpp == 8 )
        {
            SDL_SetColors(frames[n], Palette::color, 0, 256);
        }
    }

    cur_frame = frames[0];
    twidth = w;
    theight= h;
    tpitch = frames[0]->pitch;
    Surface::mem        = (Uint8*)cur_frame->pixels;
} // end Surface::alloc

// create
//---------------------------------------------------------------------------
void
Surface::create(unsigned int w, unsigned int h, unsigned int nframes)
{
    //reset();
    alloc( w, h, nframes, 32);
} // end Surface::create

void
Surface::linkData(Surface &other)
{
	freeFrames();
	frames = other.frames;
	cur_frame = other.cur_frame;
	curFrame = other.curFrame;
	fps = other.fps;
	mem = other.mem;
	theight = other.theight;
	tpitch = other.tpitch;
	twidth = other.twidth;

	for ( unsigned int n = 0; n < frames.size(); ++n )
	{
		++(frames[n]->refcount);
	}
}

void
Surface::createShadow(Surface &other)
{
    freeFrames();
    frames.resize(other.frames.size());
    theight = other.theight;
    twidth = other.twidth;
    tpitch = other.frames[0]->pitch;
    fps = other.fps;
    curFrame = 0;

    for ( unsigned int n = 0; n < frames.size(); ++n )
    {
        frames[n] = SDL_CreateRGBSurface(SDL_SWSURFACE, twidth, theight, 32, 0, 0, 0, 0);
        SDL_BlitSurface(other.frames[n],0,frames[n],0);
        Uint32 *src = (Uint32*)frames[n]->pixels;
        Uint32 *dst = (Uint32*)frames[n]->pixels;
        Uint32 *final = (Uint32*)((Uint8*)src + (tpitch * theight));
        while ( src < final )
        {
            if ( *src )
            {
                *dst = 0;
            }
            else
            {
                *dst = 0xff00ff;
            }
            ++src;
            ++dst;
        }
        SDL_SetColorKey(frames[n],SDL_SRCCOLORKEY | SDL_RLEACCEL, 0xff00ff);
    }
    cur_frame = frames[0];
    mem = (Uint8*)cur_frame->pixels;
}

// blt
//---------------------------------------------------------------------------
// Purpose: Puts the surface onto the destination while performing clipping
//          on the bounds of the object.
//---------------------------------------------------------------------------
void Surface::blt(Surface &dest, int x, int y) const
{
    SDL_Rect r = { x + offset.x, y+offset.y, 0, 0 };
    SDL_BlitSurface( cur_frame, 0, dest.cur_frame, &r);
} // end Surface::blt

void Surface::bltBlend(Surface &dest, int x, int y, Uint8 alpha) const
{
    SDL_Rect r = { x + offset.x, y+offset.y, 0, 0 };
    SDL_SetAlpha(cur_frame, cur_frame->flags | SDL_SRCALPHA, alpha);
    SDL_BlitSurface( cur_frame, 0, dest.cur_frame, &r);
}

void Surface::bltSolid(Surface &dest, int x, int y) const
{
    SDL_Rect r = { x + offset.x, y+offset.y, 0, 0 };
    SDL_SetAlpha(cur_frame, cur_frame->flags & (~SDL_SRCALPHA), 255);
    SDL_BlitSurface( cur_frame, 0, dest.cur_frame, &r);
}

void
Surface::bltRect(SDL_Rect *sourceRect, Surface &dest, int x, int y)
{
    SDL_Rect r = { x, y, 0, 0 };
    SDL_BlitSurface(cur_frame, sourceRect, dest.cur_frame, &r);
}

// bltTrans
//---------------------------------------------------------------------------
// Purpose: Puts the surface onto the destination using the slowest form of
//          transparency detection (pixel by pixel basis), while performing
//          clipping on the bounds of the object.
//---------------------------------------------------------------------------
void Surface::bltTrans(Surface &dest, int x, int y) const
{
    SDL_Rect r = { x + offset.x, y+offset.y, 0, 0 };
    SDL_SetColorKey( cur_frame, SDL_SRCCOLORKEY, 0);
    SDL_BlitSurface( cur_frame, 0, dest.cur_frame, &r);
} // end Surface::bltTrans

// drawHLine
//---------------------------------------------------------------------------
// Purpose: Draws a horizontal drawLine.
//---------------------------------------------------------------------------
void Surface::drawHLine(int x1, int y, int x2, const IntColor color)
{
    orderCoords(x1, x2);
    if ( y < 0 || x2 <= 0 || y >= (int)getHeight() || x1 >= (int)getWidth() )
    {
		return;
    }

    SDL_Rect r = { x1, y, x2-x1, 1 };
    SDL_FillRect(cur_frame, &r, color);

} // end Surface::drawHLine

// drawVLine
//---------------------------------------------------------------------------
// Purpose: Draws a vertical drawLine.
//---------------------------------------------------------------------------
void Surface::drawVLine(int x, int y1, int y2, const IntColor color)
{
    orderCoords(y1, y2);
    if ( x < 0 || y2 <= 0 || x >= (int)getWidth() || y1 >= (int)getHeight() )
    {
    	return;
    }

    SDL_Rect r = { x, y1, 1, y2-y1 };
    SDL_FillRect(cur_frame, &r, color);
} // end Surface::drawVLine

// fill
//---------------------------------------------------------------------------
// Purpose: Fills the Surface will the specified color.
//---------------------------------------------------------------------------
void Surface::fill(const IntColor color)
{
	SDL_Rect r = { 0, 0, getWidth(), getHeight() };
	SDL_FillRect(cur_frame,&r, color);
} // end Surface::fill

// fillRect
//---------------------------------------------------------------------------
// Purpose: Fills the specified rectangle in the calling Surface with the
//          specified color.
//---------------------------------------------------------------------------
void Surface::fillRect(iRect bounds, const IntColor color)
{
	SDL_Rect r = { bounds.min.x, bounds.min.y, bounds.getSizeX(), bounds.getSizeY() };
	SDL_FillRect(cur_frame,&r, color);
} // end Surface::fillRect

// drawRect
//---------------------------------------------------------------------------
// Purpose: Draws a rectagle in the specified color on the calling Surface.
//---------------------------------------------------------------------------
void Surface::drawRect(iRect bounds, const IntColor color)
{
    if ( !getWidth() || !getHeight() )
	{
    	return;
	}

    orderCoords(bounds);

    // Check for trivial rejection
    if      (bounds.max.x <  0)     return;
    else if (bounds.max.y <  0)     return;
    else if (bounds.min.x >= (int)getWidth()) return;
    else if (bounds.min.y >= (int)getHeight()) return;

    drawHLine(bounds.min.x, bounds.min.y, bounds.max.x,   color);
    drawHLine(bounds.min.x, bounds.max.y, bounds.max.x+1, color);
    drawVLine(bounds.min.x, bounds.min.y, bounds.max.y,   color);
    drawVLine(bounds.max.x, bounds.min.y, bounds.max.y,   color);
} // end Surface::drawRect

// drawLine
//---------------------------------------------------------------------------
// Purpose: Draws a drawLine with any slope.
//---------------------------------------------------------------------------
void Surface::drawLine(int x1, int y1, int x2, int y2, const IntColor color)
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

// copy
//---------------------------------------------------------------------------
// Purpose: Copies the specified number of frames from the source Surface to
//          the calling Surface.
//---------------------------------------------------------------------------
void Surface::copy(const Surface &source)
{
    if(!source.getDoesExist())
        return;
    
    // Create a Surface the surface the same size as the source.
    create(source.getWidth(), source.getHeight(), source.getNumFrames());

    for (unsigned int frameNum = 0; frameNum < source.getNumFrames(); frameNum++)
    {
    	SDL_BlitSurface(source.frames[frameNum], 0, frames[frameNum], 0);
    }
} // end Surface::copy

// scale
//---------------------------------------------------------------------------
// Purpose: Scales all the frames of the surface from the current size to the
//          requested size.
//---------------------------------------------------------------------------
void Surface::scale(unsigned int x, unsigned int y)
{
    if ( !x || !y || (x==getWidth() && y==getHeight()))
        return;
        
    // Create a temporary surface to scale the image onto.
    Surface tempSurface(x, y, getNumFrames());

    iRect r(0, 0, x, y);

    // Go through all the frames of the surface.
    unsigned int frame;
    for (frame = 0; frame < tempSurface.getNumFrames(); frame++)
    {
        tempSurface.setFrame(frame);
        Surface::setFrame(frame);

        tempSurface.bltScale(*this, r);
    }

    freeFrames();
    frames.swap(tempSurface.frames);
    tempSurface.freeFrames();
    setFrame(0);

} // end Surface::scale

void Surface::bltLookup(const iRect &destRect)
{
    SDL_Rect r = { destRect.min.x, destRect.min.y, destRect.getSizeX(), destRect.getSizeY() };
    SDL_BlitSurface(blackScreen, &r, cur_frame, &r);
}

//---------------------------------------------------------------------------
void Surface::bltScale(const Surface &source, const iRect &destRect)
{
    assert(getDoesExist());
    assert(source.getDoesExist());
    assert(this != 0);

    iXY min = destRect.min + source.offset;
    iXY max = destRect.max + source.offset;

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

    Uint32 * dRow = (Uint32*)mem; // Pointer to the destination Surface

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
        dRow = (Uint32*)((Uint8*)(dRow)+(min.y*getPitch()));
    }

    // CLIP BOTTOM
    if (max.y > (int)getHeight()) {
        numRows -= max.y-getHeight();
    }

    // Check to make sure the clipping is working.
    if (pixelsPerRow <= 0) return;
    if (numRows <= 0) return;

    float xdelta = float(source.getWidth()) / float(max.x - min.x);
    for (size_t yCount = 0 ; yCount < numRows ; yCount++) {
        const Uint32 *sRow = source.pixPtr(0, srcY >> 16) + (srcX1 >> 16);

        // XXX: WARNING SLOW CODE
        float sPos = 0;
        for(size_t x=0; x<pixelsPerRow; x++)
        {
            dRow[x] = sRow[(size_t) sPos];
            sPos += xdelta;
        }

        srcY += ySrcDelta;
        dRow = (Uint32*)((Uint8*)(dRow)+getPitch());
    }
}

static inline float getRand(float lo, float hi)
{
    return (float(rand()%10000)/10000.0*(hi-lo))+lo;
}

unsigned int
Surface::getFontHeight()
{
    return FONT_HEIGHT;
}

int Surface::getTextLength(const char* text)
{
    return FONT_WIDTH * strlen(text);
}

// renderText
//---------------------------------------------------------------------------
// Purpose: Renders a string of text to the surface using color and background
//          color. The surface is created, if it was already created and is
//          the exact size needed, clears it and use it, if size is different
//          will delete it and create new one
// Parameters:
//   str     = string to render
//   color   = foreground color
//   bgcolor = background color
//---------------------------------------------------------------------------
void
Surface::renderText(const char *str, IntColor color, IntColor bgcolor)
{
    if ( !str )
        return;
    
    int len = strlen(str);
    if ( !len )
        return;

    unsigned int need_width = len * FONT_WIDTH;
    unsigned int need_height = FONT_HEIGHT;
    
    if ( cur_frame )
    {
        if ( getWidth() != need_width || getHeight() != need_height )
        {
            freeFrames();
            create( need_width, need_height, 1);
        }
    }
    else
    {
        create( need_width, need_height, 1);
    }
    
    fill(bgcolor);
    int x = 0;
    for ( unsigned char c = *str; c; c= *(++str))
	{
		if ( c < 128 && c != 32 )
		{
			bltChar8x8(x, 0, c, color);
		}

		x += FONT_WIDTH;
	}
}

void
Surface::renderShadowedText(const char *str, IntColor color, IntColor bgcolor, IntColor shadowcolor)
{
    if ( !str )
        return;

    int len = strlen(str);
    if ( !len )
        return;

    unsigned int need_width = (len * FONT_WIDTH) + 1;
    unsigned int need_height = FONT_HEIGHT + 1;

    if ( cur_frame )
    {
        if ( getWidth() != need_width || getHeight() != need_height )
        {
            freeFrames();
            create( need_width, need_height, 1);
        }
    }
    else
    {
        create( need_width, need_height, 1);
    }
    
    fill(bgcolor);
	int x = 0;
	for ( unsigned char c = *str; c; c= *(++str))
	{
		if ( c < 128 && c != 32 )
		{
			bltChar8x8(x+1, 1, c, shadowcolor);
			bltChar8x8(x, 0, c, color);
		}

		x += FONT_WIDTH;
	}
}

// bltChar8x8
//---------------------------------------------------------------------------
// Purpose: Blits the specied rom character to the screen at the specified
//          location.
//---------------------------------------------------------------------------
void Surface::bltChar8x8(int x, int y, unsigned char character, const IntColor color)
{
    if (character >= 128 || x <= -8 || x >= (signed int)getWidth()
                         || y <= -8 || y >= (signed int)getHeight()
       )
    {
        return;
    }

    int dolines = FONT_HEIGHT;
    int docolumns = FONT_WIDTH;
    int skipBytes = 0;

    if ( x < 0 )
    {
        skipBytes -= x;
        docolumns += x;
        x = 0;
    }
    else if ( x >= (signed int)(getWidth() - FONT_WIDTH) )
    {
        docolumns -= x - (getWidth()-FONT_WIDTH);
    }

    if ( y < 0 )
    {
        skipBytes -= ( y * FONT_HEIGHT );
        dolines += y;
        y = 0;
    }
    else if ( y >= (signed int)(getHeight() - FONT_HEIGHT) )
    {
        dolines -= y - (getHeight() - FONT_HEIGHT);
    }

    char * fptr = (staticFont + (character * FONT_WIDTH) + skipBytes);
    int skipFontLine = (128*FONT_WIDTH) - docolumns;
    Uint32 * dptr = pixPtr(x, y);
    int n;
    do
    {
        for ( n = 0; n < docolumns; ++n )
        {
            if ( *fptr )
            {
                *dptr = color;
            }
            ++fptr;
            ++dptr;
        }
        fptr += skipFontLine;
        dptr = (Uint32*)((Uint8*)(dptr)+getPitch()-(docolumns*4));
    } while (--dolines);
} // end Surface::bltChar8x8

// bltString
//---------------------------------------------------------------------------
// Purpose: Blits the specified string of text to the screen by making
//          calls to blitChar for each character of the string. Does not
//          handle wrapping.
//---------------------------------------------------------------------------
void Surface::bltString(int x, int y, const char * str, const IntColor color)
{
    for (int index = 0; str[index] != 0; index++) {
        // Don't attempt blank spaces.
        if (str[index] == 32) {
            continue;
        }

        bltChar8x8(x + (index << 3), y, str[index], color);
    }
} // end Surface::bltString

// bltStringShadowed
//---------------------------------------------------------------------------
void Surface::bltStringShadowed(int x, int y, char const *str, const IntColor textColor, const IntColor shadowColor)
{
    for (int index = 0; str[index] != 0; index++) {
        bltChar8x8(x + (index << 3) + 1, y + 1, str[index], shadowColor);
        bltChar8x8(x + (index << 3),     y,     str[index], textColor);
    }

} // end Surface::bltStringShadowed

// bltStringCenter
//---------------------------------------------------------------------------
// Purpose: Blits a string of text and centers it horizontally and vertically
//          on the screen. Does not handle wrapping.
//---------------------------------------------------------------------------
void Surface::bltStringCenter(const char *string, IntColor color)
{
    bltString(  (getWidth() - (strlen(string) * FONT_WIDTH)) / 2,
                (getHeight() - getFontHeight()) / 2,
                string, color);

} // end Surface::bltStringCenter

// bltStringShadowedCenter
//---------------------------------------------------------------------------
// Purpose: Blits a string of text and centers it horizontally and vertically
//          on the screen. Does not handle wrapping.
//---------------------------------------------------------------------------
void Surface::bltStringShadowedCenter(const char *string, IntColor foreground, IntColor background)
{
    bltStringShadowed((getWidth() - (strlen(string) * FONT_WIDTH)) / 2,
                      (getHeight() - getFontHeight()) / 2,
                      string, foreground, background);

} // end Surface::bltStringShadowedCenter

// bltStringCenteredInRect
//---------------------------------------------------------------------------
// Purpose: Blits the string centered inside the specified rectangle.
//---------------------------------------------------------------------------
void Surface::bltStringCenteredInRect(const iRect &rect, const char *string, const IntColor color)
{
    int length = strlen(string);

    iXY destPos;
    destPos.x = rect.min.x + (rect.getSizeX() - (length * FONT_WIDTH)) / 2;
    destPos.y = rect.min.y + (rect.getSizeY() - getFontHeight()) / 2;

    for (int i = 0; string[i] != 0; i++) {
        bltChar8x8(destPos.x + (i << 3), destPos.y, string[i], color);
    }

} // end Surface::bltStringCenteredInRect


// nextFrame
//
// Move to the next frame based off time and the fps.  Returns a 0 if it
// was the last frame and 1 otherwise.
//---------------------------------------------------------------------------
int Surface::nextFrame()
{
    curFrame += TimerInterface::getTimeSlice() * fps;

    if (curFrame >= getNumFrames()) {
        curFrame = 0.0;
        return 0;
    }

    setFrame(curFrame);
    return 1;
}

void Surface::loadBMP(const char *fileName)
{
    freeFrames();

    SDL_Surface * image = SDL_LoadBMP(filesystem::getRealName(fileName).c_str());
    if ( image )
    {
    	frames.resize(1);
    	frames[0] = image;
        cur_frame = image;

        mem = (Uint8*)cur_frame->pixels;
    	twidth = image->w;
    	theight = image->h;
    	tpitch = image->pitch;
    }
}

void Surface::loadPNG(const char *fileName)
{
    freeFrames();

    SDL_RWops * pfile = SDL_RWFromFile(filesystem::getRealName(fileName).c_str(), "rb");
    SDL_Surface * image = IMG_LoadPNG_RW(pfile);
    pfile->close(pfile);

    if ( image )
    {
        frames.resize(1);
        frames[0] = image;
        cur_frame = image;

        mem = (Uint8*)cur_frame->pixels;
        twidth = image->w;
        theight = image->h;
        tpitch = image->pitch;
    }
    else
    {
        LOGGER.warning("Couldn't load image %s", fileName);
    }
}

void
Surface::loadPNGSheet(const char * fileName, int width, int height, int count)
{
    Surface t;
    t.loadPNG(fileName);

    alloc(width, height, count, 32);

    SDL_Rect r = {0, 0, width, height};

    for ( int n = 0; n < count; ++n )
    {
        SDL_BlitSurface(t.frames[0], &r, frames[n], 0);
        r.x += width;
        if ( r.x > t.frames[0]->w )
        {
            r.x = 0;
            r.y += height;
        }
    }
}
// drawButtonBorder
//---------------------------------------------------------------------------
void Surface::drawButtonBorder(iRect bounds, IntColor topLeftColor, IntColor bottomRightColor)
{
    assert(getDoesExist());
    assert(this != 0);

    orderCoords(bounds);
    drawHLine(bounds.min.x,bounds.min.y,bounds.max.x-1,topLeftColor);
    drawVLine(bounds.min.x,bounds.min.y,bounds.max.y-1,topLeftColor);
    drawHLine(bounds.min.x,bounds.max.y-1,bounds.max.x-1,bottomRightColor);
    drawVLine(bounds.max.x-1,bounds.min.y,bounds.max.y-1,bottomRightColor);

} // end Surface::drawButtonBorder

// drawWindowsBorder
//--------------------------------------------------------------------------
void Surface::drawWindowsBorder()
{
    drawRect(iRect(0,0,getWidth(),getHeight()), Color::darkGray);
} // end Surface::drawWindowsBorder

// bltStringInBox
//--------------------------------------------------------------------------
void Surface::bltStringInBox(const iRect &rect, const char *string, IntColor color, int gapSpace, bool drawBox)
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
            pos.x += FONT_WIDTH;
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

        if ((int) (pos.x + strlen(strBuf) * FONT_WIDTH) > rect.max.x) {
            pos.x = rect.min.x;
            pos.y += gapSpace;
        }

        bltString(pos.x, pos.y, strBuf, color);

        if (done) {
            return;
        }

        pos.x += strlen(strBuf) * FONT_WIDTH;

        length += strBufLength;
    }

} // end Surface::bltStringInBox

// drawBoxCorners
//--------------------------------------------------------------------------
// Purpose: Draws lines in the corners of the surface of the specified length
//          and color.
//--------------------------------------------------------------------------
void Surface::drawBoxCorners(const iRect &rect, int cornerLength, IntColor color)
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

bool
Surface::setPixelsFromRawData( Uint8 * rawdata, int buffer_length )
{
    if ( buffer_length < getArea() )
    {
        return false;
    }

    for ( unsigned int line=0; line<getHeight(); ++line )
    {
        for ( unsigned int column=0; column < getWidth(); ++column )
        {
            Uint8 c = *rawdata;
            ++rawdata;
            putPixel(column, line, SDL_MapRGB(cur_frame->format,
                                              Palette::color[c].r,
                                              Palette::color[c].g,
                                              Palette::color[c].b));
//            memcpy(pixPtr(0,line), rawdata+(line*getWidth()), getWidth());
        }
    }
    return true;
}

bool
Surface::getRawDataFromPixels( Uint8 * rawdata, int buffer_length )
{
	if ( buffer_length < getArea() )
	{
		return false;
	}

	for ( unsigned int line=0; line<getHeight(); ++line )
	{
		memcpy(rawdata+(line*getWidth()),  pixPtr(0,line), getWidth());
	}
	return true;
}

void
Surface::brigthenFrames()
{
	SDL_Color newPal[256];
	unsigned int n;
	for ( n = 0; n < 256; ++n )
	{
//		newPal[n].r = (Palette::color[n].r >> 1) | 0x80;
//		newPal[n].g = (Palette::color[n].g >> 1) | 0x80;
//		newPal[n].b = (Palette::color[n].b >> 1) | 0x80;
		newPal[n].r |= 0x80;
		newPal[n].g |= 0x80;
		newPal[n].b |= 0x80;
		newPal[n].unused = 0;
	}

	for ( n = 0; n < frames.size(); ++n )
	{
		SDL_SetColors(frames[n], newPal, 0, 256);
	}
}

void
Surface::setColorkey()
{
    for ( unsigned int n = 0; n < frames.size(); ++n )
    {
        SDL_SetColorKey(frames[n], SDL_SRCCOLORKEY | SDL_RLEACCEL, 0);
    }
}

void
Surface::setAlpha()
{
    for ( unsigned int n = 0; n < frames.size(); ++n )
    {
        SDL_SetAlpha(frames[n], SDL_SRCALPHA, 128);
    }
}

void
Surface::optimize()
{
    SDL_Surface *op = 0;
    for ( unsigned int n = 0; n < frames.size(); ++n )
    {
        op = SDL_DisplayFormat(frames[n]);
        if ( op )
        {
            if ( cur_frame == frames[n] )
            {
                cur_frame = op;
            }
            SDL_FreeSurface(frames[n]);
            frames[n] = op;
        }
    }
}
