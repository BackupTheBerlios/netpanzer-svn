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
 * Created on August 29, 2013, 10:18 PM
 */

#include "ScrollableList.hpp"

#include "Button.hpp"

#include "ArrayUtil/PtrArray.hpp"
#include "Util/Log.hpp"

#include "GameInput/InputManager.hpp"

#include "2D/TextRenderingSystem.hpp"

#include <algorithm>
#include <math.h>

#define BSIZE (15)
#define REPEAT_INITIAL (500)
#define REPEAT_TIME (50)

struct Events
{
    enum
    {
        ScrollChange = 1,
        GoUp,
        GoDown
    };
};

class ScrollSlider : public Component
{
public:
    ScrollSlider()
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
        
        repeat_timer.setTimeOut(0);
        repeat_scroll = 0;
        repeat_initial = 0;
        repeat_other = 0;
    }
    
    ~ScrollSlider() {}
    
    void draw(Surface& s) const
    {
        s.fillRect(rect, Color::darkGray);
        if ( ! movableBar.isEmpty() )
        {
            s.fillRect(movableBar, overScrollable ? Color::gray128 : Color::gray96);
        }
    }
    
    int getValue() const { return value; }
    
    void setValue(const int v)
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
            movableBar.setLocationY( rect.getLocationY() + std::min( movableAreaHeight-1, starth) );
        }
    }
    
    void updateValue(const int v)
    {
        setValue( std::max(0, value + v) );
    }
    
    void setTotalHeight(const int h)
    {
        if ( h != totalHeight )
        {
            totalHeight = h;
            virtualTotalHeight = std::max(0, h - visibleHeight);
            updateMovableBarSize();
            setValue(value);
        }
    }
    
    void setLocation(const int x, const int y)
    {
        Component::setLocation(x, y);
        movableBar.setLocationX(x);
        setValue(value);
    }
    
    void setSize(const int x, const int y)
    {
        Component::setSize(x, y);
        movableBar.setWidth(x);
        updateMovableBarSize();
        setValue(value);
    }

    void setVisibleHeight( const int h )
    {
        if ( h != visibleHeight )
        {
            visibleHeight = h;
            virtualTotalHeight = std::max(0, totalHeight - h);
            updateMovableBarSize();
            setValue(value);
        }
    }
    
    void setStepSize(const int s)
    {
        stepSize = s;
    }
    
    void pageUp()
    {
        updateValue(-visibleHeight);
    }
    
    void pageDown()
    {
        updateValue(visibleHeight);
    }
    
    void stepUp()
    {
        updateValue(-stepSize);
    }
    
    void stepDown()
    {
        updateValue(stepSize);
    }
    
    void onHoverStart()
    {
        const int mx = GameInput::InputManager::getMouseX();
        const int my = GameInput::InputManager::getMouseY();
        overScrollable = movableBar.contains(mx, my);
    }
    
    void onHoverStop()
    {
        overScrollable = false;
    }
    
    void onSelectStart()
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
    
    void onSelectStop()
    {
        repeat_scroll = 0;
        mouseScrolling = false;
    }

    void onPointerMove(int rel_x, int rel_y)
    {
        const int mx = GameInput::InputManager::getMouseX();
        const int my = GameInput::InputManager::getMouseY();
        overScrollable = mouseScrolling || movableBar.contains(mx, my);
        
        if ( mouseScrolling )
        {
            int between = movableAreaHeight / virtualTotalHeight;
            int new_value = (((my-getLocationY()+movableClickPos) + (between/2)) * virtualTotalHeight ) / movableAreaHeight;
            
            setValue(new_value);
        }
    }
    
    void updateMovableBarSize()
    {
        if ( totalHeight <= visibleHeight )
        {
            movableBar.setHeight(0);
        }
        else
        {
            // min 24;
            // totalHeight here IS > 0
            const int h = (getHeight() * visibleHeight) / totalHeight;
            movableBar.setHeight(std::max(24, h));
            movableAreaHeight = getHeight() - movableBar.getHeight();
        }
    }
    
    void setChangeEvent(int e)
    {
        changeEvent = e;
    }
    
    void logic()
    {
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
    
    void setAutoRepeat(const int initial, const int other)
    {
        repeat_initial = initial;
        repeat_other = other;
    }
    
private:
    iRect movableBar;
    bool overScrollable;
    bool mouseScrolling;
    int movableClickPos;
    
    int changeEvent;
    int value;
    int totalHeight;
    int visibleHeight;
    int virtualTotalHeight;
    int movableAreaHeight;
    int stepSize;

    NTimer repeat_timer;
    int repeat_scroll;
    int repeat_initial;
    int repeat_other;
    
    // scrollbar height is component height

};

ScrollableList::ScrollableList(const int width, const int height)
{
    firstVisibleLineIterator = lines.end();
    totalLinesHeight = 0;
    totalDisplayStart = 0;
    rendedTotalDisplayStart = 0; // or -1?
    maxLines = 1000;
    visibleWidth = 0;
    drawRenderOffset = 0;
    dirty = false;

    hoveringComponent = 0;
    selectingComponent = 0;
    
    Surface pics;
    pics.loadBMP("pics/buttons/scrollbutton.bmp");
    
    PtrArray<Surface> piclist(3);

    piclist.fillNew();
    piclist[0]->grab(pics, iRect(BSIZE*3, 0, BSIZE, BSIZE));
    piclist[1]->grab(pics, iRect(BSIZE*2, 0, BSIZE, BSIZE));
    piclist[2]->grab(pics, iRect(BSIZE*2, 0, BSIZE, BSIZE));
    
    upButton = new Button();
    upButton->setImage(piclist);
    upButton->setStateOffset(Button::BPRESSED, 1, 1);
    upButton->setComponentEvents(&myEvents);
    upButton->setClickEvent(Events::GoUp);
    upButton->setAutoRepeat(REPEAT_INITIAL, REPEAT_TIME);

    piclist.fillNew();
    piclist[0]->grab(pics, iRect(BSIZE, 0, BSIZE, BSIZE));
    piclist[1]->grab(pics, iRect(    0, 0, BSIZE, BSIZE));
    piclist[2]->grab(pics, iRect(    0, 0, BSIZE, BSIZE));
    
    downButton = new Button();
    downButton->setImage(piclist);
    downButton->setStateOffset(Button::BPRESSED, 1, 1);
    downButton->setComponentEvents(&myEvents);
    downButton->setClickEvent(Events::GoDown);
    downButton->setAutoRepeat(REPEAT_INITIAL, REPEAT_TIME);
    
    scrollSlider = new ScrollSlider();

    setSize(width, height);
    
    scrollSlider->setStepSize(TextRenderingSystem::line_height());
    scrollSlider->setComponentEvents(&myEvents);
    scrollSlider->setChangeEvent(Events::ScrollChange);
    scrollSlider->setAutoRepeat(REPEAT_INITIAL, REPEAT_TIME);
}

ScrollableList::~ScrollableList()
{
    delete upButton;
    delete downButton;
    delete scrollSlider;
}

void ScrollableList::setMaxLines(const unsigned n)
{
    bool iter_removed = false;
    maxLines = n;
    while ( lines.size() > maxLines )
    {
        totalLinesHeight -= lines.front().height;
        if ( firstVisibleLineIterator == lines.begin() )
        {
            iter_removed = true;
        }
        lines.pop_front();
    }
    
    if ( iter_removed )
    {
        firstVisibleLineIterator = lines.begin();
    }
    
    scrollSlider->setTotalHeight(totalLinesHeight);
}

void ScrollableList::addLine(const UString& str, const int firstLineOffset, const uint8_t color)
{
    bool iter_removed = false;
    if ( lines.size() >= maxLines )
    {
        totalLinesHeight -= lines.front().height;
        iter_removed = firstVisibleLineIterator == lines.begin();
        lines.pop_front();
    }
    
    const int vh = TextRenderingSystem::getMultilineStringHeight(str, visibleWidth);
    lines.push_back(ListElement(str, vh, firstLineOffset, color));
    totalLinesHeight += vh;
    
    if ( iter_removed || (firstVisibleLineIterator == lines.end()) )
    {
        firstVisibleLineIterator = lines.begin();
        dirty = true;
    }
    
    scrollSlider->setTotalHeight(totalLinesHeight);
}

void ScrollableList::setLocation(const int x, const int y)
{
    Component::setLocation(x, y);
    const int x_start = rect.getEndX() - BSIZE;
    const int y_bottom = rect.getEndY() - BSIZE;
    upButton->setLocation(x_start, y);
    downButton->setLocation(x_start,y_bottom);
    scrollSlider->setLocation(x_start, y+BSIZE);
}

void ScrollableList::setSize(const int x, const int y)
{
    Component::setSize(x, y);
    scrollSlider->setSize(BSIZE, y - BSIZE*2);
    scrollSlider->setVisibleHeight(y);
    visibleWidth = x - BSIZE;
}

void ScrollableList::draw(Surface& s) const
{
    scrollSlider->draw(s);
    upButton->draw(s);
    downButton->draw(s);
    
    if ( ! visibleRenders.empty() )
    {
        std::list<ListElement>::iterator i = firstVisibleLineIterator;
        iRect r(0, drawRenderOffset, visibleWidth, visibleRenders[0]->getHeight() - drawRenderOffset);
        int x = getLocationX();
        int y = getLocationY();
        
        visibleRenders[0]->drawPart(s, x, y, r, i->color);
        
        i++;
        y += visibleRenders[0]->getHeight() - drawRenderOffset;
        r.setLocationY(0);
        
        for ( unsigned n = 1; n < visibleRenders.size(); n++ )
        {
            r.setHeight(rect.getEndY() - y);
            visibleRenders[n]->drawPart(s, x, y, r, i->color);
            y += visibleRenders[n]->getHeight();
            i++;
        }
    }
}

void ScrollableList::logic()
{
    scrollSlider->logic();
    upButton->logic();
    downButton->logic();
    
    int event;
    while ( (event = myEvents.nextEvent()) ) switch ( event )
    {
        case Events::ScrollChange:
//            LOGGER.warning("Scroll pos changed: %d", scrollSlider->getValue());
            totalDisplayStart = scrollSlider->getValue();
            dirty = true;
            break;
            
        case Events::GoUp:
            scrollSlider->stepUp();
            break;
            
        case Events::GoDown:
            scrollSlider->stepDown();
            break;
            
    }
    
    myEvents.reset();
    
    // @todo optimize the rendering, it is possible that doesn't need to re-render all
    if ( dirty )
    {
        visibleRenders.deleteAll();
        if ( lines.size() )
        {
            if ( rendedTotalDisplayStart < totalDisplayStart )
            {
                // note: in theory all data is correct and iterator++ cannot fail.
                while ( (rendedTotalDisplayStart + firstVisibleLineIterator->height - 1) < totalDisplayStart )
                {
                    rendedTotalDisplayStart += firstVisibleLineIterator->height;
                    firstVisibleLineIterator++;
                }
            }
            else if ( rendedTotalDisplayStart > totalDisplayStart )
            {
                // note: in theory all data is correct and iterator++ cannot fail.
                do
                {
                    firstVisibleLineIterator--;
                    rendedTotalDisplayStart -= firstVisibleLineIterator->height;
                } while ( rendedTotalDisplayStart > totalDisplayStart );
            }
            
            TextRenderer * tr;
            std::list<ListElement>::iterator i = firstVisibleLineIterator;
            int cur_h = rendedTotalDisplayStart;
            const int end_h = totalDisplayStart + getHeight();
            
            do
            {
                tr = new TextRenderer(i->str, visibleWidth, i->firstLineOffset);
                visibleRenders.push_back(tr);
                cur_h += tr->getHeight();
                i++;
            } while ( i != lines.end() && (cur_h < end_h) );
            
            drawRenderOffset = totalDisplayStart - rendedTotalDisplayStart;
            
            dirty = false;
        }
        
    }
}

void ScrollableList::onSelectStart()
{
    if ( hoveringComponent )
    {
        hoveringComponent->onSelectStart();
        selectingComponent = hoveringComponent;
    }
}

void ScrollableList::onSelectStop()
{
    if ( selectingComponent )
    {
        selectingComponent->onSelectStop();
        selectingComponent = 0;
        onPointerMove( 0, 0);
    }
}

void ScrollableList::onHoverStart()
{
    // unused
}

void ScrollableList::onHoverStop()
{
    if ( ! selectingComponent && hoveringComponent )
    {
        hoveringComponent->onHoverStop();
        hoveringComponent = 0;
    }
}

void ScrollableList::onPointerMove(int rel_x, int rel_y)
{
    const int mx = GameInput::InputManager::getMouseX();
    const int my = GameInput::InputManager::getMouseY();
    
    if ( ! selectingComponent )
    {
        Component * c = 0;

        if ( upButton->contains(mx, my) )
        {
            c = upButton;
        }
        else if ( downButton->contains(mx, my) )
        {
            c = downButton;
        }
        else if ( scrollSlider->contains(mx, my) )
        {
            c = scrollSlider;
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
    }
    else
    {
        selectingComponent->onPointerMove(rel_x, rel_y);
    }
    
}
