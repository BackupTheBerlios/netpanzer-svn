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
#ifndef __cMouse_hpp__
#define __cMouse_hpp__

#include <SDL.h>

#include "2D/Surface.hpp"
#include "Types/iXY.hpp"

#define LMOUSE_BUTTON_MASK 0x01
#define MMOUSE_BUTTON_MASK 0x02
#define RMOUSE_BUTTON_MASK 0x04

class cMouse
{
private:
    Surface *pointer;
    unsigned char button_mask;

public:
    cMouse()
        : pointer(0), button_mask(0)
    {}
    ~cMouse()
    {}

    // cMouse Operation Functions
    void draw(Surface &dest); // Handles the drawing of the cMouse cursor

    const Surface* getCurPointer() const
    {
        return pointer;
    }

    iXY getScreenPos() const
    {
        int x, y;
        SDL_GetMouseState(&x, &y);
        return iXY(x, y);
    }

    int getScreenX() const
    {
        return getScreenPos().x;
    }

    int getScreenY() const
    {
        return getScreenPos().y;
    }

    void setButtonMask(unsigned char mask)
    {
        button_mask = button_mask | mask;
    }

    void clearButtonMask(unsigned char mask)
    {
        button_mask = button_mask & mask;
    }

    unsigned char getButtonMask() const
    {
        return button_mask;
    }

    void setPointer(Surface *pointer);
}; // end cMouse

extern cMouse mouse;

#endif // end __cMouse_hpp__
