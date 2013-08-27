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


#include "2D/Palette.hpp"
#include "Views/GameViewGlobals.hpp"
#include "Interfaces/GameConfig.hpp"

iRect MenuRect(80, 26, 640, 480-26-26);
iRect bodyTextRect(MenuRect.getLocationX()+10, MenuRect.getLocationY()+10,  MenuRect.getWidth()-10,  MenuRect.getHeight()-80);
iRect newsRect(bodyTextRect.getLocationX(),    bodyTextRect.getEndY() + 10, bodyTextRect.getWidth(), MenuRect.getHeight()-90);

iXY mainPos(MenuRect.getLocationX()+8, MenuRect.getLocationY()-26);
iXY joinPos(mainPos.x+54, mainPos.y);
iXY hostPos(joinPos.x+54, mainPos.y);
iXY optionsPos(hostPos.x+54, mainPos.y);
iXY helpPos(optionsPos.x+78, mainPos.y);

iXY exitPos(MenuRect.getEndX() - 140, mainPos.y);
iXY playPos(MenuRect.getEndX()-58, MenuRect.getEndY());

iXY returnToGamePos(mainPos);
iXY resignPos(optionsPos);

iXY creditsPos(0, 0);
iXY readyPos(480, 32);
iXY backPos(480, 575);

const iXY sub1(mainPos.x, mainPos.y+ButonHeight);
const iXY sub2(joinPos.x, sub1.y);
const iXY sub3(hostPos.x, sub1.y);
const iXY sub4(optionsPos.x, sub1.y);

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

bool gDrawGameHelp          = false;

//--------------------------------------------------------------------------
void bltViewBackground(Surface &dest)
{
    dest.BltRoundRect(dest.getRect(), 10, Palette::filterDarkGray());
}
