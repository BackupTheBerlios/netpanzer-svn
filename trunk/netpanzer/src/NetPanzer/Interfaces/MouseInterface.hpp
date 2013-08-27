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
#ifndef _MOUSEINTERFACE_HPP
#define _MOUSEINTERFACE_HPP

#include <string>
#include <map>
#include <deque>

#include "SDL.h"
#include "2D/Surface.hpp"
#include "GameInput/InputManager.hpp"

class MouseInterface
{
private:
    static Surface * cursor;
    static unsigned char cursor_x_size;
    static unsigned char cursor_y_size;

    typedef std::map<std::string,Surface*> cursors_t;
    static cursors_t cursors;

protected:
    static iXY mouse_offset;
    
public:

    enum CursorType {
        defaultcursor, noentry, move, select, target, allie, break_allie, CURSOR_TYPE_MAX
    };

    static void initialize();
    static void shutdown();
    
    static void draw(Surface &dest)
    {
        if (cursor)
        {
            cursor->bltTrans(dest, GameInput::InputManager::getMouseX() - mouse_offset.x,
                                   GameInput::InputManager::getMouseY() - mouse_offset.y);
        }
    }

    static void setCursor(const char* cursorname);
};

#endif // ** _MOUSEINTERFACE_HPP
