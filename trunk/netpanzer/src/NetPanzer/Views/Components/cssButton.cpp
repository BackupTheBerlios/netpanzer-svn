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


#include "Views/Components/cssButton.hpp"
#include "Views/Components/View.hpp"
#include "Util/Log.hpp"

#include "MouseEvent.hpp"

cssButton::cssButton(const std::string &cname)
    : Component("Button." + cname)
{
    setTextColors(Color::white, Color::red, Color::yellow, Color::darkGray);
    position.zero();
    label.clear();
    bstate = NORMAL;
    dirty = true;
}

// render
void
cssButton::render()
{
    surface.fill(0);
    Surface text;
    text.renderText( label.c_str(), textColors[bstate], 0);
    if (bstate == PRESSED)
    {
        bimage.bltTrans(surface, 1, 1);
        text.bltTrans(surface, ((surface.getWidth()/2) - (text.getWidth()/2))+1,
                      ((surface.getHeight()/2) - (text.getHeight()/2))+1);
    }
    else
    {
        bimage.bltTrans(surface, 0, 0);
        text.bltTrans(surface, (surface.getWidth()/2) - (text.getWidth()/2),
                      (surface.getHeight()/2) - (text.getHeight()/2));
    }

    dirty = false;
}

// actionPerformed
//---------------------------------------------------------------------------
void
cssButton::actionPerformed(const mMouseEvent &me)
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
} // end Button::actionPerformed

cssButton *
cssButton::createcssButton( std::string cname,
                            std::string label,
                            iXY loc)
{
    Surface bstart;
    bstart.loadBMP("pics/backgrounds/menus/buttons/default/bs-black.bmp");
    Surface bend;
    bend.loadBMP("pics/backgrounds/menus/buttons/default/be-black.bmp");

    Surface spbutton(Surface::getTextLength(label)+20, bstart.getHeight(), 1);

    spbutton.setFrame(0);
    bstart.blt(spbutton,0,0);
    bend.blt(spbutton,spbutton.getWidth()-10,0);

    cssButton *b = new cssButton(cname);
    b->setImage(spbutton);
    b->setLabel(label);
    b->setLocation(loc);
    b->setTextColors(Color::white, Color::yellow, Color::yellow, Color::darkGray);

    return b;
}

cssButton *
cssButton::createcssButton( std::string cname,
                            std::string label,
                            iXY loc, int width)
{
    Surface bstart;
    bstart.loadBMP("pics/backgrounds/menus/buttons/default/bs-black.bmp");
    Surface bend;
    bend.loadBMP("pics/backgrounds/menus/buttons/default/be-black.bmp");

    Surface spbutton(width, bstart.getHeight(), 1);

    spbutton.setFrame(0);
    bstart.blt(spbutton,0,0);
    bend.blt(spbutton,spbutton.getWidth()-10,0);

    cssButton *b = new cssButton(cname);
    b->setImage(spbutton);
    b->setLabel(label);
    b->setLocation(loc);
    b->setTextColors(Color::white, Color::yellow, Color::yellow, Color::darkGray);

    return b;
}
