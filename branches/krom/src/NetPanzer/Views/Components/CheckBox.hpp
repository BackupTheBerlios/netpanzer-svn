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
#include "Util/Log.hpp"
#include "Core/CoreTypes.hpp"
#include "Util/MinSignal.hpp"

//--------------------------------------------------------------------------
class CheckBox : public Component
{
protected:
    std::string label;
    bool   state;

    PIX textColor;
    PIX backgroundColor;
    virtual void draw(Surface &dest);

public:
    CheckBox()
            : Component(), state(false)
    {
        setSize( 14, 14);
        textColor = Color::white;
    }

    CheckBox(const std::string& newlabel, bool newstate = false)
            : Component(), label(newlabel), state(newstate)
    {
        setSize( 14+label.length()*8, 14);
        textColor = Color::white;
        backgroundColor = Color::brown;
    }

    virtual ~CheckBox()
    {
    }

    MinSignal stateChanged;

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
        setSize( 14+label.length()*8, 14);
    }
    void setState(bool state)
    {
        CheckBox::state = state;
    }

    virtual void actionPerformed(const mMouseEvent &me);
}; // end CheckBox

#endif // end __CheckBox_hpp__
