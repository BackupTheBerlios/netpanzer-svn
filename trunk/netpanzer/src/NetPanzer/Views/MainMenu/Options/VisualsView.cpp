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

#include "VisualsView.hpp"
#include "ParticleSystemGlobals.hpp"
#include "GameConfig.hpp"
#include "GameManager.hpp"
#include "GameViewGlobals.hpp"
#include "Desktop.hpp"

// VisualsView
//---------------------------------------------------------------------------
VisualsView::VisualsView() : OptionsTemplateView()
{
    setSearchName("VisualsView");
    setTitle("Visual Options");
    setSubTitle("");

    selectedColorStartY = 0;

    initButtons();

    int xTextStart       = bodyTextRect.min.x;
    int x                = xTextStart + 10;
    int y                = bodyTextRect.min.y;
    int yOffset          =  20;

    // Settings
    //----------------------------------------------------------------------
    int minWidth = 19 * 8;

    x = xTextStart + 10;
    y = 100;
    choiceResolution.setName("Resolution");
    choiceResolution.addItem("640x480");
    choiceResolution.addItem("800x600");
    choiceResolution.addItem("1024x768");
    choiceResolution.setLocation(x, y);
    choiceResolution.select(gameconfig->screenresolution);
    choiceResolution.setMinWidth(minWidth);
    choiceResolution.setStateChangedCallback(this);

    checkBoxFullscreen.setLabel("Fullscreen");
    checkBoxFullscreen.setState(gameconfig->fullscreen);    
    checkBoxFullscreen.setLocation(x+ 200, y);
    checkBoxFullscreen.setStateChangedCallback(this);
    y += yOffset;
    y += yOffset;

    choiceMiniMapUnitSize.setName("Mini Map Unit Size");
    choiceMiniMapUnitSize.addItem("Small");
    choiceMiniMapUnitSize.addItem("Large");
    choiceMiniMapUnitSize.setLocation(x, y);
    choiceMiniMapUnitSize.select(gameconfig->radar_unitsize);
    choiceMiniMapUnitSize.setMinWidth(minWidth);
    choiceMiniMapUnitSize.setStateChangedCallback(this);
    y += yOffset;
    y += yOffset;


    // Other visual options to add.
    // Gamma
    // Blend Mouse.

    checkBoxDrawAllShadows.setLabel("Draw All Shadows");
    checkBoxDrawAllShadows.setState(gameconfig->displayshadows);
    checkBoxDrawAllShadows.setLocation(x, y);
    checkBoxDrawAllShadows.setStateChangedCallback(this);
    y += yOffset;

    checkBoxBlendSmoke.setLabel("Blend Smoke");
    checkBoxBlendSmoke.setState(gameconfig->blendsmoke);
    checkBoxBlendSmoke.setLocation(x, y);
    checkBoxBlendSmoke.setStateChangedCallback(this);
    y += yOffset;
} // end VisualsView::VisualsView

// initButtons
//---------------------------------------------------------------------------
void VisualsView::initButtons()
{
    OptionsTemplateView::initButtons();

    add(&checkBoxDrawAllShadows);
    add(&checkBoxBlendSmoke);
    add(&checkBoxFullscreen);
    add(&choiceResolution);
    //add(&choiceGameViewBackgroundColor);
    //add(&choiceMiniMapObjectiveDrawMode);
    add(&choiceMiniMapUnitSize);
    //add(&choiceUnitSelectionDrawMode);
    //add(&choiceUnitInfoDrawLayer);
    //add(&choiceYourRadarUnit);
    //add(&choiceAlliedRadarUnit);
    //add(&choiceYourRadarObjective);
    //add(&choiceAlliedRadarObjective);
    //add(&choiceEnemyRadarObjective);
    //add(&choiceVehicleSelectionBox);
    //add(&choiceConsoleText);
} // end VisualsView::initButtons

// doDraw
//---------------------------------------------------------------------------
void VisualsView::doDraw(Surface &viewArea, Surface &clientArea)
{
    MenuTemplateView::doDraw(viewArea, clientArea);

    View::doDraw(viewArea, clientArea);

} // end VisualsView::doDraw

// processEvents
//---------------------------------------------------------------------------
void VisualsView::processEvents()
{
    OptionsTemplateView::processEvents();
} // end VisualsView::processEvents

// loadTitleSurface
//---------------------------------------------------------------------------
void VisualsView::loadTitleSurface()
{
    doLoadTitleSurface("pics/backgrounds/menus/menu/visualsTitle.bmp");
} // end VisualsView::loadTitleSurface

// actionPerformed
//---------------------------------------------------------------------------
void VisualsView::stateChanged(Component* source)
{
    // Check Box Draw All Shadows
    if (source == &checkBoxDrawAllShadows) {
        gameconfig->displayshadows = checkBoxDrawAllShadows.getState();
    }
    // Check Box Blend Smoke
    else if (source == &checkBoxBlendSmoke) {
        gameconfig->blendsmoke = checkBoxBlendSmoke.getState();
    } else if (source == &checkBoxFullscreen) {
        gameconfig->fullscreen = checkBoxFullscreen.getState();
        GameManager::setVideoMode();
    }
    // Choice Resolution
    else if (source == &choiceResolution) {
        gameconfig->screenresolution = choiceResolution.getSelectedIndex();
        GameManager::setVideoMode();
    }
    // Choice Mini Map Unit Size
    else if (source == &choiceMiniMapUnitSize) {
        if (choiceMiniMapUnitSize.getSelectedIndex() == 0) {
            gameconfig->radar_unitsize = _mini_map_unit_size_small;
        } else if (choiceMiniMapUnitSize.getSelectedIndex() == 1) {
            gameconfig->radar_unitsize = _mini_map_unit_size_large;
        }
    }
}

