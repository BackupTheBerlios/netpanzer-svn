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
#include "Stats.hpp"

#include "Exception.hpp"
#include "ObjectiveInterface.hpp"
#include "GameConfig.hpp"

int            Stats::sort_order_enum = _stats_sort_order_game_type;
char           Stats::PlayerName[64];
unsigned short Stats::MaxPlayers;
unsigned short Stats::Count;
unsigned short Stats::NumActivePlayers;
PArray         Stats::PlayerArray;
PlayerID       Stats::winner_player_id;
PlayerState    *Stats::winner_player_state;



int StatsNamesSortKey( const void *elem1, const void *elem2 )
{
    if (elem1 == elem2) return 0;

    PlayerState *player_state1 = *(PlayerState **) (elem1);
    PlayerState *player_state2 = *(PlayerState **) (elem2);

    int strcmp_result;

    strcmp_result = strcasecmp( player_state1->getName(), player_state2->getName() );

    if ( strcmp_result != 0 )
        return( strcmp_result );

    // Tie based on the real comparison basis - impose an arbitrary
    // but consistent sort order to break the tie.

    if (player_state1 > player_state2) return +1;
    if (player_state1 < player_state2) return -1;

    // We should never get here, unless the same sprite is
    // inserted in the list twice
    throw Exception("sprite_key called to compare the same sprite against itself!");
    return 0;
}

int StatsFragsSortKey( const void *elem1, const void *elem2 )
{
    if (elem1 == elem2) return 0;

    PlayerState *player_state1 = *(PlayerState **) (elem1);
    PlayerState *player_state2 = *(PlayerState **) (elem2);


    short kills1 = player_state1->getTotal();
    short kills2 = player_state2->getTotal();

    if ( kills1 > kills2 ) return -1;
    if ( kills1 < kills2 ) return +1;

    // Tie based on the real comparison basis - impose an arbitrary
    // but consistent sort order to break the tie.

    if (player_state1 > player_state2) return +1;
    if (player_state1 < player_state2) return -1;

    // We should never get here, unless the same sprite is
    // inserted in the list twice
    throw Exception("sprite_key called to compare the same sprite against itself!");
    return 0;
}

int StatsObjectivesSortKey( const void *elem1, const void *elem2 )
{
    if (elem1 == elem2) return 0;

    PlayerState *player_state1 = *(PlayerState **) (elem1);
    PlayerState *player_state2 = *(PlayerState **) (elem2);


    short objectives1 = player_state1->getObjectivesHeld();
    short objectives2 = player_state2->getObjectivesHeld();

    if ( objectives1 > objectives2 ) return -1;
    if ( objectives1 < objectives2 ) return +1;

    // Tie based on the real comparison basis - impose an arbitrary
    // but consistent sort order to break the tie.

    if (player_state1 > player_state2) return +1;
    if (player_state1 < player_state2) return -1;

    // We should never get here, unless the same sprite is
    // inserted in the list twice
    throw Exception("sprite_key called to compare the same sprite against itself!");
    return 0;
}



Stats::Stats()
{
}

//menu system calls this when player requests current scores.
//immediately after this returns the menu system calls GetPlayerStats()
//until a 0 is returned.
void Stats::Initialize()
{
    unsigned short index;
    PlayerID playerId;
    PlayerState *player;

    //I initialize Count because I have to have an index for accessing
    //player states via Vlad's code in the GetPlayerStat function below,
    //and I want that number to be separate from NumActivePlayers for
    //clarity.
    winner_player_state = 0;
    Count = 0;
    NumActivePlayers = 0;

    //get the maximum possible players--
    MaxPlayers = PlayerInterface::getMaxPlayers();

    //get the number of active players (a subset of max possible players)--
    for(index = 0; index < MaxPlayers; index ++) {
        playerId = PlayerInterface::getPlayerID( index);
        player = PlayerInterface::getPlayerState( playerId);
        if((player -> getStatus()) == (unsigned char)_player_state_active) NumActivePlayers++;
    }

    //initialize an array to hold all the active players--
    PlayerArray.initialize( NumActivePlayers);

    int add_index = 0;
    //build an array of pointers to player states--
    for(index = 0; index < MaxPlayers; index++) {
        playerId = PlayerInterface::getPlayerID( index);
        player = PlayerInterface::getPlayerState( playerId);
        if((player -> getStatus()) == (unsigned char)_player_state_active) {
            PlayerArray.add( player, add_index );
            add_index++;
        }
    }

    //sort player array here--
    switch( sort_order_enum ) {
    case _stats_sort_order_player_name : {
            PlayerArray.sort( PlayerArray.getSize(), StatsNamesSortKey );
        }
        break;

    case _stats_sort_order_game_type : {
            switch( gameconfig->gametype ) {
            case _gametype_objective :
                ObjectiveInterface::updatePlayerObjectiveCounts();
                PlayerArray.sort( PlayerArray.getSize(), StatsObjectivesSortKey );
                break;

            case _gametype_fraglimit :
                PlayerArray.sort( PlayerArray.getSize(), StatsFragsSortKey );
                break;

            case _gametype_timelimit :
                PlayerArray.sort( PlayerArray.getSize(), StatsFragsSortKey );
                break;
            } // ** switch
        }
        break;

    case _stats_sort_order_winner : {
            switch( gameconfig->gametype ) {
            case _gametype_objective :
                ObjectiveInterface::updatePlayerObjectiveCounts();
                PlayerArray.sort( PlayerArray.getSize(), StatsObjectivesSortKey );
                break;

            case _gametype_fraglimit :
                PlayerArray.sort( PlayerArray.getSize(), StatsFragsSortKey );
                break;

            case _gametype_timelimit :
                PlayerArray.sort( PlayerArray.getSize(), StatsFragsSortKey );
                break;
            } // ** switch

            winner_player_state = (PlayerState *) PlayerArray[0];
            winner_player_id = winner_player_state->getPlayerID();

        }
        break;

    } // ** switch
}
//////////////////////////////////
//////////////////////////////////

void Stats::SortPlayers()
{


}
//////////////////////////////////
//////////////////////////////////


//this function returns 1 if there are still players
//to process, a 0 if all data in PArray has been returned.
//menu system should call this function until the 0 gets
//returned.
char Stats::GetPlayerStats(char  *flag,
                           short *kills,
                           short *losses,
                           short *total,
                           short *objectives,
                           char  **name,
                           int   *stats_display_type )
{
    PlayerID local_player_id;
    PlayerID player_id;

    PlayerState *player;

    if(NumActivePlayers) {
        local_player_id = PlayerInterface::getLocalPlayerID();

        player = (PlayerState *) PlayerArray[Count];
        player_id = player->getPlayerID();

        if( flag != 0 ) {
            *flag = player->getFlag();
        }

        if( kills != 0 ) {
            *kills = player->getKills();
        }

        if ( losses != 0 ) {
            *losses = player->getLosses();
        }

        if ( total != 0 ) {
            *total =  player->getTotal();
        }

        if ( objectives != 0 ) {
            *objectives = player->getObjectivesHeld();
        }

        if ( name != 0 ) {
            *name = player->getName();
        }

        if ( stats_display_type != 0 ) {
            if ( (sort_order_enum == _stats_sort_order_player_name) ||
                    (sort_order_enum == _stats_sort_order_game_type)
               ) {
                if( player_id == local_player_id ) {
                    *stats_display_type = _stats_display_type_local_player;
                } else
                    if( PlayerInterface::isAllied( player_id, local_player_id ) &&
                            PlayerInterface::isAllied( local_player_id, player_id )
                      ) {
                        *stats_display_type = _stats_display_type_ally;
                    } else {
                        *stats_display_type = _stats_display_type_default;
                    }
            } else
                if( sort_order_enum == _stats_sort_order_winner ) {
                    if( player_id == winner_player_id ) {
                        *stats_display_type = _stats_display_type_winner;
                    } else
                        if( PlayerInterface::isAllied( player_id, winner_player_id ) &&
                                PlayerInterface::isAllied( winner_player_id, player_id )
                          ) {
                            *stats_display_type = _stats_display_type_winner_allie;
                        } else {
                            *stats_display_type = _stats_display_type_default;
                        }
                }
        } // ** if ( stats_display_type != 0 )

        //have to increment count so that next time through
        //i'll get the next player.
        Count++;
        NumActivePlayers--;
        return (1);

    } else return (0);


}
//////////////////////////////////
//////////////////////////////////
