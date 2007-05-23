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
#include "HostOptionsView.hpp"
#include "GameConfig.hpp"
#include "HostView.hpp"
#include "ParticleSystemGlobals.hpp"
#include "MapSelectionView.hpp"
#include "GameViewGlobals.hpp"

int HostOptionsView::cloudCoverageCount = 1;
int HostOptionsView::windSpeed          = 1;
int HostOptionsView::gameType           = 0;

std::string HostOptionsView::cloudCoverageString;
std::string HostOptionsView::windSpeedString;

static int getCurMaxPlayersCount()
{
    return gameconfig->maxplayers;
}

static int  getCurMaxUnitCount()
{
    return gameconfig->maxunits;
}

static void bDecreasePlayerCount()
{
    if(gameconfig->maxplayers - 1 >= gameconfig->maxplayers.getMin())
        gameconfig->maxplayers = gameconfig->maxplayers - 1;
}

static void bIncreasePlayerCount()
{
    if(gameconfig->maxplayers + 1 <= gameconfig->maxplayers.getMax())
        gameconfig->maxplayers = gameconfig->maxplayers + 1;
}

static void bDecreaseMaxUnitCount()
{
    if(gameconfig->maxunits - 5 >= gameconfig->maxunits.getMin())
        gameconfig->maxunits = gameconfig->maxunits - 5;
}

static void bIncreaseMaxUnitCount()
{
    if(gameconfig->maxunits + 5 <= gameconfig->maxunits.getMax())
        gameconfig->maxunits = gameconfig->maxunits + 5;
}

void HostOptionsView::updateGameConfigCloudCoverage()
{
    // Make sure the cloud settings are the same for different size
    // maps, like clear on 128x128 should have far less clouds than
    // clear on 800x800.

    float cloudCount = MapSelectionView::mapList[MapSelectionView::curMap]->cells.getArea() / baseTileCountPerCloud;
    float randCount  = rand() % int(cloudCount / randomDivisorOfBase);

    cloudCount += randCount;

    float clearCloudCount         = float(cloudCount) * clearPercentOfBase;
    float brokenCloudCount        = float(cloudCount) * brokenPercentOfBase;
    float partlyCloudyCloudCount  = float(cloudCount) * partlyCloudyPercentOfBase;
    float overcastCloudCount      = float(cloudCount) * overcastPercentOfBase;
    float fuckingCloudyCloudCount = float(cloudCount) * extremelyCloudyPercentOfBase;

    switch (cloudCoverageCount) {
    case 0: {
            cloudCoverageString = "Clear";
            gameconfig->cloudcoverage = (int(clearCloudCount));
        }
        break;
    case 1: {
            cloudCoverageString = "Broken";
            gameconfig->cloudcoverage = (int(brokenCloudCount));
        }
        break;
    case 2: {
            cloudCoverageString = "Partly Cloudy";
            gameconfig->cloudcoverage = (int(partlyCloudyCloudCount));
        }
        break;
    case 3: {
            cloudCoverageString = "Overcast";
            gameconfig->cloudcoverage = (int(overcastCloudCount));
        }
        break;
    case 4: {
            cloudCoverageString = "Extremely Cloudy";
            gameconfig->cloudcoverage = (int(fuckingCloudyCloudCount));
        }
        break;
    }
}

void HostOptionsView::updateGameConfigGameType()
{
    switch (gameType) {
    case 0: {
            gameconfig->gametype = _gametype_objective;
        }
        break;

    case 1: {
            gameconfig->gametype = _gametype_fraglimit;
        }
        break;

    case 2: {
            gameconfig->gametype = _gametype_timelimit;
        }
        break;

    }

}

static char * getGameTypeString()
{
    switch ( gameconfig->gametype ) {
    case _gametype_objective: {
            return( "Objective" );
        }
        break;

    case _gametype_fraglimit : {
            return( "Time Limit" );
        }
        break;

    case _gametype_timelimit : {
            return( "Frag Limit" );
        }
        break;

    }
    return( "Unknown" );
}

static void bIncreaseTimeLimit()
{
    if(gameconfig->timelimit + 5 <= gameconfig->timelimit.getMax())
        gameconfig->timelimit = gameconfig->timelimit + 5;
}

static void bDecreaseTimeLimit()
{
    if(gameconfig->timelimit - 5 >= gameconfig->timelimit.getMin())
        gameconfig->timelimit = gameconfig->timelimit - 5;
}

static int getTimeLimitHours()
{
    return gameconfig->timelimit / 60;
}

static int getTimeLimitMinutes()
{
    return gameconfig->timelimit % 60;
}

static void bIncreaseFragLimit()
{
    if(gameconfig->fraglimit + 5 <= gameconfig->fraglimit.getMax())
        gameconfig->fraglimit = gameconfig->fraglimit + 5;
}

static void bDecreaseFragLimit()
{
    if(gameconfig->fraglimit - 5 >= gameconfig->fraglimit.getMin())
        gameconfig->fraglimit = gameconfig->fraglimit - 5;
}

static int getFragLimit()
{
    return gameconfig->fraglimit;
}

void HostOptionsView::updateWindSpeedString()
{
    float calmWindSpeed    = float(baseWindSpeed) * calmWindsPercentOfBase;
    float breezyWindSpeed  = float(baseWindSpeed) * breezyWindsPercentOfBase;
    float briskWindSpeed   = float(baseWindSpeed) * briskWindsPercentOfBase;
    float heavyWindSpeed   = float(baseWindSpeed) * heavyWindsPercentOfBase;
    float typhoonWindSpeed = float(baseWindSpeed) * typhoonWindsPercentOfBase;

    switch (windSpeed) {
    case 0: {
            windSpeedString = "Calm";
            gameconfig->windspeed = int(calmWindSpeed);
        }
        break;
    case 1: {
            windSpeedString = "Breezy";
            gameconfig->windspeed = int(breezyWindSpeed);
        }
        break;
    case 2: {
            windSpeedString = "Brisk Winds";
            gameconfig->windspeed = int(briskWindSpeed);
        }
        break;
    case 3: {
            windSpeedString = "Heavy Winds";
            gameconfig->windspeed = int(heavyWindSpeed);
        }
        break;
    case 4: {
            windSpeedString = "Typhoon";
            gameconfig->windspeed = int(typhoonWindSpeed);
        }
        break;
    }
}

static int getObjectiveCapturePercent()
{
    return gameconfig->objectiveoccupationpercentage;
}

static void bIncreaseObjectiveCapturePercent()
{
    if(gameconfig->objectiveoccupationpercentage + 5 <=
            gameconfig->objectiveoccupationpercentage.getMax())
        gameconfig->objectiveoccupationpercentage =
            gameconfig->objectiveoccupationpercentage + 5;
}

static void bDecreaseObjectiveCapturePercent()
{
    if(gameconfig->objectiveoccupationpercentage - 5 >=
            gameconfig->objectiveoccupationpercentage.getMin())
        gameconfig->objectiveoccupationpercentage = 
            gameconfig->objectiveoccupationpercentage - 5;
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
void HostOptionsView::doDraw(Surface &viewArea, Surface &clientArea)
{
    drawMeterInfo(clientArea, iXY(BORDER_SPACE, BORDER_SPACE));

    clientArea.bltString( 4, clientArea.getHeight() - Surface::getFontHeight(),
                    "Note: Use the right mouse button to accomplish fast mouse clicking.",
                    Color::white);

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

    checkPublic.setLabel("Public");
    checkPublic.setState(gameconfig->publicServer);
    checkPublic.setLocation(2, 125);
    add(&checkPublic);

    checkPowerUp.setLabel("PowerUps");
    checkPowerUp.setState(gameconfig->powerups);
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
void HostOptionsView::drawMeterInfo(Surface &dest, const iXY &pos)
{
    char strBuf[256];

    const int arrowButtonWidth = 16;
    const int yOffset          = 15;

    int x = pos.x + 270 + arrowButtonWidth-1; // xxx hack
    int y = pos.y;

    Surface tempSurface(meterWidth+1, 14, 1);
    tempSurface.fill(meterColor);

    // Game Max Player Count
    tempSurface.drawHLine(0,0,tempSurface.getWidth(),meterTopLeftBorderColor);
    tempSurface.drawHLine(0,tempSurface.getHeight()-1,tempSurface.getWidth(),meterBottomRightBorderColor);
    sprintf(strBuf, "%d", getCurMaxPlayersCount());
    tempSurface.bltStringCenter(strBuf, meterTextColor);
    tempSurface.blt(dest, x, y);

    // Game Max Unit Count
    y += yOffset;
    tempSurface.fill(meterColor);
    tempSurface.drawHLine(0,0,tempSurface.getWidth(),meterTopLeftBorderColor);
    tempSurface.drawHLine(0,tempSurface.getHeight()-1,tempSurface.getWidth(),meterBottomRightBorderColor);
    sprintf(strBuf, "%d - %d max per player", getCurMaxUnitCount(), getCurMaxUnitCount() / getCurMaxPlayersCount());
    tempSurface.bltStringCenter(strBuf, meterTextColor);
    tempSurface.blt(dest, x, y);
    // Objective Capture Percent
    y += yOffset;
    tempSurface.fill(meterColor);
    tempSurface.drawHLine(0,0,tempSurface.getWidth(),meterTopLeftBorderColor);
    tempSurface.drawHLine(0,tempSurface.getHeight()-1,tempSurface.getWidth(),meterBottomRightBorderColor);
    if (MapSelectionView::curMap >= 0 && MapSelectionView::mapList.size() > 0) {
        int objectiveCount =
            MapSelectionView::mapList[MapSelectionView::curMap]->objectiveCount;
        sprintf(strBuf, "%d%% - %d of %d", getObjectiveCapturePercent(),
                int(float(objectiveCount) * (float(getObjectiveCapturePercent()
                            ) / 100.0f) + 0.999), objectiveCount);
    } else {
        sprintf(strBuf, "Map Data Needed");
    }
    tempSurface.bltStringCenter(strBuf, meterTextColor);
    tempSurface.blt(dest, x, y);

    y += yOffset;
    tempSurface.fill(meterColor);
    tempSurface.drawHLine(0,0,tempSurface.getWidth(),meterTopLeftBorderColor);
    tempSurface.drawHLine(0,tempSurface.getHeight()-1,tempSurface.getWidth(),meterBottomRightBorderColor);
    sprintf(strBuf, "%d:%d", getTimeLimitHours(), getTimeLimitMinutes() );
    tempSurface.bltStringCenter(strBuf, meterTextColor);
    tempSurface.blt(dest, x, y);

    y += yOffset;
    tempSurface.fill(meterColor);
    tempSurface.drawHLine(0,0,tempSurface.getWidth(),meterTopLeftBorderColor);
    tempSurface.drawHLine(0,tempSurface.getHeight()-1,tempSurface.getWidth(),meterBottomRightBorderColor);
    sprintf(strBuf, "%d Frags", getFragLimit() );
    tempSurface.bltStringCenter(strBuf, meterTextColor);
    tempSurface.blt(dest, x, y);

} // end HostOptionsView::drawMeterInfo

// actionPerformed
//---------------------------------------------------------------------------
void HostOptionsView::actionPerformed(mMouseEvent me)
{
    if (me.getSource(checkPublic)) {
        if ( getVisible() ) {
            gameconfig->publicServer = checkPublic.getState();
        }
    } else if (me.getSource(checkPowerUp)) {
        if ( getVisible() ) {
            gameconfig->powerups = checkPowerUp.getState();
        }
    } else if (me.getSource(choiceWindSpeed)) {
        windSpeed = choiceWindSpeed.getSelectedIndex();

        updateWindSpeedString();
    } else if (me.getSource(choiceCloudCoverage)) {
        cloudCoverageCount = choiceCloudCoverage.getSelectedIndex();

        updateGameConfigCloudCoverage();
    } else if (me.getSource(choiceGameType)) {
        if ( getVisible() ) {
            gameType = choiceGameType.getSelectedIndex();

            updateGameConfigGameType();
        }
    }
} // end HostOptionsView::actionPerformed
