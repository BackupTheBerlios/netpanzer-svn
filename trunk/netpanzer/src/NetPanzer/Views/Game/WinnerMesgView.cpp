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
#include "WinnerMesgView.hpp"
#include "GameViewGlobals.hpp"
#include "Desktop.hpp"
#include "Stats.hpp"
#include "Sound.hpp"
#include "DDHardSurface.hpp"
#include "ParticleSystem2D.hpp"
#include "Particle2D.hpp"
#include "GameManager.hpp"
#include "MenuTemplateView.hpp"
#include "OptionsTemplateView.hpp"
#include "SoundView.hpp"
#include "ControlsView.hpp"
#include "VisualsView.hpp"
#include "InterfaceView.hpp"
#include "ProgressView.hpp"

#include "GameConfig.hpp"

static void bReturnToMainMenu()
{
 if ( GameConfig::getMapCycleState() == true )
  {	
   progressView.open();
   progressView.scrollAndUpdate( "Waiting for next game map ..." );
  }
 else
  {  
    sound->stopTankIdle();

	// Blank the screen.

	GameManager::drawTextCenteredOnScreen("Loading Main View...", Color::white);

	ParticleSystem2D::removeAll();
	Particle2D::removeAll();
	sprintf(MenuTemplateView::currentMultiView, "GetConnectionTypeView");

	// put all code in here for shutdown.
	//----------------------
    GameManager::quitNetPanzerGame();
	//----------------------

	// Swap to the menu resolution.
	GameManager::setVideoMode(iXY(640, 480), false);

	GameManager::drawTextCenteredOnScreen("Loading Main View...", Color::white);

	GameManager::loadPalette("wads/netpmenu.act");

	Desktop::setVisibilityAllWindows(false);

	View *v = Desktop::getView("OptionsView");
	if (v != 0)
	{
		v->removeAllButtons();
		((OptionsTemplateView *)v)->initButtons();
		((MenuTemplateView *)v)->initButtons();
	} else
	{
		assert(false);
	}

	v = Desktop::getView("SoundView");
	if (v != 0)
	{
		v->removeAllButtons();
		((OptionsTemplateView *)v)->initButtons();
		((MenuTemplateView *)v)->initButtons();
		((SoundView *)v)->initButtons();
	} else
	{
		assert(false);
	}

	v = Desktop::getView("ControlsView");
	if (v != 0)
	{
		v->removeAllButtons();
		((OptionsTemplateView *)v)->initButtons();
		((MenuTemplateView *)v)->initButtons();
		((ControlsView *)v)->initButtons();
	} else
	{
		assert(false);
	}

	v = Desktop::getView("VisualsView");
	if (v != 0)
	{
		v->removeAllButtons();
		((OptionsTemplateView *)v)->initButtons();
		((MenuTemplateView *)v)->initButtons();
		((VisualsView *)v)->initButtons();
	} else
	{
		assert(false);
	}

	v = Desktop::getView("InterfaceView");
	if (v != 0)
	{
		v->removeAllButtons();
		((OptionsTemplateView *)v)->initButtons();
		((MenuTemplateView *)v)->initButtons();
		((InterfaceView *)v)->initButtons();
	} else
	{
		assert(false);
	}

	Desktop::setVisibilityAllWindows(false);
	Desktop::setVisibility("MainView", true);
  }
}



char WinnerMesgView::player_name[64];
int WinnerMesgView::player_index = 0;


// WinnerMesgView
//--------------------------------------------------------------------------
WinnerMesgView::WinnerMesgView() : GameTemplateView()
{
	setSearchName("WinnerMesgView");
	setTitle("Session Over");
	setSubTitle("");

	setAllowResize(false);
	setAllowMove(false);
	setDisplayStatusBar(false);
	setVisible(false);

	moveTo(iXY(0, 0));
	resize(640, 480);

	int buttonSizeX = 170;
 	int x           = (getClientRect().getSize().x - buttonSizeX) / 2;
    int y           = getClientRect().getSize().y - 15;

	addButtonCenterText(iXY(x, y), buttonSizeX, "OK", "", bReturnToMainMenu);

	const unsigned MAX_NAME_CHARS   = 30;
    const unsigned MAX_FLAG_CHARS   =  8;
	const unsigned MAX_KILLS_CHARS  =  9;
	const unsigned MAX_LOSSES_CHARS =  10;
	const unsigned MAX_POINTS_CHARS =  10;
	const unsigned MAX_OBJECTIVE_CHARS = 13;

	x =  2;
	y = 12;
	addLabel(iXY(x, y), "Name", Color::red);
	x += MAX_NAME_CHARS * CHAR_XPIX;
	addLabel(iXY(x, y), "Flag", Color::red);
	x += MAX_FLAG_CHARS * CHAR_XPIX;
	addLabel(iXY(x, y), "Kills", Color::red);
	x += MAX_KILLS_CHARS * CHAR_XPIX;
	addLabel(iXY(x, y), "Losses", Color::red);
	x += MAX_LOSSES_CHARS * CHAR_XPIX;
	addLabel(iXY(x, y), "Points", Color::red);
	x += MAX_POINTS_CHARS *CHAR_XPIX;
	addLabel(iXY(x, y), "Objective", Color::red);
	x += MAX_OBJECTIVE_CHARS *CHAR_XPIX;

    strcpy(player_name, "");

} // end cMultiplayerGetSessionView::WinnerMesgView

void WinnerMesgView::setWinnerInfoHack( char *name, int player_index )
{
	strcpy( player_name, name );
	WinnerMesgView::player_index = player_index; 
} 

// doDraw
//---------------------------------------------------------------------------
void WinnerMesgView::doDraw(const Surface &viewArea, const Surface &clientArea)
{
	centerAbsolute();

	bltViewBackground(viewArea);

	char strBuf[256];

	Stats::setSortOrder( _stats_sort_order_winner );
    Stats::Initialize();

	char   playerFlagName[256];
	short  playerKills;
	short  playerLosses;
	short  playerTotalPoints;
    short  playerObjectives;
	char  *playerName;
    int   playerStatsDisplayType;
	int    numPlayers = Stats::getActivePlayers();
	iXY    offset;
	offset.zero();

	const int ITEM_GAP_SPACE = 2;

	const unsigned MAX_NAME_CHARS   = 30;
    const unsigned MAX_FLAG_CHARS   =  8;
	const unsigned MAX_KILLS_CHARS  =  9;
	const unsigned MAX_LOSSES_CHARS =  10;
	const unsigned MAX_POINTS_CHARS =  10;
	const unsigned MAX_OBJECTIVE_CHARS = 13;

    for (int i = 0; i < numPlayers; i++)
	{
		offset.x = 2;
		offset.y = 32 + i * (CHAR_YPIX + ITEM_GAP_SPACE / 2);

		Stats::GetPlayerStats(playerFlagName, 
                              &playerKills, 
                              &playerLosses, 
                              &playerTotalPoints, 
                              &playerObjectives, 
                              &playerName,
                              &playerStatsDisplayType);

		PIX color;
         
         switch( playerStatsDisplayType )
          {
           case _stats_display_type_winner :
            color = Color::yellow;
           break;

           case _stats_display_type_winner_allie :
            color = Color::orange;
           break;

           case _stats_display_type_local_player :
            color = Color::blue;
           break;

           case _stats_display_type_ally :
            color = Color::orange;
           break;

           case _stats_display_type_default :
            color = Color::white;
           break;
          } // ** switch

        
        // Add player flag name.
        sprintf(strBuf, "%-30s", playerName);
		clientArea.bltString(offset.x, offset.y, strBuf, color);

        // Kills
		offset.x += (MAX_NAME_CHARS + MAX_FLAG_CHARS) * CHAR_XPIX;
		sprintf(strBuf, "%5i", playerKills);
		clientArea.bltString(offset.x, offset.y, strBuf, color);

        // Losses
		offset.x += MAX_KILLS_CHARS * CHAR_XPIX;
		sprintf(strBuf, "%6i", playerLosses);
		clientArea.bltString(offset.x, offset.y, strBuf, color);

        // Total Points
		offset.x += MAX_LOSSES_CHARS * CHAR_XPIX;
		sprintf(strBuf, "%6i", playerTotalPoints);
		clientArea.bltString(offset.x, offset.y, strBuf, color);

        // Objectives
		offset.x += MAX_POINTS_CHARS * CHAR_XPIX;
		sprintf(strBuf, "%9i", playerObjectives);
		clientArea.bltString(offset.x, offset.y, strBuf, color);
	}

	View::doDraw(viewArea, clientArea);

} // end doDraw
