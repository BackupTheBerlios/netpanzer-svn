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
#include "2D/Palette.hpp"
#include "Interfaces/PlayerInterface.hpp"
#include "Interfaces/TeamManager.hpp"
#include "Interfaces/GameConfig.hpp"
#include "Interfaces/GameControlRulesDaemon.hpp"
#include "Interfaces/MapInterface.hpp"
#include "Interfaces/KeyboardInterface.hpp"
#include "Interfaces/ConsoleInterface.hpp"
#include "Views/Components/tButton.hpp"
#include "Views/Theme.hpp"
#include "Objectives/ObjectiveInterface.hpp"
#include "Resources/ResourceManager.hpp"

static const char * stats_format = "%-20s";
static Uint8 newteam = 0;

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

    loaded = false;
}

void PrepareTeam::init()
{
    removeComponents();
    removeAllButtons();
    iRect viewrect = getClientRect();

    menuImageXY.x = (viewrect.getSizeX()/2) - 382;
    menuImageXY.y = (viewrect.getSizeY()/2.5) - 190;

    rect.min.x = menuImageXY.x+ ((menuImage.getWidth()-500)/2);
    rect.min.y = menuImageXY.y+menuImage.getHeight();
    rect.max.x = rect.min.x+500;
    rect.max.y = rect.min.y+330;

    firstrect.min.x = rect.min.x+7;
    firstrect.min.y = rect.min.y+7;
    firstrect.max.x =  rect.min.x+200;
    firstrect.max.y = rect.max.y-7;

    secondrect.min.x = rect.max.x-207;
    secondrect.min.y = firstrect.min.y;
    secondrect.max.x = rect.max.x-7;
    secondrect.max.y = firstrect.max.y;
    
    changebutton = tButton::createtButton( "changeteam", " >> ", iXY(firstrect.max.x+10, (firstrect.min.y+50)), (secondrect.min.x-firstrect.max.x)-20);
    add(changebutton);
    readybutton = tButton::createtButton( "ready", "Ready", iXY(firstrect.max.x+10, (firstrect.min.y+85)), (secondrect.min.x-firstrect.max.x)-20);
    add(readybutton);
    loaded = true;
}

void PrepareTeam::doDraw(Surface &viewArea, Surface &clientArea)
{
    menuImage.bltTrans(clientArea, menuImageXY.x, menuImageXY.y);
    clientArea.FillRoundRect(rect, 12, ctWindowsbackground);
    clientArea.RoundRect(rect,12, ctWindowsBorder);

    clientArea.RoundRect(firstrect,10, TeamManager::getTeamColor(0));
    clientArea.RoundRect(secondrect,10, TeamManager::getTeamColor(1));
    DrawInfo(clientArea);
    drawTeams(clientArea);
    vsImage.bltTrans(clientArea, firstrect.max.x+15, firstrect.max.y-vsImage.getHeight()-10);
    View::doDraw(viewArea, clientArea);
}

void PrepareTeam::DrawInfo(Surface &dest)
{
    iXY start;
    start.x = menuImageXY.x+245;
    start.y = menuImageXY.y+115;
    int nextx = 145;
    char statBuf[256];
    if (GameControlRulesDaemon::getTeamCD() < 1)
    {
        dest.bltString(start.x-5 , rect.min.y-5, "Get ready, the battle will begin...", ctTexteNormal);
    }
    else
    {
        snprintf(statBuf, sizeof(statBuf), "%d", GameControlRulesDaemon::getTeamCD());
        dest.bltString(firstrect.max.x+40 , firstrect.min.y+5, statBuf, ctTexteNormal);
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

    states.clear();
    PlayerID i;
    for( i = 0; i < PlayerInterface::getMaxPlayers(); ++i)
    {
        PlayerState* state = PlayerInterface::getPlayer(i);
        if( state->isActive() )
        {
            states.push_back(state);
        }
    }
    std::sort(states.begin(), states.end(), StatesSortByTeam());

    int cur_line_pos = firstrect.min.y +10;
    snprintf(statBuf, sizeof(statBuf), "Team: %-20s", TeamManager::getTeamName(0).c_str());
    dest.bltString(firstrect.min.x+40, cur_line_pos, statBuf,TeamManager::getTeamColor(0));
    TeamManager::drawFlag(0, dest, firstrect.min.x+10, cur_line_pos-2);

    snprintf(statBuf, sizeof(statBuf), "Team: %-20s", TeamManager::getTeamName(1).c_str());
    dest.bltString(secondrect.min.x+40, cur_line_pos, statBuf,TeamManager::getTeamColor(1));
    TeamManager::drawFlag(1, dest, secondrect.min.x+10, cur_line_pos-2);

    cur_line_pos += 25;
    dest.drawLine(firstrect.min.x+10, cur_line_pos-10, firstrect.max.x-10, cur_line_pos-10, TeamManager::getTeamColor(0));
    dest.drawLine(secondrect.min.x+10, cur_line_pos-10, secondrect.max.x-10, cur_line_pos-10, TeamManager::getTeamColor(1));

    int Start_x = firstrect.min.x+30;
    int current_Team = 0;
    Surface * flag = 0;
    PIX textcolor;
    for(std::vector<const PlayerState*>::iterator i = states.begin();
            i != states.end(); ++i)
    {
        const PlayerState* state = *i;

        if (current_Team != state->getTeamID())
        {
            cur_line_pos = firstrect.min.y +35;
            Start_x = secondrect.min.x+30;
            current_Team = state->getTeamID();
        }
        flag = ResourceManager::getFlag(state->getFlag());
        flag->blt( dest, Start_x-23, cur_line_pos-5 );

        textcolor = ctTexteNormal;
        if (TeamManager::isPlayerReady(state->getID()))
            textcolor = ctTexteDisable;
        else if ( state->getID() == PlayerInterface::getLocalPlayerIndex() )
            textcolor = ctTexteOver;

        if ( state->getID() == PlayerInterface::getLocalPlayerIndex() )
        {
            snprintf(statBuf, sizeof(statBuf),
                     stats_format, state->getName().substr(0,20).c_str());
            dest.bltString(Start_x, cur_line_pos, statBuf,textcolor);
            if (current_Team > 0)
            {
                newteam=0;
                changebutton->setLabel("<<");
            }
            else
            {
                newteam=1;
                changebutton->setLabel(">>");
            }
        }
        else
        {
            snprintf(statBuf, sizeof(statBuf),
                     stats_format, state->getName().substr(0,20).c_str());
            dest.bltString(Start_x, cur_line_pos, statBuf,textcolor);
        }
        cur_line_pos += 20;
    }
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
        removeComponents();
        removeAllButtons();
        loaded = false;
    }
}

void PrepareTeam::checkResolution(iXY oldResolution, iXY newResolution)
{
    resize(iXY(newResolution.x, newResolution.y));
    moveTo(iXY(0,0));
}

void PrepareTeam::processEvents()
{
    COMMAND_PROCESSOR.processChat();
}

void PrepareTeam::onComponentClicked(Component* c)
{
    if ( c == changebutton )
    {
        TeamManager::PlayerrequestchangeTeam(PlayerInterface::getLocalPlayerIndex(), newteam);
    }
    else if ( c == readybutton )
    {
        TeamManager::PlayerRequestReady(PlayerInterface::getLocalPlayerIndex());
        changebutton->Disable();
        readybutton->Disable();
    }
    else
    {
        View::onComponentClicked(c);
    }
}

void PrepareTeam::resize(const iXY &size)
{
    View::resize(size);
    init();
}




