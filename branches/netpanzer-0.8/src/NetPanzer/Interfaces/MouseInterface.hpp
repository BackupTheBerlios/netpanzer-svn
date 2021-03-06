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

#include "2D/Surface.hpp"

#include "Util/TimeStamp.hpp"

#define _LEFT_BUTTON_MASK    0x01
#define _MIDDLE_BUTTON_MASK  0x02
#define _RIGHT_BUTTON_MASK   0x04

class MouseEvent
{
public:
    enum {
        EVENT_DOWN = 0x04,
        EVENT_UP = 0x08
    };
    unsigned char button;
    unsigned char event;
    iXY pos;
};

typedef std::deque<MouseEvent> MouseEventQueue;

class MouseInterface
{
private:
    static Surface mouse_cursor;
    static unsigned char cursor_x_size;
    static unsigned char cursor_y_size;

    typedef std::map<std::string,Surface*> cursors_t;
    static cursors_t cursors;

protected:
    static iXY mouse_pos;

    static float    button_hold_threshold;

    static bool  left_button_down;
    static bool  left_button_up;

    static bool right_button_down;
    static bool right_button_up;

    static bool middle_button_down;
    static bool middle_button_up;

    static unsigned char button_mask;

public:
    enum {
        left_button = 0x01,
        middle_button = 0x02,
        right_button = 0x04
    };

    enum CursorType {
        defaultcursor, noentry, move, select, target, allie, break_allie
    };

    static MouseEventQueue event_queue;

    static void initialize();
    static void shutdown();

    static inline void getMousePosition( int *x, int *y )
    {
        *x = mouse_pos.x;
        *y = mouse_pos.y;
    }

    static inline unsigned char getButtonMask()
    {
        return( button_mask );
    }

    static void setLeftButtonDown();
    static void setLeftButtonUp();

    static void setRightButtonDown();
    static void setRightButtonUp();

    static void setMiddleButtonDown();
    static void setMiddleButtonUp();

    static void setCursor(const char* cursorname);
    static void updateCursor();
};

#endif // ** _MOUSEINTERFACE_HPP
