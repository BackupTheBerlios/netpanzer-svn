/*
Copyright (C) 2013 by Aaron Perez <aaronps@gmail.com>

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

 * Created on January 12, 2013, 7:37 PM
 */

#ifndef BOXEDLABEL_HPP
#define	BOXEDLABEL_HPP

#include "Component.hpp"
#include "Core/CoreTypes.hpp"

class BoxedLabel : public Component
{
public:
     BoxedLabel(int x, int y, int w, const NPString& t, PIX color, PIX bgcolor, bool hasBorder = false)
    {
        text = t;
        background_color = bgcolor;
        text_color = color;
        position.x = x;
        position.y = y;
        setSize(w, 16);
        bordered = hasBorder;
    }
    
    void draw(Surface &dest)
    {
        iRect r(position.x, position.y, size.x, size.y);
        dest.FillRoundRect(r, 3, background_color);
        if ( bordered )
        {
            dest.RoundRect(r, 3, Color::lightGray);
        }
        dest.bltStringCenteredInRect(r, getText().c_str(), text_color);
    }
    
    void render()
    {
        // nothing
    }
    
    void actionPerformed(const mMouseEvent &me)
    {
        // nothing
    }
    
    void setText(const NPString& t) { text = t; }
    virtual NPString getText() const { return text; }
    
private:
    NPString text;
    bool bordered;
    PIX background_color;
    PIX text_color;
};

#endif	/* BOXEDLABEL_HPP */

