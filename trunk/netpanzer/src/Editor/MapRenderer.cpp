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
    // TODO: take area into account...
    
    size_t tilewidth = tilesets[0]->getTileWidth();
    size_t tileheight = tilesets[0]->getTileHeight();

    size_t mapxstart = mapxpos / tilewidth;
    size_t mapystart = mapypos / tileheight;
    size_t xpos = 0;
    if(mapxpos % tilewidth != 0)
        xpos = -tilewidth + mapxpos % tilewidth;
    size_t ypos = 0;
    if(mapypos % tileheight != 0)
        ypos = -tileheight + mapypos % tileheight;

    SDL_Rect from_rect;
    SDL_Rect to_rect;
    
    size_t currenty = ypos; // the position at the destination surface
                            // where we draw now (in pixel)
    size_t currentx;
    for(size_t mapy = mapystart; mapy < map->getHeight(); mapy++) {
        currentx = xpos;
        for(size_t mapx = mapxstart; mapx < map->getWidth(); mapx++) {

            // Calculate from/to rectangle
            if(currentx < 0) {
                from_rect.x = tilewidth + currentx;
                from_rect.w = -currentx;
                to_rect.x = 0;
            } else {
                from_rect.x = 0;
                from_rect.w = std::min(tilewidth, surface->w - currentx);
                to_rect.x = currentx;
            }
            
            if(currenty < 0) {
                from_rect.y = tileheight + currenty;
                from_rect.h = -currenty;
                to_rect.y = 0;
            } else {
                from_rect.y = 0;
                from_rect.h = std::min(tileheight, surface->h - currenty);
                to_rect.y = currenty;
            }

            Map::MapTile tile = map->getTile(mapx, mapy);
            TileSet* tileset = tilesets[tile.tileset_id];
            SDL_Surface* tilesurface = tileset->getTile(tile.tile_id);
            SDL_BlitSurface(tilesurface, &from_rect, surface, &to_rect);
                    
            currentx += tilewidth;
            if(currentx >= (size_t) surface->w)
                break;
        }
        currenty += tileheight;
        if(currenty >= (size_t) surface->h)
            break;
    }
}

size_t MapRenderer::getMapWidth()
{
    return map->getWidth() * tilesets[0]->getTileWidth();
}

size_t MapRenderer::getMapHeight()
{
    return map->getHeight() * tilesets[0]->getTileHeight();
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

