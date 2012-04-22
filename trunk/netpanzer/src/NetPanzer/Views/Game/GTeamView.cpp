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

static const char * stats_format = "%-20s%6i%7i%7i%6i";

GTeamView::GTeamView() : View()
{
    setSearchName("GTeamView");
    setTitle("GTeamView");
    setSubTitle("");

    setAllowResize(false);
    setAllowMove(false);
    setVisible(false);
    setBordered(false);

    loaded = false;
}

void GTeamView::init()
{
    removeComponents();

    iRect viewrect = getClientRect();

    rect.min.x = (viewrect.getSizeX()/2) - 250;
    rect.min.y = (viewrect.getSizeY()/2) - 250;
    rect.max.x = rect.min.x + 500;
    rect.max.y = rect.min.y + 500;

    loaded = true;
}

void GTeamView::doDraw(Surface &viewArea, Surface &clientArea)
{
    clientArea.BltRoundRect(rect, 14, Palette::darkGray256.getColorArray());
    clientArea.RoundRect(rect,14, Color::gray);

    View::doDraw(viewArea, clientArea);
    drawTeams(clientArea, 20);
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

    int cur_line_pos = rect.min.y +30 + ((20 - Surface::getFontHeight())/2);
    int Start_x = rect.min.x+40;
    int current_Team = 0;
    snprintf(statBuf, sizeof(statBuf), "Team %2i:", current_Team);
    dest.bltStringShadowed(Start_x, cur_line_pos, statBuf,Color::lightGreen, Color::gray64);
    cur_line_pos += 20;

    for(std::vector<const PlayerState*>::iterator i = states.begin();
            i != states.end(); ++i)
    {
        const PlayerState* state = *i;
        if (current_Team != state->getTeamID())
        {
            cur_line_pos += 30;
            current_Team = state->getTeamID();
            snprintf(statBuf, sizeof(statBuf), "Team %2i:", current_Team);
            dest.bltStringShadowed(Start_x, cur_line_pos, statBuf,Color::lightGreen, Color::gray64);
            cur_line_pos += 20;
        }

        snprintf(statBuf, sizeof(statBuf),
                 stats_format, state->getName().substr(0,20).c_str(),
                 state->getKills(), state->getLosses(), state->getTotal(),
                 state->getObjectivesHeld());

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
        loaded = false;
    }
}

void
GTeamView::checkResolution(iXY oldResolution, iXY newResolution)
{
    resize(iXY(newResolution.x, newResolution.y));
    moveTo(iXY(0,0));
}

void GTeamView::processEvents()
{
    COMMAND_PROCESSOR.process(false);
}
