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
#include "GameViewGlobals.hpp"

iXY exitPos(6, 454);
iXY readyPos(480, 32);
iXY backPos(347, 454);
iXY nextPos(493, 454);
iXY mainPos(16, 7);
iXY multiPos(169, 7);
iXY optionsPos(325, 7);
iXY helpPos(480, 7);
iXY creditsPos(0, 0);
iXY soundPos(16, 32);
iXY tcpipPos = soundPos;
iXY joinPos  = soundPos;
iXY interfacePos(169, 32);
iXY directConnectPos = interfacePos;
iXY hostPos          = interfacePos;
iXY visualsPos(325, 32);
iXY modemPos = visualsPos;
iXY controlsPos(480, 32);
iXY returnToGamePos = mainPos;
iXY resignPos = multiPos;

iRect bodyTextRect(27, 72, 620, 430);

bool gDrawGameHelp          = false;
int  viewDrawBackgroundMode = VIEW_BACKGROUND_DARK_GRAY_BLEND;

//--------------------------------------------------------------------------
void bltViewBackground(const Surface &dest)
{
	bltBlendRect(dest, dest.getRect());
}

//--------------------------------------------------------------------------
void bltBlendRect(const Surface &dest, const iRect &r)
{
	if (viewDrawBackgroundMode == VIEW_BACKGROUND_DARK_GRAY_BLEND)
	{
		dest.bltLookup(r, Palette::darkGray256.getColorArray());
	}
	else if (viewDrawBackgroundMode == VIEW_BACKGROUND_LIGHT_GRAY_BLEND)
	{
		dest.bltLookup(r, Palette::gray256.getColorArray());
	}
	else if (viewDrawBackgroundMode == VIEW_BACKGROUND_SOLID_BLACK)
	{
		dest.fillRect(r, Color::black);
	}
	else if (viewDrawBackgroundMode == VIEW_BACKGROUND_TRANSPARENT)
	{
	}
}
