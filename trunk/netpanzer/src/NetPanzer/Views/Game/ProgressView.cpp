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
#include "Exception.hpp"
#include "Desktop.hpp"
#include "UIDraw.hpp"
#include "DDHardSurface.hpp"
#include "GameManager.hpp"


ProgressView progressView;


// ProgressView
//---------------------------------------------------------------------------
ProgressView::ProgressView() : View()
{
} // end ProgressView::ProgressView

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

	background.create(628 - 179, 302 - 153, 628 - 179, 1);
	background.fill(0);

	moveTo(0, 0);
	resize(640, 480);

	iXY size(getClientRect().getSize());

} // end ProgressView::init

// doDraw
//---------------------------------------------------------------------------
void ProgressView::doDraw(const Surface &viewArea, const Surface &clientArea)
{
	if (!backgroundSurface.getDoesExist())
	{
		loadBackgroundSurface();
	}
	
	backgroundSurface.blt(clientArea);

	//MenuTemplateView::doDraw(viewArea, clientArea);

	background.blt(clientArea, 179, 153);

	View::doDraw(viewArea, clientArea);

} // end ProgressView::doDraw

// update
//---------------------------------------------------------------------------
// Purpose: Adds the specified text to the current surface, erasing 
//          whatever is on the current line.
//---------------------------------------------------------------------------
void ProgressView::update(const char *text)
{
	int yOffset = background.getPix().y-CHAR_YPIX - 1;

	// Clear the area for the text and draw the new text.
	background.fillRect(0, yOffset, background.getPix().x, yOffset + CHAR_YPIX, Color::black);
	background.bltString(0, background.getPix().y - CHAR_YPIX - 1, text, Color::white);

} // end ProgressView::update

void ProgressView::updateDirect(const char *text)
 {
    FRAME_BUFFER.lock();
    screen.lock(FRAME_BUFFER.mem);

    if (!backgroundSurface.getDoesExist())
	 {
	   loadBackgroundSurface();
	 }
	
	backgroundSurface.blt(screen);

    int yOffset = background.getPix().y-CHAR_YPIX - 1;

    // Clear the area for the text and draw the new text.
    background.fillRect(0, yOffset, background.getPix().x, yOffset + CHAR_YPIX, Color::black);
    background.bltString(0, background.getPix().y - CHAR_YPIX - 1, text, Color::white);

	background.blt(screen, 179, 153);
  
    FRAME_BUFFER.unlock();
    screen.unlock();
    Screen->copyDoubleBufferandFlip(); 
 }

// scroll
//---------------------------------------------------------------------------
// Purpose: Moves all the current text up one line and creates a new
//          clean line for text.
//---------------------------------------------------------------------------
void ProgressView::scroll()
{
	Surface tempSurface;
	tempSurface.copy(background);

	// Move the current text up by the height of the app font.
	tempSurface.blt(background, 0, -CHAR_YPIX - 1);

} // end ProgressView::scroll

void ProgressView::scrollDirect()
{
	Surface tempSurface;

    FRAME_BUFFER.lock();
    screen.lock(FRAME_BUFFER.mem);

    if (!backgroundSurface.getDoesExist())
	 {
	   loadBackgroundSurface();
	 }
	
	backgroundSurface.blt(screen);

	tempSurface.copy(background);

	// Move the current text up by the height of the app font.
	tempSurface.blt(background, 0, -CHAR_YPIX - 1);

	background.blt(screen, 179, 153);
  
    FRAME_BUFFER.unlock();
    screen.unlock();
    Screen->copyDoubleBufferandFlip(); 

} // end ProgressView::scrollDirect

void ProgressView::scrollAndUpdateDirect(const char *text)
 {
	Surface tempSurface;

    FRAME_BUFFER.lock();
    screen.lock(FRAME_BUFFER.mem);

    if (!backgroundSurface.getDoesExist())
	 {
	   loadBackgroundSurface();
	 }
	
	backgroundSurface.blt(screen);

	tempSurface.copy(background);

	// Move the current text up by the height of the app font.
	tempSurface.blt(background, 0, -CHAR_YPIX - 1);

    int yOffset = background.getPix().y-CHAR_YPIX - 1;

    // Clear the area for the text and draw the new text.
    background.fillRect(0, yOffset, background.getPix().x, yOffset + CHAR_YPIX, Color::black);
    background.bltString(0, background.getPix().y - CHAR_YPIX - 1, text, Color::white);

	background.blt(screen, 179, 153);
  
    FRAME_BUFFER.unlock();
    screen.unlock();
    Screen->copyDoubleBufferandFlip(); 

 } // end ProgressView::scrollDirect


void ProgressView::reset()
 {
  background.fill(0);
 }

void ProgressView::open()
{
  	if ( Desktop::getView("ProgressView")->getVisible() == false )
   	{
	   	//GameManager::setVideoMode(iXY(640, 480), false);

		GameManager::drawTextCenteredOnScreen("Sec...", Color::white);

		GameManager::loadPalette("wads/netpmenu.act");

		Desktop::setVisibilityAllWindows(false);
		Desktop::setVisibility("ProgressView", true); 
	}
}


//---------------------------------------------------------------------------
void ProgressView::close()
{
  if ( Desktop::getView("ProgressView")->getVisible() > 0 )
   {
    reset();
    GameManager::drawTextCenteredOnScreen("Sec...", Color::white);

    //GameManager::restorePreviousVideoMode();
	Desktop::checkViewPositions();

    GameManager::loadPalette("wads/netp.act");

    backgroundSurface.free();
    Desktop::setVisibility("ProgressView", false);
   }
} // end ProgressView::close

// toggleGameView
//---------------------------------------------------------------------------
void ProgressView::toggleGameView()
{
	reset();
    backgroundSurface.free();
    GameManager::drawTextCenteredOnScreen("Sec...", Color::white);

    //GameManager::restorePreviousVideoMode();
	Desktop::checkViewPositions();

	// Set the palette to the game palette.
	GameManager::loadPalette("wads/netp.act");

	GameManager::setNetPanzerGameOptions();

	Desktop::setVisibilityAllWindows(false);
	Desktop::setVisibility("GameToolbarView", true);
	Desktop::setVisibility("GameInfoView", true);
	Desktop::setVisibility("MiniMapView", true);
	Desktop::setVisibility("GameView", true);

} // end ProgressView::toggleGameView

// toggleMainMenu
//---------------------------------------------------------------------------
void ProgressView::toggleMainMenu()
{
	reset();
    backgroundSurface.free();
    Desktop::setVisibilityAllWindows(false);
	Desktop::setVisibility("MainView", true);

} // end ProgressView::toggleMainMenu

// doActivate
//---------------------------------------------------------------------------
void ProgressView::doActivate()
{
	Desktop::setActiveView(this);

} // end VehicleSelectionView::doActivate

// loadBackgroundSurface
//---------------------------------------------------------------------------
void ProgressView::loadBackgroundSurface()
{
	String string = "pics/backgrounds/menus/menu/til/loadingMB.til";

	if (!backgroundSurface.loadTIL(string))
	{
		throw Exception("ERROR: Unable to load menu background surface: %s", (const char *) string);
	}

} // end MenuTemplateView::loadBackgroundSurface
