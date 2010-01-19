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


#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <memory>
#include <cassert>

#include "SDL_endian.h"

#include "Classes/WorldMap.hpp"
#include "Port/MapData.hpp"
#include "Util/FileSystem.hpp"
#include "Util/Exception.hpp"
#include "Util/Log.hpp"


WorldMap::WorldMap()
    : loaded(false)
{
}

WorldMap::~WorldMap()
{
}

void
WorldMap::loadHeader(filesystem::ReadFile& file)
{
    file.read(&header.netp_id_header, sizeof(header.netp_id_header), 1);
    header.id = file.readULE16();
    file.read(&header.name, sizeof(header.name), 1);
    file.read(&header.description, sizeof(header.description), 1);
    header.width = file.readULE16();
    header.height = file.readULE16();
    file.read(&header.tile_set, sizeof(header.tile_set), 1);
    header.thumbnail_width = file.readULE16();
    header.thumbnail_height = file.readULE16();
}

void
WorldMap::loadTiles(filesystem::ReadFile &file)
{
    data.resize(getSize());

    for( MapData::iterator i = data.begin(); i != data.end(); ++i)
    {
        *i = file.readULE16();
    }
}

void WorldMap::loadMapFile(const std::string& filename)
{
    try {
	std::auto_ptr<filesystem::ReadFile> file(
                filesystem::openRead(filename));

        loaded = false;

        loadHeader(*file);
        loadTiles(*file);

        loaded = true;

    } catch(std::exception& e) {
	throw Exception("Error while reading mapfile '%s': %s",
		filename.c_str(), e.what());
    }
    dumpInfo();
}

void
WorldMap::dumpInfo()
{
    LOGGER.info("WorldMap info:");
    LOGGER.info("\tName: %s", header.name);
    LOGGER.info("\tDescription: %s", header.description);
    LOGGER.info("\tSize: %dx%d", header.width, header.height);
    LOGGER.info("\tTile Set: %s", header.tile_set);
    LOGGER.info("\tThumbnail Size: %dx%d", header.thumbnail_width, header.thumbnail_height);
}
