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
#ifndef __GameViewGlobals_hpp__
#define __GameViewGlobals_hpp__

#include "Surface.hpp"
#include "iRect.hpp"
#include "iXY.hpp"

// Globals meter colors.
#define windowTextColor             (Color::white)
#define windowTextColorShadow       (Color::black)
#define meterColor                  (Color::terreVerte)
#define meterTextColor              (Color::black)
#define meterTopLeftBorderColor     (Color::white)
#define meterBottomRightBorderColor (Color::black)
#define viewHeadingColor            (Color::tan)
#define meterWidth                  (280)

#define optionsMeterWidth           (148)
#define optionsMeterStartX          (448)
#define optionsColorMeterStartX     (342)

// Button placement locations.
extern iXY exitPos;
extern iXY readyPos;
extern iXY backPos;
extern iXY nextPos;
extern iXY mainPos;
extern iXY multiPos;
extern iXY optionsPos;
extern iXY helpPos;
extern iXY creditsPos;
extern iXY soundPos;
extern iXY tcpipPos;
extern iXY joinPos;
extern iXY interfacePos;
extern iXY directConnectPos;
extern iXY hostPos;
extern iXY visualsPos;
extern iXY modemPos;
extern iXY controlsPos;
extern iXY returnToGamePos;
extern iXY resignPos;

// This is the text area in the menus.
extern iRect bodyTextRect;

// These are the possible background colors for the menus.
extern int viewDrawBackgroundMode;
enum
{
    VIEW_BACKGROUND_DARK_GRAY_BLEND,
    VIEW_BACKGROUND_LIGHT_GRAY_BLEND,
    VIEW_BACKGROUND_SOLID_BLACK,
    VIEW_BACKGROUND_TRANSPARENT,
    VIEW_BACKGROUND_COUNT,
};

void bltViewBackground(const Surface &dest);
void bltBlendRect(const Surface &dest, const iRect &r);

// Is the game help currently open?
extern bool gDrawGameHelp;

#endif // __GameViewGlobals_hpp__
