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

#include "VerticalScrollBox.hpp"

namespace UI{
    VerticalScrollBox::VerticalScrollBox(iRect area):Component(area){
        int width = area.max.x - area.min.x;
        int height = area.max.y - area.min.y;
        scrollDown = new Button("scroll_down_1.png", "scroll_down_2.png", iRect(width-8, height-8, width-1, height),0); //Test, don't panic :)
        scrollUp = new Button("scroll_up_1.png", "scroll_up_2.png", iRect(width-8, 0, width-1, 7) , 0); //Test, don't panic :)
        scrollDown->addCallback(this);
        scrollUp->addCallback(this);
    }

    void VerticalScrollBox::draw(Painter & painter){
        painter.pushTransform(area.min);
        scrollUp->draw(painter);
        scrollDown->draw(painter);
        painter.popTransform();
    }

    void VerticalScrollBox::buttonPressed(MouseEventParameter & event, Button * source){
        std::cerr << "click" << std::endl;
    }

    void VerticalScrollBox::mouseEntered(MouseEventParameter param){
         iXY mousePosition = param.getPosition() - area.min;
         if(scrollDown->contains(mousePosition))
             scrollDown->mouseEntered(param);
         if(scrollUp->contains(mousePosition))
             scrollUp->mouseEntered(param);         
    }

    void VerticalScrollBox::mouseExited(MouseEventParameter param){
         iXY mousePosition = param.getPosition() - area.min;
         if(scrollDown->contains(mousePosition))
             scrollDown->mouseExited(param);
         if(scrollUp->contains(mousePosition))
             scrollUp->mouseExited(param);         
    }

    void VerticalScrollBox::mousePressed(MouseEventParameter param){
         iXY mousePosition = param.getPosition() - area.min;
         if(scrollDown->contains(mousePosition))
             scrollDown->mousePressed(param);
         if(scrollUp->contains(mousePosition))
             scrollUp->mousePressed(param);         
    }

    void VerticalScrollBox::mouseReleased(MouseEventParameter param){
         iXY mousePosition = param.getPosition() - area.min;
         if(scrollDown->contains(mousePosition))
             scrollDown->mouseReleased(param);
         if(scrollUp->contains(mousePosition))
             scrollUp->mouseReleased(param);         
    }

}
