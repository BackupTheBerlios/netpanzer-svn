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

#include <ctype.h>
#include "HostJoinTemplateView.hpp"
#include "Desktop.hpp"
#include "HostJoinTemplateView.hpp"
#include "GameConfig.hpp"
#include "PlayerGameManager.hpp"
#include "HostView.hpp"
#include "Exception.hpp"
#include "MapSelectionView.hpp"
#include "PlayerNameView.hpp"
#include "FlagSelectionView.hpp"
#include "Palette.hpp"
#include "GameViewGlobals.hpp"
#include "IPAddressView.hpp"
#include "IRCLobbyView.hpp"

#include "Client.hpp"
#include "Server.hpp"

char HostJoinTemplateView::gameTypeBuf[256];

Surface playerColor;

int cDarkBlue;
int cLightBlue;
int cLightGreen;
int cOrange;

/////////////////////////////////////////////////////////////////////////////
// Button functions.
/////////////////////////////////////////////////////////////////////////////

static void bBack()
{
    if (gameconfig->hostorjoin == _game_session_join) {
        CLIENT->closeSession();
    } else
        if (gameconfig->hostorjoin == _game_session_host) {
            SERVER->closeSession();
        }

    Desktop::setVisibilityAllWindows(false);
    Desktop::setVisibility("GetSessionView", true);
}

static void bNext()
{
    if ((const std::string&) gameconfig->playername == "")
        return;

    // Check a few things which should be ok.
    if (strlen(HostJoinTemplateView::gameTypeBuf) == 0) {
        return;
    }
    if (MapSelectionView::curMap == -1) {
        return;
    }
    if (gameconfig->hostorjoin == _game_session_join &&
        strcmp(IPAddressView::szServer.getString(), "") == 0)
        return;

    // Set the player flag.
    gameconfig->playerflag = playerFlagSelected;

    if (gameconfig->hostorjoin == _game_session_join) {
        //winsock hack
        //		if (!IsSelectedGameValid())
        //		{
        //			return;
        //		}

        // Close all menu views.
        Desktop::setVisibilityAllWindows(false);

        Desktop::setVisibility("LobbyView", true);
        lobby_view->stopIRC();

        //this call should be redundant -- enumeration ceases
        //when a session is opened in any case:
        //StopAsyncGameEnumeration( gapp.hwndApp );
        CLIENT->stopEnumeration();
    } else {
        // Close all menu views.
        Desktop::setVisibilityAllWindows(false);
        lobby_view->startIRC();
    }

    // Free the menu pictures.
    //MenuTemplateView::netPanzerLogo.free();
    MenuTemplateView::backgroundSurface.free();
    //MenuTemplateView::titleSurface.free();

    //TODO: I don't like static methods
    PlayerGameManager::launchMultiPlayerGame();
}

// HostJoinTemplateView
//---------------------------------------------------------------------------
HostJoinTemplateView::HostJoinTemplateView() : MenuTemplateView()
{
    setSearchName("HostJoinTemplateView");
    setTitle("");
    setSubTitle("");

    // Back.
    addSpecialButton(	backPos,
                      "Back",
                      bBack);

    addSpecialButton(   nextPos,
                      "Next",
                      bNext);

} // end HostJoinTemplateView constructor

// doDraw
//---------------------------------------------------------------------------
void HostJoinTemplateView::doDraw(Surface &viewArea, Surface &clientArea)
{
    MenuTemplateView::doDraw(viewArea, clientArea);

    View::doDraw(viewArea, clientArea);
} // end doDraw

// drawNameInfo
//--------------------------------------------------------------------------
void HostJoinTemplateView::drawNameInfo(Surface &dest, const iXY &pos)
{
    iXY size(358, 30);
    iRect flagBorder(pos.x, pos.y, pos.x + size.x, pos.y + size.y);
    dest.bltLookup(flagBorder, Palette::darkGray256.getColorArray());
    dest.drawButtonBorder(flagBorder, Color::lightGreen, Color::darkGreen);

    dest.bltStringVGradient(iXY(pos.x + 10, pos.y + 12), "NAME", Palette::gradientWhite2Green);

} // end HostJoinTemplateView::drawNameInfo

// drawPlayerInfo
//--------------------------------------------------------------------------
void HostJoinTemplateView::drawPlayerInfo(Surface &dest, const iXY &pos)
{
    char strBuf[256];

    int x = pos.x;
    sprintf(strBuf, "Player Name");
    dest.bltString(x, pos.y + (playerColor.getPix().y - Surface::getFontHeight()) / 2, strBuf, Color::white);

    //x = 236;
    //playerColor.blt(clientArea, x, pos.y);

    x = 290;
    playerFlag.blt(dest, x, pos.y);

    x = 342;
    sprintf(strBuf, "Status");
    dest.bltString(x, pos.y + (playerColor.getPix().y - Surface::getFontHeight()) / 2, strBuf, Color::white);

} // end drawPlayerInfo

// drawFlagInfo
//--------------------------------------------------------------------------
void HostJoinTemplateView::drawFlagInfo(Surface &dest, const iXY &pos)
{
    // Draw the dimmed background.
    iXY size(180, 115);
    iRect flagBorder(pos.x, pos.y, pos.x + size.x, pos.y + size.y);
    dest.bltLookup(flagBorder, Palette::darkGray256.getColorArray());
    dest.drawButtonBorder(flagBorder, Color::lightGreen, Color::darkGreen);

    dest.bltStringVGradient(pos + iXY(10,10) , "FLAG", Palette::gradientWhite2Green);
    playerFlag.blt(dest, iXY(pos.x + 55, pos.y + 10));

} // end HostJoinTemplateView::drawFlagInfo

// doActivate
//---------------------------------------------------------------------------
void HostJoinTemplateView::doActivate()
{
    MenuTemplateView::doActivate();

    sprintf(currentMultiView, searchName);

} // end doActivate

// loadBackgroundSurface
//---------------------------------------------------------------------------
void HostJoinTemplateView::loadBackgroundSurface()
{
    doLoadBackgroundSurface("pics/backgrounds/menus/menu/defaultMB.bmp");
} // end HostJoinTemplateView::loadBackgroundSurface
