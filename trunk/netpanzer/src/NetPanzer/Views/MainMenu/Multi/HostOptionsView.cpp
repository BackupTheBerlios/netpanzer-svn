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
#include "HostOptionsView.hpp"
#include "GameConfig.hpp"
#include "HostView.hpp"
#include "ParticleSystemGlobals.hpp"
#include "MapSelectionView.hpp"
#include "GameViewGlobals.hpp"


int HostOptionsView::cloudCoverageCount = 1;
int HostOptionsView::windSpeed          = 1;
int HostOptionsView::gameType           = 0;

String HostOptionsView::cloudCoverageString;
String HostOptionsView::windSpeedString;


static int getCurMaxPlayersCount()
{
	return GameConfig::GetNumberPlayers();
}

static int getMaxPlayersCount()
{
	return GameConfig::getNumberUnitsBoundsUpper();
}

static int  getCurMaxUnitCount()
{
	return GameConfig::GetNumberUnits();
}

static int  getMaxUnitCount()
{
	return GameConfig::getNumberUnitsBoundsUpper();
}

static int  getRespawnUnitCount()
{
	return GameConfig::GetNumberInitialUnits();
}

static int  getMaxRespawnUnitCount()
{
	return GameConfig::getNumberInitialUnitsBoundUpper();
}

static const char *getAllowAllies()
{
	return GameConfig::getAllieStateString();
	//return "Yes";
}

static const char *getSpawnPlacement()
{
	return GameConfig::getRespawnTypeString();
	//return "Round Robin";
}

static const char *getAllowFogOfWar()
{
	//return GameConfig::getAllowFogOfWarString()
	return "Not available in netPanzerTest.";
}

static void bDecreasePlayerCount()
{
	GameConfig::SetNumberPlayers(GameConfig::GetNumberPlayers() - 1);
}

static void bIncreasePlayerCount()
{
	GameConfig::SetNumberPlayers(GameConfig::GetNumberPlayers() + 1);
}

static void bDecreaseMaxUnitCount()
{
	GameConfig::SetNumberUnits(GameConfig::GetNumberUnits() - 5);
}

static void bIncreaseMaxUnitCount()
{
	GameConfig::SetNumberUnits(GameConfig::GetNumberUnits() + 5);
}

static void bDecreaseRespawnCount()
{
	GameConfig::SetNumberInitialUnits(GameConfig::GetNumberInitialUnits() - 1);
}

static void bIncreaseRespawnCount()
{
	GameConfig::SetNumberInitialUnits(GameConfig::GetNumberInitialUnits() + 1);
}

static void bSetAllowAlliesFalse()
{
	GameConfig::setAllieState(false);
}

static void bSetAllowAlliesTrue()
{
	GameConfig::setAllieState(true);
}

static void bPreviousSpawnPlacement()
{
 GameConfig::setPreviousRespawnType();
}

static void bNextSpawnPlacement()
{
 GameConfig::setNextRespawnType();
}

static void bIncreaseCloudCoverage()
{
	HostOptionsView::setCloudCoverageCount(HostOptionsView::getCloudCoverageCount() + 1);

	if (HostOptionsView::getCloudCoverageCount() > 4)
	{
		HostOptionsView::setCloudCoverageCount(4);
	}

	HostOptionsView::updateGameConfigCloudCoverage();
}

static void bDecreaseCloudCoverage()
{
	HostOptionsView::setCloudCoverageCount(HostOptionsView::getCloudCoverageCount() - 1);

	if (HostOptionsView::getCloudCoverageCount() < 0)
	{
		HostOptionsView::setCloudCoverageCount(0);
	}

	HostOptionsView::updateGameConfigCloudCoverage();
}

void HostOptionsView::updateGameConfigCloudCoverage()
{
	// Make sure the cloud settings are the same for different size
	// maps, like clear on 128x128 should have far less clouds than
	// clear on 800x800.

	float cloudCount = MapSelectionView::mapList[MapSelectionView::curMap].cells.getArea() / baseTileCountPerCloud;
	float randCount  = rand() % int(cloudCount / randomDivisorOfBase);
	
	cloudCount += randCount;

	float clearCloudCount         = float(cloudCount) * clearPercentOfBase;
	float brokenCloudCount        = float(cloudCount) * brokenPercentOfBase;
	float partlyCloudyCloudCount  = float(cloudCount) * partlyCloudyPercentOfBase;
	float overcastCloudCount      = float(cloudCount) * overcastPercentOfBase;
	float fuckingCloudyCloudCount = float(cloudCount) * extremelyCloudyPercentOfBase;

	switch (cloudCoverageCount)
	{
		case 0:
		{
			cloudCoverageString = "Clear";
			GameConfig::setCloudCoverage(clearCloudCount);
		} break;
		case 1:
		{
			cloudCoverageString = "Broken";
			GameConfig::setCloudCoverage(brokenCloudCount);
		} break;
		case 2:
		{
			cloudCoverageString = "Partly Cloudy";
			GameConfig::setCloudCoverage(partlyCloudyCloudCount);
		} break;
		case 3:
		{
			cloudCoverageString = "Overcast";
			GameConfig::setCloudCoverage(overcastCloudCount);
		} break;
		case 4:
		{
			cloudCoverageString = "Extremely Cloudy";
			GameConfig::setCloudCoverage(fuckingCloudyCloudCount);
		} break;
	}
}

void HostOptionsView::updateGameConfigGameType()
 {
  switch (gameType)
   {
    case 0:
     {
      GameConfig::SetGameType( _gametype_objective );
     }break;

    case 1:
     {
      GameConfig::SetGameType( _gametype_fraglimit );
     }break;

    case 2:
     {
      GameConfig::SetGameType( _gametype_timelimit );
     }break;
   
   }

 }

static char * getGameTypeString()
 {
  switch ( GameConfig::GetGameType() )
   {
    case _gametype_objective:
     {
      return( "Objective" );
     }break;

    case _gametype_fraglimit :
     {
      return( "Time Limit" );
     }break;

    case _gametype_timelimit :
     {
      return( "Frag Limit" );
     }break;   
   
   }
  return( "Unknown" );
 }

static void bSetAllowFogOfWarFalse()
{
	//GameConfig::setAllowFogOfWar(false);
}

static void bSetAllowFogOfWarTrue()
{
	//GameConfig::setAllowFogOfWar(true);
}

static void bIncreaseWindSpeed()
{
	HostOptionsView::setWindSpeed(HostOptionsView::getWindSpeed() + 1);
	
	if(HostOptionsView::getWindSpeed() > 4)
	{
		HostOptionsView::setWindSpeed(4);
	}
	
	HostOptionsView::updateWindSpeedString();
}

static void bDecreaseWindSpeed()
{
	HostOptionsView::setWindSpeed(HostOptionsView::getWindSpeed() - 1);
	
	if(HostOptionsView::getWindSpeed() < 0)
	{
		HostOptionsView::setWindSpeed(0);
	}
	
	HostOptionsView::updateWindSpeedString();
}

static void bIncreaseTimeLimit()
{
 GameConfig::SetTimeLimit( GameConfig::GetTimeLimit() + 5 );
}

static void bDecreaseTimeLimit()
{
 GameConfig::SetTimeLimit( GameConfig::GetTimeLimit() - 5 );
}

static int getTimeLimitHours()
 {
  return( GameConfig::GetTimeLimit() / 60 );
 }

static int getTimeLimitMinutes()
 {
  return( GameConfig::GetTimeLimit() % 60 );
 }

static void bIncreaseFragLimit()
 {
  GameConfig::SetFragLimit( GameConfig::GetFragLimit() + 5 );
 }

static void bDecreaseFragLimit()
 {
  GameConfig::SetFragLimit( GameConfig::GetFragLimit() - 5 );
 }

static int getFragLimit()
 {
  return( GameConfig::GetFragLimit() );
 }

void HostOptionsView::updateWindSpeedString()
{
	float calmWindSpeed    = float(baseWindSpeed) * calmWindsPercentOfBase;
	float breezyWindSpeed  = float(baseWindSpeed) * breezyWindsPercentOfBase;
	float briskWindSpeed   = float(baseWindSpeed) * briskWindsPercentOfBase;
	float heavyWindSpeed   = float(baseWindSpeed) * heavyWindsPercentOfBase;
	float typhoonWindSpeed = float(baseWindSpeed) * typhoonWindsPercentOfBase;
	
	switch (windSpeed)
	{
		case 0:
		{
			windSpeedString = "Calm";
			GameConfig::setWindSpeed(calmWindSpeed);
		} break;
		case 1:
		{
			windSpeedString = "Breezy";
			GameConfig::setWindSpeed(breezyWindSpeed);
		} break;
		case 2:
		{
			windSpeedString = "Brisk Winds";
			GameConfig::setWindSpeed(briskWindSpeed);
		} break;
		case 3:
		{
			windSpeedString = "Heavy Winds";
			GameConfig::setWindSpeed(heavyWindSpeed);
		} break;
		case 4:
		{
			windSpeedString = "Typhoon";
			GameConfig::setWindSpeed(typhoonWindSpeed);
		} break;
	}
}

static int getObjectiveCapturePercent()
{
	return( GameConfig::getObjectiveOccuapationPercentage() ); 
}

static void bIncreaseObjectiveCapturePercent()
{
	GameConfig::increaseObjectiveOccuapationPercentage();
}

static void bDecreaseObjectiveCapturePercent()
{
	GameConfig::decreaseObjectiveOccuapationPercentage();
}


// HostOptionsView
//---------------------------------------------------------------------------
HostOptionsView::HostOptionsView() : RMouseHackView()
{
	setSearchName("HostOptionsView");
	setTitle("Host Options");
	setSubTitle("");

	setAllowResize(false);
	setAllowMove(false);
	setVisible(false);

	moveTo(bodyTextRect.min.x, bodyTextRect.min.y + 190);
	resizeClientArea(bodyTextRect.getSizeX(), 168);

	addMeterButtons(iXY(BORDER_SPACE, BORDER_SPACE));

} // end HostOptionsView::HostOptionsView

// doDraw
//---------------------------------------------------------------------------
void HostOptionsView::doDraw(const Surface &viewArea, const Surface &clientArea)
{
	drawMeterInfo(clientArea, iXY(BORDER_SPACE, BORDER_SPACE));

	iXY pos;

	pos.x = 4;
	pos.y = clientArea.getPixY() - CHAR_YPIX;
	
	clientArea.bltString(pos, "Note: Use the right mouse button to accomplish fast mouse clicking.", Color::white);

	View::doDraw(viewArea, clientArea);

} // end HostOptionsView::doDraw


// addMeterButtons
//---------------------------------------------------------------------------
void HostOptionsView::addMeterButtons(const iXY &pos)
{
	const int yOffset          = 15;
	const int arrowButtonWidth = 16;

	int x;
	int y = pos.y;

	int xTextStart    = pos.x;
	int xControlStart = 270;

	x = xTextStart;
	addLabelShadowed(iXY(x, y), "Max Players", windowTextColor, windowTextColorShadow);
	x += xControlStart;
	addButtonCenterText(iXY(x - 1, y), arrowButtonWidth, "<", "", bDecreasePlayerCount);
	x += arrowButtonWidth + meterWidth;
	addButtonCenterText(iXY(x + 1, y), arrowButtonWidth, ">", "", bIncreasePlayerCount);
	y += yOffset;

	x = xTextStart;
	addLabelShadowed(iXY(x, y), "Game Max Unit Count", windowTextColor, windowTextColorShadow);
	x += xControlStart;
	addButtonCenterText(iXY(x - 1, y), arrowButtonWidth, "<", "", bDecreaseMaxUnitCount);
	x += arrowButtonWidth + meterWidth;
	addButtonCenterText(iXY(x + 1, y), arrowButtonWidth, ">", "", bIncreaseMaxUnitCount);
	y += yOffset;
/*
	x = xTextStart;
	addLabelShadowed(iXY(x, y), "Player Respawn Unit Count", windowTextColor, windowTextColorShadow);
	x += xControlStart;
	addButtonCenterText(iXY(x - 1, y), arrowButtonWidth, "<", "", bDecreaseRespawnCount);
	x += arrowButtonWidth + meterWidth;
	addButtonCenterText(iXY(x + 1, y), arrowButtonWidth, ">", "", bIncreaseRespawnCount);
	y += yOffset;
*/
	//x = xTextStart;
	//addLabelShadowed(iXY(x, y), "Allow Allies", windowTextColor, windowTextColorShadow);
	//x += xControlStart;
	//addButtonCenterText(iXY(x - 1, y), arrowButtonWidth, "<", "", bSetAllowAlliesFalse);
	//x += arrowButtonWidth + meterWidth;
	//addButtonCenterText(iXY(x + 1, y), arrowButtonWidth, ">", "", bSetAllowAlliesTrue);
	//y += yOffset;
	//
	//x = xTextStart;
	//addLabelShadowed(iXY(x, y), "Spawn Placement", windowTextColor, windowTextColorShadow);
	//x += xControlStart;
	//addButtonCenterText(iXY(x - 1, y), arrowButtonWidth, "<", "", bPreviousSpawnPlacement);
	//x += arrowButtonWidth + meterWidth;
	//addButtonCenterText(iXY(x + 1, y), arrowButtonWidth, ">", "", bNextSpawnPlacement);
	//y += yOffset;

	x = xTextStart;
	addLabelShadowed(iXY(x, y), "Objective Capture Percent", windowTextColor, windowTextColorShadow);
	x += xControlStart;
	addButtonCenterText(iXY(x - 1, y), arrowButtonWidth, "<", "", bDecreaseObjectiveCapturePercent);
	x += arrowButtonWidth + meterWidth;
	addButtonCenterText(iXY(x + 1, y), arrowButtonWidth, ">", "", bIncreaseObjectiveCapturePercent);
	y += yOffset;

	x = xTextStart;
	addLabelShadowed(iXY(x, y), "Time Limit", windowTextColor, windowTextColorShadow);
	x += xControlStart;
	addButtonCenterText(iXY(x - 1, y), arrowButtonWidth, "<", "", bDecreaseTimeLimit);
	x += arrowButtonWidth + meterWidth;
	addButtonCenterText(iXY(x + 1, y), arrowButtonWidth, ">", "", bIncreaseTimeLimit);
	y += yOffset;

	x = xTextStart;
	addLabelShadowed(iXY(x, y), "Frag Limit", windowTextColor, windowTextColorShadow);
	x += xControlStart;
	addButtonCenterText(iXY(x - 1, y), arrowButtonWidth, "<", "", bDecreaseFragLimit);
	x += arrowButtonWidth + meterWidth;
	addButtonCenterText(iXY(x + 1, y), arrowButtonWidth, ">", "", bIncreaseFragLimit);
	y += yOffset;

	const int minWidth = 150;
    int xChoiceOffset = 2;

	choiceGameType.setName("Game Type");
	choiceGameType.addItem("Objective");
	choiceGameType.addItem("Frag Limit");
	choiceGameType.addItem("Time Limit");
	choiceGameType.setMinWidth(minWidth);
	choiceGameType.setLocation(xChoiceOffset, 100);
	choiceGameType.select( getGameTypeString() );
	add(&choiceGameType);
    xChoiceOffset += minWidth + 123;

	choiceCloudCoverage.setName("Cloud Coverage");
	choiceCloudCoverage.addItem("Clear");
	choiceCloudCoverage.addItem("Broken");
	choiceCloudCoverage.addItem("Partly Cloudy");
	choiceCloudCoverage.addItem("Overcast");
	choiceCloudCoverage.addItem("Extremely Cloudy");
	choiceCloudCoverage.setMinWidth(minWidth);
	choiceCloudCoverage.setLocation(xChoiceOffset, 100);
	choiceCloudCoverage.select(cloudCoverageCount);
	add(&choiceCloudCoverage);
    xChoiceOffset += minWidth + 13;

	choiceWindSpeed.setName("Wind Speed");
	choiceWindSpeed.addItem("Calm");
	choiceWindSpeed.addItem("Breezy");
	choiceWindSpeed.addItem("Brisk");
	choiceWindSpeed.addItem("Heavy");
	choiceWindSpeed.addItem("Typhoon");
	choiceWindSpeed.setMinWidth(minWidth);
	choiceWindSpeed.setLocation(xChoiceOffset, 100);
	choiceWindSpeed.select(windSpeed);
	add(&choiceWindSpeed);
    xChoiceOffset += minWidth + 10;

    checkMapCycle.setLabel("Cycle Maps");
	checkMapCycle.setState( (bool) (GameConfig::getMapCycleState()) );
	checkMapCycle.setLocation(2, 125);
	add(&checkMapCycle);

    checkPowerUp.setLabel("PowerUps");
	checkPowerUp.setState( (bool) (GameConfig::getPowerUpState()) );
	checkPowerUp.setLocation(120, 125);
	add(&checkPowerUp);


/*
	x = xTextStart;
	addLabelShadowed(iXY(x, y), "Allow Fog Of War", windowTextColor, windowTextColorShadow);
	x += xControlStart;
	addButtonCenterText(iXY(x - 1, y), arrowButtonWidth, "<", "", bSetAllowFogOfWarFalse);
	x += arrowButtonWidth + meterWidth;
	addButtonCenterText(iXY(x + 1, y), arrowButtonWidth, ">", "", bSetAllowFogOfWarTrue);
*/
} // end HostOptionsView::addMeterButtons

// drawMeterInfo
//---------------------------------------------------------------------------
void HostOptionsView::drawMeterInfo(const Surface &dest, const iXY &pos)
{
	char strBuf[256];

	const int xTextStart       = pos.x;
	const int arrowButtonWidth = 16;
	const int yOffset          = 15;
	const int buttonStartY     = pos.y;

	// Draw a border around the meters.
	iXY shit(30, buttonStartY - 20);
	//iXY size(SCREEN_XPIX - 60, 154);
	//iXY size(SCREEN_XPIX - 60, 140);
	//iRect meterBorder(shit.x, shit.y, shit.x + size.x, shit.y + size.y);
	//dest.bltLookup(meterBorder, Palette::darkGray256.getColorArray());
	//dest.drawButtonBorder(meterBorder, Color::lightGreen, Color::darkGreen);

	int x = pos.x + 270 + arrowButtonWidth;
	int y = pos.y;

	Surface tempSurface(meterWidth, 14, meterWidth, 1);
	tempSurface.fill(meterColor);

	// Game Max Player Count
	tempSurface.drawButtonBorder(meterTopLeftBorderColor, meterBottomRightBorderColor);
	sprintf(strBuf, "%d", getCurMaxPlayersCount());
	tempSurface.bltStringCenter(strBuf, meterTextColor);
	tempSurface.blt(dest, x, y);
/*
	meterUsageXSize = tempSurface.getPixX() / getCurMaxPlayersCount();
	if (meterUsageXSize < 3) { meterUsageXSize = 3; }
	visualMeter = iRect(0, 0, meterUsageXSize, 14);
	percent = getCurMaxPlayersCount() / getMaxPlayersCount();
	visualMeter.translate(iXY(percent * tempSurface.getPixX(), 0));
	tempSurface.fillRect(visualMeter, Color::red);
*/

	// Game Max Unit Count
	y += yOffset;
	tempSurface.fill(meterColor);
	tempSurface.drawButtonBorder(meterTopLeftBorderColor, meterBottomRightBorderColor);
	sprintf(strBuf, "%d - %d max per player", getCurMaxUnitCount(), getCurMaxUnitCount() / getCurMaxPlayersCount());
	tempSurface.bltStringCenter(strBuf, meterTextColor);
	tempSurface.blt(dest, x, y);
/*
	// Respawn Unit Count
	y += yOffset;
	tempSurface.fill(meterColor);
	tempSurface.drawButtonBorder(meterTopLeftBorderColor, meterBottomRightBorderColor);
	sprintf(strBuf, "%d", getRespawnUnitCount());
	tempSurface.bltStringCenter(strBuf, meterTextColor);
	tempSurface.blt(dest, x, y);
*/
	// Allow Allies
	//y += yOffset;
	//tempSurface.fill(meterColor);
	//tempSurface.drawButtonBorder(meterTopLeftBorderColor, meterBottomRightBorderColor);
	//sprintf(strBuf, "%s", getAllowAllies());
	//tempSurface.bltStringCenter(strBuf, meterTextColor);
	//tempSurface.blt(dest, x, y);
	//
	//// Spawn Placement
	//y += yOffset;
	//tempSurface.fill(meterColor);
	//tempSurface.drawButtonBorder(meterTopLeftBorderColor, meterBottomRightBorderColor);
	//sprintf(strBuf, "%s", getSpawnPlacement());
	//tempSurface.bltStringCenter(strBuf, meterTextColor);
	//tempSurface.blt(dest, x, y);

	// Objective Capture Percent
	y += yOffset;
	tempSurface.fill(meterColor);
	tempSurface.drawButtonBorder(meterTopLeftBorderColor, meterBottomRightBorderColor);
	if (MapSelectionView::curMap >= 0 && MapSelectionView::mapList.getCount() > 0)
	{
		int objectiveCount = MapSelectionView::mapList[MapSelectionView::curMap].objectiveCount;
		sprintf(strBuf, "%d%% - %d of %d", getObjectiveCapturePercent(), int(float(objectiveCount) * (float(getObjectiveCapturePercent()) / 100.0f)), objectiveCount);
	} else
	{
		sprintf(strBuf, "Map Data Needed");
	}
	tempSurface.bltStringCenter(strBuf, meterTextColor);
	tempSurface.blt(dest, x, y);

	y += yOffset;
	tempSurface.fill(meterColor);
	tempSurface.drawButtonBorder(meterTopLeftBorderColor, meterBottomRightBorderColor);
	sprintf(strBuf, "%d:%d", getTimeLimitHours(), getTimeLimitMinutes() );
	tempSurface.bltStringCenter(strBuf, meterTextColor);
	tempSurface.blt(dest, x, y);

	y += yOffset;
	tempSurface.fill(meterColor);
	tempSurface.drawButtonBorder(meterTopLeftBorderColor, meterBottomRightBorderColor);
	sprintf(strBuf, "%d Frags", getFragLimit() );
	tempSurface.bltStringCenter(strBuf, meterTextColor);
	tempSurface.blt(dest, x, y);


/*
	// Fog of War
	y += yOffset;
	tempSurface.fill(meterColor);
	tempSurface.drawButtonBorder(meterTopLeftBorderColor, meterBottomRightBorderColor);
	sprintf(strBuf, "%s", getAllowFogOfWar());
	tempSurface.bltStringCenter(strBuf, meterTextColor);
	tempSurface.blt(dest, x, y);
*/

} // end HostOptionsView::drawMeterInfo

// actionPerformed
//---------------------------------------------------------------------------
void HostOptionsView::actionPerformed(mMouseEvent me)
{
	if (me.getSource(checkMapCycle)) 
	{
	  if ( getVisible() )	
       {
         if (checkMapCycle.getState())
		  {
	  		GameConfig::setMapCycleState( true );
		  } else
		  {
			GameConfig::setMapCycleState( false );
		  }
	   }
    }
    else if (me.getSource(checkPowerUp)) 
    {
	  if ( getVisible() )	
       {
         if (checkPowerUp.getState())
		  {
	  		GameConfig::setPowerUpState( true );
		  } else
		  {
			GameConfig::setPowerUpState( false );
		  }
	   }
    }
	else if (me.getSource(choiceWindSpeed))
	{
		windSpeed = choiceWindSpeed.getSelectedIndex();
		
		updateWindSpeedString();
	}
	else if (me.getSource(choiceCloudCoverage))
	{
		cloudCoverageCount = choiceCloudCoverage.getSelectedIndex();
		
		updateGameConfigCloudCoverage();
	}
    else if (me.getSource(choiceGameType))
    {
	  if ( getVisible() )	
       {
		gameType = choiceGameType.getSelectedIndex();
		
		updateGameConfigGameType();
	   }
    }
} // end HostOptionsView::actionPerformed
