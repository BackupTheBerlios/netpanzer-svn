#ifndef __TILESET_HPP__
#define __TILESET_HPP__

#include <SDL/SDL_video.h>

#include <vector>
#include <string>

#include "iXY.hpp"

class TileSetHeader;
class ReadFile;
class WriteFile;
class Image;
class TileTemplate;

class TileSet
{
public:
    TileSet();
    ~TileSet();

    void load(ReadFile& filename);
    void save(WriteFile& filename);

    size_t getTileCount() const;
    SDL_Surface* getTile(size_t num);
    void addTile(SDL_Surface* image, SDL_Rect* sourcerect = 0);

    iXY getTileSize() const;
        
private:
    void resizeBuffer(size_t newbuffersize);
    
    TileSetHeader* header;
    size_t tilesize;
    char* tiledata;
    size_t tilebuffersize;

    std::vector<TileTemplate*> templates;
};

#endif

