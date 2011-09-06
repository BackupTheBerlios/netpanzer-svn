/*
Copyright (C) 2011 by Aaron Perez <aaronps@gmail.com>

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

#include "Slider.hpp"
#include "2D/Color.hpp"
#include "Views/Components/MouseEvent.hpp"

#include "Util/Log.hpp"

#include <algorithm>

#include <math.h>

Slider::Slider()
{
    this->min_value = 1;
    this->max_value = 100;
    this->value = 1;
    this->setSize(106, 8);
}

Slider::~Slider()
{

}

void Slider::setSize(int x, int y)
{
    Component::setSize(x, 8);

    if ( y > 8 )
    {
        int to_move = (y - 8) / 2;
        setLocation( position.x, position.y + to_move);
    }

    update_step_rate();
    update_selector_position();
}

void Slider::draw(Surface &dest)
{
    iRect r;
    getBounds(r);

    r.min.y += 2;
    r.min.x += 2;
    r.max.y -= 3;
    r.max.x -= 3;
    dest.drawRect(r, Color::gray64);

    ++r.min.y;
    ++r.min.x;
    dest.fillRect(r, Color::chartreuse);

    r.min.y -= 3;
    r.min.x -= 3;
    r.max.y += 2;
//    r.max.x += 2;

    r.min.x += selector_position-1; // XXX why -1?
    r.max.x = r.min.x + 6;

    dest.drawRect(r, Color::gray64);

    ++r.min.y;
    ++r.min.x;
    dest.fillRect(r, Color::chartreuse);

}

void Slider::actionPerformed(const mMouseEvent &me)
{
    if ( me.getID() == mMouseEvent::MOUSE_EVENT_PRESSED
        || me.getID() == mMouseEvent::MOUSE_EVENT_DRAGGED )
    {
        iRect r;
        getBounds(r);

        int pressy = me.getY() - r.min.y;

        LOGGER.info("pressed in y: %d", pressy);
        if ( pressy >= 0 && pressy < 8 )
        {
            int pressx = me.getX() - r.min.x - 2;
            LOGGER.info("pressed in x: %d, rate is %f", pressx, step_rate);
            int val = round(pressx / step_rate); // XXX double to int conversion
            setValue(val+min_value);
            LOGGER.info("Setted new value to: %d", this->value);
            changed();
        }
    }
}

void Slider::setMax(int max_value)
{
    this->max_value = max_value;
    update_step_rate();
    update_selector_position();
}

void Slider::setMin(int min_value)
{
    this->min_value = min_value;
    update_step_rate();
    update_selector_position();
}

int Slider::setValue(int value)
{
    this->value = std::min(std::max(min_value, value), max_value);
    update_selector_position();
    return this->value;
}
