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

#include "2D/Color.hpp"

#include "Types/UString.hpp"

IntroLayer::IntroLayer() : ComponentLayer(0)
{
    label = new Label( UString("The input field"), Color::white);
    input_field = new InputField();
    input_field->setMaxTextLength(20);
    input_field->setExcludedChars(UString("bc"));
    input_field->setSize(100, 21);
    input_field->setText(UString("Jafgjy this is a very long line lets see what happens"));
//    input_field->setText(UString("12345"));
    
    addComponent(label);
    addComponent(input_field);
}

IntroLayer::~IntroLayer()
{
    
}

void IntroLayer::recalculateComponentLocations()
{
    int mx = screen->getWidth();
    int my = screen->getHeight();
    
    label->setLocation(mx/2, my/2);
    input_field->setLocation(mx/2, (my/2) + 30);
}

void IntroLayer::handleComponentEvents()
{
    
}
