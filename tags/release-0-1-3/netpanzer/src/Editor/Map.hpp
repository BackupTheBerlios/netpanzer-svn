#ifndef __MAP_HPP__
#define __MAP_HPP__

#include <stdlib.h>
#include <stdint.h>
#include <vector>
#include <string>

class Map
{
public:
    struct MapTile {
        MapTile()
            : tileset_id(0), tile_id(0)
        { }

        MapTile(uint16_t newtileset_id, uint16_t newtile_id)
            : tileset_id(newtileset_id), tile_id(newtile_id)
        { }
        
        uint16_t tileset_id;
        uint16_t tile_id;
    };                                                          

    /// creates a new empty map
    Map(size_t width, size_t height);
    /// loads a map from disk
    Map(const std::string& mapname);
    ~Map();

    void resize(size_t newwidth, size_t newheight);
    
    void setTile(size_t x, size_t y, MapTile tile);
    MapTile getTile(size_t x, size_t y) const;

    size_t getWidth() const
    { return width; }
    size_t getHeight() const
    { return height; }

    void setName(const std::string& newname)
    { name = newname; }
    const std::string& getName() const
    { return name; }

    size_t getTileSetCount() const
    { return tilesets.size(); }
    const std::string& getTileSet(size_t num) const
    { return tilesets.at(num); }

    void addTileSet(const std::string& tilesetname);
    
    /// saves the map to disk
    void save();

private:
	void load();	

    std::vector<std::string> tilesets;

    size_t width, height;
    MapTile* map;

    std::string name;
};

#endif
