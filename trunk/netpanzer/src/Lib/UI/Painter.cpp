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

#include <assert.h>

#include "Painter.hpp"
#include "Util/Log.hpp"
#include "2D/Palette.hpp"

#include "DrawingFunctions.hpp"

namespace UI
{
    Painter::Painter(SDL_Surface* surface, FontManager * fm)
        : drawingSurface(surface)
    {
        if(fm == NULL){
            LOG(("Error : Painter::Painter() you must provide a valid FontManager"));
        }
        fontManager = fm;
        
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

        UI::drawRect(drawingSurface, rect, brushColor);

    }

    void Painter::drawString(iXY pos, const std::string & text, const std::string & font)
    {
        currentTransform.apply(pos);
        Palette p;
        RGBColor c = p[brushColor];
        SDL_Color c2;
        c2.r = c.red;
        c2.g = c.green;
        c2.b = c.blue;
                
        SDL_Surface * surface = TTF_RenderText_Solid(fontManager->getFont(font),text.c_str(), c2);

        SDL_Rect r;
        r.x = pos.x;
        r.y = pos.y;

        SDL_BlitSurface(surface, NULL, drawingSurface, &r);
    }

    void Painter::drawImage(SDL_Surface* surface, iXY pos)
    {
        currentTransform.apply(pos);
        SDL_Rect r;
        r.x = pos.x;
        r.y = pos.y;
        SDL_BlitSurface(surface, NULL, drawingSurface, &r);
    }

    void Painter::fillRect(iRect rect)
    {
        currentTransform.apply(rect);
        UI::fillRect(drawingSurface, rect, fillColor);
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
