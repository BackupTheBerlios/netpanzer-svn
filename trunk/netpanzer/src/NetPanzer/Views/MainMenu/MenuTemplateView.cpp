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
#include <config.h>

#include "MenuTemplateView.hpp"
#include "Palette.hpp"
#include "Desktop.hpp"
#include "GameConfig.hpp"
#include "GameManager.hpp"
#include "Sound.hpp"
#include "cMouse.hpp"
#include "ViewGlobals.hpp"
#include "RadarPingParticle2D.hpp"
#include "ScreenSurface.hpp"
#include "Particle2D.hpp"
#include "ParticleSystem2D.hpp"
#include "PackedSurface.hpp"
#include "UtilInterface.hpp"
#include "GameManager.hpp"
#include "Exception.hpp"
#include "GameViewGlobals.hpp"

Surface       MenuTemplateView::backgroundSurface;
PackedSurface MenuTemplateView::titlePackedSurface;

//PackedSurface MenuTemplateView::netPanzerLogo;

char MenuTemplateView::currentMultiView[] = "GetSessionView";
char MenuTemplateView::currentView[]      = "";

static void bMain()
{
    Desktop::setVisibilityAllWindows(false);
    Desktop::setVisibility("MainView", true);
}

static void bMulti()
{
    if (strcmp(MenuTemplateView::currentMultiView, "GetSessionView") == 0) {
        Desktop::setVisibilityAllWindows(false);
        Desktop::setVisibility("GetSessionView", true);
    } else if (strcmp(MenuTemplateView::currentMultiView, "HostView") == 0) {
        Desktop::setVisibilityAllWindows(false);
        Desktop::setVisibility("HostView", true);
        Desktop::setVisibility("UnitSelectionView", true);
        Desktop::setVisibility("FlagSelectionView", true);
        Desktop::setVisibility("HostOptionsView", true);
        Desktop::setVisibility("MapSelectionView", true);
        Desktop::setVisibility("PlayerNameView", true);
    } else if (strcmp(MenuTemplateView::currentMultiView, "JoinView") == 0) {
        printf("EnteringJoinView.\n");
        Desktop::setVisibilityAllWindows(false);
        // XXX
        Desktop::setVisibility("JoinView", true);
        //Desktop::setVisibility("GetSessionHostView", true);
        //Desktop::setVisibility("UnitSelectionView", true);
        Desktop::setVisibility("FlagSelectionView", true);
        Desktop::setVisibility("PlayerNameView", true);
        Desktop::setVisibility("IPAddressView", true);
    } else {
        assert(false);
    }
}

static void bOptions()
{
    Desktop::setVisibilityAllWindows(false);
    Desktop::setVisibility("OptionsView", true);
}

static void bHelp()
{
    Desktop::setVisibilityAllWindows(false);
    Desktop::setVisibility("HelpView", true);
}

static void bExit()
{
    GameManager::exitNetPanzer();
}

//---------------------------------------------------------------------------
static void bResign()
{
    Desktop::setVisibility("ControlsView", false);
    Desktop::setVisibility("VisualsView", false);
    Desktop::setVisibility("InterfaceView", false);
    Desktop::setVisibility("SoundView", false);
    Desktop::setVisibility("OptionsView", false);

    Desktop::setVisibility("AreYouSureResignView", true);
}

//---------------------------------------------------------------------------
static void bCloseOptions()
{
    GameManager::setNetPanzerGameOptions();

    Desktop::setVisibility("ControlsView", false);
    Desktop::setVisibility("VisualsView", false);
    Desktop::setVisibility("InterfaceView", false);
    Desktop::setVisibility("SoundView", false);
    Desktop::setVisibility("OptionsView", false);
}

//---------------------------------------------------------------------------
static void bExitNetPanzer()
{
    Desktop::setVisibility("ControlsView", false);
    Desktop::setVisibility("VisualsView", false);
    Desktop::setVisibility("InterfaceView", false);
    Desktop::setVisibility("SoundView", false);
    Desktop::setVisibility("OptionsView", false);

    Desktop::setVisibility("AreYouSureExitView", true);
}

// MenuTemplateView
//---------------------------------------------------------------------------
MenuTemplateView::MenuTemplateView() : SpecialButtonView()
{
    setSearchName("MenuTemplateView");
    setTitle("MenuTemplate");
    setSubTitle("");

    setAllowResize(false);
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
    addSpecialButton(	mainPos,
                      "Main",
                      bMain);

    addSpecialButton(	multiPos,
                      "Multiplayer",
                      bMulti);

    addSpecialButton(	optionsPos,
                      "Options",
                      bOptions);

    addSpecialButton(	helpPos,
                      "Help",
                      bHelp);

    addSpecialButton( exitPos,
                      "Exit netPanzer",
                      bExit);

} // end MenuTemplateView::initPreGameOptionButtons

// initInGameOptionButtons
//---------------------------------------------------------------------------
void MenuTemplateView::initInGameOptionButtons()
{
    addSpecialButton(	resignPos,
                      "Resign",
                      bResign);

    addSpecialButton(	returnToGamePos,
                      "Close Options",
                      bCloseOptions);

    addSpecialButton(	exitPos,
                      "Exit netPanzer",
                      bExitNetPanzer);
} // end MenuTemplateView::initInGameOptionButtons

// initButtons
//---------------------------------------------------------------------------
void MenuTemplateView::initButtons()
{
    if (Desktop::getVisible("GameView")) {
        initInGameOptionButtons();
    } else {
        initPreGameOptionButtons();
    }
} // end MenuTemplateView::initButtons

// doDraw
//---------------------------------------------------------------------------
void MenuTemplateView::doDraw(Surface &viewArea, Surface &clientArea)
{
    //setWorldRect();
    if (Desktop::getVisible("GameView")) {
	// When ingame, tint the game into gray
        clientArea.bltLookup(getClientRect(), Palette::darkGray256.getColorArray());
        clientArea.drawWindowsBorder(Color::white, Color::white, Color::white);

    } else {
	// When in mainmenu, make background dark and draw menu image
        if(screen->getPixX() > 640 ||
           screen->getPixY() > 480)
            screen->fillRect(0, 0, screen->getPixX(), screen->getPixY(),
                            Color::black);
        
        // Set the following to get does exist.
        if (backgroundSurface.getFrameCount() > 0) {
            backgroundSurface.blt(viewArea, 0, 0);
        } else {
            throw Exception("Where is the background surface?");
        }

        //titlePackedSurface.blt(clientArea, bodyTextRect.min.x, 390);
        titlePackedSurface.bltBlend(clientArea, bodyTextRect.min.x, 390, Palette::colorTable6040);
    }

    View::doDraw(viewArea, clientArea);
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
    loadNetPanzerLogo();

    SpecialButtonView::doActivate();
} // end doActivate

// loadBackgroundSurface
//---------------------------------------------------------------------------
void MenuTemplateView::loadBackgroundSurface()
{
    doLoadBackgroundSurface("pics/backgrounds/menus/menu/til/defaultMB.til");
} // end MenuTemplateView::loadBackgroundSurface

// doLoadBackgroundSurface
//---------------------------------------------------------------------------
void MenuTemplateView::doLoadBackgroundSurface(String string)
{
    backgroundSurface.loadTIL(string);
} // end MenuTemplateView::doLoadBackgroundSurface

// loadTitleSurface
//---------------------------------------------------------------------------
void MenuTemplateView::loadTitleSurface()
{
    doLoadTitleSurface("pics/backgrounds/menus/menu/til/mainTitle.til");
} // end MenuTemplateView::loadTitleSurface

// doLoadBackgroundSurface
//---------------------------------------------------------------------------
void MenuTemplateView::doLoadTitleSurface(String string)
{
    curTitleFlashTime  = 0.0f;
    titleFlashTimeHalf = 2.5;

    String pakString;
    pakString = "pics/backgrounds/menus/menu/pak/";
    pakString += UtilInterface::getFilename(string);
    pakString += ".pak";

    if (UtilInterface::getFileSize(pakString)) {
        titlePackedSurface.load((const char *) pakString);

    } else {
        Surface titleSurface;

        try {
            titleSurface.loadTIL(string);
        } catch(Exception&) {
            titleSurface.create(300, 50, 300, 1);
            titleSurface.fill(128);
            titleSurface.bltStringCenter("No title image", Color::white);
        }

        titlePackedSurface.pack(titleSurface);

        titlePackedSurface.save(pakString);
    }
} // end MenuTemplateView::doLoadTitleSurface

// doDeactivate
//---------------------------------------------------------------------------
void MenuTemplateView::doDeactivate()
{
} // end doDeactivate

//---------------------------------------------------------------------------
void MenuTemplateView::loadNetPanzerLogo()
{} // end MenuTemplateView::loadNetPanzerLogo

// processEvents
//---------------------------------------------------------------------------
void MenuTemplateView::processEvents()
{
} // end MenuTemplateView::processEvents

