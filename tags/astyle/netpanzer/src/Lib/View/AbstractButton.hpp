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

#ifndef __AbstractButton_hpp__
#define __AbstractButton_hpp__


#if _MSC_VER > 1000
	#pragma once
#endif


#include "Button.hpp"
#include "cTimeStamp.hpp"
#include "String.hpp"
#include "Icon.hpp"
#include "MouseEvent.hpp"


//--------------------------------------------------------------------------
class AbstractButton : public Button
{
protected:
    String label;
    String actionCommand;
    String text;

    Icon disableIcon;
    Icon selectedIcon;
    Icon pressedIcon;
    Icon rolloverIcon;
    Icon rolloverSelectedIcon;
    Icon icon;

public:
    AbstractButton()
    {}
    AbstractButton(String s) : Button(s)
    {}
    virtual ~AbstractButton()
    {}

    void init(const String &text, const Icon &icon)
    {
        AbstractButton::text = text;
        AbstractButton::icon = icon;
    }

    void  doClick()
    {}
    void  doClick(TIMESTAMP time)
    {}
    const Surface &getDisableIcon()
    {
        return disableIcon;
    }
    const Surface &getIcon()
    {
        return icon;
    }
    const Surface &getPressedIcon()
    {
        return pressedIcon;
    }
    const Surface &getRolloverIcon()
    {
        return rolloverIcon;
    }
    const Surface &getRolloverSelectedIcon()
    {
        return rolloverSelectedIcon;
    }
    const Surface &getSelectedIcon()
    {
        return selectedIcon;
    }
    const String  &getText()
    {
        return text;
    }

    // XXX doesn't seem to be defined
    //bool isRolloverEnabled() { return isRolloverEnabled; }

    void setDisabledIcon(const Icon &disabledIcon)
    {}
    void setEnabled(bool b)
    {
        enabled = b;
    }
    void setIcon(const Icon &icon)
    { }
    void setPressedIcon(const Icon &pressedIcon)
    { }
    void setRolloverEnabled(bool b)
    { }
    void setRolloverIcon(const Icon &rolloverIcon)
    { }
    void setSelected(bool b)
    {}
    void setSelectedIcon(Icon selectedIcon)
    {}
    void setText(String text)
    {}

    virtual void actionPerformed(const mMouseEvent &me);
    virtual void draw(const Surface &dest)
    {}

}
; // end AbstractButton

#endif // end __AbstractButton_hpp__
