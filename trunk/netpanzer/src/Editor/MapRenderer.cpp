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
    int startx = area ? area->x : 0;
    int starty = area ? area->y : 0;

    size_t tilewidth = tilesets[0]->getTileWidth();
    size_t tileheight = tilesets[0]->getTileHeight();

    size_t mapxstart = mapxpos / tilewidth;
    size_t mapystart = mapypos / tileheight;
    size_t xpos = -tilewidth + mapxpos % tilewidth;
    size_t ypos = -tileheight + mapypos % tileheight;
    
    for(size_t mapy = mapystart; mapy < map->getHeight(); mapy++) {
        for(size_t mapx = mapxstart; mapx < map->getWidth(); mapx++) {
            
        }
    }
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

