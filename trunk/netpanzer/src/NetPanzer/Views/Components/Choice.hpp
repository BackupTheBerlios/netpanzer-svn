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

#include <vector>
#include <string>

#include "Component.hpp"
#include "MouseEvent.hpp"
#include "Core/CoreTypes.hpp"

//--------------------------------------------------------------------------
class Choice : public Component
{
private:
    std::vector<std::string> choiceList;
    size_t              index;
    int                 minWidth;
    bool                isOpen;
    size_t              mouseover;
    int                 real_y;

    NPString label;
    
protected:
    virtual void onSelectionChanged() {}

public:
    Choice()
    {
        reset();
    }
            
    virtual ~Choice()
    {}

    void   addItem(const std::string& item);

    void setLocation(const int x, const int y) { real_y = y; Component::setLocation(x,y); }

    size_t getItemCount() const
    {
        return choiceList.size();
    }

    size_t getSelectedIndex() const
    {
        return index;
    }

    void   select(size_t index);
    void   setMinWidth(int minWidth);

    void   setLabel(const NPString& label) { this->label = label; }

    virtual void draw(Surface &dest);
    virtual void render()
    {
    }
    virtual void actionPerformed(const mMouseEvent &me);

private:
    void reset();
}; // end Choice

#endif // end __Choice_hpp__
