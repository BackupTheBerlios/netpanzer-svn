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


#include "VisualsView.hpp"
#include "Particles/ParticleSystemGlobals.hpp"
#include "Interfaces/GameConfig.hpp"
#include "Interfaces/GameManager.hpp"
#include "Views/GameViewGlobals.hpp"
#include "Views/Components/Desktop.hpp"

// VisualsView
//---------------------------------------------------------------------------
VisualsView::VisualsView() : OptionsTemplateView()
{
    setSearchName("VisualsView");
    setTitle("Visual Options");
    setSubTitle("");

    selectedColorStartY = 0;

    initButtons();

} // end VisualsView::VisualsView

// initButtons
//---------------------------------------------------------------------------
void VisualsView::initButtons()
{
    OptionsTemplateView::initButtons(); // XXX this clears buttons and delete components
    
    int xTextStart       = bodyTextRect.min.x;
    int x                = xTextStart + 10;
    int y                = bodyTextRect.min.y;
    int yOffset          =  20;
    
    // Settings
    //----------------------------------------------------------------------
    int minWidth = 19 * 8;
    
    x = xTextStart + 10;
    y = 100;
    
    char res_str[20];
    choiceResolution = new Choice();
    choiceResolution->setLabel("Resolution");
    choiceResolution->addItem("current");
    SDL_Rect** modes = SDL_ListModes(0, SDL_FULLSCREEN);
    int cur_mode = 0;
    if ( modes && modes != (SDL_Rect**)-1 )
    {
        while ( modes[cur_mode] )
        {
            snprintf(res_str,sizeof(res_str),"%dx%d", modes[cur_mode]->w, modes[cur_mode]->h);
            res_str[sizeof(res_str)-1] = 0;
            choiceResolution->addItem(res_str);
            ++cur_mode;
        }
    }

    choiceResolution->setLocation(x, y);
    choiceResolution->select(0);
    choiceResolution->setMinWidth(minWidth);
    choiceResolution->stateChanged.connect(this, &VisualsView::onResolutionChoiceModified);
    add(choiceResolution);
    
    current_width = 0;
    current_height = 0;

    checkBoxFullscreen = new CheckBox();
    checkBoxFullscreen->setLabel("Fullscreen");
    checkBoxFullscreen->setLocation(x+ 200, y);
    checkBoxFullscreen->stateChanged.connect(this, &VisualsView::onFullscreenCheckBoxModified);
    add(checkBoxFullscreen);
    y += yOffset;
    y += yOffset;
    
    choiceMiniMapUnitSize = new Choice();
    choiceMiniMapUnitSize->setLabel("Mini Map Unit Size");
    choiceMiniMapUnitSize->addItem("Small");
    choiceMiniMapUnitSize->addItem("Large");
    choiceMiniMapUnitSize->setLocation(x, y);
    choiceMiniMapUnitSize->select(gameconfig->radar_unitsize);
    choiceMiniMapUnitSize->setMinWidth(minWidth);
    choiceMiniMapUnitSize->stateChanged.connect(this, &VisualsView::onMiniMapUnitSizeChoiceModified);
    add(choiceMiniMapUnitSize);
    y += yOffset;
    y += yOffset;
    
    
    // Other visual options to add.
    // Gamma
    // Blend Mouse.
    
    checkBoxDrawAllShadows = new CheckBox();
    checkBoxDrawAllShadows->setLabel("Draw All Shadows");
    checkBoxDrawAllShadows->setState(GameConfig::video_shadows);
    checkBoxDrawAllShadows->setLocation(x, y);
    checkBoxDrawAllShadows->stateChanged.connect(this, &VisualsView::onDrawAllShadowsCheckBoxModified);
    add(checkBoxDrawAllShadows);
    y += yOffset;
    
    checkBoxBlendSmoke = new CheckBox();
    checkBoxBlendSmoke->setLabel("Blend Smoke");
    checkBoxBlendSmoke->setState(GameConfig::video_blendsmoke);
    checkBoxBlendSmoke->setLocation(x, y);
    checkBoxBlendSmoke->stateChanged.connect(this, &VisualsView::onBlendSmokeCheckBoxModified);
    add(checkBoxBlendSmoke);
    y += yOffset;

#ifdef _WIN32
    checkBoxUseDirectX = new CheckBox();
    checkBoxUseDirectX->setLabel("Use DirectX");
    checkBoxUseDirectX->setState(GameConfig::video_usedirectx);
    checkBoxUseDirectX->setLocation(x, y);
    checkBoxUseDirectX->stateChanged.connect(this, &VisualsView::onUseDirectXCheckBoxModified);
    add(checkBoxUseDirectX);
    y += yOffset;
#endif
    
    //removeAllButtons();
    //removeComponents();

    //add(&choiceGameViewBackgroundColor);
    //add(&choiceMiniMapObjectiveDrawMode);
    
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
    checkBoxFullscreen->setState(GameConfig::video_fullscreen);
    checkBoxBlendSmoke->setState(GameConfig::video_blendsmoke);
    checkBoxDrawAllShadows->setState(GameConfig::video_shadows);

    MenuTemplateView::doDraw(viewArea, clientArea);

//    View::doDraw(viewArea, clientArea);

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
    doLoadTitleSurface("visualsTitle");
} // end VisualsView::loadTitleSurface

void VisualsView::onFullscreenCheckBoxModified()
{
    GameConfig::video_fullscreen = checkBoxFullscreen->getState();
    GameManager::setVideoMode();
}

void VisualsView::onDrawAllShadowsCheckBoxModified()
{
    GameConfig::video_shadows = checkBoxDrawAllShadows->getState();
}

void VisualsView::onBlendSmokeCheckBoxModified()
{
    GameConfig::video_blendsmoke = checkBoxBlendSmoke->getState();
}

#ifdef _WIN32
void VisualsView::onUseDirectXCheckBoxModified()
{
    GameConfig::video_usedirectx = checkBoxUseDirectX->getState();
}
#endif

void VisualsView::onResolutionChoiceModified()
{
    int sel_index = choiceResolution->getSelectedIndex()-1;
    if ( sel_index < 0 )
    {
        return;
    }

    SDL_Rect** modes = SDL_ListModes(0, SDL_FULLSCREEN);
    SDL_Rect* mode = 0;
    if ( modes && modes != (SDL_Rect**)-1 )
    {
        mode = modes[sel_index];
    }

    if ( mode )
    {
        GameConfig::video_width = mode->w;
        GameConfig::video_height = mode->h;
    }

    if ( sel_index == 0 && ! GameConfig::video_fullscreen )
    {
        // on Mac crash if we are in window and we select the biggest
        // resolution (the first one in theory), we make it smaller so it
        // wont crash, it is a SDL error.
        GameConfig::video_height -= 50;
    }

    GameManager::setVideoMode();
}

void VisualsView::onMiniMapUnitSizeChoiceModified()
{
    if (choiceMiniMapUnitSize->getSelectedIndex() == 0)
    {
        gameconfig->radar_unitsize = _mini_map_unit_size_small;
    }
    else if (choiceMiniMapUnitSize->getSelectedIndex() == 1)
    {
        gameconfig->radar_unitsize = _mini_map_unit_size_large;
    }
}

