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
#ifndef __OptionsTemplateView_hpp__
#define __OptionsTemplateView_hpp__

#include "Views/Components/View.hpp"
#include "2D/Surface.hpp"
#include "Views/MainMenu/MenuTemplateView.hpp"
#include "Views/Components/CheckBox.hpp"
#include "Views/Components/Choice.hpp"

//--------------------------------------------------------------------------
class OptionsTemplateView : public MenuTemplateView
{
private:
    int      selectedColorStartY;

    // Option check boxes.
    CheckBox * checkBoxDrawAllShadows;
    CheckBox * checkBoxBlendSmoke;
    CheckBox * checkBoxFullscreen;
    CheckBox * checkBoxSoundEnabled;
    CheckBox * checkBoxMusicEnabled;
#ifdef _WIN32
    CheckBox * checkBoxUseDirectX;
#endif
    //CheckBox drawUnitReload;

    // Option choices.
    Choice   * choiceResolution;
    unsigned int current_width;
    unsigned int current_height;

    Choice   * choiceGameViewBackgroundColor;
    Choice   * choiceMiniMapObjectiveDrawMode;
    Choice   * choiceMiniMapUnitSize;
    Choice   * choiceUnitSelectionDrawMode;

    //Choice   choiceUnitInfoDrawLayer;

#if 0
    // Color choices.
    Choice   choiceYourRadarUnit;
    Choice   choiceAlliedRadarUnit;
    Choice   choiceYourRadarObjective;
    Choice   choiceAlliedRadarObjective;
    Choice   choiceEnemyRadarObjective;
    Choice   choiceVehicleSelectionBox;
    Choice   choiceConsoleText;
#endif

    virtual void loadTitleSurface();
protected:
    virtual void loadBackgroundSurface();

public:
    OptionsTemplateView();

    virtual void doDraw(Surface &windowArea, Surface &clientArea);
    virtual void initButtons();
    virtual void stateChanged(Component* source);
}; // end OptionsTemplateView

#endif // end __OptionsTemplateView_hpp__
