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
#include "Classes/PlayerState.hpp"
#include "Interfaces/UnitInterface.hpp"

class PlayerInterface
{
protected:
    static PlayerState *player_lists;
    static unsigned short max_players;
    static bool *alliance_matrix;

    static void resetAllianceMatrix();

    static unsigned short local_player_index;

    static SDL_mutex* mutex;

public:

    static void initialize(unsigned short maxPlayers);

    static void reset();

    static void cleanUp();

    static void lock();
    static void unLock();

    static void setKill(PlayerState* by_player, PlayerState* on_player,
            UnitType unit_type );

    static void setAlliance( const PlayerID& by_player, const PlayerID& with_player );

    static void setAlliance( unsigned short by_player, unsigned short with_player );

    static void clearAlliance( unsigned short by_player, unsigned short with_player );

    static bool isAllied(const PlayerID& player, const PlayerID& with_player);
    static bool isAllied(unsigned short player, unsigned short with_player);

    static void lockPlayerStats();
    static void unlockPlayerStats();

    static unsigned short getMaxPlayers( )
    {
        return max_players;
    }

    static PlayerState* getPlayerState(const PlayerID& player)
    {
        assert(player.getIndex() < max_players);
        return &player_lists[ player.getIndex() ];
    }

    static PlayerState* getPlayer(Uint16 player_index)
    {
        if ( player_index < max_players )
        {
            return &player_lists[player_index];
        }
        return NULL;
    }

    static PlayerState* getPlayerByNetworkID(NetClientID id);

    static PlayerState* getLocalPlayer()
    {
        return &player_lists[ local_player_index ];
    }

    static PlayerID getLocalPlayerID()
    {
        return player_lists[ local_player_index ].getPlayerID();
    }

    static Uint16 getLocalPlayerIndex()
    {
        return local_player_index;
    }

    static PlayerID getPlayerID(Uint16 player_index )
    {
        assert( player_index < max_players );
        return player_lists[ player_index ].getPlayerID();
    }

    static void resetPlayerStats();

    static int getActivePlayerCount();

    static PlayerState * allocateLoopBackPlayer();

    static PlayerState * allocateNewPlayer();
    static int countPlayers();

    static void spawnPlayer( Uint16 player_index, const iXY &location );

    static bool testRuleScoreLimit( long score_limit, PlayerState ** player_state );

    static bool testRuleObjectiveRatio( float precentage, PlayerState ** player_state );

protected:
    static unsigned short respawn_rule_player_index;
public:
    static bool testRulePlayerRespawn( bool *completed, PlayerState **player_state );

protected:
    static Uint16 player_sync_index;
    static Uint16 player_sync_connect_player_index;
    static Timer player_sync_timer;

    static void netMessageConnectID(const NetMessage *message );
    static void netMessageSyncState(const NetMessage *message );
    static void netMessageScoreUpdate(const NetMessage *message );
    static void netMessageAllianceRequest(const NetMessage *message );
    static void netMessageAllianceUpdate(const NetMessage *message );

public:
    static void startPlayerStateSync(Uint16 player_index);
    static bool syncNextPlayerState( NetworkPlayerState &dest, int *percent_complete);
    static void processNetMessage(const NetMessage *message );
    static void disconnectPlayerCleanup( Uint16 index );
};

#endif // ** _PLAYERINTERFACE_HP
