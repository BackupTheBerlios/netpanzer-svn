#include <config.h>

#include <iostream>

#include "Map.hpp"
#include "MapRenderer.hpp"

MapRenderer::MapRenderer(const Map* newmap)
    : map(newmap)
{
    loadTiles();
}

MapRenderer::~MapRenderer()
{
    std::vector<TileSet*>::iterator i;
    for(i=tilesets.begin(); i!=tilesets.end(); i++)
        delete *i;
}

void MapRenderer::render(SDL_Surface* surface, SDL_Rect* area,
        size_t mapxpos, size_t mapypos)
{
}

void MapRenderer::loadTiles()
{
    for(size_t i = 0; i < map->getTileSetCount(); i++) {
        std::auto_ptr<TileSet> tileset (new TileSet());
        std::cout << "Loading Tileset: " << map->getTileSet(i) << "\n";
        tileset->load(map->getTileSet(i));
        tilesets.push_back(tileset.release());
    }
}

