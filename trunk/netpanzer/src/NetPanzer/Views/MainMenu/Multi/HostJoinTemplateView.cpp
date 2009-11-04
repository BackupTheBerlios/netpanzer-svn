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


#include <ctype.h>
#include <sstream>
#include "HostJoinTemplateView.hpp"
#include "Views/Components/Desktop.hpp"
#include "HostJoinTemplateView.hpp"
#include "Interfaces/GameConfig.hpp"
#include "Interfaces/PlayerGameManager.hpp"
#include "HostView.hpp"
#include "Util/Exception.hpp"
#include "MapSelectionView.hpp"
#include "PlayerNameView.hpp"
#include "FlagSelectionView.hpp"
#include "2D/Palette.hpp"
#include "Views/GameViewGlobals.hpp"
#include "IPAddressView.hpp"
#include "ServerListView.hpp"

#include "Classes/Network/NetworkClient.hpp"
#include "Classes/Network/NetworkServer.hpp"

#include "Core/GlobalEngineState.hpp"

char HostJoinTemplateView::gameTypeBuf[256];

Surface playerColor;

int cDarkBlue;
int cLightBlue;
int cLightGreen;
int cOrange;

enum
{
    BTN_BACK,
    BTN_NEXT
};

// HostJoinTemplateView
//---------------------------------------------------------------------------
HostJoinTemplateView::HostJoinTemplateView() : MenuTemplateView()
{
    setSearchName("HostJoinTemplateView");
    setTitle("");
    setSubTitle("");

    add( Button::createSpecialButton("back", "Back", backPos, BTN_BACK));
    add( Button::createSpecialButton("next", "Next", nextPos, BTN_NEXT));
} // end HostJoinTemplateView constructor

// doDraw
//---------------------------------------------------------------------------
void HostJoinTemplateView::doDraw()
{
    MenuTemplateView::doDraw();

    View::doDraw();
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
    doLoadBackgroundSurface("pics/default/defaultMB.png");
} // end HostJoinTemplateView::loadBackgroundSurface

void
HostJoinTemplateView::onComponentClicked(Component* c)
{
    switch ( c->getCustomCode() )
    {
        case BTN_BACK:
            if (gameconfig->hostorjoin == _game_session_join)
            {
                NetworkClient::partServer();
            }
            else if (gameconfig->hostorjoin == _game_session_host)
            {
                NetworkServer::closeSession();
            }

            Desktop::setVisibilityAllWindows(false);
            Desktop::setVisibility("GetSessionView", true);
            break;

        case BTN_NEXT:
            if (   (const std::string&) gameconfig->playername == ""
                || strlen(HostJoinTemplateView::gameTypeBuf) == 0
                || MapSelectionView::curMap == -1
                || (gameconfig->hostorjoin == _game_session_join
                        && strcmp(IPAddressView::szServer.getString(), "") == 0
                   )
               )
            {
                return;
            }

            // Set the player flag.
            gameconfig->playerflag = FlagSelectionView::getSelectedFlag();

            // Close all menu views.
            Desktop::setVisibilityAllWindows(false);

            if(gameconfig->hostorjoin == _game_session_join)
            {
                gameconfig->serverConnect = IPAddressView::szServer.getString();
            }

            serverlistview->endQuery();

            MenuTemplateView::freeBackgroundSurface();

            ((PlayerGameManager*)global_engine_state->game_manager)->launchMultiPlayerGame();
            break;

        default:
            MenuTemplateView::onComponentClicked(c);
    }
}
