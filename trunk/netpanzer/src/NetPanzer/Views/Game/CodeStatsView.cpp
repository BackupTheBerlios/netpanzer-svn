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

#include "CodeStatsView.hpp"
#include "Views/GameViewGlobals.hpp"
#include "Classes/SpriteSorter.hpp"
#include "Classes/Network/NetworkState.hpp"
#include "Classes/AI/PathingState.hpp"
#include "Interfaces/PathScheduler.hpp"
#include "Interfaces/PlayerInterface.hpp"
#include "Units/UnitInterface.hpp"
#include "Views/Components/Button.hpp"

enum{ _display_mode_network_stats,
      _display_mode_pathing_stats,
      _display_mode_sorter_stats,
      _display_mode_unit_stats
    };

int gPacketSize = 0;
int display_mode;

enum
{
    BTN_NETWORK,
    BTN_SORTER,
    BTN_UNIT,
    BTN_PATHING,
    BTN_DEBUG,
    BTN_SAMPLE,
    BTN_NETLOG
};

static long INFO_AREA_Y_OFFSET = 60;

CodeStatsView::CodeStatsView() : GameTemplateView()
{
    setSearchName("CodeStatsView");
    setTitle("CodeStatsView");
    setSubTitle(" - F4");

    setAllowResize(false);
    setAllowMove(true);
    setDisplayStatusBar(true);
    setVisible(false);

    moveTo(iXY(0, 0));

    iXY  area_size = iXY(280, 280) ;
    long bXOffset;
    INFO_AREA_Y_OFFSET = 2;

    resize(area_size);

    bXOffset = area_size.x / 3;
    add( Button::createTextButton( "net", "Net", iXY(0, INFO_AREA_Y_OFFSET), bXOffset, BTN_NETWORK));
    add( Button::createTextButton( "sprite", "Sprite", iXY(bXOffset, INFO_AREA_Y_OFFSET), bXOffset, BTN_SORTER));
    add( Button::createTextButton( "unit", "Unit", iXY(bXOffset*2, INFO_AREA_Y_OFFSET), bXOffset, BTN_UNIT));

    INFO_AREA_Y_OFFSET += 18;
    add( Button::createTextButton( "path", "Path", iXY(0, INFO_AREA_Y_OFFSET), area_size.x, BTN_PATHING));

    INFO_AREA_Y_OFFSET += 18;
    bXOffset = area_size.x / 3;
    add( Button::createTextButton( "debug", "Debug", iXY(0, INFO_AREA_Y_OFFSET), bXOffset, BTN_DEBUG));
    add( Button::createTextButton( "sample", "Sample", iXY(bXOffset, INFO_AREA_Y_OFFSET), bXOffset, BTN_SAMPLE));
    add( Button::createTextButton( "netlog", "NetLog", iXY(bXOffset*2, INFO_AREA_Y_OFFSET), bXOffset, BTN_NETLOG));

    INFO_AREA_Y_OFFSET += 18;
    /*
       addButtonCenterText(iXY(0, INFO_AREA_Y_OFFSET), 20, "+", "", bPlusPacketSize );
    addButtonCenterText(iXY(25, INFO_AREA_Y_OFFSET), 20, "-", "", bMinusPacketSize );
    addButtonCenterText(iXY(50, INFO_AREA_Y_OFFSET), 20, "0", "", bMinusPacketSize );
    addButtonCenterText(iXY(65, INFO_AREA_Y_OFFSET), 20, "1", "", bMinusPacketSize );

    addButtonCenterText(iXY(45, INFO_AREA_Y_OFFSET), 40, "Send", "", bSend );

       INFO_AREA_Y_OFFSET += 18;
       INFO_AREA_Y_OFFSET += 18;
       */

    display_mode = _display_mode_network_stats;

} // end CodeStatsView::CodeStatsView

// doDraw
//---------------------------------------------------------------------------
void CodeStatsView::doDraw()
{
    drawViewBackground();

    switch( display_mode ) {
    case _display_mode_network_stats :
        drawNetworkStats();
        break;

    case _display_mode_sorter_stats :
        drawSorterStats();
        break;

    case _display_mode_pathing_stats :
        drawPathingStats();
        break;

    case _display_mode_unit_stats :
        drawUnitStats();
        break;
    }

    /*
    char strBuf[256];

    sprintf(strBuf, "Packet Size : %d", gPacketSize );

    clientArea.bltString(2, INFO_AREA_Y_OFFSET - 18, strBuf, Color::white);
    */
    View::doDraw();

} // end CodeStatsView::doDraw

//---------------------------------------------------------------------------
void CodeStatsView::drawNetworkStats()
{

    char strBuf[256];

    iXY str_loc(2, INFO_AREA_Y_OFFSET);

    sprintf(strBuf, "Packets" );

    drawString(str_loc.x, str_loc.y, strBuf, Color::white);

    str_loc.y += 12;

    sprintf(strBuf, "Sent %ld, %.4f 1/s, %.4f Avg",NetworkState::packets_sent,
            NetworkState::packets_sent_per_sec,
            ((float) NetworkState::packets_sent) / ((float) NetworkState::packets_sent_time) );
    drawString(str_loc.x, str_loc.y, strBuf, Color::white);

    str_loc.y += 12;

    sprintf(strBuf, "Recv %ld, %.4f 1/s, %.4f Avg",NetworkState::packets_received,
            NetworkState::packets_received_per_sec,
            ((float) NetworkState::packets_received) / ((float) NetworkState::packets_received_time) );
    drawString(str_loc.x, str_loc.y, strBuf, Color::white);

    str_loc.y += 12;

    sprintf(strBuf, "Bytes" );

    drawString(str_loc.x, str_loc.y, strBuf, Color::white);

    str_loc.y += 12;

    sprintf(strBuf, "Sent %ld, %.4f 1/s, %.4f Avg",NetworkState::bytes_sent,
            NetworkState::bytes_sent_per_sec,
            ((float) NetworkState::bytes_sent) / ((float) NetworkState::packets_sent_time) );
    drawString(str_loc.x, str_loc.y, strBuf, Color::white);

    str_loc.y += 12;

    sprintf(strBuf, "Recv %ld, %.4f 1/s, %.4f Avg",NetworkState::bytes_received,
            NetworkState::bytes_received_per_sec,
            ((float) NetworkState::bytes_received) / ((float) NetworkState::packets_received_time) );
    drawString(str_loc.x, str_loc.y, strBuf, Color::white);

    str_loc.y += 12;

    sprintf(strBuf, "Opcodes" );
    drawString(str_loc.x, str_loc.y, strBuf, Color::white);

    str_loc.y += 12;

    sprintf(strBuf, "Sent : %ld, %.4f 1/s ", NetworkState::opcodes_sent,
            NetworkState::opcodes_sent_per_sec );
    drawString(str_loc.x, str_loc.y, strBuf, Color::white);

    str_loc.y += 12;

    sprintf(strBuf, "Recv : %ld, %.4f 1/s", NetworkState::opcodes_received,
            NetworkState::opcodes_received_per_sec );
    drawString(str_loc.x, str_loc.y, strBuf, Color::white);

    str_loc.y += 12;

    if ( NetworkState::status == _network_state_client ) {
        sprintf(strBuf, "Ping Time: %.4f (ms)", NetworkState::ping_time );
    } else {
        sprintf(strBuf, "Ping Time: NA" );
    }

    drawString(str_loc.x, str_loc.y, strBuf, Color::white);
    str_loc.y += 12;
}

//---------------------------------------------------------------------------
void CodeStatsView::drawSorterStats()
{

    char strBuf[256];

    iXY str_loc(2, INFO_AREA_Y_OFFSET);

    sprintf(strBuf, "Sorter" );
    drawString(str_loc.x, str_loc.y, strBuf, Color::white);

    str_loc.y += 12;

    for ( unsigned long i = 0; i < _MAX_HEIGHT_LEVELS; i++ ) {
        sprintf(strBuf, "Level %lu : %lu  ", i, SPRITE_SORTER.getMaxSprites( i ) );
        drawString(str_loc.x, str_loc.y, strBuf, Color::white);
        str_loc.y += 12;
    }

}

//---------------------------------------------------------------------------
void CodeStatsView::drawPathingStats()
{

    char strBuf[256];

    iXY str_loc(2, INFO_AREA_Y_OFFSET);

    sprintf(strBuf, "Pathing" );
    drawString(str_loc.x, str_loc.y, strBuf, Color::white);

    str_loc.y += 12;

    sprintf(strBuf, "A* Time: %.6f s", PathingState::astar_gen_time );
    drawString(str_loc.x, str_loc.y, strBuf, Color::white);

    str_loc.y += 12;

    sprintf(strBuf, "A* Avg: %.6f s", PathingState::getAverageAstarPathTime() );
    drawString(str_loc.x, str_loc.y, strBuf, Color::white);

    str_loc.y += 12;

    sprintf(strBuf, "A* Total: %.6f s", PathingState::astar_gen_time_total );
    drawString(str_loc.x, str_loc.y, strBuf, Color::white);


    str_loc.y += 12;

    sprintf(strBuf, "Path Length: %ld ", PathingState::path_length );
    drawString(str_loc.x, str_loc.y, strBuf, Color::white);


    str_loc.y += 12;

    sprintf(strBuf, "Paths: %lu ", PathingState::path_gen_count );
    drawString(str_loc.x, str_loc.y, strBuf, Color::white);

    str_loc.y += 12;

    sprintf(strBuf, "Updates: %lu ", PathingState::update_gen_count );
    drawString(str_loc.x, str_loc.y, strBuf, Color::white);

    str_loc.y += 12;

    sprintf(strBuf, "Cache Hits: %lu ", PathingState::path_cache_hits );
    drawString(str_loc.x, str_loc.y, strBuf, Color::white);

    str_loc.y += 12;

    sprintf(strBuf, "Cache Misses: %lu ", PathingState::path_cache_misses );
    drawString(str_loc.x, str_loc.y, strBuf, Color::white);
}

//---------------------------------------------------------------------------
void CodeStatsView::drawUnitStats()
{

    char strBuf[256];
    int total = 0;
    int max_players;


    iXY str_loc(2, INFO_AREA_Y_OFFSET);

    sprintf(strBuf, "Units" );
    drawString(str_loc.x, str_loc.y, strBuf, Color::white);

    str_loc.y += 12;

    max_players = PlayerInterface::getMaxPlayers();

    for ( int i = 0; i < max_players; i++ ) {
        unsigned long units;
        units = UnitInterface::getUnitCount( i );
        total += units;

        sprintf(strBuf, "Player %d : %lu  ", i, units);
        drawString(str_loc.x, str_loc.y, strBuf, Color::white);
        str_loc.y += 12;
    }

    sprintf(strBuf, "UnitTotal : %d", total );
    drawString(str_loc.x, str_loc.y, strBuf, Color::white);

    str_loc.y += 12;

}

void
CodeStatsView::onComponentClicked(Component* c)
{
    switch ( c->getCustomCode() )
    {
        case BTN_NETWORK:
            display_mode = _display_mode_network_stats;
            break;

        case BTN_SORTER:
            display_mode = _display_mode_sorter_stats;
            break;

        case BTN_UNIT:
            display_mode = _display_mode_unit_stats;
            break;

        case BTN_PATHING:
            display_mode = _display_mode_pathing_stats;
            break;

        case BTN_DEBUG:
            static bool previous_flag = false;
            previous_flag = !previous_flag;
            PathScheduler::setLongPatherDebug(previous_flag);
            break;

        case BTN_SAMPLE:
            PathScheduler::sampleLongPather();
            break;

        case BTN_NETLOG:
            NetworkState::logNetworkStats();
            break;
    }
}
