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
    void load(const std::string& dir);
    /// save the tileset
    void save();

    /// returns the directory containing the tileset
    const std::string& getDirectory() const;

    void setDirectory(const std::string& newdirectory);

    size_t getTileCount() const;
    SDL_Surface* getTile(size_t num);
    size_t addTile(SDL_Surface* image, SDL_Rect* sourcerect = 0);

    size_t getTemplateCount() const;
    void addTemplate(TileTemplate* tiletemplate);
    TileTemplate* getTemplate(size_t num);

    iXY getTileSize() const;
        
private:
    void resizeBuffer(size_t newbuffersize);

    void readTemplates(const std::string& dir);
    
    TileSetHeader* header;
    size_t tilesize;
    char* tiledata;
    size_t tilebuffersize;

    std::string dir;
    std::vector<TileTemplate*> templates;
};

#endif

