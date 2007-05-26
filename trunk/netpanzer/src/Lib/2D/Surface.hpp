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

#include "Types/iXY.hpp"
#include "Types/iRect.hpp"
#include "Util/NoCopy.hpp"
#include "Util/Exception.hpp"

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
public:
    static int getTotalSurfaceCount()   { return totalSurfaceCount; }
    static int getTotalByteCount()      { return totalByteCount; }

    Surface();
    Surface(unsigned int w, unsigned int h, unsigned int nframes);

    virtual ~Surface();
    
    void setColorKey(Uint32 c) { SDL_SetColorKey(thisFrame, SDL_SRCCOLORKEY|SDL_RLEACCEL, c); }
    void setAllColorKey(Uint32 c);
    void setColors(const std::string& paletteName);
    void setAllColorsBlack();

    void setOffset(const iXY &o) { offset = o; }
    void setOffsetX(int ox)     { offset.x = ox; }
    void setOffsetY(int oy)     { offset.y = oy; }
    void setFPS(int f)          { fps = f; }
    void setOffsetCenter();
    
    void lockFrame() { if ( SDL_MUSTLOCK(thisFrame) ) SDL_LockSurface(thisFrame); }
    void unlockFrame() { if ( SDL_MUSTLOCK(thisFrame) ) SDL_UnlockSurface(thisFrame); }

    // Accessor functions.
    unsigned int getWidth()  const { return thisFrame->w; }
    unsigned int getHeight() const { return thisFrame->h; }
    unsigned int getPitch()  const { return thisFrame->pitch; }
    
    iXY     getCenter()     const { return iXY(getWidth()>>1, getHeight()>>1); }
    unsigned int getCenterX()    const { return getWidth()>>1; }
    unsigned int getCenterY()    const { return getHeight()>>1; }

    unsigned int getNumFrames() const { return numFrames; }
    
    int  loadAllBMPInDirectory(const char *path);
    void loadBMP(const char *fileName);
    void loadPAK(const std::string& filename);

    unsigned int getFPS()      const { return fps; }
    float        getCurFrame() const { return curFrame; }
    iXY          getOffset()   const { return offset; }
    int          getOffsetX()  const { return offset.x; }
    int          getOffsetY()  const { return offset.y; }
    Uint8 * getMem() const
    { 
        if ( ! thisFrame->pixels )
            assert("Surface::getMem() Error, surface must be locked"==0);
        return (PIX *)thisFrame->pixels;
    }

    iRect  getRect() const
    {
        return iRect(0, 0, getWidth() - 1, getHeight() - 1);
    }

    void putPixel(unsigned int x, unsigned int y, Uint32 color) 
    {
        if ( x >= getWidth() || y >= getHeight() )
            return;
            
        Uint8 * mp = getMem() + (y * thisFrame->pitch);
        switch (thisFrame->format->BytesPerPixel) {
            case 1:
                mp+=x;
                *mp=color;
                break;
            case 2:
                mp+=x<<1;
                *(Uint16 *)mp=color;
                break;
            case 3:
                throw Exception("putPixel: Not supported for 3 byte surfaces");
            case 4:
                mp+=x;
                *(Uint32 *)mp=color;
                break;
            default:
                throw Exception("putPixel here we are");
        }
//        *(pixPtr(x,y)) = color;
    }

    Uint32 getPixel(unsigned int x, unsigned int y) const
    {
        if ( x >= getWidth() || y >= getHeight() )
            return 0;
    
        Uint8 * mp = getMem() + (y * thisFrame->pitch);
        Uint32 val=0;
        switch (thisFrame->format->BytesPerPixel) {
            case 1:
                mp+=x;
                val=*mp;
                break;
            case 2:
                mp+=x<<1;
                val=*(Uint16 *)mp;
                break;
            case 3:
                throw Exception("getPixel: Not supported for 3 byte surfaces");
            case 4:
                mp+=x<<2;
                val=*(Uint32 *)mp;
                break;
            default:
                throw Exception("getPixel here we are");
        }
        return val;
    }

    void fill(Uint32 color);
    void fillRect(iRect bounds, Uint32 color);
    void drawRect(iRect bounds, Uint32 color);
    void drawHLine(int x1, int y,  int x2, Uint32 color);
    void drawVLine(int x,  int y1, int y2, Uint32 color);
    void drawLine(int x1, int y1, int x2, int y2, Uint32 color);

private:
    friend class ScreenSurface;
    SDL_Surface **frames;
    SDL_Surface *thisFrame;

    iXY   offset;     // Used like a hot spot for drawing.
    unsigned int fps;        // The speed to change the frames. Make sure TimerInterface::getTimeSlice() is being updated.
    unsigned int numFrames;
    float curFrame;   // Current frame of frameCount.

    static int totalSurfaceCount;  // The number of surfaces alive.
    static int totalByteCount;     // The number of bytes of the surfaces alive.
    
    void alloc(unsigned int w, unsigned int h, int nframes);
    void freedata();

    Uint8 * pixPtr(unsigned int x, unsigned int y) const
    {
        if ( ! thisFrame->pixels )
            throw Exception("Surface Needs to be locked");
            
        if ( x > getWidth() || y > getHeight() )
            throw Exception("Point out of Surface");
            
        return getMem() + (y * getPitch()) + (x*thisFrame->format->BytesPerPixel);
    }


public:
    
    void setTo(const Surface &s, iRect bounds);
    void setTo(const Surface &source);

    void blt(Surface &dest, int x, int y) const;
    void bltScale(const Surface &source, const iRect &destRect);

    // Surface Effects.
    void rotate(int angle);
    void copy(const Surface &source);

    int nextFrame();

    void setFrame(const unsigned int frameNum)
    {
        assert(frameNum < getNumFrames());
        curFrame=frameNum;
        thisFrame = frames[frameNum];
    }

    void scale(unsigned int x, unsigned int y);


    // Blit a single character of text.
    void bltChar8x8(int x, int y, unsigned char character, const PIX &color);
    void bltString(int x, int y, const char * str, const PIX& color);
    void bltStringInBox(const iRect &rect, const char *string, PIX color, int gapSpace = 14, bool drawBox = false);

    // Blit a shadowed string of text.
    void bltStringShadowed(int x, int y, const char *str, const PIX &textColor, const PIX &shadowColor);

    // Blits a string of text and centers it horizontally and vertically on the screen.
    void bltStringCenter(const char *string, PIX color);

    void bltStringShadowedCenter(const char *string, PIX foreground, PIX background);
    void bltStringCenteredInRect(const iRect &rect, const char *string, const PIX &color);


    void drawBoxCorners(const iRect &rect, int cornerLength, Uint32 color);

    static unsigned int getFontHeight();
    static int getTextLength(const char* text);
    static int getTextLength(const std::string& text)
    { return getTextLength(text.c_str()); } 
}; // end Surface

class SurfaceList : public std::vector<Surface*>
{
public:
    SurfaceList()
    { }

    ~SurfaceList()
    {
        std::vector<Surface*>::iterator i;
        for(i = begin(); i != end(); i++)
            delete *i;
    }
};

int loadAllPAKInDirectory(const char *path, SurfaceList& list);


#endif // __Surface_HPP__
