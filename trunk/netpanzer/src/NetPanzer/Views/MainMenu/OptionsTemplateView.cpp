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

#include "Views/MainMenu/OptionsTemplateView.hpp"
#include "Views/Components/Desktop.hpp"
#include "Views/GameViewGlobals.hpp"
#include "Views/Components/Button.hpp"

#define BTN_INTERFACE "OptionsTemplateView.Interface"
#define BTN_VISUALS "OptionsTemplateView.Visuals"
#define BTN_SOUND "OptionsTemplateView.Sound"

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
    removeComponents();

    MenuTemplateView::initButtons();

    add( Button::createSpecialButton( BTN_SOUND,"Sound", soundPos));
    add( Button::createSpecialButton( BTN_INTERFACE,"Interface", interfacePos));
    add( Button::createSpecialButton( BTN_VISUALS,"Visuals",visualsPos));

#if 0 // XXX
    addSpecialButton( controlsPos,  "Controls",  bControls);
#endif

} // end OptionsTemplateView::initButtons

// doDraw
//---------------------------------------------------------------------------
void OptionsTemplateView::doDraw()
{
    MenuTemplateView::doDraw();

    drawStringCenter("Select your option category above", Color::white);

} // end OptionsTemplateView::doDraw

// loadBackgroundSurface
//---------------------------------------------------------------------------
void OptionsTemplateView::loadBackgroundSurface()
{
    doLoadBackgroundSurface("pics/backgrounds/menus/menu/optionsMB.bmp");

} // end OptionsTemplateView::loadBackgroundSurface

// loadTitleSurface
//---------------------------------------------------------------------------
void OptionsTemplateView::loadTitleSurface()
{
    doLoadTitleSurface("optionsTitle");

} // end ControlsView::loadTitleSurface

void
OptionsTemplateView::onComponentClicked(Component* c)
{
    string cname = c->getName();

    if ( ! cname.compare("Button." BTN_INTERFACE) )
    {
        if (Desktop::getVisible("GameView"))
        {
            Desktop::setVisibility("ControlsView", false);
            Desktop::setVisibility("VisualsView", false);
            Desktop::setVisibility("InterfaceView", false);
            Desktop::setVisibility("SoundView", false);
            Desktop::setVisibility("OptionsView", false);
        }
        else
        {
            Desktop::setVisibilityAllWindows(false);
        }

        Desktop::setVisibility("InterfaceView", true);
    }
    else if ( ! cname.compare("Button." BTN_VISUALS) )
    {
        if (Desktop::getVisible("GameView"))
        {
            Desktop::setVisibility("ControlsView", false);
            Desktop::setVisibility("VisualsView", false);
            Desktop::setVisibility("InterfaceView", false);
            Desktop::setVisibility("SoundView", false);
            Desktop::setVisibility("OptionsView", false);
        }
        else
        {
            Desktop::setVisibilityAllWindows(false);
        }

        Desktop::setVisibility("VisualsView", true);
    }
    else if ( ! cname.compare("Button." BTN_SOUND) )
    {
        if (Desktop::getVisible("GameView"))
        {
            Desktop::setVisibility("ControlsView", false);
            Desktop::setVisibility("VisualsView", false);
            Desktop::setVisibility("InterfaceView", false);
            //Desktop::setVisibility("SoundView", false);
            Desktop::setVisibility("OptionsView", false);
        }
        else
        {
            Desktop::setVisibilityAllWindows(false);
        }
        Desktop::setVisibility("SoundView", true);
    }
    else
    {
        MenuTemplateView::onComponentClicked(c);
    }
}