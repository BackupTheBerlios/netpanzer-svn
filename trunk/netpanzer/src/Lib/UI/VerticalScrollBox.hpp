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

#ifndef __UI_VERTICAL_SCROLL_BOX_HPP__
#define __UI_VERTICAL_SCROLL_BOX_HPP__


#include "Container.hpp"

#include "Button.hpp"

namespace UI{
    class VerticalScrollBox : public Component, public ButtonCallback{
    public:
        VerticalScrollBox(iRect area);
        void draw(Painter & painter);
    
        void buttonPressed(MouseEventParameter & event, Button * source);
        
        void mouseEntered(MouseEventParameter param);
        void mouseExited(MouseEventParameter param);
        void mousePressed(MouseEventParameter param);
        void mouseReleased(MouseEventParameter param);


    private:
        Button * scrollDown;
        Button * scrollUp;
        Container * viewPort;
    };
}

#endif
