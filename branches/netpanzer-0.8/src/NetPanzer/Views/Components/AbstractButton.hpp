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

#include "Button.hpp"
#include "MouseEvent.hpp"

//--------------------------------------------------------------------------
class AbstractButton : public Button
{
protected:
    std::string label;
    std::string actionCommand;
    std::string text;

    Surface image;

public:
    AbstractButton()
    {}
    AbstractButton(const std::string& s) : Button(s)
    {}
    virtual ~AbstractButton()
    {}

    void init(const std::string& text, const Surface* image)
    {
        this->text = text;
        this->image.copy(*image);
    }

    const std::string& getText() const
    {
        return text;
    }

    // XXX doesn't seem to be defined
    //bool isRolloverEnabled() { return isRolloverEnabled; }

    void setEnabled(bool b)
    {
        enabled = b;
    }

    virtual void actionPerformed(const mMouseEvent &me);
    virtual void draw(Surface& )
    {}
}; // end AbstractButton

#endif // end __AbstractButton_hpp__
