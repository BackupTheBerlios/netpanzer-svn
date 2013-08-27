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

class Slider : public Component
{
public:
    Slider();
    Slider( const int minval, const int maxval, const int val,
            const unsigned width, const int changeEvent);
    virtual ~Slider();

    void setMin(int min_value);
    void setMax(int max_value);

    int setValue(int value);
    int getValue() const { return value; }

    virtual void setSize(const int x, const int y);

    virtual void draw(Surface &dest);

    void setChangeEvent(int event) { changeEvent = event; }

private:
    int changeEvent;
    
    bool isPressing;
    bool isHovering;
    void onSelectStart();
    void onSelectStop();
    void onHoverStart();
    void onHoverStop();
    void handleInput( GameInput::InputState * input );
    
    void render() {}
    
    int min_value;
    int max_value;
    int value;
    int selector_position;

    void update_selector_position();
};

#endif // SLIDER_HPP
