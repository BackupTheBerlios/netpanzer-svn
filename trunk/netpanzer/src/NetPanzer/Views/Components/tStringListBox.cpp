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
    callback = newcallback;
    setLocation(rect.min);
    setSize(rect.getSizeX(), rect.getSizeY());
    background = ctWindowsbackground;
    VScrollBar = 0;
    StartItem = 0;
    SelectedItem = -1;
    MaxItemView = (size.y-5)/ItemHeight;
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

void tStringListBox::Add(const std::string S)
{
    DataItem data;
    data.text = S;
    List.push_back(data);
    if (VScrollBar) VScrollBar->setMax(List.size()-MaxItemView);
    dirty = true;
}

void tStringListBox::AddData(const std::string S, void * D)
{
    DataItem data;
    data.text = S;
    data.Data = D;
    List.push_back(data);
    if (VScrollBar) VScrollBar->setMax(List.size()-MaxItemView);
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
    for(PlayerID i = StartItem; i < MaxItem; ++i)
    {
        if (SelectedItem == i) 
        {
            iRect r(1, row-3, size.x, row+ItemHeight-3);
            surface.fillRect(r, ctTexteOver);
        }
        onPaint(i, row);
        row += ItemHeight;
    }
}

void tStringListBox::onPaint(int Index, int row)
{
    surface.bltString(5 , row, List[Index].text.c_str(), ctTexteNormal);
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

void tStringListBox::stateChanged(Component* source)
{
    if (source == VScrollBar)
    {
        StartItem = VScrollBar->getPosition();
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

