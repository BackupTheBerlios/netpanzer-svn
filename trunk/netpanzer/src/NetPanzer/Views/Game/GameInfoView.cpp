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

#include "Util/Log.hpp"
#include "GameInfoView.hpp"
#include "GameViewGlobals.hpp"
#include "Desktop.hpp"
#include "GameConfig.hpp"
#include "GameManager.hpp"
#include "UnitInterface.hpp"
#include "PlayerInterface.hpp"
#include "NetworkState.hpp"
#include "ScreenSurface.hpp"
#include "ObjectiveInterface.hpp"

static int getPlayerFrags()
{
    PlayerState *player_state;

    player_state = PlayerInterface::getLocalPlayerState();
    return( (int) player_state->getTotal() );
}

static int getPlayerObjectives()
{
    PlayerState* player_state = PlayerInterface::getLocalPlayerState();
    return player_state->getObjectivesHeld();
}

static const char* getPlayerTime()
{
    static char time_string[256];

    int hrs = (GameManager::getGameTime() / 3600);
    int min = (GameManager::getGameTime() / 60) % 60;

    snprintf(time_string, 256, "%d:%d", hrs, min);

    return time_string;
}

static const char* getTimeLimit()
{
    static char time_string[256];

    int hrs = (gameconfig->timelimit / 60);
    int min = (gameconfig->timelimit % 60);

    sprintf(time_string, "%d:%d", hrs, min);

    return time_string;
}


// GameInfoView
//--------------------------------------------------------------------------
GameInfoView::GameInfoView() : GameTemplateView()
{
    setSearchName("GameInfoView");
    setTitle("Information");
    setSubTitle("");

    setAllowResize(false);
    setAllowMove(false);
    setDisplayStatusBar(false);
    setVisible(false);
    setBordered(true);

    moveTo(iXY(0,0));
    //resizeClientArea(iXY(100, 2 * 2 + 12 * 3 + 8));
    resizeClientArea(iXY(140, 2 * 2 + 4 * 12 + 8));

    if(gameconfig->gameinfoposition.isDefaultValue()) {
        // Start it in the top-right corner.
        iXY pos(screen->getPixX() - getSize().x,0);
        gameconfig->gameinfoposition=pos;
    }
    moveTo(gameconfig->gameinfoposition); 
    checkArea(screen->getPix());
} // end GameInfoView::GameInfoView

// doDraw
//---------------------------------------------------------------------------
void GameInfoView::doDraw(Surface &viewArea, Surface &clientArea)
{
    char unitsBuf[64];
    //char killsBuf[64];
    //char lossesBuf[64];
    char fragsBuf[64];
    char timeBuf[64];
    char pingBuf[64];
    char gameBuf[64];
    char fpsBuf[64];
    char objectiveBuf[64];

    ObjectiveInterface::updatePlayerObjectiveCounts();

    iXY pos(2, 2);

    // Resize the game info rect to nothing so it can be resized based on the text,
    // to be used in it.
    gameInfoRect = getClientRect();
    //gameInfoRect.max.x = 0;

    snprintf(gameBuf, sizeof(gameBuf), "game   %s",
            gameconfig->getGameTypeString() );
    checkGameInfoRect(gameBuf);


    int unitPerPlayer = gameconfig->maxunits / gameconfig->maxplayers;
    sprintf(unitsBuf, "units  %d/%d", int(UnitInterface::getUnitCount(PlayerInterface::getLocalPlayerIndex())), unitPerPlayer);
    checkGameInfoRect(unitsBuf);

    if( gameconfig->gametype == _gametype_fraglimit ) {
        snprintf(fragsBuf, sizeof(fragsBuf), "frags  %d/%d", getPlayerFrags(),
                (int) gameconfig->fraglimit);
        checkGameInfoRect(fragsBuf);
    } else {
        snprintf(fragsBuf, sizeof(fragsBuf), "frags  %d", getPlayerFrags() );
        checkGameInfoRect(fragsBuf);
    }

    if(gameconfig->gametype == _gametype_objective) {
        float percentage 
            = (float) gameconfig->objectiveoccupationpercentage / 100.0;
        int wincount = (int) (
                ((float) ObjectiveInterface::getObjectiveCount()) * percentage
                + 0.999);
        if(wincount == 0)
            wincount = 1;
        snprintf(objectiveBuf, sizeof(objectiveBuf), "Objectives %d/%d",
                getPlayerObjectives(), wincount);
    } else {
        snprintf(objectiveBuf, sizeof(objectiveBuf), "Objectives %d",
                getPlayerObjectives());
    }

    /*
    sprintf(killsBuf, "kills  %d", getPlayerKills());
    checkGameInfoRect(killsBuf);

    sprintf(lossesBuf, "losses %d", getPlayerLosses());
    checkGameInfoRect(lossesBuf);
    */

    if( gameconfig->gametype == _gametype_timelimit ) {
        snprintf(timeBuf, 64, "time   %s/%s", getPlayerTime(),
                (const char*) getTimeLimit() );
        checkGameInfoRect(timeBuf);
    } else {
        snprintf(timeBuf, 64, "time   %s", getPlayerTime() );
        checkGameInfoRect(timeBuf);
    }

    sprintf(pingBuf, "ping   %.0f ms", NetworkState::ping_time );
    checkGameInfoRect(pingBuf);

    sprintf(fpsBuf, "FPS    %f", TimerInterface::getFPSAvg() );
    checkGameInfoRect(fpsBuf);

    bltViewBackground(viewArea);

    clientArea.bltStringShadowed(pos, gameBuf, Color::white, Color::black);
    pos.y += 12;
    clientArea.bltStringShadowed(pos, unitsBuf, Color::white, Color::black);
    pos.y += 12;
    clientArea.bltStringShadowed(pos, fragsBuf, Color::white, Color::black);
    pos.y += 12;
    clientArea.bltStringShadowed(pos, objectiveBuf, Color::white, Color::black);
    pos.y += 12;
    /*
    clientArea.bltStringShadowed(pos, killsBuf, Color::white, Color::black);
    pos.y += 12;
    clientArea.bltStringShadowed(pos, lossesBuf, Color::white, Color::black);
    pos.y += 12;
    */
    clientArea.bltStringShadowed(pos, timeBuf, Color::white, Color::black);
    pos.y += 12;
    /*clientArea.bltStringShadowed(pos, pingBuf, Color::white, Color::black);
    pos.y += 12;*/
    clientArea.bltStringShadowed(pos, fpsBuf, Color::white, Color::black);
    pos.y += 12;

    View::doDraw(viewArea, clientArea);
} // end GameInfoView::doDraw

// checkGameInfoRect
//---------------------------------------------------------------------------
// Purpose: Makes sure the rect is the size of the text inside.
//---------------------------------------------------------------------------
void GameInfoView::checkGameInfoRect(const std::string& string)
{
    int length = Surface::getTextLength(string) + 2;

    if (length > gameInfoRect.getSizeX()) {
        gameInfoRect.max.x = gameInfoRect.min.x + length;
    }

} // end GameInfoView::checkGameInfoRect


// save position, background into gameconfig
void GameInfoView::notifyMoveTo()
{
    gameconfig->gameinfoposition=min;
}

