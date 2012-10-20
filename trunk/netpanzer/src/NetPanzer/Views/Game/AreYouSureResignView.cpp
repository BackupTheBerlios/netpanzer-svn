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


#include "AreYouSureResignView.hpp"
#include "Views/Components/Desktop.hpp"
#include "Views/Components/Button.hpp"
#include "Interfaces/GameManager.hpp"
#include "Interfaces/StrManager.hpp"
#include "Particles/ParticleSystem2D.hpp"
#include "Particles/Particle2D.hpp"
#include "System/Sound.hpp"
#include "Classes/ScreenSurface.hpp"
#include "2D/Palette.hpp"
#include "Interfaces/GameConfig.hpp"
#include "Views/MainMenu/OptionsTemplateView.hpp"
#include "Views/MainMenu/MenuTemplateView.hpp"

#include "Actions/ActionManager.hpp"

// AreYouSureResignView
//---------------------------------------------------------------------------
AreYouSureResignView::AreYouSureResignView() : SpecialButtonView()
{
    setSearchName("AreYouSureResignView");
    setTitle(_("Resign"));
    setSubTitle("");
    loaded = false;
	} // end AreYouSureResignView::AreYouSureResignView

// init
//---------------------------------------------------------------------------
void AreYouSureResignView::init()
{
    setBordered(false);
    setAllowResize(false);
    setDisplayStatusBar(false);

    moveTo(iXY(0, 0));
    resize(iXY(800, 600));

    int x = (getClientRect().getSize().x - (141 * 2 + 20)) / 2;
    int y = getClientRect().getSize().y/2 + 30;
    add( Button::createTextButton( _("YES"), iXY(x, y), 137, ActionManager::getAction("disconnect")));
    x += 141 + 10;
    add( Button::createTextButton( _("NO"), iXY(x, y), 137, ActionManager::getAction("hide_confirmdisconnect")));
    loaded = true;
} // end AreYouSureResignView::init

// doDraw
//---------------------------------------------------------------------------
void AreYouSureResignView::doDraw(Surface &viewArea, Surface &clientArea)
{
    viewArea.bltLookup(getClientRect(), Palette::darkGray256.getColorArray());
    //viewArea.drawButtonBorder(r, Color::lightGreen, Color::darkGreen);

    viewArea.bltStringCenter(_("Are you sure you wish to Resign?"), Color::white);

    View::doDraw(viewArea, clientArea);
} // end AreYouSureResignView::doDraw

// doActivate
//---------------------------------------------------------------------------
void AreYouSureResignView::doActivate()
{
    if ( ! loaded )
    {
	init();
    }
    Desktop::setActiveView(this);
} // end AreYouSureResignView::doActivate
