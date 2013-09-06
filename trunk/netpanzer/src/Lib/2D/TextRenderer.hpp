/*
Copyright (C) 2013 Netpanzer Team. (www.netpanzer.org), Aaron Perez

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
 * Created on February 21, 2013, 8:02 PM
 */

#ifndef TEXTRENDERER_HPP
#define	TEXTRENDERER_HPP

#include "Surface.hpp"
#include "Types/UString.hpp"
#include "Types/iRect.hpp"

class TextRenderer
{
private:
    Surface surface;
    PIX color;

public:
    TextRenderer();
    TextRenderer(const char * utf8str);
    TextRenderer(const UString& str);
    TextRenderer(const UString& str, const int width, const int firstLineOffset);

    void setText(const char * utf8str);
    void setText(const UString& str);
    void setMultilineText(const UString& str, const int width, const int firstLineOffset = 0);
    
    void setColor(PIX new_color) { color = new_color; }

    inline unsigned getWidth() const  { return surface.getWidth(); }
    inline unsigned getHeight() const { return surface.getHeight(); }

    void draw( Surface& dest, const int x, const int y, const PIX c = 0 ) const
    {
        surface.bltAlphaColor( dest, x, y, c ? c : color);
    }

    void drawCenter( Surface& dest, const PIX c = 0 ) const
    {
        surface.bltAlphaColor( dest,
                               (dest.getWidth()  - surface.getWidth())  / 2,
                               (dest.getHeight() - surface.getHeight()) / 2,
                               c ? c : color);
    }
    
    void drawPart( Surface& dest, const int x, const int y, const iRect& r, const PIX c = 0 ) const
    {
        if ( surface.getWidth() && surface.getHeight() )
        {
            surface.bltRectAlphaColor( dest, x, y, r, c ? c : color );
        }
    }

};

#endif	/* TEXTRENDERER_HPP */

