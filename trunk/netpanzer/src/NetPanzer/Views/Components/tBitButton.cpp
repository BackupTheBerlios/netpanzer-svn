/*
Copyright (C) 2012 Netpanzer Team. (www.netpanzer.org), Laurent Jacques

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


#include "Views/Components/tBitButton.hpp"
#include "Views/Components/View.hpp"
#include "Views/Theme.hpp"
#include "2D/Palette.hpp"

#include "MouseEvent.hpp"

tBitButton::tBitButton(const std::string &cname)
    : Component("Button." + cname)
{
    position.zero();
    bstate = NORMAL;
    dirty = true;
}

void tBitButton::render()
{
    surface.fill(0);
    dirty = false;

    if ((enabled == false) || bstate == NORMAL)
    {
        bimage.bltTrans(surface, 0, 0);
        surface.bltLookup(surface.getRect(), Palette::darkGray256.getColorArray());
        return;
    }

    if (bstate == PRESSED)
    {
        bimage.bltTrans(surface, 1, 1);
    }
    else
    {
        bimage.bltTrans(surface, 0, 0);
    }
}

void tBitButton::actionPerformed(const mMouseEvent &me)
{
    if (enabled == false) return;
    
    if (me.getID() == mMouseEvent::MOUSE_EVENT_ENTERED
            || me.getID() == mMouseEvent::MOUSE_EVENT_RELEASED)
    {
        bstate = OVER;
        dirty = true;
    }
    else if (me.getID() == mMouseEvent::MOUSE_EVENT_EXITED)
    {
        bstate = NORMAL;
        dirty = true;
    }
    else if (me.getID() == mMouseEvent::MOUSE_EVENT_PRESSED)
    {
        bstate = PRESSED;
        dirty = true;
    }
    else if (me.getID() == mMouseEvent::MOUSE_EVENT_CLICKED)
    {
        ((View *)parent)->onComponentClicked(this);
    }
}

tBitButton *tBitButton::createButton(const std::string cname,const Surface &Bitmap, const iXY loc)
{
    tBitButton *b = new tBitButton(cname);
    b->setImage(Bitmap);
    b->setLocation(loc);
    return b;
}
