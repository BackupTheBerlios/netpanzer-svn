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


#include "SDL.h"

#include "MouseInterface.hpp"
#include "Util/FileSystem.hpp"
#include "Util/Exception.hpp"
#include "Util/Log.hpp"
#include "Util/NTimer.hpp"

unsigned char MouseInterface::cursor_x_size;
unsigned char MouseInterface::cursor_y_size;

iXY MouseInterface::mouse_offset;

MouseInterface::cursors_t MouseInterface::cursors;
Surface * MouseInterface::cursor;

void MouseInterface::initialize()
{
    const char* cursorpath = "pics/cursors/";
    char** cursorfiles = filesystem::enumerateFiles(cursorpath);
    for(char** i = cursorfiles; *i != 0; i++) {
        Surface* surface = new Surface;
        try {
            std::string filename = cursorpath;
            filename += *i;
            if(filesystem::isDirectory(filename.c_str())) {
                continue;
            }
            surface->loadBMP(filename.c_str());
            cursors.insert(std::pair<std::string,Surface*> (*i, surface));
        } catch(std::exception& e) {
            LOG(("Couldn't load cursorfile '%s': %s", *i, e.what()));
        }
    }
    filesystem::freeList(cursorfiles);

    setCursor("default.bmp");
}

void MouseInterface::shutdown()
{
    cursor = 0;
    cursors_t::iterator i = cursors.begin();
    for( ; i != cursors.end(); i++) {
        delete i->second;
    }
}

void MouseInterface::setCursor(const char* cursorname)
{
    cursors_t::iterator i = cursors.find(cursorname);
    if(i == cursors.end()) {
        LOGGER.warning("WARNING: Mouse '%s' not found", cursorname);
        cursor = 0;
    } else {
        cursor = i->second;
        mouse_offset.x = cursor->getWidth() / 2;
        mouse_offset.y = cursor->getHeight() / 2;

    }
}
