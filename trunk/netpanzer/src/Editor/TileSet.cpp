#include <config.h>

#include <iostream>
#include <stdint.h>
#include <memory>

#include "NoCopy.hpp"
#include "TileSet.hpp"
#include "FileSystem.hpp"
#include "Exception.hpp"
#include "Log.hpp"

static const char* MAGICSTRING="TLST";
static const uint32_t FILEFORMATVERSION=0001;

class TileSetHeader : public NoCopy
{
public:
    char        magic[4];
    uint32_t    fileformatversion;
    uint32_t    tilewidth;
    uint32_t    tileheight;
    uint32_t    tilebitsperpixel;
    uint32_t    tilecount;

    uint32_t    reserved[10];

    TileSetHeader();
    TileSetHeader(ReadFile& file);
    
    void write(WriteFile& file);
} __attribute__((packed));

TileSetHeader::TileSetHeader()
    : fileformatversion(FILEFORMATVERSION), tilewidth(32), tileheight(32),
      tilebitsperpixel(24), tilecount(0)
{
    strncpy(magic, MAGICSTRING, 4);
    for(size_t i=0;i<sizeof(reserved)/sizeof(uint32_t); i++)
        reserved[i]=0;
}

TileSetHeader::TileSetHeader(ReadFile& file)
{
    try {
        file.read(magic, 4, 1);
        fileformatversion = file.readULE32();
        tilewidth = file.readULE32();
        tileheight = file.readULE32();
        tilebitsperpixel = file.readULE32();
        tilecount = file.readULE32();
    
        for(size_t i=0; i<sizeof(reserved)/sizeof(uint32_t); i++) {
            reserved[i] = file.readULE32();
        }
    } catch(Exception& e) {
        throw Exception("Header too short (%s)", e.what());
    }
}

void TileSetHeader::write(WriteFile& file)
{
    try {
        file.write(magic, 4, 1);
        file.writeULE32(fileformatversion);
        file.writeULE32(tilewidth);
        file.writeULE32(tileheight);
        file.writeULE32(tilebitsperpixel);
        file.writeULE32(tilecount);
        
        for(size_t i=0; i<sizeof(reserved)/sizeof(uint32_t); i++) {
            file.writeULE32(reserved[i]);
        }
    } catch(Exception& e) {
        throw Exception("Couldn't write Tileset Header: %s", e.what());
    }
}

//---------------------------------------------------------------------------

TileSet::TileSet()
    : header(0), tilesize(0), tiledata(0), tilebuffersize(0)
{
    header = new TileSetHeader();
}

TileSet::~TileSet()
{
    delete header;
    delete[] tiledata;
}

void TileSet::load(ReadFile& file)
{
    std::auto_ptr<TileSetHeader> fileheader (new TileSetHeader(file));

    if(memcmp(fileheader->magic, MAGICSTRING, 4) != 0)
        throw Exception("File is not a tileset");

    if(fileheader->fileformatversion < FILEFORMATVERSION)
        throw Exception("Tileset has incompatible file format version.");

    if(fileheader->fileformatversion > FILEFORMATVERSION)
        LOG(("Warning: TileSet header is newer than program version."));
    
    // Clear old data
    delete header;
    header = fileheader.release();
    
    delete[] tiledata;
    tilebuffersize = 0;
    tiledata = 0;
    
    tilesize = header->tilewidth * header->tileheight * (header->tilebitsperpixel / 8);
    if(header->tilecount > 0) {
        resizeBuffer(tilesize * header->tilecount);
        if(file.read(tiledata, tilesize, header->tilecount) != header->tilecount)
            throw Exception("Tileset file is too short.");
    }
}

void TileSet::save(WriteFile& file)
{
    header->write(file);
    if(tilesize * header->tilecount != 0)
        file.write(tiledata, tilesize, header->tilecount);
}

size_t TileSet::getTileCount() const
{
    return header ? header->tilecount : 0;
}

SDL_Surface* TileSet::getTile(size_t num)
{
    SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(
        tiledata + (tilesize*num), header->tilewidth, header->tileheight,
        header->tilebitsperpixel, header->tilewidth, 
        0x000000ff, 0x0000ff00, 0x00ff0000, 0);
    if(!surface)
        throw Exception(
                "Couldn't create a new SDL Surface for tile number '%d'", num);
    
    return surface;
}

void TileSet::addTile(SDL_Surface* surface, SDL_Rect* srcrect)
{
    if(surface->format->BitsPerPixel != header->tilebitsperpixel)
        throw Exception("Surface has incompatible format to be added as a tile"
                "(conversion not implemented yet). Surface: %d, shoudl be %d",
                surface->format->BitsPerPixel, header->tilebitsperpixel);
    
    SDL_Rect* rect = srcrect;
    if(rect == 0) {
        rect = new SDL_Rect;
        rect->x = rect->y = 0;
        rect->w = header->tilewidth;
        rect->h = header->tileheight;            
    } else {
        if(rect->w != header->tilewidth || rect->h != header->tileheight) {
            throw Exception(
                    "Can't add tile with wrong size %dx%d, should be %dx%d",
                    rect->w, rect->h, header->tilewidth, header->tilewidth);
        }
    }

    if(rect->x + rect->w > surface->w ||
       rect->y + rect->h > surface->h)
        throw Exception("Invalid source rectangle, can't add tile");

    if(tilesize * (header->tilecount+1) > tilebuffersize)
        resizeBuffer(tilebuffersize * 2 + tilesize);

    int bpp = surface->format->BytesPerPixel;
    char* sptr = ((char*) surface->pixels) +
        rect->y * surface->pitch + rect->x * bpp;
    char* dptr = tiledata + (tilesize * header->tilecount);

    for(int lines=rect->h-1; lines>=0; lines--) {
        for(int x=0;x<rect->w; x++) {
            // XXX slow... this switch should be put outside the loop...
            switch(bpp) {
                case 3:
                    memcpy(dptr, sptr, 3);
                    break;
                case 4:
                    *((uint32_t*) dptr) = *((uint32_t*) sptr);
                    break;
                default:
                    throw Exception("unsupported bpp");
            }
            sptr += bpp;
        }
        sptr += surface->pitch;
    }

    header->tilecount++;
}

iXY TileSet::getTileSize() const
{
    return iXY(header->tilewidth, header->tileheight);
}

void TileSet::resizeBuffer(size_t newbuffersize)
{
    std::auto_ptr<char> newbuffer (new char[newbuffersize]);

    if(tiledata)
        memcpy(newbuffer.get(),
                tiledata, std::min(tilebuffersize, newbuffersize));

    delete[] tiledata;
    tiledata = newbuffer.release();
    tilebuffersize = newbuffersize;
}

