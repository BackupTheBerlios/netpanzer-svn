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
#ifndef _PLAYERINTERFACE_HPP
#define _PLAYERINTERFACE_HPP

#include <SDL_thread.h>
#include "Core/CoreTypes.hpp"
#include "Classes/PlayerState.hpp"
#include "Units/UnitInterface.hpp"

class PlayerInterface
{
protected:
    static PlayerState *player_lists;
    static PlayerID max_players;
    static PlayerID local_player_index;

    static SDL_mutex* mutex;

public:

    static void initialize(const unsigned int _max_players);

    static void reset();

    static void cleanUp();

    static void lock();
    static void unLock();

    static void setKill(PlayerState* by_player, PlayerState* on_player,
            UnitType unit_type );

    static bool isAllied(PlayerID player, PlayerID with_player);
    //static bool isSingleAllied(unsigned short player, unsigned short with_player);

    static bool isPlayerActive(PlayerID player)
    {
        return (player < getMaxPlayers())
                ? player_lists[player].getStatus() == _player_state_active
                : false;
    }

    static void lockPlayerStats();
    static void unlockPlayerStats();

    static PlayerID getMaxPlayers( )
    {
        return max_players;
    }

    static PlayerState* getPlayer(PlayerID player_id)
    {
        if ( player_id < max_players )
        {
            return &player_lists[player_id];
        }
        return NULL;
    }

    static PlayerState* getLocalPlayer()
    {
        return &player_lists[ local_player_index ];
    }

    static PlayerID getLocalPlayerIndex()
    {
        return local_player_index;
    }

    static void resetPlayerStats();

    static int getActivePlayerCount();

    static PlayerState * allocateLoopBackPlayer();

    static PlayerState * allocateNewPlayer();
    static PlayerID countPlayers();

    static void spawnPlayer( PlayerID player_id, const iXY &location );

    static bool testRuleScoreLimit( long score_limit, PlayerState ** player_state );

    static bool testRuleObjectiveRatio( float precentage, PlayerState ** player_state );

    static bool testRulePlayerRespawn( bool *completed, PlayerState **player_state );

protected:
    static PlayerID respawn_rule_player_index;
    static void netMessageConnectID(const NetMessage *message );
    static void netMessageSyncState(const NetMessage *message );
    static void netMessageScoreUpdate(const NetMessage *message );
    //static void netMessageAllianceRequest(const NetMessage *message );
    //static void netMessageAllianceUpdate(const NetMessage *message );

public:
    static void processNetMessage(const NetMessage *message );
    static void disconnectPlayerCleanup( PlayerID player_id );
};

#endif // ** _PLAYERINTERFACE_HP