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

#include "ProgressView.hpp"
#include "Desktop.hpp"
#include "ScreenSurface.hpp"
#include "GameManager.hpp"

LoadingView *progressView = 0;

// ProgressView
//---------------------------------------------------------------------------
ProgressView::ProgressView() : LoadingView(), background(0), backgroundSurface(0)
{} // end ProgressView::ProgressView

// init
//---------------------------------------------------------------------------
void ProgressView::init()
{
    setSearchName("ProgressView");
    setTitle("Progress Update");
    setSubTitle("");

    setAllowResize(false);
    setAllowMove(false);
    setDisplayStatusBar(false);
    setVisible(false);
    setBordered(false);

    if ( background )
        delete background;
    
    background = new Surface(628 - 179, 302 - 153, 1);
    background->fill(0);

    resize(640, 480);
} // end ProgressView::init

// doDraw
//---------------------------------------------------------------------------
void ProgressView::doDraw(Surface &viewArea, Surface &clientArea)
{
    screen->fill(Color::black);
    backgroundSurface->blt(clientArea, 0, 0);
    background->blt(clientArea, 179, 153);

    View::doDraw(viewArea, clientArea);
} // end ProgressView::doDraw

// update
//---------------------------------------------------------------------------
// Purpose: Adds the specified text to the current surface, erasing
//          whatever is on the current line.
//---------------------------------------------------------------------------
void ProgressView::update(const char *text)
{
    int CHAR_YPIX = Surface::getFontHeight();
    int yOffset = background->getHeight()-CHAR_YPIX - 1;

    // Clear the area for the text and draw the new text.
    background->fillRect(iRect(0, yOffset, background->getWidth(), yOffset + CHAR_YPIX), 0);
    background->bltString(0, background->getHeight() - CHAR_YPIX - 1, text, Color::white);
} // end ProgressView::update

// scroll
//---------------------------------------------------------------------------
// Purpose: Moves all the current text up one line and creates a new
//          clean line for text.
//---------------------------------------------------------------------------
void ProgressView::scroll()
{
    Surface * tempSurface = new Surface(background->getWidth(), background->getHeight(), 1);
    background->blt(*tempSurface, 0, - Surface::getFontHeight() - 1);
    delete background;
    background = tempSurface;
} // end ProgressView::scroll

void ProgressView::blitToScreen()
{
//    screen->lock();
                                                  
    screen->fill(Color::black);
    backgroundSurface->blt(*screen, min.x, min.y);
    background->blt(*screen, min.x+179, min.y+153);
                                                  
//    screen->unlock();
    screen->copyToVideoFlip();
}

void ProgressView::reset()
{
    background->fill(0);
}

void ProgressView::open()
{
    if ( Desktop::getView("ProgressView")->getVisible() == false ) {
        GameManager::drawTextCenteredOnScreen("Sec...", Color::white);

        GameManager::loadPalette("netpmenu");

        Desktop::setVisibilityAllWindows(false);
        Desktop::setVisibility("ProgressView", true);
    }
}

//---------------------------------------------------------------------------
void ProgressView::close()
{
    if ( Desktop::getView("ProgressView")->getVisible() > 0 ) {
        reset();
        GameManager::drawTextCenteredOnScreen("Sec...", Color::white);

        GameManager::loadPalette("netp");

        if ( backgroundSurface )
            delete backgroundSurface;
        backgroundSurface = 0;

        Desktop::setVisibility("ProgressView", false);
    }
} // end ProgressView::close

// toggleGameView
//---------------------------------------------------------------------------
void ProgressView::toggleGameView()
{
    reset();
    if ( backgroundSurface )
        delete backgroundSurface;
    backgroundSurface = 0;

    GameManager::drawTextCenteredOnScreen("Sec...", Color::white);

    // Set the palette to the game palette.
    GameManager::loadPalette("netp");

    GameManager::setNetPanzerGameOptions();

    Desktop::setVisibilityAllWindows(false);
    Desktop::setVisibility("GameInfoView", true);
    Desktop::setVisibility("MiniMapView", true);
    Desktop::setVisibility("GameView", true);
} // end ProgressView::toggleGameView

// toggleMainMenu
//---------------------------------------------------------------------------
void ProgressView::toggleMainMenu()
{
    reset();
    if ( backgroundSurface )
        delete backgroundSurface;
    backgroundSurface = 0;

    Desktop::setVisibilityAllWindows(false);
    Desktop::setVisibility("MainView", true);
} // end ProgressView::toggleMainMenu

// doActivate
//---------------------------------------------------------------------------
void ProgressView::doActivate()
{
    backgroundSurface = new Surface();
    backgroundSurface->loadBMP("pics/backgrounds/menus/menu/loadingMB.bmp");

    Desktop::setActiveView(this);
} // end VehicleSelectionView::doActivate
