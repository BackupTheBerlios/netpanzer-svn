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

#include "AreYouSureResignView.hpp"
#include "Views/Components/Desktop.hpp"
#include "Interfaces/GameManager.hpp"
#include "Particles/ParticleSystem2D.hpp"
#include "Particles/Particle2D.hpp"
#include "Views/MainMenu/MenuTemplateView.hpp"
#include "System/Sound.hpp"
#include "Views/MainMenu/Options/SoundView.hpp"
#include "Classes/ScreenSurface.hpp"
#include "Views/MainMenu/Options/ControlsView.hpp"
#include "Views/MainMenu/Options/VisualsView.hpp"
#include "2D/Palette.hpp"
#include "Views/MainMenu/Options/InterfaceView.hpp"
#include "Interfaces/GameConfig.hpp"
#include "Views/Components/Button.hpp"

enum
{
    BTN_YES,
    BTN_NO
};

// AreYouSureResignView
//---------------------------------------------------------------------------
AreYouSureResignView::AreYouSureResignView() : SpecialButtonView()
{
    setSearchName("AreYouSureResignView");
    setTitle("Resign");
    setSubTitle("");
} // end AreYouSureResignView::AreYouSureResignView

// init
//---------------------------------------------------------------------------
void AreYouSureResignView::init()
{
    setBordered(false);
    setAllowResize(false);
    setDisplayStatusBar(false);

    resize(screen->getWidth(), screen->getHeight());
    moveTo(0,0);

    int x = (getClientRect().getSize().x - (141 * 2 + 20)) / 2;
    int y = getClientRect().getSize().y/2 + 30;

    iXY pos(x,y);
    add( Button::createSpecialButton("yes","Yes", pos, BTN_YES));

    pos.x += 141 + 10;
    add( Button::createSpecialButton("no","No", pos, BTN_NO));

} // end AreYouSureResignView::init

// doDraw
//---------------------------------------------------------------------------
void AreYouSureResignView::doDraw()
{
    //iRect r(min, max);

    drawTransRect(*this, Palette::darkGray256.getColorArray());
    //viewArea.drawButtonBorder(r, Color::lightGreen, Color::darkGreen);

    drawStringCenter("Are you sure you wish to Resign?", Color::white);

    View::doDraw();
} // end AreYouSureResignView::doDraw

// doActivate
//---------------------------------------------------------------------------
void AreYouSureResignView::doActivate()
{
    removeComponents();
    init();
    Desktop::setActiveView(this);
} // end AreYouSureResignView::doActivate

void
AreYouSureResignView::onComponentClicked(Component* c)
{
    View *v = 0;
    switch ( c->getCustomCode() )
    {
        case BTN_YES:
            if(gameconfig->quickConnect == true)
            {
                GameManager::exitNetPanzer();
                return;
            }

            GameManager::drawTextCenteredOnScreen("Loading Main View...", Color::white);
            sprintf(MenuTemplateView::currentMultiView, "GetSessionView");

            // Vlad put all code in here for shutdown.
            //----------------------
            GameManager::quitNetPanzerGame();
            //----------------------

            GameManager::loadPalette("netpmenu");

            // Must remove the gameView first so that the initButtons detects that
            // and loads the correct buttons.
            Desktop::setVisibilityAllWindows(false);
            Desktop::setVisibility("MainView", true);

            v = Desktop::getView("OptionsView");
            if (v != 0)
            {
                ((OptionsTemplateView *)v)->initButtons();
                ((OptionsTemplateView *)v)->setAlwaysOnBottom(true);
            }
            else
            {
                assert(false);
            }

            v = Desktop::getView("SoundView");
            if (v != 0)
            {
                ((SoundView *)v)->initButtons();
                ((OptionsTemplateView *)v)->setAlwaysOnBottom(true);
            }
            else
            {
                assert(false);
            }

            v = Desktop::getView("ControlsView");
            if (v != 0)
            {
                ((ControlsView *)v)->initButtons();
                ((OptionsTemplateView *)v)->setAlwaysOnBottom(true);
            }
            else
            {
                assert(false);
            }

            v = Desktop::getView("VisualsView");
            if (v != 0)
            {
                ((VisualsView *)v)->initButtons();
                ((OptionsTemplateView *)v)->setAlwaysOnBottom(true);
            }
            else
            {
                assert(false);
            }

            v = Desktop::getView("InterfaceView");
            if (v != 0)
            {
                ((InterfaceView *)v)->initButtons();
                ((OptionsTemplateView *)v)->setAlwaysOnBottom(true);
            }
            else
            {
                assert(false);
            }
            break;

        case BTN_NO:
            Desktop::setVisibility("AreYouSureResignView", false);
            break;
    }
}
