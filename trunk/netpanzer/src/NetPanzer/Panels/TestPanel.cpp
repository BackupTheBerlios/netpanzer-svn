#include "TestPanel.hpp"
#include "Types/iRect.hpp"

namespace Panels{
    const int TestPanel::WIDTH = 220;
    const int TestPanel::HEIGHT = 270;

    TestPanel::TestPanel(iXY position):Container(position, iXY(WIDTH, HEIGHT)){
        b1 = new UI::Button("Test1", iRect(10,10, 50, 30));
        b2 = new UI::Button("Test2", iRect(10,100, 50, 120));
        addComponent(b1);
        addComponent(b2);
    }

    void TestPanel::draw(UI::Painter & painter){
        painter.setBrushColor(RGBColor(255,255,255));
        iRect t = area;
        t.max = iXY(area.max.x-1,area.max.y-1);
        painter.drawRect(t);

        Container::draw(painter);
    }
}
