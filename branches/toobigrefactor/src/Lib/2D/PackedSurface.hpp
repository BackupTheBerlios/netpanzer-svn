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
#ifndef __PackedSurface_hpp__
#define __PackedSurface_hpp__

#include <string>
#include <stdint.h>

#include "Types/iRect.hpp"
#include "Types/iXY.hpp"
#include "ArrayUtil/PtrArray.hpp"

#include <assert.h>

class Surface;
class PackedSurface;

//--------------------------------------------------------------------------
class PackedSurface
{
private:
    PackedSurface(const PackedSurface&);
    void operator=(const PackedSurface&);

public:
    static int totalDrawCount;     // The number of bytes of the surfaces alive.

    void pack(const PtrArray<Surface>& src);

    void blt(Surface &dest, int x, int y) const;
    void bltFrame(Surface &dest, const unsigned frame, const int x, const int y) const;
    void bltBlendFrame(Surface &dest, const unsigned frame, const int x, const int y, const uint8_t * blendTable) const;
    
    void blt(Surface &dest, iXY pos) const
    {
        blt(dest, pos.x, pos.y);
    }

    void bltBlend(Surface &dest, int x, int y, const uint8_t * blendTable) const;

    void free();

    static int getTotalSurfaceCount()
    {
        return totalSurfaceCount;
    }
    static int getTotalByteCount()
    {
        return totalByteCount;
    }

    PackedSurface()
    {
        totalSurfaceCount++;
        totalByteCount += sizeof(PackedSurface);

        reset();
    }
    ~PackedSurface()
    {
        totalSurfaceCount--;
        totalByteCount -= sizeof(PackedSurface);

        free();
    }

    int nextFrame();

    void setFrame(float frameNum)
    {
        assert(frameNum >= 0.0);
        assert(frameNum < frameCount);
        curFrame = frameNum;
    }

    void setOffset(const iXY &offset);
    void setOffsetCenter();

    void load(const std::string& filename);
    void save(const std::string& filename) const;

    void setFPS(float fps)
    {
        PackedSurface::fps = fps;
    }

    float  getFPS() const
    {
        return fps;
    }
    iXY    getPix() const
    {
        return pix;
    }
    int    getWidth() const
    {
        return pix.x;
    }
    int    getHeight() const
    {
        return pix.y;
    }
    //iXY    getCenter() const { return center; }
    //int    getCenterX() const { return center.x; }
    //int    getCenterY() const { return center.y; }
    int    getFrameCount() const
    {
        return frameCount;
    }
    int    getCurFrame () const
    {
        return (int) curFrame;
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
    int    getArea() const
    {
        return pix.x * pix.y;
    }
    iRect  getRect() const
    {
        return iRect(0, 0, pix.x, pix.y);
    }

    int* getRowOffsetTable() const
    {
        return rowOffsetTable;
    }

    uint8_t* getPackedDataChunk() const
    {
        return packedDataChunk;
    }

    //void setTo(const PackedSurface &source, iRect bounds);
    void setTo(const PackedSurface &source);

protected:
    void reset();

    int   frameCount;
    float fps;
    float curFrame;
    iXY   offset;
    iXY   center;
    iXY   pix;
    int  *rowOffsetTable;
    uint8_t *packedDataChunk;
    bool  myMem;

    static int totalSurfaceCount;  // The number of surfaces alive.
    static int totalByteCount;     // The number of bytes of the surfaces alive.
};

//---------------------------------------------------------------------------

class PackedSurfaceList : public PtrArray<PackedSurface>
{
public:
    PackedSurfaceList()
    { }

    ~PackedSurfaceList()
    {
        for ( size_t n = 0; n < getLastIndex(); n++ )
            delete operator[](n);
    }
};

int loadAllPAKInDirectory(const char *path, PackedSurfaceList& list);

#endif // end __PackedSurface_hpp__

