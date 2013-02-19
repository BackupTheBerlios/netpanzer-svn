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
#include "Views/Components/tStringListBox.hpp"
#include "Views/Components/tVScrollBar.hpp"
#include "Interfaces/GameManager.hpp"
#include "Views/GameViewGlobals.hpp"

#include "Interfaces/GameConfig.hpp"
#include "Interfaces/StrManager.hpp"

#ifndef PACKAGE_VERSION
	#define PACKAGE_VERSION "testing"
#endif

// size is: 620x450 located @ 90,90
static iRect mainTextRect(0, 0, 612, 270);
static iRect mainNewsRect(0, 270, 612, 70);

void MainMenuView::setNews(const std::string &news)
{
    MainMenuView * mmv = 0; //(MainMenuView*)Desktop::getView("MainView");
    mmv->news_text->Clear();
    mmv->news_text->Add(_("NetPanzer news:"));
    mmv->news_text->AddBlock(news);
}

//static Surface xxx;

// MainMenuView
//---------------------------------------------------------------------------
MainMenuView::MainMenuView() : View()
{
    setSearchName("MainView");
    
    setAllowMove(false);
    setAlwaysOnBottom(true);

    moveTo(bodyTextRect.getLocation());
    resize(bodyTextRect.getSize());
  
    char Buf[1024];    
    snprintf(Buf, sizeof(Buf), 
                _("Main Menu Message %s"),
                PACKAGE_VERSION);
    
    intro_text = new tStringListBox(mainTextRect);
    tVScrollBar * sbar = new tVScrollBar();
    sbar->setLargeChange(intro_text->getNumVisibleLines()-1);
    intro_text->setVscrollBar(sbar);
    intro_text->setColor(0); // transparent
    intro_text->setSelectable(false);
    intro_text->setBordered(false);
    intro_text->AddBlock(Buf);
    
    add(intro_text);
    add(sbar);
    
    news_text = new tStringListBox(mainNewsRect);
    sbar = new tVScrollBar();
    sbar->setLargeChange(news_text->getNumVisibleLines()-1);
    news_text->setVscrollBar(sbar);
//    news_text->setColor(0); // transparent
    news_text->setSelectable(false);
//    news_text->setBordered(false);
    news_text->Add(_("Loading news..."));
    
    add(news_text);
    add(sbar);

//    xxx.loadBMP("pics/buttons/page.bmp");
    
} // end MainMenuView::MainMenuView

MainMenuView::~MainMenuView()
{
    
}

//void MainMenuView::doDraw(Surface& surface)
//{
//    surface.drawHLine(100, 95, 100, Color::yellow);
//    surface.drawVLine(95, 100, 100, Color::yellow);
//
//    surface.drawRect(iRect(100,100, 100, 100), Color::yellow);
////    surface.fillRect(iRect(101,101,98,98), Color::green);
//
//    surface.drawLine(100,100, 199, 199, Color::cyan);
//    surface.drawLine(199,100, 100, 199, Color::cyan);
//
//    surface.BltRoundRect(iRect(100,100, 100, 100), 8, Palette::darkGray256.getColorArray());
//
//    xxx.blt(surface, 100, 100);
//    xxx.bltTrans(surface, 100, 200);
//
//    Surface X;
//    X.copy(xxx);
//
//    X.blt(surface, 350, 100);
//    X.bltTrans(surface, 350, 200);
//
//
//}
