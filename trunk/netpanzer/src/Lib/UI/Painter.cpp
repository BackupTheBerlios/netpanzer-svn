/*Copyright (C) 1998 Pyrosoft Inc. (www.pyrosoftgames.com), Matthew Bogue
 
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

#include "Painter.hpp"
#include "2D/Palette.hpp"
#include "Util/Log.hpp"

namespace UI{

    Painter::Painter()
    {
        pushTransform(iXY(0,0));
    }
    
    Painter::~Painter()
    {

    }

    void Painter::setBrushColor(RGBColor color)
    {
        this->brushColor = Palette::findNearestColor(color);
    }

    void Painter::setFillColor(RGBColor color)
    {
        this->fillColor = Palette::findNearestColor(color);
    }

    void Painter::drawLine(iXY from, iXY to){
            
        to += currentTransform.translation;
        from += currentTransform.translation;
        drawingSurface->drawLine(from, to, 255);
    }

    void Painter::drawRect(iRect rect){
        rect.min += currentTransform.translation;
        rect.max += currentTransform.translation;
        drawingSurface->drawRect(rect, brushColor); 
    }

    void Painter::fillRect(iRect rect){
        rect.min += currentTransform.translation;
        rect.max += currentTransform.translation;
        drawingSurface->fillRect(rect, fillColor); 
    }

    void Painter::pushTransform(iXY translate){
        Transform t;
        t.translation = translate;
        transforms.push(t);
        currentTransform += t;
    }

    void Painter::popTransform(){
        currentTransform -= transforms.top();
        transforms.pop();
    }
}
