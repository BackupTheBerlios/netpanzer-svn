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

#ifndef __CheckBox_hpp__
#define __CheckBox_hpp__

#include "Component.hpp"
#include "MouseEvent.hpp"

class StateChangedCallback;

//--------------------------------------------------------------------------
class CheckBox : public Component
{
protected:
    std::string label;
    bool   state;
    StateChangedCallback* callback;

    virtual void drawCheckBox(Surface &dest);
    virtual void drawCheck(Surface &dest);
    virtual void drawLabel(Surface &dest);

public:
    CheckBox(StateChangedCallback* newcallback = 0)
            : Component(), state(false), callback(newcallback)
    {
        size = iXY(14, 14);
    }

    CheckBox(const std::string& newlabel, bool newstate = false)
            : Component(), label(newlabel), state(newstate), callback(0)
    {
        size = iXY(14, 14);
    }

    virtual ~CheckBox()
    { }

    const std::string& getLabel() const
    {
        return label;
    }
    bool   getState() const
    {
        return state;
    }

    void setLabel(const std::string& label)
    {
        CheckBox::label = label;
    }
    void setState(bool state)
    {
        CheckBox::state = state;
    }
    void setStateChangedCallback(StateChangedCallback* newcallback)
    {
        callback = newcallback;
    }

    virtual void draw(Surface &dest);
    virtual void actionPerformed(const mMouseEvent &me);
}; // end CheckBox

#endif // end __CheckBox_hpp__
