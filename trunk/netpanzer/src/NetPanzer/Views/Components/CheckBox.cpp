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

#include "CheckBox.hpp"
#include "MouseEvent.hpp"
#include "StateChangedCallback.hpp"

// draw
//---------------------------------------------------------------------------
void CheckBox::draw(Surface &dest)
{
    iRect bounds;

    getBounds(bounds);

    if (state) {
        drawCheckBox(dest);
        drawCheck(dest);
    } else {
        drawCheckBox(dest);
    }

    drawLabel(dest);

} // end CheckBox::draw

// drawCheckBox
//---------------------------------------------------------------------------
void CheckBox::drawCheckBox(Surface &dest)
{
    iRect bounds;

    getBounds(bounds);

    iRect r;

    r = iRect(bounds.min.x, bounds.min.y, bounds.max.x - 2, bounds.max.y - 2);
    dest.drawRect(r, Color::gray96);

    r = iRect(bounds.min.x + 1, bounds.min.y + 1, bounds.max.x - 1, bounds.max.y - 1);
    dest.drawRect(r, Color::white);

    r = iRect(bounds.min.x + 1, bounds.min.y + 1, bounds.max.x - 2, bounds.max.y - 2);
    dest.fillRect(r, Color::black);

} // end CheckBox::drawCheckBox

// drawCheck
//---------------------------------------------------------------------------
void CheckBox::drawCheck(Surface &dest)
{
    iXY pos(min + iXY(2,2));

    iXY length(size - iXY(4,4));

    dest.drawLine(pos, pos + length, Color::white);

    dest.drawLine(pos.x + length.x, pos.y, pos.x, pos.y + length.y, Color::white);

} // end CheckBox::drawCheck

// drawLabel
//---------------------------------------------------------------------------
void CheckBox::drawLabel(Surface &dest)
{
    dest.bltString( min.x + size.x + 8,
                    min.y + (size.y - Surface::getFontHeight()) / 2,
                    label.c_str(), Color::white);
} // end CheckBox::drawLabel

// actionPerformed
//---------------------------------------------------------------------------
void CheckBox::actionPerformed(const mMouseEvent &me)
{
    if (	me.getID() == mMouseEvent::MOUSE_EVENT_CLICKED &&
            (me.getModifiers() & InputEvent::BUTTON1_MASK)) {
        state = !state;
        if(callback)
            callback->stateChanged(this);
    }

} // end CheckBox::actionPerformed
