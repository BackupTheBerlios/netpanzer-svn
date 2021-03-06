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
#include "OptionsTemplateView.hpp"
#include "Desktop.hpp"
#include "DDHardSurface.hpp"
#include "GameViewGlobals.hpp"


static void bSound()
{
    if (Desktop::getVisible("GameView")) {
        Desktop::setVisibility("ControlsView", false);
        Desktop::setVisibility("VisualsView", false);
        Desktop::setVisibility("InterfaceView", false);
        Desktop::setVisibility("SoundView", false);
        Desktop::setVisibility("OptionsView", false);
    } else {
        Desktop::setVisibilityAllWindows(false);
    }

    Desktop::setVisibility("SoundView", true);
}

static void bInterface()
{
    if (Desktop::getVisible("GameView")) {
        Desktop::setVisibility("ControlsView", false);
        Desktop::setVisibility("VisualsView", false);
        Desktop::setVisibility("InterfaceView", false);
        Desktop::setVisibility("SoundView", false);
        Desktop::setVisibility("OptionsView", false);
    } else {
        Desktop::setVisibilityAllWindows(false);
    }

    Desktop::setVisibility("InterfaceView", true);
}

static void bVisuals()
{
    if (Desktop::getVisible("GameView")) {
        Desktop::setVisibility("ControlsView", false);
        Desktop::setVisibility("VisualsView", false);
        Desktop::setVisibility("InterfaceView", false);
        Desktop::setVisibility("SoundView", false);
        Desktop::setVisibility("OptionsView", false);
    } else {
        Desktop::setVisibilityAllWindows(false);
    }

    Desktop::setVisibility("VisualsView", true);
}

static void bControls()
{
    if (Desktop::getVisible("GameView")) {
        Desktop::setVisibility("ControlsView", false);
        Desktop::setVisibility("VisualsView", false);
        Desktop::setVisibility("InterfaceView", false);
        Desktop::setVisibility("SoundView", false);
        Desktop::setVisibility("OptionsView", false);
    } else {
        Desktop::setVisibilityAllWindows(false);
    }

    Desktop::setVisibility("ControlsView", true);
}

// OptionsTemplateView
//---------------------------------------------------------------------------
OptionsTemplateView::OptionsTemplateView() : MenuTemplateView()
{
    setSearchName("OptionsView");
    setTitle("OptionsTemplate");
    setSubTitle("");

    initButtons();

} // end OptionsTemplateView::OptionsTemplateView


// initButtons
//---------------------------------------------------------------------------
void OptionsTemplateView::initButtons()
{
    removeAllButtons();
    removeComponents();

    MenuTemplateView::initButtons();

    // Sound.
    addSpecialButton(	soundPos,
                      "Sound",
                      bSound);

    // Interface.
    addSpecialButton(	interfacePos,
                      "Interface",
                      bInterface);

    // Visuals.
    addSpecialButton(	visualsPos,
                      "Visuals",
                      bVisuals);

    // Contols.
    addSpecialButton(	controlsPos,
                      "Controls",
                      bControls);

} // end OptionsTemplateView::initButtons

// doDraw
//---------------------------------------------------------------------------
void OptionsTemplateView::doDraw(const Surface &viewArea, const Surface &clientArea)
{
    MenuTemplateView::doDraw(viewArea, clientArea);

    clientArea.bltStringCenter("Select your option category above", Color::white);

} // end OptionsTemplateView::doDraw

// loadBackgroundSurface
//---------------------------------------------------------------------------
void OptionsTemplateView::loadBackgroundSurface()
{
    doLoadBackgroundSurface(("pics/backgrounds/menus/menu/til/optionsMB.til"));

} // end OptionsTemplateView::loadBackgroundSurface

// loadTitleSurface
//---------------------------------------------------------------------------
void OptionsTemplateView::loadTitleSurface()
{
    doLoadTitleSurface("pics/backgrounds/menus/menu/til/optionsTitle.til");

} // end ControlsView::loadTitleSurface
