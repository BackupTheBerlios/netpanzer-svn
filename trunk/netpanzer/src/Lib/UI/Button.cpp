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
#include <config.h>

#include "Button.hpp"
#include "2D/Color.hpp"
#include "2D/Palette.hpp"
#include "Util/Log.hpp"



#include <string>

namespace UI
{
    Button::Button(const std::string& text, iRect area, FontManager * fm)
        : Component(area), textLabel(text),  bgColor(157)
    {
        initialiseTextSurface(fm);
        clickState = false;
        setName(text);
    }

    void Button::initialiseTextSurface(FontManager * fm)
    {
        
        TTF_Font * font = fm->getFont("fixed10");
        if(font == 0){
            LOG(("Button::Button : unknown font: fixed10"));
            textSurface = 0;
        }else{
            //TODO : throw away indexed colors. Use 24 bits !
            Palette p;
            RGBColor c = p[::Color::white];
            SDL_Color c2;
            c2.r = c.red;
            c2.g = c.green;
            c2.b = c.blue;
            textSurface = TTF_RenderText_Solid(font, textLabel.c_str(), c2);
            
            textPosition.x =  area.min.x + ((area.max.x - area.min.x) - textSurface->w) /2;
            textPosition.y =  area.min.y + ((area.max.y - area.min.y) - textSurface->h) /2;

            //LOG(("area %d:%d->%d:%d", area.min.x, area.min.y, area.max.x, area.max.y));
        }
    }

    void Button::draw(Painter & painter)
    {
        //LOG(("DrawButton"));
        painter.setBrushColor(::Color::white);
        
        painter.setFillColor(bgColor);
        iRect t = area;
        t.max = iXY(area.max.x-1,area.max.y-1);
        painter.fillRect(t);
        painter.drawRect(t);
        if(textSurface != 0)
            painter.drawImage(textSurface, textPosition);
    }

//     void Button::mouseEvent(MouseEvent me){

//         if(me.getEventType() & UI::MOUSE_ENTERED)
//             LOG(("Mouse Entered"));

//         if(me.getEventType() & UI::MOUSE_EXITED)
//             LOG(("Mouse Exited"));

//         //if(me.getEventType() & UI::MOUSE_CLICKED)
//             // LOG(("Mouse Click"));
            
//     }

    void Button::mouseEntered(MouseEventParameter param){
        //LOG(("Mouse Entered"));
        bgColor = ::Color::blue;
    }


    void Button::mouseExited(MouseEventParameter param){
        //LOG(("Mouse Exited"));
        clickState = false;
        bgColor = ::Color::green;
    }
    
    void Button::mousePressed(MouseEventParameter param){
        if(param.getButtons() & SDL_BUTTON_LEFT)
            clickState = true;

    }
    void Button::mouseReleased(MouseEventParameter param){
        if(clickState)
            LOG(("%s : Click", getName().c_str()));
    }
}
    
