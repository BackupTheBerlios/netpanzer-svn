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
#include "Views/Components/Button.hpp"

list<string> LoadingView::lines;
bool LoadingView::dirty = true;

enum
{
    BTN_ABORT
};

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
    
    add( Button::createTextButton("abort","Abort", iXY(628 - 60, 302 - 15), 60, BTN_ABORT) );
    
}


void
LoadingView::doDraw()
{
    if ( Palette::getName() != "netpmenu" )
    {
        GameManager::loadPalette("netpmenu");
    }
    
    if (dirty)
        render();
    
    screen->fill(Color::black);
    drawImage(backgroundSurface, 0, 0);
    drawImage(surface, 179, 153);
    
    View::doDraw();    
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

void
LoadingView::onComponentClicked(Component* c)
{
    if ( c->getCustomCode() == BTN_ABORT )
    {
        if(gameconfig->quickConnect)
        {
            GameManager::exitNetPanzer();
            return;
        }
        sound->stopTankIdle();
        Desktop::setVisibilityAllWindows(false);
        Desktop::setVisibility("MainView", true);
    }
}
