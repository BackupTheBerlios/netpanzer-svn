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


#include "Button.hpp"
#include "Views/Theme.hpp"
#include "Util/Log.hpp"

#include "Actions/Action.hpp"
#include "2D/ComponentEvents.hpp"
#include "Util/NTimer.hpp"

Button::Button()
{
    setTextColors(Color::white, Color::red, Color::yellow, Color::darkGray);
    rect.zero();
    label.clear();
    bstate = BNORMAL;
    painted_bstate = BMAX_STATE;
    memset(borders, 0, sizeof(borders));
    extraBorder = 0;
    clickEvent = 0;
    selecting = false;
    
    repeat_timer.setTimeOut(0);
    repeat_initial = 0;
    repeat_other = 0;
}

Button::~Button()
{
    
}

void Button::setTextColors(PIX normal, PIX over, PIX pressed, PIX disabled)
{
    textColors[BNORMAL] = normal;
    textColors[BOVER] = over;
    textColors[BPRESSED] = pressed;
    textColors[BDISABLED] = disabled;
    painted_bstate = BMAX_STATE;
}

void Button::setLabel(const NPString& l)
{
    label = l;
    label_render.setText(l.c_str());
    painted_bstate = BMAX_STATE;
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
    painted_bstate = BMAX_STATE;
}

void Button::enable()
{
    if ( bstate == BDISABLED )
    {
        bstate = BNORMAL;
    }
}

void Button::disable()
{
    if ( bstate != BDISABLED )
    {
        bstate = BDISABLED;
    }
}

void Button::setExtraBorder()
{
    if ( !extraBorder )
    {
        extraBorder = 1;
        setSize( rect.getWidth(), rect.getHeight());
        painted_bstate = BMAX_STATE;
    }
}

void Button::clearExtraBorder()
{
    if ( extraBorder )
    {
        extraBorder = 0;
        setSize( rect.getWidth(), rect.getHeight());
        painted_bstate = BMAX_STATE;
    }
}


void Button::setUnitSelectionBorder()
{
    setExtraBorder();
    borders[BNORMAL]   = Color::darkGray;
    borders[BOVER]     = Color::red;
    borders[BPRESSED]  = Color::darkGray;
    borders[BDISABLED] = Color::darkGray;
    painted_bstate = BMAX_STATE;
}

void Button::setNormalBorder()
{
    setExtraBorder();
    borders[BNORMAL]   = topLeftBorderColor;
    borders[BOVER]     = topLeftBorderColor;
    borders[BPRESSED]  = bottomRightBorderColor;
    borders[BDISABLED] = Color::darkGray;
    painted_bstate = BMAX_STATE;
}

void Button::setRedGreenBorder()
{
    setExtraBorder();
    borders[BNORMAL]   = 0;
    borders[BOVER]     = Color::red;
    borders[BPRESSED]  = Color::green;
    borders[BDISABLED] = Color::darkGray;
    painted_bstate = BMAX_STATE;
}

void Button::clearBorder()
{
    memset(borders, 0, sizeof(borders));
    painted_bstate = BMAX_STATE;
}

void Button::setStateOffset(ButtonState state, int x, int y)
{
    state_offset[state].x = x;
    state_offset[state].y = y;
    painted_bstate = BMAX_STATE;
}

void Button::setSize(const int x, const int y)
{
    Component::setSize(x+(extraBorder*2), y+(extraBorder*2));
    surface.create(getWidth(), getHeight());
    painted_bstate = BMAX_STATE;
}

void Button::setTextButtonSize(int xsize)
{
    Component::setSize(xsize+(extraBorder*2), Surface::getFontHeight() + 4 + (extraBorder*2));
    painted_bstate = BMAX_STATE;
}

void Button::draw(Surface& dest) const
{
    surface.bltTrans(dest, rect.getLocationX(), rect.getLocationY());
}

void Button::logic()
{
    if ( painted_bstate != bstate )
    {
        render();
        painted_bstate = bstate;
    }
    
    if ( repeat_other && selecting && (bstate == BPRESSED) && repeat_timer.isTimeOut())
    {
        events->push(clickEvent);
        repeat_timer.setTimeOut(repeat_other);
        repeat_timer.reset();
    }
    
}

// render
void
Button::render()
{
    surface.fill(0);

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
        surface.FillRoundRect(surface.getRect(), 3, componentBodyColor);
    }

    if ( borders[bstate] && extraBorder )
    {
        surface.RoundRect(surface.getRect(), 3, borders[bstate]);
    }

    if ( ! label.empty() )
    {
        label_render.draw(surface,
                          (getWidth()  - label_render.getWidth())  / 2,
                          (getHeight() - label_render.getHeight()) / 2,
                          textColors[bstate]);
    }

}

void Button::onSelectStart()
{
    if ( bstate != BDISABLED )
    {
        bstate = BPRESSED;
        selecting = true;
        if ( repeat_other )
        {
            events->push(clickEvent);
            repeat_timer.setTimeOut(repeat_initial);
            repeat_timer.reset();
        }
    }
}

void Button::onSelectStop()
{
    if ( bstate == BPRESSED )
    {
        if ( ! repeat_other )
        {
            events->push(clickEvent);
        }
        bstate = BOVER;
    }
    
    selecting = false;
}

void Button::onHoverStart()
{
    if ( bstate != BDISABLED )
    {
        if ( selecting )
        {
            bstate = BPRESSED;
            if ( repeat_other )
            {
                events->push(clickEvent);
                repeat_timer.setTimeOut(repeat_initial);
                repeat_timer.reset();
            }   
        }
        else
        {
            bstate = BOVER;
        }
    }
}

void Button::onHoverStop()
{
    if ( bstate != BDISABLED )
    {
        bstate = BNORMAL;
    }
}

Button *
Button::createTextButton( const NPString& label,
                            const iXY& loc,
                            const int bwidth )
{
    Button * b = new Button();
    b->setLabel(label);
    b->setLocation(loc);
    b->setTextButtonSize(bwidth);
    b->setNormalBorder();
    b->setTextColors(componentInActiveTextColor, componentFocusTextColor, componentActiveTextColor, componentInActiveTextColor);
    b->setStateOffset(Button::BPRESSED, 1, 1);
    return b;
}

Button *
Button::createNewSpecialButton(    const NPString& label,
                                   const iXY& loc,
                                   int width )
{
    Surface bitmap;
    bitmap.loadBMP(itButton);

    Surface bstart;
    bstart.grab(bitmap, iRect(0, 0, 15, bitmap.getHeight()));
    Surface bend;
    bend.grab(bitmap, iRect(bitmap.getWidth()-15, 0, 15, bitmap.getHeight()));
    Surface bmiddle;
    bmiddle.grab(bitmap, iRect(15, 0, bitmap.getWidth()-30, bitmap.getHeight()));

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

    return b;
}
