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
#include <sstream>
#include "HostJoinTemplateView.hpp"
#include "Desktop.hpp"
#include "HostJoinTemplateView.hpp"
#include "GameConfig.hpp"
#include "PlayerGameManager.hpp"
#include "HostView.hpp"
#include "Util/Exception.hpp"
#include "MapSelectionView.hpp"
#include "PlayerNameView.hpp"
#include "FlagSelectionView.hpp"
#include "2D/Palette.hpp"
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
#if 0
        std::stringstream join_mess;
        join_mess << "join " << IPAddressView::szServer.getString();
        lobby_view->sendIRCMessageLine(join_mess.str());
#endif
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
