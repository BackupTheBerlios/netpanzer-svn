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

#include <vector>
#include <string>
#include <algorithm>

#include "PackedSurface.hpp"
#include "Surface.hpp"
#include "TimerInterface.hpp"
#include "Span.hpp"
#include "FileSystem.hpp"
#include "Exception.hpp"
#include "UtilInterface.hpp"

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

#define TRANSPIX 0

int PackedSurface::totalSurfaceCount = 0;  // The number of surfaces alive.
int PackedSurface::totalByteCount    = 0;  // The number of bytes of the surfaces alive.
int PackedSurface::totalDrawCount    = 0;  // The number of bytes of the surfaces alive.

// PAK file format revision history:
//--------------------------------------
// 1 MB Initial version


const int CURRENT_PAK_VERSION = 1;

//--------------------------------------------------------------------------
void PackedSurface::setOffset(const iXY &offset)
{
    PackedSurface::offset = offset;
}

//--------------------------------------------------------------------------
void PackedSurface::setOffsetCenter()
{
    PackedSurface::offset = pix / -2;
}

//--------------------------------------------------------------------------
void PackedSurface::free()
{
    if (myMem && rowOffsetTable != 0) {
        totalByteCount -= (pix.y * frameCount + 1) * sizeof(*rowOffsetTable);

        ::free(rowOffsetTable);

        //assert(totalByteCount >= 0);
    }
    if (myMem && packedDataChunk != 0) {
        totalByteCount -= pix.y * frameCount;

        ::free(packedDataChunk);

        //assert(totalByteCount >= 0);
    }

    reset();
}

//--------------------------------------------------------------------------
void PackedSurface::reset()
{
    pix             = 0;
    rowOffsetTable  = 0;
    packedDataChunk = 0;
    frameCount      = 0;
    curFrame        = 0.0;
    fps             = 30.0;
    myMem           = true;

    center.zero();
    offset.zero();
}

//--------------------------------------------------------------------------
void PackedSurface::pack(const Surface &source)
{
    free();

    pix        = source.getPix();
    offset     = source.getOffset();
    frameCount = source.getFrameCount();
    fps        = source.getFPS();

    rowOffsetTable = (int *) malloc((pix.y*frameCount + 1) * sizeof(*rowOffsetTable));
    if (rowOffsetTable == 0) {
        throw Exception("ERROR: Unable to allocate rowTableOffset for PackedSurface.");
    }

    int bytesAlloced = 0;
    packedDataChunk = 0;
    int curByteOffset = 0;

    float saveFrame = source.getCurFrame();

    for (int frame = 0 ; frame < frameCount ; ++frame) {
        ((Surface *)&source)->setFrame(frame);
        for (int y = 0 ; y < pix.y ; ++y) {
            rowOffsetTable[frame*pix.y + y] = curByteOffset;
            const PIX *rowPtr = source.rowPtr(y);

            int x = 0;
            while (x < pix.x) {

                // transparent pixels until first non-transparent pixel

                if (rowPtr[x] == TRANSPIX) {
                    x++;
                    continue;
                }

                // Now scan forward until we hit the end of the span,
                // either the right side of the bitmap or a transparent pixel

                int spanX1 = x;
                do {
                    x++;
                } while((x < pix.x) && (rowPtr[x] != TRANSPIX));
                int spanLen = x - spanX1;

                // Check if we need to grow buffer to add the span

                int newSize = (curByteOffset + sizeof(SpanHead) + spanLen*sizeof(PIX) + 3) & ~3;
                if (newSize > bytesAlloced) {
                    bytesAlloced = newSize + 16*1024;
                    packedDataChunk = (BYTE *)realloc(packedDataChunk, bytesAlloced);
                    if (packedDataChunk == 0) {
                        throw Exception("ERROR: Out of memory for packedDataChunk for PackedSurface.");
                    }
                }

                // Now write the data into the chunk.

                SpanHead *span = (SpanHead *)(packedDataChunk + curByteOffset);
                span->x1 = spanX1;
                span->len = spanLen;
                memcpy(span + 1, &rowPtr[spanX1], spanLen * sizeof(PIX));
                curByteOffset = newSize;
            }
        }
    }

    // Shove in final offset table entry, which we need to know the
    // length of data for the last row of the last frame

    rowOffsetTable[frameCount*pix.y] = curByteOffset;

    // Shrink buffer to the size we really need

    packedDataChunk = (BYTE *) realloc(packedDataChunk, curByteOffset);
    if (packedDataChunk == 0) throw Exception("Hell froze");

    // Restore source surface frame number, so the function
    // is logically const

    ((Surface *)&source)->setFrame(saveFrame);
}

//--------------------------------------------------------------------------
void PackedSurface::load(const char* filename)
{
    ReadFile* file = FileSystem::openRead(filename);

    free();
    int version;
    file->read(&version, sizeof(version), 1);
    if (version < 1) {
        delete file;
        throw Exception("Invalid PAK file version: %d", version);
    }
    if (version > CURRENT_PAK_VERSION) {
        delete file;
        throw Exception("PAK file version of '%s' is newer(%d) than "
                "the currently supported version(%d)",
		filename, version, CURRENT_PAK_VERSION);
    }
    file->read(&pix, sizeof(pix), 1);

    center = pix / 2;

    file->read(&frameCount, sizeof(frameCount), 1);
    file->read(&fps, sizeof(fps), 1);
    file->read(&offset, sizeof(offset), 1);
    rowOffsetTable = (int *) malloc((pix.y * frameCount + 1) * sizeof(*rowOffsetTable));
    if (rowOffsetTable == 0) {
        delete file;
        throw Exception("ERROR: Unable to allocate rowTableOffset for PackedSurface.");
    }
    file->read(rowOffsetTable, (pix.y*frameCount + 1)*sizeof(*rowOffsetTable), 1);
    packedDataChunk = (BYTE *)malloc(rowOffsetTable[pix.y*frameCount]);
    if (packedDataChunk == 0) {
        delete file;
        throw Exception("ERROR: Unable to allocate packedDataChunk for PackedSurface.");
    }
    if(file->read(packedDataChunk, rowOffsetTable[pix.y*frameCount], 1) != 1)
        throw Exception("error while reading %s.", filename);

    // Add size of rowTableOffset.
    totalByteCount += (pix.y * frameCount + 1) * sizeof(*rowOffsetTable);

    // Add size of packedDataChunk.
    totalByteCount += pix.y * frameCount;

    delete file;
}

//--------------------------------------------------------------------------
void PackedSurface::save(const char* filename) const
{
    WriteFile* file = FileSystem::openWrite(filename);

    int version = CURRENT_PAK_VERSION;
    file->write(&version, sizeof(version), 1);
    file->write(&pix, sizeof(pix), 1);
    file->write(&frameCount, sizeof(frameCount), 1);
    file->write(&fps, sizeof(fps), 1);
    file->write(&offset, sizeof(offset), 1);
    file->write(rowOffsetTable, (pix.y*frameCount + 1)*sizeof(*rowOffsetTable), 1);
    if (file->write(packedDataChunk, rowOffsetTable[pix.y*frameCount], 1) != 1) {
        delete file;
        throw Exception("error while writing '%s'.", filename);
    }

    delete file;
}

//--------------------------------------------------------------------------
void PackedSurface::blt(const Surface &dest, int destX, int destY) const
{
    totalDrawCount++;

    destX += offset.x;
    destY += offset.y;

    // Clip and trivial reject

    int needClipX = 0;

    iXY srcMax;
    srcMax.x = dest.getPixX() - destX;
    if (srcMax.x <= 0) return; // off right
    srcMax.y = dest.getPixY() - destY;
    if (srcMax.y <= 0) return; // off bottom

    iXY srcMin;
    if (destX < 0) {
        srcMin.x = -destX;
        if (srcMin >= pix.x) return; // off left
        needClipX = 1;
    } else {
        srcMin.x = 0;
    }
    if (destY < 0) {
        srcMin.y = -destY;
        if (srcMin.y >= pix.y) return; // off right
    } else {
        srcMin.y = 0;
    }

    if (srcMax.x > pix.x) {
        srcMax.x = pix.x;
    } else {
        needClipX = 1;
    }
    if (srcMax.y > pix.y) {
        srcMax.y = pix.y;
    }

    const int *table = &rowOffsetTable[int(curFrame)*pix.y];

    if (needClipX) {
        const BYTE *rowData = packedDataChunk + table[srcMin.y];
        PIX *destRowPtr = dest.rowPtr(destY + srcMin.y) + destX;
        for (int y = srcMin.y ; y < srcMax.y ; ++y) {

            const BYTE *rowEnd = packedDataChunk + table[y+1];

            // Search for first span which is not completely off to the left

            int x1, x2;
            SpanHead *span;
            const PIX *data;
            for (;;) {
                if (rowData >= rowEnd) goto nextRow;
                span = (SpanHead *)rowData;
                x1 = span->x1;
                x2 = x1 + span->len;
                rowData += (sizeof(*span) + span->len*sizeof(PIX) + 3) & ~3;
                if (x2 > srcMin.x) break;
            }

            // Clip against left edge

            data = (const PIX *)(span + 1);
            if (x1 < srcMin.x) {
                data += srcMin.x - x1;
                x1 = srcMin.x;
            }

            // Output spans to the screen until we hit the first
            // span which is partially or completely out to the
            // right

            for (;;) {
                if (x1 >= srcMax.x) {
                    rowData = rowEnd;
                    goto nextRow;
                }

                // Clip current span against right edge
                if (x2 > srcMax.x) {
                    memcpy(destRowPtr + x1, data, (srcMax.x-x1) * sizeof(PIX));
                    rowData = rowEnd;
                    goto nextRow;
                }

                memcpy(destRowPtr + x1, data, (x2-x1) * sizeof(PIX));

                if (rowData >= rowEnd) goto nextRow;
                span = (SpanHead *)rowData;
                x1 = span->x1;
                if (x1 >= srcMax.x) {
                    rowData = rowEnd;
                    goto nextRow;
                }
                x2 = x1 + span->len;
                data = (const PIX *)(span + 1);
                rowData += (sizeof(*span) + span->len*sizeof(PIX) + 3) & ~3;
            }

nextRow:
            assert(rowData == rowEnd);
            destRowPtr += dest.getStride();
        }
    } else {
        const BYTE *rowData = packedDataChunk + table[srcMin.y];
        PIX *destRowPtr = dest.rowPtr(destY + srcMin.y) + destX;
        for (int y = srcMin.y ; y < srcMax.y ; ++y) {

            const BYTE *rowEnd = packedDataChunk + table[y+1];

            while (rowData < rowEnd) {
                SpanHead *span = (SpanHead *)rowData;
                memcpy(destRowPtr + span->x1, span + 1, span->len * sizeof(PIX));
                rowData += (sizeof(*span) + span->len*sizeof(PIX) + 3) & ~3;
            }
            assert(rowData == rowEnd);
            destRowPtr += dest.getStride();
        }
    }

}

// setTo
//---------------------------------------------------------------------------
// Purpose: Maps a PackedSurface's coordinates to an existing surface.  This can
//          save you from having to allocate memory for every single PackedSurface.
//          You can just draw everything onto one PackedSurface in the given bounds.
//---------------------------------------------------------------------------
/*
void PackedSurface::setTo(const PackedSurface &source, iRect bounds)
{
	assert(this != 0);
 
	free();
	reset();
 
	orderCoords(bounds);
 
	frameCount = source.getFrameCount();
	fps        = source.getFPS();
	pix        = bounds.getSize();
	center     = ;
	myMem      = false;
	offset     = source.getOffset();
 
	int   frameCount;
	float fps;
	float curFrame;
	iXY   offset;
	iXY   center;
	iXY   pix;
	int  *rowOffsetTable;
	BYTE *packedDataChunk;
	bool  myMem;
 
} // end Surface::setTo
*/
// setTo
//---------------------------------------------------------------------------
// Purpose: Maps the calling PackedSurface to some specified coordinates of
//          the another PackedSurface.
//---------------------------------------------------------------------------
void PackedSurface::setTo(const PackedSurface &source)
{
    assert(this != 0);

    free();

    frameCount      = source.getFrameCount();
    fps             = source.getFPS();
    offset          = source.getOffset();
    center          = source.getCenter();
    pix             = source.getPix();
    rowOffsetTable  = source.getRowOffsetTable();
    curFrame        = source.getCurFrame();
    packedDataChunk = source.getPackedDataChunk();
    myMem           = false;

} // end Surface::setTo

// nextFrame
//---------------------------------------------------------------------------
// Purpose: Move to the next frame based off time and the fps.  Returns a 0 if it
//          was the last frame and 1 otherwise.
//---------------------------------------------------------------------------
int PackedSurface::nextFrame()
{
    curFrame += TimerInterface::getTimeSlice() * fps;

    if (curFrame >= frameCount) {
        curFrame = 0.0;
        return 0;
    }

    return 1;
}

// bltBlend
//--------------------------------------------------------------------------
void PackedSurface::bltBlend(const Surface &dest, int destX, int destY, ColorTable &colorTable) const
{
    totalDrawCount++;

    destX += offset.x;
    destY += offset.y;

    // Clip and trivial reject

    int needClipX = 0;

    iXY srcMax;
    srcMax.x = dest.getPixX() - destX;
    if (srcMax.x <= 0) return; // off right
    srcMax.y = dest.getPixY() - destY;
    if (srcMax.y <= 0) return; // off bottom

    iXY srcMin;
    if (destX < 0) {
        srcMin.x = -destX;
        if (srcMin.x >= pix.x) return; // off left
        needClipX = 1;
    } else {
        srcMin.x = 0;
    }
    if (destY < 0) {
        srcMin.y = -destY;
        if (srcMin.y >= pix.y) return; // off right
    } else {
        srcMin.y = 0;
    }

    if (srcMax.x > pix.x) {
        srcMax.x = pix.x;
    } else {
        needClipX = 1;
    }
    if (srcMax.y > pix.y) {
        srcMax.y = pix.y;
    }

    const BYTE *cTable = colorTable.getColorArray();
    const int  *table  = &rowOffsetTable[int(curFrame)*pix.y];

    if (needClipX) {
        const BYTE *rowData = packedDataChunk + table[srcMin.y];
        PIX *destRowPtr = dest.rowPtr(destY + srcMin.y) + destX;
        for (int y = srcMin.y ; y < srcMax.y ; ++y) {

            const BYTE *rowEnd = packedDataChunk + table[y+1];

            // Search for first span which is not completely off to the left

            int x1, x2;
            SpanHead *span;
            const PIX *data;
            for (;;) {
                if (rowData >= rowEnd) goto nextRow;
                span = (SpanHead *)rowData;
                x1 = span->x1;
                x2 = x1 + span->len;
                rowData += (sizeof(*span) + span->len*sizeof(PIX) + 3) & ~3;
                if (x2 > srcMin.x) break;
            }

            // Clip against left edge

            data = (const PIX *)(span + 1);
            if (x1 < srcMin.x) {
                data += srcMin.x - x1;
                x1 = srcMin.x;
            }

            // Output spans to the screen until we hit the first
            // span which is partially or completely out to the
            // right

            for (;;) {
                if (x1 >= srcMax.x) {
                    rowData = rowEnd;
                    goto nextRow;
                }

                // Clip current span against right edge
                if (x2 > srcMax.x) {
                    bltBlendSpan(destRowPtr + x1, data, srcMax.x - x1, cTable);

                    rowData = rowEnd;
                    goto nextRow;
                }

                bltBlendSpan(destRowPtr + x1, data, x2 - x1, cTable);

                if (rowData >= rowEnd) goto nextRow;
                span = (SpanHead *)rowData;
                x1 = span->x1;
                if (x1 >= srcMax.x) {
                    rowData = rowEnd;
                    goto nextRow;
                }
                x2 = x1 + span->len;
                data = (const PIX *)(span + 1);
                rowData += (sizeof(*span) + span->len*sizeof(PIX) + 3) & ~3;
            }

nextRow:
            assert(rowData == rowEnd);
            destRowPtr += dest.getStride();
        }
    } else {
        const BYTE *rowData = packedDataChunk + table[srcMin.y];
        PIX *destRowPtr = dest.rowPtr(destY + srcMin.y) + destX;
        for (int y = srcMin.y ; y < srcMax.y ; ++y) {

            const BYTE *rowEnd = packedDataChunk + table[y+1];

            while (rowData < rowEnd) {
                SpanHead *span = (SpanHead *)rowData;

                bltBlendSpan(destRowPtr + span->x1, (const PIX *)(span + 1), span->len, cTable);

                rowData += (sizeof(*span) + span->len*sizeof(PIX) + 3) & ~3;
            }
            assert(rowData == rowEnd);
            destRowPtr += dest.getStride();
        }
    }

} // end PackedSurface::bltBlend

// loadAllPAKInDirectory
//---------------------------------------------------------------------------
int loadAllPAKInDirectory(const char *path, cGrowList<PackedSurface> &growList)
{
    char** list = FileSystem::enumerateFiles(path);

    std::vector<std::string> filenames;
    for(char** file = list; *file != 0; file++) {
	std::string name = path;
	name += *file;
	if(name.find(".pak") != std::string::npos)
	    filenames.push_back(name);
    }

    FileSystem::freeList(list);
   
    std::sort(filenames.begin(), filenames.end()); 

    // Allocate enough slots into the growList.
    growList.setNum(filenames.size());

    // Now load in the sorted PAK names.
    for (size_t i = 0; i < filenames.size(); i++) {
        growList[i].load(filenames[i].c_str());
    }

    return 1;
} // end loadAllPAKInDirectory

