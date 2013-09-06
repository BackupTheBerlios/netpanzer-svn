/*
 Copyright (C) 2008 by Aaron Perez <aaronps@gmail.com>
 
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

#ifndef _LABEL_HPP_
#define _LABEL_HPP_

#include "Views/Components/Component.hpp"
#include "Core/CoreTypes.hpp"
#include "2D/Color.hpp"
#include "2D/TextRenderer.hpp"
#include "Types/UString.hpp"

class Label : public Component
{
public:
    Label(int x, int y, const NPString& t, PIX color)
    {
        text_render.setText(t.c_str());
        text_color = color;
        rect.setLocation(x, y);
    }
    
    Label( const UString& text, PIX color )
    {
        text_render.setText( text );
        text_color = color;
    }
    
    void setText(const NPString& t) { text_render.setText(t.c_str()); }
    
    size_t getTextWidth() const { return text_render.getWidth(); }
    size_t getTextHeight() const { return text_render.getHeight(); }
    
    void draw(Surface &dest) const;
    
private:
    TextRenderer text_render;
    PIX text_color;
};

#endif
