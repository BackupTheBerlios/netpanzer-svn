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
#include "GameControlRulesDaemon.hpp"

#include "GameManager.hpp"
#include "GameConfig.hpp"
#include "PlayerInterface.hpp"
#include "ObjectiveInterface.hpp"
#include "MapsManager.hpp"

#include "NetworkState.hpp"
#include "SystemNetMessage.hpp"
#include "GameControlNetMessage.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "ServerConnectDaemon.hpp"

#include "Desktop.hpp"
#include "ProgressView.hpp"

enum { _map_cycle_server_state_idle,
       _map_cycle_server_state_display_endgame_views,
       _map_cycle_server_state_cycle_next_map,
       _map_cycle_server_state_load_map,
       _map_cycle_server_state_wait_for_client_map_load,
       _map_cycle_server_state_respawn_players
     };

enum { _map_cycle_client_idle,
       _map_cycle_client_start_map_load,
       _map_cycle_client_load_map,
       _map_cycle_client_wait_for_respawn_ack
     };


#define _MAP_CYCLE_ENDGAME_WAIT_PERIOD  (30) // seconds
#define _MAP_CYCLE_MAP_LOAD_WAIT_PERIOD (20) // seconds

int GameControlRulesDaemon::map_cycle_fsm_server_state = _map_cycle_server_state_idle;
Timer GameControlRulesDaemon::map_cycle_fsm_server_endgame_timer;
Timer GameControlRulesDaemon::map_cycle_fsm_server_map_load_timer;

int GameControlRulesDaemon::map_cycle_fsm_client_state = _map_cycle_client_idle;
bool GameControlRulesDaemon::map_cycle_fsm_client_respawn_ack_flag = false;
char GameControlRulesDaemon::map_cycle_fsm_client_map_name[256];

void GameControlRulesDaemon::mapCycleFsmClient( void )
{
    bool end_cycle = false;

    do {
        switch( map_cycle_fsm_client_state ) {
        case _map_cycle_client_idle : {
                end_cycle = true;
            }
            break;

        case _map_cycle_client_start_map_load : {
                progressView.open();

                GameManager::shutdownParticleSystems();
                ObjectiveInterface::resetLogic();

                progressView.scrollAndUpdate( "Loading Game Map ..." );

                int result_code;
                GameManager::startGameMapLoad( map_cycle_fsm_client_map_name, 16, &result_code );

                if( result_code != _mapload_result_success ) {
                    mapLoadFailureResponse( result_code, map_cycle_fsm_client_map_name );
                    map_cycle_fsm_client_state = _map_cycle_client_idle;
                } else {
                    GameManager::resetGameLogic();

                    map_cycle_fsm_client_state = _map_cycle_client_load_map;
                }
                end_cycle = true;
            }
            break;

        case _map_cycle_client_load_map : {
                int percent_complete;
                char str_buf[128];

                if ( GameManager::gameMapLoad( &percent_complete ) == false ) {
                    map_cycle_fsm_client_state = _map_cycle_client_wait_for_respawn_ack;

                    sprintf( str_buf, "Loading Game Map ... (%d%%)", percent_complete);
                    progressView.update( str_buf );

                    progressView.scrollAndUpdate( "Waiting to respawn ..." );
                } else {
                    sprintf( str_buf, "Loading Game Map ... (%d%%)", percent_complete);
                    progressView.update( str_buf );
                }

                end_cycle = true;
            }
            break;

        case _map_cycle_client_wait_for_respawn_ack : {
                if( map_cycle_fsm_client_respawn_ack_flag == true ) {
                    progressView.toggleGameView();
                    map_cycle_fsm_client_respawn_ack_flag = false;
                    map_cycle_fsm_client_state = _map_cycle_client_idle;
                }

                end_cycle = true;
            }
            break;

        } // ** switch
    } while ( end_cycle == false );

}


void GameControlRulesDaemon::mapCycleFsmServer( void )
{
    bool end_cycle = false;

    do {
        switch( map_cycle_fsm_server_state ) {

        case _map_cycle_server_state_idle : {
                end_cycle = true;
            }
            break;

        case _map_cycle_server_state_display_endgame_views: {
                SystemViewControl view_control;

                ServerConnectDaemon::lockConnectProcess();

                view_control.set( "WinnerMesgView", _view_control_flag_visible_on | _view_control_flag_close_all );

                if ( GameManager::execution_mode == _execution_mode_loop_back_server ) {
                    Desktop::setVisibilityAllWindows(false);
                    Desktop::setVisibility("GameView", true);
                    Desktop::setVisibility("WinnerMesgView", true );
                }

                SERVER->sendMessage( &view_control, sizeof( SystemViewControl ), 0 );

                map_cycle_fsm_server_endgame_timer.changePeriod( _MAP_CYCLE_ENDGAME_WAIT_PERIOD );
                map_cycle_fsm_server_endgame_timer.reset();

                map_cycle_fsm_server_state = _map_cycle_server_state_cycle_next_map;
                end_cycle = true;
            }
            break;

        case _map_cycle_server_state_cycle_next_map : {
                if ( map_cycle_fsm_server_endgame_timer.count() &&
                        (ServerConnectDaemon::isConnecting() == false)
                   ) {
                    char map_name[256];

                    GameManager::shutdownParticleSystems();

                    MapsManager::cycleNextMapName( map_name );
                    gameconfig->map = map_name;

                    GameControlCycleMap cycle_map_mesg;
                    cycle_map_mesg.set( map_name );

                    SERVER->sendMessage( &cycle_map_mesg, sizeof( GameControlCycleMap ), 0 );

                    if ( GameManager::execution_mode == _execution_mode_dedicated_server ) {
                        ObjectiveInterface::resetLogic();

                        GameManager::dedicatedLoadGameMap( map_name );

                        GameManager::resetGameLogic();

                        map_cycle_fsm_server_map_load_timer.changePeriod(_MAP_CYCLE_MAP_LOAD_WAIT_PERIOD);
                        map_cycle_fsm_server_map_load_timer.reset();
                        map_cycle_fsm_server_state = _map_cycle_server_state_wait_for_client_map_load;
                    } else {
                        progressView.open();

                        progressView.scrollAndUpdate( "Loading Game Map ..." );
                        ObjectiveInterface::resetLogic();

                        int result_code;

                        GameManager::startGameMapLoad( map_name, 16, &result_code );

                        if( result_code != _mapload_result_success ) {
                            mapLoadFailureResponse( result_code, map_name );
                            map_cycle_fsm_server_state = _map_cycle_server_state_idle;
                        } else {
                            GameManager::resetGameLogic();

                            map_cycle_fsm_server_state = _map_cycle_server_state_load_map;
                        }
                    }

                }

                end_cycle = true;
            }
            break;

        case _map_cycle_server_state_load_map : {
                int percent_complete;
                char str_buf[128];

                if ( GameManager::gameMapLoad( &percent_complete ) == false ) {
                    map_cycle_fsm_server_state = _map_cycle_server_state_respawn_players;
                }

                sprintf( str_buf, "Loading Game Map ... (%d%%)", percent_complete);
                progressView.update( str_buf );

                end_cycle = true;
            }
            break;

        case _map_cycle_server_state_wait_for_client_map_load : {
                if ( map_cycle_fsm_server_map_load_timer.count() ) {
                    map_cycle_fsm_server_state = _map_cycle_server_state_respawn_players;
                }

                end_cycle = true;
            }
            break;

        case _map_cycle_server_state_respawn_players : {
                SystemResetGameLogic reset_game_logic_mesg;

                GameManager::resetGameLogic();
                SERVER->sendMessage( &reset_game_logic_mesg, sizeof(SystemResetGameLogic), 0 );

                GameManager::respawnAllPlayers();

                PlayerInterface::unlockPlayerStats();
                GameManager::game_state = _game_state_in_progress;

                if ( GameManager::execution_mode == _execution_mode_loop_back_server ) {
                    progressView.toggleGameView();
                }

                GameControlCycleRespawnAck respawn_ack_mesg;
                SERVER->sendMessage( &respawn_ack_mesg, sizeof(GameControlCycleRespawnAck), 0 );

                map_cycle_fsm_server_state = _map_cycle_server_state_idle;

                ServerConnectDaemon::unlockConnectProcess();
                end_cycle = true;
            }
            break;

        } // ** switch
    } while ( end_cycle == false );

}

void GameControlRulesDaemon::onTimelimitGameCompleted( void )
{
    PlayerInterface::lockPlayerStats();

    if( gameconfig->mapcycling == true ) {
        map_cycle_fsm_server_state = _map_cycle_server_state_display_endgame_views;
        GameManager::game_state = _game_state_completed;
    } else {
        SystemViewControl view_control;

        view_control.set( "WinnerMesgView", _view_control_flag_visible_on | _view_control_flag_close_all );

        Desktop::setVisibilityAllWindows(false);
        Desktop::setVisibility("GameView", true);
        Desktop::setVisibility("WinnerMesgView", true );

        SERVER->sendMessage( &view_control, sizeof( SystemViewControl ), 0 );

        GameManager::game_state = _game_state_completed;
    }
}

void GameControlRulesDaemon::onFraglimitGameCompleted( void )
{
    PlayerInterface::lockPlayerStats();

    if( gameconfig->mapcycling == true ) {
        map_cycle_fsm_server_state = _map_cycle_server_state_display_endgame_views;
        GameManager::game_state = _game_state_completed;
    } else {
        SystemViewControl view_control;

        view_control.set( "WinnerMesgView", _view_control_flag_visible_on | _view_control_flag_close_all );

        Desktop::setVisibilityAllWindows(false);
        Desktop::setVisibility("GameView", true);
        Desktop::setVisibility("WinnerMesgView", true );

        SERVER->sendMessage( &view_control, sizeof( SystemViewControl ), 0 );

        GameManager::game_state = _game_state_completed;
    }

}

void GameControlRulesDaemon::onObjectiveGameCompleted( void )
{
    PlayerInterface::lockPlayerStats();

    if( gameconfig->mapcycling == true ) {
        map_cycle_fsm_server_state = _map_cycle_server_state_display_endgame_views;
        GameManager::game_state = _game_state_completed;
    } else {
        SystemViewControl view_control;

        view_control.set( "WinnerMesgView", _view_control_flag_visible_on | _view_control_flag_close_all );

        Desktop::setVisibilityAllWindows(false);
        Desktop::setVisibility("GameView", true);
        Desktop::setVisibility("WinnerMesgView", true );

        SERVER->sendMessage( &view_control, sizeof( SystemViewControl ), 0 );

        GameManager::game_state = _game_state_completed;
    }
}

void GameControlRulesDaemon::forceMapCycle( void )
{
    if( gameconfig->mapcycling == true ) {
        map_cycle_fsm_server_state = _map_cycle_server_state_display_endgame_views;
        GameManager::game_state = _game_state_completed;
    }
}

void GameControlRulesDaemon::checkGameRules( void )
{
    PlayerState *player_state;

    if ( (GameManager::game_state == _game_state_in_progress) &&
            (NetworkState::status == _network_state_server)
       ) {
        unsigned char game_type;
        game_type = gameconfig->gametype;

        switch( game_type ) {
        case  _gametype_timelimit : {
                int game_minutes = GameManager::getGameTime() / 60;
                if( game_minutes >= gameconfig->timelimit ) {
                    onTimelimitGameCompleted();
                }
            }

        case _gametype_fraglimit : {
                if ( PlayerInterface::testRuleScoreLimit( gameconfig->fraglimit, &player_state ) == true ) {
                    onFraglimitGameCompleted();
                }
            }
            break;

        case _gametype_objective : {
                if ( PlayerInterface::testRuleObjectiveRatio(
                            gameconfig->objectiveoccupationpercentage / 100.0, &player_state ) == true ) {
                    onObjectiveGameCompleted( );
                }
            }
            break;

        } // ** switch

        // ** Check for Player Respawns **
        bool respawn_rule_complete = false;
        while( respawn_rule_complete == false ) {
            if ( PlayerInterface::testRulePlayerRespawn( &respawn_rule_complete, &player_state ) ) {
                GameManager::spawnPlayer( player_state );
            } // ** if testRulePlayerRespawn

        } // ** while

    }

}

void GameControlRulesDaemon::netMessageCycleMap( NetMessage *message )
{
    GameControlCycleMap *cycle_map_mesg;

    cycle_map_mesg = (GameControlCycleMap *) message;
    strcpy(map_cycle_fsm_client_map_name, cycle_map_mesg->map_name);
    map_cycle_fsm_client_state =_map_cycle_client_start_map_load;
}

void GameControlRulesDaemon::netMessageCycleRespawnAck( NetMessage *message )
{
    map_cycle_fsm_client_respawn_ack_flag = true;
}

void GameControlRulesDaemon::processNetMessage( NetMessage *message )
{
    switch( message->message_id ) {
    case _net_message_id_game_control_cycle_map :
        netMessageCycleMap( message );
        break;

    case _net_message_id_game_control_cycle_respawn_ack :
        netMessageCycleRespawnAck( message );
        break;

    }
}

void GameControlRulesDaemon::updateGameControlFlow( void )
{
    if ( NetworkState::status == _network_state_server ) {
        mapCycleFsmServer();
    } else {
        mapCycleFsmClient();
    }

    checkGameRules();
}

void GameControlRulesDaemon::mapLoadFailureResponse( int result_code, char *map_name )
{
    char str_buf[128];

    if( result_code == _mapload_result_no_map_file ) {
        sprintf( str_buf, "MAP %s NOT FOUND!", map_name );
        progressView.scrollAndUpdate( str_buf);
        progressView.scrollAndUpdate( "please download this map from" );
        progressView.scrollAndUpdate( "www.pyrosoftgames.com" );
    } else
        if( result_code == _mapload_result_no_wad_file ) {
            progressView.scrollAndUpdate( "MAP TILE SET NOT FOUND!" );
            progressView.scrollAndUpdate( "please download the appropriate tileset" );
            progressView.scrollAndUpdate( "from www.pyrosoftgames.com" );
        }
}
