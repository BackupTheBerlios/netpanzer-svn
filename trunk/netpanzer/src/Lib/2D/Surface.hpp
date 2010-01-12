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
#include <vector>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "Types/iXY.hpp"
#include "Types/iRect.hpp"
#include "Core/CoreTypes.hpp"

#include "SDL.h"

class Palette;
class Surface;
struct lua_State;

typedef std::vector<Surface*> SurfaceList;

/////////////////////////////////////////////////////////////////////////////
// class Surface
/////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------
class Surface
{
public:
    static int getTotalSurfaceCount()   { return totalSurfaceCount; }
    static int getTotalByteCount()      { return totalByteCount; }
    static unsigned int getFontHeight();
    static int getTextLength(const char* text);
    static int getTextLength(const std::string& text)
    {
        return getTextLength(text.c_str());
    }

    Surface();
    Surface(unsigned int w, unsigned int h, unsigned int nframes);
    Surface(unsigned int w, unsigned int h, unsigned int nframes, int bpp);

    ~Surface(); // no inheritance please
    
    void freeFrames();

    void create(unsigned int w, unsigned int h, unsigned int nframes);

    void setOffset(const iXY &o) { offset = o; }
    void setOffsetX(int ox)      { offset.x = ox; }
    void setOffsetY(int oy)      { offset.y = oy; }
    void setFPS(unsigned int f)  { fps = f; }
    void setOffsetCenter();
    bool setPixelsFromRawData( Uint8 * rawdata, int buffer_length );
    bool getRawDataFromPixels( Uint8 * rawdata, int buffer_length );

    // Accessor functions.
    unsigned int getWidth()  const { return twidth; }
    unsigned int getHeight() const { return theight; }
    unsigned int getPitch()  const { return tpitch; }
    
    iXY     getCenter()     const { return iXY(getWidth()>>1, getHeight()>>1); }
    int     getCenterX()    const { return getWidth()>>1; }
    int     getCenterY()    const { return getHeight()>>1; }

    unsigned int getNumFrames() const { return frames.size(); }

    float  getFPS()         const { return fps; }
    bool   getDoesExist()   const { return cur_frame!=0; }
    int    getCurFrame ()   const { return (int) curFrame; }
    iXY    getOffset()      const { return offset; }
    int    getArea()        const { return getWidth() * getHeight(); }

    inline void putPixel(const unsigned int x, const unsigned int y, const Uint32 color)
    {
        if ( x >= getWidth() || y >= getHeight())
            return;
        (*pixPtr(x,y)) = color;
    }

    inline void putPixel(const unsigned int x, const unsigned int y,
                         const Uint8 r, const Uint8 g, const Uint8 b)
    {
        putPixel(x, y, SDL_MapRGB(cur_frame->format, r, g, b));
    }

    inline Uint32 getPixel(const unsigned int x, const unsigned int y) const
    {
        return *pixPtr(x,y);
    }

    void drawRect(iRect bounds, const IntColor color);
    void fillRect(iRect bounds, const IntColor color);

    void linkData(Surface &other);

    void blt(Surface &dest, int x, int y) const;
    void bltBlend(Surface &dest, int x, int y, Uint8 alpha) const;
    void bltSolid(Surface &dest, int x, int y) const;
    void bltTrans(Surface &dest, int x, int y) const;
    void bltScale(const Surface &source, const iRect &destRect);
    void bltLookup(const iRect &destRect);
    void bltRect(SDL_Rect *sourceRect, Surface &dest, int x, int y);

    void createShadow(Surface &other);

    void drawHLine(int x1, int y,  int x2, const IntColor color);
    void drawVLine(int x,  int y1, int y2, const IntColor color);

    void drawLine(int x1, int y1, int x2, int y2, const IntColor color);

    void drawLine(const iXY &a, const iXY &b, const IntColor color)
    {
        drawLine(a.x, a.y, b.x, b.y, color);
    }

    // Surface Effects.
    void drawButtonBorder(iRect bounds, IntColor topLeftColor, IntColor bottomRightColor);
    void drawButtonBorder(IntColor topLeftColor, IntColor bottomRightColor)
    {
        drawButtonBorder(iRect(0, 0, getWidth(), getHeight()), topLeftColor, bottomRightColor);
    }

    void drawWindowsBorder();

    void fill(const IntColor color);
    void copy(const Surface &source);

    int nextFrame();

    void setFrame(const float &frameNum)
    {
        cur_frame = frames[(int)frameNum];
        mem = (Uint8 *)cur_frame->pixels;
    }

    void scale(unsigned int x, unsigned int y);

    // Text rendering functions
    void renderText(const char *str, IntColor color, IntColor bgcolor);
    void renderShadowedText(const char *str, IntColor color, IntColor bgcolor, IntColor shadowcolor);

    // Blit a single character of text.
    void bltChar8x8(int x, int y, unsigned char character, const IntColor color);
    void bltString(int x, int y, const char * str, const IntColor color);
    void bltStringInBox(const iRect &rect, const char *string, IntColor color, int gapSpace = 14, bool drawBox = false);

    // Blit a shadowed string of text.
    void bltStringShadowed(int x, int y, const char *str, const IntColor textColor, const IntColor shadowColor);

    // Blits a string of text and centers it horizontally and vertically on the screen.
    void bltStringCenter(const char *string, IntColor color);

    void bltStringShadowedCenter(const char *string, IntColor foreground, IntColor background);
    void bltStringCenteredInRect(const iRect &rect, const char *string, const IntColor color);

    void loadBMP(const char *fileName);
    void loadPNG(const char *fileName);
    void loadPNGSheet(const char * fileName, int width, int height, int count);

    void drawBoxCorners(const iRect &rect, int cornerLength, IntColor color);
    void drawBoxCorners(const iXY &min, const iXY &max,
            int cornerLength, IntColor color)
    {
        drawBoxCorners(iRect(min.x, min.y, max.x, max.y), cornerLength, color);
    }
    void drawBoxCorners(int cornerLength, IntColor color)
    {
        drawBoxCorners(iRect(0, 0, getWidth() - 1, getHeight() - 1), cornerLength, color);
    }

    void brigthenFrames();
    void setColorkey();
    void setAlpha(unsigned int alpha = 128);
    void optimize();

    // for scripts
    // pointer is: pointer to a surface pointer;
    static int loadPNGSheetPointer(lua_State *L, void *v);

private:
    static SDL_Surface * blackScreen;
    static int totalSurfaceCount;  // The number of surfaces alive.
    static int totalByteCount;     // The number of bytes of the surfaces alive.

    friend class ScreenSurface;
    friend class PackedSurface;
    friend class SDLVideo;

    Surface(const Surface& );
    Surface& operator=(const Surface& );

    std::vector<SDL_Surface*> frames;
    SDL_Surface * cur_frame;
    Uint8   *mem;       // Pointer to upperleft most pixel

    void alloc(unsigned int w, unsigned int h, int nframes, int bpp, bool alpha=false);

    inline Uint32 *pixPtr(const unsigned int x, const unsigned int y) const
    {
//        assert((y * getPitch() + x) < getPitch() * getHeight());
        return (Uint32*)(mem + (y * getPitch()) + (x * 4));
    }

    iXY   offset;     // Used like a hot spot for drawing.
    float fps;        // The speed to change the frames. Make sure TimerInterface::getTimeSlice() is being updated.
    unsigned int twidth;

    unsigned int theight;
    unsigned int tpitch;     // Number of bytes from one row to the next.

    float curFrame;   // Current frame of frameCount.

    void        reset();

}; // end Surface

#endif // __Surface_HPP__
