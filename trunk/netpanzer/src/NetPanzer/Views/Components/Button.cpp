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

#include "Button.hpp"
#include "ViewGlobals.hpp"
#include "MouseEvent.hpp"

// draw
//---------------------------------------------------------------------------
void Button::draw(Surface &dest)
{
    iRect bounds;
    getBounds(bounds);

    if (clicked) {
        dest.fillRect(bounds, componentBodyColor);
        drawBorder(dest, bounds, bottomRightBorderColor, topLeftBorderColor);
        dest.bltStringCenteredInRect(bounds, label.c_str(), Color::yellow);
    } else if (highlighted) {
        dest.fillRect(bounds, componentBodyColor);
        drawBorder(dest, bounds, topLeftBorderColor, bottomRightBorderColor);
        dest.bltStringCenteredInRect(bounds, label.c_str(), Color::red);
    } else {
        dest.fillRect(bounds, componentBodyColor);
        drawBorder(dest, bounds, topLeftBorderColor, bottomRightBorderColor);
        dest.bltStringCenteredInRect(bounds, label.c_str(), Color::white);
    }

} // end Button::draw

// actionPerformed
//---------------------------------------------------------------------------
void Button::actionPerformed(const mMouseEvent &me)
{
    if (me.getID() == mMouseEvent::MOUSE_EVENT_ENTERED) {
        highlighted = true;
    } else if (me.getID() == mMouseEvent::MOUSE_EVENT_EXITED) {
        highlighted = false;
        clicked = false;
    } else if (me.getID() == mMouseEvent::MOUSE_EVENT_PRESSED) {
        clicked = true;
    } else if (me.getID() == mMouseEvent::MOUSE_EVENT_RELEASED) {
        clicked = false;
    }

} // end Button::actionPerformed

void
Button::drawBorder(Surface &viewArea, iRect &bounds, Uint32 topLeftColor, Uint32 bottomRightColor)
{
    viewArea.drawHLine(bounds.min.x  ,bounds.min.y  ,bounds.max.x, topLeftColor);
    viewArea.drawVLine(bounds.min.x  ,bounds.min.y  ,bounds.max.y, topLeftColor);
    viewArea.drawHLine(bounds.min.x  ,bounds.max.y-1,bounds.max.x, bottomRightColor);
    viewArea.drawVLine(bounds.max.x-1,bounds.min.y  ,bounds.max.y, bottomRightColor);
}
