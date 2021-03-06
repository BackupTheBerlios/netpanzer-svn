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

    static char	text[] =
        "This is NetPanzer, a massively multiplayer tank battle game.  "
        "This application is free software under the terms of the "
        "Gnu General Public license (GPL). See the COPYING file for details."
        "\n"
        "NetPanzer only supports TCP/IP multiplayer games over "
        "LAN systems and over the Internet. A Skirmish mode might "
        "be added later. We're still searching for talented coders "
        "and artists. Look at http://www.nongnu.org/netpanzer for details. ";

    viewArea.bltStringInBox(bodyTextRect, text, Color::white, 12);

} // end MainMenuView::doDraw
