/*
 *  LoadingView.cpp
 *  netpanzer-osx
 *
 *  Created by krom on 4/27/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "LoadingView.hpp"

#include "Classes/ScreenSurface.hpp"
#include "Classes/Network/NetworkClient.hpp"

#include "Interfaces/GameConfig.hpp"
#include "Interfaces/GameManager.hpp"
#include "Interfaces/PlayerInterface.hpp"
#include "Interfaces/StrManager.hpp"

#include "2D/Components/Label.hpp"
#include "2D/Components/InputField.hpp"
#include "Views/Game/VehicleSelectionView.hpp"
#include "Views/Game/ChatView.hpp"

#include "Particles/ParticleInterface.hpp"

#include "Actions/Action.hpp"
#include "Actions/ActionManager.hpp"

#include "Util/Log.hpp"

list<string> LoadingView::lines;
bool LoadingView::dirty = true;

class BeginPasswordConnectAction : public Action
{
public:
    LoadingView * view;
    BeginPasswordConnectAction(LoadingView * view) : Action(false), view(view) {}
    void execute()
    {
        CLIENT->joinServer(gameconfig->serverConnect, NPString(view->getPassword()));
        ClientConnectDaemon::startConnectionProcess();
        view->setNeedPassword(false);
    }
};
        
void
LoadingView::init()
{
    setSearchName("LoadingView");
//    setTitle(_("Loading Progress"));

    setAllowMove(false);
    need_password = false;
    
    password = new InputField();
    password->setLocation(175+88, 80);
    password->setSize(32 * 8, 16);
    password->setMaxTextLength(31);

    resize(800, 480);

    Action * onAbort = ( gameconfig->quickConnect )
                       ? ActionManager::getAction("quit")
                       : ActionManager::getAction("disconnect");
    
    add( Button::createTextButton(_("Abort"), iXY(340, 15), 100));

    okButton = Button::createTextButton(_("Enter"), iXY(340,100), 100); // new BeginPasswordConnectAction(this)
    
    passwordLabel = new Label(340, 68, _("Game Password"), Color::white);

}


void
LoadingView::doDraw( Surface& dest )
{
    if (dirty)
        render();

    screen->fill(Color::black);
    backgroundSurface.blt(dest, 0, 0); // full blit
    dest.FillRoundRect(iRect(165, 40, 470, 185), 10, Color::black);
    dest.RoundRect(iRect(165, 40, 470, 185), 10, Color::yellow);
    surface.blt(dest, 172, 45); // full blit

    View::doDraw( dest );
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
    backgroundSurface.loadBMP("pics/backgrounds/defaultMB.bmp");
    surface.create(455, 175);
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
            password->clearText();
            add(password);
            add(okButton);
            add(passwordLabel);
        }
        else
        {
            removeComponent(password);
            removeComponent(okButton);
            removeComponent(passwordLabel);
        }
    }
}

//void
//LoadingView::update(const NPString& text)
//{
//    LOGGER.info("Loading: %s", text.c_str());
//    lines.pop_back();
//    lines.push_back(text);
//    dirty=true;
//}

//void
//LoadingView::append(const NPString& text)
//{
//    LOGGER.info("Loading: %s", text.c_str());
//    if ( lines.size() >= LINELIMIT )
//    {
//        lines.pop_front();
//    }
//    lines.push_back(text);
//    dirty=true;
//}

//void
//LoadingView::loadFinish()
//{
//    // XXX rebuild the particle stuff here for units...
//    ParticleInterface::rebuildUnitParticleData();
//
//    // XXX this needed because has to create the special buttons for the
//    // defined units, has to be here because it has to be the "netp" palette
//    // and after loading the unit profiles
//    View * v = 0; //Desktop::getView("VehicleSelectionView");
//    if ( v )
//    {
////        Desktop::remove(v);
//        delete v;
//    }
//
////    Desktop::add(new VehicleSelectionView());
//
//    GameManager::setNetPanzerGameOptions();
////    Desktop::setVisibility("MiniMapView", true);
////    Desktop::setVisibility("GameView", true);
////    Desktop::setVisibility("ChatView", true);
//    ChatView *cv = 0; // (ChatView*) Desktop::getView("ChatView");
//    if (cv) cv->clear();
//    
//    if ( PlayerInterface::getLocalPlayer()->isSelectingFlag() )
//    {
////        Desktop::setVisibility("GFlagSelectionView", true);
////        Desktop::setActiveView("GFlagSelectionView");
//    }
//}

//void
//LoadingView::show()
//{
////    Desktop::setVisibilityAllWindows(false);
////    Desktop::setVisibility("LoadingView", true);
////    View *v = Desktop::getView("LoadingView");
////    Desktop::setFocusView(v);
//}

//void
//LoadingView::hide()
//{
////    Desktop::setVisibility("LoadingView", false);
//}
