/*
Copyright (C) 2013 Netpanzer Team. (www.netpanzer.org), Aaron Perez

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
 *
 * Created on March 4, 2013, 12:53 AM
 */

#include "IntroLayer.hpp"

#include "Classes/ScreenSurface.hpp"

#include "2D/Components/Label.hpp"
#include "2D/Components/InputField.hpp"
#include "2D/Components/ScrollableList.hpp"
#include "2D/Components/AreaComponent.hpp"

#include "2D/Color.hpp"

#include "Types/UString.hpp"
#include "Interfaces/StrManager.hpp"
#include "Util/StringTokenizer.hpp"
#ifdef TEST_TABLE
#include "2D/Components/Table.hpp"
#include "2D/Components/Button.hpp"
#include "2D/TextRenderingSystem.hpp"
#include "Util/Log.hpp"
#endif

#define MENU_WIDTH  (640)
#define MENU_HEIGHT (480-26-26)

#ifndef PACKAGE_VERSION
	#define PACKAGE_VERSION "testing"
#endif

#ifdef TEST_TABLE

#include <vector>
#include <string>

struct Events
{
    enum
    {
        AddToTable = 1,
        TableSelectionChanged,
        ClearSelection
    };
};

class TestData
{
public:
    TestData(const std::string& s1, const std::string& s2, const std::string &s3)
        : s1(s1), s2(s2), s3(s3)
        {}
    std::string s1, s2, s3;
};

class TestRowPainter : public Table::RowPainter
{
public:
    TestRowPainter() : Table::RowPainter()
    {
        
    }
    
    ~TestRowPainter()
    {
        
    }
    
    void setData(const TestData& data)
    {
        c1.setText(data.s1.c_str());
        c2.setText(data.s2.c_str());
        c3.setText(data.s3.c_str());
    }
    
    void paintCell(Surface& dest, const int x, const int y, const iRect& rect, const unsigned column) const
    {
        switch ( column )
        {
            case 0: c1.drawPart(dest, x, y, rect); break;
            case 1: c2.drawPart(dest, x, y, rect); break;
            case 2: c3.drawPart(dest, x, y, rect); break;
        }
    }
            
private:
    
    TextRenderer c1, c2, c3;
};

class TestDataSource : public Table::DataSource
{
public:
    std::vector<TestData> testrows;
    TestDataSource() : Table::DataSource()
    {
        testrows.push_back(TestData("0,0","0,1","0,2"));
        testrows.push_back(TestData("1,0","1,1","1,2"));
        testrows.push_back(TestData("2,0","2,1","2,2"));
        testrows.push_back(TestData("3,0","3,1","3,2"));
        testrows.push_back(TestData("4,0","4,1","4,2"));
        testrows.push_back(TestData("5,0","4,1","4,2"));
        testrows.push_back(TestData("6,0","4,1","4,2"));
        testrows.push_back(TestData("7,0","4,1","4,2"));
        testrows.push_back(TestData("8,0","4,1","4,2"));
        testrows.push_back(TestData("9,0","4,1","4,2"));
        testrows.push_back(TestData("10,0","4,1","4,2"));
        testrows.push_back(TestData("11,0","4,1","4,2"));
        testrows.push_back(TestData("12,0","4,1","4,2"));
        testrows.push_back(TestData("13,0","4,1","4,2"));
        testrows.push_back(TestData("14,0","4,1","4,2"));
        testrows.push_back(TestData("15,0","4,1","4,2"));
        testrows.push_back(TestData("16,0","4,1","4,2"));
    }
    
    ~TestDataSource()
    {
        
    }
    
    void addRow(const std::string& s1, const std::string& s2, const std::string& s3)
    {
        testrows.push_back(TestData(s1,s2,s3));
        notifyDataChanged();
    }
    
    const TestData& getRowData(const unsigned n)
    {
        return testrows[n];
    }
    
    unsigned getRowCount() const
    {
        return testrows.size();
    }
    
    Table::RowPainter * newRowPainter() const
    {
        return new TestRowPainter();
    }
    
    unsigned getRowHeight() const
    {
        return TextRenderingSystem::line_height();
    }
    
    void configureRowPainter(Table::RowPainter * painter, const unsigned row) const
    {
        TestRowPainter * p = reinterpret_cast<TestRowPainter*>(painter);
        if ( row < testrows.size() )
        {
            p->setData(testrows[row]);
        }
    }
};

TestDataSource tds;
        
#endif

IntroLayer::IntroLayer() : ComponentLayer(0)
{
    area = new AreaComponent( MENU_WIDTH, MENU_HEIGHT );
    
    label = new Label( UString("The input field"), Color::white);
    input_field = new InputField();
    input_field->setMaxTextLength(20);
    input_field->setExcludedChars(UString("bc"));
    input_field->setSize(100, 21);
    input_field->setText(UString("Jafgjy this is a very long line lets see what happens"));

#ifndef TEST_TABLE
    intro_text = new ScrollableList(MENU_WIDTH-20, MENU_HEIGHT-150);

    char Buf[1024];    
    snprintf(Buf, sizeof(Buf), 
                _("Main Menu Message %s"),
                PACKAGE_VERSION);
    
    std::string s(Buf);
    StringTokenizer tok(s,'\n');
    
    while ( tok.hasMore() )
    {
        intro_text->addLine(UString(tok.getNextToken(true).c_str()));
    }
#else
    table = new Table(3, 1);
    table->setSize(MENU_WIDTH-20, MENU_HEIGHT-150);
    
    table->addColumn(UString("First Column"), 150);
    table->addColumn(UString("Second Column"), 175);
    table->addColumn(UString("Thirth Column"), 80);
    
    table->setChangeEvent(Events::TableSelectionChanged);
    
    tds.setTable(table);
    
    tableButton = Button::createNewSpecialButton("Add", iXY(0,0), 100);
    tableButton->setClickEvent(Events::AddToTable);
    
    clearTableButton = Button::createNewSpecialButton("Clear selection", iXY(0,0), 150);
    clearTableButton->setClickEvent(Events::ClearSelection);
    
#endif
    
    addComponent(area);
#ifndef TEST_TABLE
    addComponent(intro_text);
#else
    addComponent(table);
    addComponent(tableButton);
    addComponent(clearTableButton);
#endif
//    addComponent(label);
//    addComponent(input_field);
}

IntroLayer::~IntroLayer()
{
    
}

void IntroLayer::recalculateComponentLocations()
{
    int sw = screen->getWidth();
    int sh = screen->getHeight();
    
    area->setLocation((sw/2) - (area->getWidth()/2), ((sh/2) - (area->getHeight()/2)) );

    int x = area->getLocationX() + 10;
    int y = area->getLocationY() + 10;
#ifndef TEST_TABLE
    intro_text->setLocation(x, y);
    y += intro_text->getHeight();
#else
    table->setLocation(x, y);
    y += table->getHeight();
    tableButton->setLocation(x+250, y+50);
    clearTableButton->setLocation(x+350, y+50);
#endif
    
#ifdef TEST_INPUTFIELD
    label->setLocation(x, y+20);
    input_field->setLocation(x, y + 50);
#endif
}

void IntroLayer::handleComponentEvents()
{
#ifdef TEST_TABLE
    int event;
    while ( (event = component_events.nextEvent()) ) switch ( event )
    {
        case Events::AddToTable:
            tds.addRow("Added col 1","Added col 2","Even more");
            break;
            
        case Events::TableSelectionChanged:
        {
            int sindex = table->getSelectedIndex();
            const TestData& td = tds.getRowData(sindex);
            LOGGER.warning("Row selected %d: '%s' '%s' '%s'", sindex, td.s1.c_str(), td.s2.c_str(), td.s3.c_str());
        }
            break;
            
        case Events::ClearSelection:
        {
            table->clearSelection();
            LOGGER.warning("Unselected");
        }
            break;
            
    }
#endif
}
