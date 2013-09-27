/*
Copyright (C) 2012 by Aaron Perez <aaronps@gmail.com>

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

#include <cstdlib>

#include "SDL.h"

#include "Util/FileSystem.hpp"
#include "Types/iXY.hpp"
#include "2D/PackedSurface.hpp"
#include "2D/Surface.hpp"
#include "2D/Palette.hpp"

#include <string>
#include <sstream>
#include <iomanip>
#include <cstring>

#include "libpng-1.6.5/png.h"

using namespace std;

int main ( int argc, char** argv )
{
    printf("pak2png for NetPanzer V 1.1\n");
    if (argc < 4 )
    {
        printf("use: %s <filename> <output_name_no_ext> <images_per_row>\n", argv[0]);
        printf("Will create output_name_no_ext.png and .json\n");
        printf("note: even on windows the path must be separated by '/'\n");
        printf("example:\n");
        printf("%s units/pics/pak/TitaHNSD.pak titan 9\n", argv[0]);
        return 1;
    }

    string palettefile = "netp";
    
    string pakFilename = argv[1];
    string filename_noext = argv[2];
    int picsPerRow = atoi(argv[3]);
    
    printf("base is '%s'\n", filename_noext.c_str());

    if ( ! PHYSFS_init(argv[0]) )
    {
        printf("Error initializing filesystem: %s", PHYSFS_getLastError());
        return 1;
    }

    PHYSFS_addToSearchPath(PHYSFS_getBaseDir(), 1);
    PHYSFS_setWriteDir(PHYSFS_getBaseDir());

    Palette::init(palettefile);
    
    PackedSurface pak;
    pak.load(pakFilename);
    
    Surface unpacked(pak.getWidth(), pak.getHeight());

    printf("There are %d frames\n", pak.getFrameCount());
    
    int x = 0, y = 0;
    
    const int pak_offset_x = pak.getOffsetX();
    const int pak_offset_y = pak.getOffsetY();
    
    pak.setOffset(iXY(0,0));
    
    int dest_width = pak.getWidth() * pak.getFrameCount();
    int dest_height = pak.getHeight();
    
    if ( picsPerRow && (pak.getFrameCount() > picsPerRow) )
    {
        dest_width = picsPerRow * pak.getWidth();
        dest_height = pak.getHeight() * (pak.getFrameCount() / picsPerRow);
        if ( pak.getFrameCount() % picsPerRow )
        {
            dest_height += pak.getHeight();
        }
    }

    Surface all(dest_width, dest_height);
    
    all.fill(0);
    
    for ( int n = 0; n < pak.getFrameCount(); n++ )
    {
        pak.setFrame(n);
        
        pak.blt(all, x, y);
        
        x += pak.getWidth();
        if ( x >= all.getWidth() )
        {
            x = 0;
            y += pak.getHeight();
        }
    }
    
    std::stringstream pngfile;
    pngfile << filename_noext << ".png";
    
    all.savePNG(pngfile.str().c_str());
    
    std::stringstream deffile;
    deffile << "/" << filename_noext << ".json";

    FILE *f = fopen(filesystem::getRealWriteName(deffile.str().c_str()).c_str(), "w");
    if ( !f )
    {
        printf("Error: cannot write file %s\n", filesystem::getRealWriteName(deffile.str().c_str()).c_str());
        PHYSFS_deinit();
        return 1;
    }

    fprintf(f,"{\n\t\"file\": \"%s\"", pngfile.str().c_str());
    fprintf(f,",\n\t\"size\": [ %d, %d ]", pak.getWidth(), pak.getHeight());
    fprintf(f,",\n\t\"offset\": [ %d, %d ]", pak_offset_x, pak_offset_y);
    fprintf(f,",\n\t\"frames\": %d", pak.getFrameCount());
    fprintf(f,",\n\t\"fps\": %f", pak.getFPS());
    fprintf(f,"\n}");

    fclose(f);
    
    PHYSFS_deinit();
    printf("Exited cleanly\n");
    return 0;
}
