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

#include "GetSessionView.hpp"
#include "GetSessionHostView.hpp"
#include "Desktop.hpp"
#include "GameConfig.hpp"
#include "GameManager.hpp"
#include "MouseInterface.hpp"
#include "GameViewGlobals.hpp"

//winsock hack
#include "IPAddressView.hpp"

#include "Server.hpp"
#include "Client.hpp"

static int previousSessionType = _game_session_join;


/////////////////////////////////////////////////////////////////////////////
// Button functions.
/////////////////////////////////////////////////////////////////////////////

static void bBack()
{
    Desktop::setVisibilityAllWindows(false);
    Desktop::setVisibility("MainView", true);
}

void bNext()
{
    //Bug#15
    //bool DPlayReturnValue;
    //bool ConnectTypeReturnValue;
    //HRESULT hr;

    //int return_value;

    //winsock hack
    //Desktop::setVisibilityAllWindows(false);

    if (gameconfig->hostorjoin == _game_session_host) {
        //winsock hack
        Desktop::setVisibilityAllWindows(false);

        Desktop::setVisibility("HostView", true);
        Desktop::setVisibility("UnitSelectionView", true);
        Desktop::setVisibility("FlagSelectionView", true);
        Desktop::setVisibility("HostOptionsView", true);
        Desktop::setVisibility("MapSelectionView", true);
        Desktop::setVisibility("PlayerNameView", true);

        SERVER->openSession();
    } else if (gameconfig->hostorjoin == _game_session_join) {
        CLIENT->openSession();

        // XXX no server enumertation/info yet
#if 0
        CLIENT->startEnumeration();
        /*
          if ( CLIENT->startEnumeration( ) == false )
           {
            Desktop::setVisibilityAllWindows(false);
         Desktop::setVisibility("GetSessionView", true);
           }
          else
           {*/
#endif
        Desktop::setVisibility("JoinView", true);
        //Desktop::setVisibility("GetSessionHostView", true);
        //Desktop::setVisibility("UnitSelectionView", true);
        Desktop::setVisibility("FlagSelectionView", true);
        Desktop::setVisibility("PlayerNameView", true);
        //}

        /* winsock hack
        bool minimize;

        //initialize dplay, fix Bug#15--
        //if the DPlayReturnValue is 0 dplay ain't working.
        //we need to put up a message box that says so, then pop the
        //user back to the SetConnectionTypeView--
        //ShutDownConnection();
        DPlayReturnValue = InitializeDirectPlay( gapp.hwndApp);
        ConnectTypeReturnValue = GameManager::initializeConnectionType();

        DDraw.setGDIStatus(true);
        //fix dialup problem
        minimize = MinimizeOrNot(gapp.hwndApp);

         //bug#15+
         hr =	EnumerateGames( gapp.hwndApp );

        if (minimize == false)
        {
        DDraw.setGDIStatus(false);
        }

        MouseInterface::hideHardwareCursor();   

         // had to rearrange the code so that it would 
         // react correctly to the dialog box return 
         if(hr == DPERR_USERCANCEL)
          {
           //WE NEED TO BACK OUT TO SELECTSESSIONTYPEVIEW if this
           //happens
        //Desktop::toggleVisibility("DirectPlayErrorUserCancelView");
           Desktop::setVisibilityAllWindows(false);
        Desktop::setVisibility("GetSessionView", true);
          }
         else
          if((hr == DPERR_EXCEPTION) || (hr == DPERR_GENERIC))
           {
            //WE NEED TO TELL THEM THAT A WEIRD DPLAY ERROR OCCURRED
            //AND BUMP THEM OUT TO SELECTSESSIONTYPEVIEW
        Desktop::toggleVisibility("DirectPlayErrorExceptionView");
        Desktop::setVisibility("GetSessionView", true);
           }
          else
           { 
           
             end winsock hack*/


        Desktop::setVisibility("IPAddressView", true);
        //Desktop::setVisibility("JoinView", true);
        //Desktop::setVisibility("GetSessionHostView", true);
        //Desktop::setVisibility("UnitSelectionView", true);
        //Desktop::setVisibility("FlagSelectionView", true);
        //Desktop::setVisibility("PlayerNameView", true);


        //}

    }
}

static void bHost()
{
    previousSessionType = gameconfig->hostorjoin;

    gameconfig->hostorjoin = _game_session_host;
}

static void bJoin()
{
    previousSessionType = gameconfig->hostorjoin;

    gameconfig->hostorjoin = _game_session_join;
}


// GetSessionView
//---------------------------------------------------------------------------
GetSessionView::GetSessionView() : MenuTemplateView()
{
    setSearchName("GetSessionView");
    setTitle("Select Session");
    setSubTitle("");

    // Join.
    addSpecialButton(	joinPos,
                      "Join",
                      bJoin);

    // Host.
    addSpecialButton(	hostPos,
                      "Host",
                      bHost);

    // Next.
    addSpecialButton(	nextPos,
                      "Next",
                      bNext);

    // Back.
    addSpecialButton(	backPos,
                      "Back",
                      bBack);

    //    Surface tempSurface;
    //
    //	tempSurface.loadAllTILInDirectory("pics/backgrounds/menus/multi/getSession/buttons/join/til/");
    //    assert(tempSurface.getFrameCount() == 3);
    //    addButtonSurface(iXY(137, 118), tempSurface, "Join a multiplayer session.", bJoin);
    //
    //	tempSurface.loadAllTILInDirectory("pics/backgrounds/menus/multi/getSession/buttons/host/til/");
    //    assert(tempSurface.getFrameCount() == 3);
    //    addButtonSurface(iXY(106, 138), tempSurface, "Host a multiplayer session.", bHost);
    //
    //	addButtonCenterText(iXY(360, 438), 100, "Next", "Accepts the current selection.", bNext);
    //
    //	tempSurface.loadAllTILInDirectory("pics/backgrounds/menus/buttons/back/til/");
    //    assert(tempSurface.getFrameCount() == 3);
    //    addButtonSurface(iXY(505, 440), tempSurface, "Move back menu.", bBack);
    //
    //	gameconfig->SetHostOrJoin(_game_session_host);

} // end GetSessionView::GetSessionView

// doDraw
//---------------------------------------------------------------------------
void GetSessionView::doDraw(Surface &viewArea, Surface &clientArea)
{
    if (previousSessionType != gameconfig->hostorjoin) {
        loadTitleSurface();
    }

    MenuTemplateView::doDraw(viewArea, clientArea);

    drawInfo(clientArea);

} // end GetSessionView::doDraw

//---------------------------------------------------------------------------
void GetSessionView::drawInfo(Surface &dest)
{
    int connectionType = gameconfig->hostorjoin;

    if (connectionType == _game_session_host) {
        drawHostInfo(dest, bodyTextRect);
    } else if (connectionType == _game_session_join) {
        drawJoinInfo(dest, bodyTextRect);
    }
}

//---------------------------------------------------------------------------
void GetSessionView::drawHostInfo(Surface &dest, const iRect &rect)
{
    static char	tcpipInfo[] =	"HOSTING LAN GAMES\n"
                              "\n"
                              "Over LAN systems up to 25 players can be supported. For games "
                              "that large you should use the dedicated server option. Start "
                              "netPanzerTest from a dosbox using the -dedicated command line "
                              "argument.\n"
                              "\n"
                              "HOSTING INTERNET GAMES\n"
                              "\n"
                              "Over the Internet varying numbers of players can be "
                              "supported depending on connection speed and quality. "
                              "Use the -dedicated command line argument to host large "
                              "scale games.\n"
                              "\n"
                              "Maximum Internet Player Recommendations:\n"
                              "\n"
                              "  28.8 modem:      4\n"
                              "  33.6 modem:      6\n"
                              "  56.6 modem:      8\n"
                              "  ISDN or faster: 25\n"
                              "\n"
                              "If you need to know what your machine's IP address is "
                              "so that others can join your game you can go to the "
                              "windows Start menu, click on 'Programs' and select the"
                              "'PyroSoft' entry. Then select Winipcfg. The winipcfg "
                              "dialog box appears. Get the IP address for the adapter "
                              "you are using, either LAN or PPP. NOTE: most Internet "
                              "Service Providers give you a different IP address "
                              "each time you connect to the Internet. Therefore, if "
                              "for some reason your Internet connection is dropped by "
                              "your ISP, you will have to check your IP address after "
                              "you reconnect.\n"
                              "\n"
                              "Click the Next button to proceed";


    dest.bltStringInBox(rect, tcpipInfo, Color::white, 12);
}

//---------------------------------------------------------------------------
void GetSessionView::drawJoinInfo(Surface &dest, const iRect &rect)
{
    static char	tcpipInfo[] =	"JOINING LAN OR INTERNET GAMES\n"
                              "\n"
                              "netPanzerTest will prompt you for an IP address when you "
                              "try to join a game. You must specify the IP address or the "
                              "name of the host computer.\n"
                              "\n"
                              "Click the Next button to proceed.";


    dest.bltStringInBox(rect, tcpipInfo, Color::white, 12);
}

// doActivate
//---------------------------------------------------------------------------
void GetSessionView::doActivate()
{
    MenuTemplateView::doActivate();

    sprintf(currentMultiView, searchName);

} // end GetSessionView::doActivate

// loadBackgroundSurface
//---------------------------------------------------------------------------
void GetSessionView::loadBackgroundSurface()
{
    doLoadBackgroundSurface(("pics/backgrounds/menus/menu/til/sessionMB.til"));

} // end GetSessionView::loadBackgroundSurface

// loadTitleSurface
//---------------------------------------------------------------------------
void GetSessionView::loadTitleSurface()
{
    if (gameconfig->hostorjoin == _game_session_host) {
        doLoadTitleSurface("pics/backgrounds/menus/menu/til/hostTitle.til");
    } else if (gameconfig->hostorjoin == _game_session_join) {
        doLoadTitleSurface("pics/backgrounds/menus/menu/til/joinTitle.til");
    }

} // end GetSessionView::loadTitleSurface
