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

#include "Views/Components/Component.hpp"
#include "Core/CoreTypes.hpp"
#include "2D/TextRenderer.hpp"


//--------------------------------------------------------------------------
class CheckBox : public Component
{
public:
    CheckBox(const NPString& label, bool* state, const int changeEvent);

    const NPString& getLabel() const { return label; }
    
    void setLabel(const NPString& label);

    void setChangeEvent(int event) { changeEvent = event; }

protected:
    virtual void onStateChanged() {}
        
private:
    int changeEvent;

    bool selecting;
    void onSelectStart();
    void onSelectStop();
    void onHoverStart();
    void onHoverStop();

    TextRenderer label_render;
    NPString label;
    bool*   state;

    PIX textColor;
    
    void render() {}
    
    void draw(Surface& s);
    
};

#endif // end __CheckBox_hpp__
