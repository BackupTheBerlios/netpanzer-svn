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

/////////////////////////////////////////////////////////////////////////////
// Includes.
/////////////////////////////////////////////////////////////////////////////

#include "GameView.hpp"
#include "Desktop.hpp"
#include "TileEngine.hpp"
#include "UnitInterface.hpp"
#include "ProjectileInterface.hpp"
#include "MouseInterface.hpp"
#include "ObjectiveInterface.hpp"
#include "WorldInputCmdProcessor.hpp"
#include "WorldViewInterface.hpp"
#include "DDHardSurface.hpp"
#include "ConsoleInterface.hpp"
#include "Particle2D.hpp"
#include "ParticleSystem2D.hpp"
#include "cMouse.hpp"
#include "ViewGlobals.hpp"
#include "FontSystem2D.hpp"
#include "MiniRankView.hpp"
#include "ParticleInterface.hpp"
#include "PackedSurface.hpp"
#include "VehicleSelectionView.hpp"
#include "PowerUpInterface.hpp"

GameView gameView;
int GameView::gDrawSolidBackground = 0;


// GameView
//---------------------------------------------------------------------------
GameView::GameView() : View()
{
} // end GameView::GameView

// setSize
//---------------------------------------------------------------------------
void GameView::setSize(iXY size)
{
	resize(size);

} // end GameView::setSize

// init
//---------------------------------------------------------------------------
void GameView::init()
{
	setSearchName("GameView");
	setTitle("Game");
	setSubTitle("");

	setBordered(false);
	setAlwaysOnBottom(true);
	setAllowResize(false);
	setDisplayStatusBar(false);
	setVisible(false);

	moveTo(iXY(0, 0));
	resize(iXY(SCREEN_XPIX, SCREEN_YPIX));

} // end GameView::init

// doDraw
//---------------------------------------------------------------------------
void GameView::doDraw(const Surface &viewArea, const Surface &clientArea)
{
	// Added for debugging, accesible through LibView.
	if (gDrawSolidBackground)
	{
		screen.fill(250);
	} else
	{
		TileEngine::blitWorld();
	}
	
	// Added for debugging, accesible through LibView.
	if (ParticleInterface::gTestSim)
	{
		ParticleInterface::testSim();
		//ParticleInterface::testSimText(viewArea);
	}

	Recti world_win;
	WorldViewInterface::getViewWindow( &world_win );
	SPRITE_SORTER.reset(world_win);

	PackedSurface::totalDrawCount = 0;
	ParticleSystem2D::drawAll(clientArea, SPRITE_SORTER );
	Particle2D::drawAll(clientArea, SPRITE_SORTER );

	UnitInterface::offloadGraphics( SPRITE_SORTER );
	ProjectileInterface::offloadGraphics( SPRITE_SORTER );
	ObjectiveInterface::offloadGraphics( SPRITE_SORTER );
    PowerUpInterface::offloadGraphics( SPRITE_SORTER );

	SPRITE_SORTER.blitLists( &FRAME_BUFFER );  

	VehicleSelectionView::drawMiniProductionStatus(screen);

	COMMAND_PROCESSOR.updateControls();

	// Make sure the console info is the last thing drawn.
	ConsoleInterface::update( FRAME_BUFFER );

} // end GameView::doDraw

// lMouseDown
//---------------------------------------------------------------------------
void GameView::doActivate()
{
	View::doActivate();
	MouseInterface::event_queue.reset();
	MouseInterface::resetButtonHoldStatus();
	COMMAND_PROCESSOR.inFocus();

} // end GameView::doActivate

// processEvents
//---------------------------------------------------------------------------
void GameView::processEvents()
{
	COMMAND_PROCESSOR.process();

} // end GameView::processEvents

// mouseEnter
//--------------------------------------------------------------------------
//void GameView::mouseEnter(const iXY &pos)
//{
//	if (!mouse.getButtonMask())
//	{
//		Desktop::setActiveView(this);
//	}
//
//} // end GameView::mouseEnter

// mouseMove
//--------------------------------------------------------------------------
void GameView::mouseMove(const iXY & prevPos, const iXY &newPos)
{
	View::mouseMove(prevPos, newPos);

	if (!mouse.getButtonMask() && Desktop::getFocus() != this)
	{
		Desktop::setFocusView(this);
		//Desktop::setActiveView(this);
		MouseInterface::setCursor(_mcursor_default);
	}

} // end GameView::mouseMove
