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
#include <config.h>

#include "SDL.h"

#include "MouseInterface.hpp"
#include "Util/FileSystem.hpp"
#include "Util/Exception.hpp"
#include "Util/Log.hpp"
#include "cMouse.hpp"

unsigned char MouseInterface::cursor_x_size;
unsigned char MouseInterface::cursor_y_size;

iXY MouseInterface::mouse_pos;

float    MouseInterface::button_hold_threshold = (const float) .10;

bool   MouseInterface::left_button_down = false;
bool   MouseInterface::left_button_up = false;

bool   MouseInterface::right_button_down = false;
bool   MouseInterface::right_button_up = false;

bool   MouseInterface::middle_button_down = false;
bool   MouseInterface::middle_button_up = false;

unsigned char MouseInterface::button_mask;

MouseEventQueue MouseInterface::event_queue;
MouseInterface::cursors_t MouseInterface::cursors;

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
            surface->setOffsetCenter();
            surface->setColorKey(0);
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
    cursors_t::iterator i = cursors.begin();
    for( ; i != cursors.end(); i++) {
        delete i->second;
    }
}

void MouseInterface::setLeftButtonDown()
{
    MouseEvent event;

    //assert( left_button_down == false );
    left_button_down = true;
    left_button_up = false;
    button_mask = button_mask | 0x01;

    event.button = left_button;
    event.event = MouseEvent::EVENT_DOWN;
    event.pos = mouse_pos;
    event_queue.push_back(event);
}

void MouseInterface::setLeftButtonUp()
{
    MouseEvent event;

    left_button_down = false;
    left_button_up = true;
    button_mask = button_mask & (~0x01);

    event.event = MouseEvent::EVENT_UP;
    event.button = left_button;
    event.pos = mouse_pos;
    event_queue.push_back(event);
}

void MouseInterface::setRightButtonDown()
{
    MouseEvent event;

    right_button_down = true;
    right_button_up = false;
    button_mask = button_mask | 0x04;

    event.button = right_button;
    event.event = MouseEvent::EVENT_DOWN;
    event.pos = mouse_pos;
    event_queue.push_back( event );
}

void MouseInterface::setRightButtonUp( void )
{
    MouseEvent event;

    right_button_down = false;
    right_button_up = true;
    button_mask = button_mask & (~0x04);

    event.button = right_button;
    event.pos = mouse_pos;
    event.event = MouseEvent::EVENT_UP;
    event_queue.push_back( event );
}

void MouseInterface::setMiddleButtonDown()
{
    MouseEvent event;

    middle_button_down = true;
    middle_button_up = false;
    button_mask = button_mask | 0x02;

    event.button = middle_button;
    event.event = MouseEvent::EVENT_DOWN;
    event.pos = mouse_pos;
    event_queue.push_back( event );
}

void MouseInterface::setMiddleButtonUp()
{
    MouseEvent event;

    middle_button_down = false;
    middle_button_up = true;
    button_mask = button_mask & (~0x02);

    event.button = middle_button;
    event.pos = mouse_pos;
    event.event = MouseEvent::EVENT_UP;
    event_queue.push_back( event );
}

void MouseInterface::setCursor(const char* cursorname)
{
    cursors_t::iterator i = cursors.find(cursorname);
    if(i == cursors.end())
        throw Exception("mouse cursor '%s' not found.", cursorname);

    mouse.setPointer(i->second);
}

void MouseInterface::updateCursor()
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    mouse_pos.x = x;
    mouse_pos.y = y;
}

