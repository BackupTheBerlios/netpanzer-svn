#include <config.h>

#include <memory>
#include <string.h>

#include "Util/FileSystem.hpp"
#include "Util/Exception.hpp"
#include "TileSet.hpp"
#include "TileTemplate.hpp"

TileTemplate::TileTemplate(TileSet* newtileset, size_t newsizex, size_t newsizey)
    : tileset(newtileset), sizex(newsizex), sizey(newsizey)
{
    tiles = new uint16_t[sizex * sizey];
    for(size_t y=0; y<sizey; y++)
        for(size_t x=0; x<sizex; x++)
            tiles[y*sizex + x] = NOTILE;
}

TileTemplate::TileTemplate(TileSet* newtileset, const std::string& newname)
{
    tileset = newtileset;
    name = newname;
    
    std::string filename = tileset->getDirectory();
    filename += "/";
    filename += name;
    
    std::auto_ptr<ReadFile> file (FileSystem::openRead(filename));

    try {
        sizex = file->readULE32();
        sizey = file->readULE32();
    
        tiles = new uint16_t[sizex * sizey];
        for(size_t y=0; y<sizey; y++)
            for(size_t x=0; x<sizex; x++)
                tiles[y*sizex + x] = file->readULE16();
    } catch(std::exception& e) {
        throw Exception(
                "Error while reading template file '%s': file too short",
                filename.c_str());
    }
}

void TileTemplate::save()
{
    std::string filename = tileset->getDirectory();
    filename += "/";
    filename += name;
    
    std::auto_ptr<WriteFile> file(FileSystem::openWrite(filename));

    try {
        file->writeULE32(sizex);
        file->writeULE32(sizey);

        for(size_t y=0; y<sizey; y++)
            for(size_t x=0; x<sizex; x++)
                file->writeULE16(tiles[y*sizex + x]);
    } catch(std::exception& e) {
        throw Exception("Error while saving '%s': %s",
                filename.c_str(), e.what());
    }           
}

TileTemplate::~TileTemplate()
{
    delete[] tiles;
}


