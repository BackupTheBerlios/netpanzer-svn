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


#include "GameView.hpp"
#include "Views/Components/Desktop.hpp"
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
#include "2D/Surface.hpp"
#include "Views/Game/VehicleSelectionView.hpp"
#include "PowerUps/PowerUpInterface.hpp"

#include "Classes/ViewCamera.hpp"
#include "Interfaces/MapInterface.hpp"
#include "Interfaces/WorldViewInterface.hpp"

#include "Views/Components/InfoBar.hpp"
#include "Views/Components/MiniMap.hpp"

#include "Core/GlobalGameState.hpp"

int GameView::gDrawSolidBackground = 0;

// GameView
//---------------------------------------------------------------------------
GameView::GameView() : View()
{
    setSearchName("GameView");
    setTitle("Game");
    setSubTitle("");

    setBordered(false);
    setAlwaysOnBottom(true);
    setDisplayStatusBar(false);
    setVisible(false);

    resize(640,480);
    moveTo(iXY(0, 0));

    add(new InfoBar(0,0));
    // will add after input handling is done
    //add(new MiniMap(100,100));

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
    resize(iXY(screen->getWidth(), screen->getHeight()));
    moveTo(iXY(0, 0));
} // end GameView::init

void
GameView::checkResolution(iXY oldResolution, iXY newResolution)
{
    resize(iXY(newResolution.x, newResolution.y));
    moveTo(iXY(0,0));
}
// doDraw
//---------------------------------------------------------------------------
void GameView::doDraw()
{
    // Added for debugging, accesible through LibView.
    //screen->fill(0);

    if (gDrawSolidBackground) {
        fill(250);
    } else {
        drawMap(*screen);
    }

    // Added for debugging, accesible through LibView.
    if (ParticleInterface::gTestSim) {
        ParticleInterface::testSim();
        //ParticleInterface::testSimText(viewArea);
    }

    iRect world_win;
    WorldViewInterface::getViewWindow( &world_win );
    SPRITE_SORTER.reset(world_win);

    ParticleSystem2D::drawAll( SPRITE_SORTER );
    Particle2D::drawAll( SPRITE_SORTER );

    global_game_state->unit_manager->offloadGraphics( SPRITE_SORTER );
    ProjectileInterface::offloadGraphics( SPRITE_SORTER );
    ObjectiveInterface::offloadGraphics( SPRITE_SORTER );
    PowerUpInterface::offloadGraphics( SPRITE_SORTER );

    SPRITE_SORTER.blitLists(screen);

    VehicleSelectionView::drawMiniProductionStatus(*screen);

    COMMAND_PROCESSOR.draw();

    // Make sure the console info is the last thing drawn.
    ConsoleInterface::update(*screen);
    
    View::doDraw();
} // end GameView::doDraw

// lMouseDown
//---------------------------------------------------------------------------
void GameView::doActivate()
{
    View::doActivate();
    MouseInterface::event_queue.clear();
    COMMAND_PROCESSOR.inFocus();
} // end GameView::doActivate

void
GameView::doDeactivate()
{
    COMMAND_PROCESSOR.closeSelectionBox();
}

// processEvents
//---------------------------------------------------------------------------
void GameView::processEvents()
{
    COMMAND_PROCESSOR.process(true);
} // end GameView::processEvents

// mouseMove
//--------------------------------------------------------------------------
void GameView::mouseMove(const iXY & prevPos, const iXY &newPos)
{
    View::mouseMove(prevPos, newPos);

    if (!MouseInterface::getButtonMask() && Desktop::getFocus() != this) {
        Desktop::setFocusView(this);
        //Desktop::setActiveView(this);
        MouseInterface::setCursor("default.png");
    }

} // end GameView::mouseMove

void
GameView::drawMap(Surface &window)
{
    TileSet * ts = global_game_state->tile_set;
    unsigned long world_x;
    unsigned long world_y;
    unsigned short map_x;
    unsigned short map_y;
    
    WorldViewInterface::getMainCamera()->getViewStart(window.getWidth(), window.getHeight(),
                              &world_x, &world_y);
    MapInterface::pointXYtoMapXY( world_x, world_y, &map_x, &map_y );
        
    unsigned short tile_size = ts->getTileXsize();
    
    long partial_y = world_y % tile_size;
    int y = 0;
    if ( partial_y )
    {
        y -= partial_y;
    }
    
    long partial_x = world_x % tile_size;
    int start_x = 0;
    if ( partial_x )
    {
        start_x -= partial_x;
    }
    
    unsigned int tile = 0;
    
    WorldMap * map = global_game_state->world_map;
    
    unsigned short tmx;
    Surface * tile_surf = 0;
    
    for ( ; y < (int)window.getHeight(); y += tile_size )
    {
        tmx = map_x;
        for ( int x = start_x; x < (int)window.getWidth(); x += tile_size )
        {
            tile = map->getValue(tmx++, map_y);
            tile_surf = ts->getTile(tile);
            if ( tile_surf )
            {
            	tile_surf->blt(window, x, y);
            }
        }
        map_y ++;
    }
}
