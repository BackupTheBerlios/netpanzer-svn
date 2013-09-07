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

#ifndef VSCROLLBAR_HPP
#define	VSCROLLBAR_HPP

#include "Views/Components/Component.hpp"
#include "Util/NTimer.hpp"

class Button;

class VScrollBar : public Component
{
public:
    VScrollBar();
    ~VScrollBar();
    
    void setLocation(const int x, const int y);
    void setSize(const int x, const int y);
    
    int getValue() const { return value; }
    void setValue(const int v);
    void updateValue(const int v) { setValue(value+v); }
    
    void pageUp() { updateValue(-visibleHeight); }
    void pageDown() { updateValue(visibleHeight); }
    void stepUp() { updateValue(-stepSize); }
    void stepDown() { updateValue(stepSize); }
    
    
    void setTotalHeight(const int h);
    void setStepSize(const int s) { stepSize = s; }
    void setAutoRepeat(const int initial, const int other);
    
    void setChangeEvent(int e) { changeEvent = e; }
    
    void draw(Surface& s) const;
    void logic();
    void onSelectStart();
    void onSelectStop();

    void onHoverStart();
    void onHoverStop();
    
    void onPointerMove(int rel_x, int rel_y);
    
private:
    void setVisibleHeight( const int h );
    void updateMovableBarSize();
    
    
    Button       * upButton;
    Button       * downButton;

    iRect scrollArea;
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
    
    ComponentEvents myEvents;
    Component * selectingComponent;
    Component * hoveringComponent;
};

#endif	/* VSCROLLBAR_HPP */

