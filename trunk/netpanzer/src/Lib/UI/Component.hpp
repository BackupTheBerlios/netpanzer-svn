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

#ifndef __UI_Component_hpp__
#define __UI_Component_hpp__

#include <string>

#include "Painter.hpp"
#include "Types/iRect.hpp"
#include "MouseEventParameter.hpp"
#include "Util/NoCopy.hpp"

namespace UI{
    
    class Component : public NoCopy {
    protected:
        iRect area; //drawing area
        std::string name; //name, used for debugging
        

    public:
        Component(iRect area);

        virtual ~Component(void){
        }

        iRect getArea(void) const;
        
        virtual void drawFrame(Painter & painter);

        virtual void draw(Painter & painter) = 0;

        void setName(const std::string & name){
            this->name = name;
        }

        const std::string & getName(void) const{
            return name;
        }

        virtual void mouseMoved(MouseEventParameter ){};
        virtual void mousePressed(MouseEventParameter ){};
        virtual void mouseReleased(MouseEventParameter ){};
        virtual void mouseEntered(MouseEventParameter ){};
        virtual void mouseExited(MouseEventParameter ){};

        bool contains(const iXY &point){
            return area.contains(point);
        }

    };

}

#endif
