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
#include "MainMenuView.hpp"
#include "Desktop.hpp"
#include "DDHardSurface.hpp"
#include "FontSystem2D.hpp"
#include "GameManager.hpp"
#include "GameViewGlobals.hpp"

#include "HostJoinTemplateView.hpp"
#include "GetSessionView.hpp"
#include "GameConfig.hpp"

// MainMenuView
//---------------------------------------------------------------------------
MainMenuView::MainMenuView() : MenuTemplateView()
{
	setSearchName("MainView");
	setTitle("Main");
	setSubTitle("");

	setVisible(true);

	//addButtonCenterText(iXY(240, 454), 260, "QuickLaunch of Host Session", "Accepts the current selection.", bQuickLaunch);

} // end MainMenuView::MainMenuView

// doDraw
//---------------------------------------------------------------------------
void MainMenuView::doDraw(const Surface &viewArea, const Surface &clientArea)
{
	MenuTemplateView::doDraw(viewArea, clientArea);

	static char	text[] =	"This is a special limited multiplayer TEST version of netPanzer.  "
							"netPanzer's legal use must conform to the software license "
							"which was explicitly agreed to when netPanzer was installed "
							"on this system.  That license agreement stipulates that this "
							"version of netPanzer can be posted for download.  See "
							"license.txt for further details."
							"\n"
							"netPanzerTest only supports TCP/IP multiplayer games over "
							"LAN systems and over the Internet. Skirmish mode, direct connect, "
							"and modem games will not work in netPanzerTest. These connection "
							"types and the skirmish mode will be supported in the downloadable "
							"public beta.";


	viewArea.bltStringInBox(bodyTextRect, text, Color::white, 12);

} // end MainMenuView::doDraw
