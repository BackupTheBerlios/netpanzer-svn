#ifndef __TILESET_HPP__
#define __TILESET_HPP__

#include <SDL/SDL_video.h>

#include <vector>
#include <string>

#include "Types/iXY.hpp"

class TileSetHeader;
class Image;
class TileTemplate;

class TileSet
{
public:
    TileSet();
    ~TileSet();

    /// loads a tileset from disk
    void load(const std::string& name);
    /// save the tileset
    void save();

    /// returns the directory containing the tileset
    std::string getDirectory();
    void setName(const std::string& newname);

    size_t getTileWidth() const;
    size_t getTileHeight() const;

    size_t getTileCount() const;
    SDL_Surface* getTile(size_t num);
    size_t addTile(SDL_Surface* image, SDL_Rect* sourcerect = 0);

    size_t getTemplateCount() const;
    void addTemplate(TileTemplate* tiletemplate);
    TileTemplate* getTemplate(size_t num);

    iXY getTileSize() const;
        
private:
    void resizeBuffer(size_t newbuffersize);

    void readTemplates();
    
    TileSetHeader* header;
    size_t tilesize;
    char* tiledata;
    size_t tilebuffersize;

    std::string name;
    std::vector<TileTemplate*> templates;
};

#endif

