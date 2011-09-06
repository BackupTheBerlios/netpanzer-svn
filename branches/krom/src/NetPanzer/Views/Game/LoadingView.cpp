/*
 *  LoadingView.cpp
 *  netpanzer-osx
 *
 *  Created by krom on 4/27/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "LoadingView.hpp"

#include "Interfaces/GameConfig.hpp"
#include "System/Sound.hpp"
#include "2D/Palette.hpp"
#include "Classes/ScreenSurface.hpp"
#include "Interfaces/GameManager.hpp"
#include "Views/Components/Desktop.hpp"

list<string> LoadingView::lines;
bool LoadingView::dirty = true;

static void bAbort()
{
    if(gameconfig->quickConnect) {
        GameManager::exitNetPanzer();
        return;
    }
    sound->stopTankIdle();
    Desktop::hideAllWindows();
    Desktop::setVisibility("MainView", true);
}


void
LoadingView::init()
{
    setSearchName("LoadingView");
    setTitle("Loading Progress");
    setSubTitle("");
    
    setAllowResize(false);
    setAllowMove(false);
    setDisplayStatusBar(false);
    setVisible(false);
    setBordered(false);

    resize(640, 480);
    
    addButtonCenterText(iXY(628 - 60, 302 - 15), 60, "Abort", "Cancel the joining of this game.", bAbort);
    
}


void
LoadingView::doDraw(Surface &viewArea, Surface &clientArea)
{
    if ( Palette::getName() != "netpmenu" )
    {
        GameManager::loadPalette("netpmenu");
    }
    
    if (dirty)
        render();
    
    screen->fill(Color::black);
    backgroundSurface.blt(clientArea, 0, 0);
    surface.blt(clientArea, 179, 153);
    
//    View::doDraw(viewArea, clientArea);
}

void
LoadingView::render()
{
    dirty=false;
    
    surface.fill(Color::black);
    
    int ypos = surface.getHeight() - Surface::getFontHeight();
    int fontHeight = Surface::getFontHeight();
    
    list<string>::reverse_iterator i = lines.rbegin();
    while ( i != lines.rend() && ypos > -fontHeight )
    {
        surface.bltString( 0, ypos, (*i).c_str(), Color::white);
        i++;
        ypos -= fontHeight;
    }
}

void
LoadingView::doActivate()
{
    backgroundSurface.loadBMP("pics/backgrounds/menus/menu/loadingMB.bmp");
    surface.create(628 - 179, 302 - 153, 1);
    dirty=true;
}

void
LoadingView::doDeactivate()
{
    backgroundSurface.free();
    surface.free();
}
