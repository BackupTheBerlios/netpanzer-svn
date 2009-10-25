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
#include "Types/iRect.hpp"
#include "Types/iXY.hpp"

//--------------------------------------------------------------------------
class MenuTemplateView : public SpecialButtonView
{
protected:
    virtual void loadBackgroundSurface();
    virtual void loadTitleSurface();

    void doLoadBackgroundSurface(const std::string& string);
    void doLoadTitleSurface(const std::string& string);

    float curTitleFlashTime;  // Where am I at in the flash?
    float titleFlashTimeHalf; // Time it takes for a half flash.

    void onComponentClicked(Component *c);
public:
    MenuTemplateView();
    static void freeBackgroundSurface();

    virtual void doDraw();
    virtual void doActivate();
    virtual void doDeactivate();

    void initInGameOptionButtons();
    void initPreGameOptionButtons();

    virtual void initButtons();

    static char currentMultiView[256];  // Used to keep track of where you are in the multiplayer menus.
    static char currentView[256];       // Used to keep track of which menu you are currently in.

    static bool          firstTimeInMenu;

}; // end MenuTemplateView

#endif // end __MenuTemplateView_hpp__
