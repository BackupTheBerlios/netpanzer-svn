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
#include "GameToolbarView.hpp"
#include "GameViewGlobals.hpp"
#include "Desktop.hpp"
#include "GameConfig.hpp"
#include "UnitInterface.hpp"
#include "PlayerInterface.hpp"
#include "WorldInputCmdProcessor.hpp"
#include "MiniMapView.hpp"


static void bToggleMiniMap()
{
	Desktop::toggleVisibility("MiniMapView");
}

static void bToggleInfo()
{
	Desktop::toggleVisibility("GameInfoView");
}

static void bToggleRankView()
{
	Desktop::toggleVisibility("RankView");
}

static void bSendMessageAll()
{
 COMMAND_PROCESSOR.startChatMessageAll();
 Desktop::setActiveView( "GameView" );
}

static void bSendMessageAllies()
{
 COMMAND_PROCESSOR.startChatMessageAllie();
 Desktop::setActiveView( "GameView" );
}

static void bSendMessageEnemies()
{
 //!VLAD!
}

static void bToggleDrawUnitDamage()
{
	GameConfig::toggleDrawUnitDamage();
}

// XXX not used
#if 0
static void bToggleDrawUnitReload()
{
	GameConfig::toggleDrawUnitReload();
}
#endif

static void bSetGroup1()
{
 COMMAND_PROCESSOR.createGroup(1);
 Desktop::setActiveView( "GameView" );
}

static void bSetGroup2()
{
 COMMAND_PROCESSOR.createGroup(2);
 Desktop::setActiveView( "GameView" );
}

static void bSetGroup3()
{
 COMMAND_PROCESSOR.createGroup(3);
 Desktop::setActiveView( "GameView" );
}

static void bSetGroup4()
{
 COMMAND_PROCESSOR.createGroup(4);
 Desktop::setActiveView( "GameView" );
}

static void bRecallGroup1()
{
 COMMAND_PROCESSOR.activateGroup(1);
 Desktop::setActiveView( "GameView" );
}

static void bRecallGroup2()
{
 COMMAND_PROCESSOR.activateGroup(2);
 Desktop::setActiveView( "GameView" );
}

static void bRecallGroup3()
{
 COMMAND_PROCESSOR.activateGroup(3);
 Desktop::setActiveView( "GameView" );
}

static void bRecallGroup4()
{
 COMMAND_PROCESSOR.activateGroup(4);
 Desktop::setActiveView( "GameView" );
}

static void bDecreaseMiniMapSize()
{
	if (Desktop::getVisible("MiniMapView"))
	{
		MiniMapView::decreaseSize = 25;
	}
}

static void bIncreaseMiniMapSize()
{
	if (Desktop::getVisible("MiniMapView"))
	{
		MiniMapView::increaseSize = 25;
	}
}

static void bToggleDrawUnitFlags()
{
	GameConfig::toggleDisplayUnitFlags();
}


// GameToolbarView
//--------------------------------------------------------------------------
GameToolbarView::GameToolbarView() : GameTemplateView()
{
	setSearchName("GameToolbarView");
	setTitle("Toolbar - TAB");
	setSubTitle("");

	setAllowResize(false);
	setAllowMove(false);
	setDisplayStatusBar(false);
	setVisible(false);

	moveTo(iXY(0, 0));
	resizeClientArea(117, 100);

	iXY       pos(0, 0);
	const int yOffset     = 15;
	int       buttonSizeX = getClientRect().getSizeX();

	addButtonCenterText(pos, buttonSizeX - (15 + 1) * 2 - 1, "Mini Map", "Toggle the visibility of the mini map", bToggleMiniMap);
	pos.x = buttonSizeX - (15 + 1) * 2;
	addButton(pos, "-", "", bDecreaseMiniMapSize);
	pos.x += 15 + 1;
	addButton(pos, "+", "", bIncreaseMiniMapSize);
	pos.x += 15 + 1;

	pos.x = 0;
	pos.y += yOffset;
	buttonSizeX = getClientRect().getSizeX() / 2;
	addButtonCenterText(pos, buttonSizeX, "Info", "Toggle the visibility of the mini map", bToggleInfo);
	pos.x += buttonSizeX + 1;
	addButtonCenterText(pos, buttonSizeX, "Rank", "Toggle the visibility of the rank view", bToggleRankView);
	pos.y += yOffset;
	pos.x = 0;
	buttonSizeX = getClientRect().getSizeX();
	addButtonCenterText(pos, buttonSizeX, "MSG All", "", bSendMessageAll);
	pos.y += yOffset;
	addButtonCenterText(pos, buttonSizeX, "MSG Allies", "", bSendMessageAllies);
	pos.y += yOffset;
	addButtonCenterText(pos, buttonSizeX, "MSG Enemies", "", bSendMessageEnemies);
	pos.y += yOffset;

	pos.x = 0;
	addLabel(iXY(pos.x + 2, pos.y + 3), "Group", Color::white);
	pos.x += 48 + 1 + 4;

	addButton(pos, "1", "", bSetGroup1);
	pos.x += 15 + 1;
	addButton(pos, "2", "", bSetGroup2);
	pos.x += 15 + 1;
	addButton(pos, "3", "", bSetGroup3);
	pos.x += 15 + 1;
	addButton(pos, "4", "", bSetGroup4);
	pos.x += 15 + 1;

	pos.x = 0;
	pos.y += yOffset;
	addLabel(iXY(pos.x + 2, pos.y + 3), "Recall", Color::white);
	pos.x += 48 + 1 + 4;

	addButton(pos, "1", "", bRecallGroup1);
	pos.x += 15 + 1;
	addButton(pos, "2", "", bRecallGroup2);
	pos.x += 15 + 1;
	addButton(pos, "3", "", bRecallGroup3);
	pos.x += 15 + 1;
	addButton(pos, "4", "", bRecallGroup4);
	pos.x += 15 + 1;
	pos.y += yOffset;

	buttonSizeX = getClientRect().getSizeX() / 2;

	pos.x = 0;
	addButtonCenterText(pos, buttonSizeX, "Damage", "", bToggleDrawUnitDamage);
	pos.x += buttonSizeX + 1;
	addButtonCenterText(pos, buttonSizeX, "Flags", "", bToggleDrawUnitFlags);
	//pos.x += buttonSizeX + 1;
	//addButtonCenterText(pos, buttonSizeX, "RL", "", bToggleDrawUnitReload);

	pos.y += yOffset;

	resizeClientArea(iXY(117, pos.y));
	
	// Start it in the bottom-left corner.
	moveTo(iXY(10000, 10000));

} // end GameToolbarView::GameToolbarView

// doDraw
//---------------------------------------------------------------------------
void GameToolbarView::doDraw(const Surface &viewArea, const Surface &clientArea)
{
	bltViewBackground(viewArea);

	View::doDraw(viewArea, clientArea);

} // end GameToolbarView::doDraw
