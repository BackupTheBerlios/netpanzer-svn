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
#include "Views/Components/Label.hpp"
#include "Classes/Network/NetworkClient.hpp"

list<string> LoadingView::lines;
bool LoadingView::dirty = true;

static void bAbort()
{
    if(gameconfig->quickConnect) {
        GameManager::exitNetPanzer();
        return;
    }
    sound->stopTankIdle();
    Desktop::setVisibilityAllWindows(false);
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
    need_password = false;
    password_str.init("", 31,31);

    resize(800, 600);

    addButtonCenterText(iXY(340, 15), 100, "Abort", "Cancel the joining of this game.", bAbort);


    okButton = Button::createTextButton("OK", "Enter", iXY(340,100), 100);
    passwordLabel = new Label(340, 68, "Game Password", Color::white);

}


void
LoadingView::doDraw(Surface &viewArea, Surface &clientArea)
{
    if (dirty)
        render();

    screen->fill(Color::black);
    backgroundSurface.blt(clientArea, 0, 0);
    clientArea.FillRoundRect(iRect(165, 40, 635, 225), 10, Color::black);
    clientArea.RoundRect(iRect(165, 40, 635, 225), 10, Color::yellow);
    surface.blt(clientArea, 172, 45);

    View::doDraw(viewArea, clientArea);
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
    backgroundSurface.loadBMP("pics/backgrounds/menus/menu/defaultMB.bmp");
    surface.create(455, 175, 1);
    dirty=true;
}

void
LoadingView::doDeactivate()
{
    backgroundSurface.free();
    surface.free();
}

void
LoadingView::setNeedPassword(bool need_password)
{
    if ( this->need_password != need_password )
    {
        this->need_password = need_password;
        if ( need_password )
        {
            password_field = addInputField(iXY(175+88, 80), &password_str, "", true, 31);
            add(okButton);
            add(passwordLabel);
        }
        else
        {
            removeInputField(password_field);
            removeComponent(okButton);
            removeComponent(passwordLabel);
            password_field = 0;
        }
    }
}

void
LoadingView::onComponentClicked(Component* c)
{
    string cname = c->getName();
    if ( !cname.compare("Button.OK") )
    {
        CLIENT->joinServer(gameconfig->serverConnect, NPString(password_str.getString()));
        ClientConnectDaemon::startConnectionProcess();
        sound->playTankIdle();
        setNeedPassword(false);
    }
    else
    {
        View::onComponentClicked(c);
    }
}
