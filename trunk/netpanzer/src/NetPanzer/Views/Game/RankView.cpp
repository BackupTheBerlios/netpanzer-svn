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

#include "Util/Exception.hpp"
#include "RankView.hpp"
#include "Stats.hpp"
#include "GameViewGlobals.hpp"
#include "ScreenSurface.hpp"
#include "GameConfig.hpp"

#include "SelectionBoxSprite.hpp"

// RankView
//---------------------------------------------------------------------------
RankView::RankView() : GameTemplateView()
{
    setSearchName("RankView");
    setTitle("Rankings");
    setSubTitle(" - F6");

    setAllowResize(false);
    moveTo(gameconfig->rankposition);
    resize(iXY(450, 200));
    checkArea(screen->getPix());


    const unsigned MAX_NAME_CHARS      = 20;
    const unsigned MAX_FLAG_CHARS      =  5;
    const unsigned MAX_KILLS_CHARS     =  6;
    const unsigned MAX_LOSSES_CHARS    =  7;
    const unsigned MAX_POINTS_CHARS    =  7;

    // hardcoded for now
    int CHAR_XPIX = 8;

    //addLabel(iXY nPos, char *nLabel, uint8_t color);
    unsigned xOffset = 0;
    unsigned yOffset = 16;
    addLabel(iXY(xOffset, yOffset), "Name", Color::red);
    xOffset += MAX_NAME_CHARS*CHAR_XPIX;
    addLabel(iXY(xOffset, yOffset), "Flag", Color::red);
    xOffset += MAX_FLAG_CHARS*CHAR_XPIX;
    addLabel(iXY(xOffset, yOffset), "Kills", Color::red);
    xOffset += MAX_KILLS_CHARS*CHAR_XPIX;
    addLabel(iXY(xOffset, yOffset), "Losses", Color::red);
    xOffset += MAX_LOSSES_CHARS*CHAR_XPIX;
    addLabel(iXY(xOffset, yOffset), "Points", Color::red);
    xOffset += MAX_POINTS_CHARS*CHAR_XPIX;
    addLabel(iXY(xOffset, yOffset), "Objective", Color::red);
    xOffset += MAX_POINTS_CHARS*CHAR_XPIX;

    // Define the scrollBar fot this view.
    scrollBar = new ScrollBar(ScrollBar::VERTICAL, 0, 1, 0, 100);
    if (scrollBar == 0) {
        throw Exception("ERROR: Unable to allocate the scrollBar.");
    }

    //iRect clientRect(getClientRect());
    //scrollBar->setViewableAmount(clientRect.getSizeY() / (TEXT_GAP_SPACE + CHAR_YPIX) - 1);
    //scrollBar->setBlockIncrement(1);

    //add(scrollBar);

} // end RankView::RankView

// doDraw
//---------------------------------------------------------------------------
void RankView::doDraw(Surface &viewArea, Surface &clientArea)
{
    bltViewBackground(viewArea);

    clientArea.drawButtonBorder(iRect(0, 26, getClientRect().getSize().x - 1, getClientRect().getSize().y - 1), Color::gray64, Color::white);

    drawPlayerStats(clientArea);

    View::doDraw(viewArea, clientArea);
} // end doDraw

// drawPlayerStats
//---------------------------------------------------------------------------
// Purpose:
//---------------------------------------------------------------------------
void RankView::drawPlayerStats(Surface &dest)
{

    char statBuf[256];
    iXY offset;
    iXY flagOffset;
    //int index;

    Stats::setSortOrder( _stats_sort_order_game_type );
    Stats::Initialize();

    char  playerFlagIndex;
    short playerKills;
    short playerLosses;
    short playerTotalPoints;
    short playerObjectives;
    const char  *playerName;
    int   playerStatsDisplayType;
    PlayerID playerID;

    //int offsetRunner = getScrollBarTopItem() - numPlayers;

    // Scoot through the list to the first visible player, since we can't seek to an index yet.
    //while(offsetRunner > 0)
    //{
    //	offsetRunner--;
    //	Stats::GetPlayerStats(playerFlagName, &playerKills, &playerLosses, &playerTotalPoints, &playerName);
    //}
    //
    //setScrollBarViewableCount(viewableMessageCount);
    //setScrollBarItemCount(numPlayers);
    //
    //int numToDraw = (viewableMessageCount < numPlayers) ? viewableMessageCount : numPlayers;
    //
    //if(numPlayers < viewableMessageCount)
    //{
    //	setScrollBarTopItem(numPlayers);
    //} else
    //{
    //	setScrollBarTopItem(numPlayers - viewableMessageCount);
    //}

    int activePlayer = -1;
    for( int index = 0; index < PlayerInterface::getMaxPlayers(); index++ ) {
        playerID = PlayerInterface::getPlayerID( index );
        if( ( PlayerInterface::getPlayerState( playerID )->getStatus() ) != (unsigned char) _player_state_active ) {
		continue;
	}
	//Another active player
	activePlayer++;

	//Calculate screen offset
	int CHAR_YPIX = Surface::getFontHeight();
        offset.x = 2;
        flagOffset.x = offset.x + 160;
        offset.y = 40 + activePlayer * (CHAR_YPIX + (UNIT_FLAGS_SURFACE.getPixY() - CHAR_YPIX) );
        flagOffset.y = (40 - (UNIT_FLAGS_SURFACE.getPixY() - CHAR_YPIX)/2 ) + activePlayer * UNIT_FLAGS_SURFACE.getPixY();

        //index = getScrollBarTopItem() + i;
        //assert(index < numPlayers);

        Stats::GetPlayerStats(&playerFlagIndex,
                              &playerKills,
                              &playerLosses,
                              &playerTotalPoints,
                              &playerObjectives,
                              &playerName,
                              &playerStatsDisplayType,
			      playerID );

        // Add player flag name.
        sprintf(statBuf, "%-20s%10i%7i%6i%10i", playerName, playerKills, playerLosses, playerTotalPoints, playerObjectives );
        if (statBuf != 0) {
            PIX color;

            switch( playerStatsDisplayType ) {
            case _stats_display_type_local_player :
                color = Color::blue;
                break;

            case _stats_display_type_ally :
                color = Color::orange;
                break;

            case _stats_display_type_default :
//		printf( "Index: %d, PlayerID: %d!\n", i, playerID.getIndex() );
                color = PlayerInterface::getPlayerState( playerID )->getColor();
                break;


            } // ** switch

            dest.bltString( offset.x, offset.y, statBuf, color );

        }
        UNIT_FLAGS_SURFACE.setFrame( playerFlagIndex );
        UNIT_FLAGS_SURFACE.blt( dest, flagOffset.x, flagOffset.y );
    }

} // end RankView::drawPlayerStats

void RankView::notifyMoveTo()
{
    gameconfig->rankposition=min;
}
