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

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "iXY.hpp"
#include "iRect.hpp"
#include "NoCopy.hpp"

class ColorTable;
class Palette;
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
    inline float  getFPS() const
    {
        return fps;
    }
    inline bool   getDoesExist() const
    {
        return doesExist;
    }
    inline iXY    getPix() const
    {
        return pix;
    }
    inline int    getPixX() const
    {
        return pix.x;
    }
    inline int    getPixY() const
    {
        return pix.y;
    }
    inline float  getAspectXOverY() const
    {
        return float(pix.x) / float(pix.y);
    }
    inline float  getAspectYOverX() const
    {
        return float(pix.y) / float(pix.x);
    }
    inline iXY    getCenter() const
    {
        return center;
    }
    inline int    getCenterX() const
    {
        return center.x;
    }
    inline int    getCenterY() const
    {
        return center.y;
    }
    inline int    getStride() const
    {
        return stride;
    }
    inline int    getFrameCount() const
    {
        return frameCount;
    }
    inline int    getCurFrame () const
    {
        return (int) curFrame;
    }
    inline PIX   *getMem() const
    {
        return mem;
    }
    inline PIX   *getFrame0() const
    {
        return frame0;
    }
    inline iXY    getOffset() const
    {
        return offset;
    }
    inline int    getOffsetX() const
    {
        return offset.x;
    }
    inline int    getOffsetY() const
    {
        return offset.y;
    }
    inline int    getArea() const
    {
        return pix.x * pix.y;
    }
    inline iRect  getRect() const
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

    inline bool	alloc(int xPix, int yPix, bool gottaHaveIt, int stride, int frameCount)
    {
        return alloc(iXY(xPix, yPix), gottaHaveIt, stride, frameCount);
    }

    void createNoAlloc(const iXY &nPix);

    void create(iXY nPix ,int nStride, int nFrameCount);

    inline void create(int xPix, int yPix, int stride, int frameCount)
    {
        create(iXY(xPix, yPix), stride, frameCount);
    }
    inline void create(int xPix, int yPix)
    {
        create(iXY(xPix, yPix), xPix, 1);
    }
    inline void create(const iXY &pix)
    {
        create(pix, pix.x, 1);
    }
    inline void create(const Surface &source)
    {
        create(source.getPix(), source.getStride(), source.getFrameCount());
    }

    void free();

    inline PIX *pixPtr(const iXY &pos) const
    {
        assert((pos.y * stride + pos.x) < stride * pix.y);

        return mem + (pos.y * stride) + pos.x;
    }

    inline PIX *operator[](int y) const
    {
        assert(y >= 0);
        assert(y < pix.y);
        return mem + (y * stride);
    }

    inline PIX *rowPtr(int y) const
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
    inline void blt(Surface &dest) const
    {
        blt(dest, iXY(0, 0));
    }
    inline void blt(Surface &dest, int x, int y) const
    {
        blt(dest, iXY(x, y));
    }

    void bltTrans(Surface &dest, iXY pos) const;
    inline void bltTrans(Surface &dest) const
    {
        bltTrans(dest, iXY(0, 0));
    }
    inline void bltTrans(Surface &dest, int x, int y) const
    {
        bltTrans(dest, iXY(x, y));
    }

    void bltTransColor(Surface &dest, iXY pos, const PIX &color) const;
    inline void bltTransColor(Surface &dest, const PIX &color) const
    {
        bltTransColor(dest, iXY(0, 0), color);
    }
    inline void bltTransColor(Surface &dest, int x, int y, const PIX &color) const
    {
        bltTransColor(dest, iXY(x, y), color);
    }

    void bltTransVGradient(Surface &dest, iXY pos, ColorTable &colorTable) const;
    inline void bltTransVGradient(Surface &dest, ColorTable &colorTable) const
    {
        bltTransVGradient(dest, iXY(0, 0), colorTable);
    }
    inline void bltTransVGradient(Surface &dest, int x, int y, ColorTable &colorTable) const
    {
        bltTransVGradient(dest, iXY(x, y), colorTable);
    }

    void drawHLine(int x1, int y,  int x2, const PIX &color) const;
    void drawVLine(int x,  int y1, int y2, const PIX &color) const;

    void drawLine(int x1, int y1, int x2, int y2, const PIX &color) const;

    void drawLine(const iXY &a, const iXY &b, const PIX &color) const
    {
        drawLine(a.x, a.y, b.x, b.y, color);
    }

    inline void	resize(int xPix, int yPix)
    {
        resize(iXY(xPix, yPix));
    }

    inline void bltCentered(Surface &dest) const
    {
        iXY pos;
        pos.x = (dest.getPix().x - pix.x) / 2;
        pos.y = (dest.getPix().y - pix.y) / 2;

        blt(dest, pos);
    }

    // Surface Effects.
    void drawButtonBorder(iRect bounds, PIX topLeftColor, PIX bottomRightColor) const;
    inline void drawButtonBorder(PIX topLeftColor, PIX bottomRightColor) const
    {
        drawButtonBorder(iRect(0, 0, pix.x, pix.y), topLeftColor, bottomRightColor);
    }

    void drawWindowsBorder(iRect bounds, PIX light, PIX medium, PIX dark) const;
    inline void drawWindowsBorder(PIX light, PIX medium, PIX dark) const
    {
        drawWindowsBorder(iRect(0, 0, pix.x, pix.y), light, medium, dark);
    }

    void verticalWaveAll(int numWaves, float percent);
    void verticalWave(int numWaves, float percent, int offset);
    void horizontalWaveAll(int numWaves, float percent);
    void horizontalWave(int numWaves, float percent, int offset);
    void rippleAll(int numWaves, float percent);
    void ripple(int numWaves, float percent, int offset);
    void verticalWave3DAll(int numWaves, float percent);
    void verticalWave3D(int numWaves, float percent, int offset);
    void horizontalWave3DAll(int numWaves, float percent);
    void horizontalWave3D(int numWaves, float percent, int offset);

    void blendIn(const Surface &source, iXY min, ColorTable &colorTable) const;
    inline void blendIn(const Surface &source, ColorTable &colorTable) const
    {
        blendIn(source, iXY(0, 0), colorTable);
    }
    inline void blendIn(const Surface &source, int x, int y, ColorTable &colorTable) const
    {
        blendIn(source, iXY(x, y), colorTable);
    }

    void bltBlendScale(const Surface &source, const iRect &destRect, ColorTable &colorTable) const;

    void bltLookup(const iRect &destRect, const PIX table[]) const;
    void bltLookup(const PIX table[]) const
    {
        bltLookup(iRect(0, 0, pix.x, pix.y), table);
    }

    void bltScale(const Surface &source, const iRect &destRect) const;
    void bltScaleTrans(const Surface &source, const iRect &destRect) const;

    void drawRect(iRect bounds, const PIX &color) const;
    inline void drawRect(int x1, int y1, int x2, int y2, const PIX &color) const
    {
        drawRect(iRect(x1, y1, x2, y2), color);
    }
    inline void drawRect(const iXY &min, const iXY &max, const PIX &color) const
    {
        drawRect(iRect(min.x, min.y, max.x, max.y), color);
    }
    inline void drawRect(const PIX &color) const
    {
        drawRect(iRect(0, 0, pix.x, pix.y), color);
    }

    void fillRect(iRect bounds, const PIX &color) const;
    inline void fillRect(int x1, int y1, int x2, int y2, const PIX &color) const
    {
        fillRect(iRect(x1, y1, x2, y2), color);
    }

    void blendRect(iRect bounds, ColorTable &colorTable) const;
    void fill(const PIX &color) const;
    void fillAll(const PIX &color);
    void flipHorizontal();
    void flipVertical();
    void rotate(int angle);
    void copy(Surface &source);
    void setBrightness(int percent);
    int nextFrame();

    inline void setFrame(const float &frameNum)
    {
        assert(frameNum >= 0.0);
        assert(frameNum < frameCount);
        mem = frame0 + (pix.y * stride) * int(frameNum);
    }

    inline void _putPixel(const iXY &pos, const PIX &color) const
    {
        assert(getDoesExist());
        assert(pos.x >= 0 && pos.x < pix.x && pos.y >= 0 && pos.y < pix.y);
        *pixPtr(pos) = color;
    }

    inline void _putPixel(int x, int y, const PIX &color) const
    {
        _putPixel(iXY(x, y), color);
    }

    inline void putPixel(const iXY &pos, const PIX &color) const
    {
        // Clip the pixel to the surface.
        if (pos.x < 0 || pos.x >= pix.x || pos.y < 0 || pos.y >= pix.y)	return;
        _putPixel(pos, color);
    }

    inline void putPixel(int x, int y, const PIX &color) const
    {
        putPixel(iXY(x, y), color);
    }

    inline PIX getPixel(const iXY &pos) const
    {
        return *pixPtr(pos);
    }

    inline PIX getPixel(int x, int y) const
    {
        return getPixel(iXY(x, y));
    }

    PIX getAverageColor();

    void scale(const iXY &pix);
    inline void scale(int x, int y)
    {
        scale(iXY(x, y));
    }
    inline void scale(int x)
    {
        scale(iXY(x, x));
    }

    void shrinkWrap();

    inline void blendRect(ColorTable &colorTable) const
    {
        blendRect(iRect(0, 0, pix.x - 1, pix.y - 1), colorTable);
    }

    void bltBrightness(const Surface &dest, const iXY &pos, const float &percent);
    void bltLightDark(const Surface &source, const Surface &lightTable) const;
    void setToBrightnessIndexes128();
    void bltAdd(const Surface &dest, iXY min) const;
    int  loadAllBMPInDirectory(const char *path);

    // Blit a single character of text.
    void bltChar8x8(const iXY &pos, const char &character, const PIX &color);
    void bltChar8x8(int x, int y, const char &character, const PIX &color)
    {
        bltChar8x8(iXY(x, y), character, color);
    }

    void bltChar8x8VGradient(const iXY &pos, const char &character, ColorTable &colorTable);
    void bltChar8x8VGradient(int x, int y, const char &character, ColorTable &colorTable)
    {
        bltChar8x8VGradient(iXY(x, y), character, colorTable);
    }

    void bltChar5x5(const iXY &pos, const char &character, const PIX &color);
    inline void bltChar5x5(int x, int y, const char &character, const PIX &color)
    {
        bltChar5x5(iXY(x, y), character, color);
    }

    // Blit a string of text in a single color.
    void bltString(const iXY &pos, const char *string, const PIX &color);
    inline void bltString(int x, int y, const char *string, const PIX &color)
    {
        bltString(iXY(x, y), string, color);
    }

    void bltStringInBox(int minX, int minY, int maxX, int maxY, const char *string, PIX color, int gapSpace = 14, bool drawBox = false)
    {
        bltStringInBox(iRect(minX, minY, maxX, maxY), string, color, gapSpace, drawBox);
    }
    void bltStringInBox(const iRect &rect, const char *string, PIX color, int gapSpace = 14, bool drawBox = false);

    // Blit a string of text with a vertical gradient.
    void bltStringVGradient(const iXY &pos, const char *string, ColorTable &colorTable);
    void bltStringVGradient(int x, int y, const char *string, ColorTable &colorTable)
    {
        bltStringVGradient(iXY(x, y), string, colorTable);
    }

    // Blit a string of text.
    void bltString5x5(const iXY &pos, const char *string, const PIX &color);
    void bltString5x5(int x, int y, const char *string, const PIX &color)
    {
        bltString5x5(iXY(x, y), string, color);
    }

    // Blit a shadowed string of text.
    void bltStringShadowed(const iXY &pos, const char *string, const PIX &textColor, const PIX &shadowColor);
    void bltStringShadowed(int x, int y, const char *string, const PIX &textColor, const PIX &shadowColor)
    {
        bltStringShadowed(iXY(x, y), string, textColor, shadowColor);
    }

    // Blits a string of text and centers it horizontally and vertically on the screen.
    void bltStringCenter(const char *string, PIX color);

    void bltStringShadowedCenter(const char *string, PIX foreground, PIX background);
    void bltStringCenteredAtPoint(const iXY &pos, const char *string, const PIX &color);
    void bltStringCenteredInRect(const iRect &rect, const char *string, const PIX &color);

    void loadBMP(const char *fileName, bool needAlloc = true, void *returnPalette = 0);

    void mapFromPalette(const char* oldPalette);

    void drawBoxCorners(const iRect &rect, int cornerLength, PIX color) const;
    inline void drawBoxCorners(const iXY &min, const iXY &max, int cornerLength, PIX color) const
    {
        drawBoxCorners(iRect(min.x, min.y, max.x, max.y), cornerLength, color);
    }
    inline void drawBoxCorners(int minX, int minY, int maxX, int maxY, int cornerLength, PIX color) const
    {
        drawBoxCorners(iRect(minX, minY, maxX, maxY), cornerLength, color);
    }
    inline void drawBoxCorners(int cornerLength, PIX color) const
    {
        drawBoxCorners(iRect(0, 0, pix.x - 1, pix.y - 1), cornerLength, color);
    }

    void drawLookupBorder(const PIX table[]) const;

    static int getFontHeight();
    static int getTextLength(const char* text);
}; // end Surface

#endif // __Surface_HPP__
