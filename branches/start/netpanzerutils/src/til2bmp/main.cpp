/*
Copyright (C) 2003 Matthias Braun <matze@braunis.de>
                                                                                
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
#include <config.h>

#include <exception>
#include <SDL.h>
#include "option.hpp"
#include "Surface.hpp"
#include "Tile.hpp"
#include "BMP.hpp"
#include "command_line.hpp"
#include "FileSystem.hpp"

int realmain(int argc, char** argv)
{
    using namespace optionmm;

    try {
        SDL_Init(0);
        FileSystem::initialize(argv[0], "til2bmp", "til2bmp");

        command_line commandline(PACKAGE_NAME, PACKAGE_VERSION,
                "Copyright(c) 2003 Matthias Braun", "", argc, argv);

        option<std::string, true, false> inputfile_option('i', "input",
                "input file", "");
        commandline.add(&inputfile_option);
        option<std::string, true, false> output_option('o', "output",
                "output file", "");
        commandline.add(&output_option);
        option<std::string, true, false> pal_option('p', "palette",
                "palette file", "");
        commandline.add(&pal_option);
        
        if(!commandline.process() || commandline.help() ||
                commandline.version())
            exit(0);

        if(inputfile_option.value() == "" || output_option.value() == "")
            exit(0);

        Surface* surface = TileLoader::load(inputfile_option.value());
        Palette::init(pal_option.value().c_str());
        
        Palette pal;
        BMPSaver::save(output_option.value(), surface, pal); 
        
        SDL_Quit();
    } catch(std::exception& e) {
        fprintf(stderr, "Error: %s", e.what());
        return 1;
    }

    return 0;
}

// XXX these tweaks are need for SDL on win32
extern "C" {
    int main(int argc, char** argv)
    {
        return realmain(argc, argv);
    }
}

