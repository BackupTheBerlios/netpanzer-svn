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
#include <stdexcept>
#include "PlayerInterface.hpp"

#include "UnitInterface.hpp"
#include "ObjectiveInterface.hpp"

#include "PlayerNetMessage.hpp"
#include "Server.hpp"

#include "ConsoleInterface.hpp"

//#include "WorldViewInterface.hpp"

// ** PlayerInterface Statics
PlayerState PlayerInterface::local_player_state;

PlayerState   *PlayerInterface::player_lists = 0;
unsigned short PlayerInterface::max_players = 0;
bool *      PlayerInterface::alliance_matrix = 0;
unsigned short PlayerInterface::local_player_index = 0xFFFF;

NetMessageEncoder PlayerInterface::message_encoder;
unsigned short PlayerInterface::player_sync_index;
unsigned short PlayerInterface::player_sync_connect_player_index;
PlayerID PlayerInterface::player_sync_connect_id;
Timer PlayerInterface::player_sync_timer;

unsigned short PlayerInterface::respawn_rule_player_index = 0;

SDL_mutex* PlayerInterface::mutex = 0;

void PlayerInterface::initialize( unsigned short maxPlayers, unsigned char max_spawn_units )
{
    char temp_str[64];
    PlayerID player_id;
    unsigned long player_index;
    max_players = maxPlayers;

    delete[] player_lists;
    player_lists = new PlayerState[max_players];

    for ( player_index = 0; player_index < max_players; player_index++ ) {
        player_id.setIndex( player_index );
        player_lists[player_index].setID( player_id );
        player_lists[ player_index ].resetStats();
        player_lists[ player_index ].setStatus( _player_state_free );
        player_lists[ player_index ].setFlag( 0 );
        player_lists[ player_index ].unit_config.initialize( max_spawn_units );
        sprintf( temp_str, "Player %lu", player_index );
        player_lists[ player_index ].setName( temp_str );
    }

    local_player_state.setID( PlayerID( 0xFFFF, 0xFFFFFFFF ) );
    local_player_state.setStatus( _player_state_allocated );

    delete[] alliance_matrix;
    alliance_matrix = new bool [max_players * max_players];
    resetAllianceMatrix();

    mutex = SDL_CreateMutex();
    if(!mutex) {
        throw std::runtime_error("Couldn't create PlayerInterface mutex.");
    }
}

void PlayerInterface::reset()
{
    resetPlayerStats();
    resetAllianceMatrix();
}

void PlayerInterface::cleanUp()
{
    delete[] player_lists;
    player_lists = 0;
    delete[] alliance_matrix;
    alliance_matrix = 0;
    max_players = 0;

    SDL_DestroyMutex(mutex);
    mutex = 0;
}

void PlayerInterface::lock()
{
    SDL_mutexP(mutex);
}

void PlayerInterface::unLock()
{
    SDL_mutexV(mutex);
}

void PlayerInterface::setKill( unsigned short by_player_index, unsigned short on_player_index, unsigned short unit_type)
{
    assert( (by_player_index < max_players) && (on_player_index < max_players) );
    
    SDL_mutexP(mutex);
    player_lists[ by_player_index ].incKills( unit_type );
    player_lists[ on_player_index ].incLosses( unit_type );
    SDL_mutexV(mutex);
}

void PlayerInterface::setKill( const PlayerID &by_player, const PlayerID &on_player, unsigned short unit_type )
{
    assert( (by_player.getIndex() < max_players) && (on_player.getIndex() < max_players) );

    SDL_mutexP(mutex);
    player_lists[ by_player.getIndex() ].incKills( unit_type );
    player_lists[ on_player.getIndex() ].incLosses( unit_type );
    SDL_mutexV(mutex);
}

void PlayerInterface::setKill( const UnitID &by_player, const UnitID &on_player, unsigned short unit_type )
{
    unsigned short by_player_index;
    unsigned short on_player_index;

    SDL_mutexP(mutex);
    by_player_index = by_player.getPlayer();
    on_player_index = on_player.getPlayer();

    assert( (by_player_index < max_players) && (on_player_index < max_players) );

    player_lists[ by_player_index ].incKills(unit_type);
    player_lists[ on_player_index ].incLosses(unit_type);
    SDL_mutexV(mutex);
}


void PlayerInterface::setAlliance( const PlayerID& by_player, const PlayerID& with_player )
{
    assert( (by_player.getIndex() < max_players) && (with_player.getIndex() < max_players) );

    SDL_mutexP(mutex);
    *(alliance_matrix + (with_player.getIndex() * max_players) + by_player.getIndex() ) = true;
    //*(alliance_matrix + (by_player.getIndex() * max_players) + with_player.getIndex() ) = true;
    SDL_mutexV(mutex);
}


void PlayerInterface::setAlliance( unsigned short by_player, unsigned short with_player )
{
    assert( (by_player < max_players) && (with_player < max_players) );

    SDL_mutexP(mutex);
    *(alliance_matrix + (with_player * max_players) + by_player ) = true;
    //*(alliance_matrix + (by_player * max_players) + with_player ) = true;
    SDL_mutexV(mutex);
}

void PlayerInterface::clearAlliance( unsigned short by_player, unsigned short with_player )
{
    assert( (by_player < max_players) && (with_player < max_players) );

    SDL_mutexP(mutex);
    *(alliance_matrix + (with_player * max_players) + by_player ) = false;
    SDL_mutexV(mutex);
}

bool PlayerInterface::isAllied( const PlayerID& player, const PlayerID& with_player )
{
    bool val;

    assert( (player.getIndex() < max_players) && (with_player.getIndex() < max_players) );

    val = *(alliance_matrix + (with_player.getIndex() * max_players) + player.getIndex() );

    return( val );

}

bool PlayerInterface::isAllied( unsigned short player, unsigned short with_player )
{
    bool val;

    assert( (player < max_players) && (with_player < max_players) );

    val = *(alliance_matrix + (with_player * max_players) + player );

    return( val );

}

void PlayerInterface::lockPlayerStats()
{
    unsigned long player_index;

    SDL_mutexP(mutex);
    for ( player_index = 0; player_index < max_players; player_index++ ) {
        player_lists[ player_index ].lockStats();
    } // ** for
    SDL_mutexV(mutex);
}

void PlayerInterface::unlockPlayerStats()
{
    unsigned long player_index;

    SDL_mutexP(mutex);
    for ( player_index = 0; player_index < max_players; player_index++ ) {
        player_lists[ player_index ].unlockStats();
    } // ** for
    SDL_mutexV(mutex);
}

void PlayerInterface::resetPlayerStats()
{
    unsigned long player_index;

    SDL_mutexP(mutex);
    for ( player_index = 0; player_index < max_players; player_index++ ) {
        player_lists[ player_index ].resetStats();
    } // ** for
    SDL_mutexV(mutex);
}

void PlayerInterface::resetAllianceMatrix()
{
    unsigned long i;
    unsigned long matrix_size;

    SDL_mutexP(mutex);
    matrix_size = max_players * max_players;

    for ( i = 0; i < matrix_size; i++ ) {
        alliance_matrix[ i ] = false;
    }
    SDL_mutexV(mutex);
}

int PlayerInterface::getActivePlayerCount()
{
    unsigned long player_index;
    int count = 0;;

    for ( player_index = 0; player_index < max_players; player_index++ ) {
        if ( player_lists[ player_index ].getStatus() == _player_state_active ) {
            count++;
        }
    } // ** for

    return( count );
}

PlayerState * PlayerInterface::allocateLoopBackPlayer()
{
    local_player_index = 0;

    SDL_mutexP(mutex);
    player_lists[ local_player_index ].setStatus( _player_state_active );
    SDL_mutexV(mutex);

    return( &player_lists[ local_player_index ] );
}

int PlayerInterface::countPlayers()
{
    int count=0;
    for ( int player_index = 0; player_index < max_players; player_index++ ) {
        if ( player_lists[ player_index ].getStatus() != _player_state_free ) {
            count++;
        }
    }
    return count;
}

PlayerState * PlayerInterface::allocateNewPlayer()
{
    unsigned long player_index;

    SDL_mutexP(mutex);
    for ( player_index = 0; player_index < max_players; player_index++ ) {
        if ( player_lists[ player_index ].getStatus() == _player_state_free ) {
            player_lists[ player_index ].setStatus( _player_state_allocated );
            player_lists[ player_index ].resetStats();
	    player_lists[ player_index ].setColor( player_index );
            SDL_mutexV(mutex);
            return( &player_lists[ player_index ] );
        } // ** if

    } // ** for
    SDL_mutexV(mutex);

    return( 0 );
}

void PlayerInterface::spawnPlayer( unsigned short player_index, const iXY &location )
{
    assert( player_index < max_players );

    spawnPlayer( player_lists[ player_index ].getPlayerID(),
                 location
               );
}

void PlayerInterface::spawnPlayer( const PlayerID &player, const iXY &location )
{
    unsigned short player_index;

    player_index = player.getIndex();
    assert( player_index < max_players );

    SDL_mutexP(mutex);
    if ( player_lists[ player_index ].getStatus() != _player_state_free ) {
        UnitInterface::spawnPlayerUnits( location,
                                         player,
                                         player_lists[ player_index ].unit_config
                                       );
    } // ** if _player_state_active
    SDL_mutexV(mutex);
}


bool PlayerInterface::testRuleScoreLimit( long score_limit, PlayerState ** player_state )
{
    unsigned long player_index;

    for ( player_index = 0; player_index < max_players; player_index++ ) {
        if ( player_lists[ player_index ].getTotal() >= score_limit ) {
            *player_state = &player_lists[ player_index ];
            return( true );
        } // ** if

    } // ** for

    return( false );
}

bool PlayerInterface::testRuleObjectiveRatio( float precentage, PlayerState ** player_state )
{
    unsigned long player_index;

    for ( player_index = 0; player_index < max_players; player_index++ ) {
        if ( ObjectiveInterface::testRuleObjectiveOccupationRatio( player_index, precentage ) == true ) {
            *player_state = &player_lists[ player_index ];
            return( true );
        } // ** if

    } // ** for

    return( false );
}


bool PlayerInterface::testRulePlayerRespawn( bool *completed, PlayerState **player_state )
{
    PlayerID player;

    if ( respawn_rule_player_index == max_players ) {
        respawn_rule_player_index = 0;
        *completed = true;
        return( false );
    } else {
        *completed = false;
    }
    player = player_lists[ respawn_rule_player_index ].getPlayerID();

    if ( player_lists[ respawn_rule_player_index ].getStatus() == _player_state_active )
        if ( UnitInterface::getUnitCount( player.getIndex() ) == 0 ) {
            *player_state = &player_lists[ respawn_rule_player_index ];
            respawn_rule_player_index++;
            return( true );
        }

    respawn_rule_player_index++;
    return( false );
}

void PlayerInterface::startPlayerStateSync( const PlayerID &connect_player  )
{
    player_sync_index = 0;
    player_sync_connect_player_index = connect_player.getIndex();
    player_sync_connect_id = connect_player;
    player_sync_timer.changeRate( 4 );
    message_encoder.resetEncoder();
}

bool PlayerInterface::syncPlayerState( int *send_return_code, int *percent_complete )
{
    unsigned short player_count = 0;
    MultiMessage *encode_message;

    *percent_complete = -1;

    int send_ret_val;
    *send_return_code = _network_ok;


    SDL_mutexP(mutex);
    if ( player_sync_timer.count() ) {

        while( (player_sync_index != max_players) &&
                (player_count < 5)
             ) {
            if( player_sync_index != player_sync_connect_player_index ) {
                PlayerStateSync sync_mesg
                    (player_lists[player_sync_index].getNetworkPlayerState());

                while( message_encoder.encodeMessage( &sync_mesg, sizeof(PlayerStateSync), &encode_message ) ) {
                    send_ret_val = SERVER->sendMessage( player_sync_connect_id,
                                                        encode_message, encode_message->realSize(),0);
                    message_encoder.resetEncoder();

                    if ( send_ret_val != _network_ok ) {
                        *send_return_code = send_ret_val;
                        SDL_mutexV(mutex);
                        return( true );
                    }
                } // ** if

            } // ** if

            player_sync_index++;
            player_count++;

        } // ** while

        message_encoder.getEncodeMessage( &encode_message );
        if ( encode_message != 0 ) {
            send_ret_val = SERVER->sendMessage(player_sync_connect_id,
                                               encode_message, encode_message->realSize(), 0);

            if ( send_ret_val != _network_ok ) {
                *send_return_code = send_ret_val;
                SDL_mutexV(mutex);
                return( true );
            }

        } // ** if

        if( (player_sync_index == max_players) ) {
            *percent_complete = 100;
            SDL_mutexV(mutex);
            return( true );
        } // ** if

        float percent;
        percent = ( ( (float) player_sync_index) / ( (float) max_players ) ) * 100;
        *percent_complete = (int) percent;
    } // ** if

    SDL_mutexV(mutex);
    return( false );
}


void PlayerInterface::netMessageConnectID( NetMessage *message )
{
    PlayerConnectID *connect_mesg;

    connect_mesg = (PlayerConnectID *) message;

    local_player_index = connect_mesg->connect_state.getPlayerIndex();

    SDL_mutexP(mutex);
    player_lists[local_player_index].setFromNetworkPlayerState
        (&connect_mesg->connect_state);
    SDL_mutexV(mutex);
}

void PlayerInterface::netMessageSyncState( NetMessage *message )
{
    PlayerStateSync *sync_mesg;
    size_t player_index;

    sync_mesg = ( PlayerStateSync *) message;
    player_index = sync_mesg->player_state.getPlayerIndex();
    SDL_mutexP(mutex);
    player_lists[player_index].setFromNetworkPlayerState(&sync_mesg->player_state);
    SDL_mutexV(mutex);
}

void PlayerInterface::netMessageScoreUpdate( NetMessage *message )
{
    PlayerScoreUpdate *score_update;

    score_update = (PlayerScoreUpdate *) message;

    setKill( score_update->kill_by_player_index,
             score_update->kill_on_player_index,
             score_update->unit_type );
}

void PlayerInterface::netMessageAllianceRequest( NetMessage *message )
{
    PlayerAllianceRequest *allie_request;
    PlayerState *player_state;

    allie_request = (PlayerAllianceRequest *) message;

    SDL_mutexP(mutex);
    if ( allie_request->alliance_request_type == _player_make_alliance ) {
        setAlliance( allie_request->allie_by_player_index, allie_request->allie_with_player_index );

        if( (allie_request->allie_by_player_index == local_player_index) ) {
            player_state = getPlayerState( allie_request->allie_with_player_index );
            ConsoleInterface::postMessage( "Alliance created with %s.",
                    player_state->getName().c_str() );
        } else
            if( (allie_request->allie_with_player_index == local_player_index) ) {
                player_state = getPlayerState( allie_request->allie_by_player_index );
                ConsoleInterface::postMessage( "%s has allied with you.",
                        player_state->getName().c_str() );
            }
    } else {
        clearAlliance( allie_request->allie_by_player_index, allie_request->allie_with_player_index );

        if( (allie_request->allie_by_player_index == local_player_index) ) {
            player_state = getPlayerState( allie_request->allie_with_player_index );
            ConsoleInterface::postMessage( "Alliance broken with %s.",
                    player_state->getName().c_str() );
        } else
            if( (allie_request->allie_with_player_index == local_player_index) ) {
                player_state = getPlayerState( allie_request->allie_by_player_index );
                ConsoleInterface::postMessage(
                        "%s has broken their alliance with you.",
                        player_state->getName().c_str() );
            }

    }

    PlayerAllianceUpdate allie_update;

    allie_update.allie_by_player_index   = allie_request->allie_by_player_index;
    allie_update.allie_with_player_index = allie_request->allie_with_player_index;
    allie_update.alliance_update_type = allie_request->alliance_request_type;
    SDL_mutexV(mutex);

    SERVER->sendMessage( &allie_update, sizeof( PlayerAllianceUpdate ), 0 );
}

void PlayerInterface::netMessageAllianceUpdate( NetMessage *message )
{
    PlayerAllianceUpdate *allie_update;
    PlayerState *player_state;

    allie_update = (PlayerAllianceUpdate *) message;

    SDL_mutexP(mutex);
    if ( allie_update->alliance_update_type == _player_make_alliance ) {
        setAlliance( allie_update->allie_by_player_index, allie_update->allie_with_player_index );

        if( (allie_update->allie_by_player_index == local_player_index) ) {
            player_state = getPlayerState( allie_update->allie_with_player_index );
            ConsoleInterface::postMessage( "Alliance created with %s.",
                    player_state->getName().c_str() );
        } else
            if( (allie_update->allie_with_player_index == local_player_index) ) {
                player_state = getPlayerState( allie_update->allie_by_player_index );
                ConsoleInterface::postMessage( "%s has allied with you.",
                        player_state->getName().c_str() );
            }
    } else {
        clearAlliance( allie_update->allie_by_player_index, allie_update->allie_with_player_index );

        if( (allie_update->allie_by_player_index == local_player_index) ) {
            player_state = getPlayerState( allie_update->allie_with_player_index );
            ConsoleInterface::postMessage( "Alliance broken with %s.",
                    player_state->getName().c_str() );
        } else
            if( (allie_update->allie_with_player_index == local_player_index) ) {
                player_state = getPlayerState( allie_update->allie_by_player_index );
                ConsoleInterface::postMessage(
                        "%s has broken their alliance with you.",
                        player_state->getName().c_str() );
            }
    }
    SDL_mutexV(mutex);
}


void PlayerInterface::processNetMessage( NetMessage *message )
{
    switch( message->message_id ) {
    case _net_message_id_player_connect_id :
        netMessageConnectID( message );
        break;

    case _net_message_id_player_sync_state :
        netMessageSyncState( message );
        break;

    case _net_message_id_player_score_update :
        netMessageScoreUpdate( message );
        break;

    case _net_message_id_player_alliance_request :
        netMessageAllianceRequest( message );
        break;

    case _net_message_id_player_alliance_update :
        netMessageAllianceUpdate( message );
        break;

    } // ** switch
}

void PlayerInterface::disconnectPlayerCleanup( PlayerID &player_id )
{
    PlayerAllianceUpdate allie_update;

    unsigned short player_index;
    unsigned short disconnect_player_index;

    disconnect_player_index = player_id.getIndex();

    SDL_mutexP(mutex);
    for ( player_index = 0; player_index < max_players; player_index++ ) {
        if ( isAllied( disconnect_player_index, player_index ) == true ) {
            allie_update.set( disconnect_player_index, player_index, _player_break_alliance );
            SERVER->sendMessage( &allie_update, sizeof( PlayerAllianceUpdate ), 0 );
        }

        if ( isAllied( player_index, disconnect_player_index ) == true ) {
            allie_update.set( player_index, disconnect_player_index, _player_break_alliance );
            SERVER->sendMessage( &allie_update, sizeof( PlayerAllianceUpdate ), 0 );
        }

    } // ** for

    player_lists[ disconnect_player_index ].setStatus( _player_state_free );

    PlayerState *player_state = getPlayerState( player_id );

    PlayerStateSync player_state_update(player_state->getNetworkPlayerState());
    SDL_mutexV(mutex);

    SERVER->sendMessage( &player_state_update, sizeof( PlayerStateSync ), 0 );
}

