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
#include "Interfaces/GameConfig.hpp"
#include "HostView.hpp"
#include "Particles/ParticleSystemGlobals.hpp"
#include "MapSelectionView.hpp"
#include "Views/GameViewGlobals.hpp"
#include "Views/Components/Label.hpp"

int HostOptionsView::cloudCoverageCount = 1;
int HostOptionsView::windSpeed          = 1;
int HostOptionsView::gameType           = 0;

std::string HostOptionsView::cloudCoverageString;
std::string HostOptionsView::windSpeedString;

enum
{
    DEC_PLAYERCOUNT,
    INC_PLAYERCOUNT,
    DEC_MAXUNITS,
    INC_MAXUNITS,
    DEC_OBJPERCENT,
    INC_OBJPERCENT,
    DEC_TIMELIMIT,
    INC_TIMELIMIT,
    DEC_FRAGLIMIT,
    INC_FRAGLIMIT
};

static int getCurMaxPlayersCount()
{
    return gameconfig->maxplayers;
}

static int  getCurMaxUnitCount()
{
    return gameconfig->maxunits;
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

static int getTimeLimitHours()
{
    return gameconfig->timelimit / 60;
}

static int getTimeLimitMinutes()
{
    return gameconfig->timelimit % 60;
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

// HostOptionsView
//---------------------------------------------------------------------------
HostOptionsView::HostOptionsView() : RMouseHackView()
{
    setSearchName("HostOptionsView");
    setTitle("Host Options");
    setSubTitle("");

    setAllowMove(false);
    setVisible(false);

    moveTo(bodyTextRect.min.x, bodyTextRect.min.y + 190);
    resize(bodyTextRect.getSizeX(), 168);

    addMeterButtons(iXY(BORDER_SPACE, BORDER_SPACE));

} // end HostOptionsView::HostOptionsView

// doDraw
//---------------------------------------------------------------------------
void HostOptionsView::doDraw()
{
    drawMeterInfo(iXY(BORDER_SPACE, BORDER_SPACE));

    drawString( 4, clientRect.getSizeY() - Surface::getFontHeight(),
                    "Note: Use the right mouse button to accomplish fast mouse clicking.",
                    Color::white);

    View::doDraw();

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
    add( new Label(x, y, "Max Players", windowTextColor, windowTextColorShadow, true) );

    x += xControlStart;
    add( Button::createTextButton("decplayer","<",iXY(x - 1, y),arrowButtonWidth, DEC_PLAYERCOUNT));
    x += arrowButtonWidth + meterWidth;
    add( Button::createTextButton("incplayer",">",iXY(x + 1, y),arrowButtonWidth, INC_PLAYERCOUNT));

    y += yOffset;

    x = xTextStart;
    add( new Label(x, y, "Game Max Unit Count", windowTextColor, windowTextColorShadow, true) );
    x += xControlStart;
    add( Button::createTextButton("decunits","<",iXY(x - 1, y),arrowButtonWidth, DEC_MAXUNITS));
    x += arrowButtonWidth + meterWidth;
    add( Button::createTextButton("incunits",">",iXY(x + 1, y),arrowButtonWidth, INC_MAXUNITS));

    y += yOffset;
    /*
    	x = xTextStart;
    	//addLabelShadowed(iXY(x, y), "Player Respawn Unit Count", windowTextColor, windowTextColorShadow);
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
    add( new Label( x, y, "Objective Capture Percent", windowTextColor, windowTextColorShadow, true) );
    x += xControlStart;
    add( Button::createTextButton("decobjpercent","<",iXY(x - 1, y),arrowButtonWidth, DEC_OBJPERCENT));
    x += arrowButtonWidth + meterWidth;
    add( Button::createTextButton("incobjpercent",">",iXY(x + 1, y),arrowButtonWidth, INC_OBJPERCENT));
    y += yOffset;

    x = xTextStart;
    add( new Label( x, y, "Time Limit", windowTextColor, windowTextColorShadow, true) );
    x += xControlStart;
    add( Button::createTextButton("dectime","<",iXY(x - 1, y),arrowButtonWidth, DEC_TIMELIMIT));
    x += arrowButtonWidth + meterWidth;
    add( Button::createTextButton("inctime",">",iXY(x + 1, y),arrowButtonWidth, INC_TIMELIMIT));
    y += yOffset;

    x = xTextStart;
    add( new Label( x, y, "Frag Limit", windowTextColor, windowTextColorShadow, true) );
    x += xControlStart;
    add( Button::createTextButton("decfrag","<",iXY(x - 1, y),arrowButtonWidth, DEC_FRAGLIMIT));
    x += arrowButtonWidth + meterWidth;
    add( Button::createTextButton("incfrag",">",iXY(x + 1, y),arrowButtonWidth, INC_FRAGLIMIT));
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
    	//addLabelShadowed(iXY(x, y), "Allow Fog Of War", windowTextColor, windowTextColorShadow);
    	x += xControlStart;
    	addButtonCenterText(iXY(x - 1, y), arrowButtonWidth, "<", "", bSetAllowFogOfWarFalse);
    	x += arrowButtonWidth + meterWidth;
    	addButtonCenterText(iXY(x + 1, y), arrowButtonWidth, ">", "", bSetAllowFogOfWarTrue);
    */
} // end HostOptionsView::addMeterButtons

// drawMeterInfo
//---------------------------------------------------------------------------
void HostOptionsView::drawMeterInfo( const iXY &pos)
{
    char strBuf[256];

    const int arrowButtonWidth = 16;
    const int yOffset          = 15;

    int x = pos.x + 270 + arrowButtonWidth;
    int y = pos.y;

    Surface tempSurface(meterWidth, 14, 1);
    tempSurface.fill(meterColor);

    // Game Max Player Count
    tempSurface.drawButtonBorder(meterTopLeftBorderColor, meterBottomRightBorderColor);
    sprintf(strBuf, "%d", getCurMaxPlayersCount());
    tempSurface.bltStringCenter(strBuf, meterTextColor);
    drawImage(tempSurface, x, y);

    // Game Max Unit Count
    y += yOffset;
    tempSurface.fill(meterColor);
    tempSurface.drawButtonBorder(meterTopLeftBorderColor, meterBottomRightBorderColor);
    sprintf(strBuf, "%d - %d max per player", getCurMaxUnitCount(), getCurMaxUnitCount() / getCurMaxPlayersCount());
    tempSurface.bltStringCenter(strBuf, meterTextColor);
    drawImage(tempSurface, x, y);
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
    drawImage(tempSurface, x, y);

    y += yOffset;
    tempSurface.fill(meterColor);
    tempSurface.drawButtonBorder(meterTopLeftBorderColor, meterBottomRightBorderColor);
    sprintf(strBuf, "%d:%d", getTimeLimitHours(), getTimeLimitMinutes() );
    tempSurface.bltStringCenter(strBuf, meterTextColor);
    drawImage(tempSurface, x, y);

    y += yOffset;
    tempSurface.fill(meterColor);
    tempSurface.drawButtonBorder(meterTopLeftBorderColor, meterBottomRightBorderColor);
    sprintf(strBuf, "%d Frags", getFragLimit() );
    tempSurface.bltStringCenter(strBuf, meterTextColor);
    drawImage(tempSurface, x, y);


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
    if (me.getSource()==&checkPublic) {
        if ( getVisible() ) {
            gameconfig->publicServer = checkPublic.getState();
        }
    } else if (me.getSource()==&checkPowerUp) {
        if ( getVisible() ) {
            gameconfig->powerups = checkPowerUp.getState();
        }
    } else if (me.getSource()==&choiceWindSpeed) {
        windSpeed = choiceWindSpeed.getSelectedIndex();

        updateWindSpeedString();
    } else if (me.getSource()==&choiceCloudCoverage) {
        cloudCoverageCount = choiceCloudCoverage.getSelectedIndex();

        updateGameConfigCloudCoverage();
    } else if (me.getSource()==&choiceGameType) {
        if ( getVisible() ) {
            gameType = choiceGameType.getSelectedIndex();

            updateGameConfigGameType();
        }
    }
} // end HostOptionsView::actionPerformed

void
HostOptionsView::onComponentClicked(Component* c)
{
    switch ( c->getCustomCode() )
    {
        case DEC_PLAYERCOUNT:
            if(gameconfig->maxplayers - 1 >= gameconfig->maxplayers.getMin())
            {
                gameconfig->maxplayers = gameconfig->maxplayers - 1;
            }
            break;

        case INC_PLAYERCOUNT:
            if(gameconfig->maxplayers + 1 <= gameconfig->maxplayers.getMax())
            {
                gameconfig->maxplayers = gameconfig->maxplayers + 1;
            }
            break;

        case DEC_MAXUNITS:
            if(gameconfig->maxunits - 5 >= gameconfig->maxunits.getMin())
            {
                gameconfig->maxunits = gameconfig->maxunits - 5;
            }
            break;

        case INC_MAXUNITS:
            if(gameconfig->maxunits + 5 <= gameconfig->maxunits.getMax())
            {
                gameconfig->maxunits = gameconfig->maxunits + 5;
            }
            break;

        case DEC_OBJPERCENT:
            if(gameconfig->objectiveoccupationpercentage - 5 >=
                            gameconfig->objectiveoccupationpercentage.getMin())
            {
                gameconfig->objectiveoccupationpercentage =
                                gameconfig->objectiveoccupationpercentage - 5;
            }
            break;

        case INC_OBJPERCENT:
            if(gameconfig->objectiveoccupationpercentage + 5 <=
                            gameconfig->objectiveoccupationpercentage.getMax())
            {
                gameconfig->objectiveoccupationpercentage =
                                gameconfig->objectiveoccupationpercentage + 5;
            }
            break;

        case DEC_TIMELIMIT:
            if(gameconfig->timelimit + 5 <= gameconfig->timelimit.getMax())
            {
                gameconfig->timelimit = gameconfig->timelimit + 5;
            }
            break;

        case INC_TIMELIMIT:
            if(gameconfig->timelimit - 5 >= gameconfig->timelimit.getMin())
            {
                gameconfig->timelimit = gameconfig->timelimit - 5;
            }
            break;

        case DEC_FRAGLIMIT:
            if(gameconfig->fraglimit - 5 >= gameconfig->fraglimit.getMin())
            {
                gameconfig->fraglimit = gameconfig->fraglimit - 5;
            }
            break;

        case INC_FRAGLIMIT:
            if(gameconfig->fraglimit + 5 <= gameconfig->fraglimit.getMax())
            {
                gameconfig->fraglimit = gameconfig->fraglimit + 5;
            }
            break;
    }
}
