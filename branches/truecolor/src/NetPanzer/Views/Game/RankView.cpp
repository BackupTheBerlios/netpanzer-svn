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


#include "Resources/ResourceManager.hpp"

#include <vector>
#include <algorithm>

#include "Util/Exception.hpp"
#include "RankView.hpp"
#include "Views/GameViewGlobals.hpp"
#include "Classes/ScreenSurface.hpp"
#include "Classes/WorldInputCmdProcessor.hpp"
#include "Classes/Network/PlayerNetMessage.hpp"
#include "Classes/Network/NetworkClient.hpp"
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

    moveTo(gameconfig->rankposition);
    resize(iXY(450 + 20, 200));
    checkArea(iXY(screen->getWidth(),screen->getHeight()));

    const unsigned MAX_ALLY_CHARS		=  6;
//    const unsigned MAX_FLAG_CHARS      =  5;
	const unsigned MAX_NAME_CHARS      = 20;
    const unsigned MAX_KILLS_CHARS     =  6;
    const unsigned MAX_LOSSES_CHARS    =  7;
    const unsigned MAX_POINTS_CHARS    =  7;

    // hardcoded for now
    int CHAR_XPIX = 8;

    unsigned xOffset = 0;
    unsigned yOffset = 16;
    add( new Label( xOffset, yOffset, "*Flag", Color::red, Color::gray64, true) );
    xOffset += MAX_ALLY_CHARS*CHAR_XPIX;
//    add( new Label( xOffset, yOffset, "Flag", Color::red, Color::gray64, true) );
//    xOffset += MAX_FLAG_CHARS*CHAR_XPIX;
    add( new Label( xOffset, yOffset, "Name", Color::red, Color::gray64, true) );
    xOffset += MAX_NAME_CHARS*CHAR_XPIX;
    add( new Label( xOffset, yOffset, "Kills", Color::red, Color::gray64, true) );
    xOffset += MAX_KILLS_CHARS*CHAR_XPIX;
    add( new Label( xOffset, yOffset, "Losses", Color::red, Color::gray64, true) );
    xOffset += MAX_LOSSES_CHARS*CHAR_XPIX;
    add( new Label( xOffset, yOffset, "Points", Color::red, Color::gray64, true) );
    xOffset += MAX_POINTS_CHARS*CHAR_XPIX;
    add( new Label( xOffset, yOffset, "Objective", Color::red, Color::gray64, true) );
    xOffset += MAX_POINTS_CHARS*CHAR_XPIX;

    // shall be in resource manager
    allyImage.loadPNG("pics/default/ally.png");
    allyRequestImage.loadPNG("pics/default/allyRequest.png");
    allyOtherImage.loadPNG("pics/default/allyOther.png");
    noAllyImage.loadPNG("pics/default/noAlly.png");

    states.clear();
} // end RankView::RankView

// doDraw
//---------------------------------------------------------------------------
void RankView::doDraw()
{
    // make sure the window is big enough for all players
    unsigned int CHAR_YPIX = Surface::getFontHeight();
    unsigned int flagHeight = ResourceManager::getFlag(0)->getHeight();
    unsigned int entryheight = std::max(CHAR_YPIX, flagHeight) + 2;
    unsigned int newheight = 60 + (entryheight * (PlayerInterface::countPlayers()-1));
    
    if ( newheight != (unsigned int)clientRect.getSizeY() ) {
        resize(iXY(450+20, newheight));
        return; // this frame draws nothing
    }
    
    drawViewBackground();

    drawButtonBorder(
            iRect(0, 26, clientRect.getSize().x - 1,
                clientRect.getSize().y - 1), Color::gray64, Color::white);

    drawPlayerStats( flagHeight);

    View::doDraw();
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
void RankView::drawPlayerStats( unsigned int flagHeight)
{
    char statBuf[256];

	states.clear();
	for(size_t i = 0; i < PlayerInterface::getMaxPlayers(); ++i)
	{
        PlayerState* state = PlayerInterface::getPlayer(i);
        if(state->getStatus() != _player_state_active)
        {
			continue;
		}
        states.push_back(state);
    }

    switch(gameconfig->gametype)
	{
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
    iXY offset( 6*8, 40);
    iXY flagOffset(26, 40 + (int(CHAR_YPIX - flagHeight))/2);
    Surface * flag = 0;

    for(std::vector<const PlayerState*>::iterator i = states.begin();
            i != states.end(); ++i) {
        const PlayerState* state = *i;

        snprintf(statBuf, sizeof(statBuf),
                "%-20s%5i%7i%7i%10i", state->getName().substr(0,20).c_str(),
                state->getKills(), state->getLosses(), state->getTotal(),
                state->getObjectivesHeld());
        drawStringShadowed(offset.x, offset.y, statBuf, Color::white, Color::gray64);
        
        flag = ResourceManager::getFlag(state->getFlag());
        drawImage( *flag, flagOffset.x, flagOffset.y );
		if ( state->getID() != PlayerInterface::getLocalPlayerIndex() )
		{
			bool meWithHim = PlayerInterface::isSingleAllied(PlayerInterface::getLocalPlayerIndex(), state->getID());
			bool himWithMe = PlayerInterface::isSingleAllied(state->getID(), PlayerInterface::getLocalPlayerIndex());
			if ( meWithHim && himWithMe )
			{
				drawImage( allyImage, 4, flagOffset.y );
			}
			else if ( meWithHim )
			{
				drawImage( allyRequestImage, 4, flagOffset.y );
			}
			else if ( himWithMe )
			{
				drawImage( allyOtherImage, 4, flagOffset.y );
			}
			else
			{
				drawImage( noAllyImage, 4, flagOffset.y );
			}
		}
		
        offset.y += entryHeight;
        flagOffset.y += entryHeight;        
    }

} // end RankView::drawPlayerStats

void RankView::notifyMoveTo()
{
    gameconfig->rankposition=min;
}

void
RankView::lMouseDown(const iXY &pos)
{
	GameTemplateView::lMouseDown(pos);

	if ( pos.x >= 4 && pos.x <= 24 && pos.y >= 40 )
	{
		unsigned int ypos = pos.y - 40;
		unsigned int CHAR_YPIX = Surface::getFontHeight();
		unsigned int flagHeight = ResourceManager::getFlag(0)->getHeight();
		unsigned int entryHeight = std::max(CHAR_YPIX, flagHeight) + 2;
		unsigned int linepos = ypos / entryHeight;
		if ( linepos < states.size() )
		{
			unsigned int destplayer = states[linepos]->getID();
			unsigned int localplayer = PlayerInterface::getLocalPlayerIndex();
			if ( destplayer != localplayer )
			{
				PlayerAllianceRequest allie_request;

				if ( PlayerInterface::isSingleAllied(localplayer, destplayer) )
				{
					allie_request.set( localplayer, destplayer, _player_break_alliance);
				}
				else
				{
					allie_request.set( localplayer, destplayer, _player_make_alliance);
				}
				
				NetworkClient::sendMessage( &allie_request, sizeof(PlayerAllianceRequest));
			}
		}
	}
}

void
RankView::processEvents()
{
    COMMAND_PROCESSOR.process(false);
}
