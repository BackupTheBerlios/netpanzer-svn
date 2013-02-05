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
#include "Views/Components/View.hpp"
#include "Views/Theme.hpp"
#include "Util/Log.hpp"

#include "MouseEvent.hpp"
#include "Actions/Action.hpp"

Button::Button()
{
    setTextColors(Color::white, Color::red, Color::yellow, Color::darkGray);
    position.zero();
    label.clear();
    bstate = BNORMAL;
    memset(borders, 0, sizeof(borders));
    extraBorder = 0;
    clickAction = 0;
    dirty = true;
}

Button::~Button()
{
    if ( clickAction && ! clickAction->isShared() )
    {
        delete clickAction;
        clickAction = 0;
    }
}

void
Button::setAction(Action* action)
{
    if ( clickAction && ! clickAction->isShared() )
    {
        delete clickAction;
    }

    clickAction = action;
}

// render
void
Button::render()
{
    surface.fill(0);
    dirty = false;
    if (!visible) return;
    if ( background_images.size() == 1 )
    {
        background_images[0]->bltTrans(surface, extraBorder + state_offset[bstate].x, extraBorder + state_offset[bstate].y); // blit full
    }
    else if ( background_images.size() >= 3 )
    {
        background_images[(bstate < background_images.size()) ? bstate : 0]->bltTrans(surface, extraBorder + state_offset[bstate].x, extraBorder + state_offset[bstate].y);
    }
    else
    {
//        surface.fill(componentBodyColor);
        surface.FillRoundRect(surface.getRect(), 3, componentBodyColor);
    }

    if ( borders[bstate][0]|extraBorder ) // only 1 | (binary or)
    {
        surface.RoundRect(surface.getRect(), 3, borders[bstate][0]);
//        surface.drawButtonBorder(borders[bstate][0], borders[bstate][1]);
    }

    if ( ! label.empty() )
    {
        surface.bltStringCenter(label.c_str(), textColors[bstate]);
    }

}

// actionPerformed
//---------------------------------------------------------------------------
void
Button::actionPerformed(const mMouseEvent &me)
{
    if ( bstate == BDISABLED || !visible)
    {
        return;
    }

    if (me.getID() == mMouseEvent::MOUSE_EVENT_ENTERED
                || me.getID() == mMouseEvent::MOUSE_EVENT_RELEASED)
    {
        bstate = BOVER;
        dirty = true; // draw text in red
    }
    else if (me.getID() == mMouseEvent::MOUSE_EVENT_EXITED)
    {
        bstate = BNORMAL;
        dirty = true; // draw defaults;
    }
    else if (me.getID() == mMouseEvent::MOUSE_EVENT_PRESSED)
    {
        bstate = BPRESSED;
        dirty = true;
    }
    else if (me.getID() == mMouseEvent::MOUSE_EVENT_CLICKED)
    {
        if ( clickAction )
        {
            clickAction->execute();
        }
    }
} // end Button::actionPerformed

Button *
Button::createTextButton( const NPString& label,
                            const iXY& loc,
                            const int bwidth,
                            Action * action)
{
    Button * b = new Button();
    b->setLabel(label);
    b->setLocation(loc);
    b->setTextButtonSize(bwidth);
    b->setNormalBorder();
    b->setAction(action);
    b->setTextColors(componentInActiveTextColor, componentFocusTextColor, componentActiveTextColor, componentInActiveTextColor);
    b->setStateOffset(Button::BPRESSED, 1, 1);

    if ( ! action )
    {
        LOGGER.warning("No action defined for button '%s'", label.c_str());
    }
    return b;
}

void Button::setImage( PtrArray<Surface>& sl )
{
    background_images.deleteAll();
    background_images.clear();

    for ( size_t n = 0; n < sl.size(); n++ )
    {
        background_images.push_back(sl[n]);
    }

    sl.clear();

    setSize(background_images[0]->getWidth(), background_images[0]->getHeight());
    dirty = true;
}

Button *
Button::createNewSpecialButton(    const NPString& label,
                                   const iXY& loc,
                                   int width,
                                   Action * action)
{
    Surface bitmap;
    bitmap.loadBMP(itButton);

    Surface bstart;
    bstart.grab(bitmap, iRect(0, 0, 15, bitmap.getHeight()));
    Surface bend;
    bend.grab(bitmap, iRect(bitmap.getWidth()-15, 0, bitmap.getWidth(), bitmap.getHeight()));
    Surface bmiddle;
    bmiddle.grab(bitmap, iRect(15, 0, bitmap.getWidth()-15, bitmap.getHeight()));

    Surface * spbutton = new Surface(width, bstart.getHeight());

    bstart.blt(*spbutton,0,0); // full blit

    int msize = bmiddle.getWidth();
    for (int i = 0; i < (int)((spbutton->getWidth())/msize);i++)
    {
        bmiddle.blt(*spbutton,15+(msize*i),0); // full blit
    }

    bend.blt(*spbutton,spbutton->getWidth()-15,0); // full blit

    PtrArray<Surface> bg(1);
    bg.push_back(spbutton);

    Button *b = new Button();
    b->setImage(bg);
    b->setLabel(label);
    b->setLocation(loc);
    b->setTextColors(ctTexteNormal, ctTexteOver, ctTextePressed, ctTexteDisable);
    b->setStateOffset(Button::BPRESSED, 1, 1);

    b->setAction(action);

    return b;
}
