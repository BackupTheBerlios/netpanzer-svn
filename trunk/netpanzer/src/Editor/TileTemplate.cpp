#include <config.h>

#include "TileTemplate.hpp"

TileTemplate::TileTemplate(TileSet* newtileset, size_t newsizex, size_t newsizey)
    : tileset(newtileset), sizex(newsizex), sizey(newsizey)
{
    tiles = new size_t[sizex * sizey];
}

TileTemplate::TileTemplate(ReadFile* file)
{
}

TileTemplate::~TileTemplate()
{
    delete[] tiles;
}


