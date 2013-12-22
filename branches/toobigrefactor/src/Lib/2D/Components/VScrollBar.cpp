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
 * Created on September 7, 2013, 10:48 AM
 */

#include "VScrollBar.hpp"
#include "Button.hpp"
#include "GameInput/InputManager.hpp"
#include <algorithm>

#include "Util/Log.hpp"
#include "Resources/MImage.hpp"
#include "Resources/ResourceManager.hpp"

#define BSIZE (15)
#define REPEAT_INITIAL (500)
#define REPEAT_TIME (50)

struct Events
{
    enum
    {
        GoUp = 1,
        GoDown
    };
};

VScrollBar::VScrollBar()
{
    overScrollable = false;
    mouseScrolling = false;
    value = 0;
    visibleHeight = 0;
    totalHeight = 0;
    stepSize = 15;
    virtualTotalHeight = 0;
    movableAreaHeight = 0;
    changeEvent = 0;
    movableClickPos = 0;
    selectingComponent = 0;
    hoveringComponent = 0;

    repeat_timer.setTimeOut(0);
    repeat_scroll = 0;
    repeat_initial = 0;
    repeat_other = 0;
    
    scrollArea.setWidth(BSIZE);
    movableBar.setWidth(BSIZE);
    
//    MImageResource sbuttons = ResourceManager::getMImage("scroll-buttons");
    MImageResource sbuttons = ResourceManager::getResource<MImageResource>("new-scroll-buttons");
    
    PtrArray<Surface> piclist(3);
    
    piclist.push_back(new Surface(sbuttons.getWidth(), sbuttons.getHeight()));
    piclist.push_back(new Surface(sbuttons.getWidth(), sbuttons.getHeight()));
    piclist.push_back(new Surface(sbuttons.getWidth(), sbuttons.getHeight()));

    piclist[0]->fill(0);
    piclist[1]->fill(0);
    piclist[2]->fill(0);
    
    sbuttons.draw(*piclist[0], 0, 0, 0); // 3,2,2
    sbuttons.draw(*piclist[1], 1, 0, 0);
    sbuttons.draw(*piclist[2], 2, 0, 0);
    
    upButton = new Button();
    upButton->setImage(piclist);
//    upButton->setStateOffset(Button::BPRESSED, 1, 1);
    upButton->setComponentEvents(&myEvents);
    upButton->setClickEvent(Events::GoUp);

    piclist.push_back(new Surface(sbuttons.getWidth(), sbuttons.getHeight()));
    piclist.push_back(new Surface(sbuttons.getWidth(), sbuttons.getHeight()));
    piclist.push_back(new Surface(sbuttons.getWidth(), sbuttons.getHeight()));

    piclist[0]->fill(0);
    piclist[1]->fill(0);
    piclist[2]->fill(0);
    
    sbuttons.draw(*piclist[0], 4, 0, 0); // 1, 0, 0
    sbuttons.draw(*piclist[1], 5, 0, 0);
    sbuttons.draw(*piclist[2], 6, 0, 0);

    downButton = new Button();
    downButton->setImage(piclist);
//    downButton->setStateOffset(Button::BPRESSED, 1, 1);
    downButton->setComponentEvents(&myEvents);
    downButton->setClickEvent(Events::GoDown);
    
    rect.setWidth(BSIZE);
}

VScrollBar::~VScrollBar()
{
    delete upButton;
    delete downButton;
}

void VScrollBar::draw(Surface& s) const
{
    s.fillRect(scrollArea, Color::darkGray);
    if ( ! movableBar.isEmpty() )
    {
        s.fillRect(movableBar, overScrollable ? Color::gray128 : Color::gray96);
    }
    
    upButton->draw(s);
    downButton->draw(s);
}

void VScrollBar::setValue(const int v)
{
    const int newValue = std::max(0,std::min(virtualTotalHeight, v));
    if ( newValue != value )
    {
        value = newValue;
        if ( changeEvent )
        {
            events->push(changeEvent);
        }
    }

    if ( movableBar.getHeight() )
    {
        const int starth = (movableAreaHeight * value) / virtualTotalHeight;
        movableBar.setLocationY( scrollArea.getLocationY() + std::min( movableAreaHeight, starth) );
    }
}

void VScrollBar::setTotalHeight(const int h)
{
    if ( h != totalHeight )
    {
        totalHeight = h;
        virtualTotalHeight = std::max(0, h - visibleHeight);
        updateMovableBarSize();
        setValue(value);
    }
}

void VScrollBar::setVisibleHeight( const int h )
{
    if ( h != visibleHeight )
    {
        Component::setSize(BSIZE, h);
        visibleHeight = h;
        scrollArea.setHeight(h - (BSIZE * 2) );
        downButton->setLocation(downButton->getLocationX(), scrollArea.getEndY() + 1);
        virtualTotalHeight = std::max(0, totalHeight - h);
        updateMovableBarSize();
        setValue(value);
    }
}

void VScrollBar::setLocation(const int x, const int y)
{
    Component::setLocation(x, y);
    upButton->setLocation(x, y);
    scrollArea.setLocation(x, y + BSIZE);
    downButton->setLocation(x, scrollArea.getEndY() + 1);
    
    // @todo might want to adjust 'y' here instead of recalculating, with setValue
    movableBar.setLocationX(x);
    setValue(value);
}

void VScrollBar::setSize(const int x, const int y)
{
//    Component::setSize(BSIZE, y);
    setVisibleHeight(y);
}

void VScrollBar::updateMovableBarSize()
{
    if ( totalHeight <= visibleHeight )
    {
        movableBar.setHeight(0);
    }
    else
    {
        // min 24;
        // totalHeight here IS > 0
        const int h = (scrollArea.getHeight() * visibleHeight) / totalHeight;
        movableBar.setHeight(std::max(24, h));
        movableAreaHeight = scrollArea.getHeight() - movableBar.getHeight();
    }
}
    
void VScrollBar::logic()
{
    upButton->logic();
    downButton->logic();
    
    int event;
    while ( (event = myEvents.nextEvent()) ) switch ( event )
    {
        case Events::GoUp:   stepUp(); break;
        case Events::GoDown: stepDown(); break;
    }
    
    myEvents.reset();
    
    if ( repeat_scroll && repeat_timer.isTimeOut() )
    {
        if ( repeat_scroll > 0 )
        {
            pageDown();
        }
        else
        {
            pageUp();
        }

        repeat_timer.setTimeOut(repeat_other);
        repeat_timer.reset();

        if ( (value == 0) || (value == virtualTotalHeight) )
        {
            repeat_scroll = 0;
        }
    }

}

void VScrollBar::setAutoRepeat(const int initial, const int other)
{
    upButton->setAutoRepeat(initial, other);
    downButton->setAutoRepeat(initial, other);
    repeat_initial = initial;
    repeat_other = other;
}

void VScrollBar::onHoverStart()
{
//    const int mx = GameInput::InputManager::getMouseX();
//    const int my = GameInput::InputManager::getMouseY();
//    overScrollable = movableBar.contains(mx, my);
}

void VScrollBar::onHoverStop()
{
    overScrollable = false;
    if ( hoveringComponent )
    {
        hoveringComponent->onHoverStop();
        hoveringComponent = 0;
    }
}

void VScrollBar::onSelectStart()
{
    if ( hoveringComponent )
    {
        hoveringComponent->onSelectStart();
        selectingComponent = hoveringComponent;
    }
    else
    {
        const int my = GameInput::InputManager::getMouseY();
        if ( ! overScrollable )
        {
            if ( my < movableBar.getLocationY() )
            {
                pageUp();
                repeat_scroll = -1;
                repeat_timer.setTimeOut(repeat_initial);
                repeat_timer.reset();
            }
            else if ( my >= movableBar.getEndY() )
            {
                pageDown();
                repeat_scroll = 1;
                repeat_timer.setTimeOut(repeat_initial);
                repeat_timer.reset();
            }
        }
        else
        {
            mouseScrolling = true;
            movableClickPos = movableBar.getLocationY() - my;
        }
    }
}

void VScrollBar::onSelectStop()
{
    repeat_scroll = 0;
    mouseScrolling = false;
    if ( selectingComponent )
    {
        selectingComponent->onSelectStop();
        selectingComponent = 0;
    }
}

void VScrollBar::onPointerMove(int rel_x, int rel_y)
{
    const int mx = GameInput::InputManager::getMouseX();
    const int my = GameInput::InputManager::getMouseY();

    if ( ! selectingComponent )
    {
        overScrollable = mouseScrolling || movableBar.contains(mx, my);
        
        Component * c = 0;

        if ( upButton->contains(mx, my) )
        {
            c = upButton;
        }
        else if ( downButton->contains(mx, my) )
        {
            c = downButton;
        }

        if ( c != hoveringComponent )
        {
            if ( hoveringComponent ) hoveringComponent->onHoverStop();
            hoveringComponent = c;
            if ( hoveringComponent ) hoveringComponent->onHoverStart();
        }
        
        if ( hoveringComponent )
        {
            hoveringComponent->onPointerMove(rel_x, rel_y);
        }
        else
        {
            if ( mouseScrolling )
            {
                const int between = movableAreaHeight / virtualTotalHeight;
                const int new_value = (((my-scrollArea.getLocationY()+movableClickPos) + (between/2)) * virtualTotalHeight ) / movableAreaHeight;
                setValue(new_value);
            }
        }
    }
    else
    {
        selectingComponent->onPointerMove(rel_x, rel_y);
    }
}