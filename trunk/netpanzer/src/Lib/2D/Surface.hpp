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
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "Types/iXY.hpp"
#include "Types/iRect.hpp"
#include "Util/NoCopy.hpp"

class ColorTable;
class Palette;
class ReadFile;
typedef uint8_t PIX;

// This must be called before any of the string blitting functions are used.
void initFont();

/////////////////////////////////////////////////////////////////////////////
// class Surface
/////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------
class Surface : public NoCopy
{
public:
    void setOffset(const iXY &offset)
    {
        Surface::offset = offset;
    }
    void setOffsetX(int offset)
    {
        Surface::offset.x = offset;
    }
    void setOffsetY(int offset)
    {
        Surface::offset.y = offset;
    }
    void setOffsetCenter();
    void setFPS(int fps)
    {
        Surface::fps = fps;
    }
    void setFrame0(PIX *frame0)
    {
        assert(frame0 != 0);

        Surface::frame0 = frame0;
    }

    PIX   *mem;       // Pointer to upperleft most pixel
    PIX   *frame0;    // Pointer to first frame

protected:
    iXY   offset;     // Used like a hot spot for drawing.
    float fps;        // The speed to change the frames. Make sure TimerInterface::getTimeSlice() is being updated.
    iXY	  pix;        // # of pixels in the x and y diiRecton.
    iXY	  center;     // Center of the pixels in the x and y diiRecton.
    int   stride;     // Number of bytes from one row to the next.
    int	  frameCount; // Number of frames.
    float curFrame;   // Current frame of frameCount.
    bool  myMem;	  // Am I the owner of this surface, or am I mapped to some other surface.
    bool  doesExist;  // Is it ok o write to this surface?

    static int totalSurfaceCount;  // The number of surfaces alive.
    static int totalByteCount;     // The number of bytes of the surfaces alive.

    int   wipeCount;      // Used for font wiping.

public:
    // Accessor functions.
    float  getFPS() const
    {
        return fps;
    }
    bool   getDoesExist() const
    {
        return doesExist;
    }
    iXY    getPix() const
    {
        return pix;
    }
    int    getPixX() const
    {
        return pix.x;
    }
    int    getPixY() const
    {
        return pix.y;
    }
    float  getAspectXOverY() const
    {
        return float(pix.x) / float(pix.y);
    }
    float  getAspectYOverX() const
    {
        return float(pix.y) / float(pix.x);
    }
    iXY    getCenter() const
    {
        return center;
    }
    int    getCenterX() const
    {
        return center.x;
    }
    int    getCenterY() const
    {
        return center.y;
    }
    int    getStride() const
    {
        return stride;
    }
    int    getFrameCount() const
    {
        return frameCount;
    }
    int    getCurFrame () const
    {
        return (int) curFrame;
    }
    PIX   *getMem() const
    {
        return mem;
    }
    PIX   *getFrame0() const
    {
        return frame0;
    }
    iXY    getOffset() const
    {
        return offset;
    }
    int    getOffsetX() const
    {
        return offset.x;
    }
    int    getOffsetY() const
    {
        return offset.y;
    }
    int    getArea() const
    {
        return pix.x * pix.y;
    }
    iRect  getRect() const
    {
        return iRect(0, 0, pix.x - 1, pix.y - 1);
    }

    static int    getTotalSurfaceCount()
    {
        return totalSurfaceCount;
    }
    static int    getTotalByteCount()
    {
        return totalByteCount;
    }

protected:
    void        reset();

public:
    Surface();
    Surface(const iXY &nPix, int nStride, int nFrameCount);
    Surface(int xPix, int yPix, int nStride, int nFrameCount);
    Surface(const Surface &source, const iXY &min, const iXY &max, bool doGrab);
    Surface(void *nFrame0, const iXY &nPix, int nStride, int nFrameCount);

    virtual ~Surface();

    bool alloc(const iXY &pix, bool gottaHaveIt, int stride, int frameCount);

    bool	alloc(int xPix, int yPix, bool gottaHaveIt, int stride, int frameCount)
    {
        return alloc(iXY(xPix, yPix), gottaHaveIt, stride, frameCount);
    }

    void createNoAlloc(const iXY &nPix);

    void create(iXY nPix ,int nStride, int nFrameCount);

    void create(int xPix, int yPix, int stride, int frameCount)
    {
        create(iXY(xPix, yPix), stride, frameCount);
    }
    void create(int xPix, int yPix)
    {
        create(iXY(xPix, yPix), xPix, 1);
    }
    void create(const iXY &pix)
    {
        create(pix, pix.x, 1);
    }
    void create(const Surface &source)
    {
        create(source.getPix(), source.getStride(), source.getFrameCount());
    }

    void free();

    PIX *pixPtr(const iXY &pos) const
    {
        assert((pos.y * stride + pos.x) < stride * pix.y);

        return mem + (pos.y * stride) + pos.x;
    }

    PIX *operator[](int y) const
    {
        assert(y >= 0);
        assert(y < pix.y);
        return mem + (y * stride);
    }

    PIX *rowPtr(int y) const
    {
        assert(y >= 0);
        assert(y < pix.y);
        return mem + (y * stride);
    }

    void resize(const iXY &pix);
    void setTo(const Surface &s, iRect bounds);
    void setTo(void *frame0, const iXY &pix, int stride, int frameCount);
    void setTo(const Surface &source);
    bool grab(const Surface &s, iRect bounds, bool gottaHaveIt, int stride);

    void blt(Surface &dest, iXY pos) const;
    void blt(Surface &dest) const
    {
        blt(dest, iXY(0, 0));
    }
    void blt(Surface &dest, int x, int y) const
    {
        blt(dest, iXY(x, y));
    }

    void bltTrans(Surface &dest, iXY pos) const;
    void bltTrans(Surface &dest) const
    {
        bltTrans(dest, iXY(0, 0));
    }
    void bltTrans(Surface &dest, int x, int y) const
    {
        bltTrans(dest, iXY(x, y));
    }

    void bltTransColor(Surface &dest, iXY pos, const PIX &color) const;
    void bltTransColor(Surface &dest, const PIX &color) const
    {
        bltTransColor(dest, iXY(0, 0), color);
    }
    void bltTransColor(Surface &dest, int x, int y, const PIX &color) const
    {
        bltTransColor(dest, iXY(x, y), color);
    }

    void drawHLine(int x1, int y,  int x2, const PIX &color);
    void drawVLine(int x,  int y1, int y2, const PIX &color);

    void drawLine(int x1, int y1, int x2, int y2, const PIX &color);

    void drawLine(const iXY &a, const iXY &b, const PIX &color)
    {
        drawLine(a.x, a.y, b.x, b.y, color);
    }

    void resize(int xPix, int yPix)
    {
        resize(iXY(xPix, yPix));
    }

    void bltCentered(Surface &dest) const
    {
        iXY pos;
        pos.x = (dest.getPix().x - pix.x) / 2;
        pos.y = (dest.getPix().y - pix.y) / 2;

        blt(dest, pos);
    }

    // Surface Effects.
    void drawButtonBorder(iRect bounds, PIX topLeftColor, PIX bottomRightColor);
    void drawButtonBorder(PIX topLeftColor, PIX bottomRightColor)
    {
        drawButtonBorder(iRect(0, 0, pix.x, pix.y), topLeftColor, bottomRightColor);
    }

    void drawWindowsBorder(iRect bounds, PIX light, PIX medium, PIX dark);
    void drawWindowsBorder(PIX light, PIX medium, PIX dark)
    {
        drawWindowsBorder(iRect(0, 0, pix.x, pix.y), light, medium, dark);
    }

    void blendIn(const Surface &source, iXY min, ColorTable &colorTable);
    void blendIn(const Surface &source, ColorTable &colorTable)
    {
        blendIn(source, iXY(0, 0), colorTable);
    }
    void blendIn(const Surface &source, int x, int y, ColorTable &colorTable)
    {
        blendIn(source, iXY(x, y), colorTable);
    }

    void bltBlendScale(const Surface &source, const iRect &destRect, ColorTable &colorTable);

    void bltLookup(const iRect &destRect, const PIX table[]);
    void bltLookup(const PIX table[])
    {
        bltLookup(iRect(0, 0, pix.x, pix.y), table);
    }

    void bltScale(const Surface &source, const iRect &destRect);
    void bltScaleTrans(const Surface &source, const iRect &destRect);

    void drawRect(iRect bounds, const PIX &color);
    void drawRect(int x1, int y1, int x2, int y2, const PIX &color)
    {
        drawRect(iRect(x1, y1, x2, y2), color);
    }
    void drawRect(const iXY &min, const iXY &max, const PIX &color)
    {
        drawRect(iRect(min.x, min.y, max.x, max.y), color);
    }
    void drawRect(const PIX &color)
    {
        drawRect(iRect(0, 0, pix.x, pix.y), color);
    }

    void fillRect(iRect bounds, const PIX &color);
    void fillRect(int x1, int y1, int x2, int y2, const PIX &color)
    {
        fillRect(iRect(x1, y1, x2, y2), color);
    }

    void blendRect(iRect bounds, ColorTable &colorTable);
    void fill(const PIX &color);
    void fillAll(const PIX &color);
    void flipHorizontal();
    void flipVertical();
    void rotate(int angle);
    void copy(const Surface &source);
    int nextFrame();

    void setFrame(const float &frameNum)
    {
        assert(frameNum >= 0.0);
        assert(frameNum < frameCount);
        mem = frame0 + (pix.y * stride) * int(frameNum);
    }

    void _putPixel(const iXY &pos, const PIX &color)
    {
        assert(getDoesExist());
        assert(pos.x >= 0 && pos.x < pix.x && pos.y >= 0 && pos.y < pix.y);
        *pixPtr(pos) = color;
    }

    void _putPixel(int x, int y, const PIX &color) 
    {
        _putPixel(iXY(x, y), color);
    }

    void putPixel(const iXY &pos, const PIX &color) 
    {
        // Clip the pixel to the surface.
        if (pos.x < 0 || pos.x >= pix.x || pos.y < 0 || pos.y >= pix.y)
            return;
        _putPixel(pos, color);
    }

    void putPixel(int x, int y, const PIX &color) 
    {
        putPixel(iXY(x, y), color);
    }

    PIX getPixel(const iXY &pos) const
    {
        return *pixPtr(pos);
    }

    PIX getPixel(int x, int y) const
    {
        return getPixel(iXY(x, y));
    }

    PIX getAverageColor();

    void scale(const iXY &pix);
    void scale(int x, int y)
    {
        scale(iXY(x, y));
    }
    void scale(int x)
    {
        scale(iXY(x, x));
    }

    void shrinkWrap();

    void blendRect(ColorTable &colorTable)
    {
        blendRect(iRect(0, 0, pix.x - 1, pix.y - 1), colorTable);
    }

    int  loadAllBMPInDirectory(const char *path);

    // Blit a single character of text.
    void bltChar8x8(const iXY &pos, unsigned char character, const PIX &color);
    void bltChar8x8(int x, int y, unsigned char character, const PIX &color)
    {
        bltChar8x8(iXY(x, y), character, color);
    }

    void bltChar5x5(const iXY &pos, unsigned char character, const PIX &color);
    void bltChar5x5(int x, int y, unsigned char character, const PIX &color)
    {
        bltChar5x5(iXY(x, y), character, color);
    }

    // Blit a string of text in a single color.
    void bltString(const iXY& pos, const char *string, const PIX &color);
    void bltString(const iXY& pos, const std::string& string, const PIX& color)
    {
        bltString(pos, string.c_str(), color);
    }
    void bltString(int x, int y, const std::string& string, const PIX& color)
    {
        bltString(iXY(x, y), string.c_str(), color);
    }

    void bltStringInBox(const iRect &rect, const char *string, PIX color, int gapSpace = 14, bool drawBox = false);

    // Blit a string of text.
    void bltString5x5(const iXY &pos, const char *string, const PIX &color);

    // Blit a shadowed string of text.
    void bltStringShadowed(const iXY &pos, const char *string, const PIX &textColor, const PIX &shadowColor);

    // Blits a string of text and centers it horizontally and vertically on the screen.
    void bltStringCenter(const char *string, PIX color);

    void bltStringShadowedCenter(const char *string, PIX foreground, PIX background);
    void bltStringCenteredAtPoint(const iXY &pos, const char *string, const PIX &color);
    void bltStringCenteredInRect(const iRect &rect, const char *string, const PIX &color);

    void loadBMP(const char *fileName, bool needAlloc = true, void *returnPalette = 0);

    void mapFromPalette(const std::string& oldPalette);

    void drawBoxCorners(const iRect &rect, int cornerLength, PIX color);
    void drawBoxCorners(const iXY &min, const iXY &max,
            int cornerLength, PIX color)
    {
        drawBoxCorners(iRect(min.x, min.y, max.x, max.y), cornerLength, color);
    }
    void drawBoxCorners(int cornerLength, PIX color)
    {
        drawBoxCorners(iRect(0, 0, pix.x - 1, pix.y - 1), cornerLength, color);
    }

    void drawLookupBorder(const PIX table[]);

    static int getFontHeight();
    static int getTextLength(const char* text);
    static int getTextLength(const std::string& text)
    { return getTextLength(text.c_str()); } 
}; // end Surface

#endif // __Surface_HPP__
