#ifndef __TILETMEPLATE_HPP__
#define __TILETMEPLATE_HPP__

#include <stdio.h>
#include <assert.h>

#include "Util/NoCopy.hpp"

class ReadFile;
class TileSet;

class TileTemplate : public NoCopy
{
public:
    TileTemplate(TileSet* tileset, size_t sizex, size_t sizey);
    TileTemplate(ReadFile* file);
    ~TileTemplate();

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
    size_t* tiles;
};

#endif

