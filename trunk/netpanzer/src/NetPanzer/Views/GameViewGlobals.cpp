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

#include "2D/Palette.hpp"
#include "GameViewGlobals.hpp"
#include "GameConfig.hpp"

iXY exitPos(6, 454);
iXY readyPos(480, 32);
iXY backPos(347, 454);
iXY nextPos(493, 454);
iXY mainPos(16, 7);
iXY multiPos(169, 7);
iXY optionsPos(325, 7);
iXY helpPos(480, 7);
iXY creditsPos(0, 0);

const iXY sub1(16, 32);
const iXY sub2(169, 32);
const iXY sub3(325, 32);
const iXY sub4(480, 32);

#if 0 // XXX
iXY soundPos = sub1;
iXY interfacePos = sub2;
iXY visualsPos = sub3;
iXY controlsPos = sub4;
#else
iXY visualsPos = sub1;
iXY interfacePos = sub2;
iXY soundPos = sub3; // XXX 2
#endif

iXY hostPos = sub1;
iXY joinPos = sub2;

iXY returnToGamePos = mainPos;
iXY resignPos = multiPos;

iRect bodyTextRect(27, 72, 620, 430);

bool gDrawGameHelp          = false;

//--------------------------------------------------------------------------
void bltViewBackground(Surface &dest)
{
    bltBlendRect(dest, dest.getRect());
}

//--------------------------------------------------------------------------
void bltBlendRect(Surface &dest, const iRect &r)
{
    if (gameconfig->viewdrawbackgroundmode == VIEW_BACKGROUND_DARK_GRAY_BLEND) {
        dest.bltLookup(r, Palette::darkGray256.getColorArray());
    } else if (gameconfig->viewdrawbackgroundmode == VIEW_BACKGROUND_LIGHT_GRAY_BLEND) {
        dest.bltLookup(r, Palette::gray256.getColorArray());
    } else if (gameconfig->viewdrawbackgroundmode == VIEW_BACKGROUND_SOLID_BLACK) {
        dest.fillRect(r, Color::black);
    } else if (gameconfig->viewdrawbackgroundmode == VIEW_BACKGROUND_TRANSPARENT) {}
}
