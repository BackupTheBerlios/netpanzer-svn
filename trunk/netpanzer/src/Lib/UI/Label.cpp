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

#include "config.h"
#include "Label.hpp"

#include <iostream>
namespace UI{

    void Label::updateTextSurface(void){
        TTF_Font * font = fontManager->getFont("fixed10");
        if(textSurface != 0)
            SDL_FreeSurface(textSurface);

        if(font !=0){
            textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);

            if(alignment & H_CENTER)
                textPosition.x = area.min.x + ((area.max.x - area.min.x) - textSurface->w) /2;
            else if(alignment & RIGHT)
                textPosition.x = area.min.x + ((area.max.x - area.min.x) - textSurface->w);
            else
                textPosition.x = area.min.x;
            
            if(alignment & V_CENTER)
                textPosition.y =  area.min.y + ((area.max.y - area.min.y) - textSurface->h) /2;        
        }else
            textSurface = 0;

    }

    void Label::setText(std::string newText){
        if(this->text != newText){
            this->text = newText;
            updateTextSurface();
        }
    }

    const std::string & Label::getText(void) const{
        return text;
    }

    SDL_Color Label::transformColor(Color col){
        SDL_Color sdlc;
        Palette p;
        RGBColor c = p[col];
        
        sdlc.r = c.red;
        sdlc.g = c.green;
        sdlc.b = c.blue;
        return sdlc;
    }

    void Label::setTextColor(Color col){
        textColor = transformColor(col);
//         std::cerr.flags ( std::ios_base::right | std::ios_base::hex | std::ios_base::showbase );
//         std::cerr << "Text Color : " << textColor.r << "," << textColor.g << "," << textColor.b << std::endl;
        updateTextSurface();
    }


    void Label::draw(Painter & painter){
        if(textSurface != 0)
            painter.drawImage(textSurface, textPosition);
        iRect t = area;
        t.max = iXY(area.max.x-1,area.max.y-1);
        painter.drawRect(t);

    }
}