#include <config.h>

#include <assert.h>
#include <memory>

#include "Util/FileSystem.hpp"
#include "Util/Exception.hpp"
#include "Map.hpp"

Map::Map(size_t newwidth, size_t newheight)
    : width(0), height(0), map(0)
{
    resize(newwidth, newheight);
    addTileSet("default");
}

Map::Map(const std::string& newname)
	: width(0), height(0), map(0)
{
    name = newname;
    load();
}

Map::~Map()
{
}

void Map::resize(size_t newwidth, size_t newheight)
{
    MapTile* newmap = new MapTile[newwidth * newheight];
    
    for(size_t y = 0; y<newheight; y++) {
        for(size_t x = 0; x<newwidth; x++) {
            if(y < height && x < width)
                newmap[y * newwidth + x] = map[y * width + x];
            else
                newmap[y * newwidth + x] = MapTile(0,0);
        }
    }
    
    delete[] map;
    map = newmap;
    width = newwidth;
    height = newheight;
}

void Map::setTile(size_t x, size_t y, MapTile tile)
{
    assert(x < width);
    assert(y < height);

    map[y * width + x] = tile;    
}

Map::MapTile Map::getTile(size_t x, size_t y) const
{
    assert(x < width);
    assert(y < height);

    return map[y * width + x];
}

void Map::addTileSet(const std::string& tilesetname)
{
    tilesets.push_back(tilesetname);
}

void Map::save()
{
    std::string mapdir = "/maps/";
    mapdir += name;
    
    if(!FileSystem::isDirectory(mapdir))
        FileSystem::mkdir(mapdir);
    
    // write list of tilesets
    std::string filename = mapdir;
    filename += "/tilesets";
    
    std::auto_ptr<WriteFile> tilesetfile (FileSystem::openWrite(filename));
    std::vector<std::string>::iterator i;
    for(i=tilesets.begin(); i != tilesets.end(); i++) {
        tilesetfile->writeLine(*i);
    }
    
    // write tiles
    filename = mapdir;
    filename += "/tiles";
    std::auto_ptr<WriteFile> tilefile (FileSystem::openWrite(filename));
    tilefile->writeULE32(width);
    tilefile->writeULE32(height);
    
    for(size_t y=0; y<height; y++) {
        for(size_t x=0; x<width; x++) {
            tilefile->writeULE16(map[y*width +x].tileset_id);
            tilefile->writeULE16(map[y*width +x].tile_id);
        }
    }
}

void Map::load()
{
    std::string mapdir = "/maps/";
    mapdir += name;
    
    if(!FileSystem::isDirectory(mapdir))
        throw Exception("Map with name '%s' does not exist.", name.c_str());
    
    // read tiles	
    std::string filename = mapdir;
    filename += "/tiles";
    std::auto_ptr<ReadFile> tilefile (FileSystem::openRead(filename));
    
    size_t newwidth = tilefile->readULE32();
    size_t newheight = tilefile->readULE32();
    resize(newwidth, newheight);
    for(size_t y=0; y<height; y++) {
        for(size_t x=0; x<width; x++) {
            uint16_t tileset_id = tilefile->readULE16();
            uint16_t tile_id = tilefile->readULE16();
            map[y*width+x] = MapTile(tileset_id, tile_id);
        }
    }
    
    filename = mapdir;
    filename += "/tilesets";
    
    // read list of tilesets
    std::auto_ptr<ReadFile> tilesetfile (FileSystem::openRead(filename));
    tilesets.clear();
    while(!tilesetfile->isEOF()) {
        std::string tileset;
        tilesetfile->readLine(tileset);
	    
        tilesets.push_back(tileset);
    }
}
