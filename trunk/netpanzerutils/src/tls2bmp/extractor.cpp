#include <iostream>
#include <fstream>
#include <string>

#include "TileSetStruct.hpp"
#include "2D/Surface.hpp"
#include "2D/Palette.hpp"
#include "BMP.hpp"
#include "Util/FileSystem.hpp"

int main(int argc, char** argv)
{
    std::ifstream in("wads/summer12mb.tls");

    FileSystem::initialize(argv[0], "tls2bmp", "tls2bmp");

    TILE_DBASE_HEADER header;
    in.read((char*) &header, sizeof(TILE_DBASE_HEADER));
    
    std::cout << "Count: " << header.tile_count << "\n";
    std::cout << "Size: " << header.x_pix << "," << header.y_pix << "\n";

    Palette::init("wads/netp.act");
    Palette pal;

    TILE_HEADER tileheader;
    for(size_t i = 0; i<header.tile_count; ++i) {
        in.read((char*) &tileheader, sizeof(TILE_HEADER));    
        std::cout << i << " " << "attribute " << (int) tileheader.attrib 
            << " movevalue " << (int) tileheader.move_value << "\n";
    }

#if 0
    for(size_t i = 0; i<header.tile_count; ++i) {
        char filename[128];
        snprintf(filename, 128, "tile%04d.bmp", i);
    
        // read the tile
        char* pixeldata = new char[header.x_pix * header.y_pix];
        in.read((char*) pixeldata, header.x_pix * header.y_pix);
        
        Surface* surface = new Surface(pixeldata, iXY(header.x_pix,
                    header.y_pix), header.x_pix, 1);
        BMPSaver::save(filename, surface, pal);
    }
#endif
}
