#ifndef __TILETMEPLATE_HPP__
#define __TILETMEPLATE_HPP__

#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <string>

#include "Util/NoCopy.hpp"

class ReadFile;
class TileSet;

class TileTemplate : public NoCopy
{
public:
    enum {
        NOTILE = 0xffff
    };
    
    TileTemplate(TileSet* tileset, size_t sizex, size_t sizey);
    TileTemplate(TileSet* tileset, const std::string& name);
    ~TileTemplate();

    void save();

    void setName(const std::string& newname)
    { name = newname; }
    const std::string& getName() const
    { return name; }

    void setTile(size_t x, size_t y, size_t tilenum)
    { 
        assert(x < sizex);
        assert(y < sizey);
        tiles[y*sizex + x] = tilenum;
    }
    size_t getTile(size_t x, size_t y) const
    { 
        assert(x < sizex);
        assert(y < sizey);
        return tiles[y*sizex + x];
    }

private:
    TileSet* tileset;
    size_t sizex, sizey;
    uint16_t* tiles;

    std::string name;
};

#endif

