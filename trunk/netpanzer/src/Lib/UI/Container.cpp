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

#include "Container.hpp"
#include <iostream>
#include "Util/Log.hpp"
#include "2D/Color.hpp"
#include "config.h"

namespace UI{
    Container::Container(iRect area):Component(area), previousMousePosition(-1,-1){
        lastFocusedComponent = 0;
        previousMouseState = 0;
    }

    Container::Container(iXY position, iXY size):Component(iRect(position, position+size)), previousMousePosition(-1,-1)
    {
        lastFocusedComponent = 0;
        previousMouseState = 0;
    }

    /**
     * Delete a container and all of its subcomponent
     */
    Container::~Container(void)
    {
        std::list<Component *>::iterator i;
        for(i = components.begin(); i != components.end(); i++){
            if(*i != 0)
                delete *i;
        }
    }

    /**
     * Draw a container and all its subcomponent
     * Adjust coordinates with his position
     */
    void Container::draw(Painter & painter)
    {
        painter.setBrushColor(::Color::white);
        iRect t = area;
        t.max = iXY(area.max.x-1,area.max.y-1);
        painter.drawRect(t);

        painter.pushTransform(area.min);

        std::list<Component *>::iterator i;
        for(i = components.begin(); i != components.end(); i++)
            (*i)->draw(painter);
        painter.popTransform();
    }

    void Container::addComponent(Component * comp)
    {
        components.push_front(comp);
    }

    void Container::removeComponent(Component * comp)
    {
        components.remove(comp);
    }

    void Container::processEvents(iXY mousePosition, int mouseButtons, SDLMod modifier){
        mousePosition = mousePosition - area.min;

        mouseMoved(MouseEventParameter(mouseButtons, mousePosition, modifier));
    }

    void Container::mouseMoved(MouseEventParameter param){
        iXY mousePosition = param.getPosition();

        if(lastFocusedComponent != 0 && !lastFocusedComponent->contains(mousePosition)){
            lastFocusedComponent->mouseExited(param);
            lastFocusedComponent = 0;
        }

        Component * c = findContainingComponent(mousePosition);
        

        if(c != 0){
        
            c->mouseMoved(MouseEventParameter(param, mousePosition - c->getArea().min));
    
            if(int mouseChange = previousMouseState ^ param.getButtons()){
                // On mouse button changed (pressed or released)
                //std::cerr << getName() <<  " : Mouse changed in " << c->getName() << ". new state : " <<std::hex << mouseChange << std::endl;

                Uint8 releasedButtons = previousMouseState & mouseChange;
                Uint8 pressedButtons = param.getButtons() & mouseChange;
                
                if(releasedButtons != 0)
                    c->mouseReleased(MouseEventParameter(releasedButtons, mousePosition, param.getModifier()));
                if(pressedButtons != 0)
                    c->mousePressed(MouseEventParameter(pressedButtons, mousePosition, param.getModifier()));
            }

            if(lastFocusedComponent == 0 || c != lastFocusedComponent){
                c->mouseEntered(param);
                lastFocusedComponent = c;
            }
        }
        previousMouseState = param.getButtons();
    }

    void Container::mouseEntered(MouseEventParameter param){
        //LOG(("%s : Mouse Entered", getName().c_str()));
//         iXY p = param.getPosition() - area.min;
//         Component * c = findContainingComponent(p);
//         if(c != 0)
//             c->mouseEntered(MouseEventParameter(param,p));
    }


    void Container::mouseExited(MouseEventParameter param){
        //LOG(("%s : Mouse Exited", getName().c_str()));
    }
    

    Component * Container::findContainingComponent(iXY point){
        std::list<Component *>::iterator i = components.begin();
        while(i != components.end() && !(*i)->contains(point))
            i++;
        if(i==components.end())
            return 0;
        return *i;
        
    }
}
