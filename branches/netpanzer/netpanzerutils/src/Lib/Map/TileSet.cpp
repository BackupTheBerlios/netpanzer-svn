#include <config.h>

#define TILESETFORMAT       (0001)

struct TileSetHeader
{
    uint32_t formatversion;
    uint32_t tilewidth;
    uint32_t tileheight;
    uint32_t tilebpp;
    uint32_t tilecount;
};

struct TileHeader
{
    char tilename[32];
    uint32_t flags;
    uint8_t movevalue;
};

