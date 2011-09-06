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


#include "Views/Components/Button.hpp"
#include "Util/Log.hpp"

#include "MouseEvent.hpp"

void
Button::draw(Surface &dest)
{
    iRect r;
    getBounds(r);

    if ( bimage.getFrameCount() == 1 ) {
        bimage.blt(dest, position.x + extraBorder, position.y + extraBorder);
    }
    else if ( bimage.getFrameCount() == 3 )
    {
        bimage.setFrame(bstate);
        bimage.blt(dest, position.x + extraBorder, position.y + extraBorder);
    }
    else
    {
        dest.fillRect(r, componentBodyColor);
    }

    if ( borders[bstate][0]|extraBorder ) // only 1 | (binary or)
    {
        dest.drawButtonBorder(r, borders[bstate][0], borders[bstate][1]);
    }
    
    if ( label.length() )
    {
        // XXX 4 is half the height of text
        dest.bltString( position.x + (r.getSizeX()/2) - (Surface::getTextLength(label.c_str())/2),
                        position.y + (r.getSizeY()/2) - 4,
                        label.c_str(), textColor);
    }
}

// actionPerformed
//---------------------------------------------------------------------------
void Button::actionPerformed(const mMouseEvent &me)
{
    if (me.getID() == mMouseEvent::MOUSE_EVENT_ENTERED
                || me.getID() == mMouseEvent::MOUSE_EVENT_RELEASED) {
        bstate = BOVER;
        textColor = Color::yellow;
    } else if (me.getID() == mMouseEvent::MOUSE_EVENT_EXITED) {
        bstate = BNORMAL;
        textColor = Color::white;
    } else if (me.getID() == mMouseEvent::MOUSE_EVENT_PRESSED) {
        bstate = BPRESSED;
        textColor = Color::red;
        clicked();
    }
} // end Button::actionPerformed
