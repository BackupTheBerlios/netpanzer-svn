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
#ifndef __STATS_HPP
#define __STATS_HPP

#include <vector>

#include "PArray.hpp"
#include "PlayerInterface.hpp"

//the way this works: menu system makes call to Initialize, then it
//calls GetPlayerStats() immediately thereafter. Menu system keeps
//calling GetPlayerStats() until it returns 0, which means all active
//players have been processed.

enum { _stats_sort_order_player_name,
       _stats_sort_order_game_type,
       _stats_sort_order_winner
     };

enum { _stats_display_type_winner,
       _stats_display_type_winner_allie,
       _stats_display_type_local_player,
       _stats_display_type_ally,
       _stats_display_type_default
     };

class Stats
{
protected:
    static int sort_order_enum;

    static char PlayerName[64];

    static unsigned short MaxPlayers;
    static unsigned short Count;
    static unsigned short NumActivePlayers;

    static std::vector<PlayerState*> PlayerArray;

    static PlayerID winner_player_id;
    static PlayerState *winner_player_state;

public:

    Stats();

    static void Initialize();

    static void setSortOrder( int sort_order_enum )
    {
        Stats::sort_order_enum = sort_order_enum;
    }

    static unsigned short getMaxPlayers( void )
    {
        return ( MaxPlayers );
    }

    static unsigned short getActivePlayers( void )
    {
        return ( NumActivePlayers );
    }

    static const char * getLocalPlayerName( void )
    {
        return ( PlayerName );
    }

    static char GetPlayerStats(char  *flag,
                               short *kills,
                               short *losses,
                               short *total,
                               short *objectives,
                               const char** name,
                               int   *stats_display_type );


};

#endif
