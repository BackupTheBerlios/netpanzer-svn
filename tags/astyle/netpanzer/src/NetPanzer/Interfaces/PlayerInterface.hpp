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

#include "PlayerState.hpp"
#include "UnitInterface.hpp"
#include "NetMessageEncoder.hpp"

class PlayerInterface
{
private:
    static PlayerState local_player_state;

protected:
    static PlayerState *player_lists;
    static unsigned short max_players;
    static bool *alliance_matrix;

    static void setKill( unsigned short by_player_index, unsigned short on_player_index, unsigned short unit_type);

    static void resetAllianceMatrix( void );

    static unsigned short local_player_index;

public:

    static void initialize( unsigned short maxPlayers, unsigned char max_spawn_units );

    static void reset( void );

    static void cleanUp( void );

    static void setKill( const PlayerID &by_player, const PlayerID &on_player, unsigned short unit_type );

    static void setKill( const UnitID &by_player, const UnitID &on_player, unsigned short unit_type );

    static void setAlliance( const PlayerID& by_player, const PlayerID& with_player );

    static void setAlliance( unsigned short by_player, unsigned short with_player );

    static void clearAlliance( unsigned short by_player, unsigned short with_player );

    static bool isAllied( const PlayerID& player, const PlayerID& with_player );

    static bool isAllied( unsigned short player, unsigned short with_player );

    static void lockPlayerStats( void );
    static void unlockPlayerStats( void );

    static inline unsigned short getMaxPlayers( )
    {
        return( max_players );
    }

    static inline PlayerState * getPlayerState( const PlayerID& player )
    {
        assert( player.getIndex() < max_players );
        return( &player_lists[ player.getIndex() ] );
    }

    static inline PlayerState * getPlayerState( unsigned short player_index )
    {
        assert( player_index < max_players );
        return( &player_lists[ player_index ] );
    }

    static inline PlayerState * getLocalPlayerState( void )
    {
        if( local_player_index == 0xFFFF ) {
            return( &local_player_state);
        }

        return( &player_lists[ local_player_index ] );
    }

    static inline PlayerID getLocalPlayerID( void )
    {
        if( local_player_index == 0xFFFF ) {
            return( local_player_state.getPlayerID() );
        }

        return( player_lists[ local_player_index ].getPlayerID() );
    }

    static inline unsigned short getLocalPlayerIndex( void )
    {
        return( local_player_index );
    }

    static inline PlayerID getPlayerID( unsigned short player_index )
    {
        assert( player_index < max_players );
        return( player_lists[ player_index ].getPlayerID() );
    }

    static inline unsigned short getPlayerStatus( const PlayerID& player )
    {
        assert( player.getIndex() < max_players );
        return( player_lists[ player.getIndex() ].getStatus() );
    }

    static inline unsigned short getPlayerStatus( unsigned short player_index )
    {
        assert( player_index < max_players );
        return( player_lists[ player_index ].getStatus() );
    }

    static void resetPlayerStats( void );

    static int getActivePlayerCount( void );

    static PlayerState * allocateLoopBackPlayer( void );

    static PlayerState * allocateNewPlayer( void );

    static void spawnPlayer( unsigned short player_index, const iXY &location );
    static void spawnPlayer( const PlayerID &player, const iXY &location );

    static bool testRuleScoreLimit( long score_limit, PlayerState ** player_state );

    static bool testRuleObjectiveRatio( float precentage, PlayerState ** player_state );

protected:
    static unsigned short respawn_rule_player_index;
public:
    static bool testRulePlayerRespawn( bool *completed, PlayerState **player_state );

protected:
    static NetMessageEncoder message_encoder;
    static unsigned short player_sync_index;
    static unsigned short player_sync_connect_player_index;
    static PlayerID player_sync_connect_id;
    static Timer player_sync_timer;

    static void netMessageConnectID( NetMessage *message );
    static void netMessageSyncState( NetMessage *message );
    static void netMessageScoreUpdate( NetMessage *message );
    static void netMessageAllianceRequest( NetMessage *message );
    static void netMessageAllianceUpdate( NetMessage *message );

public:
    static void    startPlayerStateSync( const PlayerID &connect_player );
    static bool syncPlayerState( int *send_return_code, int *percent_complete );

    static void processNetMessage( NetMessage *message );

    static void disconnectPlayerCleanup( PlayerID &player_id );
};

#endif // ** _PLAYERINTERFACE_HPP
