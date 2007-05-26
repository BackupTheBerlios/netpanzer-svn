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
#include "cMouse.hpp"

cMouse mouse;

// draw
//---------------------------------------------------------------------------
// Purpose: Draws the graphical mouse cursor.
//---------------------------------------------------------------------------
void cMouse::draw(Surface &dest)
{
    assert(pointer != 0);

    if (pointer) {
        pointer->nextFrame();
        pointer->bltTrans(dest, getScreenPos().x, getScreenPos().y);
    }
} // end draw

// setPointer
//---------------------------------------------------------------------------
// Purpose: Change the current image used for the mouse pointer.
//---------------------------------------------------------------------------
void cMouse::setPointer(Surface *pointer)
{
    assert(pointer != 0);

    cMouse::pointer = pointer;
} // end setPointer
