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

#include <SDL.h>

#include "MouseInterface.hpp"
#include "FileSystem.hpp"
#include "Exception.hpp"
#include "Log.hpp"
#include "cMouse.hpp"

unsigned char MouseInterface::cursor_x_size;
unsigned char MouseInterface::cursor_y_size;

iXY MouseInterface::mouse_pos;

float    MouseInterface::button_hold_threshold = (const float) .10;

bool   MouseInterface::left_button_down = false;
bool   MouseInterface::left_button_up = false;
bool   MouseInterface::left_button_dbclick_active = false;
iXY  MouseInterface::left_button_down_pos;
iXY  MouseInterface::left_button_up_pos;
TIMESTAMP MouseInterface::left_button_hold_time;

bool   MouseInterface::right_button_down = false;
bool   MouseInterface::right_button_up = false;
bool   MouseInterface::right_button_dbclick_active = false;
iXY  MouseInterface::right_button_down_pos;
iXY  MouseInterface::right_button_up_pos;
TIMESTAMP MouseInterface::right_button_hold_time;

bool   MouseInterface::middle_button_down = false;
bool   MouseInterface::middle_button_up = false;
bool   MouseInterface::middle_button_dbclick_active = false;
iXY  MouseInterface::middle_button_down_pos;
iXY  MouseInterface::middle_button_up_pos;
TIMESTAMP MouseInterface::middle_button_hold_time;

unsigned char MouseInterface::button_mask;

MouseEventQueue MouseInterface::event_queue;
MouseInterface::cursors_t MouseInterface::cursors;

void MouseInterface::initialize()
{
    const char* cursorpath = "pics/cursors/";
    char** cursorfiles = FileSystem::enumerateFiles(cursorpath);
    for(char** i = cursorfiles; *i != 0; i++) {
        Surface* surface = new Surface;
        try {
            std::string filename = cursorpath;
            filename += *i;
            if(FileSystem::isDirectory(filename.c_str())) {
                continue;
            }
            surface->loadBMP(filename.c_str());
            surface->setOffsetCenter();
            cursors.insert(std::pair<std::string,Surface*> (*i, surface));
        } catch(Exception& e) {
            LOG(("Couldn't load cursorfile '%s': %s", *i, e.what()));
        }
    }
    FileSystem::freeList(cursorfiles);

    setCursor("default.bmp");
}

void MouseInterface::shutdown()
{
    cursors_t::iterator i = cursors.begin();
    for( ; i != cursors.end(); i++) {
        delete i->second;
    }
}

bool MouseInterface::buttonHeld(unsigned char mask)
{
    if ( mask & button_mask & left_button ) {
        if( (now() - left_button_hold_time) > button_hold_threshold )
            return true;
    } else if ( mask & button_mask & right_button ) {
        if( (now() - right_button_hold_time) > button_hold_threshold )
            return true;
    } else if ( mask & button_mask & middle_button ) {
        if( (now() - middle_button_hold_time) > button_hold_threshold )
            return true;
    }

    return false;
}

void MouseInterface::resetButtonHoldStatus( void )
{
    left_button_hold_time  = now();
    right_button_hold_time = now();
    middle_button_hold_time = now();
}

void MouseInterface::setLeftButtonDown()
{
    MouseEvent event;

    //assert( left_button_down == false );
    left_button_down = true;
    left_button_up = false;
    button_mask = button_mask | 0x01;
    left_button_down_pos = mouse_pos;
    left_button_hold_time = now();

    event.button = left_button;
    event.event = MouseEvent::EVENT_DOWN;
    event.down_pos = left_button_down_pos;
    event_queue.push_back(event);
}

void MouseInterface::setLeftButtonUp()
{
    MouseEvent event;

    //assert( left_button_up == false );
    left_button_down = false;
    left_button_up = true;
    button_mask = button_mask & (~0x01);
    left_button_up_pos = mouse_pos;

    event.button = left_button;
    event.up_pos = left_button_up_pos;

    if ( left_button_dbclick_active == true ) {
        event.event = MouseEvent::EVENT_DBCLICK;
        left_button_dbclick_active = false;
    } else
        event.event  = MouseEvent::EVENT_CLICK;

    event.down_pos = left_button_down_pos;
    event_queue.push_back(event);

    event.event = MouseEvent::EVENT_UP;
    event_queue.push_back(event);
}

void MouseInterface::setLeftButtonDoubleDown()
{
    left_button_down = true;
    left_button_dbclick_active = true;
    left_button_up = false;
    button_mask = button_mask | 0x01;
    left_button_down_pos = mouse_pos;
    left_button_hold_time = now();
}

void MouseInterface::setRightButtonDown()
{
    MouseEvent event;

    //assert( right_button_down == false );
    right_button_down = true;
    right_button_up = false;
    button_mask = button_mask | 0x04;
    right_button_down_pos = mouse_pos;
    right_button_hold_time = now();

    event.button = right_button;
    event.event = MouseEvent::EVENT_DOWN;
    event.down_pos = right_button_down_pos;
    event_queue.push_back( event );
}

void MouseInterface::setRightButtonUp( void )
{
    MouseEvent event;

    //assert( right_button_up == false );
    right_button_down = false;
    right_button_up = true;
    button_mask = button_mask & (~0x04);
    right_button_up_pos = mouse_pos;

    event.button = right_button;
    event.up_pos = right_button_up_pos;

    if ( right_button_dbclick_active == true ) {
        event.event = MouseEvent::EVENT_DBCLICK;
        right_button_dbclick_active = false;
    } else
        event.event  = MouseEvent::EVENT_CLICK;

    event.down_pos = right_button_down_pos;
    event_queue.push_back( event );

    event.event = MouseEvent::EVENT_UP;
    event_queue.push_back( event );
}

void MouseInterface::setRightButtonDoubleDown()
{
    right_button_down = true;
    right_button_dbclick_active = true;
    right_button_up = false;
    button_mask = button_mask | 0x04;
    right_button_down_pos = mouse_pos;
    right_button_hold_time = now();
}

void MouseInterface::setMiddleButtonDown()
{
    MouseEvent event;

    //assert( middle_button_down == false );
    middle_button_down = true;
    middle_button_up = false;
    button_mask = button_mask | 0x02;
    middle_button_down_pos = mouse_pos;
    middle_button_hold_time = now();

    event.button = middle_button;
    event.event = MouseEvent::EVENT_DOWN;
    event.down_pos = middle_button_down_pos;
    event_queue.push_back( event );
}

void MouseInterface::setMiddleButtonUp()
{
    MouseEvent event;

    //assert( middle_button_up == false );
    middle_button_down = false;
    middle_button_up = true;
    button_mask = button_mask & (~0x02);
    middle_button_up_pos = mouse_pos;

    event.button = middle_button;
    event.up_pos = middle_button_up_pos;

    if ( middle_button_dbclick_active == true ) {
        event.event = MouseEvent::EVENT_DBCLICK;
        middle_button_dbclick_active = false;
    } else
        event.event  = MouseEvent::EVENT_CLICK;

    event.down_pos = middle_button_down_pos;
    event_queue.push_back( event );

    event.event = MouseEvent::EVENT_UP;
    event_queue.push_back( event );
}

void MouseInterface::setMiddleButtonDoubleDown()
{
    middle_button_down = true;
    middle_button_dbclick_active = true;
    middle_button_up = false;
    button_mask = button_mask | 0x02;
    middle_button_down_pos = mouse_pos;
    middle_button_hold_time = now();
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

