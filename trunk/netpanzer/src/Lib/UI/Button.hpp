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

#ifndef __UI_Button_hpp__
#define __UI_Button_hpp__

#include "Component.hpp"
#include "FontManager.hpp"

#include "Label.hpp"

#include "Util/Log.hpp"
#include <iostream>
#include <list>

namespace UI{

    class Button;
    class ButtonCallback{
    public:
        virtual void buttonPressed(MouseEventParameter & event, Button * source) = 0;
    };

    class Button : public Component{
    public:
        Button(const std::string& text, iRect area, FontManager * fm);
        void draw(Painter & painter);

        void addCallback(ButtonCallback * callback){
            callbacks.push_front(callback);
        }

        void removeCallback(ButtonCallback * callback){
            callbacks.remove(callback);
        }

    protected:
        std::string textLabel;
        SDL_Surface * textSurface;
        
        iXY textPosition;

        Color bgColor;

        std::list<ButtonCallback *> callbacks;

        void initialiseTextSurface(FontManager * fm);

        void mouseEntered(MouseEventParameter param);
        void mouseExited(MouseEventParameter param);
        void mousePressed(MouseEventParameter param);
        void mouseReleased(MouseEventParameter param);

    private:
        bool clickState;

    };
}

#endif
