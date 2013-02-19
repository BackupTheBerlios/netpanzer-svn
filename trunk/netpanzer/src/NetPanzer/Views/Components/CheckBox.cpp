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

#include "CheckBox.hpp"
#include "2D/Color.hpp"
#include "ViewGlobals.hpp"

CheckBox::CheckBox(const int x, const int y, const NPString& label, bool* state)
    : Component(), label(label), state(state)
{
    setSize( 20+label.length()*8, 14);
    setLocation(x, y);
    textColor = componentActiveTextColor;
}

void CheckBox::draw(Surface& s)
{
    iRect r(position.x, position.y, 14, 14);
    s.FillRoundRect( r,2,  componentBodyColor);

    r.grow(-1,-1);
    s.RoundRect( r, 2, textColor);

    s.bltString(r.getLocationX() + 17, r.getLocationY() + 1, label.c_str(), textColor);

    if ( *state )
    {
        int x = r.getLocationX() + 2;
        int y = r.getLocationY() + 5;
        s.drawLine(x, y, x+3, y+3, textColor);
        y += 1;
        s.drawLine(x, y, x+3, y+3, textColor);
        y+=3;
        x+=3;
        s.drawLine(x, y, x+3, y-6, textColor);
        y -= 1;
        s.drawLine(x, y, x+3, y-6, textColor);
    }
}

void CheckBox::actionPerformed(const mMouseEvent& me)
{
    if (    ( me.getID() == mMouseEvent::MOUSE_EVENT_CLICKED )
         && ( me.getModifiers() & InputEvent::BUTTON1_MASK) )
    {
        *state = !(*state);
        onStateChanged();
    }
    else if ( me.getID() == mMouseEvent::MOUSE_EVENT_ENTERED )
    {
        textColor = componentFocusTextColor;
    }
    else if ( me.getID() == mMouseEvent::MOUSE_EVENT_EXITED )
    {
        textColor = componentActiveTextColor;
    }
}