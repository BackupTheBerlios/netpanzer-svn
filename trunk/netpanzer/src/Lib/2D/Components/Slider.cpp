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
#include "Views/Components/ViewGlobals.hpp"

#include "Util/Log.hpp"
#include "2D/ComponentEvents.hpp"
#include "GameInput/InputConstants.hpp"
#include "GameInput/InputState.hpp"
#include "GameInput/InputManager.hpp"

#include <stdio.h>
#include <string.h>

Slider::Slider()
{
    this->min_value = 1;
    this->max_value = 100;
    this->value = 1;
    this->setSize(106, 8);
    changeEvent = 0;

    isPressing = false;
    isHovering = false;
}

Slider::Slider( const int minval, const int maxval, const int val,
                const unsigned width, const int changeEvent )
{
    this->min_value = minval;
    this->max_value = maxval;
    this->setSize(width, 0);
    this->changeEvent = changeEvent;
    
    this->setValue(val);
    
    isPressing = false;
    isHovering = false;
}

Slider::~Slider()
{
    
}

void Slider::setSize(const int x, const int y)
{
    Component::setSize(x, 20);

    update_selector_position();
}

void Slider::draw(Surface &dest) const
{
    iRect r(rect);
    
    r.grow( -(getHeight()/2), -(getHeight()/4));

    dest.FillRoundRect(r, 3, componentBodyColor);
    dest.RoundRect(r, 3, Color::darkGray);

    r.setLocation(rect.getLocationX() + selector_position, rect.getLocationY());
    r.setSize( getHeight(), getHeight());

    dest.FillRoundRect(r, getHeight()/2, isHovering ? componentActiveTextColor : Color::gray);
    dest.RoundRect(r, getHeight()/2, Color::gray96);
}

void Slider::handleInput( GameInput::InputState * input )
{
    if ( isPressing )
    {
        const int mx = GameInput::InputManager::getMouseX();
        
        int new_value = 0;
        const int range = max_value - min_value;
        if ( range > 0 )
        {
            const int mov_size = rect.getWidth() - rect.getHeight();
            const int x = mx - (rect.getLocationX() + (rect.getHeight() / 2));
            if ( x <= 0 )
            {
                new_value = min_value;
                selector_position = 0;
            }
            else if ( x >= mov_size )
            {
                new_value = max_value;
                selector_position = mov_size;
            }
            else
            {
                int between = mov_size / range;
                new_value = min_value + (((x + (between/2)) * range ) / mov_size);
                selector_position = x;
            }
        }
        else
        {
            new_value = min_value;
            selector_position = 0;
        }

        int sp = selector_position; // this to keep the bar nice
        setValue( new_value );
        selector_position = sp;

    }
    else
    {
        if ( input->hasAction(GameInput::Action_StartLeft) )
        {
            setValue(value - 1);
        }
        else if ( input->hasAction(GameInput::Action_StartRight) )
        {
            setValue(value + 1);
        }
    }

}

void Slider::onSelectStart()
{
    isPressing = true;
}

void Slider::onSelectStop()
{
    isPressing = false;
}

void Slider::onHoverStart()
{
    isHovering = true;
}

void Slider::onHoverStop()
{
    isHovering = false;
}

void Slider::update_selector_position()
{
    const int range = max_value - min_value;
    if ( range > 0 )
    {
        selector_position = ((value-min_value) * (rect.getWidth() - rect.getHeight())) / (max_value - min_value);
    }
    else
    {
        selector_position = 0;
    }
}

void Slider::setMax(int max_value)
{
    this->max_value = max_value > min_value ? max_value : min_value;
    setValue(value);
}

void Slider::setMin(int min_value)
{
    this->min_value = min_value < max_value ? min_value : max_value;
    setValue(value);
}

int Slider::setValue(int new_value)
{
    int old_value = value;
    
    if ( new_value < min_value )      value = min_value;
    else if ( new_value > max_value ) value = max_value;
    else                              value = new_value;

    if ( (value != old_value) && events )
    {
        events->push( changeEvent );
    }

    update_selector_position();
    return value;
}
