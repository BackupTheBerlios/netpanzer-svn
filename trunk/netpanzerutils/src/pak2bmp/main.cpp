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
#include <sstream>
#include <SDL.h>
#include "optionmm/option.hpp"
#include "2D/Surface.hpp"
#include "2D/PackedSurface.hpp"
#include "BMP.hpp"
#include "optionmm/command_line.hpp"
#include "Util/FileSystem.hpp"

std::string extractName(const std::string& imagefile)
{
    size_t lastslash = 0;
    size_t firstdot = imagefile.size();
    for(size_t pos = 0; pos < imagefile.size()-1; ++pos) {
        if(imagefile[pos] == '/') {
            lastslash = pos+1;
            firstdot = imagefile.size();
        }
        if(imagefile[pos] == '.')
            firstdot = pos;
    }
                                                                                
    return imagefile.substr(lastslash, firstdot - lastslash);
}

int realmain(int argc, char** argv)
{
    using namespace optionmm;

    try {
        SDL_Init(0);
        FileSystem::initialize(argv[0], "pak2bmp", "pak2bmp");

        command_line commandline(PACKAGE_NAME, PACKAGE_VERSION,
                "Copyright(c) 2003 Matthias Braun", "", argc, argv);

        option<std::string, true, false> inputfile_option('i', "input",
                "input file", "");
        commandline.add(&inputfile_option);
        option<std::string, true, false> pal_option('p', "palette",
                "palette file", "");
        commandline.add(&pal_option);
        
        if(!commandline.process() || commandline.help() ||
                commandline.version())
            exit(0);

        if(inputfile_option.value() == "")
            exit(0);

        PackedSurface* paksurface = new PackedSurface();
        paksurface->load(inputfile_option.value().c_str());
        Palette::init(pal_option.value().c_str());
        Surface* surface = new Surface(paksurface->getPix(),
                paksurface->getPixX(), 1);

        Palette pal;
      
        for(int i=0; i<paksurface->getFrameCount(); ++i) {
            paksurface->setFrame(i);
            surface->fill(0);
            paksurface->blt(*surface, 0, 0);
            std::stringstream filenamestr;
            filenamestr << extractName(inputfile_option.value())
                << "-" << i << ".bmp";
            std::cout << "Writing: " << filenamestr.str() << std::endl;
            BMPSaver::save(filenamestr.str(), surface, pal);
        }

        delete paksurface;
        delete surface;
        
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

