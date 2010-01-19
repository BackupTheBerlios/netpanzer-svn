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

#include "Resources/ResourceManager.hpp"

#include <vector>
#include <algorithm>

#include "Util/Exception.hpp"
#include "RankView.hpp"
#include "Views/GameViewGlobals.hpp"
#include "Classes/ScreenSurface.hpp"
#include "Interfaces/PlayerInterface.hpp"
#include "Interfaces/GameConfig.hpp"
#include "Objectives/ObjectiveInterface.hpp"

#include "Views/Components/Label.hpp"

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

    unsigned xOffset = 0;
    unsigned yOffset = 16;
    add( new Label( xOffset, yOffset, "Name", Color::red, Color::gray64, true) );
    xOffset += MAX_NAME_CHARS*CHAR_XPIX;
    add( new Label( xOffset, yOffset, "Flag", Color::red, Color::gray64, true) );
    xOffset += MAX_FLAG_CHARS*CHAR_XPIX;
    add( new Label( xOffset, yOffset, "Kills", Color::red, Color::gray64, true) );
    xOffset += MAX_KILLS_CHARS*CHAR_XPIX;
    add( new Label( xOffset, yOffset, "Losses", Color::red, Color::gray64, true) );
    xOffset += MAX_LOSSES_CHARS*CHAR_XPIX;
    add( new Label( xOffset, yOffset, "Points", Color::red, Color::gray64, true) );
    xOffset += MAX_POINTS_CHARS*CHAR_XPIX;
    add( new Label( xOffset, yOffset, "Objective", Color::red, Color::gray64, true) );
    xOffset += MAX_POINTS_CHARS*CHAR_XPIX;

    // Define the scrollBar fot this view.
    scrollBar = new ScrollBar(ScrollBar::VERTICAL, 0, 1, 0, 100);
    if (scrollBar == 0) {
        throw Exception("ERROR: Unable to allocate the scrollBar.");
    }
} // end RankView::RankView

// doDraw
//---------------------------------------------------------------------------
void RankView::doDraw(Surface &viewArea, Surface &clientArea)
{
    // make sure the window is big enough for all players
    unsigned int CHAR_YPIX = Surface::getFontHeight();
    unsigned int flagHeight = ResourceManager::getFlag(0)->getHeight();
    unsigned int entryheight = std::max(CHAR_YPIX, flagHeight) + 2;
    unsigned int newheight = 60 + entryheight * PlayerInterface::countPlayers();
    
    if ( newheight != (unsigned int)getSizeY() ) {
        resize(iXY(450, newheight));
        return; // this frame draws nothing
    }
    
    bltViewBackground(viewArea);

    clientArea.drawButtonBorder(
            iRect(0, 26, getClientRect().getSize().x - 1,
                getClientRect().getSize().y - 1), Color::gray64, Color::white);

    drawPlayerStats(clientArea, flagHeight);

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
void RankView::drawPlayerStats(Surface &dest, unsigned int flagHeight)
{
    char statBuf[256];

    std::vector<const PlayerState*> states;
    for(size_t i = 0; i < PlayerInterface::getMaxPlayers(); ++i) {
        PlayerState* state = PlayerInterface::getPlayer(i);
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
    unsigned int entryHeight = std::max(CHAR_YPIX, flagHeight) + 2;
    iXY offset(2, 40);
    iXY flagOffset(162, 40 + (int(CHAR_YPIX - flagHeight))/2);
    Surface * flag = 0;

    for(std::vector<const PlayerState*>::iterator i = states.begin();
            i != states.end(); ++i) {
        const PlayerState* state = *i;

        snprintf(statBuf, sizeof(statBuf),
                "%-20s%10i%7i%6i%10i", state->getName().substr(0,20).c_str(),
                state->getKills(), state->getLosses(), state->getTotal(),
                state->getObjectivesHeld());
        dest.bltStringShadowed(offset.x, offset.y, statBuf, state->getColor(), Color::gray64);
        
        flag = ResourceManager::getFlag(state->getFlag());
        flag->blt( dest, flagOffset.x, flagOffset.y );

        offset.y += entryHeight;
        flagOffset.y += entryHeight;        
    }

} // end RankView::drawPlayerStats

void RankView::notifyMoveTo()
{
    gameconfig->rankposition=min;
}
