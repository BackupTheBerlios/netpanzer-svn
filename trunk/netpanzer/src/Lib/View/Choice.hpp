/*
Copyright (C) 1998 Pyrosoft Inc. (www.pyrosoftgames.com), Matthew Bogue
 
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
#ifndef __Choice_hpp__
#define __Choice_hpp__

#include "Component.hpp"
#include "cGrowList.hpp"
#include "MouseEvent.hpp"

class StateChangedCallback;

//--------------------------------------------------------------------------
class Choice : public Component
{
private:
    cGrowList <String> choiceList;
    int                index;
    int                minWidth;
    bool               isOpen;
    int				   mouseover;
    int                adjustedY;  // The y translation value to keep the choice on the screen.
    StateChangedCallback* callback;

    enum { ChoiceItemHeight = 14 };

public:
    Choice(StateChangedCallback* newcallback = 0)
            : callback(newcallback)
    {
        reset();
    }
    virtual ~Choice()
    {}

    void   add(const String &item);
    void   addItem(const String &item);
    void   copyItems(const Choice &choice);
    int    getItemCount() const
    {
        return choiceList.getCount();
    }
    int    getSelectedIndex() const
    {
        return index;
    }
    String paramString()
    {
        return String();
    }
    void   select(const String &item);
    void   select(int index);
    void   setMinWidth(int minWidth);

    void setStateChangedCallback(StateChangedCallback* newcallback)
    {
        callback = newcallback;
    }

    virtual void draw(const Surface &dest);
    virtual void actionPerformed(const mMouseEvent &me);

private:
    void reset();
}
; // end Choice

#endif // end __Choice_hpp__
