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

#include "GTeamView.hpp"
#include "Classes/ScreenSurface.hpp"
#include "Classes/WorldInputCmdProcessor.hpp"
#include "Views/Components/Desktop.hpp"
#include "2D/Palette.hpp"
#include "Interfaces/PlayerInterface.hpp"
#include "Interfaces/TeamManager.hpp"
#include "Views/Components/cButton.hpp"


static const char * stats_format = "%-20s%6i%7i";
static Uint8 newteam = 0;

static void bChangeTeam()
{
    TeamManager::PlayerrequestchangeTeam(PlayerInterface::getLocalPlayerIndex(), newteam);
}

GTeamView::GTeamView() : GameTemplateView()
{
    setSearchName("GTeamView");
    setTitle("Team View");
    setSubTitle("");

    setAllowResize(false);
    setAllowMove(false);
    setVisible(false);
    setBordered(false);

    loaded = false;
    TeamOneImage.loadBMP("pics/default/team-one.bmp");
    TeamTwoImage.loadBMP("pics/default/team-two.bmp");

}

void GTeamView::init()
{
    removeComponents();
    removeAllButtons();
    iRect viewrect = getClientRect();


    firstrect.min.x = (viewrect.getSizeX()/2) - 350;
    firstrect.min.y = (viewrect.getSizeY()/2) - 250;
    firstrect.max.x = firstrect.min.x + 350;
    firstrect.max.y = firstrect.min.y + 500;

    secondrect.min.x = firstrect.max.x+30;
    secondrect.min.y = firstrect.min.y;
    secondrect.max.x = secondrect.min.x + 350;
    secondrect.max.y = secondrect.min.y + 500;
    
    addButtonCenterText( iXY(firstrect.max.x+2, firstrect.min.y+18), 28, "<->", "", bChangeTeam);
    loaded = true;
}

void GTeamView::doDraw(Surface &viewArea, Surface &clientArea)
{
    clientArea.BltRoundRect(firstrect, 14, Palette::darkGray256.getColorArray());
    clientArea.RoundRect(firstrect,14, Color::gray);

    clientArea.BltRoundRect(secondrect, 14, Palette::darkGray256.getColorArray());
    clientArea.RoundRect(secondrect,14, Color::gray);

    drawTeams(clientArea, 20);
    View::doDraw(viewArea, clientArea);
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

void GTeamView::drawTeams(Surface &dest, unsigned int flagHeight)
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

    int cur_line_pos = firstrect.min.y +20;
    dest.bltStringShadowed(firstrect.min.x+10, cur_line_pos, "Team :",Color::lightGreen, Color::gray64);
    TeamOneImage.bltTrans(dest, firstrect.min.x+60, cur_line_pos-5 );
    dest.bltStringShadowed(secondrect.min.x+10, cur_line_pos, "Team :",Color::lightGreen, Color::gray64);
    TeamTwoImage.bltTrans(dest, secondrect.min.x+60, cur_line_pos-5 );
    cur_line_pos += 25;
    dest.drawLine(firstrect.min.x+10, cur_line_pos-10, firstrect.max.x-10, cur_line_pos-10, Color::lightGreen);
    dest.drawLine(secondrect.min.x+10, cur_line_pos-10, secondrect.max.x-10, cur_line_pos-10, Color::lightGreen);

    int Start_x = firstrect.min.x+10;
    int current_Team = 0;

    for(std::vector<const PlayerState*>::iterator i = states.begin();
            i != states.end(); ++i)
    {
        const PlayerState* state = *i;

        if (current_Team != state->getTeamID())
        {
            cur_line_pos = firstrect.min.y +45;
            Start_x = secondrect.min.x+10;
            current_Team = state->getTeamID();
        }
        if ( state->getID() == PlayerInterface::getLocalPlayerIndex() )
        {
            removeComponents();
            if (current_Team > 0)
            {
                 newteam=0;
            }
            else
            {
                 newteam=1;
            }
        }
        snprintf(statBuf, sizeof(statBuf),
                 stats_format, state->getName().substr(0,20).c_str(),
                 state->getKills(), state->getLosses());
        dest.bltStringShadowed(Start_x, cur_line_pos, statBuf,Color::gray224, Color::gray64);
        cur_line_pos += 20;
    }
}
void GTeamView::doActivate()
{
    if ( ! loaded )
    {
        init();
    }
    Desktop::setActiveView(this);
}

void GTeamView::doDeactivate()
{
    if ( ! getVisible() )
    {
        removeComponents();
        removeAllButtons();
        loaded = false;
    }
}

void GTeamView::checkResolution(iXY oldResolution, iXY newResolution)
{
    resize(iXY(newResolution.x, newResolution.y));
    moveTo(iXY(0,0));
}

void GTeamView::processEvents()
{
    COMMAND_PROCESSOR.process(false);
}
