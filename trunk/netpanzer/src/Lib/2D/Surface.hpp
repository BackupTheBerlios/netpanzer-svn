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
#include <memory.h>
#include <stdint.h>

#include "Types/iXY.hpp"
#include "Types/iRect.hpp"

typedef uint8_t PIX;

/////////////////////////////////////////////////////////////////////////////
// class Surface
/////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------
class Surface
{
private:
    Surface(const Surface&);
    void operator=(const Surface&);
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
    void reset();

    // Accessor functions.
    unsigned int getWidth()  const { return twidth; }
    unsigned int getHeight() const { return theight; }
    unsigned int getPitch()  const { return tpitch; }
    
    iXY     getCenter()     const { return iXY(getWidth()>>1, getHeight()>>1); }
    int     getCenterX()    const { return getWidth()>>1; }
    int     getCenterY()    const { return getHeight()>>1; }

    bool   getDoesExist()   const { return mem != 0; }
    PIX   *getMem()         const { return mem; }

    PIX *pixPtr(unsigned int x, unsigned int y) const
    {
        assert( (y < getHeight()) && (x < getWidth()) );
        return mem + (y * getPitch()) + x;
    }

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

    void drawRect(iRect bounds, const PIX color);
    void fillRect(iRect bounds, const PIX color);
    bool grab(const Surface &s, iRect bounds);

private:
    friend class ScreenSurface;
    friend class PackedSurface;
    PIX   *mem;
    unsigned int twidth;
    unsigned int theight;
    unsigned int tpitch;     // Number of bytes from one row to the next.


public:
    /**
     * Blits this whole surface onto the destination surface performing clipping
     * on it.
     * @param dest Destination surface
     * @param x Destination x
     * @param y Destination y
     */
    void blt(Surface &dest, int x, int y) const;
    
    /**
     * Blits this whole surface onto the destination surface ignoring transparent
     * pixels (value = 0), performs clipping.
     * @param dest Destination surface
     * @param x Destination x
     * @param y Destination y
     */
    void bltTrans(Surface &dest, int x, int y) const;
    
    /**
     * Blits this whole surface onto the destination surface setting the
     * transparent pixels (value = 0) to the passed color, performs clipping.
     * @param dest Destination surface
     * @param x Destination x
     * @param y Destination y
     * @param color New color to use for the transparent pixels
     */
    void bltTransColor(Surface &dest, int x, int y, const PIX color) const;
    
    /**
     * <p><b>Note: parameter is source not destination as the other blits</b></p>
     * <p>Blits the whole passed surface onto this surface on the position and
     * size passed (may perform scaling). Performs clipping.</p>
     * @param source 
     * @param destRect
     */
    void bltScale(const Surface &source, const iRect &destRect);
    
    /**
     * For each pixel in the surface, replaces it value with the mapped value of
     * the passed table.
     * @param destRect Area to perform the lookup
     * @param table Lookup table
     */
    void bltLookup(const iRect &destRect, const PIX table[]);

    /**
     * 
     * @param dest Destination surface
     * @param x Destination x
     * @param y Destination y
     * @param color
     */
    void bltAlphaColor(Surface &dest, int x, int y, const PIX color) const;
    
    /**
     * 
     * @param dest
     * @param x
     * @param y
     * @param rect
     * @param color
     */
    void bltRectAlphaColor(Surface &dest, int x, int y, const iRect& rect, const PIX color) const;


    void drawHLine(int x1, int y,  int size, const PIX color);
    void drawVLine(int x,  int y1, int size, const PIX color);

    void drawLine(int x1, int y1, int x2, int y2, const PIX color);

    void drawLine(const iXY &a, const iXY &b, const PIX color)
    {
        drawLine(a.x, a.y, b.x, b.y, color);
    }
    
    void fill(const PIX color);
    void flipVertical();
    void copy(const Surface &source);

    PIX getAverageColor();

    // Blit a single character of text.
    void bltChar8x8(int x, int y, unsigned char character, const PIX color);
    void bltString(int x, int y, const char * str, const PIX color);
    void bltStringLen(int x, int y, const char * str, int len, const PIX color);

    // Blit a shadowed string of text.
    void bltStringShadowed(int x, int y, const char *str, const PIX textColor, const PIX shadowColor);
    void bltStringCenteredInRect(const iRect &rect, const char *string, const PIX color);

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
        
    void frameToBuffer(uint8_t* dest, size_t dest_len)
    {
        size_t frame_len = getPitch()*getHeight();
        memcpy(dest, mem, std::min(frame_len, dest_len));
    }

    void bufferToFrame(const uint8_t* src, const size_t src_len)
    {
        size_t frame_len = getPitch()*getHeight();
        memcpy(mem, src, std::min(frame_len, src_len));
    }

}; // end Surface

#endif // __Surface_HPP__
