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
#include "GameInfoView.hpp"
#include "GameViewGlobals.hpp"
#include "Desktop.hpp"
#include "GameConfig.hpp"
#include "GameManager.hpp"
#include "UnitInterface.hpp"
#include "PlayerInterface.hpp"
#include "NetworkState.hpp"
#include "String.hpp"


static int getPlayerKills()
{
 PlayerState *player_state;

 player_state = PlayerInterface::getLocalPlayerState();
 return( (int) player_state->getKills() );
}

static int getPlayerLosses()
{
 PlayerState *player_state;

 player_state = PlayerInterface::getLocalPlayerState();
 return( (int) player_state->getLosses() );
}

static int getPlayerFrags()
{
 PlayerState *player_state;

 player_state = PlayerInterface::getLocalPlayerState();
 return( (int) player_state->getTotal() );
}

static String getPlayerTime()
{
 char time_string[256];

 int hrs = (GameManager::getGameTime() / 3600);
 int min = (GameManager::getGameTime() / 60) % 60;
 
 sprintf(time_string, "%d:%d", hrs, min);
                                 
 return(time_string);
}

static String getTimeLimit()
{
 char time_string[256];

 int hrs = (GameConfig::GetTimeLimit() / 60);
 int min = (GameConfig::GetTimeLimit() % 60);
 
 sprintf(time_string, "%d:%d", hrs, min);
                                 
 return(time_string);
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

	moveTo(iXY(0, 0));
	//resizeClientArea(iXY(100, 2 * 2 + 12 * 3 + 8));
	resizeClientArea(iXY(140, 2 * 2 + 12 * 4 + 8));

	// Start it in the top-left corner.
	moveTo(iXY(10000, 0));

} // end GameInfoView::GameInfoView

// doDraw
//---------------------------------------------------------------------------
void GameInfoView::doDraw(const Surface &viewArea, const Surface &clientArea)
{
	char unitsBuf[64];
	char killsBuf[64];
	char lossesBuf[64];
	char fragsBuf[64];
    char timeBuf[64];
    char pingBuf[64];
    char gameBuf[64];
    	
	iXY pos(2, 2);

	// Resize the game info rect to nothing so it can be resized based on the text,
	// to be used in it.
	gameInfoRect = getClientRect();
	//gameInfoRect.max.x = 0;

    sprintf(gameBuf, "game   %s", GameConfig::getGameTypeString() );
    checkGameInfoRect(gameBuf);
   

 	int unitPerPlayer = GameConfig::GetNumberUnits() / GameConfig::GetNumberPlayers();
	sprintf(unitsBuf, "units  %d/%d", int(UnitInterface::getUnitCount(PlayerInterface::getLocalPlayerIndex())), unitPerPlayer);
	checkGameInfoRect(unitsBuf);

    if( GameConfig::GetGameType() == _gametype_fraglimit )
     {
      sprintf(fragsBuf, "frags  %d/%d", getPlayerFrags(), GameConfig::GetFragLimit() );
	  checkGameInfoRect(fragsBuf);
     }
    else
     {
      sprintf(fragsBuf, "frags  NA" );
	  checkGameInfoRect(fragsBuf);
     }

    /*
    sprintf(killsBuf, "kills  %d", getPlayerKills());
	checkGameInfoRect(killsBuf);

	sprintf(lossesBuf, "losses %d", getPlayerLosses());
	checkGameInfoRect(lossesBuf);
    */

   if( GameConfig::GetGameType() == _gametype_timelimit )
    {
	 sprintf(timeBuf, "time   %s/%s", (const char *) getPlayerTime(), getTimeLimit() );
	 checkGameInfoRect(timeBuf);
    }
   else
    {
	 sprintf(timeBuf, "time   %s", (const char *) getPlayerTime() );
	 checkGameInfoRect(timeBuf);
    }

	sprintf(pingBuf, "ping   %.0f ms", NetworkState::ping_time );
	checkGameInfoRect(pingBuf);

	bltViewBackground(viewArea);

	clientArea.bltStringShadowed(pos, gameBuf, Color::white, Color::black);
	pos.y += 12;
	clientArea.bltStringShadowed(pos, unitsBuf, Color::white, Color::black);
	pos.y += 12;
	clientArea.bltStringShadowed(pos, fragsBuf, Color::white, Color::black);
	pos.y += 12;
	/*
    clientArea.bltStringShadowed(pos, killsBuf, Color::white, Color::black);
	pos.y += 12;
	clientArea.bltStringShadowed(pos, lossesBuf, Color::white, Color::black);
	pos.y += 12;
	*/
    clientArea.bltStringShadowed(pos, timeBuf, Color::white, Color::black);
	pos.y += 12;
	clientArea.bltStringShadowed(pos, pingBuf, Color::white, Color::black);
	pos.y += 12;

	View::doDraw(viewArea, clientArea);

	// Make sure the view stays in the screen when the view is resized.
	//resizeClientArea(gameInfoRect.getSize());
	//moveTo(min);

} // end GameInfoView::doDraw

// checkGameInfoRect
//---------------------------------------------------------------------------
// Purpose: Makes sure the rect is the size of the text inside.
//---------------------------------------------------------------------------
void GameInfoView::checkGameInfoRect(String string)
{
	int length = (strlen(string) * CHAR_XPIX + 2);

	if (length > gameInfoRect.getSizeX())
	{
		gameInfoRect.max.x = gameInfoRect.min.x + length;
	}

} // end GameInfoView::checkGameInfoRect
