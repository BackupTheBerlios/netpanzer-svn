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

#include "VerticalScrollBox.hpp"

namespace UI{
    VerticalScrollBox::VerticalScrollBox(iRect area):Component(area){
        int width = area.max.x - area.min.x;
        int height = area.max.y - area.min.y;
        scrollDown = new Button("scroll_down_1.png", "scroll_down_2.png", iRect(width-8, height-8, width-1, height),0); //Test, don't panic :)
        scrollUp = new Button("scroll_up_1.png", "scroll_up_2.png", iRect(width-8, 0, width-1, 7) , 0); //Test, don't panic :)
        scrollDown->addCallback(this);
        scrollUp->addCallback(this);
        viewPort = new VerticalViewPort(iRect(0,0, width-8, height));
    }

    VerticalScrollBox::~VerticalScrollBox(void){
        delete scrollUp;
        delete scrollDown;
        delete viewPort;
    }

    void VerticalScrollBox::draw(Painter & painter){
        drawFrame(painter);
        painter.pushTransform(area.min);
        scrollUp->draw(painter);
        scrollDown->draw(painter);
        painter.drawLine(iXY(area.max.x - area.min.x - 9, 0), iXY( area.max.x - area.min.x - 9, area.max.y - area.min.y -1));
        viewPort->draw(painter);

        float r = (float)viewPort->getArea().getSizeY() / viewPort->getVerticalSize();
        if (r > 1.0f) r = 1.0f;
        int sy = (int) ((area.getSizeY() - 18) * r);

        float p = (float)( -viewPort->getVerticalOffset()) / (viewPort->getVerticalSize() - viewPort->getArea().getSizeY());
        int py = (int) ((viewPort->getArea().getSizeY() - sy - 18) * p);
        

        painter.setFillColor(::Color::green);
        painter.drawRect(iRect(area.getSizeX() - 8, 9 + py, area.getSizeX()-2, 9 + sy + py));
        painter.fillRect(iRect(area.getSizeX() - 7, 10 + py, area.getSizeX()-2, 9 + sy + py));
        
        painter.popTransform();
    }

    void VerticalScrollBox::buttonPressed(MouseEventParameter& , Button * source){
        if(source == scrollUp && viewPort->getVerticalOffset() < 0)
            viewPort->addVerticalOffset(5);
        if(source == scrollDown &&  viewPort->getArea().getSizeY() - viewPort->getVerticalOffset() < viewPort->getVerticalSize())
            viewPort->addVerticalOffset(-5);
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

    int VerticalScrollBox::addBoxElement(BoxElement * box){
        viewPort->addComponent(box);
        return viewPort->getLastElementIndex();
    }


    void BoxElement::draw(Painter & painter){
        painter.pushTransform(iXY(0, position * area.getSizeY()));
        Container::draw(painter);
        painter.drawLine(iXY(area.min.x, area.max.y), area.max);
        painter.popTransform();
    }

    void VerticalScrollBox::VerticalViewPort::draw(Painter & painter){
        painter.setClipRect(area);
        painter.pushTransform(offset);
        
        Container::draw(painter);
        painter.popTransform();
        painter.unsetClipRect();
    }

    int VerticalScrollBox::VerticalViewPort::getVerticalSize(void){
        int s = 0;
        std::list<Component *>::iterator i;
        for(i = components.begin(); i != components.end(); i++){
            s += (*i)->getArea().getSizeY();
        }
        return s;
    }


}
