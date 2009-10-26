/*
Copyright (C) 1998 Pyrosoft Inc. (www.pyrosoftgames.com), Matthew Bogue
 
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.
 
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
 
You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
#ifndef _TILESET_HPP
#define _TILESET_HPP

#include <vector>
#include "2D/Surface.hpp"
#include "SDL.h"
#include "Util/FileStream.hpp"

namespace filesystem { class ReadFile; }

typedef struct TileSetHeader_s
{
    unsigned char netp_id_header[64];
    int     version;
    int     x_pix;
    int     y_pix;
    int     tile_count;
    Uint8   palette[768];

    std::string image_file;
    std::string unused;

} TileSetHeader;

typedef struct TileAttributes_s
{
    Uint8 attrib;
    Uint8 move_value;
    SDL_Color avg_color;
} TileAttributes;

class TileSet
{
protected:
    bool tile_set_loaded;
    TileSetHeader header;
    std::vector<TileAttributes> tile_attributes;
    std::vector<Surface*> tiles;


    unsigned long tile_size;
    void computeTileConsts( void );

public:
    TileSet();
    ~TileSet();

    void readTileSetHeader(IFileStream& file);
    void loadTileSetInfo( const char *file_path );

    void loadTileSet( const char *file_path );

    inline Surface * getTile(unsigned long index) const
    {
        return tiles[index];
    }

    inline unsigned short getTileXsize() const
    {
        return ( header.x_pix );
    }

    inline unsigned short getTileYsize() const
    {
        return ( header.y_pix );
    }

    inline unsigned short getTileCount() const
    {
        return ( header.tile_count );
    }

    inline const SDL_Color* getAverageTileColor(unsigned long index) const
    {
        return( &(tile_attributes[ index ].avg_color) );
    }

    inline char getTileMovementValue( unsigned long index )
    {
        return( tile_attributes[ index ].move_value );
    }

    inline unsigned char getTilePixel( unsigned long index , unsigned int pixX,
                                       unsigned int pixY)
    {
        if( index < tiles.size() )
        {
            return getTile(index)->getPixel(pixX, pixY);
        }

        return( 0 );
    }
private:
    void loadTileAttributes(IFileStream& file);
    void freeTiles();
};

#endif // ** _TILESET_HPP
