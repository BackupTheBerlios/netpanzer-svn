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


#include "Views/Components/CheckBox.hpp"
#include "MouseEvent.hpp"

void
CheckBox::draw(Surface &dest)
{
    iRect r;
    getBounds(r);

    iRect boxr(r);

    boxr.max.x = boxr.min.x + 12;
    boxr.max.y = boxr.min.y + 12;

    dest.fillRect( boxr, Color::blue);
    dest.drawRect( boxr, Color::gray96);

    boxr.min.x += 1;
    boxr.min.y += 1;

    boxr.max.x += 1;
    boxr.max.y += 1;

    dest.drawRect(boxr, Color::white);

    boxr.max.x -= 1;
    boxr.max.y -= 1;

    dest.drawRect(boxr, Color::black);

    boxr.min.x += 3;
    boxr.min.y += 3;

    boxr.max.x -= 2;
    boxr.max.y -= 2;

    if (state) {
        dest.fillRect( boxr, Color::cobaltGreen);
    }
    
    // 4 = font height / 2
    // 3 = offset to center the label (14/2) - font height
    dest.bltString(position.x + 14+2, position.y + 3, label.c_str(), textColor);
}

// actionPerformed
//---------------------------------------------------------------------------
void CheckBox::actionPerformed(const mMouseEvent &me)
{
    if (	me.getID() == mMouseEvent::MOUSE_EVENT_CLICKED &&
            (me.getModifiers() & InputEvent::BUTTON1_MASK)) {
        state = !state;
        stateChanged();
    } else if (me.getID() == mMouseEvent::MOUSE_EVENT_ENTERED) {
        textColor = Color::yellow;
    } else if (me.getID() == mMouseEvent::MOUSE_EVENT_EXITED) {
        textColor = Color::white;
    }


} // end CheckBox::actionPerformed
