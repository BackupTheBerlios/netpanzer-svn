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


#include "Types/iRect.hpp"
#include "Views/Components/Button.hpp"

#include "Views/MainMenu/MenuTemplateView.hpp"
#include "2D/Palette.hpp"
#include "Views/Components/Desktop.hpp"
#include "Interfaces/GameConfig.hpp"
#include "Interfaces/GameManager.hpp"
#include "System/Sound.hpp"
#include "Views/Components/ViewGlobals.hpp"
#include "Classes/ScreenSurface.hpp"
#include "Particles/Particle2D.hpp"
#include "Particles/ParticleSystem2D.hpp"
#include "2D/Surface.hpp"
#include "Interfaces/GameManager.hpp"
#include "Util/Exception.hpp"
#include "Views/GameViewGlobals.hpp"

Surface backgroundSurface;
Surface titlePackedSurface;

#define BTN_MAIN            "MenuTemplateView.Main"
#define BTN_MULTI           "MenuTemplateView.Multiplayer"
#define BTN_OPTIONS         "MenuTemplateView.Options"
#define BTN_HELP            "MenuTemplateView.Help"
#define BTN_EXIT            "MenuTemplateView.Exit"
#define BTN_INGAMEEXIT      "MenuTemplateView.InGamExit"
#define BTN_RESIGN          "MenuTemplateView.Resign"
#define BTN_CLOSEOPTIONS    "MenuTemplateView.CloseOptions"

char MenuTemplateView::currentMultiView[] = "GetSessionView";
char MenuTemplateView::currentView[]      = "";

// MenuTemplateView
//---------------------------------------------------------------------------
MenuTemplateView::MenuTemplateView() : SpecialButtonView()
{
    setSearchName("MenuTemplateView");
    setTitle("MenuTemplate");
    setSubTitle("");

    setVisible(false);
    setAllowMove(false);
    setDisplayStatusBar(false);
    setBordered(false);
    setAlwaysOnBottom(true);

    moveTo(iXY(0, 0));
    resize(iXY(640, 480));

    initButtons();

    curTitleFlashTime  = 0.0f;
    titleFlashTimeHalf = 0.5;
} // end MenuTemplateView constructor

//---------------------------------------------------------------------------
// initPreGameOptionButtons
void MenuTemplateView::initPreGameOptionButtons()
{
    add( Button::createSpecialButton( BTN_MAIN, "Main", mainPos) );
    add( Button::createSpecialButton( BTN_MULTI, "Multiplayer", multiPos) );
    add( Button::createSpecialButton( BTN_OPTIONS, "Options", optionsPos) );
    add( Button::createSpecialButton( BTN_HELP, "Help", helpPos) );
    add( Button::createSpecialButton( BTN_EXIT, "Exit netPanzer", exitPos) );
} // end MenuTemplateView::initPreGameOptionButtons

// initInGameOptionButtons
//---------------------------------------------------------------------------
void MenuTemplateView::initInGameOptionButtons()
{
    if(!gameconfig->quickConnect)
    {
        add( Button::createSpecialButton( BTN_INGAMEEXIT, "Exit netPanzer", exitPos) );
        add( Button::createSpecialButton( BTN_RESIGN, "Resign", resignPos) );
    }
    else
    {
        add( Button::createSpecialButton( BTN_RESIGN, "Resign", exitPos) );
    }
    add( Button::createSpecialButton( BTN_CLOSEOPTIONS, "Close Options", returnToGamePos) );
} // end MenuTemplateView::initInGameOptionButtons

// initButtons
//---------------------------------------------------------------------------
void MenuTemplateView::freeBackgroundSurface()
{
	backgroundSurface.freeFrames();
}

void MenuTemplateView::initButtons()
{
    if (Desktop::getVisible("GameView"))
    {
        initInGameOptionButtons();
    }
    else
    {
        initPreGameOptionButtons();
    }
} // end MenuTemplateView::initButtons

// doDraw
//---------------------------------------------------------------------------
void MenuTemplateView::doDraw()
{
    //setWorldRect();
    if (Desktop::getVisible("GameView")) {
	// When ingame, tint the game into gray
        //r.translate(min);
        //clientArea.drawWindowsBorder();
        drawViewBackground();
        drawRect(iRect(0,0,clientRect.getSizeX(), clientRect.getSizeY()), Color::gray64);
        
    } else {
	// When in mainmenu, make background dark and draw menu image
        if(screen->getWidth() > 640 ||
           screen->getHeight() > 480)
            screen->fill(Color::black);
        
        // Set the following to get does exist.
        if (backgroundSurface.getNumFrames() > 0) {
            drawImage( backgroundSurface, 0, 0);
        } else {
            throw Exception("Where is the background surface?");
        }

        titlePackedSurface.blt(*screen, min.x + 40, min.y + 390);
//        titlePackedSurface.bltBlend(*screen, min.x + 40, min.y+390, Palette::colorTable6040);
    }

    View::doDraw();
} // end doDraw

// doActivate
//---------------------------------------------------------------------------
void MenuTemplateView::doActivate()
{
    // Make the activating view active, redo this please!
    sprintf(currentView, searchName);
    Desktop::setActiveView(searchName);

    loadBackgroundSurface();
    loadTitleSurface();

    SpecialButtonView::doActivate();
} // end doActivate

// loadBackgroundSurface
//---------------------------------------------------------------------------
void MenuTemplateView::loadBackgroundSurface()
{
    doLoadBackgroundSurface("pics/default/defaultMB.png");
} // end MenuTemplateView::loadBackgroundSurface

// doLoadBackgroundSurface
//---------------------------------------------------------------------------
void MenuTemplateView::doLoadBackgroundSurface(const std::string& string)
{
    backgroundSurface.loadPNG(string.c_str());
} // end MenuTemplateView::doLoadBackgroundSurface

// loadTitleSurface
//---------------------------------------------------------------------------
void MenuTemplateView::loadTitleSurface()
{
    doLoadTitleSurface("mainTitle");
} // end MenuTemplateView::loadTitleSurface

// doLoadBackgroundSurface
//---------------------------------------------------------------------------
void MenuTemplateView::doLoadTitleSurface(const std::string& string)
{
    curTitleFlashTime  = 0.0f;
    titleFlashTimeHalf = 2.5;

    std::string pakString = "pics/default/";
    pakString += string;
    pakString += ".png";

    titlePackedSurface.loadPNG(pakString.c_str());
    titlePackedSurface.setColorkey();
} // end MenuTemplateView::doLoadTitleSurface

// doDeactivate
//---------------------------------------------------------------------------
void MenuTemplateView::doDeactivate()
{
} // end doDeactivate

void
MenuTemplateView::onComponentClicked(Component* c)
{
    string cname = c->getName();
    if ( !cname.compare("Button." BTN_MAIN) )
    {
        Desktop::setVisibilityAllWindows(false);
        Desktop::setVisibility("MainView", true);
    }
    else if ( !cname.compare("Button." BTN_MULTI) )
    {
        if (strcmp(currentMultiView, "GetSessionView") == 0)
        {
            Desktop::setVisibilityAllWindows(false);
            Desktop::setVisibility("GetSessionView", true);
        }
        else if (strcmp(currentMultiView, "HostView") == 0)
        {
            Desktop::setVisibilityAllWindows(false);
            Desktop::setVisibility("HostView", true);
            Desktop::setVisibility("FlagSelectionView", true);
            Desktop::setVisibility("HostOptionsView", true);
            Desktop::setVisibility("MapSelectionView", true);
            Desktop::setVisibility("PlayerNameView", true);
        }
        else if (strcmp(currentMultiView, "JoinView") == 0)
        {
            Desktop::setVisibilityAllWindows(false);
            Desktop::setVisibility("JoinView", true);
            Desktop::setVisibility("FlagSelectionView", true);
            Desktop::setVisibility("PlayerNameView", true);
            Desktop::setVisibility("IPAddressView", true);
            Desktop::setVisibility("ServerListView", true);
        }
    }
    else if ( !cname.compare("Button." BTN_OPTIONS) )
    {
        Desktop::setVisibilityAllWindows(false);
        Desktop::setVisibility("OptionsView", true);
    }
    else if ( !cname.compare("Button." BTN_HELP) )
    {
        Desktop::setVisibilityAllWindows(false);
        Desktop::setVisibility("HelpView", true);
    }
    else if ( !cname.compare("Button." BTN_EXIT) )
    {
        GameManager::exitNetPanzer();
    }
    else if ( !cname.compare("Button." BTN_RESIGN) )
    {
        Desktop::setVisibility("ControlsView", false);
        Desktop::setVisibility("VisualsView", false);
        Desktop::setVisibility("InterfaceView", false);
        Desktop::setVisibility("SoundView", false);
        Desktop::setVisibility("OptionsView", false);

        Desktop::setVisibility("AreYouSureResignView", true);
    }
    else if ( !cname.compare("Button." BTN_INGAMEEXIT) )
    {
        Desktop::setVisibility("ControlsView", false);
        Desktop::setVisibility("VisualsView", false);
        Desktop::setVisibility("InterfaceView", false);
        Desktop::setVisibility("SoundView", false);
        Desktop::setVisibility("OptionsView", false);

        Desktop::setVisibility("AreYouSureExitView", true);
    }
    else if ( !cname.compare("Button." BTN_CLOSEOPTIONS) )
    {
        GameManager::setNetPanzerGameOptions();

        Desktop::setVisibility("ControlsView", false);
        Desktop::setVisibility("VisualsView", false);
        Desktop::setVisibility("InterfaceView", false);
        Desktop::setVisibility("SoundView", false);
        Desktop::setVisibility("OptionsView", false);
    }
}
