/*
Copyright (C) 1998 Pyrosoft Inc. (www.pyrosoftgames.com), Matthew Bogue
 
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
#include <config.h>

#include "Painter.hpp"
#include "Util/Log.hpp"

#include "Line.hpp"

namespace UI
{
    Painter::Painter(SDL_Surface* surface)
        : drawingSurface(surface)
    {
    }
    
    Painter::~Painter()
    {
        if(transforms.size() > 0)
            LOG(("Warning: transform list is not empty at Painter::~Painter"));
    }

    void Painter::setSurface(SDL_Surface* surface)
    {
        drawingSurface = surface;
    }

    void Painter::setBrushColor(Color color)
    {
        brushColor = color;
    }

    void Painter::setFillColor(Color color)
    {
        fillColor = color;
    }

    void Painter::drawLine(iXY from, iXY to)
    {
        currentTransform.apply(from);
        currentTransform.apply(to);

        UI::drawLine(drawingSurface, from, to, brushColor);
    }

    void Painter::drawRect(iRect rect)
    {
        currentTransform.apply(rect);

        UI::drawLine(drawingSurface, rect.min, iXY(rect.max.x, rect.min.y),
                 brushColor);
        UI::drawLine(drawingSurface, iXY(rect.max.x, rect.min.y), rect.max,
                 brushColor);
        UI::drawLine(drawingSurface, rect.min, iXY(rect.min.x, rect.max.y),
                brushColor);
        UI::drawLine(drawingSurface, iXY(rect.min.x, rect.max.y), rect.max,
                brushColor);
    }

    void Painter::fillRect(iRect rect)
    {
        // TODO
    }

    void Painter::pushTransform(iXY translate)
    {
        transforms.push(Transform(translate));
        currentTransform += transforms.top();
    }

    void Painter::popTransform()
    {
        assert(transforms.size() > 0);

        currentTransform -= transforms.top();
        transforms.pop();
    }
}
