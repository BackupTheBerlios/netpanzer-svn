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
#include <ctype.h>
#include "HostJoinTemplateView.hpp"
#include "Desktop.hpp"
#include "HostJoinTemplateView.hpp"
#include "GameConfig.hpp"
#include "GameManager.hpp"
#include "DirectPlay.h"
#include "gapp.hpp"
#include "DDHardSurface.hpp"
#include "FontSystem2D.hpp"
#include "HostView.hpp"
#include "MapSelectionView.hpp"
#include "PlayerNameView.hpp"
#include "FlagSelectionView.hpp"
#include "GameViewGlobals.hpp"

#include "Client.hpp"
#include "Server.hpp"

static int humveeCount         =  0;
static int lightTankCount      =  0;
static int mediumTankCount     =  0;
static int heavyTankCount      =  0;
static int missleLauncherCount =  0;
static int selectionsRemaining = 10;

char HostJoinTemplateView::gameTypeBuf[256];

Surface playerColor;

int cDarkBlue;
int cLightBlue;
int cLightGreen;
int cOrange;

/////////////////////////////////////////////////////////////////////////////
// Button functions.
/////////////////////////////////////////////////////////////////////////////

static void bSetColorDarkBlue()
{ 
	// Set the ready flag to false.

	playerColor.fill(cDarkBlue);
}

static void bSetColorLightBlue()
{
	// Set the ready flag to false.

	playerColor.fill(cLightBlue);
}

static void bSetColorLightGreen()
{
	// Set the ready flag to false.

	playerColor.fill(cLightGreen);
}

static void bSetColorRed()
{
	// Set the ready flag to false.

	playerColor.fill(Color::red);
}

static void bSetColorOrange()
{
	// Set the ready flag to false.

	playerColor.fill(cOrange);
}

static void bBack()
{
 //fix bug#15--
 //winsock hack
 //ShutDownConnection();
 if (GameConfig::GetHostOrJoin() == _game_session_join)
  {
   CLIENT->closeSession();
  }
 else
  if (GameConfig::GetHostOrJoin() == _game_session_host)
   {
    SERVER->closeSession();  
   }   
 
 Desktop::setVisibilityAllWindows(false);
 Desktop::setVisibility("GetSessionView", true);
}

void bReady()
{
	char temp_str[64];

    if (strlen(PlayerNameView::playerName.getString()) <= 0) { return; }

    strcpy( temp_str, PlayerNameView::playerName.getString() );

    // make string lowercase
    for(char* p = temp_str; *p != 0; p++) {
	*p = tolower(*p);
    }
    if (strstr( temp_str, "server" ) != 0 ) { return; }

    // Check a few things which should be ok.
    if (strlen(HostJoinTemplateView::gameTypeBuf) == 0) { return; }
	if (MapSelectionView::curMap == -1) { return; }

	// Set the player name.
	GameConfig::SetPlayerName(PlayerNameView::playerName.getString());

	// Set the player flag.
	GameConfig::SetPlayerFlag( (short) playerFlagSelected );

	if (GameConfig::GetHostOrJoin() == _game_session_join)
	{
  //winsock hack
//		if (!IsSelectedGameValid())
//		{
//			return;
//		}

		// Close all menu views.
		Desktop::setVisibilityAllWindows(false);

		Desktop::setVisibility("LobbyView", true);

		//this call should be redundant -- enumeration ceases
        //when a session is opened in any case:
        //StopAsyncGameEnumeration( gapp.hwndApp );
	    CLIENT->stopEnumeration();
    } else
	{
		// Close all menu views.
		Desktop::setVisibilityAllWindows(false);
	}

	// Free the menu pictures.
	//MenuTemplateView::netPanzerLogo.free();
	MenuTemplateView::backgroundSurface.free();
	//MenuTemplateView::titleSurface.free();
	
	// Make sure no views were left in weird places from a previous game.
	Desktop::checkViewPositions();

	GameManager::launchNetPanzerGame();
}
/*
enum  { _unit_type_valentine,
        _unit_type_leopard,
        _unit_type_abrams,
        _unit_type_hammerhead,
        _unit_type_humvee,
*/

// Increase unit count.
static void bIncreaseHumvee()
{
	GameConfig::unit_spawn_config.incrementSpawnUnitCount(_unit_type_humvee);
}

static void bIncreaseLightTank()
{
	GameConfig::unit_spawn_config.incrementSpawnUnitCount(_unit_type_valentine);
}

static void bIncreaseMediumTank()
{
	GameConfig::unit_spawn_config.incrementSpawnUnitCount(_unit_type_leopard);
}

static void bIncreaseHeavyTank()
{
	GameConfig::unit_spawn_config.incrementSpawnUnitCount(_unit_type_abrams);
}

static void bIncreaseMissleLauncher()
{
	GameConfig::unit_spawn_config.incrementSpawnUnitCount(_unit_type_hammerhead);
}

// Decrease unit count.
static void bDecreaseHumvee()
{
	GameConfig::unit_spawn_config.decrementSpawnUnitCount(_unit_type_humvee);
}

static void bDecreaseLightTank()
{
	GameConfig::unit_spawn_config.decrementSpawnUnitCount(_unit_type_valentine);
}

static void bDecreaseMediumTank()
{
	GameConfig::unit_spawn_config.decrementSpawnUnitCount(_unit_type_leopard);
}

static void bDecreaseHeavyTank()
{
	GameConfig::unit_spawn_config.decrementSpawnUnitCount(_unit_type_abrams);
}

static void bDecreaseMissleLauncher()
{
	GameConfig::unit_spawn_config.decrementSpawnUnitCount(_unit_type_hammerhead);
}

// Get unit count.
static int getHumveeCount()
{
	return GameConfig::unit_spawn_config.getSpawnUnitCount(_unit_type_humvee);
}

static int getLightTankCount()
{
	return GameConfig::unit_spawn_config.getSpawnUnitCount(_unit_type_valentine);
}

static int getMediumTankCount()
{
	return GameConfig::unit_spawn_config.getSpawnUnitCount(_unit_type_leopard);
}

static int getHeavyTankCount()
{
	return GameConfig::unit_spawn_config.getSpawnUnitCount(_unit_type_abrams);
}

static int getMissleLauncherCount()
{
	return GameConfig::unit_spawn_config.getSpawnUnitCount(_unit_type_hammerhead);
}

static int getSelectionsRemaining()
{
	return GameConfig::unit_spawn_config.getMaxAllowedUnits() - GameConfig::unit_spawn_config.unitTotal();
}


// HostJoinTemplateView
//---------------------------------------------------------------------------
HostJoinTemplateView::HostJoinTemplateView() : MenuTemplateView()
{
	setSearchName("HostJoinTemplateView");
	setTitle("");
	setSubTitle("");

	// Back.
	addSpecialButton(	backPos,
						"Back",
						bBack);

	// Ready.
	addSpecialButton(	readyPos,
						"Ready",
						bReady);

	//addVehicleButtons(iXY(220, 195));
/*
	Surface s(20, 15, 20, 1);
	x = 84;
	cDarkBlue   = pal.findNearestColor(RGB_COLOR(0, 0, 100));
	cLightBlue  = pal.findNearestColor(RGB_COLOR(173, 216, 230));
	cLightGreen = pal.findNearestColor(RGB_COLOR(0, 160, 0));
	cOrange     = pal.findNearestColor(RGB_COLOR(255, 128, 0));
	
	y += 20;
	// Dark Blue
	s.fill(cDarkBlue);
	s.drawButtonBorder(Color::white, Color::gray64);
	addButtonSurface(iXY(x, y), s, "Dark Blue", bSetColorDarkBlue);
	x += xOffset;

	// Light Blue
	s.fill(cLightBlue);
	s.drawButtonBorder(Color::white, Color::gray64);
	addButtonSurface(iXY(x, y), s, "Light Blue", bSetColorLightBlue);
	x += xOffset;

	// Light Green
	s.fill(cLightGreen);
	s.drawButtonBorder(Color::white, Color::gray64);
	addButtonSurface(iXY(x, y), s, "Light Green", bSetColorLightGreen);
	x += xOffset;

	// Red
	s.fill(Color::red);
	s.drawButtonBorder(Color::white, Color::gray64);
	addButtonSurface(iXY(x, y), s, "Red", bSetColorRed);
	x += xOffset;

	// Orange
	s.fill(cOrange);
	s.drawButtonBorder(Color::white, Color::gray64);
	addButtonSurface(iXY(x, y), s, "Orange", bSetColorOrange);
	x += xOffset;

	// Surface for the player's current color.
	playerColor.create(20, 15, 20, 1);
	playerColor.fill(cDarkBlue);
*/
	//for (int i = 0; i < NUM_FLAGS; i++)
	//{
	//	playerFlag.setFrame(i);
	//	playerFlag.fill(rand() % 256);
	//	playerFlag.drawButtonBorder(Color::white, Color::gray64);
	//
	//	sprintf(strBuf, "%d", i);
	//	bltString(playerFlag, 2, 3, strBuf, Color::white);
	//
	//	// Create a button off the created surface.
	//	addButtonSurface(iXY(x, 38), playerFlag, strBuf, 0);
	//	x += xOffset;
	//}

} // end HostJoinTemplateView constructor

// addVehicleButtons
//---------------------------------------------------------------------------
void HostJoinTemplateView::addVehicleButtons(const iXY &pos)
{
	// Add the vehicle buttons.
	// Get the dimensions of the buttons to draw.
	Surface tempSurface;
	if (!tempSurface.loadTIL("pics/vehicleSelectionMenu/light.til"))
	{
		FUBAR("ERROR: Unable to open pics/vehicleSelectionMenu/light.til to find the size to make the buttons.");
	}
	
	iXY buttonSize(tempSurface.getPix());
	int arrowButtonWidth =  16;

	int x;
	int y;

	x = pos.x;

	y = pos.y;

	addButtonTILBordered(iXY(x, y), "pics/vehicleSelectionMenu/humvee.til", "", 0);
	x += buttonSize.x + 1;

	addButtonTILBordered(iXY(x, y), "pics/vehicleSelectionMenu/light.til", "", 0);
	x += buttonSize.x + 1;

	addButtonTILBordered(iXY(x, y), "pics/vehicleSelectionMenu/medium.til", "", 0);
	x += buttonSize.x + 1;

	addButtonTILBordered(iXY(x, y), "pics/vehicleSelectionMenu/heavy.til", "", 0);
	x += buttonSize.x + 1;
	
	addButtonTILBordered(iXY(x, y), "pics/vehicleSelectionMenu/missle.til", "", 0);
	x += buttonSize.x + 1;

	// Draw the arrows to change the numbers.
	x = pos.x;
	y = pos.y + buttonSize.y;

	x = pos.x + (buttonSize.x + 1) * 0;
	x += (buttonSize.x - (arrowButtonWidth * 2)) / 2;
	addButtonCenterText(iXY(x, y), arrowButtonWidth, "<", "", bDecreaseHumvee);
	addButtonCenterText(iXY(x + arrowButtonWidth, y), arrowButtonWidth, ">", "", bIncreaseHumvee);

	x = pos.x + (buttonSize.x + 1) * 1;
	x += (buttonSize.x - (arrowButtonWidth * 2)) / 2;
	addButtonCenterText(iXY(x, y), arrowButtonWidth, "<", "", bDecreaseLightTank);
	addButtonCenterText(iXY(x + arrowButtonWidth, y), arrowButtonWidth, ">", "", bIncreaseLightTank);

	x = pos.x + (buttonSize.x + 1) * 2;
	x += (buttonSize.x - (arrowButtonWidth * 2)) / 2;
	addButtonCenterText(iXY(x, y), arrowButtonWidth, "<", "", bDecreaseMediumTank);
	addButtonCenterText(iXY(x + arrowButtonWidth, y), arrowButtonWidth, ">", "", bIncreaseMediumTank);

	x = pos.x + (buttonSize.x + 1) * 3;
	x += (buttonSize.x - (arrowButtonWidth * 2)) / 2;
	addButtonCenterText(iXY(x, y), arrowButtonWidth, "<", "", bDecreaseHeavyTank);
	addButtonCenterText(iXY(x + arrowButtonWidth, y), arrowButtonWidth, ">", "", bIncreaseHeavyTank);

	x = pos.x + (buttonSize.x + 1) * 4;
	x += (buttonSize.x - (arrowButtonWidth * 2)) / 2;
	addButtonCenterText(iXY(x, y), arrowButtonWidth, "<", "", bDecreaseMissleLauncher);
	addButtonCenterText(iXY(x + arrowButtonWidth, y), arrowButtonWidth, ">", "", bIncreaseMissleLauncher);


} // end HostJoinTemplateView::addVehicleButtons

void HostJoinTemplateView::drawVehicleInfo(const Surface &dest, const iXY &pos)
{
	// Draw the dimmed background.
	iXY size(580, 118);
	iRect mapBorder(pos.x, pos.y, pos.x + size.x, pos.y + size.y);
	dest.bltLookup(mapBorder, Palette::darkGray256.getColorArray());
	dest.drawButtonBorder(mapBorder, Color::lightGreen, Color::darkGreen);

	// Draw the text.
	char strBuf[256];
	const PIX color = Color::lightGreen;
	int x = pos.x + 10;
	int y = pos.y + 10;
	
	const int yOffset = 15;
	
	sprintf(strBuf, "VEHICLE SELECTION");
	//dest.bltString(x, y, strBuf, color);
	dest.bltStringVGradient(x, y, strBuf, Palette::gradientWhite2Green);
	y += yOffset;

	sprintf(strBuf, " Humvee          - %2d", getHumveeCount());
	dest.bltString(x, y, strBuf, color);
	y += yOffset;

	sprintf(strBuf, " Light Tank      - %2d", getLightTankCount());
	dest.bltString(x, y, strBuf, color);
	y += yOffset;

	sprintf(strBuf, " Medium Tank     - %2d", getMediumTankCount());
	dest.bltString(x, y, strBuf, color);
	y += yOffset;

	sprintf(strBuf, " Heavy Tank      - %2d", getHeavyTankCount());
	dest.bltString(x, y, strBuf, color);
	y += yOffset;

	sprintf(strBuf, " Missle Launcher - %2d", getMissleLauncherCount());
	dest.bltString(x, y, strBuf, color);
	y += yOffset;

	sprintf(strBuf, " Selections Remaining - %2d", getSelectionsRemaining());
	dest.bltString(x, y, strBuf, color);
	y += yOffset;

} // end HostView::drawVehicleInfo

// doDraw
//---------------------------------------------------------------------------
void HostJoinTemplateView::doDraw(const Surface &viewArea, const Surface &clientArea)
{
	MenuTemplateView::doDraw(viewArea, clientArea);

	iXY pos(420, 38);

	//clientArea.bltString(pos, "Are You", Color::white);

	// Display the game type.
	//sprintf(strBuf, "Game Type: Call game type function.");
	//clientArea.bltStringShadowed(5, y, strBuf, Color::white, Color::gray64);

	//yOffset
	// Display the player count.
	//sprintf(strBuf, "Player Count: Call player count function.");
	//clientArea.bltString(5, 78, strBuf, Color::white);

	//sprintf(strBuf, "Player Name                        Flag   Status");
	//clientArea.bltStringShadowed(5, 96, strBuf, Color::white, Color::gray64);

	//clientArea.drawButtonBorder(iRECT(4, 106, getClientRect().size().x - 5, 210), Color::white, Color::gray64);
	//iXY pos(7, 108);
	//drawPlayerInfo(clientArea, pos);

	View::doDraw(viewArea, clientArea);

} // end doDraw

// drawNameInfo
//--------------------------------------------------------------------------
void HostJoinTemplateView::drawNameInfo(const Surface &dest, const iXY &pos)
{
	iXY size(358, 30);
	iRect flagBorder(pos.x, pos.y, pos.x + size.x, pos.y + size.y);
	dest.bltLookup(flagBorder, Palette::darkGray256.getColorArray());
	dest.drawButtonBorder(flagBorder, Color::lightGreen, Color::darkGreen);

	dest.bltStringVGradient(iXY(pos.x + 10, pos.y + 12), "NAME", Palette::gradientWhite2Green);

} // end HostJoinTemplateView::drawNameInfo

// drawPlayerInfo
//--------------------------------------------------------------------------
void HostJoinTemplateView::drawPlayerInfo(const Surface &dest, const iXY &pos)
{
	char strBuf[256];

	int x = pos.x;
	sprintf(strBuf, "Player Name");
	dest.bltString(x, pos.y + (playerColor.getPix().y - CHAR_YPIX) / 2, strBuf, Color::white);

	//x = 236;
	//playerColor.blt(clientArea, x, pos.y);

	x = 290;
	playerFlag.blt(dest, x, pos.y);

	x = 342;
	sprintf(strBuf, "Status");
	dest.bltString(x, pos.y + (playerColor.getPix().y - CHAR_YPIX) / 2, strBuf, Color::white);

} // end drawPlayerInfo

// drawFlagInfo
//--------------------------------------------------------------------------
void HostJoinTemplateView::drawFlagInfo(const Surface &dest, const iXY &pos)
{
	// Draw the dimmed background.
	iXY size(180, 115);
	iRect flagBorder(pos.x, pos.y, pos.x + size.x, pos.y + size.y);
	dest.bltLookup(flagBorder, Palette::darkGray256.getColorArray());
	dest.drawButtonBorder(flagBorder, Color::lightGreen, Color::darkGreen);

	dest.bltStringVGradient(pos + 10, "FLAG", Palette::gradientWhite2Green);
	playerFlag.blt(dest, iXY(pos.x + 55, pos.y + 10));

} // end HostJoinTemplateView::drawFlagInfo

// doActivate
//---------------------------------------------------------------------------
void HostJoinTemplateView::doActivate()
{
	MenuTemplateView::doActivate();

	sprintf(currentMultiView, searchName);

} // end doActivate

void HostJoinTemplateView::drawMapInfo(const Surface &dest, const iXY &pos)
{
	// Draw the dimmed background.
	iXY size(300, 115);
	iRect mapBorder(pos.x, pos.y, SCREEN_XPIX - 30, pos.y + size.y);
	dest.bltLookup(mapBorder, Palette::darkGray256.getColorArray());
	dest.drawButtonBorder(mapBorder, Color::lightGreen, Color::darkGreen);

	// Draw the text.
	char strBuf[256];
	const PIX color = Color::lightGreen;
	int x = 340;
	int y = pos.y + 10;
	
	const int yOffset = 15;
	
	sprintf(strBuf, "MAP INFORMATION");
	//dest.bltString(x, y, strBuf, color);
	dest.bltStringVGradient(x, y, strBuf, Palette::gradientWhite2Green);
	y += yOffset;

	sprintf(strBuf, " Name");
	dest.bltString(x, y, strBuf, color);
	y += yOffset;

	sprintf(strBuf, " Size");
	dest.bltString(x, y, strBuf, color);
	y += yOffset;
/*
	sprintf(strBuf, " Bases");
	dest.bltString(x, y, strBuf, color);
	y += yOffset;

	sprintf(strBuf, " Factories");
	dest.bltString(x, y, strBuf, color);
	y += yOffset;

	sprintf(strBuf, " Spawn Locations");
	dest.bltString(x, y, strBuf, color);
	y += yOffset;

	sprintf(strBuf, " Temperment");
	dest.bltString(x, y, strBuf, color);
	y += yOffset;
*/
} // end HostView::drawMapInfo

// loadBackgroundSurface
//---------------------------------------------------------------------------
void HostJoinTemplateView::loadBackgroundSurface()
{
	doLoadBackgroundSurface("pics/backgrounds/menus/menu/til/hostjoinMB.til");

} // end HostJoinTemplateView::loadBackgroundSurface
