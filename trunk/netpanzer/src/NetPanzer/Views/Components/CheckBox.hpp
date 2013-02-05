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
#include "ViewGlobals.hpp"
#include "Actions/Action.hpp"

//--------------------------------------------------------------------------
class CheckBox : public Component
{
public:
    CheckBox(const int x, const int y, const NPString& label, bool* state, Action * a = 0)
            : Component(), label(label), state(state), action(a)
    {
        setSize( 20+label.length()*8, 14);
        setLocation(x, y);
        textColor = componentActiveTextColor;
    }

    virtual ~CheckBox()
    {
        if ( action && ! action->isShared() )
        {
            delete action;
            action = 0;
        }
    }

    const std::string& getLabel() const
    {
        return label;
    }
    
    void setLabel(const std::string& label)
    {
        CheckBox::label = label;
        setSize( 20+label.length()*8, 14);
    }
    
    void setAction(Action * a)
    {
        if ( action && ! action->isShared() )
        {
            delete action;
        }
        action = a;
    }
    
private:
    std::string label;
    bool*   state;

    PIX textColor;

    Action * action;
    
    void render() {}
    
    void draw(Surface& s)
    {
        iRect r(position.x, position.y, 14, 14);
        s.FillRoundRect( r,2,  componentBodyColor);
        
        r.grow(-1,-1);
        s.RoundRect( r, 2, textColor);
        
        s.bltString(r.getLocationX() + 17, r.getLocationY() + 1, label.c_str(), textColor);
        
        if ( *state )
        {
            int x = r.getLocationX() + 2;
            int y = r.getLocationY() + 5;
            s.drawLine(x, y, x+3, y+3, textColor);
            y += 1;
            s.drawLine(x, y, x+3, y+3, textColor);
            y+=3;
            x+=3;
            s.drawLine(x, y, x+3, y-6, textColor);
            y -= 1;
            s.drawLine(x, y, x+3, y-6, textColor);
        }
    }
    
    void actionPerformed(const mMouseEvent &me)
    {
        if (    ( me.getID() == mMouseEvent::MOUSE_EVENT_CLICKED )
             && ( me.getModifiers() & InputEvent::BUTTON1_MASK) )
        {
            *state = !(*state);
            if ( action )
            {
                action->execute();
            }
        }
        else if ( me.getID() == mMouseEvent::MOUSE_EVENT_ENTERED )
        {
            textColor = componentFocusTextColor;
        }
        else if ( me.getID() == mMouseEvent::MOUSE_EVENT_EXITED )
        {
            textColor = componentActiveTextColor;
        }
    }
    
};

#endif // end __CheckBox_hpp__
