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

#define MENU_WIDTH  (640)
#define MENU_HEIGHT (480-26-26)

#ifndef PACKAGE_VERSION
	#define PACKAGE_VERSION "testing"
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
//    input_field->setText(UString("12345"));
    
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
    
    
    addComponent(area);
    addComponent(intro_text);
    addComponent(label);
    addComponent(input_field);
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
    
    intro_text->setLocation(x, y);
    
    y += intro_text->getHeight();
    
    label->setLocation(x, y+20);
    input_field->setLocation(x, y + 50);
}

void IntroLayer::handleComponentEvents()
{
    
}
