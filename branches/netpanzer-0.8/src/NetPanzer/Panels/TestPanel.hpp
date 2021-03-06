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


    /**************************************/
    /* A Test panel for the new ui system */
    /**************************************/

#ifndef __TestPanel_hpp__
#define __TestPanel_hpp__

#include "UI/Container.hpp"
#include "UI/Button.hpp"
#include "UI/FontManager.hpp"

namespace Panels{

    class TestPanel : public UI::Container, public UI::ButtonCallback{

    public:
        TestPanel(iXY position, UI::FontManager * fm);
        void draw(UI::Painter & painter); //draw a white border for debug only

        void buttonPressed(UI::MouseEventParameter & event, UI::Button * source);

    private:
        static const int WIDTH;
        static const int HEIGHT;
        UI::Container * subBox1;
        UI::Container * subBox2;
        UI::Label * label1;
        UI::Button * b1;
        UI::Button * b2;
        int col;
        
    };

}

#endif
