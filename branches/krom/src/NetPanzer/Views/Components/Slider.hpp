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
#ifndef SLIDER_HPP
#define SLIDER_HPP

#include "Views/Components/Component.hpp"
#include "Util/MinSignal.hpp"

class Slider : public Component
{
protected:
    virtual void draw(Surface &dest);
    virtual void actionPerformed(const mMouseEvent &me);

public:
    Slider();
    virtual ~Slider();

    MinSignal changed;

    void setMin(int min_value);
    void setMax(int max_value);

    int setValue(int value);
    int getValue() const { return value; }

    virtual void setSize(int x, int y);

private:
    int min_value;
    int max_value;
    int value;

    // total size is size-6, that is because there are 3 extra pixels left and right
    // note that all the range has 1 element less, this is the correct way.

    void update_step_rate() { step_rate = (float)(size.x-6) / (max_value - min_value); }
    float step_rate;

    void update_selector_position() { selector_position = (value-min_value) * step_rate; } // XXX float to int conversion
    int selector_position;
};

#endif // SLIDER_HPP
