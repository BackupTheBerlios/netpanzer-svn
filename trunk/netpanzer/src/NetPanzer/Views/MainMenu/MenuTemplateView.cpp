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
#include "MenuTemplateView.hpp"
#include "Desktop.hpp"
#include "GameConfig.hpp"
#include "GameManager.hpp"
#include "Sound.hpp"
#include "cMouse.hpp"
#include "ViewGlobals.hpp"
#include "DDHardSurface.hpp"
#include "FontSystem2D.hpp"
#include "RadarPingParticle2D.hpp"
#include "Particle2D.hpp"
#include "ParticleSystem2D.hpp"
#include "PackedSurface.hpp"
#include "UtilInterface.hpp"
#include "GameManager.hpp"
#include "Exception.hpp"
#include "GameViewGlobals.hpp"

Surface       MenuTemplateView::backgroundSurface;
PackedSurface MenuTemplateView::titlePackedSurface;

//PackedSurface MenuTemplateView::netPanzerLogo;

char MenuTemplateView::currentMultiView[] = "GetConnectionTypeView";
char MenuTemplateView::currentView[]      = "";

static void bMain()
{
	Desktop::setVisibilityAllWindows(false);
	Desktop::setVisibility("MainView", true);
}

// never used
#if 0
static void bSkirmish()
{
	Desktop::setVisibilityAllWindows(false);
	Desktop::setVisibility("SkirmishView", true);
}
#endif

static void bMulti()
{
	GameConfig::SetGameMode(_gamemode_multiplayer);

	// Check to see which multi menu the used is actually at since
	// it is possible to leave the multi menu at any point in the menu.
	if (strcmp(MenuTemplateView::currentMultiView, "GetConnectionTypeView") == 0)
	{
		Desktop::setVisibilityAllWindows(false);
		Desktop::setVisibility("GetConnectionTypeView", true);
	}
	else if (strcmp(MenuTemplateView::currentMultiView, "GetSessionView") == 0)
	{
		Desktop::setVisibilityAllWindows(false);
		Desktop::setVisibility("GetSessionView", true);
	}
	else if (strcmp(MenuTemplateView::currentMultiView, "HostView") == 0)
	{
		Desktop::setVisibilityAllWindows(false);
		Desktop::setVisibility("HostView", true);
		Desktop::setVisibility("UnitSelectionView", true);
		Desktop::setVisibility("FlagSelectionView", true);
		Desktop::setVisibility("HostOptionsView", true);
		Desktop::setVisibility("MapSelectionView", true);
		Desktop::setVisibility("PlayerNameView", true);
	}
	else if (strcmp(MenuTemplateView::currentMultiView, "JoinView") == 0)
	{
		Desktop::setVisibilityAllWindows(false);
		Desktop::setVisibility("JoinView", true);
		Desktop::setVisibility("GetSessionHostView", true);
		Desktop::setVisibility("UnitSelectionView", true);
		Desktop::setVisibility("FlagSelectionView", true);
		Desktop::setVisibility("PlayerNameView", true);
	} else
	{
		assert(false);
	}
}

static void bOptions()
{
	Desktop::setVisibilityAllWindows(false);
	Desktop::setVisibility("OptionsView", true);
}

static void bHelp()
{
	Desktop::setVisibilityAllWindows(false);
	Desktop::setVisibility("HelpView", true);
}

// XXX never used
#if 0
static void bOrdering()
{
	if (Desktop::getVisible("GameView"))
	{
		Desktop::setVisibility("ControlsView", false);
		Desktop::setVisibility("VisualsView", false);
		Desktop::setVisibility("InterfaceView", false);
		Desktop::setVisibility("SoundView", false);
		Desktop::setVisibility("OptionsView", false);
	} else
	{
		Desktop::setVisibilityAllWindows(false);
	}

	Desktop::setVisibility("OrderingView", true);
}
#endif

static void bExit()
{
	GameManager::exitNetPanzer();
}

//---------------------------------------------------------------------------
static void bResign()
{
	Desktop::setVisibility("ControlsView", false);
	Desktop::setVisibility("VisualsView", false);
	Desktop::setVisibility("InterfaceView", false);
	Desktop::setVisibility("SoundView", false);
	Desktop::setVisibility("OptionsView", false);

	Desktop::setVisibility("AreYouSureResignView", true);
}

//---------------------------------------------------------------------------
static void bCloseOptions()
{
	GameManager::setNetPanzerGameOptions();

	Desktop::setVisibility("ControlsView", false);
	Desktop::setVisibility("VisualsView", false);
	Desktop::setVisibility("InterfaceView", false);
	Desktop::setVisibility("SoundView", false);
	Desktop::setVisibility("OptionsView", false);

}

//---------------------------------------------------------------------------
static void bExitNetPanzer()
{
	Desktop::setVisibility("ControlsView", false);
	Desktop::setVisibility("VisualsView", false);
	Desktop::setVisibility("InterfaceView", false);
	Desktop::setVisibility("SoundView", false);
	Desktop::setVisibility("OptionsView", false);

	Desktop::setVisibility("AreYouSureExitView", true);
}

// MenuTemplateView
//---------------------------------------------------------------------------
MenuTemplateView::MenuTemplateView() : SpecialButtonView()
{
	setSearchName("MenuTemplateView");
    setTitle("MenuTemplate");
    setSubTitle("");

    setAllowResize(false);
    setVisible(false);
    setAllowMove(false);
    setDisplayStatusBar(false);
    setBordered(false);
    setAlwaysOnBottom(true);

    moveTo(iXY(0, 0));
    resize(iXY(640, 480));

	initButtons();

	curTitleFlashTime  = 0.0f;
	titleFlashTimeHalf = 0.5;

} // end MenuTemplateView constructor

//---------------------------------------------------------------------------
// initPreGameOptionButtons
void MenuTemplateView::initPreGameOptionButtons()
{
	addSpecialButton(	mainPos,
						"Main",
						bMain);

	addSpecialButton(	multiPos,
						"Multiplayer",
						bMulti);

	addSpecialButton(	optionsPos,
						"Options",
						bOptions);

	addSpecialButton(	helpPos,
						"Help",
						bHelp);

	//addSpecialButton(	iXY(11, 147),
	//					"Order",
	//					bOrdering);

	addSpecialButton(	exitPos,
						"Exit netPanzer",
						bExit);

} // end MenuTemplateView::initPreGameOptionButtons

// initInGameOptionButtons
//---------------------------------------------------------------------------
void MenuTemplateView::initInGameOptionButtons()
{
	addSpecialButton(	resignPos,
						"Resign",
						bResign);

	addSpecialButton(	returnToGamePos,
						"Close Options",
						bCloseOptions);

	addSpecialButton(	exitPos,
						"Exit netPanzer",
						bExitNetPanzer);

} // end MenuTemplateView::initInGameOptionButtons

// initButtons
//---------------------------------------------------------------------------
void MenuTemplateView::initButtons()
{
	if (Desktop::getVisible("GameView"))
	{
		initInGameOptionButtons();
	} else
	{
		initPreGameOptionButtons();
	}

} // end MenuTemplateView::initButtons

// doDraw
//---------------------------------------------------------------------------
void MenuTemplateView::doDraw(const Surface &viewArea, const Surface &clientArea)
{
	//setWorldRect();

	if (Desktop::getVisible("GameView"))
	{
		clientArea.bltLookup(getClientRect(), Palette::darkGray256.getColorArray());
		clientArea.drawWindowsBorder(Color::white, Color::white, Color::white);

	} else
	{
		// Set the following to get does exist.
		if (backgroundSurface.getFrameCount() > 0)
		{
			backgroundSurface.blt(viewArea, 0, 0);
		} else
		{
			throw Exception("Where is the background surface?");
		}

		//titlePackedSurface.blt(clientArea, bodyTextRect.min.x, 390);
		titlePackedSurface.bltBlend(clientArea, bodyTextRect.min.x, 390, Palette::colorTable6040);
	}
	
	// Set the following to get does exist.
	//if (titleSurface.getFrameCount() > 0)
	//{
	//	curTitleFlashTime += TimerInterface::getTimeSlice();
	//
	//	if (curTitleFlashTime > titleFlashTimeHalf * 2.0f)
	//	{
	//		// Since we are done, draw the solid image.
	//		titleSurface.blt(viewArea, bodyTextRect.min.x, 394);
	//	}
	//	else if (curTitleFlashTime > titleFlashTimeHalf)
	//	{
	//		float titleFlashPercent = 1.0f - (float(curTitleFlashTime) / float(titleFlashTimeHalf));
	//
	//		titleSurface.bltBrightness(viewArea, iXY(bodyTextRect.min.x, 394), titleFlashPercent);
	//	}
	//	else
	//	{
	//		float titleFlashPercent = float(curTitleFlashTime) / float(titleFlashTimeHalf);
	//
	//		titleSurface.bltBrightness(viewArea, iXY(bodyTextRect.min.x, 394), titleFlashPercent);
	//	}
	//} else
	//{
	//	throw Exception("Where is the title surface?");
	//}

	//iRect r(100, 100, 200, 110);
	//clientArea.fillRect(r, Color::chartreuse);
	//r.translate(iXY(0, 15));
	//clientArea.fillRect(r, Color::cobaltGreen);
	//r.translate(iXY(0, 15));
	//clientArea.fillRect(r, Color::emeraldGreen);
	//r.translate(iXY(0, 15));
	//clientArea.fillRect(r, Color::forestGreen);
	//r.translate(iXY(0, 15));
	//clientArea.fillRect(r, Color::darkOliveGreen);
	//r.translate(iXY(0, 15));
	//clientArea.fillRect(r, Color::terreVerte);
	//r.translate(iXY(0, 15));

	// Set the following to get does exist.
	//if (globeSurface.getFrameCount() > 0)
	//{
	//	globeSurface.nextFrame();
	//	globeSurface.blt(viewArea, 0, 0);
	//	globeSurface.bltBlend(viewArea, 0, 0, Palette::colorTable8020);
	//} else
	//{
	//	throw Exception("Where is the fucking globe surface?");
	//}

	View::doDraw(viewArea, clientArea);

	//iXY pos;
	//pos.x = 0;
	//pos.y = SCREEN_YPIX - netPanzerLogo.getPixY();

    //netPanzerLogo.nextFrame();
	//netPanzerLogo.blt(viewArea, pos);

} // end doDraw

// doActivate
//---------------------------------------------------------------------------
void MenuTemplateView::doActivate()
{
	// Make the activating view active, redo this please!
	sprintf(currentView, searchName);
	Desktop::setActiveView(searchName);

		// Load apprpriate images for this view.
		//if (globeSurface.getFrameCount() <= 0)
		//{
		//	// See if the pak image already exists.
		//	if (getFileSize("pics/backgrounds/menus/globe/pak/globe.pak") > 0)
		//	{
		//		globeSurface.load("pics/backgrounds/menus/globe/pak/globe.pak");
		//	} else
		//	{
		//		Surface tempSurface;
		//
		//		if (!tempSurface.loadAllTILInDirectory("pics/backgrounds/menus/globe/til/"))
		//		{
		//			throw Exception("Shit");
		//		}
		//		tempSurface.setFPS(14);
		//
		//		globeSurface.pack(tempSurface);
		//		globeSurface.save("pics/backgrounds/menus/globe/pak/globe.pak");
		//	}
		//}
	loadBackgroundSurface();
	loadTitleSurface();
	loadNetPanzerLogo();

	SpecialButtonView::doActivate();
} // end doActivate

// loadBackgroundSurface
//---------------------------------------------------------------------------
void MenuTemplateView::loadBackgroundSurface()
{
	doLoadBackgroundSurface("pics/backgrounds/menus/menu/til/defaultMB.til");

} // end MenuTemplateView::loadBackgroundSurface

// doLoadBackgroundSurface
//---------------------------------------------------------------------------
void MenuTemplateView::doLoadBackgroundSurface(String string)
{
	if (!backgroundSurface.loadTIL(string))
	{
		throw Exception("ERROR: Unable to load menu background surface: %s", (const char *) string);
	}

} // end MenuTemplateView::doLoadBackgroundSurface

// loadTitleSurface
//---------------------------------------------------------------------------
void MenuTemplateView::loadTitleSurface()
{
	doLoadTitleSurface("pics/backgrounds/menus/menu/til/mainTitle.til");

} // end MenuTemplateView::loadTitleSurface

// doLoadBackgroundSurface
//---------------------------------------------------------------------------
void MenuTemplateView::doLoadTitleSurface(String string)
{
	curTitleFlashTime  = 0.0f;
	titleFlashTimeHalf = 2.5;

	String pakString;
	pakString = "pics/backgrounds/menus/menu/pak/";
	pakString += UtilInterface::getFilename(string);
	pakString += ".pak";

	if (UtilInterface::getFileSize(pakString))
	{
		titlePackedSurface.load((const char *) pakString);

	} else
	{
		Surface titleSurface;
	
		if (!titleSurface.loadTIL(string))
		{
			titleSurface.create(300, 50, 300, 1);
			titleSurface.fill(128);
			titleSurface.bltStringCenter("No title image", Color::white);
		}

		titlePackedSurface.pack(titleSurface);

		titlePackedSurface.save(pakString);
	}

} // end MenuTemplateView::doLoadTitleSurface

// doDeactivate
//---------------------------------------------------------------------------
void MenuTemplateView::doDeactivate()
{
    FontSystem2D::removeAll();
	
} // end doDeactivate

//---------------------------------------------------------------------------
void MenuTemplateView::loadNetPanzerLogo()
{
} // end MenuTemplateView::loadNetPanzerLogo

// processEvents
//---------------------------------------------------------------------------
void MenuTemplateView::processEvents()
{
	centerAbsolute();

} // end MenuTemplateView::processEvents
