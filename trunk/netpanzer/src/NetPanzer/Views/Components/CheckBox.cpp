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

#include "Views/Components/CheckBox.hpp"
#include "MouseEvent.hpp"
#include "Views/Components/StateChangedCallback.hpp"

// draw
//---------------------------------------------------------------------------
void CheckBox::draw(Surface &dest)
{
    iRect bounds;

    getBounds(bounds);
    if ( dirty )
        render();

    surface.blt(dest, bounds.min.x, bounds.min.y);

} // end CheckBox::draw

void
CheckBox::render()
{
    surface.fill(Color::black);
    Surface text;
    text.renderText( label.c_str(), textColor, 0);
    surface.drawRect( iRect(0,0,12,12), Color::gray96);
    surface.drawRect( iRect(1,1,13,13), Color::white);
    surface.drawRect( iRect(1,1,12,12), Color::black);
    if (state) {
        surface.drawLine( 2, 2, 11, 11, Color::white);
        surface.drawLine( 2, 11, 11, 2, Color::white);
    }
    
    text.blt( surface, 14+2, (surface.getHeight()/2) - (text.getHeight()/2) );
    dirty = false;
}

// actionPerformed
//---------------------------------------------------------------------------
void CheckBox::actionPerformed(const mMouseEvent &me)
{
    if (	me.getID() == mMouseEvent::MOUSE_EVENT_CLICKED &&
            (me.getModifiers() & InputEvent::BUTTON1_MASK)) {
        state = !state;
        if(callback)
            callback->stateChanged(this);
        dirty = true;
    } else if (me.getID() == mMouseEvent::MOUSE_EVENT_ENTERED) {
        textColor = Color::yellow;
        dirty = true; // draw text in red
    } else if (me.getID() == mMouseEvent::MOUSE_EVENT_EXITED) {
        textColor = Color::white;
        dirty = true; // draw defaults;
    }


} // end CheckBox::actionPerformed
