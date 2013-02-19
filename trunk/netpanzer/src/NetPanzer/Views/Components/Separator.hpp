/*
Copyright (C) 2013 Netpanzer Team. (www.netpanzer.org), Laurent Jacques

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

 *
 * Created on January 12, 2013, 9:10 PM
 */

#ifndef SEPARATOR_HPP
#define	SEPARATOR_HPP

#include "Component.hpp"
#include "Core/CoreTypes.hpp"

class Separator:public Component
{
public:
    Separator(int x, int y, int endx, const NPString& t, PIX color)
    {
        text = t;
        this->color = color;
        position.x = x;
        position.y = y;
        xend = endx - x;
    }
    
    void draw(Surface &dest)
    {
        dest.drawHLine(position.x, position.y+5, 19, color);
        dest.drawHLine(position.x+1, position.y+6, 19, Color::black);
        dest.bltStringShadowed(position.x+25,position.y, text.c_str(),  color, Color::black);
        int lentxt = 30+dest.getTextLength(text);
        dest.drawHLine(position.x+lentxt,   position.y+5, xend - lentxt - 1, color);
        dest.drawHLine(position.x+lentxt+1, position.y+6, xend - lentxt - 1, Color::black);
    }
    
    virtual void render()
    {
        // nothing
    }
    
    void actionPerformed(const mMouseEvent &me)
    {
        // nothing
    }
    
private:
    NPString text;
    int xend;
    PIX color;
};

#endif	/* SEPARATOR_HPP */

