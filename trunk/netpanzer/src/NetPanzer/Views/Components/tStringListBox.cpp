/*
Copyright (C) 2012 Netpanzer Team. (www.netpanzer.org), Laurent Jacques

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


#include "Views/Components/tStringListBox.hpp"
#include "Views/Components/tVScrollBar.hpp"
#include "Views/Components/tHScrollBar.hpp"
#include "Views/Components/View.hpp"
#include "Views/Theme.hpp"

#include "Util/StringTokenizer.hpp"

#include "2D/Color.hpp"

#define MAX_CHAT_LINES (2000)

tStringListBox::tStringListBox(iRect rrect)
{
    VScrollBar = 0;
    HScrollBar = 0;
    setLocation(rrect.getLocation());
    setSize(rrect.getSize());
    background_color = ctWindowsbackground;
    StartItem = List.end();
    StartSubLine = 0;
    StartWidth = 0;
    TotalLines = 0;
    TotalPosition = 0;
    MaxItemWidth = rect.getWidth();
    SelectedItem = List.end();
    AutoScroll = false;
    AutoWrap = false;
    Selectable = true;
    Bordered = true;
    dirty = true;
    hasHeader = false;
}

void tStringListBox::draw(Surface& dest) const
{
//    if ( dirty )
//    {
//        render();
//        dirty = false;
//    }
    
    surface.bltTrans(dest, rect.getLocationX(), rect.getLocationY());
}

void tStringListBox::Clear()
{
    List.clear();
    dirty = true;
    SelectedItem = List.end();
    StartItem = List.end();
    StartSubLine = 0;
    TotalLines = 0;
    TotalPosition = 0;
    MaxItemWidth = rect.getWidth();

    if (VScrollBar)
    {
        VScrollBar->setPosition(0);
        VScrollBar->setMax(0);
    }

    if (HScrollBar)
    {
        HScrollBar->setPosition(0);
        HScrollBar->setMax(MaxItemWidth);
    }
}

void tStringListBox::actionPerformed(const mMouseEvent &me)
{
    if ( ! Selectable )
    {
        return;
    }
    
    if (me.getID() == mMouseEvent::MOUSE_EVENT_PRESSED &&
        (me.getModifiers() & InputEvent::BUTTON1_MASK)) 
    {
        int contents_start = rect.getLocationY() + 4;
        if ( hasHeader )
        {
            contents_start += ItemHeight;
        }
        
        int SelectedLine = (me.getY()-(contents_start))/ItemHeight;
        
        SelectedItem = StartItem;
        int TmpSubLine = StartSubLine;
        
        SelectedItem = StartItem;
        
        while ( SelectedLine-- && SelectedItem != List.end() )
        {
            if ( ++TmpSubLine >= (*SelectedItem).num_lines )
            {
                SelectedItem++;
                TmpSubLine = 0;
            }
        }
        dirty = true;
    }
}

std::string tStringListBox::getTextItem()
{
    if ( SelectedItem == List.end() )
    {
        return "";
    }
    return (*SelectedItem).text;
}

int tStringListBox::getMaxItemWidth(const DataItem& data)
{
    return Surface::getTextLength(data.text)+10;
}

void tStringListBox::AddBlock(const std::string& S)
{
    StringTokenizer tok(S,'\n');
    
    while ( tok.hasMore() )
    {
        Add(tok.getNextToken(true));
    }
    
    dirty = true;
}

void tStringListBox::AddData(const std::string& S, void * D)
{
    int DisplaySize = rect.getWidth() - 6;
    
    DataItem data;
    data.text = S;
    data.Data = D;
    data.num_lines = getNumLines(DisplaySize, data);
    List.push_back(data);

    if ( List.size() > MAX_CHAT_LINES )
    {
        bool was_first = StartItem == List.begin();
        bool was_selected = SelectedItem == List.begin();
        
        const DataItem& d = (*List.begin());
        
        TotalLines -= d.num_lines;
        TotalPosition = std::max(0, TotalPosition - d.num_lines);
        
        deleteData(d);
        
        List.pop_front();
        
        if ( was_first )
        {
            StartItem = List.begin();
        }
        
        if ( was_selected )
        {
            SelectedItem = List.end();
        }
    }
    
    if ( StartItem == List.end() )
    {
        StartItem = List.begin();
    }
    
    TotalLines += data.num_lines;
    
    int HSize = getMaxItemWidth(data);
    
    if (MaxItemWidth < HSize)
    {
        MaxItemWidth = HSize;
    }
    
    if (VScrollBar)
    {
        VScrollBar->setMax(TotalLines-getNumVisibleLines());
    }
    
    if (HScrollBar)
    {
        HScrollBar->setMax(MaxItemWidth-rect.getWidth());
    }
    
    if (AutoScroll && VScrollBar) 
    {
        VScrollBar->setPosition(VScrollBar->getMax());
        
        int new_pos = VScrollBar->getPosition();
        
        if ( new_pos < TotalPosition )
        {
            while ( TotalPosition > new_pos )
            {
                if ( (--StartSubLine) < 0 )
                {
                    StartItem--;
                    StartSubLine = (*StartItem).num_lines - 1;
                }
                TotalPosition -= 1;
            }
        }
        else if ( new_pos > TotalPosition )
        {
            while ( TotalPosition < new_pos )
            {
                if ( (++StartSubLine) >= (*StartItem).num_lines )
                {
                    StartItem++;
                    StartSubLine = 0;
                }
                TotalPosition += 1;
            }
        }
    }
    
    dirty = true;
}

void tStringListBox::render()
{
    surface.fill(background_color);
    if ( Bordered )
    {
        surface.drawRect(surface.getRect(), ctWindowsBorder);
    }
    int row = 4;
    
    int maxw = 0;
    if ( AutoWrap )
    {
        maxw = rect.getWidth() - 6;
    }
    else
    {
        maxw = MaxItemWidth+10;
    }
    
    Surface RowPaint(maxw, ItemHeight);
    Surface Bitmap(rect.getWidth()-6, ItemHeight);
    
    if ( hasHeader )
    {
        Surface hh(rect.getWidth() - 2, ItemHeight);
        paintHeader(hh);
        hh.blt(surface, 1, 1); // full blit
        row += ItemHeight;
    }
    
    std::list<DataItem>::iterator item = StartItem;
    int subline_num = StartSubLine;
    int max_size_y = rect.getHeight() - ItemHeight;
    
    while ( (row < max_size_y) && (item != List.end()) )
    {
        RowPaint.fill(background_color);
        if ( SelectedItem == item )
        {
            RowPaint.fill(ctTexteOver);
        }
        
        onPaint(RowPaint, *item, subline_num);
        
        iRect r(StartWidth, 0, StartWidth+(rect.getWidth()-6), ItemHeight);
        Bitmap.grab(RowPaint, r);
        Bitmap.blt(surface, 3, row); // full blit
        
        row += ItemHeight;
        subline_num += 1;
        if ( subline_num >= (*item).num_lines )
        {
            subline_num = 0;
            item++;
        }
    }
}

void tStringListBox::onPaint(Surface &dst, const DataItem& data, int SubLine)
{
    dst.bltString(1 , 4, data.text.c_str(), ctTexteNormal);
}

void tStringListBox::setColor(PIX newColor)
{
    background_color = newColor;
    dirty = true;
}

void tStringListBox::setVscrollBar(tVScrollBar *newVScrollBar)
{
    VScrollBar = newVScrollBar;
    if (VScrollBar)
    {
        VScrollBar->setLocation(rect.getLocationX()+rect.getWidth(), rect.getLocationY());
        VScrollBar->setHeight(rect.getHeight());
        VScrollBar->setStateChangedCallback(this);
        VScrollBar->setMax(List.size()-getNumVisibleLines());
    }
}

void tStringListBox::setHscrollBar(tHScrollBar *newHScrollBar)
{
    HScrollBar = newHScrollBar;
    if (HScrollBar)
    {
        HScrollBar->setLocation(rect.getLocationX(), rect.getLocationY()+rect.getHeight());
        HScrollBar->setWidth(rect.getWidth());
        HScrollBar->setStateChangedCallback(this);
        HScrollBar->setSmallChange(3);
        HScrollBar->setMax(MaxItemWidth-rect.getWidth());
    }
}

void tStringListBox::stateChanged(Component* source)
{
    if (source == VScrollBar)
    {
        int new_pos = VScrollBar->getPosition();
        
        if ( new_pos < TotalPosition )
        {
            while ( TotalPosition > new_pos )
            {
                if ( (--StartSubLine) < 0 )
                {
                    StartItem--;
                    StartSubLine = (*StartItem).num_lines - 1;
                }
                TotalPosition -= 1;
            }
        }
        else if ( new_pos > TotalPosition )
        {
            while ( TotalPosition < new_pos )
            {
                if ( (++StartSubLine) >= (*StartItem).num_lines )
                {
                    StartItem++;
                    StartSubLine = 0;
                }
                TotalPosition += 1;
            }
        }

        dirty = true;
    }
    if (source == HScrollBar)
    {
        StartWidth = HScrollBar->getPosition();
        dirty = true;
    }
}

void tStringListBox::setLocation(int x, int y)
{
    Component::setLocation(x, y);
    if (VScrollBar)
    {
        VScrollBar->setLocation(rect.getEndX() + 1, rect.getLocationY());
    }
    if (HScrollBar)
    {
        HScrollBar->setLocation(rect.getLocationX(), rect.getEndY() + 1);
    }
}

void tStringListBox::setAutoWrap(bool autowrap)
{
    if ( this->AutoWrap != autowrap )
    {
        this->AutoWrap = autowrap;
        StartSubLine = 0;
        if ( autowrap )
        {
            int NumLines = 0;
            int DisplaySize = rect.getWidth() - 6;
            int c;
            
            std::list<DataItem>::iterator i = List.begin();
            while ( i != List.end() )
            {
                c = getNumLines( DisplaySize, *i);
                NumLines += c;
                (*i).num_lines = c;
                i++;
            }
            
            TotalLines = NumLines;
        }
        else
        {
            TotalLines = List.size();
        }
    }
}
