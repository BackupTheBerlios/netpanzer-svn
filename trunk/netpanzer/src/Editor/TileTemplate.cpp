#include <config.h>

#include <memory>
#include <string.h>

#include "Util/FileSystem.hpp"
#include "Util/Exception.hpp"
#include "TileSet.hpp"
#include "TileTemplate.hpp"

TileTemplate::TileTemplate(TileSet* newtileset, const iXY newsize)
    : tileset(newtileset), size(newsize)
{
    tiles = new uint16_t[size.x * size.y];
    for(int y=0; y<size.y; y++)
        for(int x=0; x<size.x; x++)
            tiles[y*size.x + x] = NOTILE;
}

TileTemplate::TileTemplate(TileSet* newtileset, const std::string& newname)
{
    tileset = newtileset;
    name = newname;
    
    std::string filename = tileset->getDirectory();
    filename += "/templates/";
    filename += name;
    
    std::auto_ptr<ReadFile> file (FileSystem::openRead(filename));

    try {
        size.x = file->readULE32();
        size.y = file->readULE32();
    
        tiles = new uint16_t[size.x * size.y];
        for(int y=0; y<size.y; y++)
            for(int x=0; x<size.x; x++)
                tiles[y*size.x + x] = file->readULE16();
    } catch(std::exception& e) {
        throw Exception(
                "Error while reading template file '%s': file too short",
                filename.c_str());
    }
}

void TileTemplate::save()
{
    std::string templatedir = tileset->getDirectory();
    templatedir += "/templates";
    if(!FileSystem::isDirectory(templatedir))
        FileSystem::mkdir(templatedir);
    
    std::string filename = templatedir;
    filename += "/";
    filename += name;
    
    std::auto_ptr<WriteFile> file(FileSystem::openWrite(filename));

    try {
        file->writeULE32(size.x);
        file->writeULE32(size.y);

        for(int y=0; y<size.y; y++)
            for(int x=0; x<size.x; x++)
                file->writeULE16(tiles[y*size.x + x]);
    } catch(std::exception& e) {
        throw Exception("Error while saving '%s': %s",
                filename.c_str(), e.what());
    }           
}

TileTemplate::~TileTemplate()
{
    delete[] tiles;
}

