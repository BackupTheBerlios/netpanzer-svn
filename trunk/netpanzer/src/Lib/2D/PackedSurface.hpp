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

#include "iRect.hpp"
#include "iXY.hpp"
#include "ColorTable.hpp"
#include "cGrowList.hpp"

class Surface;
class PackedSurface;

int loadAllPAKInDirectory(const char *path, cGrowList <PackedSurface> &growList);

//--------------------------------------------------------------------------
class PackedSurface
{
public:

    static int totalDrawCount;     // The number of bytes of the surfaces alive.

    typedef void (*SPAN_FUNC)(const uint8_t *src, uint8_t *dest, int count);

    void pack(const Surface &src);

    void blt(Surface &dest, int x, int y) const;
    inline void blt(Surface &dest, iXY pos) const
    {
        blt(dest, pos.x, pos.y);
    }

    void bltBlend(Surface &dest, int x, int y, ColorTable &colorTable) const;
    inline void bltBlend(Surface &dest, iXY pos, ColorTable &colorTable) const
    {
        bltBlend(dest, pos.x, pos.y, colorTable);
    }

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

    inline void setFrame(float frameNum)
    {
        assert(frameNum >= 0.0);
        assert(frameNum < frameCount);
        curFrame = frameNum;
    }

    void setOffset(const iXY &offset);
    void setOffsetCenter();

    void load(const char *filename);
    void save(const char *filename) const;

    inline void setFPS(float fps)
    {
        PackedSurface::fps = fps;
    }

    inline float  getFPS() const
    {
        return fps;
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
    //inline iXY    getCenter() const { return center; }
    //inline int    getCenterX() const { return center.x; }
    //inline int    getCenterY() const { return center.y; }
    inline int    getFrameCount() const
    {
        return frameCount;
    }
    inline int    getCurFrame () const
    {
        return (int) curFrame;
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
    inline int    getArea() const
    {
        return pix.x * pix.y;
    }
    inline iRect  getRect() const
    {
        return iRect(0, 0, pix.x - 1, pix.y - 1);
    }

    inline int  *getRowOffsetTable() const
    {
        return rowOffsetTable;
    }
    inline uint8_t *getPackedDataChunk() const
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

#endif // end __PackedSurface_hpp__

