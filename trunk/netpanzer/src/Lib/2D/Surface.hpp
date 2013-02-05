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
#ifndef __Surface_hpp__
#define __Surface_hpp__

#include <string>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "Types/iXY.hpp"
#include "Types/iRect.hpp"
#include "Util/NoCopy.hpp"

#include "SDL.h"

class ColorTable;
class Palette;
typedef Uint8 PIX;

// This must be called before any of the string blitting functions are used.
void initFont();

/////////////////////////////////////////////////////////////////////////////
// class Surface
/////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------
class Surface : public NoCopy
{
private:
    static int totalSurfaceCount;  // The number of surfaces alive.
    static int totalByteCount;     // The number of bytes of the surfaces alive.

public:
    static int getTotalSurfaceCount()   { return totalSurfaceCount; }
    static int getTotalByteCount()      { return totalByteCount; }

    Surface();
    Surface(unsigned int w, unsigned int h);

    virtual ~Surface();
    
    void create(unsigned int w, unsigned int h);
    void free();

    // Accessor functions.
    unsigned int getFullWidth()  const { return twidth; }
    unsigned int getFullHeight() const { return theight; }
    unsigned int getWidth()  const { return clip_rect.getWidth(); }
    unsigned int getHeight() const { return clip_rect.getHeight(); }
    unsigned int getPitch()  const { return tpitch; }
    
    iXY     getCenter()     const { return iXY(getWidth()>>1, getHeight()>>1); }
    int     getCenterX()    const { return getWidth()>>1; }
    int     getCenterY()    const { return getHeight()>>1; }

    bool   getDoesExist()   const { return frame0 != 0; }
    PIX   *getMem()         const { return mem; }
    PIX   *getFrame0()      const { return frame0; }

    iRect  getRect() const
    {
        return iRect(0, 0, getWidth(), getHeight());
    }

    void putPixel(unsigned int x, unsigned int y, const PIX &color) 
    {
        if ( x >= getWidth() || y >= getHeight())
            return;
        *(mem + (y * getPitch()) + x) = color;
    }

    PIX getPixel(unsigned int x, unsigned int y) const
    {
        return *pixPtr(x,y);
    }

    void drawRect(iRect bounds, const PIX &color);
    void fillRect(iRect bounds, const PIX &color);
    bool grab(const Surface &s, iRect bounds);

    void resetClip()
    {
        clip_rect.setLocation( 0, 0 );
        clip_rect.setSize( twidth, theight );
        mem = frame0;
    }

    void setAbsoluteClip(const iRect& clip)
    {
        unsigned x = clip.getLocationX();
        unsigned y = clip.getLocationY();
        if ( (x >= getFullWidth()) || (y >= getFullHeight()) )
        {
            resetClip();
            return;
        }

        clip_rect.setLocation( clip.getLocationX(), clip.getLocationY() );
        clip_rect.setSize(std::min(clip.getWidth(), twidth - x),
                          std::min(clip.getHeight(), theight - y) );

        mem = frame0 + (clip_rect.getLocationY() * getPitch()) + clip_rect.getLocationX();
    }

private:
    friend class ScreenSurface;
    friend class PackedSurface;
    PIX   *mem;       // Pointer to upperleft most pixel
    PIX   *frame0;    // Pointer to first frame
    iRect clip_rect;
    unsigned int twidth;
    unsigned int theight;
    unsigned int tpitch;     // Number of bytes from one row to the next.

    void alloc(unsigned int w, unsigned int h);

    PIX *pixPtr(unsigned int x, unsigned int y) const
    {
        assert((y * getPitch() + x) < getPitch() * getHeight());
        return mem + (y * getPitch()) + x;
    }

    void        reset();

public:
    void blt(Surface &dest, int x, int y) const;
    void bltTrans(Surface &dest, int x, int y) const;
    void bltTransColor(Surface &dest, int x, int y, const PIX color) const;
    void bltScale(const Surface &source, const iRect &destRect);
    void bltLookup(const iRect &destRect, const PIX table[]);


    void drawHLine(int x1, int y,  int size, const PIX &color);
    void drawVLine(int x,  int y1, int size, const PIX &color);

    void drawLine(int x1, int y1, int x2, int y2, const PIX &color);

    void drawLine(const iXY &a, const iXY &b, const PIX &color)
    {
        drawLine(a.x, a.y, b.x, b.y, color);
    }

    // Surface Effects.
    void drawButtonBorder(iRect bounds, PIX topLeftColor, PIX bottomRightColor);
    void drawButtonBorder(PIX topLeftColor, PIX bottomRightColor)
    {
        drawButtonBorder(iRect(0, 0, getWidth(), getHeight()), topLeftColor, bottomRightColor);
    }

    void drawWindowsBorder();
    
    void fill(const PIX &color);
    void flipVertical();
    void copy(const Surface &source);

    PIX getAverageColor();

    // Text rendering functions
    void renderText(const char *str, PIX color, PIX bgcolor);
    
    // Blit a single character of text.
    void bltChar8x8(int x, int y, unsigned char character, const PIX &color);
    void bltString(int x, int y, const char * str, const PIX& color);
    void bltStringLen(int x, int y, const char * str, int len, const PIX& color);

    // Blit a shadowed string of text.
    void bltStringShadowed(int x, int y, const char *str, const PIX &textColor, const PIX &shadowColor);

    // Blits a string of text and centers it horizontally and vertically on the screen.
    void bltStringCenter(const char *string, PIX color);

    void bltStringShadowedCenter(const char *string, PIX foreground, PIX background);
    void bltStringCenteredInRect(const iRect &rect, const char *string, const PIX &color);

    void loadBMP(const char *fileName, bool needAlloc = true);

    void drawBoxCorners(const iRect &rect, unsigned cornerLength, PIX color);

    void drawBoxCorners(int cornerLength, PIX color)
    {
        drawBoxCorners(iRect(0, 0, getWidth(), getHeight()), cornerLength, color);
    }

    static unsigned int getFontHeight();
    static int getTextLength(const char* text);
    static int getTextLength(const std::string& text)
    {
        return getTextLength(text.c_str());
    }

    void circle(int cx, int cy, int radius, PIX color);
    void FillCircle(int cx, int cy, int radius, PIX color);
    void BltCircle(int cx, int cy, int radius, const PIX table[]);
    void RoundRect(iRect rect, int radius, PIX color);
    void FillRoundRect(iRect rect, int radius, PIX color);
    void BltRoundRect(iRect rect, int radius, const PIX table[]);
    void bltHLine(int x1, int y, int x2, const PIX table[]);
        
    void frameToBuffer(Uint8* dest, size_t dest_len)
    {
        size_t frame_len = getPitch()*getHeight();
        memcpy(dest, mem, std::min(frame_len, dest_len));
    }

    void bufferToFrame(const Uint8* src, const size_t src_len)
    {
        size_t frame_len = getPitch()*getHeight();
        memcpy(mem, src, std::min(frame_len, src_len));
    }

}; // end Surface

#endif // __Surface_HPP__
