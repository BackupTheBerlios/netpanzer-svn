#ifndef __TILETMEPLATE_HPP__
#define __TILETMEPLATE_HPP__

#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <string>

#include "Util/NoCopy.hpp"
#include "Types/iXY.hpp"

class ReadFile;
class TileSet;

class TileTemplate : public NoCopy
{
public:
    enum {
        NOTILE = 0xffff
    };
    
    TileTemplate(TileSet* tileset, const iXY size);
    TileTemplate(TileSet* tileset, const std::string& name);
    ~TileTemplate();

    void save();

    void setName(const std::string& newname)
    { name = newname; }
    const std::string& getName() const
    { return name; }

    void setTile(const iXY pos, size_t tilenum)
    { 
        assert(pos.x < size.x && pos.y < size.y);
        tiles[pos.y * size.x + pos.x] = tilenum;
    }
    size_t getTile(const iXY pos) const
    { 
        assert(pos.x < size.x && pos.y < size.y);
        return tiles[pos.y * size.x + pos.x];
    }

    const iXY& getSize() const
    {
        return size;
    }

private:
    TileSet* tileset;
    iXY size;
    uint16_t* tiles;

    std::string name;
};

#endif

