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

#include "VScrollBar.hpp"

#include "ArrayUtil/PtrArray.hpp"
#include "Util/Log.hpp"

#include "GameInput/InputManager.hpp"

#include "2D/TextRenderingSystem.hpp"
#include "2D/TextRenderer.hpp"

#include <algorithm>
#include <math.h>

#define BSIZE (15)
#define REPEAT_INITIAL (500)
#define REPEAT_TIME (50)

struct Events
{
    enum
    {
        ScrollChange = 1
    };
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
    

    hoveringScrollbar = false;
    selectingScrollbar = false;
    
    scrollbar = new VScrollBar();
    scrollbar->setStepSize(TextRenderingSystem::line_height());
    scrollbar->setComponentEvents(&myEvents);
    scrollbar->setChangeEvent(Events::ScrollChange);
    scrollbar->setAutoRepeat(REPEAT_INITIAL, REPEAT_TIME);

    setSize(width, height);
}

ScrollableList::~ScrollableList()
{
    delete scrollbar;
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
    
    scrollbar->setTotalHeight(totalLinesHeight);
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
    
    scrollbar->setTotalHeight(totalLinesHeight);
}

void ScrollableList::setLocation(const int x, const int y)
{
    Component::setLocation(x, y);
    const int x_start = (rect.getEndX() - BSIZE) + 1;
    scrollbar->setLocation(x_start, y);
}

void ScrollableList::setSize(const int x, const int y)
{
    Component::setSize(x, y);
    scrollbar->setSize(BSIZE, y);
//    scrollbar->setVisibleHeight(y); // @todo shall remove this or keep?
    visibleWidth = x - BSIZE;
}

void ScrollableList::draw(Surface& s) const
{
    scrollbar->draw(s);
    
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
    scrollbar->logic();
    
    int event;
    while ( (event = myEvents.nextEvent()) ) switch ( event )
    {
        case Events::ScrollChange:
//            LOGGER.warning("Scroll pos changed: %d", scrollSlider->getValue());
            totalDisplayStart = scrollbar->getValue();
            dirty = true;
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
    if ( hoveringScrollbar )
    {
        scrollbar->onSelectStart();
        selectingScrollbar = true;
    }
}

void ScrollableList::onSelectStop()
{
    if ( selectingScrollbar )
    {
        scrollbar->onSelectStop();
        selectingScrollbar = false;
        onPointerMove( 0, 0);
    }
}

void ScrollableList::onHoverStart()
{
    // unused
}

void ScrollableList::onHoverStop()
{
    if ( (! selectingScrollbar) && hoveringScrollbar )
    {
        scrollbar->onHoverStop();
        hoveringScrollbar = false;
    }
}

void ScrollableList::onPointerMove(int rel_x, int rel_y)
{
    const int mx = GameInput::InputManager::getMouseX();
    const int my = GameInput::InputManager::getMouseY();
    
    if ( ! selectingScrollbar )
    {
        if ( scrollbar->contains(mx, my) )
        {
            if ( hoveringScrollbar )
            {
                hoveringScrollbar = true;
                scrollbar->onHoverStart();
                scrollbar->onPointerMove(rel_x, rel_y);
            }
        }
        else
        {
            if ( hoveringScrollbar )
            {
                hoveringScrollbar = false;
                scrollbar->onHoverStart();
            }
        }
    }
    else
    {
        scrollbar->onPointerMove(rel_x, rel_y);
    }
}
