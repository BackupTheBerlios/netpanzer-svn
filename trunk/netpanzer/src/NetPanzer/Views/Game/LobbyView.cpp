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

#include "Util/Exception.hpp"
#include "LobbyView.hpp"
#include "Desktop.hpp"
#include "GameManager.hpp"
#include "GameConfig.hpp"
#include "ScreenSurface.hpp"
#include "System/Sound.hpp"

LoadingView *lobbyView = 0;

static void bAbort()
{
    if(gameconfig->quickConnect) {
        GameManager::exitNetPanzer();
        return;
    }
    sound->stopTankIdle();
    Desktop::setVisibilityAllWindows(false);
    Desktop::setVisibility("MainView", true);
}


// LobbyView
//---------------------------------------------------------------------------
LobbyView::LobbyView() : LoadingView(), currentline(0)
{} // end LobbyView::LobbyView

// init
//---------------------------------------------------------------------------
void LobbyView::init()
{
    setSearchName("LobbyView");
    setTitle("Join Lobby");
    setSubTitle("");

    setAllowResize(false);
    setAllowMove(false);
    setDisplayStatusBar(false);
    setVisible(false);
    setBordered(false);

    background.create(628 - 179, 302 - 153, 628 - 179, 1);
    background.fill(0);

    moveTo(0, 0);
    resize(640, 480);

    addButtonCenterText(iXY(628 - 60, 302 - 15), 60, "Abort", "Cancel the joining of this game.", bAbort);

    currentline = 0;
} // end LobbyView::init

// doDraw
//---------------------------------------------------------------------------
void LobbyView::doDraw(Surface &viewArea, Surface &clientArea)
{
    if (!backgroundSurface.getDoesExist()) {
        loadBackgroundSurface();
    }

    screen->fill(Color::black);
    backgroundSurface.blt(clientArea);
    background.blt(clientArea, 179, 153);

    View::doDraw(viewArea, clientArea);
} // end LobbyView::doDraw

// update
//---------------------------------------------------------------------------
// Purpose: Adds the specified text to the current surface, erasing
//          whatever is on the current line.
//---------------------------------------------------------------------------
void LobbyView::update(const char *text)
{
    //int yOffset = background.getPix().y-CHAR_YPIX - 1;
    int yOffset = Surface::getFontHeight() * currentline;

    // Clear the area for the text and draw the new text.
    background.fillRect(0, yOffset, background.getPix().x, yOffset + Surface::getFontHeight(), Color::black);
    background.bltString(0, yOffset, text, Color::white);

} // end LobbyView::update

// scroll
//---------------------------------------------------------------------------
// Purpose: Moves all the current text up one line and creates a new
//          clean line for text.
//---------------------------------------------------------------------------
void LobbyView::scroll()
{
    if(currentline * Surface::getFontHeight() > background.getPix().y - Surface::getFontHeight()) {
        Surface tempSurface;
        tempSurface.copy(background);

        // Move the current text up by the height of the app font.
        tempSurface.blt(background, Surface::getFontHeight(), 0);
    } else {
        currentline++;
    }
} // end LobbyView::scroll

// close

void LobbyView::reset()
{
    background.fill(0);
}

void LobbyView::open()
{
    if ( Desktop::getView("LobbyView")->getVisible() == false ) {
        GameManager::drawTextCenteredOnScreen("Sec...", Color::white);

        GameManager::loadPalette("wads/netpmenu.act");

        Desktop::setVisibilityAllWindows(false);
        Desktop::setVisibility("LobbyView", true);
    }
}


//---------------------------------------------------------------------------
void LobbyView::close()
{
    backgroundSurface.free();
    Desktop::setVisibility("LobbyView", false);
} // end LobbyView::close

// toggleGameView
//---------------------------------------------------------------------------
void LobbyView::toggleGameView()
{
    reset();
    backgroundSurface.free();
    GameManager::drawTextCenteredOnScreen("Sec...", Color::white);

    // Set the palette to the game palette.
    GameManager::loadPalette("wads/netp.act");

    GameManager::setNetPanzerGameOptions();

    Desktop::setVisibilityAllWindows(false);
    Desktop::setVisibility("GameInfoView", true);
    Desktop::setVisibility("MiniMapView", true);
    Desktop::setVisibility("GameView", true);

} // end LobbyView::toggleGameView

// toggleMainMenu
//---------------------------------------------------------------------------
void LobbyView::toggleMainMenu()
{
    reset();
    backgroundSurface.free();
    Desktop::setVisibilityAllWindows(false);
    Desktop::setVisibility("MainView", true);

} // end LobbyView::toggleMainMenu

// doActivate
//---------------------------------------------------------------------------
void LobbyView::doActivate()
{
    Desktop::setActiveView(this);

} // end VehicleSelectionView::doActivate

// loadBackgroundSurface
//---------------------------------------------------------------------------
void LobbyView::loadBackgroundSurface()
{
    backgroundSurface.loadBMP("pics/backgrounds/menus/menu/loadingMB.bmp");
} // end MenuTemplateView::loadBackgroundSurface

