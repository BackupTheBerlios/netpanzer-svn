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

#include <stdexcept>
#include "Core/GlobalGameState.hpp"

#include "Interfaces/PlayerInterface.hpp"
#include "Interfaces/GameConfig.hpp"
#include "Interfaces/MapInterface.hpp"
#include "Interfaces/WorldViewInterface.hpp"
#include "Classes/Network/SystemNetMessage.hpp"
#include "Units/UnitInterface.hpp"
#include "Objectives/ObjectiveInterface.hpp"
#include "Interfaces/ConsoleInterface.hpp"

#include "Classes/Network/PlayerNetMessage.hpp"
#include "Classes/Network/NetworkServer.hpp"
#include "Util/Log.hpp"

// for UNIT_FLAGS_SURFACE

// ** PlayerInterface Statics
PlayerState   *PlayerInterface::player_lists = 0;
unsigned short PlayerInterface::max_players = 0;
bool *      PlayerInterface::alliance_matrix = 0;
unsigned short PlayerInterface::local_player_index = 0xFFFF;

unsigned short PlayerInterface::player_sync_index;
unsigned short PlayerInterface::player_sync_connect_player_index;
Timer PlayerInterface::player_sync_timer;

SDL_mutex* PlayerInterface::mutex = 0;

void PlayerInterface::initialize(unsigned short maxPlayers)
{
    char temp_str[64];
    Uint16 player_index;
    max_players = maxPlayers;

    delete[] player_lists;
    player_lists = new PlayerState[max_players];

    for ( player_index = 0; player_index < max_players; player_index++ )
    {
        player_lists[ player_index ].setID( player_index );
        player_lists[ player_index ].resetStats();
        player_lists[ player_index ].setStatus( _player_state_free );
        player_lists[ player_index ].setFlag( 0 );
        player_lists[ player_index ].unit_config.initialize();
        sprintf( temp_str, "Player %u", player_index );
        player_lists[ player_index ].setName( temp_str );
    }

    delete[] alliance_matrix;
    alliance_matrix = new bool [max_players * max_players];
    resetAllianceMatrix();

    mutex = SDL_CreateMutex();
    if(!mutex)
    {
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

void PlayerInterface::setKill(PlayerState* by_player, PlayerState* on_player,
        UnitType unit_type)
{
    SDL_mutexP(mutex);
    by_player->incKills( unit_type );
    on_player->incLosses( unit_type );
    SDL_mutexV(mutex);
}

void
PlayerInterface::setAlliance( unsigned short by_player, unsigned short with_player )
{
    *(alliance_matrix + (by_player * max_players) + with_player ) = true;
}

void
PlayerInterface::clearAlliance( unsigned short by_player, unsigned short with_player )
{
    *(alliance_matrix + (by_player * max_players) + with_player ) = false;
}

bool
PlayerInterface::isAllied( unsigned short player, unsigned short with_player )
{
    if (    player < max_players && with_player < max_players 
		 && *(alliance_matrix + (with_player * max_players) + player )
		 && *(alliance_matrix + (player * max_players) + with_player )
		)
	{
		return true;
	}
	return false;
}

bool
PlayerInterface::isSingleAllied( unsigned short player, unsigned short with_player )
{
    if (    player < max_players && with_player < max_players 
		 && *(alliance_matrix + (player * max_players) + with_player )
		)
	{
		return true;
	}
	return false;
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
    int count = 0;

    for ( player_index = 0; player_index < max_players; player_index++ ) {
        if ( player_lists[ player_index ].getStatus() == _player_state_active ) {
            count++;
        }
    } // ** for

    return( count );
}

PlayerState* PlayerInterface::allocateLoopBackPlayer()
{
    local_player_index = 0;

    SDL_mutexP(mutex);
    player_lists[local_player_index].setStatus(_player_state_active);
    SDL_mutexV(mutex);

    return &player_lists[local_player_index];
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
    for ( player_index = 0; player_index < max_players; player_index++ )
    {
        if ( player_lists[ player_index ].getStatus() == _player_state_free )
        {
            player_lists[ player_index ].setStatus( _player_state_allocated );
            player_lists[ player_index ].resetStats();
	    player_lists[ player_index ].setColor( player_index );
            player_lists[ player_index ].setID((unsigned short)player_index);
            SDL_mutexV(mutex);
            return( &player_lists[ player_index ] );
        } // ** if

    } // ** for
    SDL_mutexV(mutex);

    return( 0 );
}

void PlayerInterface::spawnPlayer( unsigned short player_index, const iXY &location )
{
    if ( player_index < max_players )
    {
        SDL_mutexP(mutex);
        if ( player_lists[player_index].getStatus() != _player_state_free )
        {
            UnitInterface::spawnPlayerUnits( location,
                                             player_index,
                                             player_lists[ player_index ].unit_config
                                           );
            
            iXY world_loc;
            MapInterface::mapXYtoPointXY( location, &world_loc );
            if ( getLocalPlayerIndex() == player_index )
            {
                WorldViewInterface::setCameraPosition( world_loc );
            }
            else
            {
                SystemSetPlayerView set_view(world_loc.x, world_loc.y);
                NetworkServer::sendMessage(player_index, &set_view, sizeof(SystemSetPlayerView));
            }
        } // ** if _player_state_active
        SDL_mutexV(mutex);
    }
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
            return true;
        } // ** if
    } // ** for

    return false;
}

void PlayerInterface::startPlayerStateSync(Uint16 player_index)
{
    player_sync_index = 0;
    player_sync_connect_player_index = player_index;
    player_sync_timer.changeRate( 4 );
}

bool
PlayerInterface::syncNextPlayerState( NetworkPlayerState &dest, int *percent_complete)
{
    bool hasUpdatedData = false;

    *percent_complete = -1;

    if ( player_sync_index == player_sync_connect_player_index )
    {
        ++player_sync_index;
    }
    
    if ( player_sync_index < max_players )
    {
        dest = player_lists[player_sync_index].getNetworkPlayerState();
        ++player_sync_index;
        hasUpdatedData = true;
    }
    
    if ( player_sync_index >= max_players )
    {
        *percent_complete = 100;
    }
    else
    {
        float percent;
        percent = ( ( (float) player_sync_index) / ( (float) max_players ) ) * 100;

        *percent_complete = (int) percent;
    }
    
    return hasUpdatedData;
}


void PlayerInterface::netMessageConnectID(const NetMessage* message)
{
    const PlayerConnectID *connect_mesg
        = (const PlayerConnectID *) message;

    local_player_index = connect_mesg->connect_state.getPlayerIndex();
    if(local_player_index >= max_players) {
        LOGGER.warning("Invalide netMessageConnectID Message");
        return;
    }

    SDL_mutexP(mutex);
    player_lists[local_player_index].setFromNetworkPlayerState
        (&connect_mesg->connect_state);
    SDL_mutexV(mutex);
}

void PlayerInterface::netMessageSyncState(const NetMessage* message)
{
    const PlayerStateSync *sync_mesg
        = (const PlayerStateSync *) message;
    Uint16 player_index = sync_mesg->player_state.getPlayerIndex();

    if(player_index >= max_players) {
        LOGGER.warning("Malformed MessageSyncState message");
        return;
    }
    
    SDL_mutexP(mutex);
    
	player_lists[player_index].setFromNetworkPlayerState(&sync_mesg->player_state);
	if ( player_lists[player_index].getStatus() == _player_state_free )
	{
		disconnectedPlayerAllianceCleanup(player_index);
	}
	
    SDL_mutexV(mutex);
}

void PlayerInterface::netMessageScoreUpdate(const NetMessage *message)
{
    const PlayerScoreUpdate* score_update 
        = (const PlayerScoreUpdate *) message;

    if(score_update->getKillByPlayerIndex() >= PlayerInterface::getMaxPlayers()
            || score_update->getKillOnPlayerIndex() 
            >= PlayerInterface::getMaxPlayers())
    {
        LOGGER.warning("Malformed score update packet.");
        return;
    }

    PlayerState* player1 = getPlayer(score_update->getKillByPlayerIndex());
    PlayerState* player2 = getPlayer(score_update->getKillOnPlayerIndex());
    setKill(player1, player2, (UnitType) score_update->unit_type );
}

void PlayerInterface::netMessageAllianceRequest(const NetMessage *message)
{
    if ( gameconfig->allowallies == false )
    {
        return;
    }
	
    PlayerState *player_state;

    const PlayerAllianceRequest *allie_request
        = (const PlayerAllianceRequest *) message;

    if(allie_request->getAllieByPlayerIndex() >= max_players
       || allie_request->getAllieWithPlayerIndex() >= max_players)
    {
        LOGGER.warning("Invalid alliance request message");
        return;                                                       
    }

    SDL_mutexP(mutex);
    if ( allie_request->alliance_request_type == _player_make_alliance )
    {
        setAlliance(allie_request->getAllieByPlayerIndex(),
                    allie_request->getAllieWithPlayerIndex());

        if ( allie_request->getAllieByPlayerIndex() == local_player_index )
        {
            player_state = getPlayer(allie_request->getAllieWithPlayerIndex());
            if ( isSingleAllied( player_state->getID(), local_player_index) )
            {
                ConsoleInterface::postMessage(Color::yellow, false, 0,
                                              "You accepted %s alliance request.",
                                              player_state->getName().c_str());
            }
            else
            {
                ConsoleInterface::postMessage(Color::yellow, false, 0,
                                              "You request alliance with %s.",
                                              player_state->getName().c_str());
            }
        }
        else if ( allie_request->getAllieWithPlayerIndex() == local_player_index )
        {
            player_state = getPlayer( allie_request->getAllieByPlayerIndex() );
            if ( isSingleAllied( local_player_index, player_state->getID()) )
            {
                ConsoleInterface::postMessage(Color::yellow, false, 0,
                                           "%s accepted your alliance request.",
                                           player_state->getName().c_str());
            }
            else
            {
                ConsoleInterface::postMessage(Color::yellow, false, 0,
                                              "%s request to ally with you.",
                                              player_state->getName().c_str());
            }
        }
    }
    else
    {
        // break alliance cancels both alliances
        clearAlliance( allie_request->getAllieByPlayerIndex(),
                       allie_request->getAllieWithPlayerIndex());

        if ( allie_request->getAllieByPlayerIndex() == local_player_index )
        {
            player_state = getPlayer( allie_request->getAllieWithPlayerIndex() );
            if ( isSingleAllied( player_state->getID(), local_player_index) )
            {
                ConsoleInterface::postMessage(Color::yellow, false, 0,
                                              "You broke the alliance with %s.",
                                              player_state->getName().c_str());
            }
            else
            {
                ConsoleInterface::postMessage(Color::yellow, false, 0,
                                              "You cancel your alliance request with %s.",
                                              player_state->getName().c_str());
            }
        }
        else if ( allie_request->getAllieWithPlayerIndex() == local_player_index )
        {
            player_state = getPlayer( allie_request->getAllieByPlayerIndex() );
            if ( isSingleAllied( local_player_index, player_state->getID()) )
            {
                ConsoleInterface::postMessage(Color::yellow, false, 0,
                                              "%s broke the alliance with you.",
                                              player_state->getName().c_str());
            }
            else
            {
                ConsoleInterface::postMessage(Color::yellow, false, 0,
                                              "%s cancelled the alliance request with you.",
                                              player_state->getName().c_str());
            }
        }

        clearAlliance(allie_request->getAllieWithPlayerIndex(),
                      allie_request->getAllieByPlayerIndex());
    }
    SDL_mutexV(mutex);

    PlayerAllianceUpdate allie_update;
    allie_update.set(allie_request->getAllieByPlayerIndex(),
                     allie_request->getAllieWithPlayerIndex(),
                     allie_request->alliance_request_type);
    //NetworkServer::broadcastMessage(&allie_update, sizeof(PlayerAllianceUpdate));
    if ( allie_request->getAllieByPlayerIndex() != local_player_index )
    {
        NetworkServer::sendMessage(allie_request->getAllieByPlayerIndex(),
                                   &allie_update, sizeof(PlayerAllianceUpdate));
    }
    if ( allie_request->getAllieWithPlayerIndex() != local_player_index )
    {
        NetworkServer::sendMessage(allie_request->getAllieWithPlayerIndex(),
                                   &allie_update, sizeof(PlayerAllianceUpdate));
    }
}

void PlayerInterface::netMessageAllianceUpdate(const NetMessage* message)
{
    PlayerState *player_state;

    const PlayerAllianceUpdate* allie_update
        = (const PlayerAllianceUpdate *) message;

    if(allie_update->getAllieByPlayerIndex() >= max_players
       || allie_update->getAllieWithPlayerIndex() >= max_players) {
        LOGGER.warning("Invalid alliance update message");
        return;
    }

    SDL_mutexP(mutex);
    if (allie_update->alliance_update_type == _player_make_alliance)
    {
        setAlliance(  allie_update->getAllieByPlayerIndex(),
					  allie_update->getAllieWithPlayerIndex());

        if ( allie_update->getAllieByPlayerIndex() == local_player_index )
		{
            player_state = getPlayer( allie_update->getAllieWithPlayerIndex() );
			if ( isSingleAllied( player_state->getID(), local_player_index) )
			{
				ConsoleInterface::postMessage( Color::yellow, false, 0, "You accepted %s alliance request.",
												player_state->getName().c_str() );
			}
			else
			{
				ConsoleInterface::postMessage( Color::yellow, false, 0, "You request alliance with %s.",
												player_state->getName().c_str() );
			}
        }
		else if ( allie_update->getAllieWithPlayerIndex() == local_player_index )
		{
			player_state = getPlayer( allie_update->getAllieByPlayerIndex() );
			if ( isSingleAllied( local_player_index, player_state->getID() ) )
			{
				ConsoleInterface::postMessage( Color::yellow, false, 0, "%s accepted your alliance request.",
												player_state->getName().c_str() );
			}
			else
			{
				ConsoleInterface::postMessage( Color::yellow, false, 0, "%s request to ally with you.",
												player_state->getName().c_str() );
			}
        }
    }
    else
    {
		// break alliance cancels both alliances
		clearAlliance( allie_update->getAllieByPlayerIndex(),
                       allie_update->getAllieWithPlayerIndex());

        if ( allie_update->getAllieByPlayerIndex() == local_player_index )
		{
            player_state = getPlayer( allie_update->getAllieWithPlayerIndex() );
			if ( isSingleAllied( player_state->getID(), local_player_index) )
			{
				ConsoleInterface::postMessage( Color::yellow, false, 0, "You broke the alliance with %s.",
												player_state->getName().c_str() );
			}
			else
			{
				ConsoleInterface::postMessage( Color::yellow, false, 0, "You cancel your alliance request with %s.",
												player_state->getName().c_str() );
			}
        }
		else if ( allie_update->getAllieWithPlayerIndex() == local_player_index )
		{
			player_state = getPlayer( allie_update->getAllieByPlayerIndex() );
			if ( isSingleAllied( local_player_index, player_state->getID()) )
			{
				ConsoleInterface::postMessage( Color::yellow, false, 0, "%s broke the alliance with you.",
												player_state->getName().c_str() );
			}
			else
			{
				ConsoleInterface::postMessage( Color::yellow, false, 0, "%s cancelled the alliance request with you.",
												player_state->getName().c_str() );
			}
		}
		
		clearAlliance( allie_update->getAllieWithPlayerIndex(),
						allie_update->getAllieByPlayerIndex() );
    }
    SDL_mutexV(mutex);
}

void PlayerInterface::processNetMessage(const NetMessage* message)
{
    switch(message->message_id) {
        case _net_message_id_player_connect_id :
            netMessageConnectID(message);
            break;

        case _net_message_id_player_sync_state :
            netMessageSyncState(message);
            break;

        case _net_message_id_player_score_update :
            netMessageScoreUpdate(message);
            break;

        case _net_message_id_player_alliance_request :
            netMessageAllianceRequest(message);
            break;

        case _net_message_id_player_alliance_update :
            netMessageAllianceUpdate(message);
            break;
    }
}

void PlayerInterface::disconnectPlayerCleanup( Uint16 index )
{
    PlayerState *player_state = getPlayer( index );
    if ( player_state )
    {
        PlayerAllianceUpdate allie_update;

        unsigned short disconnect_player_index;

        disconnect_player_index = index;

        SDL_mutexP(mutex);
		
        disconnectedPlayerAllianceCleanup(disconnect_player_index);

        player_state->setStatus( _player_state_free );

        PlayerStateSync player_state_update(player_state->getNetworkPlayerState());
        
        SDL_mutexV(mutex);

        NetworkServer::broadcastMessage(&player_state_update, sizeof(PlayerStateSync));
    }
}

void
PlayerInterface::disconnectedPlayerAllianceCleanup( Uint16 index )
{
	for ( Uint16 player_index = 0; player_index < max_players; ++player_index )
	{
		clearAlliance( index, player_index );
		clearAlliance( player_index, index );
	}
}

