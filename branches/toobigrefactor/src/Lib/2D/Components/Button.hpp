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

#ifndef __Button_hpp__
#define __Button_hpp__

#include "Views/Components/Component.hpp"
#include "2D/Surface.hpp"
#include "2D/Color.hpp"
#include "2D/TextRenderer.hpp"
#include "Views/Components/ViewGlobals.hpp"
#include "ArrayUtil/PtrArray.hpp"
#include "Core/CoreTypes.hpp"
#include "Types/iXY.hpp"
#include "Util/NTimer.hpp"

//--------------------------------------------------------------------------
class Button : public Component
{
public:
    typedef enum
    {
        BNORMAL =   0,
        BOVER   =   1,
        BPRESSED=   2,
        BDISABLED=  3,
        BMAX_STATE
    } ButtonState;
    
private:
    int extraBorder;
    int clickEvent;
    TextRenderer label_render;
    bool selecting;
    Surface surface;
    
    NTimer repeat_timer;
    int repeat_initial;
    int repeat_other;
    
protected:
    void onSelectStart();
    void onSelectStop();
    void onHoverStart();
    void onHoverStop();

    std::string label;
       
    PIX textColors[BMAX_STATE];
    iXY state_offset[BMAX_STATE];
   
    PtrArray<Surface> background_images;
   
    void render();
    ButtonState bstate;
    ButtonState painted_bstate;
   
    void resetState()
    {
        bstate = BNORMAL;
        painted_bstate = BMAX_STATE;
    }

public:
    PIX borders[BMAX_STATE];
    
    Button();

    virtual ~Button();

    void setAutoRepeat(const int initial, const int other) { repeat_initial = initial; repeat_other = other; }
    
    void setClickEvent(int event) { clickEvent = event; }
    
    virtual void draw(Surface& dest) const;
    virtual void logic();

    static Button * createTextButton(   const NPString& label,
                                        const iXY& loc,
                                        const int bwidth);

    static Button * createNewSpecialButton(     const NPString& label,
                                                const iXY& loc,
                                                int width);
    
    void setTextColors( PIX normal, PIX over, PIX pressed, PIX disabled);
    void setLabel(const NPString& l);
    const std::string& getLabel() const { return label; }
    unsigned getLabelWidth() const { return label_render.getWidth(); }
    void setImage( PtrArray<Surface>& sl );
    
    void enable();
    void disable();
    bool isEnabled() const { return bstate != BDISABLED; }
    
    void setExtraBorder();
    void clearExtraBorder();
    void setUnitSelectionBorder();   
    void setNormalBorder();
    void setRedGreenBorder();
    void clearBorder();

    void setStateOffset(ButtonState state, int x, int y);
       
    void setSize(const int x, const int y);
    void setTextButtonSize(int xsize);

}; // end Button

#endif // end __Button_hpp__
