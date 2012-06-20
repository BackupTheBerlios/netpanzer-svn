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
#include "Views/Components/View.hpp"
#include "Views/Theme.hpp"

tStringListBox::tStringListBox(iRect rect, StateChangedCallback* newcallback)
{
    VScrollBar = 0;
    HScrollBar = 0;
    callback = newcallback;
    setLocation(rect.min);
    setSize(rect.getSizeX(), rect.getSizeY());
    background = ctWindowsbackground;
    StartItem = 0;
    StartWidth = 0;
    MaxItemWidth = size.x;
    SelectedItem = -1;
    MaxItemView = (size.y-5)/ItemHeight;
    AutoScroll = false;
    dirty = true;
}

void tStringListBox::actionPerformed(const mMouseEvent &me)
{
    if (me.getID() == mMouseEvent::MOUSE_EVENT_PRESSED &&
        (me.getModifiers() & InputEvent::BUTTON1_MASK)) 
    {
        int tmpSelected = (me.getY()-(position.y+3))/ItemHeight;
        if ((tmpSelected+StartItem) < (int)List.size())
        {
            SelectedItem = tmpSelected+StartItem;
            dirty = true;
        }
    }
}

int tStringListBox::getSelectedItem()
{
    return SelectedItem;
}

std::string tStringListBox::getTextItem()
{
    if (SelectedItem < 0) return "";
    else return List[SelectedItem].text;
}

int tStringListBox::getMaxItemWidth(int Index)
{
    return Surface::getTextLength(List[Index].text)+10;
}

void tStringListBox::Add(const std::string S)
{
    DataItem data;
    data.text = S;
    List.push_back(data);
    int HSize = getMaxItemWidth(List.size()-1);
    if (MaxItemWidth < HSize) MaxItemWidth = HSize;
    if (VScrollBar) VScrollBar->setMax(List.size()-MaxItemView);
    if (HScrollBar) HScrollBar->setMax(MaxItemWidth-size.x);
    if (AutoScroll && VScrollBar) 
    {
        VScrollBar->setPosition(VScrollBar->getMax());
        StartItem = VScrollBar->getPosition();
    }
    dirty = true;
}

void tStringListBox::AddData(const std::string S, void * D)
{
    DataItem data;
    data.text = S;
    data.Data = D;
    List.push_back(data);
    int HSize = getMaxItemWidth(List.size()-1);
    if (MaxItemWidth < HSize) MaxItemWidth = HSize;
    if (VScrollBar) VScrollBar->setMax(List.size()-MaxItemView);
    if (HScrollBar) HScrollBar->setMax(MaxItemWidth-size.x);
    if (AutoScroll && VScrollBar) 
    {
        VScrollBar->setPosition(VScrollBar->getMax());
        StartItem = VScrollBar->getPosition();
    }
    dirty = true;
}

void tStringListBox::render()
{
    surface.fill(background);
    surface.drawRect(surface.getRect(), ctWindowsBorder);
    int MaxItem = MaxItemView-StartItem;
    if ( (StartItem+MaxItemView) > (int)List.size()) MaxItem  = List.size();
    else MaxItem = StartItem+MaxItemView;
    int row = 5;
    Surface RowPaint(MaxItemWidth+10, ItemHeight, 1);
    Surface Bitmap(size.x, ItemHeight, 1);
    for(PlayerID i = StartItem; i < MaxItem; ++i)
    {
        RowPaint.fill(background);
        if (SelectedItem == i)
        {
            RowPaint.fill(ctTexteOver);
        }
        onPaint(RowPaint, i);
        iRect r(StartWidth, 0, StartWidth+size.x, ItemHeight);
        Bitmap.grab(RowPaint, r);
        Bitmap.blt(surface, 0, row);
        row += ItemHeight;
    }
}

void tStringListBox::onPaint(Surface &dst, int Index)
{
    dst.bltString(1 , 4, List[Index].text.c_str(), ctTexteNormal);
}

void tStringListBox::setColor(PIX newColor)
{
    background = newColor;
    dirty = true;
}

void tStringListBox::setVscrollBar(tVScrollBar *newVScrollBar)
{
    VScrollBar = newVScrollBar;
    if (VScrollBar)
    {
        VScrollBar->setLocation(position.x+size.x, position.y);
        VScrollBar->setHeight(size.y);
        VScrollBar->setStateChangedCallback(this);
        VScrollBar->setMax(List.size()-MaxItemView);
    }
}

void tStringListBox::setHscrollBar(tHScrollBar *newHScrollBar)
{
    HScrollBar = newHScrollBar;
    if (HScrollBar)
    {
        HScrollBar->setLocation(position.x, position.y+size.y);
        HScrollBar->setWidth(size.x);
        HScrollBar->setStateChangedCallback(this);
        HScrollBar->setSmallChange(3);
        HScrollBar->setMax(MaxItemWidth-size.x);
    }
}

void tStringListBox::stateChanged(Component* source)
{
    if (source == VScrollBar)
    {
        StartItem = VScrollBar->getPosition();
        dirty = true;
    }
    if (source == HScrollBar)
    {
        StartWidth = HScrollBar->getPosition();
        dirty = true;
    }
}

void tStringListBox::Delete(int Index)
{
    if (Index > (int)List.size())
    {
        List.erase(List.begin()+Index);
        dirty = true;
    }
}

int tStringListBox::IndexOf(const std::string S)
{
    for (int i = 0; i < (int)List.size(); ++i)
    {
        if (S == List[i].text) return i;
    }
    return -1;
}

void tStringListBox::setLocation(int x, int y)
{
    position.x = x;
    position.y = y;
    if (VScrollBar)
    {
        VScrollBar->setLocation(position.x+size.x, position.y);
    }
    if (HScrollBar)
    {
        HScrollBar->setLocation(position.x, position.y+size.y);
    }
}

