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


#include "MainMenuView.hpp"
#include "JoinView.hpp"
#include "Views/Components/Desktop.hpp"
#include "Views/Components/Button.hpp"
#include "Interfaces/GameManager.hpp"
#include "Views/GameViewGlobals.hpp"
#include "Classes/ScreenSurface.hpp"

#include "Views/MainMenu/Multi/HostJoinTemplateView.hpp"
#include "Views/MainMenu/Multi/GetSessionView.hpp"
#include "Interfaces/GameConfig.hpp"

#ifndef PACKAGE_VERSION
	#define PACKAGE_VERSION "testing"
#endif

class JoinView;

//static void bJoin()
//{
//    View *jv = Desktop::getView("XJoinView");
//    if ( !jv )
//    {
//        jv = new JoinView();
//        Desktop::add(jv);
//    }
////    Desktop::hideLastView();
//    Desktop::setVisibility(jv, true);
//}

// MainMenuView
//---------------------------------------------------------------------------
MainMenuView::MainMenuView()
{
    setSearchName("MainView");
    setTitle("Menu");
    setSubTitle("");

    setAllowResize(false);
    setAllowMove(false);
    //setBordered(false);

    int x = 10;
    int y = 10;

    moveTo(iXY(0, 0));

    Button * joinbutton = new Button();
    joinbutton->loadPackedImage("pics/backgrounds/menus/buttons/default/pak/inMenusJoin.pak");
    joinbutton->setLocation(x, y);
//    exitbutton->clicked.connect(this, &MainMenuView::onExitButtonClicked);
    add(joinbutton);

    y += 22;
    Button * hostbutton = new Button();
    hostbutton->loadPackedImage("pics/backgrounds/menus/buttons/default/pak/inMenusHost.pak");
    hostbutton->setLocation(x, y);
//    exitbutton->clicked.connect(this, &MainMenuView::onExitButtonClicked);
    add(hostbutton);

    y += 22;
    Button * optionsbutton = new Button();
    optionsbutton->loadPackedImage("pics/backgrounds/menus/buttons/default/pak/inMenusOptions.pak");
    optionsbutton->setLocation(x, y);
//    exitbutton->clicked.connect(this, &MainMenuView::onExitButtonClicked);
    add(optionsbutton);

    y += 22;
    Button * helpbutton = new Button();
    helpbutton->loadPackedImage("pics/backgrounds/menus/buttons/default/pak/inMenusHelp.pak");
    helpbutton->setLocation(x, y);
//    exitbutton->clicked.connect(this, &MainMenuView::onExitButtonClicked);
    add(helpbutton);

    y += 22;
    Button * mainbutton = new Button();
    mainbutton->loadPackedImage("pics/backgrounds/menus/buttons/default/pak/inMenusMain.pak");
    mainbutton->setLocation(x, y);
//    exitbutton->clicked.connect(this, &MainMenuView::onExitButtonClicked);
    add(mainbutton);

    y += 22;

    Button * exitbutton = new Button();
    exitbutton->loadPackedImage("pics/backgrounds/menus/buttons/default/pak/inMenusExit netPanzer.pak");
    exitbutton->setLocation(x, y);
    exitbutton->clicked.connect(this, &MainMenuView::onExitButtonClicked);
    add(exitbutton);

    y += 22;
    resize(iXY(x*2 + 141, y + 20));
/*
    textRect.min.x = x*2 + 141 + 10;
    textRect.min.y = 10;
    textRect.max.x = 630 - textRect.min.x;
    textRect.max.y = 470 - textRect.min.y;
*/

    setBackgroundColor(Color::black);
    setVisible(true);
} // end MainMenuView::MainMenuView

void MainMenuView::onExitButtonClicked()
{
    GameManager::exitNetPanzer();
}

/* // for later drawing in a box
    static char text[] =
        "This is NetPanzer version " PACKAGE_VERSION
        ", a massively multiplayer tank battle game.  "
        "This application is free software under the terms of the "
        "Gnu General Public license (GPL). See the COPYING file for details."
        "\n\nWe command you to go to www.netpanzer.org and meet us in forum.\n\n\n"
        "Current Team\n\n"
        "   krom: the warrior\n"
        "   C-D: the insurgent\n"
        "   fu: the anarchist\n"
        "\n\n\n"
        "Authors\n\n"
        "Original Game (Pyrosoft)\n"
        "   Vlad Rahkoy, Skip Rhudy, Matt Bogue, Clint Bogue\n\n"
        "Linux Port, Polishing\n"
        "   Matthias Braun, Ivo Danihelka, Hollis Blanchard, Hankin Chick,\n"
        "   Tyler Nielsesn, Pronobozo\n\n"
        "Packaging:\n"
        "   Bastosz Fenski, BenUrban\n\n"
        "Related Tools:\n"
        "   Tobias Blerch, Ingo Ruhnke\n";

    viewArea.bltStringInBox(textRect, text, Color::white, 12, true);
*/
