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
#ifndef __MiniRankView_hpp__
#define __MiniRankView_hpp__

#include "iXY.hpp"
#include "iRect.hpp"
#include "Surface.hpp"
#include "Stats.hpp"


void drawMiniRankView(const Surface &dest)
{
	iXY size(200, 60);

	iRect r(iXY(SCREEN_XPIX - size.x, SCREEN_YPIX - size.y), SCREEN_PIX);

	// Just shove it in the bottom corner for now.
	dest.bltLookup(r, Palette::darkGray256.getColorArray());

	Stats::Initialize();
	Stats::SortPlayers();

	char  strBuf[256];
	char  playerFlagName[256];
	short playerKills;
	short playerLosses;
	short playerTotalPoints;
	char  *playerName;

	int playerCount = Stats::getActivePlayers();

	iXY pos(5, 5);

	int yOffset = 10;

	for (int i = 0; i < MIN(playerCount, 4); i++)
	{
		Stats::GetPlayerStats(playerFlagName, &playerKills, &playerLosses, &playerTotalPoints, 0, &playerName, 0);

		sprintf(strBuf, "%-10s%10i", playerName, playerTotalPoints);
		
		dest.bltString(r.min.x + pos.x, r.min.y + pos.y, strBuf, Color::white);
		pos.y += yOffset;
	}
}

#endif // end __MiniRankView_hpp__
