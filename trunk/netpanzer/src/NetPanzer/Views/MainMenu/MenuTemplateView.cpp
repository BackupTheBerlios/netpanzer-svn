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


#include "Views/MainMenu/MenuTemplateView.hpp"
#include "2D/Palette.hpp"
#include "Views/Components/Desktop.hpp"
#include "Views/Components/Button.hpp"
#include "Interfaces/GameConfig.hpp"
#include "Interfaces/GameManager.hpp"
#include "System/Sound.hpp"
#include "Views/Components/ViewGlobals.hpp"
#include "Particles/RadarPingParticle2D.hpp"
#include "Classes/ScreenSurface.hpp"
#include "Particles/Particle2D.hpp"
#include "Particles/ParticleSystem2D.hpp"
#include "2D/PackedSurface.hpp"
#include "Interfaces/GameManager.hpp"
#include "Util/Exception.hpp"
#include "Views/GameViewGlobals.hpp"
#include "Multi/ServerListView.hpp"

#include "Multi/MapSelectionView.hpp"
#include "Multi/IPAddressView.hpp"
#include "Resources/ResourceManager.hpp"
#include "Interfaces/PlayerGameManager.hpp"

#ifndef PACKAGE_VERSION
	#define PACKAGE_VERSION "testing"
#endif

Surface       MenuTemplateView::backgroundSurface;
PackedSurface MenuTemplateView::titlePackedSurface;

static void bMain()
{
    Desktop::setVisibilityAllWindows(false);
    Desktop::setVisibility("MenuTemplateView", true);
    Desktop::setVisibility("MainView", true);
    ((MenuTemplateView*)Desktop::getView("MenuTemplateView"))->hidePlayButton();
}

static void bHost()
{
    gameconfig->hostorjoin = _game_session_host;
    Desktop::setVisibilityAllWindows(false);
    Desktop::setVisibility("MenuTemplateView", true);
    Desktop::setVisibility("UnitSelectionView", true);
    Desktop::setVisibility("HostOptionsView", true);
    Desktop::setVisibility("MapSelectionView", true);
    Desktop::setVisibility("PlayerNameView", true);
    ((MenuTemplateView*)Desktop::getView("MenuTemplateView"))->showPlayButton();
    // TODO: do doLoadTitleSurface("hostTitle");
}
static void bJoin()
{
    gameconfig->hostorjoin = _game_session_join;
    Desktop::setVisibilityAllWindows(false);
    Desktop::setVisibility("MenuTemplateView", true);
    Desktop::setVisibility("PlayerNameView", true);
    Desktop::setVisibility("IPAddressView", true);
    Desktop::setVisibility("ServerListView", true);
    serverlistview->refresh();
    ((MenuTemplateView*)Desktop::getView("MenuTemplateView"))->showPlayButton();
    // TODO: do doLoadTitleSurface("joinTitle");
}

static void bOptions()
{
    Desktop::setVisibilityAllWindows(false);
    Desktop::setVisibility("OptionsView", true);
    ((MenuTemplateView*)Desktop::getView("MenuTemplateView"))->hidePlayButton();
}

static void bHelp()
{
    Desktop::setVisibilityAllWindows(false);
    // TODO: should doLoadTitleSurface("helpTitle");
    Desktop::setVisibility("MenuTemplateView", true);
    Desktop::setVisibilityNoDoAnything("HelpScrollView", true);
    ((MenuTemplateView*)Desktop::getView("MenuTemplateView"))->hidePlayButton();
}

static void bExit()
{
    Desktop::setVisibilityAllWindows(false);
    GameManager::exitNetPanzer();
}

//---------------------------------------------------------------------------
static void bResign()
{
    Desktop::setVisibility("OptionsView", false);
    Desktop::setVisibility("AreYouSureResignView", true);
}

//---------------------------------------------------------------------------
static void bCloseOptions()
{
    GameManager::setNetPanzerGameOptions();

    Desktop::setVisibility("OptionsView", false);
}

//---------------------------------------------------------------------------
static void bExitNetPanzer()
{
    Desktop::setVisibility("OptionsView", false);
    Desktop::setVisibility("AreYouSureExitView", true);
}

static void bPlay()
{
    if ( GameConfig::player_name->length() == 0 )
        return;

    // Check a few things which should be ok.

    if (MapSelectionView::curMap == -1)
    {
        return;
    }
    
    if (  gameconfig->hostorjoin == _game_session_join
       && strcmp(IPAddressView::szServer.getString(), "") == 0 )
    {
        return;
    }

    // Close all menu views.
    Desktop::setVisibilityAllWindows(false);

    if ( gameconfig->hostorjoin == _game_session_join )
    {
        gameconfig->serverConnect = IPAddressView::szServer.getString();
        IPAddressView::szServer.setString("");
    }
    
    serverlistview->endQuery();

    // TODO: do it?
//    MenuTemplateView::backgroundSurface.free();

    ResourceManager::updateFlagData( 0,
                                     GameConfig::player_flag_data,
                                     sizeof(GameConfig::player_flag_data) );

    PlayerGameManager* manager = (PlayerGameManager*) gamemanager;
    manager->launchMultiPlayerGame();
}

// MenuTemplateView
//---------------------------------------------------------------------------
MenuTemplateView::MenuTemplateView() : RMouseHackView()
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
    resize(iXY(800, 600));

    initButtons();
    
    curTitleFlashTime  = 0.0f;
    titleFlashTimeHalf = 0.5;
} // end MenuTemplateView constructor

//---------------------------------------------------------------------------
// initPreGameOptionButtons
void MenuTemplateView::initPreGameOptionButtons()
{
    add( Button::createMenuButton("MAIN", "Main", mainPos, false) );
    add( Button::createMenuButton( "JOIN", "Join", joinPos, false) );
    add( Button::createMenuButton( "HOST", "Host", hostPos, false) );
    add( Button::createMenuButton( "OPTIONS", "Options", optionsPos, false) );
    add( Button::createMenuButton( "HELP", "Help", helpPos, false) );
    add( Button::createMenuButton( "EXITNP", "Exit netPanzer", exitPos, false) );
    
    playButton = Button::createMenuButton("PLAY", "Play", iXY(-100,-100), true);
    add( playButton );
} // end MenuTemplateView::initPreGameOptionButtons

// initInGameOptionButtons
//---------------------------------------------------------------------------
void MenuTemplateView::initInGameOptionButtons()
{
    if(!gameconfig->quickConnect) {
        add( Button::createMenuButton( "RESIGN", "Resign", resignPos, false) );
        add( Button::createMenuButton( "EXITNETNP", "Exit netPanzer", exitPos, false) );
    } else {
        add( Button::createMenuButton( "RESIGN", "Resign", exitPos, true) );
    }

    add( Button::createMenuButton( "CLOSEOPT", "Close Options", returnToGamePos, false) );
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
    if (Desktop::getVisible("GameView"))
    {
	// When ingame, tint the game into gray
        clientArea.BltRoundRect(getClientRect(), 10, Palette::darkGray256.getColorArray());
        clientArea.RoundRect(MenuRect, 10, Color::gray);
        clientArea.drawWindowsBorder();

    }
    else
    {        
        screen->fill(0);
		// Set the following to get does exist.
        if (backgroundSurface.getNumFrames() > 0) {
            backgroundSurface.blt(viewArea, 0, 0);
        } else {
            throw Exception("Where is the background surface?");
        }
        clientArea.BltRoundRect(MenuRect, 10, Palette::darkGray256.getColorArray());
        clientArea.RoundRect(MenuRect, 10, Color::gray);

        //titlePackedSurface.blt(clientArea, bodyTextRect.min.x, 390);
        titlePackedSurface.bltBlend(clientArea, MenuRect.min.x, MenuRect.max.y-60, Palette::colorTable6040);
    }

    static char text[] =
        "NetPanzer " PACKAGE_VERSION;
        clientArea.bltString(10, 590, text, Color::yellow);

    View::doDraw(viewArea, clientArea);
} // end doDraw

// doActivate
//---------------------------------------------------------------------------
void MenuTemplateView::doActivate()
{
    // Make the activating view active, redo this please!
    Desktop::setActiveView(searchName);

    loadBackgroundSurface();
    loadTitleSurface();
    loadNetPanzerLogo();
} // end doActivate

// loadBackgroundSurface
//---------------------------------------------------------------------------
void MenuTemplateView::loadBackgroundSurface()
{
    doLoadBackgroundSurface("pics/backgrounds/menus/menu/defaultMB.bmp");
} // end MenuTemplateView::loadBackgroundSurface

// doLoadBackgroundSurface
//---------------------------------------------------------------------------
void MenuTemplateView::doLoadBackgroundSurface(const std::string& string)
{
    backgroundSurface.loadBMP(string.c_str());
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

    std::string pakString = "pics/backgrounds/menus/menu/pak/";
    pakString += string;
    pakString += ".pak";

    titlePackedSurface.load(pakString);
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

void MenuTemplateView::onComponentClicked(Component* c)
{
    string cname = c->getName();
    if ( !cname.compare("Button.MAIN") )
    {
        bMain();
    }
    else if ( !cname.compare("Button.JOIN") )
    {
        bJoin();
    }
    else if ( !cname.compare("Button.HOST") )
    {
        bHost();
    }
    else if ( !cname.compare("Button.OPTIONS") )
    {
        bOptions();
    }
    else if ( !cname.compare("Button.HELP") )
    {
        bHelp();
    }
    else if ( !cname.compare("Button.EXITNP") )
    {
        bExit();
    }
    else if ( !cname.compare("Button.EXITNETNP") )
    {
        bExitNetPanzer();
    }
    else if ( !cname.compare("Button.RESIGN") )
    {
        bResign();
    }
    else if ( !cname.compare("Button.CLOSEOPT") )
    {
        bCloseOptions();
    }
    else if ( !cname.compare("Button.PLAY") )
    {
        bPlay();
    }
}

void
MenuTemplateView::showPlayButton()
{
    playButton->setLocation(playPos);
}

void
MenuTemplateView::hidePlayButton()
{
    playButton->setLocation(-100,-100);
}
