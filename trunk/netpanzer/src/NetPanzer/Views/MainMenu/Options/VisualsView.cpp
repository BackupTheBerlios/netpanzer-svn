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
    choiceResolution->setName("Resolution");
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
    choiceResolution->setStateChangedCallback(this);
    add(choiceResolution);

    current_width = 0;
    current_height = 0;
    
    checkBoxFullscreen = new CheckBox();
    checkBoxFullscreen->setLabel("Fullscreen");
    checkBoxFullscreen->setState(GameConfig::video_fullscreen);
    checkBoxFullscreen->setLocation(x+ 250, y);
    checkBoxFullscreen->setStateChangedCallback(this);
    add(checkBoxFullscreen);
    y += yOffset;
    y += yOffset;
    
    choiceMiniMapUnitSize = new Choice();
    choiceMiniMapUnitSize->setName("Mini Map Unit Size");
    choiceMiniMapUnitSize->addItem("Small");
    choiceMiniMapUnitSize->addItem("Large");
    choiceMiniMapUnitSize->setLocation(x, y);
    choiceMiniMapUnitSize->select(gameconfig->radar_unitsize);
    choiceMiniMapUnitSize->setMinWidth(minWidth);
    choiceMiniMapUnitSize->setStateChangedCallback(this);
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
    checkBoxDrawAllShadows->setStateChangedCallback(this);
    add(checkBoxDrawAllShadows);
    y += yOffset;
    
    checkBoxBlendSmoke = new CheckBox();
    checkBoxBlendSmoke->setLabel("Blend Smoke");
    checkBoxBlendSmoke->setState(GameConfig::video_blendsmoke);
    checkBoxBlendSmoke->setLocation(x, y);
    checkBoxBlendSmoke->setStateChangedCallback(this);
    add(checkBoxBlendSmoke);
    y += yOffset;

    checkBoxDrawHitpoints = new CheckBox();
    checkBoxDrawHitpoints->setLabel("Draw Hitpoints Bar");
    checkBoxDrawHitpoints->setState(gameconfig->drawunitdamage);
    checkBoxDrawHitpoints->setLocation(x, y);
    checkBoxDrawHitpoints->setStateChangedCallback(this);
    add(checkBoxDrawHitpoints);
    y += yOffset;

    checkBoxDrawFlags = new CheckBox();
    checkBoxDrawFlags->setLabel("Draw Flags");
    checkBoxDrawFlags->setState(gameconfig->drawunitflags);
    checkBoxDrawFlags->setLocation(x, y);
    checkBoxDrawFlags->setStateChangedCallback(this);
    add(checkBoxDrawFlags);
    y += yOffset;

    checkBoxDrawNames = new CheckBox();
    checkBoxDrawNames->setLabel("Draw Player Names");
    checkBoxDrawNames->setState(gameconfig->drawunitowner);
    checkBoxDrawNames->setLocation(x, y);
    checkBoxDrawNames->setStateChangedCallback(this);
    add(checkBoxDrawNames);
    y += yOffset;

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
void VisualsView::doDraw()
{
    checkBoxFullscreen->setState(GameConfig::video_fullscreen);
    checkBoxBlendSmoke->setState(GameConfig::video_blendsmoke);
    checkBoxDrawAllShadows->setState(GameConfig::video_shadows);

    if (   current_width != GameConfig::video_width
        || current_height != GameConfig::video_height )
    {
        current_width = GameConfig::video_width;
        current_height = GameConfig::video_height;
        char new_res[100];
        snprintf(new_res,sizeof(new_res),"current(%d x %d)", current_width, current_height);
        choiceResolution->updateItem(new_res,0);
    }

    MenuTemplateView::doDraw();

    View::doDraw();

} // end VisualsView::doDraw

// loadTitleSurface
//---------------------------------------------------------------------------
void VisualsView::loadTitleSurface()
{
    doLoadTitleSurface("visualsTitle");
} // end VisualsView::loadTitleSurface

// actionPerformed
//---------------------------------------------------------------------------
void VisualsView::stateChanged(Component* source)
{
    if ( source == checkBoxDrawAllShadows )
    {
        GameConfig::video_shadows = checkBoxDrawAllShadows->getState();
    }
    else if ( source == checkBoxBlendSmoke )
    {
        GameConfig::video_blendsmoke = checkBoxBlendSmoke->getState();
    }
    else if ( source == checkBoxDrawHitpoints )
    {
        gameconfig->drawunitdamage = checkBoxDrawHitpoints->getState();
    }
    else if ( source == checkBoxDrawFlags )
    {
        gameconfig->drawunitflags = checkBoxDrawFlags->getState();
    }
    else if ( source == checkBoxDrawNames )
    {
        gameconfig->drawunitowner = checkBoxDrawNames->getState();
    }
    else if ( source == checkBoxFullscreen )
    {
        GameConfig::video_fullscreen = checkBoxFullscreen->getState();
        GameManager::setVideoMode();
    }
    else if ( source == choiceResolution )
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
    else if ( source == choiceMiniMapUnitSize )
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
}
