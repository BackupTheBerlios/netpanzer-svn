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

#include "Component.hpp"

namespace UI{

    UI::Component::Component(iRect area){
        this->area = area;
    }

    iRect UI::Component::getArea(void) const{
        return area;
    }

    void Component::drawFrame(Painter & painter){
        iRect t = area;
        t.max = iXY(area.max.x-1,area.max.y-1);
        painter.drawRect(t);
    }

}
