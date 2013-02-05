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
#ifndef __MenuTemplateView_hpp__
#define __MenuTemplateView_hpp__

#include "Views/MainMenu/SpecialButtonView.hpp"
#include "2D/Surface.hpp"
#include "2D/PackedSurface.hpp"
#include "Types/iRect.hpp"
#include "Types/iXY.hpp"

class Button;

//--------------------------------------------------------------------------
class MenuTemplateView : public RMouseHackView
{
private:
    Button * playButton;
    
    void onDesktopResized( const iXY& oldResolution, const iXY& newResolution);
    
protected:
    virtual void loadBackgroundSurface();

    void doLoadBackgroundSurface(const std::string& string);

public:
    MenuTemplateView();

    virtual void doDraw( Surface& dest );
    virtual void doActivate();
    virtual void doDeactivate();
    virtual void processEvents();

    void initInGameOptionButtons();
    void initPreGameOptionButtons();

    virtual void init();
    
    void showPlayButton();
    void hidePlayButton();

    //static PackedSurface netPanzerLogo;
    static bool          firstTimeInMenu;
    //static PackedSurface globeSurface;
    static Surface       backgroundSurface;
}; // end MenuTemplateView

#endif // end __MenuTemplateView_hpp__
