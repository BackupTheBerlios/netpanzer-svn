/*
Copyright (C) 2012 Netpanzer Team. (www.netpanzer.org), Laurent Jacques

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

#include "PrepareTeam.hpp"
#include "Classes/ScreenSurface.hpp"
#include "Classes/WorldInputCmdProcessor.hpp"
#include "Views/Components/Desktop.hpp"
#include "Views/Components/Button.hpp"
#include "Views/Theme.hpp"
#include "2D/Palette.hpp"
#include "Interfaces/PlayerInterface.hpp"
#include "Interfaces/TeamManager.hpp"
#include "Interfaces/GameConfig.hpp"
#include "Interfaces/GameControlRulesDaemon.hpp"
#include "Interfaces/MapInterface.hpp"
#include "Interfaces/KeyboardInterface.hpp"
#include "Interfaces/ConsoleInterface.hpp"
#include "Objectives/ObjectiveInterface.hpp"

PrepareTeam::PrepareTeam() : GameTemplateView()
{
    setSearchName("PrepareTeam");
    setTitle("Prepare Team");
    setSubTitle("");

    setAllowResize(false);
    setAllowMove(false);
    setVisible(false);
    setBordered(false);
    menuImage.loadBMP("pics/backgrounds/menus/menu/canon.bmp");
    vsImage.loadBMP("pics/backgrounds/menus/menu/vs.bmp");

    moveTo((screen->getWidth()/2) - (menuImage.getWidth()/2), 
           (screen->getHeight()/2) - ( menuImage.getHeight()/1.5));
    resize(menuImage.getWidth(), menuImage.getHeight()+300);
    menuImageXY.x = 0;
    menuImageXY.y = 0;

    rect.min.x = menuImageXY.x+ ((menuImage.getWidth()-500)/2);
    rect.min.y = menuImageXY.y+menuImage.getHeight();
    rect.max.x = rect.min.x+500;
    rect.max.y = rect.min.y+300;

    firstrect.min.x = rect.min.x+7;
    firstrect.min.y = rect.min.y+30;
    firstrect.max.x =  rect.min.x+180;
    firstrect.max.y = rect.max.y-7;

    secondrect.min.x = rect.max.x-185;
    secondrect.min.y = firstrect.min.y;
    secondrect.max.x = rect.max.x-20;
    secondrect.max.y = firstrect.max.y;
    
    changebutton = Button::createNewSpecialButton( "changeteam", " >> ", iXY(firstrect.max.x+29, (firstrect.min.y+50)), (secondrect.min.x-firstrect.max.x)-39);
    add(changebutton);
    readybutton = Button::createNewSpecialButton( "ready", "Ready", iXY(firstrect.max.x+29, (firstrect.min.y+85)), (secondrect.min.x-firstrect.max.x)-39);
    add(readybutton);

    scTeam1 = new tVScrollBar();
    add(scTeam1);
    StateTeam1 = new tPlayerStateBox(firstrect, 0);
    StateTeam1->setVscrollBar(scTeam1);
    StateTeam1->setShowTeam(0);
    add(StateTeam1);

    scTeam2 = new tVScrollBar();
    add(scTeam2);
    StateTeam2 = new tPlayerStateBox(secondrect, 0);
    StateTeam2->setVscrollBar(scTeam2);
    StateTeam2->setShowTeam(1);
    add(StateTeam2);
    loaded = false;
}

void PrepareTeam::init()
{
    moveTo((screen->getWidth()/2) - (menuImage.getWidth()/2), 
           (screen->getHeight()/3) - ( menuImage.getHeight()));
    resize(menuImage.getWidth(), menuImage.getHeight()+300);
    menuImageXY.x = 0;
    menuImageXY.y = 0;

    rect.min.x = menuImageXY.x+ ((menuImage.getWidth()-500)/2);
    rect.min.y = menuImageXY.y+menuImage.getHeight();
    rect.max.x = rect.min.x+500;
    rect.max.y = rect.min.y+300;

    firstrect.min.x = rect.min.x+7;
    firstrect.min.y = rect.min.y+30;
    firstrect.max.x =  rect.min.x+177;
    firstrect.max.y = rect.max.y-7;

    secondrect.min.x = rect.max.x-190;
    secondrect.min.y = firstrect.min.y;
    secondrect.max.x = rect.max.x-25;
    secondrect.max.y = firstrect.max.y;
    
    changebutton->setLocation(iXY(firstrect.max.x+29, (firstrect.min.y+50)));
    readybutton->setLocation(iXY(firstrect.max.x+29, (firstrect.min.y+85)));
    StateTeam1->setLocation(firstrect.min);
    StateTeam2->setLocation(secondrect.min);
    StateTeam1->UpdateState(true);
    StateTeam2->UpdateState(true);
    loaded = true;
}

void PrepareTeam::doDraw(Surface &viewArea, Surface &clientArea)
{
    menuImage.bltTrans(clientArea, menuImageXY.x, menuImageXY.y);
    clientArea.FillRoundRect(rect, 12, ctWindowsbackground);
    clientArea.RoundRect(rect,12, ctWindowsBorder);

    DrawInfo(clientArea);
    drawTeams(clientArea);
    vsImage.bltTrans(clientArea, firstrect.max.x+40, firstrect.max.y-vsImage.getHeight()-10);
    View::doDraw(viewArea, clientArea);
    StateTeam1->UpdateState(false);
    StateTeam2->UpdateState(false);
}

void PrepareTeam::DrawInfo(Surface &dest)
{
    iXY start;
    start.x = menuImageXY.x+210;
    start.y = menuImageXY.y+100;
    int nextx = 125;
    char statBuf[256];
    if (GameControlRulesDaemon::getTeamCD() < 1)
    {
        dest.bltString(start.x-5 , rect.min.y-5, "Get ready, the battle will begin...", ctTexteNormal);
    }
    else
    {
        snprintf(statBuf, sizeof(statBuf), "%d", GameControlRulesDaemon::getTeamCD());
        dest.bltString(firstrect.max.x+60 , firstrect.min.y+5, statBuf, ctTexteNormal);
    }
    dest.bltString(start.x , start.y, "Game:", ctTexteNormal);
    dest.bltString(start.x+nextx , start.y, "Map:", ctTexteNormal);
    start.y+= 10;
    dest.drawLine(start.x, start.y, start.x+100, start.y, ctTexteNormal);
    dest.drawLine(start.x+nextx, start.y, start.x+nextx+100, start.y,ctTexteNormal);
    start.y+= 7;
    switch(GameConfig::game_gametype)
    {
    case  _gametype_timelimit:
    {
        snprintf(statBuf, sizeof(statBuf), "Time limit: %i", GameConfig::game_timelimit);
        break;
    }
    case _gametype_fraglimit:
    {
        snprintf(statBuf, sizeof(statBuf), "Frag limit: %i", GameConfig::game_fraglimit);
        break;
    }
    case _gametype_objective:
    {
        snprintf(statBuf, sizeof(statBuf), "Objective: %i", GameConfig::game_occupationpercentage);
        break;
    }
    }
    dest.bltString(start.x, start.y, statBuf, ctTexteNormal);
    snprintf(statBuf, sizeof(statBuf), "Name: %-20s", MapInterface::getMap()->getName().c_str());
    dest.bltString(start.x+nextx, start.y, statBuf, ctTexteNormal);
    start.y+= 13;
    snprintf(statBuf, sizeof(statBuf), "Players: %i/%i",  
             PlayerInterface::getActivePlayerCount(), 
             GameConfig::game_maxplayers);
    dest.bltString(start.x, start.y, statBuf, ctTexteNormal);
    snprintf(statBuf, sizeof(statBuf), "Objectives: %i", (int) ObjectiveInterface::getObjectiveCount());
    dest.bltString(start.x+nextx, start.y, statBuf, ctTexteNormal);
    start.y+= 13;
    snprintf(statBuf, sizeof(statBuf), "Units: %i", GameConfig::game_maxunits/GameConfig::game_maxplayers);
    dest.bltString(start.x, start.y, statBuf, ctTexteNormal);
}

class StatesSortByTeam
    : public std::binary_function<const PlayerState*, const PlayerState*, bool>
{
public:
    bool operator() (const PlayerState* state1, const PlayerState* state2)
    {
        return state1->getTeamID() < state2->getTeamID();
    }
};

void PrepareTeam::drawTeams(Surface &dest)
{
    char statBuf[256];

    int cur_line_pos = rect.min.y +10;
    snprintf(statBuf, sizeof(statBuf), "Team: %-20s", TeamManager::getTeamName(0).c_str());
    dest.bltString(firstrect.min.x+40, cur_line_pos, statBuf,TeamManager::getTeamColor(0));
    TeamManager::drawFlag(0, dest, firstrect.min.x+10, cur_line_pos-2);

    snprintf(statBuf, sizeof(statBuf), "Team: %-20s", TeamManager::getTeamName(1).c_str());
    dest.bltString(secondrect.min.x+40, cur_line_pos, statBuf,TeamManager::getTeamColor(1));
    TeamManager::drawFlag(1, dest, secondrect.min.x+10, cur_line_pos-2);

    cur_line_pos += 25;
    dest.drawLine(firstrect.min.x+10, cur_line_pos-10, firstrect.max.x-10, cur_line_pos-10, TeamManager::getTeamColor(0));
    dest.drawLine(secondrect.min.x+10, cur_line_pos-10, secondrect.max.x-10, cur_line_pos-10, TeamManager::getTeamColor(1));
}
void PrepareTeam::doActivate()
{
    if ( ! loaded )
    {
        init();
    }
    Desktop::setActiveView(this);
}

void PrepareTeam::doDeactivate()
{
    if ( ! getVisible() )
    {
        loaded = false;
    }
}

void PrepareTeam::checkResolution(iXY oldResolution, iXY newResolution)
{
    init();
}

void PrepareTeam::processEvents()
{
    //COMMAND_PROCESSOR.processChat();
    View::processEvents();
}

void PrepareTeam::onComponentClicked(Component* c)
{
    if ( c == changebutton )
    {
        
        if (PlayerInterface::getLocalPlayer()->getTeamID() == 0)
        {
            TeamManager::PlayerrequestchangeTeam(PlayerInterface::getLocalPlayerIndex(), 1);
            changebutton->setLabel(" << ");
        }
        else
        {
            TeamManager::PlayerrequestchangeTeam(PlayerInterface::getLocalPlayerIndex(), 0);
            changebutton->setLabel(" >> ");
        }
    }
    else if ( c == readybutton )
    {
        TeamManager::PlayerRequestReady(PlayerInterface::getLocalPlayerIndex());
        changebutton->disable();
        readybutton->disable();
    }
    else
    {
        View::onComponentClicked(c);
    }
}




