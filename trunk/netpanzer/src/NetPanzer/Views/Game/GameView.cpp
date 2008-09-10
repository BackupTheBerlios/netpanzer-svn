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

#include "GameView.hpp"
#include "Views/Components/Desktop.hpp"
#include "Classes/TileEngine.hpp"
#include "Units/UnitInterface.hpp"
#include "Weapons/ProjectileInterface.hpp"
#include "Interfaces/MouseInterface.hpp"
#include "Objectives/ObjectiveInterface.hpp"
#include "Classes/WorldInputCmdProcessor.hpp"
#include "Interfaces/WorldViewInterface.hpp"
#include "Classes/ScreenSurface.hpp"
#include "Interfaces/ConsoleInterface.hpp"
#include "Particles/Particle2D.hpp"
#include "Particles/ParticleSystem2D.hpp"
#include "Views/Components/ViewGlobals.hpp"
#include "Particles/ParticleInterface.hpp"
#include "2D/PackedSurface.hpp"
#include "Views/Game/VehicleSelectionView.hpp"
#include "PowerUps/PowerUpInterface.hpp"

GameView gameView;
int GameView::gDrawSolidBackground = 0;


// GameView
//---------------------------------------------------------------------------
GameView::GameView() : View()
{} // end GameView::GameView

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
    resize(iXY(screen->getWidth(), screen->getHeight()));

} // end GameView::init

// doDraw
//---------------------------------------------------------------------------
void GameView::doDraw(Surface&, Surface &clientArea)
{
    // Added for debugging, accesible through LibView.
    if (gDrawSolidBackground) {
        screen->fill(250);
    } else {
        TileEngine::blitWorld();
    }

    // Added for debugging, accesible through LibView.
    if (ParticleInterface::gTestSim) {
        ParticleInterface::testSim();
        //ParticleInterface::testSimText(viewArea);
    }

    iRect world_win;
    WorldViewInterface::getViewWindow( &world_win );
    SPRITE_SORTER.reset(world_win);

    PackedSurface::totalDrawCount = 0;
    ParticleSystem2D::drawAll(clientArea, SPRITE_SORTER );
    Particle2D::drawAll(clientArea, SPRITE_SORTER );

    UnitInterface::offloadGraphics( SPRITE_SORTER );
    ProjectileInterface::offloadGraphics( SPRITE_SORTER );
    ObjectiveInterface::offloadGraphics( SPRITE_SORTER );
    PowerUpInterface::offloadGraphics( SPRITE_SORTER );

    SPRITE_SORTER.blitLists(screen);

    VehicleSelectionView::drawMiniProductionStatus(*screen);

    COMMAND_PROCESSOR.draw();

    // Make sure the console info is the last thing drawn.
    ConsoleInterface::update(*screen);

} // end GameView::doDraw

// lMouseDown
//---------------------------------------------------------------------------
void GameView::doActivate()
{
    View::doActivate();
    MouseInterface::event_queue.clear();
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

    if (!MouseInterface::getButtonMask() && Desktop::getFocus() != this) {
        Desktop::setFocusView(this);
        //Desktop::setActiveView(this);
        MouseInterface::setCursor("default.bmp");
    }

} // end GameView::mouseMove
