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
#include "Desktop.hpp"
#include "DDHardSurface.hpp"
#include "GameManager.hpp"
#include "ParticleSystem2D.hpp"
#include "Particle2D.hpp"
#include "MenuTemplateView.hpp"
#include "Sound.hpp"
#include "SoundView.hpp"
#include "ControlsView.hpp"
#include "VisualsView.hpp"
#include "InterfaceView.hpp"

//---------------------------------------------------------------------------
static void bYES()
{
	sound->StopTankIdle();

	GameManager::drawTextCenteredOnScreen("Loading Main View...", Color::white);

	ParticleSystem2D::removeAll();
	Particle2D::removeAll();
	sprintf(MenuTemplateView::currentMultiView, "GetConnectionTypeView");

	// Vlad put all code in here for shutdown.
	//----------------------
    GameManager::quitNetPanzerGame();
	//----------------------

	// Swap to the menu resolution.
	GameManager::setVideoMode(iXY(640, 480), false);

	GameManager::drawTextCenteredOnScreen("Loading Main View...", Color::white);

	GameManager::loadPalette("wads/netpmenu.act");

	// Must remove the gameView first so that the initButtons detects that
	// and loads the correct buttons.
	Desktop::setVisibilityAllWindows(false);
	Desktop::setVisibility("MainView", true);

	View *v = Desktop::getView("OptionsView");
	
	if (v != 0)
	{
		((OptionsTemplateView *)v)->initButtons();
		((OptionsTemplateView *)v)->setAlwaysOnBottom(true);
	} else
	{
		assert(false);
	}

	v = Desktop::getView("SoundView");
	if (v != 0)
	{
		((SoundView *)v)->initButtons();
		((OptionsTemplateView *)v)->setAlwaysOnBottom(true);
	} else
	{
		assert(false);
	}

	v = Desktop::getView("ControlsView");
	if (v != 0)
	{
		((ControlsView *)v)->initButtons();
		((OptionsTemplateView *)v)->setAlwaysOnBottom(true);
	} else
	{
		assert(false);
	}

	v = Desktop::getView("VisualsView");
	if (v != 0)
	{
		((VisualsView *)v)->initButtons();
		((OptionsTemplateView *)v)->setAlwaysOnBottom(true);
	} else
	{
		assert(false);
	}

	v = Desktop::getView("InterfaceView");
	if (v != 0)
	{
		((InterfaceView *)v)->initButtons();
		((OptionsTemplateView *)v)->setAlwaysOnBottom(true);
	} else
	{
		assert(false);
	}
}

//---------------------------------------------------------------------------
static void bNO()
{
	Desktop::setVisibility("AreYouSureResignView", false);
	//Desktop::setVisibility("ResignView", true);
}

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
	removeAllButtons();

	setBordered(false);
	setAllowResize(false);
	setDisplayStatusBar(false);

	moveTo(0, 0);
	resize(SCREEN_PIX);

	int x = (getClientRect().getSize().x - (141 * 2 + 20)) / 2;
	int y = SCREEN_YCENTER + 30;
	addSpecialButton(	iXY(x, y),
						"YES",
						bYES);

	x += 141 + 10;
	addSpecialButton(	iXY(x, y),
						"NO",
						bNO);

} // end AreYouSureResignView::init

// doDraw
//---------------------------------------------------------------------------
void AreYouSureResignView::doDraw(const Surface &viewArea, const Surface &clientArea)
{
	iXY   pos(0, 0);
	iXY   size(SCREEN_PIX);
	iRect r(pos, pos + size);

	viewArea.bltLookup(r, Palette::darkGray256.getColorArray());
	//viewArea.drawButtonBorder(r, Color::lightGreen, Color::darkGreen);

	viewArea.bltStringCenter("Are you sure you wish to Resign?", Color::white);

	View::doDraw(viewArea, clientArea);

} // end AreYouSureResignView::doDraw

// doActivate
//---------------------------------------------------------------------------
void AreYouSureResignView::doActivate()
{
	init();
	Desktop::setActiveView(this);

} // end AreYouSureResignView::doActivate
