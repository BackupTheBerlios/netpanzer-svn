#ifndef __MAPRENDERER_HPP__
#define __MAPRENDERER_HPP__

#include <SDL/SDL.h>
#include <vector>

#include "TileSet.hpp"

class Map;

class MapRenderer
{
public:
    MapRenderer(const Map* map);
    ~MapRenderer();

    void render(SDL_Surface* surface, SDL_Rect* area,
            size_t mapxpos, size_t mapypos);

private:
    void loadTiles();
    
    const Map* map;
    std::vector<TileSet*> tilesets;
};

#endif

