/*
Copyright (C) 2003 Matthias Braun <matze@braunis.de>
                                                                                
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

#include <memory>

#include "Surface.hpp"
#include "Exception.hpp"
#include "FileSystem.hpp"
#include "Tile.hpp"
#include "iXY.hpp"

// A ".til" file.
class FletchTileHeader
{
public:
    uint8_t    minVer;    // Version number
    uint8_t    majVer;    // Version number
    uint16_t   headSize;  // Size (in bytes) of header.  Also offset of data
    uint32_t   xSize;     // Horizontal pixel count
    uint32_t   ySize;     // Vertical pixel count
    uint32_t   frameCount; // Number of frames
    uint8_t    avgR;      // Average color in 24-bit format
    uint8_t    avgG;      // .
    uint8_t    avgB;      // .
    uint8_t    avgIndex;  // Closest palette entry to the average color
                                                                                
    FletchTileHeader(ReadFile* file);
};

FletchTileHeader::FletchTileHeader(ReadFile* file)
{
    minVer = file->read8();
    majVer = file->read8();
    headSize = file->readULE16();
    xSize = file->readULE32();
    ySize = file->readULE32();
    frameCount = file->readULE32();
    avgR = file->read8();
    avgG = file->read8();
    avgB = file->read8();
    avgIndex = file->read8();
}

Surface* TileLoader::load(const std::string& filename)
{
    std::auto_ptr<ReadFile> file(FileSystem::openRead(filename.c_str()));

    FletchTileHeader header(file.get());

    std::auto_ptr<Surface> surface
        (new Surface(iXY(header.xSize, header.ySize), header.xSize, 1));

    size_t size = surface->getPix().x * surface->getPix().y * sizeof(uint8_t);
    if(file->read(surface->getMem(), size, 1) != 1)
        throw Exception("Error reading file '%s': File too short.",
                        filename.c_str());

    return surface.release();
}

