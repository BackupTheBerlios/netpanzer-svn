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
*/

#include "TextRenderer.hpp"
#include "TextRenderingSystem.hpp"
#include "Color.hpp"

TextRenderer::TextRenderer()
{
    surface.create(TextRenderingSystem::FONT_WIDTH, TextRenderingSystem::line_height());
    surface.fill(0);
    color = Color::white;
}

TextRenderer::TextRenderer(const char * utf8str)
{
    UString str(utf8str);
    setText(str);
    color = Color::white;
}

TextRenderer::TextRenderer(const UString& str)
{
    setText(str);
    color = Color::white;
}

void TextRenderer::setText(const char * utf8str)
{
    UString str(utf8str);
    setText(str);
}

void TextRenderer::setText(const UString& str)
{
    iXY area;
    TextRenderingSystem::getRenderedStringArea(str, area);
    if ( (surface.getWidth() != (unsigned)area.x) || (surface.getHeight() != (unsigned)area.y) )
    {
        surface.create( area.x, area.y);
    }
    
    TextRenderingSystem::renderString(str, surface);
}
