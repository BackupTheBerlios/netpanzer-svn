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

#include "TestPanel.hpp"
#include "Types/iRect.hpp"

#include "2D/Color.hpp"
#include "Util/Log.hpp"
#include "UI/Label.hpp"

namespace Panels{
    const int TestPanel::WIDTH = 440;
    const int TestPanel::HEIGHT = 270;

    TestPanel::TestPanel(iXY position, UI::FontManager * fm):Container(position, iXY(WIDTH, HEIGHT))
    {
        
        this->setName("RootPanel");
        col = 0;

        iRect labelPosition(200, 15, 300, 50);
        label1 = new UI::Label("Test", labelPosition, fm, UI::H_CENTER|UI::BOTTOM);


        iXY sub1Position(15, 15);
        iXY sub2Position(15, 15);

        subBox1 = new Container(sub1Position, iXY(150, 230));
        subBox2 = new Container(sub2Position, iXY(120, 200));
        
        subBox1->setName("subBox1");
        subBox2->setName("subBox2");
        

        b1 = new UI::Button("Test1", iRect(20,20, 60, 40), fm);
        b2 = new UI::Button("Test2", iRect(20,100, 60, 120), fm);
        b1->setName("Test1");
        b2->setName("Test2");
        b1->addCallback(this);
        b2->addCallback(this);
        subBox2->addComponent(b1);
        subBox2->addComponent(b2);
        subBox1->addComponent(subBox2);
        subBox1->addComponent(label1);
        addComponent(subBox1);

    }

    void TestPanel::draw(UI::Painter & painter)
    {
//         painter.setBrushColor(Color::white);
//         iRect t = area;
//         t.max = iXY(area.max.x-1,area.max.y-1);
//         painter.drawRect(t);

//         t = subBox1->getArea() + getArea();
//         t.max = iXY(subBox1->getArea().max.x-1,subBox1->getArea().max.y-1);
//         painter.drawRect(t);

//         t = subBox2->getArea() + getArea();
//         t.max = iXY(subBox2->getArea().max.x-1,subBox2->getArea().max.y-1);
//         painter.drawRect(t);
        
        
        Container::draw(painter);
    }

    void TestPanel::buttonPressed(UI::MouseEventParameter & event, UI::Button * source){
        LOG(("Button Pressed : %s", source->getName().c_str()));
        if(source == b1)
            label1->setTextColor(::Color::red);
        else
            label1->setTextColor(::Color::yellow);
    }
}
