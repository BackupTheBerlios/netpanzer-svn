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


// render
void
Button::render()
{
    if ( bimage.getNumFrames() == 1 ) {
        bimage.blt(surface, extraBorder, extraBorder);
    }
    else if ( bimage.getNumFrames() == 3 )
    {
        bimage.setFrame(bstate);
        bimage.blt(surface, extraBorder, extraBorder);
    }
    else
    {
        surface.fill(componentBodyColor);        
    }

    if ( borders[bstate][0]|extraBorder ) // only 1 | (binary or)
    {
        surface.drawButtonBorder(borders[bstate][0], borders[bstate][1]);
    }
    
    if ( label.length() )
    {
        Surface text;
        text.renderText( label.c_str(), textColor, 0);
        // blit centered and transparent
        text.bltTrans(surface, (surface.getWidth()/2) - (text.getWidth()/2),
                      (surface.getHeight()/2) - (text.getHeight()/2));        
    }
    
    dirty = false;
}

// actionPerformed
//---------------------------------------------------------------------------
void Button::actionPerformed(const mMouseEvent &me)
{
    if (me.getID() == mMouseEvent::MOUSE_EVENT_ENTERED
                || me.getID() == mMouseEvent::MOUSE_EVENT_RELEASED) {
        bstate = BOVER;
        textColor = Color::red;
        dirty = true; // draw text in red
    } else if (me.getID() == mMouseEvent::MOUSE_EVENT_EXITED) {
        bstate = BNORMAL;
        textColor = Color::white;
        dirty = true; // draw defaults;
    } else if (me.getID() == mMouseEvent::MOUSE_EVENT_PRESSED) {
        bstate = BPRESSED;
        textColor = Color::yellow;
        dirty = true;
    }
} // end Button::actionPerformed
