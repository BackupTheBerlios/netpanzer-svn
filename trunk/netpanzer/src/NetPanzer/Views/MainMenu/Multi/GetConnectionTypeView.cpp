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


#include "stdafx.hpp"
#include "GetConnectionTypeView.hpp"
#include "Desktop.hpp"
#include "GameConfig.hpp"
#include "TimerInterface.hpp"
#include "GameViewGlobals.hpp"


/////////////////////////////////////////////////////////////////////////////
// Button functions.
/////////////////////////////////////////////////////////////////////////////

int previousConnectionType = _connection_tcpip;

static void bTCP()
{
	previousConnectionType = GameConfig::GetNetworkConnectType();

	GameConfig::SetNetworkConnectType(_connection_tcpip);
}

static void bDirect()
{
	previousConnectionType = GameConfig::GetNetworkConnectType();

	GameConfig::SetNetworkConnectType(_connection_direct_connect);
}

static void bModem()
{
	previousConnectionType = GameConfig::GetNetworkConnectType();

	GameConfig::SetNetworkConnectType(_connection_modem);
}

static void bNext()
{
	if (GameConfig::GetNetworkConnectType() == _connection_tcpip)
	{
		Desktop::setVisibilityAllWindows(false);
		Desktop::setVisibility("GetSessionView", true);
	}
}


// GetConnectionTypeView
//---------------------------------------------------------------------------
GetConnectionTypeView::GetConnectionTypeView() : MenuTemplateView()
{
	setSearchName("GetConnectionTypeView");
	setTitle("Select Connection Type");
	setSubTitle("");

	// TCP/IP.
	addSpecialButton(	tcpipPos,
						"TCP/IP",
						"TCPIP",
						bTCP);

	// Direct.
	addSpecialButton(	directConnectPos,
						"Direct Connect",
						bDirect);

	// Modem.
	addSpecialButton(	modemPos,
						"Modem",
						bModem);

	// Next.
	addSpecialButton(	nextPos,
						"Next",
						bNext);


    //Surface tempSurface;
	//
	//tempSurface.loadAllTILInDirectory("pics\\backgrounds\\menus\\multi\\getConnection\\buttons\\tcpip\\til\\");
    //assert(tempSurface.getFrameCount() == 3);
    //addButtonSurface(iXY(173, 99), tempSurface, "Selects a TCP/IP connection type.", bTCP);
	//
    //tempSurface.loadAllTILInDirectory("pics\\backgrounds\\menus\\multi\\getConnection\\buttons\\directConnect\\til\\");
    //assert(tempSurface.getFrameCount() == 3);
    //addButtonSurface(iXY(134, 119), tempSurface, "Selects a DirectConnect connection type.", bDirect);
	//
    //tempSurface.loadAllTILInDirectory("pics\\backgrounds\\menus\\multi\\getConnection\\buttons\\modem\\til\\");
    //assert(tempSurface.getFrameCount() == 3);
    //addButtonSurface(iXY(104, 138), tempSurface, "Selects a modem connection type.", bModem);

	//addButtonCenterText(iXY(502, 438), 100, "Next", "Accepts the current selection.", bNext);

} // end GetConnectionTypeView::GetConnectionTypeView

// doDraw
//---------------------------------------------------------------------------
void GetConnectionTypeView::doDraw(const Surface &viewArea, const Surface &clientArea)
{
	if (GameConfig::GetNetworkConnectType() != previousConnectionType)
	{
		loadTitleSurface();
	}

	MenuTemplateView::doDraw(viewArea, clientArea);

	drawInfo(clientArea);

} // end GetConnectionTypeView::doDraw

//---------------------------------------------------------------------------
void GetConnectionTypeView::drawInfo(const Surface &dest)
{
	if (GameConfig::GetNetworkConnectType() == _connection_tcpip)
	{
		drawTCPIPInfo(dest, bodyTextRect);
	}
	else if (GameConfig::GetNetworkConnectType() == _connection_direct_connect)
	{
		drawDirectConnectInfo(dest, bodyTextRect);
	}
	else if (GameConfig::GetNetworkConnectType() == _connection_modem)
	{
		drawModemInfo(dest, bodyTextRect);
	}
}

//---------------------------------------------------------------------------
void GetConnectionTypeView::drawTCPIPInfo(const Surface &dest, const iRect &rect)
{
	static char	tcpipInfo[] =   "TCP/IP GAMES\n"
                                "\n"
								"netPanzerTest supports TCP/IP multiplayer modes over "
								"LAN systems and over the Internet.  You must have TCP/IP "
								"configured in Windows95 or Windows98 to play a multiplayer "
								"game.  See your Windows95 or Windows98 help files for "
								"instructions on configuring TCP/IP on your computer.\n"
								"\n"
                                "Click the Next button to proceed";


	dest.bltStringInBox(rect, tcpipInfo, Color::white, 12);
}

//---------------------------------------------------------------------------
void GetConnectionTypeView::drawDirectConnectInfo(const Surface &dest, const iRect &rect)
{
	static char	directConnectInfo[] =
                                "DIRECT CONNECT GAMES\n"
                                "\n"
								"netPanzerTest does not support direct connect."
								"This type of connection will be supported in the"
                                "downloadable public netPanzer beta.";

	dest.bltStringInBox(rect, directConnectInfo, Color::white, 12);
}

//---------------------------------------------------------------------------
void GetConnectionTypeView::drawModemInfo(const Surface &dest, const iRect &rect)
{
	static char	modemInfo[] =	"MODEM GAMES\n"
                                "\n"
                                "netPanzerTest does not support modem connections. "
								"This type of connection will be supported in the "
								"downloadable public netPanzer beta.";

	dest.bltStringInBox(rect, modemInfo, Color::white, 12);
}

// doActivate
//---------------------------------------------------------------------------
void GetConnectionTypeView::doActivate()
{
	MenuTemplateView::doActivate();
	sprintf(currentMultiView, searchName);

} // end doActivate

// loadBackgroundSurface
//---------------------------------------------------------------------------
void GetConnectionTypeView::loadBackgroundSurface()
{
	doLoadBackgroundSurface(("pics\\backgrounds\\menus\\menu\\til\\connectionMB.til"));

} // end MenuTemplateView::loadBackgroundSurface

// loadTitleSurface
//---------------------------------------------------------------------------
void GetConnectionTypeView::loadTitleSurface()
{
	if (GameConfig::GetNetworkConnectType() == _connection_tcpip)
	{
		doLoadTitleSurface("pics\\backgrounds\\menus\\menu\\til\\tcpipTitle.til");
	}
	else if (GameConfig::GetNetworkConnectType() == _connection_direct_connect)
	{
		doLoadTitleSurface("pics\\backgrounds\\menus\\menu\\til\\directConnectionTitle.til");
	}
	else if (GameConfig::GetNetworkConnectType() == _connection_modem)
	{
		doLoadTitleSurface("pics\\backgrounds\\menus\\menu\\til\\modemTitle.til");
	}

} // end GetConnectionTypeView::loadTitleSurface
