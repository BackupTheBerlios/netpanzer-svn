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
#include <config.h>

#include <vector>
#include <algorithm>

#include "Util/Exception.hpp"
#include "RankView.hpp"
#include "GameViewGlobals.hpp"
#include "ScreenSurface.hpp"
#include "PlayerInterface.hpp"
#include "GameConfig.hpp"
#include "ObjectiveInterface.hpp"

#include "SelectionBoxSprite.hpp"

// RankView
//---------------------------------------------------------------------------
RankView::RankView() : GameTemplateView()
{
    setSearchName("RankView");
    setTitle("Rankings");
    setSubTitle(" - TAB");

    setAllowResize(false);
    moveTo(gameconfig->rankposition);
    resize(iXY(450, 200));
    checkArea(iXY(screen->getWidth(),screen->getHeight()));

    const unsigned MAX_NAME_CHARS      = 20;
    const unsigned MAX_FLAG_CHARS      =  5;
    const unsigned MAX_KILLS_CHARS     =  6;
    const unsigned MAX_LOSSES_CHARS    =  7;
    const unsigned MAX_POINTS_CHARS    =  7;

    // hardcoded for now
    int CHAR_XPIX = 8;

    //addLabel(iXY nPos, char *nLabel, Uint8 color);
    unsigned xOffset = 0;
    unsigned yOffset = 16;
    addLabel(iXY(xOffset, yOffset), "Name", Color::red);
    xOffset += MAX_NAME_CHARS*CHAR_XPIX;
    addLabel(iXY(xOffset, yOffset), "Flag", Color::red);
    xOffset += MAX_FLAG_CHARS*CHAR_XPIX;
    addLabel(iXY(xOffset, yOffset), "Kills", Color::red);
    xOffset += MAX_KILLS_CHARS*CHAR_XPIX;
    addLabel(iXY(xOffset, yOffset), "Losses", Color::red);
    xOffset += MAX_LOSSES_CHARS*CHAR_XPIX;
    addLabel(iXY(xOffset, yOffset), "Points", Color::red);
    xOffset += MAX_POINTS_CHARS*CHAR_XPIX;
    addLabel(iXY(xOffset, yOffset), "Objective", Color::red);
    xOffset += MAX_POINTS_CHARS*CHAR_XPIX;

} // end RankView::RankView

// doDraw
//---------------------------------------------------------------------------
void RankView::doDraw(Surface &viewArea, Surface &clientArea)
{
    // make sure the window is big enough for all players
    unsigned int CHAR_YPIX = Surface::getFontHeight();
    unsigned int entryheight = std::max(CHAR_YPIX, UNIT_FLAGS_SURFACE.getHeight()) + 2;
    resize(iXY(450, 60 + entryheight * PlayerInterface::countPlayers()));
    
    bltViewBackground(viewArea);

    clientArea.drawRect(
            iRect(0, 26, getClientRect().getSize().x - 1,
                getClientRect().getSize().y - 1), Color::gray64);

    drawPlayerStats(clientArea);

    View::doDraw(viewArea, clientArea);
} // end doDraw

class StatesSortByFrags
    : public std::binary_function<const PlayerState*, const PlayerState*, bool>
{
public:
    bool operator() (const PlayerState* state1, const PlayerState* state2) {
        if(state1->getKills() > state2->getKills())
            return true;
        if(state1->getLosses() < state2->getLosses())
            return true;
        return false;
    }
};

class StatesSortByObjectives
    : public std::binary_function<const PlayerState*, const PlayerState*, bool>
{
public:
    bool operator() (const PlayerState* state1, const PlayerState* state2) {
        return state1->getObjectivesHeld() > state2->getObjectivesHeld();
    }
};


// drawPlayerStats
//---------------------------------------------------------------------------
// Purpose:
//---------------------------------------------------------------------------
void RankView::drawPlayerStats(Surface &dest)
{
    char statBuf[256];

    ObjectiveInterface::updatePlayerObjectiveCounts();

    std::vector<const PlayerState*> states;
    for(size_t i = 0; i < PlayerInterface::getMaxPlayers(); ++i) {
        PlayerState* state = PlayerInterface::getPlayerState(i);
        if(state->getStatus() != _player_state_active)
            continue;
        states.push_back(state);
    }

    switch(gameconfig->gametype) {
        case _gametype_objective:
            std::sort(states.begin(), states.end(), StatesSortByObjectives());
            break;
        case _gametype_timelimit:
        case _gametype_fraglimit:
            std::sort(states.begin(), states.end(), StatesSortByFrags());
            break;
    }

    unsigned int CHAR_YPIX = Surface::getFontHeight();
    unsigned int entryHeight = std::max(CHAR_YPIX, UNIT_FLAGS_SURFACE.getHeight()) + 2;
    iXY offset(2, 40);
    iXY flagOffset(162, 40 + (int(CHAR_YPIX - UNIT_FLAGS_SURFACE.getHeight()))/2);

    char shortname[21];
    for(std::vector<const PlayerState*>::iterator i = states.begin();
            i != states.end(); ++i) {
        const PlayerState* state = *i;
        
        SDL_strlcpy(shortname,state->getName().c_str(),sizeof(shortname));

        SDL_snprintf(statBuf, sizeof(statBuf),
                "%-20s%10i%7i%6i%10i", shortname,
                state->getKills(), state->getLosses(), state->getTotal(),
                state->getObjectivesHeld());
        dest.bltString(offset.x, offset.y, statBuf, state->getColor());
        UNIT_FLAGS_SURFACE.setFrame(state->getFlag());
        UNIT_FLAGS_SURFACE.blt( dest, flagOffset.x, flagOffset.y );

        offset.y += entryHeight;
        flagOffset.y += entryHeight;        
    }

} // end RankView::drawPlayerStats

void RankView::notifyMoveTo()
{
    gameconfig->rankposition=min;
}
