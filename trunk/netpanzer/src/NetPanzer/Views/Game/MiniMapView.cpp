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
#include "cMouse.hpp"
#include "MiniMapInterface.hpp"
#include "KeyboardInterface.hpp"
#include "cMouse.hpp"
#include "ScreenSurface.hpp"
#include "WorldInputCmdProcessor.hpp"


MiniMapView miniMapView;

MAP_DRAW_TYPES MiniMapView::mapDrawType;

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

    setBordered(false);

    add(CLOSE_VIEW_BUTTON);
    add(MINMAX_VIEW_BUTTON);

    resize(100, 100);
} // end MiniMapView::MiniMapView

// init
//---------------------------------------------------------------------------
void MiniMapView::init()
{
    Surface *miniMap;
    miniMap = MiniMapInterface::getMiniMap();

    //iXY size = miniMap->getPix();
    iXY size(196, 196);
    resize(size);
    moveTo(iXY(0, 0));

    mapDrawType = MAP_SOLID;

    //int xOffset = size.x;
    //int yOffset = 0;

    MiniMapInterface::setMapScale(getSize() - iXY(2,2));

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

// doDraw
//---------------------------------------------------------------------------
void MiniMapView::doDraw(const Surface &viewArea, const Surface &clientArea)
{
    assert(this != 0);
    assert(viewArea.getDoesExist());
    assert(clientArea.getDoesExist());

    if (decreaseSize != 0) {
        doDecreaseSize(decreaseSize);
        decreaseSize = 0;
    }
    if (increaseSize != 0) {
        doIncreaseSize(increaseSize);
        increaseSize = 0;
    }

    float dt = TimerInterface::getTimeSlice();

    Surface *miniMap;
    miniMap = MiniMapInterface::getMiniMap();

    if (needScale) {
        scaleGroupWait += dt;

        if (scaleGroupWait > 1.0f) {
            miniMapSurface.create(getSize(), getSize().x , 1);

            //miniMapSurface.scale(getViewRect().getSize());
            iRect r(iXY(0, 0), getSize());

            miniMapSurface.bltScale(*miniMap, r);

            needScale      = false;
            scaleGroupWait = 0.0f;
        }
    }

    iRect r(iXY(0,0), getSize());

    if (needScale) {
        // Draw the slow on the fly scaled map.
        if (mapDrawType == MAP_SOLID) {
            clientArea.bltScale(*miniMap, r);
        } else if (mapDrawType == MAP_2080) {
            clientArea.bltBlendScale(*miniMap, r, Palette::colorTable2080);
        } else if (mapDrawType == MAP_4060) {
            clientArea.bltBlendScale(*miniMap, r, Palette::colorTable4060);
        }
        //else if (mapDrawType == MAP_BLEND_GREEN)
        //{
        //clientArea.bltLookup(iRect(iXY(0, 0), getSize()), Palette::green256.getColorArray());
        //}
        else if (mapDrawType == MAP_BLEND_GRAY) {
            clientArea.bltLookup(iRect(iXY(0, 0), getSize()), Palette::gray256.getColorArray());
        } else if (mapDrawType == MAP_BLEND_DARK_GRAY) {
            clientArea.bltLookup(iRect(iXY(0, 0), getSize()), Palette::darkGray256.getColorArray());
        } else if (mapDrawType == MAP_BLACK) {
            clientArea.fill(Color::black);
        } else if (mapDrawType == MAP_TRANSPARENT) {}
    }
    else {
        // Draw the fast not on the fly scaled map.
        if (mapDrawType == MAP_SOLID) {
            miniMapSurface.blt(clientArea, 0, 0);
        } else if (mapDrawType == MAP_2080) {
            clientArea.blendIn(miniMapSurface, iXY(0, 0), Palette::colorTable2080);
        } else if (mapDrawType == MAP_4060) {
            clientArea.blendIn(miniMapSurface, iXY(0, 0), Palette::colorTable4060);
        }
        //else if (mapDrawType == MAP_BLEND_GREEN)
        //{
        //clientArea.bltLookup(iRect(iXY(0, 0), getSize()), Palette::green256.getColorArray());
        //}
        else if (mapDrawType == MAP_BLEND_GRAY) {
            clientArea.bltLookup(iRect(iXY(0, 0), getSize()), Palette::gray256.getColorArray());
        } else if (mapDrawType == MAP_BLEND_DARK_GRAY) {
            clientArea.bltLookup(iRect(iXY(0, 0), getSize()), Palette::darkGray256.getColorArray());
        } else if (mapDrawType == MAP_BLACK) {
            clientArea.fill(Color::black);
        } else if (mapDrawType == MAP_TRANSPARENT) {}
    }

    // Draw a hairline border.
    //viewArea.drawRect(Color::white);
    viewArea.drawLookupBorder(Palette::darkGray256.getColorArray());

    // Draw the world view box.
    iRect boxpos = MiniMapInterface::getWorldWindow();
    boxpos.translate(iXY(1,1));                           
    
    clientArea.bltLookup(boxpos, Palette::darkGray256.getColorArray());
    // Draw the units and such on the minimap.
    MiniMapInterface::annotateMiniMap((Surface &) clientArea);
    // Draw the world view box corners.
    clientArea.drawBoxCorners(boxpos, 5, Color::white);

    // If the mouse is over the client area, then change the cursor.
    if (getClientRect().contains(getScreenToClientPos(mouse.getScreenPos()))) {
        if (selectionAnchor) {
            // Since we are selecting units, draw the selection box.
            clientArea.drawRect(selectionAnchorDownPos, selectionAnchorCurPos, Color::white);
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
void MiniMapView::drawMouseBox(const Surface &dest)
{
    assert(this != 0);
    assert(dest.getDoesExist());

    iRect r(MiniMapInterface::getWorldWindow());
    iXY   size(r.getSize() / 2);
    iXY   pos(getScreenToClientPos(mouse.getScreenPos()));

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

    if (getClientRect().contains(getScreenToClientPos(mouse.getScreenPos()))) {
        if (!selectionAnchor) {
            setViewWindow(pos);
        }
    }

    GameTemplateView::lMouseDown(pos);

} // end MiniMapView::lMouseDown

// rMouseDown
//--------------------------------------------------------------------------
void MiniMapView::rMouseDown(const iXY &pos)
{
    MiniMapInterface::deselectUnits();

} // end MiniMapView::rMouseDown

//--------------------------------------------------------------------------
void MiniMapView::rMouseDrag(const iXY &downPos, const iXY &prevPos, const iXY &newPos)
{
    // Let the map go up to the min screen dimension.
    //maxMapSize = std::min(SCREEN_XPIX, SCREEN_YPIX);
    maxMapSize = std::min(640, 480);

    moveTo(min + newPos - prevPos);
    checkArea(screen->getPix());

    // Check for map blending mode change.
    if (KeyboardInterface::getKeyPressed(SDLK_1)) {
        MiniMapView::mapDrawType = MAP_SOLID;
    } else if (KeyboardInterface::getKeyPressed(SDLK_2)) {
        MiniMapView::mapDrawType = MAP_2080;
    } else if (KeyboardInterface::getKeyPressed(SDLK_3)) {
        MiniMapView::mapDrawType = MAP_4060;
    } else if (KeyboardInterface::getKeyPressed(SDLK_4)) {
        MiniMapView::mapDrawType = MAP_BLEND_GRAY;
    } else if (KeyboardInterface::getKeyPressed(SDLK_5)) {
        MiniMapView::mapDrawType = MAP_BLEND_DARK_GRAY;
    } else if (KeyboardInterface::getKeyPressed(SDLK_6)) {
        MiniMapView::mapDrawType = MAP_BLACK;
    } else if (KeyboardInterface::getKeyPressed(SDLK_7)) {
        MiniMapView::mapDrawType = MAP_TRANSPARENT;
    }

    if (KeyboardInterface::getKeyState(SDLK_KP_PLUS)) {
        increaseSize = -1;
    } else if (KeyboardInterface::getKeyState(SDLK_KP_MINUS)) {
        decreaseSize = -1;
    }

} // end MiniMapView::rMouseDrag

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
    if ((min.x + destSize.x) >= screen->getPixX()) {
        int xOffset = min.x + destSize.x - screen->getPixX();

        int destXPos = min.x - xOffset;

        if (destXPos < 0) {
            moveTo(0, min.y);

        } else {
            moveTo(destXPos, min.y);
        }
    }

    // Check the validity of the Y dimension.
    if ((min.y + destSize.y) >= screen->getPixY()) {
        int yOffset = min.y + destSize.y - screen->getPixY();

        int destYPos = min.y - yOffset;

        if (destYPos < 0) {
            moveTo(min.x, 0);

        } else {
            moveTo(min.x, destYPos);
        }
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
    if (getClientRect().contains(getScreenToClientPos(mouse.getScreenPos()))) {
        // If units are selected, send the units there and deselect the units.
        // If there is a unit selected, see if we should move a unit there.
        if (	(KeyboardInterface::getKeyState(SDLK_LCTRL) ||
                KeyboardInterface::getKeyState(SDLK_RCTRL))) {
            if (MiniMapInterface::isUnitSelected()) {
                if (MiniMapInterface::isValidUnitMove(upPos)) {
                    MiniMapInterface::moveUnits(upPos);
                }
            }
        }
    }

    return View::lMouseUp(downPos, upPos);

} // end MiniMapView::lMouseUp

// mouseMove
//--------------------------------------------------------------------------
void MiniMapView::mouseMove(const iXY &prevPos, const iXY &newPos)
{
    if (getClientRect().contains(getScreenToClientPos(mouse.getScreenPos()))) {
        if (	(KeyboardInterface::getKeyState(SDLK_LCTRL) ||
                KeyboardInterface::getKeyState(SDLK_RCTRL))) {
            // Set the cursor accordinly.
            if (MiniMapInterface::isUnitSelected()) {
                if (MiniMapInterface::isValidUnitMove(newPos)) {
                    MouseInterface::setCursor("move.bmp");
                } else {
                    MouseInterface::setCursor("noentry.bmp");
                }
            } else {
                MouseInterface::setCursor("default.bmp");
            }

            if (!selectionAnchor) {
                selectionAnchorDownPos = newPos;
            }

            selectionAnchorCurPos = newPos;
            selectionAnchor       = true;

            // Set the selection cursor.
            MouseInterface::setCursor("select.bmp");

        } else {
            if (selectionAnchor) {
                MiniMapInterface::selectUnits(iRect(selectionAnchorDownPos.x, selectionAnchorDownPos.y, selectionAnchorCurPos.x, selectionAnchorCurPos.y));
            }

            selectionAnchor = false;
        }
    }

    GameTemplateView::mouseMove(prevPos, newPos);

    //enum { _mcursor_default,
    //       _mcursor_noentry,
    //       _mcursor_move,
    //       _mcursor_select,
    //       _mcursor_target,
    //       _mcursor_make_allie,
    //       _mcursor_break_allie };
    //
    //	}

    //static bool isUnitSelected( void );
    //static bool selectUnits( iRect bound_box );
    //static void moveUnits( iXY location );

} // end MiniMapView::mouseMove

// lMouseDrag
//--------------------------------------------------------------------------
void MiniMapView::lMouseDrag(const iXY &downPos, const iXY &prevPos, const iXY &newPos)
{
    lMouseDown(newPos);

} // end MiniMapView::lMouseDrag
