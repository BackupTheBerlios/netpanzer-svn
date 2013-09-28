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
 * Created on March 4, 2013, 1:06 AM
 */

#include "ModalMessageLayer.hpp"

#include "2D/Palette.hpp"
#include "2D/Color.hpp"

#include "Types/iXY.hpp"

#include "Resources/ResourceManager.hpp"

#include "Classes/ScreenSurface.hpp"

#include "2D/Components/AreaComponent.hpp"
#include "2D/Components/Label.hpp"
#include "2D/Components/Button.hpp"

#include "Interfaces/StrManager.hpp"

struct Events
{
    enum { Ok = 1 };
};

class DarkGrayPanel : public Component
{
private:
    ImageFilterResource filter;
    
public:
    DarkGrayPanel()
    {
        filter = ResourceManager::getResource<ImageFilterResource>("darkgray");
    }
    
    void draw(Surface& dest) const
    {
        filter.applyRounded(dest, rect, 10);
        dest.RoundRect(rect, 10, Color::gray);
    }
};

ModalMessageLayer::ModalMessageLayer(const char* message, ComponentEvents * parent_events, int event)
    : ComponentLayer(0)
{
    this->parent_events = parent_events;
    this->event = event;
    
    area = new AreaComponent();
    bg = new DarkGrayPanel();
    label = new Label( UString(message), Color::white);
    button = Button::createNewSpecialButton( _("Ok"), iXY(0, 0), 50);
    button->setClickEvent(Events::Ok);
    
    addComponent(bg);
    addComponent(area);
    addComponent(label);
    addComponent(button);
    
    setModal(true);
}

ModalMessageLayer::~ModalMessageLayer()
{
    
}

void ModalMessageLayer::recalculateComponentLocations()
{
    const int sw = screen->getWidth();
    const int sh = screen->getHeight();
    
    area->setSize(sw, sh);
    bg->setSize(sw, sh);
    
    label->setLocation( (sw/2) - (label->getTextWidth()/2),
                        (sh/2) - label->getTextHeight() );
    
    button->setLocation( (sw/2) - (button->getWidth()/2),
                         (sh/2) + 4);
    
}

void ModalMessageLayer::handleComponentEvents()
{
    int e;
    while ( (e = component_events.nextEvent()) ) switch ( e )
    {
        case Events::Ok:
        {
            parent_events->push(event);
        }
    }
}
