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
#include "Span.hpp"

using std::swap;
using std::min;
using std::max;

#ifdef MSVC
#pragma pack (1)
#endif
struct SpanHead
{
    unsigned short x1;
    unsigned short len;
}
__attribute__((packed));
#ifdef MSVC
#pragma pack ()
#endif

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

Surface ascii8x8(8, 8, 128);

int Surface::totalSurfaceCount = 0;
int Surface::totalByteCount    = 0;

// Surface
//---------------------------------------------------------------------------

Surface::Surface() : frames(0), thisFrame(0)
{
    numFrames   = 0;
    curFrame    = 0.0;
    fps         = 0;
    offset.zero();

    totalSurfaceCount++;
    totalByteCount += sizeof(Surface);
} // end Surface::Surface

// Surface
//---------------------------------------------------------------------------
Surface::Surface(unsigned int w, unsigned int h, unsigned int nframes)
    : frames(0), thisFrame(0)
{
    numFrames   = 0;
    curFrame    = 0.0;
    fps         = 0;
    offset.zero();
    
    alloc( w, h, nframes);

    totalSurfaceCount++;
    totalByteCount += sizeof(Surface);
} // end Surface::Surface

// ~Surface
//---------------------------------------------------------------------------
Surface::~Surface()
{
    freedata();
    
    totalSurfaceCount--;
    assert(totalSurfaceCount >= 0);

    totalByteCount -= sizeof(Surface);
    assert(totalByteCount >= 0);
} // end Surface::~Surface

//---------------------------------------------------------------------------
void Surface::freedata()
{
    if ( frames ) {
        for ( unsigned int n=0; n<numFrames; n++ ) {
            SDL_FreeSurface(frames[n]);
        }
        delete frames;
        frames=0;
    }

    thisFrame  = 0;
    
    numFrames = 0;
}

// setOffsetCenter
//---------------------------------------------------------------------------
// Purpose: Set the offset to the center of the image.
//---------------------------------------------------------------------------
void Surface::setOffsetCenter()
{
    offset = iXY(-(getWidth()>>1), -(getHeight()>>1));
} // end Surface::setOffsetCenter

// alloc
//---------------------------------------------------------------------------
// Purpose: Allocates memory for the surface, while setting the pix.x, pix.y,
//          xCenter, yCenter, and stride.  If successful, true is returned,
//          otherwise false is returned.
//---------------------------------------------------------------------------
void
Surface::alloc(unsigned int w, unsigned int h, int nframes)
{
    assert(this != 0);

    freedata();
    
    frames = new SDL_Surface*[nframes];
    for ( int n=0; n<nframes; n++ ) {
        frames[n] = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 8, 0 ,0 ,0, 0);
        if ( !frames[n] ) {
            n--;
            while (n >= 0) {
                SDL_FreeSurface(frames[n]);
                n--;
            }
            throw Exception("Can't Create Surface");
        }
        SDL_SetColors(frames[n],Palette::color,0,256);
    }
    numFrames           = nframes;
    thisFrame           = frames[0];

} // end Surface::alloc

// setTo
//---------------------------------------------------------------------------
// Purpose: Maps a Surface's coordinates to an existing surface.  This can
//          save you from having to allocate memory for every single surface.
//          You can just draw everything onto one surface in the given bounds.
//---------------------------------------------------------------------------
void Surface::setTo(const Surface &source, iRect bounds)
{
    freedata();
    
    frames = new SDL_Surface*[source.numFrames];
    for (unsigned int n=0; n<source.numFrames; n++) {
        frames[n] = SDL_CreateRGBSurfaceFrom(
                        (Uint8 *)source.frames[n]->pixels + (bounds.min.y*source.getPitch())+(bounds.min.x*source.frames[n]->format->BytesPerPixel),
                        bounds.getSizeX(),
                        bounds.getSizeY(),
                        source.frames[n]->format->BitsPerPixel,
                        source.frames[n]->pitch,
                        source.frames[n]->format->Rmask,
                        source.frames[n]->format->Gmask,
                        source.frames[n]->format->Bmask,
                        source.frames[n]->format->Amask);
        if ( source.frames[n]->format->BytesPerPixel == 1) 
            SDL_SetColors(frames[n],source.frames[n]->format->palette->colors,
                                0,
                                source.frames[n]->format->palette->ncolors);
                        
    }
        
    //orderCoords(bounds);

    curFrame   = source.curFrame;
    thisFrame  = frames[(unsigned int)curFrame];
    numFrames  = source.getNumFrames();
    fps        = source.getFPS();
    
} // end Surface::setTo

// setTo
//---------------------------------------------------------------------------
// Purpose: Maps the calling surface to some specified coordinates of the
//          another Surface.
//---------------------------------------------------------------------------
void Surface::setTo(const Surface &source)
{
    freedata();

    frames = new SDL_Surface*[source.numFrames];
    for (unsigned int n=0; n<source.numFrames; n++) {
        frames[n] = source.frames[n];
        frames[n]->refcount++;
    }

    curFrame   = source.curFrame;
    thisFrame  = frames[(unsigned int)curFrame];
    numFrames  = source.getNumFrames();
    fps        = source.getFPS();
    offset     = source.getOffset();
} // end Surface::setTo

// blt
//---------------------------------------------------------------------------
// Purpose: Puts the surface onto the destination while performing clipping
//          on the bounds of the object.
//---------------------------------------------------------------------------
void Surface::blt(Surface &dest, int x, int y) const
{
    // Add in the offset factor.
    x+=offset.x;
    y+=offset.y;

    SDL_Rect drect= {x,y,0,0};
    SDL_BlitSurface(thisFrame, 0, dest.thisFrame, &drect);

} // end Surface::blt

void
Surface::setAllColorKey(Uint32 c)
{
    for (unsigned int n=0; n<numFrames; n++) {
        SDL_SetColorKey(frames[n], SDL_SRCCOLORKEY|SDL_RLEACCEL, c);
    }
}

void Surface::drawHLine(int x1, int y, int x2, Uint32 color)
{
    if (x1>x2) {
        int xx = x2;
        x2=x1;
        x1=xx;
    }
    SDL_Rect r = {x1,y,x2-x1,1};
    SDL_FillRect(thisFrame,&r,color);
}

void Surface::drawVLine(int x, int y1, int y2, Uint32 color)
{
    if (y1>y2) {
        int yy = y2;
        y2=y1;
        y1=yy;
    }
    SDL_Rect r = {x,y1,1,y2-y1};
    SDL_FillRect(thisFrame,&r,color);
} // end Surface::drawVLine

void Surface::fill(Uint32 color)
{
    SDL_FillRect(thisFrame,0,color);
}

void Surface::fillRect(iRect bounds, Uint32 color)
{
    SDL_Rect r = {bounds.min.x, bounds.min.y, bounds.getSizeX(), bounds.getSizeY()};
    SDL_FillRect(thisFrame, &r, color);
} // end Surface::fillRect

// drawRect
//---------------------------------------------------------------------------
// Purpose: Draws a rectagle in the specified color on the calling Surface.
//---------------------------------------------------------------------------
void Surface::drawRect(iRect bounds, Uint32 color)
{
    drawHLine(bounds.min.x,   bounds.min.y,   bounds.max.x, color);
    drawHLine(bounds.min.x,   bounds.max.y,   bounds.max.x+1, color);
    drawVLine(bounds.min.x,   bounds.min.y,   bounds.max.y, color);
    drawVLine(bounds.max.x,   bounds.min.y,   bounds.max.y, color);
} // end Surface::drawRect

// drawLine
//---------------------------------------------------------------------------
// Purpose: Draws a drawLine with any slope.
//---------------------------------------------------------------------------
void Surface::drawLine(int x1, int y1, int x2, int y2, Uint32 color)
{
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
    freedata();
    
    frames = new SDL_Surface*[source.numFrames];
    for (unsigned int n=0; n<source.numFrames; n++) {
        frames[n] = SDL_CreateRGBSurface( SDL_SWSURFACE,
                        source.frames[n]->w,
                        source.frames[n]->h,
                        source.frames[n]->format->BitsPerPixel,
                        0,0,0,0);
                        
        if ( source.frames[n]->format->BytesPerPixel == 1) 
            SDL_SetColors(frames[n],source.frames[n]->format->palette->colors,
                                    0,
                                    source.frames[n]->format->palette->ncolors);
                                    
        SDL_BlitSurface(source.frames[n], 0, frames[n], 0);
    }
        
    curFrame   = 0.0;
    thisFrame  = frames[0];
    numFrames  = source.getNumFrames();
    fps        = source.getFPS();
    
} // end Surface::copy

// rotate
//---------------------------------------------------------------------------
// Purpose: Rotates a surface.  Accepts 0..360 integers for the degrees.
//---------------------------------------------------------------------------
void Surface::rotate(int angle)
{
    assert(this != 0);
    int center_x = getWidth()>>1;
    int center_y = getHeight()>>1;

    for (unsigned int i = 0; i < getNumFrames(); i++) {
        setFrame(i);

        if (angle % 360 != 0) {
            Surface tempSurface(getWidth(), getHeight(), 1);

            float angleRadians = -float(angle) / float(180.0 / M_PI);
            float cosAngle     = cos(angleRadians);
            float sinAngle     = sin(angleRadians);

            int index   = 0;

            for (int y = -center_y; y < center_y; y++) {
                for (int x = -center_x; x < center_x; x++) {
                    int xSource = int((x * cosAngle - y * sinAngle) + center_x);
                    int ySource = int((y * cosAngle + x * sinAngle) + center_y);

                    if ((xSource >= 0) && ((unsigned int)xSource < getWidth()) && (ySource >= 0) && ((unsigned int)ySource < getHeight())) {
                        tempSurface.putPixel(index % getWidth(), index / getHeight(), getPixel(xSource, ySource));
                    } else {
                        // Set the pixel transparent
                        tempSurface.putPixel(index % getWidth(), index / getHeight(), 0);
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
void Surface::scale(unsigned int x, unsigned int y)
{
    if ( !x || !y || (x==getWidth() && y==getHeight()))
        return;
        
    // Create a temporary surface to scale the image onto.
    Surface tempSurface(x, y, getNumFrames());

    iRect r(0, 0, x, y);

    // Go through all the frames of the surface.
    unsigned int n;
    for (n = 0; n < tempSurface.getNumFrames(); n++) {
        tempSurface.setFrame(n);
        Surface::setFrame(n);
        tempSurface.bltScale(*this, r);
    }

    SDL_Surface **t = frames;
    frames=tempSurface.frames;
    tempSurface.frames=t;
    setFrame((unsigned int)curFrame);

} // end Surface::scale

//---------------------------------------------------------------------------
void Surface::bltScale(const Surface &source, const iRect &destRect)
{
    iXY min = destRect.min + source.offset;
    iXY max = destRect.max + source.offset;

    if ((unsigned int)min.x >= getWidth()) return;
    if ((unsigned int)min.y >= getHeight()) return;

    // Something is overlapping, so we need to verify that both
    // surfaces are valid.

    size_t pixelsPerRow = max.x-min.x;
    size_t numRows      = max.y-min.y;

    int srcX1 = 0;
    int srcY = 0;

    // FIXME - if inverted bitmap, then flip x1 and y to right or botom edges

    // Compute source deltas
    int xSrcDelta = int((float(source.getWidth()) / float(max.x - min.x)) * 65536.0);
    int ySrcDelta = int((float(source.getHeight()) / float(max.y - min.y)) * 65536.0);

    PIX *dRow = (PIX *)pixPtr(0,0);
    // CLIP LEFT
    if (min.x < 0) {
        pixelsPerRow += min.x;
        srcX1 -= min.x*xSrcDelta;
    }	else {
        dRow += min.x;
    }

    // CLIP RIGHT
    if ((unsigned int)max.x > getWidth()) {
        pixelsPerRow -= max.x - getWidth();
    }

    // CLIP TOP
    if (min.y < 0) {
        numRows += min.y;
        srcY   -= min.y*ySrcDelta;
    }	else {
        dRow += (min.y*getPitch());
    }

    // CLIP BOTTOM
    if ((unsigned int)max.y > getHeight()) {
        numRows -= max.y-getHeight();
    }

    // Check to make sure the clipping is working.
    if (pixelsPerRow <= 0) return;
    if (numRows <= 0) return;

    float xdelta = float(source.getWidth()) / float(max.x - min.x);
    for (size_t yCount = 0 ; yCount < numRows ; yCount++) {
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

void
Surface::setColors(const std::string& paletteName)
{
    std::string lp = Palette::getName();
    if ( lp != paletteName )
        Palette::loadACT(paletteName);
        
    for (unsigned int fr=0;fr<getNumFrames();fr++)
        SDL_SetColors(frames[fr], (SDL_Color*)Palette::color,0,256);
        
    if ( lp != paletteName )
        Palette::loadACT(lp);
}

void
Surface::setAllColorsBlack()
{
    if ( ! frames ) {
        LOGGER.warning("There are no frames..");
        return;
    }
    SDL_Color c[256];
    SDL_memset(c,0,sizeof(c));
    for (unsigned int n=0; n<numFrames; n++) {
        SDL_SetColors(frames[n], c, 0, 256);
    }
}
// initFont
//---------------------------------------------------------------------------
// Purpose: Load all the characters into a surface of 128 frames.  Then the
//          characters can be accesed by changing the frame appropriately.
//---------------------------------------------------------------------------
void initFont()
{
    try {
        // already initialized at 8x8x128
        // NOTE: Make sure the file size is 128 characters.
        char charfilename[] = "pics/chars8x8.raw";

        std::auto_ptr<filesystem::ReadFile> file(
                filesystem::openRead(charfilename));
                
        for (unsigned int y = 0; y < ascii8x8.getHeight(); y++) {
            for (unsigned int curChar = 0; curChar < ascii8x8.getNumFrames();
		    curChar++) {
                ascii8x8.setFrame(curChar);
                int yOffset = y * ascii8x8.getPitch();
                file->read(ascii8x8.getMem() + yOffset, ascii8x8.getWidth(), 1);
            }
        }

        ascii8x8.setAllColorKey(0);
    } catch(std::exception& e) {
	throw Exception("Error while reading 8x8 font: %s",
		e.what());
    }

} // Surface::initFont

unsigned int
Surface::getFontHeight()
{
    return ascii8x8.getHeight();
}

int Surface::getTextLength(const char* text)
{
    return ascii8x8.getWidth() * SDL_strlen(text);
}

// bltChar8x8
//---------------------------------------------------------------------------
// Purpose: Blits the specied rom character to the screen at the specified
//          location.
//---------------------------------------------------------------------------
void Surface::bltChar8x8(int x, int y, unsigned char character, const PIX &color)
{
    if (character >= ascii8x8.getNumFrames())
        return;

    ascii8x8.setFrame(character);
    SDL_SetColors(ascii8x8.thisFrame, &Palette::color[color],255,1);
    ascii8x8.blt(*this, x, y);
} // end Surface::bltChar8x8

// bltString
//---------------------------------------------------------------------------
// Purpose: Blits the specified string of text to the screen by making
//          calls to blitChar for each character of the string. Does not
//          handle wrapping.
//---------------------------------------------------------------------------
void Surface::bltString(int x, int y, const char * str, const Uint8 &color)
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
void Surface::bltStringShadowed(int x, int y, char const *str, const Uint8 &textColor, const Uint8 &shadowColor)
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
void Surface::bltStringCenter(const char *string, PIX color)
{
    bltString(  (getWidth() - (SDL_strlen(string) * ascii8x8.getWidth())) / 2,
                (getHeight() - getFontHeight()) / 2,
                string, color);

} // end Surface::bltStringCenter

// bltStringShadowedCenter
//---------------------------------------------------------------------------
// Purpose: Blits a string of text and centers it horizontally and vertically
//          on the screen. Does not handle wrapping.
//---------------------------------------------------------------------------
void Surface::bltStringShadowedCenter(const char *string, PIX foreground, PIX background)
{
    bltStringShadowed((getWidth() - (SDL_strlen(string) * ascii8x8.getWidth())) / 2,
                      (getHeight() - getFontHeight()) / 2,
                      string, foreground, background);

} // end Surface::bltStringShadowedCenter

// bltStringCenteredInRect
//---------------------------------------------------------------------------
// Purpose: Blits the string centered inside the specified rectangle.
//---------------------------------------------------------------------------
void Surface::bltStringCenteredInRect(const iRect &rect, const char *string, const PIX &color)
{
    int length = SDL_strlen(string);

    iXY destPos;
    destPos.x = rect.min.x + (rect.getSizeX() - (length * ascii8x8.getWidth())) / 2;
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
        thisFrame = frames[0];
        return 0;
    }

    thisFrame=frames[(unsigned int)curFrame];
    return 1;
}

// loadAllBMPInDirectory
//---------------------------------------------------------------------------
int Surface::loadAllBMPInDirectory(const char *path)
{
    char** list = filesystem::enumerateFiles(path);
    
    std::vector<std::string> filenames;
    
    for(char** file = list; *file != 0; file++) {
        std::string name = path;
        name += *file;
        if(name.find(".bmp") != std::string::npos)
            filenames.push_back(name);
    }

    filesystem::freeList(list);

    std::sort(filenames.begin(), filenames.end());

    // XXX remove alloc and do byhand
    alloc(1,1,filenames.size());

    // Now load in the sorted BMP names.
    for (size_t i = 0; i < filenames.size(); i++) {
        setFrame(i);
        loadBMP(filenames[i].c_str());
    }

    return 1;
} // end loadAllBMPInDirectory

void Surface::loadBMP(const char *fileName)
{
    filesystem::ReadFile *f=0;
    try {
        f=filesystem::openRead(fileName);
    } catch(...) {
        throw;
    }
    // SDL_LoadBMP_RW will close the RWOps and free the file;
    SDL_Surface *s = SDL_LoadBMP_RW(f->getSDLRWOps(),1);
    //freedata();
    if ( !numFrames ) {
        numFrames = 1;
        curFrame = 0.0;
        frames = new SDL_Surface*;
        frames[0] = s;
        thisFrame = s;
    } else {
        SDL_free(thisFrame);
    }
    frames[(unsigned int)curFrame]=s;
    thisFrame=s;
}

void Surface::loadPAK(const std::string& filename)
{
    int  *rowOffsetTable = 0;
    Uint8 *packedDataChunk = 0;

    try {
        std::auto_ptr<filesystem::ReadFile> file(filesystem::openRead(filename));

        freedata();
        Sint32 version = file->readSLE32();
        if (version < 1)
            throw Exception("Invalid PAK file version: %d", version);
        if (version > 1) // this in current and only one, will get rid of paks
            throw Exception("PAK file version of '%s' is newer(%d) than "
                "the currently supported version(%d)",
            filename.c_str(), version, 1);

        Uint32 sizex = file->readSLE32();
        Uint32 sizey = file->readSLE32();
        //center = pix / 2;

        Sint32 t;
        t=file->readSLE32();
        numFrames = t;
        // should be done like following but this isn't backward compatible to
        // the existing files :-/
        //fps = float(file->readSLE32()) / 65536;
        // XXX is this correct?!?
        //Sint32 fpsint = file->readSLE32();
        //fps = * ((float*) (void*) &fpsint);
        fps=file->readSLE32();

        offset.x = file->readSLE32();
        offset.y = file->readSLE32();
        
        rowOffsetTable = (int *) SDL_malloc((sizey * numFrames + 1) * sizeof(*rowOffsetTable));
        if (rowOffsetTable == 0)
            throw Exception(
            "ERROR: Unable to allocate rowTableOffset for loadPAK.");
        for(unsigned int i=0;i<(sizey * numFrames+1);i++) {
            rowOffsetTable[i] = file->readSLE32();
        }
    
        packedDataChunk = (Uint8 *)SDL_malloc(rowOffsetTable[sizey*numFrames]);
        if (packedDataChunk == 0) {
            throw Exception(
            "ERROR: Unable to allocate packedDataChunk for loadPAK.");
        }
        file->read(packedDataChunk, rowOffsetTable[sizey*numFrames], 1);

        // Add size of rowTableOffset.
        //totalByteCount += (pix.y * frameCount + 1) * sizeof(*rowOffsetTable);

        // Some of the surfaces has an offset too big (millions)
        //offset.x=0;
        //offset.y=0;
        //LOGGER.warning("load packed %s: v=%d,x=%d,y=%d,fc=%d,fps=%d,ox=%d,oy=%d", filename.c_str(),version, pix.x, pix.y, frameCount, fps,offset.x, offset.y);
        
        // Add size of packedDataChunk.
        //totalByteCount += pix.y * frameCount;
        
        frames = new SDL_Surface*[numFrames];
        for (unsigned int nf=0; nf<numFrames; nf++) {
            frames[nf]=SDL_CreateRGBSurface(SDL_SWSURFACE, sizex, sizey, 8, 0, 0, 0, 0);
            if ( ! frames[nf] )
                throw Exception("PacketSurface->load: can't create surface");
            
            SDL_SetColors(frames[nf], Palette::color, 0, 256);
            SDL_FillRect(frames[nf], NULL, 0);
            
            Uint8 * data;
            Uint8 * curpixel = (Uint8 *)frames[nf]->pixels;
            data = packedDataChunk+rowOffsetTable[nf*sizey];

            for (unsigned int y=0; y<(unsigned int)sizey; y++) {
                Uint8 * endData = packedDataChunk+rowOffsetTable[(nf*sizey)+y+1];
                while ( data < endData ) {
                    Uint16 xstart = data[0] | (data[1] << 8);
                    Uint16 xlen = data[2] | (data[3] << 8);
                    data+=4;
                    SDL_memcpy(&curpixel[(y*frames[nf]->pitch)+xstart], data, xlen);
                    data+=(xlen+3) & ~3;
                }
            }
            SDL_SetColorKey(frames[nf],SDL_SRCCOLORKEY|SDL_RLEACCEL,0);
        }
        thisFrame=frames[0];
        curFrame=0.0;
        SDL_free(rowOffsetTable);
        SDL_free(packedDataChunk);

	
    } catch(std::exception& e) {
	throw Exception("Error while reading pakfile '%s': %s",
	    filename.c_str(), e.what());
    }
}

// bltStringInBox
//--------------------------------------------------------------------------
void Surface::bltStringInBox(const iRect &rect, const char *string, PIX color, int gapSpace, bool drawBox)
{
    if (drawBox) {
        drawRect(rect, Color::yellow);
    }

    iXY pos(rect.min);

    int  totalLength = SDL_strlen(string);
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
            pos.x += ascii8x8.getWidth();
            length++;
        }

        char strBuf[256];
        SDL_memset(strBuf, 0, sizeof(strBuf));
        int strBufLength = 0;

        while (!SDL_isspace(string[length + strBufLength]) && string[length + strBufLength] != '\n') {
            if (string[length + strBufLength] == '\0') {
                done = true;
                break;
            }

            assert(strBufLength < 256);
            strBuf[strBufLength] = string[length + strBufLength];
            strBufLength++;
        }

        strBuf[strBufLength] = '\0';

        if ((int) (pos.x + SDL_strlen(strBuf) * ascii8x8.getWidth()) > rect.max.x) {
            pos.x = rect.min.x;
            pos.y += gapSpace;
        }

        bltString(pos.x, pos.y, strBuf, color);

        if (done) {
            return;
        }

        pos.x += SDL_strlen(strBuf) * ascii8x8.getWidth();

        length += strBufLength;
    }

} // end Surface::bltStringInBox

// drawBoxCorners
//--------------------------------------------------------------------------
// Purpose: Draws lines in the corners of the surface of the specified length
//          and color.
//--------------------------------------------------------------------------
void Surface::drawBoxCorners(const iRect &rect, int cornerLength, Uint32 color)
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

// loadAllPAKInDirectory
//---------------------------------------------------------------------------
int loadAllPAKInDirectory(const char *path, SurfaceList& paklist)
{
    char** list = filesystem::enumerateFiles(path);

    std::vector<std::string> filenames;
    for(char** file = list; *file != 0; file++) {
	std::string name = path;
	name += *file;
	if(name.find(".pak") != std::string::npos)
	    filenames.push_back(name);
    }

    filesystem::freeList(list);
   
    std::sort(filenames.begin(), filenames.end()); 

    // Now load in the sorted PAK names.
    for (size_t i = 0; i < filenames.size(); i++) {
        Surface* surface = new Surface;
        surface->loadPAK(filenames[i].c_str());
        paklist.push_back(surface);
    }

    return filenames.size();
} // end loadAllPAKInDirectory

