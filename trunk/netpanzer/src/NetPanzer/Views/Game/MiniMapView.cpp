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

#include <algorithm>
#include "MiniMapView.hpp"
#include "Interfaces/MiniMapInterface.hpp"
#include "Interfaces/KeyboardInterface.hpp"
#include "Classes/ScreenSurface.hpp"
#include "Classes/WorldInputCmdProcessor.hpp"
#include "Interfaces/GameConfig.hpp"

MiniMapView miniMapView;


float MiniMapView::scaleDelta   = 400.0f;
int   MiniMapView::decreaseSize = 0;
int   MiniMapView::increaseSize = 0;

// MiniMapView
//---------------------------------------------------------------------------
MiniMapView::MiniMapView() : GameTemplateView()
{
    assert(this != 0);

    setSearchName("MiniMapView");
    setTitle("MiniMapView");
    setSubTitle("");
    setAllowResize(false);
    setDisplayStatusBar(false);
    setVisible(false);
    setAllowMove(false);

    setBordered(false);

    add(CLOSE_VIEW_BUTTON);
    add(MINMAX_VIEW_BUTTON);

    resize(140, 140);
} // end MiniMapView::MiniMapView

// init
//---------------------------------------------------------------------------
void MiniMapView::init()
{
    Surface *miniMap;
    miniMap = MiniMapInterface::getMiniMap();

    //iXY size = miniMap->getPix();
    // don't resize minimap, set to static
    //iXY size(((const iXY &)gameconfig->minimapsize)+iXY(2,2));
    //resize(size);

    //if(gameconfig->minimapposition.isDefaultValue()) {
    //    gameconfig->minimapposition = iXY(0, screen->getHeight() - 196);
    //}
    
    //moveTo(gameconfig->minimapposition);
    
    //checkArea(iXY(screen->getWidth(),screen->getHeight()));

    //int xOffset = size.x;
    //int yOffset = 0;

    MiniMapInterface::setMapScale(getSize() - iXY(2,2));
    //checkArea(iXY(screen->getWidth(),screen->getHeight()));

    minMapSize =  64;
    maxMapSize = 480;

    // Get the original version of the minimap.
    miniMapSurface.copy(*miniMap);

    scaleGroupWait  = 0.0f;
    needScale       = true;
    selectionAnchor = false;

    selectionAnchorDownPos.zero();
    selectionAnchorCurPos.zero();
} // end MiniMapView::init

void
MiniMapView::checkResolution(iXY oldResolution, iXY newResolution)
{
    moveTo(iXY(0,newResolution.y-140));
}

// doDraw
//---------------------------------------------------------------------------
void MiniMapView::doDraw(Surface &viewArea, Surface &clientArea)
{
    miniMapSurface.blt(clientArea, 0, 0);

    // Draw a hairline border.
    //viewArea.drawRect(Color::white);
    viewArea.drawRect(iRect(0,0,viewArea.getWidth(), viewArea.getHeight()),Color::gray);

    // Draw the world view box.
    iRect boxpos = MiniMapInterface::getWorldWindow();
    boxpos.translate(iXY(1,1));                           
    
    clientArea.bltLookup(boxpos, Palette::darkGray256.getColorArray());
    // Draw the units and such on the minimap.
    MiniMapInterface::annotateMiniMap((Surface &) clientArea);
    // Draw the world view box corners.
    clientArea.drawBoxCorners(boxpos, 5, Color::white);

    // If the mouse is over the client area, then change the cursor.
    if (getClientRect().contains(getScreenToClientPos(MouseInterface::getMousePosition()))) {
        if (selectionAnchor) {
            // Since we are selecting units, draw the selection box.
            clientArea.drawRect(iRect(selectionAnchorDownPos, selectionAnchorCurPos), Color::white);
        } else {
            // Draw a box which show where the area which you click will be located.
            drawMouseBox(clientArea);
        }
    }

    GameTemplateView::doDraw(viewArea, clientArea);

} // end doDraw

// setViewWindow
//--------------------------------------------------------------------------
// Purpose: Sets the viewable window on the screen to the position clicked
//          on the map.  pos (position) corresponds to the grid(x,y) position clicked
//          ont he map.  The pos is converted to world coords and the
//          viewable window is repositioned.
//--------------------------------------------------------------------------
void MiniMapView::setViewWindow(const iXY &pos)
{
    assert(this != 0);

    if (iRect(iXY(0,0), getSize()).contains(pos)) {
        MiniMapInterface::setWorldWindowPosition( pos - iXY(1,1) );
    }
} // end MiniMapView::setViewWindow

// drawMouseBox
//--------------------------------------------------------------------------
// Purpose: Replaces the mouse cursor with a box showing where the viewable
//          screen area would be moved to if the mouse was pressed.
//--------------------------------------------------------------------------
void MiniMapView::drawMouseBox(Surface &dest)
{
    assert(this != 0);
    assert(dest.getDoesExist());

    iRect r(MiniMapInterface::getWorldWindow());
    iXY   size(r.getSize() / 2);
    iXY   pos(getScreenToClientPos(MouseInterface::getMousePosition()));

    //dest.drawRect(pos - size - 1, pos + size - 1, Color::yellow);
    dest.drawBoxCorners(pos - size - iXY(1,1), iXY(pos.x + size.x - 1, pos.y + size.y), 5, Color::red);

} // end MiniMapView::drawMouseBox

// lMouseDown
//--------------------------------------------------------------------------
// Purpose:  Move the current viewable area of the map to the area
//           corresponding to the position of teh mouse on the map.
//--------------------------------------------------------------------------
void MiniMapView::lMouseDown(const iXY &pos)
{
    assert(this != 0);

    if (getClientRect().contains(getScreenToClientPos(MouseInterface::getMousePosition()))) {
        if (!selectionAnchor) {
            setViewWindow(pos);
        }
    }

    GameTemplateView::lMouseDown(pos);

} // end MiniMapView::lMouseDown

// rMouseDown
//--------------------------------------------------------------------------
void MiniMapView::rMouseDown(const iXY&)
{
} // end MiniMapView::rMouseDown

void MiniMapView::setMapDrawType(MAP_DRAW_TYPES type)
{
    gameconfig->minimapdrawtype=type;
}

// doIncreaseSize
//--------------------------------------------------------------------------
void MiniMapView::doIncreaseSize(int value)
{
    iXY destSize(getSize());

    if (value == -1) {
        float dt = TimerInterface::getTimeSlice();

        destSize += iXY(int(scaleDelta * dt), int(scaleDelta * dt));
    } else {
        destSize += iXY(value, value);
    }

    //resize(destSize);
    //deltaSize += deltaAmount;
    if (destSize.x > maxMapSize || destSize.y > maxMapSize) {
        destSize = iXY(maxMapSize, maxMapSize);
    }

    // Check the validity of the X dimension.
    if ((min.x + destSize.x) >= (int)screen->getWidth()) {
        int xOffset = min.x + destSize.x - screen->getWidth();

        int destXPos = min.x - xOffset;

        if (destXPos < 0) {
            moveTo(0, min.y);

        } else {
            moveTo(destXPos, min.y);
        }
        checkArea(iXY(screen->getWidth(),screen->getHeight()));
    }

    // Check the validity of the Y dimension.
    if ((min.y + destSize.y) >= (int)screen->getHeight()) {
        int yOffset = min.y + destSize.y - screen->getHeight();

        int destYPos = min.y - yOffset;

        if (destYPos < 0) {
            moveTo(min.x, 0);

        } else {
            moveTo(min.x, destYPos);
        }
        checkArea(iXY(screen->getWidth(),screen->getHeight()));
    }

    // Resize the x dimension.
    if (destSize.x > getSize().x) {
        if (destSize.x > maxMapSize) {
            resize(iXY(maxMapSize, getSize().y));

        } else {
            resize(iXY(destSize.x, getSize().y));
        }
    }

    // Resize the y dimension.
    if (destSize.y > getSize().y) {
        if (destSize.x > maxMapSize) {
            resize(iXY(getSize().x, maxMapSize));

        } else {
            resize(iXY(getSize().x, destSize.x));
        }
    }

    MiniMapInterface::setMapScale(getSize() - iXY(2,2));

    needScale      = true;
    scaleGroupWait = 0.0f;

} // end MiniMapView::doIncreaseSize

// doDecreaseSize
//--------------------------------------------------------------------------
void MiniMapView::doDecreaseSize(int value)
{
    iXY destSize(getSize());

    if (value == -1) {
        float dt = TimerInterface::getTimeSlice();

        destSize -= iXY(int(scaleDelta * dt), int(scaleDelta * dt));
    } else {
        destSize -= iXY(value, value);
    }

    resize(destSize);

    if (destSize.x < minMapSize || destSize.y < minMapSize) {
        resize(iXY(minMapSize, minMapSize));
    }

    MiniMapInterface::setMapScale(getSize() - iXY(2,2));

    needScale      = true;
    scaleGroupWait = 0.0f;

} // end MiniMapView::doDecreaseSize

// lMouseUp
//--------------------------------------------------------------------------
int MiniMapView::lMouseUp(const iXY &downPos, const iXY &upPos)
{
    return View::lMouseUp(downPos, upPos);
} // end MiniMapView::lMouseUp

// mouseMove
//--------------------------------------------------------------------------
void MiniMapView::mouseMove(const iXY &prevPos, const iXY &newPos)
{
    if (getClientRect().contains(getScreenToClientPos(MouseInterface::getMousePosition()))) {
        if (	(KeyboardInterface::getKeyState(SDLK_LCTRL) ||
                KeyboardInterface::getKeyState(SDLK_RCTRL))) {

            if (!selectionAnchor) {
                selectionAnchorDownPos = newPos;
            }

            selectionAnchorCurPos = newPos;
            selectionAnchor       = true;

            // Set the selection cursor.
            MouseInterface::setCursor("select.bmp");
        } else {
            selectionAnchor = false;
        }
    }

    GameTemplateView::mouseMove(prevPos, newPos);
} // end MiniMapView::mouseMove

// lMouseDrag
//--------------------------------------------------------------------------
void MiniMapView::lMouseDrag(const iXY&, const iXY&, const iXY &newPos)
{
    lMouseDown(newPos);

} // end MiniMapView::lMouseDrag
