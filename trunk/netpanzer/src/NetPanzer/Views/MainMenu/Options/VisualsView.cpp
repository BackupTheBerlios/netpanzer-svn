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
#include "UILib/UIDraw.hpp"
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
    int minWidth = 19 * CHAR_XPIX;

    x = xTextStart + 10;
    y = 100;
    choiceResolution.setName("Resolution");
    choiceResolution.setStateChangedCallback(this);
    choiceResolution.addItem("640x480");
    choiceResolution.addItem("800x600");
    choiceResolution.addItem("1024x768");
    choiceResolution.setLocation(x, y);
    choiceResolution.select(GameConfig::getScreenResolution());
    choiceResolution.setMinWidth(minWidth);

    checkBoxFullscreen.setLabel("Fullscreen");
    checkBoxFullscreen.setStateChangedCallback(this);
    checkBoxFullscreen.setState(Screen->isFullScreen());    
    checkBoxFullscreen.setLocation(x+ 200, y);
    y += yOffset;
    y += yOffset;

    choiceMiniMapUnitSize.setName("Mini Map Unit Size");
    choiceMiniMapUnitSize.setStateChangedCallback(this);
    choiceMiniMapUnitSize.addItem("Small");
    choiceMiniMapUnitSize.addItem("Large");
    choiceMiniMapUnitSize.setLocation(x, y);
    choiceMiniMapUnitSize.select(GameConfig::getMiniMapUnitSize());
    choiceMiniMapUnitSize.setMinWidth(minWidth);
    y += yOffset;
    y += yOffset;


#if 0
    choiceGameViewBackgroundColor.setName("Game View Background Color");
    choiceGameViewBackgroundColor.setStateChangedCallback(this);
    choiceGameViewBackgroundColor.addItemDefault("Dark Gray Blend");
    choiceGameViewBackgroundColor.addItem("Light Gray Blend");
    choiceGameViewBackgroundColor.addItem("Solid Black");
    choiceGameViewBackgroundColor.addItem("Transparent");
    choiceGameViewBackgroundColor.setLocation(x, y);
    choiceGameViewBackgroundColor.setMinWidth(minWidth);
    y += yOffset;
    y += yOffset;

    choiceMiniMapObjectiveDrawMode.setName("Mini Map Objective Draw Mode");
    choiceMiniMapObjectiveDrawMode.setStateChangedCallback(this);
    choiceMiniMapObjectiveDrawMode.addItemDefault("Outlined");
    choiceMiniMapObjectiveDrawMode.addItem("Solid");
    choiceMiniMapObjectiveDrawMode.setLocation(x, y);
    choiceMiniMapObjectiveDrawMode.setMinWidth(minWidth);
    y += yOffset;
    y += yOffset;

    x = 300;
    y = 100;
    choiceUnitSelectionDrawMode.setName("Unit Selection Draw Mode");
    choiceUnitSelectionDrawMode.setStateChangedCallback(this);
    choiceUnitSelectionDrawMode.addItemDefault("Rectangle Edges");
    choiceUnitSelectionDrawMode.addItem("Rectangle Outline");
    choiceUnitSelectionDrawMode.setLocation(x, y);
    choiceUnitSelectionDrawMode.setMinWidth(minWidth);
    y += yOffset;
    y += yOffset;

    //choiceUnitInfoDrawLayer.setName("Unit Information Draw Layer");
    //choiceUnitInfoDrawLayer.setStateChangedCallback(this);
    //choiceUnitInfoDrawLayer.addItemDefault("Bottom");
    //choiceUnitInfoDrawLayer.addItem("Top");
    //choiceUnitInfoDrawLayer.setLocation(x, y);
    //choiceUnitInfoDrawLayer.setMinWidth(minWidth);
    //y += yOffset;
    //y += yOffset;

    //x = xTextStart;
    //addLabel(iXY(x, y), "Brightness", Color::white);
    //x = optionsMeterStartX;
    //addButtonCenterText(iXY(x - 1, y), arrowButtonWidth, "<", "", bDecreaseBrightness);
    //x += optionsMeterWidth + arrowButtonWidth;
    //addButtonCenterText(iXY(x + 1, y), arrowButtonWidth, ">", "", bIncreaseBrightness);
    //y += yOffset;
#endif

    // Other visual options to add.
    // Gamma
    // Blend Mouse.

    // Color Settings
    //----------------------------------------------------------------------
#if 0
    minWidth = 13 * CHAR_XPIX;

    x = xTextStart + 10;
    y = 100 + 110;
    choiceYourRadarUnit.setName("Your Radar Unit");
    choiceYourRadarUnit.setStateChangedCallback(this);
    choiceYourRadarUnit.addItem("Aqua");
    choiceYourRadarUnit.addItem("Blue");
    choiceYourRadarUnit.addItem("Dark Blue");
    choiceYourRadarUnit.addItem("Blue Gray");
    choiceYourRadarUnit.addItem("Red");
    choiceYourRadarUnit.addItem("Dark Red");
    choiceYourRadarUnit.addItem("Light Green");
    choiceYourRadarUnit.addItem("Green");
    choiceYourRadarUnit.addItem("Dark Green");
    choiceYourRadarUnit.addItem("Yellow");
    choiceYourRadarUnit.addItem("Light Orange");
    choiceYourRadarUnit.addItem("Orange");
    choiceYourRadarUnit.addItem("Black");
    choiceYourRadarUnit.addItem("White");
    choiceYourRadarUnit.addItem("Gray");
    choiceYourRadarUnit.addItem("Dark Gray");
    choiceYourRadarUnit.setLocation(x, y);
    choiceYourRadarUnit.setMinWidth(minWidth);
    choiceYourRadarUnit.select(GameConfig::getPlayerRadarUnitColorEnum());
    y += yOffset;
    y += yOffset;

    choiceAlliedRadarUnit.setName("Allied Radar Unit");
    choiceAlliedRadarUnit.setStateChangedCallback(this);
    choiceAlliedRadarUnit.copyItems(choiceYourRadarUnit);
    choiceAlliedRadarUnit.setLocation(x, y);
    choiceAlliedRadarUnit.setMinWidth(minWidth);
    choiceAlliedRadarUnit.select(GameConfig::getAlliedRadarUnitColorEnum());
    y += yOffset;
    y += yOffset;

    choiceYourRadarObjective.setName("Your Radar Objective");
    choiceYourRadarObjective.setStateChangedCallback(this);
    choiceYourRadarObjective.copyItems(choiceYourRadarUnit);
    choiceYourRadarObjective.setLocation(x, y);
    choiceYourRadarObjective.setMinWidth(minWidth);
    choiceYourRadarObjective.select(GameConfig::getPlayerOutpostRadarColorEnum());
    y += yOffset;
    y += yOffset;

    choiceAlliedRadarObjective.setName("Allied Radar Objective");
    choiceAlliedRadarObjective.setStateChangedCallback(this);
    choiceAlliedRadarObjective.copyItems(choiceYourRadarUnit);
    choiceAlliedRadarObjective.setLocation(x, y);
    choiceAlliedRadarObjective.setMinWidth(minWidth);
    choiceAlliedRadarObjective.select(GameConfig::getAlliedOutpostRadarColorEnum());
    y += yOffset;
    y += yOffset;

    x = 300;
    y = 100 + 110;
    choiceEnemyRadarObjective.setName("Enemy Radar Objective");
    choiceEnemyRadarObjective.setStateChangedCallback(this);
    choiceEnemyRadarObjective.copyItems(choiceYourRadarUnit);
    choiceEnemyRadarObjective.setLocation(x, y);
    choiceEnemyRadarObjective.setMinWidth(minWidth);
    choiceEnemyRadarObjective.select(GameConfig::getEnemyOutpostRadarColorEnum());
    y += yOffset;
    y += yOffset;

    choiceVehicleSelectionBox.setName("Vehicle Selection Box");
    choiceVehicleSelectionBox.setStateChangedCallback(this);
    choiceVehicleSelectionBox.copyItems(choiceYourRadarUnit);
    choiceVehicleSelectionBox.setLocation(x, y);
    choiceVehicleSelectionBox.setMinWidth(minWidth);
    choiceVehicleSelectionBox.select(GameConfig::getVehicleSelectionBoxColorEnum());
    y += yOffset;
    y += yOffset;

    choiceConsoleText.setName("Console Text");
    choiceConsoleText.setStateChangedCallback(this);
    choiceConsoleText.copyItems(choiceYourRadarUnit);
    choiceConsoleText.setLocation(x, y);
    choiceConsoleText.setMinWidth(minWidth);
    choiceConsoleText.select(GameConfig::getConsoleTextColorEnum());
    y += yOffset;
    y += yOffset;

    x = 300;
    y = 344;
#endif

    checkBoxDrawAllShadows.setLabel("Draw All Shadows");
    checkBoxDrawAllShadows.setStateChangedCallback(this);
    checkBoxDrawAllShadows.setState(GameConfig::getDisplayShadowsFlag());
    checkBoxDrawAllShadows.setLocation(x, y);
    y += yOffset;

    checkBoxBlendSmoke.setLabel("Blend Smoke");
    checkBoxBlendSmoke.setStateChangedCallback(this);
    checkBoxBlendSmoke.setState(GameConfig::getBlendSmoke());
    checkBoxBlendSmoke.setLocation(x, y);
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
void VisualsView::doDraw(const Surface &viewArea, const Surface &clientArea)
{
    MenuTemplateView::doDraw(viewArea, clientArea);

    View::doDraw(viewArea, clientArea);

} // end VisualsView::doDraw

// processEvents
//---------------------------------------------------------------------------
void VisualsView::processEvents()
{
    OptionsTemplateView::processEvents();
#if 0
    GameConfig::setPlayerRadarUnitColor(choiceYourRadarUnit.getSelectedIndex());
    GameConfig::setAlliedRadarUnitColor(choiceAlliedRadarUnit.getSelectedIndex());
    GameConfig::setPlayerOutpostRadarColor(choiceYourRadarObjective.getSelectedIndex());
    GameConfig::setAlliedOutpostRadarColor(choiceAlliedRadarObjective.getSelectedIndex());
    GameConfig::setEnemyOutpostRadarColor(choiceEnemyRadarObjective.getSelectedIndex());
    GameConfig::setVehicleSelectionBoxColor(choiceVehicleSelectionBox.getSelectedIndex());
    GameConfig::setConsoleTextColor(choiceConsoleText.getSelectedIndex());

    GameManager::setNetPanzerGameOptions();
#endif

} // end VisualsView::processEvents

// loadTitleSurface
//---------------------------------------------------------------------------
void VisualsView::loadTitleSurface()
{
    doLoadTitleSurface("pics/backgrounds/menus/menu/til/visualsTitle.til");
} // end VisualsView::loadTitleSurface

// actionPerformed
//---------------------------------------------------------------------------
void VisualsView::stateChanged(Component* source)
{
    // Check Box Draw All Shadows
    if (source == &checkBoxDrawAllShadows) {
        GameConfig::setDisplayShadowsFlag(checkBoxDrawAllShadows.getState());
    }
    // Check Box Blend Smoke
    else if (source == &checkBoxBlendSmoke) {
        if (checkBoxBlendSmoke.getState()) {
            GameConfig::setBlendSmokeTrue();
        } else {
            GameConfig::setBlendSmokeFalse();
        }
    } else if (source == &checkBoxFullscreen) {
        GameConfig::setFullscreen(checkBoxFullscreen.getState());
        GameManager::setVideoMode();
    }
    // Choice Resolution
    else if (source == &choiceResolution) {
        GameConfig::setScreenResolution(choiceResolution.getSelectedIndex());
        GameManager::setVideoMode();
    }
    // Choice Game View Background Color
    else if (source == &choiceGameViewBackgroundColor) {
        if (choiceGameViewBackgroundColor.getSelectedIndex() == 0) {
            viewDrawBackgroundMode = VIEW_BACKGROUND_DARK_GRAY_BLEND;
        } else if (choiceGameViewBackgroundColor.getSelectedIndex() == 1) {
            viewDrawBackgroundMode = VIEW_BACKGROUND_LIGHT_GRAY_BLEND;
        } else if (choiceGameViewBackgroundColor.getSelectedIndex() == 2) {
            viewDrawBackgroundMode = VIEW_BACKGROUND_SOLID_BLACK;
        } else if (choiceGameViewBackgroundColor.getSelectedIndex() == 3) {
            viewDrawBackgroundMode = VIEW_BACKGROUND_TRANSPARENT;
        }
    }
    // Choice MiniMap Objective Draw Mode
    else if (source == &choiceMiniMapObjectiveDrawMode) {
        if (choiceMiniMapObjectiveDrawMode.getSelectedIndex() == 0) {
            GameConfig::setMiniMapObjectiveDrawMode(_mini_map_objective_draw_mode_outline_rect);
        } else if (choiceMiniMapObjectiveDrawMode.getSelectedIndex() == 1) {
            GameConfig::setMiniMapObjectiveDrawMode(_mini_map_objective_draw_mode_solid_rect);
        }
    }
    // Choice Mini Map Unit Size
    else if (source == &choiceMiniMapUnitSize) {
        if (choiceMiniMapUnitSize.getSelectedIndex() == 0) {
            GameConfig::setMiniMapUnitSize(_mini_map_unit_size_small);
        } else if (choiceMiniMapUnitSize.getSelectedIndex() == 1) {
            GameConfig::setMiniMapUnitSize(_mini_map_unit_size_large);
        }
    }
    // Choice Unit Selection Draw Mode
    else if (source == &choiceUnitSelectionDrawMode) {
        if (choiceUnitSelectionDrawMode.getSelectedIndex() == 0) {
            GameConfig::setUnitSelectionBoxDrawMode(_unit_selection_box_draw_mode_rect_edges);
        } else if (choiceUnitSelectionDrawMode.getSelectedIndex() == 1) {
            GameConfig::setUnitSelectionBoxDrawMode(_unit_selection_box_draw_mode_rect);
        }
    }
    // Choice Unit Info Draw Layer
    //else if (source == &choiceUnitInfoDrawLayer)
    //{
    //	if (choiceUnitInfoDrawLayer.getSelectedIndex() == 0)
    //	{
    //		GameConfig::setUnitInfoDrawLayerBottom();
    //	}
    //	else if (choiceUnitInfoDrawLayer.getSelectedIndex() == 1)
    //	{
    //		GameConfig::setUnitInfoDrawLayerTop();
    //	}
    //}
}

