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
#include "HostView.hpp"
#include "GameConfig.hpp"
#include "GameViewGlobals.hpp"
#include "ParticleSystemGlobals.hpp"
#include "HostOptionsView.hpp"


static char screenSurfacePathTIL[] = "pics\\backgrounds\\menus\\multimb.til";


/////////////////////////////////////////////////////////////////////////////
// Buttons functions.
/////////////////////////////////////////////////////////////////////////////

static void setGameTypeObjective()
{
	sprintf(HostView::gameTypeBuf, "Objective");
}

static void setGameTypeFragLimit()
{
	sprintf(HostView::gameTypeBuf, "Frag Limit");
}

static void setGameTypeTimeLimit()
{
	sprintf(HostView::gameTypeBuf, "Time Limit");
}

static void increaseFragLimit()
{  	
	GameConfig::SetFragLimit(GameConfig::GetFragLimit() + 5);
}

static void decreaseFragLimit()
{
	GameConfig::SetFragLimit(GameConfig::GetFragLimit() - 5);
}

static void increaseTimeLimit()
{
	GameConfig::SetTimeLimit(GameConfig::GetTimeLimit() + 2);
}

static void decreaseTimeLimit()
{
	GameConfig::SetTimeLimit(GameConfig::GetTimeLimit() - 2);
}


//---------------------------------------------------------------------------
HostView::HostView() : HostJoinTemplateView()
{
	setSearchName("HostView");
	setTitle("Host Options");
	setSubTitle("");

	sprintf(HostView::gameTypeBuf, "Objective");

	//addButtonCenterText(iXY(x, y), buttonXSize, "Objective", "", setGameTypeObjective);
	
	//x += buttonXSize;
	//addButtonCenterText(iXY(x, y), buttonXSize, "Frag Limit", "", setGameTypeFragLimit);
	//addButton(iXY(x + buttonXSize + 1, y), "+", "", increaseFragLimit);
	//addButton(iXY(x + buttonXSize + 15, y), "-", "", decreaseFragLimit);

	//x += buttonXSize;
	//addButtonCenterText(iXY(x, y), buttonXSize, "Time Limit", "", setGameTypeTimeLimit);
	//addButton(iXY(x + buttonXSize + 1, y), "+", "", increaseTimeLimit);
	//addButton(iXY(x + buttonXSize + 15, y), "-", "", decreaseTimeLimit);
	
	//iXY pos(200, 380);
	//int yOffset = 20;
	//choiceAllowAllies.add("Yes");
	//choiceAllowAllies.add("No");
	//choiceAllowAllies.setLocation(pos);
	//pos.y += yOffset;
	//add(&choiceAllowAllies);
	//
	//choiceSpawnPlacement.add("Round Robin");
	//choiceSpawnPlacement.add("Random");
	//choiceSpawnPlacement.setLocation(pos);
	//pos.y += yOffset;
	//add(&choiceSpawnPlacement);
	//
	//choiceCloudCoverage.add("Clear");
	//choiceCloudCoverage.add("Broken");
	//choiceCloudCoverage.add("Partly Cloudy");
	//choiceCloudCoverage.add("Overcast");
	//choiceCloudCoverage.add("Extremely Cloudy");
	//choiceCloudCoverage.setLocation(pos);
	//pos.y += yOffset;
	//add(&choiceCloudCoverage);
	//
	//choiceWindSpeed.add("Calm");
	//choiceWindSpeed.add("Breezy");
	//choiceWindSpeed.add("Brisk Winds");
	//choiceWindSpeed.add("Heavy Winds");
	//choiceWindSpeed.add("Typhoon");
	//choiceWindSpeed.setLocation(pos);
	//pos.y += yOffset;
	//add(&choiceWindSpeed);

} // end HostView::HostView


// doDraw
//---------------------------------------------------------------------------
void HostView::doDraw(const Surface &viewArea, const Surface &clientArea)
{
	HostJoinTemplateView::doDraw(viewArea, clientArea);

/*
	char strBuf[256];

	sprintf(strBuf, "Game Type: %s", gameTypeBuf);
	clientArea.bltString( x + 2, y, strBuf, Color::white);
	y += yOffset;
	y += yOffset;

	sprintf(strBuf, "%d frags", GameConfig::GetFragLimit());
	clientArea.bltString( x + 220, y, strBuf, Color::white);
	y += yOffset;

	if ((GameConfig::GetTimeLimit() / 60) > 0)
	{
		sprintf(strBuf, "%d hours  %2d minutes", GameConfig::GetTimeLimit() / 60, GameConfig::GetTimeLimit() % 60);
		clientArea.bltString( x + 220, y, strBuf, Color::white);
		y += yOffset;
	} else
	{
		sprintf(strBuf, "%d minutes", GameConfig::GetTimeLimit());
		clientArea.bltString( x + 220, y, strBuf, Color::white);
		y += yOffset;
	}

	sprintf(strBuf, "Player #:");
	clientArea.bltString(x + 2, y, strBuf, Color::white);

	sprintf(strBuf, "%3d", GameConfig::GetNumberPlayers());
	clientArea.bltString(x + 125, y, strBuf, Color::white);
*/
	View::doDraw(viewArea, clientArea);

} // end HostView::doDraw

// loadTitleSurface
//---------------------------------------------------------------------------
void HostView::loadTitleSurface()
{
	doLoadTitleSurface("pics\\backgrounds\\menus\\menu\\til\\hostTitle.til");

} // end HostView::loadTitleSurface
