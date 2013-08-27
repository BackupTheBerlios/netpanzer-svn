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

#include "Views/Components/Component.hpp"
#include "2D/TextRenderer.hpp"

class Separator : public Component
{
public:
    Separator( const char * text, unsigned width, PIX color )
    {
        text_render.setText(text);
        xend = width;
        this->color = color;
    }

    Separator(int x, int y, int endx, const char * text, PIX color)
    {
        text_render.setText(text);
        this->color = color;
        rect.setLocation( x, y );
        xend = endx - x;
    }
    
    void draw(Surface &dest)
    {
        int m = text_render.getHeight() / 2;
        dest.drawHLine(rect.getLocationX(), rect.getLocationY()+m, 19, color);
        text_render.draw(dest, rect.getLocationX() + 25, rect.getLocationY(), color);
        int lentxt = 30+text_render.getWidth();
        dest.drawHLine(rect.getLocationX()+lentxt,   rect.getLocationY()+m, xend - lentxt, color);
    }
    
    virtual void render()
    {
        // nothing
    }
    
private:
    TextRenderer text_render;
    int xend;
    PIX color;
};

#endif	/* SEPARATOR_HPP */

