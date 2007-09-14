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
#include "UI/VerticalScrollBox.hpp"

namespace Panels{
    const int TestPanel::WIDTH = 440;
    const int TestPanel::HEIGHT = 270;

    TestPanel::TestPanel(iXY position, UI::FontManager * fm):Container(position, iXY(WIDTH, HEIGHT))
    {
        
        this->setName("RootPanel");
        setDrawFrame(true);
        col = 0;

        iRect labelPosition(200, 5, 300, 20);
        label1 = new UI::Label("Test", labelPosition, fm, UI::H_CENTER|UI::V_CENTER);
        addComponent(label1);

        UI::VerticalScrollBox * scroll_box = new UI::VerticalScrollBox(iRect(200, 30, 430, 130));
        addComponent(scroll_box);

        UI::BoxElement * box1 = new UI::BoxElement(scroll_box, 20);
        box1->addComponent(new UI::Label("Box1", iRect(5, 5, 45, 15), fm));
        UI::BoxElement * box2 = new UI::BoxElement(scroll_box, 20);
        box2->addComponent(new UI::Label("Box2", iRect(5, 5, 45, 15), fm));
        UI::BoxElement * box3 = new UI::BoxElement(scroll_box, 20);
        box3->addComponent(new UI::Label("Box3", iRect(5, 5, 45, 15), fm));
        UI::BoxElement * box4 = new UI::BoxElement(scroll_box, 20);
        box4->addComponent(new UI::Label("Box4", iRect(5, 5, 45, 15), fm));
        UI::BoxElement * box5 = new UI::BoxElement(scroll_box, 20);
        box5->addComponent(new UI::Label("Box5", iRect(5, 5, 45, 15), fm));
        UI::BoxElement * box6 = new UI::BoxElement(scroll_box, 20);
        box6->addComponent(new UI::Label("Box6", iRect(5, 5, 45, 15), fm));
        UI::BoxElement * box7 = new UI::BoxElement(scroll_box, 20);
        box7->addComponent(new UI::Label("Box7", iRect(5, 5, 45, 15), fm));
        UI::BoxElement * box8 = new UI::BoxElement(scroll_box, 20);
        box8->addComponent(new UI::Label("Box8", iRect(5, 5, 45, 15), fm));
        UI::BoxElement * box9 = new UI::BoxElement(scroll_box, 20);
        box9->addComponent(new UI::Label("Box9", iRect(5, 5, 45, 15), fm));
        UI::BoxElement * box10 = new UI::BoxElement(scroll_box, 20);
        box10->addComponent(new UI::Label("Box10", iRect(5, 5, 45, 15), fm));
        UI::BoxElement * box11 = new UI::BoxElement(scroll_box, 20);
        box11->addComponent(new UI::Label("Box11", iRect(5, 5, 45, 15), fm));
        UI::BoxElement * box12 = new UI::BoxElement(scroll_box, 20);
        box12->addComponent(new UI::Label("Box12", iRect(5, 5, 45, 15), fm));

        iXY sub1Position(15, 15);
        iXY sub2Position(15, 15);

        subBox1 = new Container(sub1Position, iXY(150, 230));
        subBox1->setDrawFrame(true);

        subBox2 = new Container(sub2Position, iXY(120, 200));
        subBox2->setDrawFrame(true);
        
        subBox1->setName("subBox1");
        subBox2->setName("subBox2");
        

        b1 = new UI::Button("button.png", "button_down.png", "Test1", iRect(20,20, 90, 45), fm);
        b2 = new UI::Button("button.png", "button_down.png", "Test2", iRect(20,100, 90, 125), fm);
        b1->setName("Test1");
        b2->setName("Test2");
        b1->addCallback(this);
        b2->addCallback(this);
        subBox2->addComponent(b1);
        subBox2->addComponent(b2);
        subBox1->addComponent(subBox2);
        addComponent(subBox1);

    }

    void TestPanel::draw(UI::Painter & painter)
    {

        
        Container::draw(painter);
    }

    void TestPanel::buttonPressed(UI::MouseEventParameter& , UI::Button * source)
    {
        LOG(("Button Pressed : %s", source->getName().c_str()));
        if(source == b1)
            label1->setTextColor(::Color::red);
        else if(source == b2)
            label1->setTextColor(::Color::yellow);
    }
}
