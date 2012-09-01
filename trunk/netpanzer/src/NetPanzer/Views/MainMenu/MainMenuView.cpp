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
#include "Views/Components/Desktop.hpp"
#include "Interfaces/GameManager.hpp"
#include "Views/GameViewGlobals.hpp"

#include "Interfaces/GameConfig.hpp"

#ifndef PACKAGE_VERSION
	#define PACKAGE_VERSION "testing"
#endif

// size is: 620x450 located @ 90,90
static iRect mainTextRect(0, 0, 620, 370);
static iRect mainNewsRect(0, 380, 620, 450);

static const char loading_message[] = "Loading news...";

const char * MainMenuView::news_message = loading_message;

void MainMenuView::setNews(const std::string &news)
{
    if ( news_message != loading_message )
    {
        free(const_cast<char*>(news_message));
    }

    news_message = strdup(news.c_str());
}

// MainMenuView
//---------------------------------------------------------------------------
MainMenuView::MainMenuView() : RMouseHackView()
{
    setSearchName("MainView");
    setTitle("Main");
    setSubTitle("");
    
    setAllowResize(false);
    setVisible(false);
    setAllowMove(false);
    setDisplayStatusBar(false);
    setBordered(false);
    setAlwaysOnBottom(true);

    moveTo(iXY(90, 90));
    resize(iXY(620, 450));

//    setVisible(true);
} // end MainMenuView::MainMenuView

MainMenuView::~MainMenuView()
{
    if ( news_message != loading_message )
    {
        free(const_cast<char*>(news_message));
    }
    news_message = loading_message;
}

// doDraw
//---------------------------------------------------------------------------
void MainMenuView::doDraw(Surface &viewArea, Surface &clientArea)
{
    RMouseHackView::doDraw(viewArea, clientArea);

    static char text[] =
        "This is NetPanzer version " PACKAGE_VERSION
        ", a massively multiplayer tank battle game.  "
        "This application is free software under the terms of the "
        "Gnu General Public license (GPL). See the COPYING file for details."
        "\nWe command you to go to www.netpanzer.org and meet us in forum.\n\n"
        "Current Team\n\n"
        "   krom: the jester\n"
        "   C-D: the insurgent\n"
        "   fu: the ninja\n"
        "   Wile64: the hunter\n"
        "\n"
        "Previous Authors\n\n"
        "Original Game (Pyrosoft)\n"
        "   Vlad Rahkoy, Skip Rhudy, Matt Bogue, Clint Bogue\n\n"
        "Linux Port, Polishing\n"
        "   Matthias Braun, Ivo Danihelka, Hollis Blanchard, Hankin Chick,\n"
        "   Tyler Nielsesn, Pronobozo\n\n"
        "Packaging:\n"
        "   Bastosz Fenski, BenUrban\n\n"
        "Related Tools:\n"
        "   Tobias Blerch, Ingo Ruhnke\n\n"
        "NetPanzer News:";

    viewArea.bltStringInBox(mainTextRect, text, windowTextColor, 12, false);
    viewArea.bltStringInBox(mainNewsRect, news_message, windowTextColor, 12, false);
} // end MainMenuView::doDraw
