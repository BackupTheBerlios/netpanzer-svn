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

// old version?!?

#if 0
#include <config.h>
#include "CodeStatsView.hpp"

#include "cViewManager.hpp"

#include "SpriteSorter.hpp"
#include "NetworkState.hpp"
#include "PathingState.hpp"
#include "PathScheduler.hpp"
#include "MiniMapInterface.hpp"
#include "PlayerInterface.hpp"
#include "UnitInterface.hpp"

enum{ _display_mode_network_stats,
      _display_mode_pathing_stats,
      _display_mode_sorter_stats,
      _display_mode_unit_stats
    };

int display_mode;

static void buttonNetwork( void )
{
    display_mode = _display_mode_network_stats;

}

static void buttonSorter( void )
{
    display_mode = _display_mode_sorter_stats;
}

static void buttonPathing( void )
{
    display_mode = _display_mode_pathing_stats;
}

static void buttonDebug( void )
{
    static bool previous_flag = false;

    if ( previous_flag == false ) {
        PathScheduler::setLongPatherDebug( true );
        MiniMapInterface::setPathingDebugMode( true );
        previous_flag = true;
    } else {
        PathScheduler::setLongPatherDebug( false );
        MiniMapInterface::setPathingDebugMode( false );
        previous_flag = false;
    }
}

static void buttonSample( void )
{
    PathScheduler::sampleLongPather( );
}

static void buttonUnit( void )
{
    display_mode = _display_mode_unit_stats;
}

#define _INFO_AREA_Y_OFFSET	60

CodeStatsView::CodeStatsView() : cView()
{
    setTitle("CodeStatsView");
    setAllowResize(false);
    setAllowMove(true);
    setDisplayStatusBar(true);
    setVisible(false);

    moveTo(iXY(0, 0));
    resizeClientArea(iXY(200, 400));

    addButtonCenterText(iXY(2, 2), 64,  "Net", "", buttonNetwork);
    addButtonCenterText(iXY(67, 2), 64, "Sprite", "", buttonSorter);
    addButtonCenterText(iXY(132, 2), 64, "Unit", "", buttonUnit);
    addButtonCenterText(iXY(2, 22), 198, "Path", "", buttonPathing);
    addButtonCenterText(iXY(2, 40), 96,  "Debug", "", buttonDebug);
    addButtonCenterText(iXY(100, 40), 96, "Sample", "", buttonSample);

    display_mode = _display_mode_network_stats;

} // end CodeStatsView::CodeStatsView

// doDraw
//---------------------------------------------------------------------------
void CodeStatsView::doDraw(const Surface &viewArea, const Surface &clientArea)
{
    viewArea.fill(Color::black);

    switch( display_mode ) {
    case _display_mode_network_stats :
        drawNetworkStats( clientArea );
        break;

    case _display_mode_sorter_stats :
        drawSorterStats( clientArea );
        break;

    case _display_mode_pathing_stats :
        drawPathingStats( clientArea );
        break;

    case _display_mode_unit_stats :
        drawUnitStats( clientArea );
        break;
    }


    cView::doDraw(viewArea, clientArea);
} // end CodeStatsView::doDraw

void CodeStatsView::drawNetworkStats( const Surface &clientArea)
{
    char strBuf[256];

    iXY str_loc(2, _INFO_AREA_Y_OFFSET);

    sprintf(strBuf, "Packets" );

    clientArea.bltString(str_loc.x, str_loc.y, strBuf, Color::white);

    str_loc.y += 12;

    sprintf(strBuf, "Sent : %d, %.4f 1/s ", NetworkState::packets_sent,
            NetworkState::packets_sent_per_sec );
    clientArea.bltString(str_loc.x, str_loc.y, strBuf, Color::white);

    str_loc.y += 12;

    sprintf(strBuf, "Recv : %d, %.4f 1/s", NetworkState::packets_received,
            NetworkState::packets_received_per_sec );
    clientArea.bltString(str_loc.x, str_loc.y, strBuf, Color::white);

    str_loc.y += 12;

    sprintf(strBuf, "Opcodes" );
    clientArea.bltString(str_loc.x, str_loc.y, strBuf, Color::white);

    str_loc.y += 12;

    sprintf(strBuf, "Sent : %d, %.4f 1/s ", NetworkState::opcodes_sent,
            NetworkState::opcodes_sent_per_sec );
    clientArea.bltString(str_loc.x, str_loc.y, strBuf, Color::white);

    str_loc.y += 12;

    sprintf(strBuf, "Recv : %d, %.4f 1/s", NetworkState::opcodes_received,
            NetworkState::opcodes_received_per_sec );
    clientArea.bltString(str_loc.x, str_loc.y, strBuf, Color::white);

    str_loc.y += 12;

    if ( NetworkState::status == _network_state_client ) {
        sprintf(strBuf, "Ping Time: %.4f (ms)", NetworkState::ping_time );
    } else {
        sprintf(strBuf, "Ping Time: NA" );
    }

    clientArea.bltString(str_loc.x, str_loc.y, strBuf, Color::white);
}

void CodeStatsView::drawSorterStats( const Surface &clientArea)
{
    char strBuf[256];

    iXY str_loc(2, _INFO_AREA_Y_OFFSET);

    sprintf(strBuf, "Sorter" );
    clientArea.bltString(str_loc.x, str_loc.y, strBuf, Color::white);

    str_loc.y += 12;

    for ( unsigned long i = 0; i < _MAX_HEIGHT_LEVELS; i++ ) {
        sprintf(strBuf, "Level %d : %d  ", i, SPRITE_SORTER.getMaxSprites( i ) );
        clientArea.bltString(str_loc.x, str_loc.y, strBuf, Color::white);
        str_loc.y += 12;
    }

}

void CodeStatsView::drawPathingStats( const Surface &clientArea)
{
    char strBuf[256];

    iXY str_loc(2, _INFO_AREA_Y_OFFSET);

    sprintf(strBuf, "Pathing" );
    clientArea.bltString(str_loc.x, str_loc.y, strBuf, Color::white);

    str_loc.y += 12;

    sprintf(strBuf, "A* Time: %.6f s", PathingState::astar_gen_time );
    clientArea.bltString(str_loc.x, str_loc.y, strBuf, Color::white);

    str_loc.y += 12;

    sprintf(strBuf, "A* Avg: %.6f s", PathingState::getAverageAstarPathTime() );
    clientArea.bltString(str_loc.x, str_loc.y, strBuf, Color::white);

    str_loc.y += 12;

    sprintf(strBuf, "A* Total: %.6f s", PathingState::astar_gen_time_total );
    clientArea.bltString(str_loc.x, str_loc.y, strBuf, Color::white);


    str_loc.y += 12;

    sprintf(strBuf, "Path Length: %d ", PathingState::path_length );
    clientArea.bltString(str_loc.x, str_loc.y, strBuf, Color::white);


    str_loc.y += 12;

    sprintf(strBuf, "Paths: %d ", PathingState::path_gen_count );
    clientArea.bltString(str_loc.x, str_loc.y, strBuf, Color::white);

    str_loc.y += 12;

    sprintf(strBuf, "Updates: %d ", PathingState::update_gen_count );
    clientArea.bltString(str_loc.x, str_loc.y, strBuf, Color::white);

    str_loc.y += 12;

    sprintf(strBuf, "Cache Hits: %d ", PathingState::path_cache_hits );
    clientArea.bltString(str_loc.x, str_loc.y, strBuf, Color::white);

    str_loc.y += 12;

    sprintf(strBuf, "Cache Misses: %d ", PathingState::path_cache_misses );
    clientArea.bltString(str_loc.x, str_loc.y, strBuf, Color::white);

}

void CodeStatsView::drawUnitStats( const Surface &clientArea )
{
    char strBuf[256];
    int total = 0;
    int max_players;


    iXY str_loc(2, _INFO_AREA_Y_OFFSET);

    sprintf(strBuf, "Units" );
    clientArea.bltString(str_loc.x, str_loc.y, strBuf, Color::white);

    str_loc.y += 12;

    max_players = PlayerInterface::getMaxPlayers();

    for ( int i = 0; i < max_players; i++ ) {
        unsigned long units;
        units = UnitInterface::getUnitCount( i );
        total += units;

        sprintf(strBuf, "Player %d : %d  ", i, units);
        clientArea.bltString(str_loc.x, str_loc.y, strBuf, Color::white);
        str_loc.y += 12;
    }

    sprintf(strBuf, "UnitTotal : %d", total );
    clientArea.bltString(str_loc.x, str_loc.y, strBuf, Color::white);

    str_loc.y += 12;
}

#endif
