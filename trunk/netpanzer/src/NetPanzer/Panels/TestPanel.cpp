#include <config.h>

#include "TestPanel.hpp"
#include "Types/iRect.hpp"

#include "2D/Color.hpp"

namespace Panels{
    const int TestPanel::WIDTH = 440;
    const int TestPanel::HEIGHT = 270;

    TestPanel::TestPanel(iXY position, UI::FontManager * fm):Container(position, iXY(WIDTH, HEIGHT))
    {
        

        UI::Button * b1;
        UI::Button * b2;

        iXY sub1Position(15, 15);
        iXY sub2Position(15, 15);

        subBox1 = new Container(sub1Position, iXY(150, 230));
        subBox2 = new Container(sub2Position, iXY(120, 200));
        
        subBox1->setName("subBox1");
        subBox2->setName("subBox2");
        

        b1 = new UI::Button("Test1", iRect(20,20, 60, 40), fm);
        b2 = new UI::Button("Test2", iRect(20,100, 60, 120), fm);
        subBox2->addComponent(b1);
        subBox2->addComponent(b2);
        subBox1->addComponent(subBox2);
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
}
