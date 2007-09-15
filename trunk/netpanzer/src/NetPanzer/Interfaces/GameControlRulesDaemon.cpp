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
#include "ConsoleInterface.hpp"
#include "ChatInterface.hpp"

#include "NetworkState.hpp"
#include "SystemNetMessage.hpp"
#include "GameControlNetMessage.hpp"
#include "Server.hpp"
#include "NetworkServer.hpp"
#include "Client.hpp"
#include "ServerConnectDaemon.hpp"

#include "Desktop.hpp"
#include "ProgressView.hpp"
#include "Util/Log.hpp"

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

enum { _execution_mode_loop_back_server,
       _execution_mode_dedicated_server
     };
enum { _game_state_idle,
       _game_state_in_progress,
       _game_state_completed
     };

int GameControlRulesDaemon::execution_mode = _execution_mode_loop_back_server;
unsigned char GameControlRulesDaemon::game_state  = _game_state_idle;
std::string GameControlRulesDaemon::nextmap = "";

#define _MAP_CYCLE_ENDGAME_WAIT_PERIOD  (20) // seconds
#define _MAP_CYCLE_MAP_LOAD_WAIT_PERIOD (7) // seconds

int GameControlRulesDaemon::map_cycle_fsm_server_state = _map_cycle_server_state_idle;
Timer GameControlRulesDaemon::map_cycle_fsm_server_endgame_timer;
Timer GameControlRulesDaemon::map_cycle_fsm_server_map_load_timer;

int GameControlRulesDaemon::map_cycle_fsm_client_state = _map_cycle_client_idle;
bool GameControlRulesDaemon::map_cycle_fsm_client_respawn_ack_flag = false;
char GameControlRulesDaemon::map_cycle_fsm_client_map_name[256];

//-----------------------------------------------------------------
void GameControlRulesDaemon::setStateServerInProgress()
{
    GameControlRulesDaemon::game_state = _game_state_in_progress;
}
//-----------------------------------------------------------------
void GameControlRulesDaemon::setStateServerIdle()
{
    GameControlRulesDaemon::game_state = _game_state_idle;
}
//-----------------------------------------------------------------
void GameControlRulesDaemon::setDedicatedServer()
{
    GameControlRulesDaemon::execution_mode = _execution_mode_dedicated_server;
}

//-----------------------------------------------------------------
void GameControlRulesDaemon::mapCycleFsmClient()
{
    switch( map_cycle_fsm_client_state ) {
        case _map_cycle_client_idle :
            return;

        case _map_cycle_client_start_map_load : {
                progressView->open();

                GameManager::shutdownParticleSystems();
                ObjectiveInterface::resetLogic();

                gameconfig->map = map_cycle_fsm_client_map_name;

                char buf[256];
                snprintf(buf, sizeof(buf), "Next Map '%s'.",
                        gameconfig->map.c_str());
                progressView->scrollAndUpdate( buf);
                progressView->scrollAndUpdate( "Loading Game Map ..." );

                try {
                    GameManager::startGameMapLoad(gameconfig->map.c_str(), 16);
                } catch(std::exception& e) {
                    progressView->scrollAndUpdate("Error while loading map:");
                    progressView->scrollAndUpdate(e.what());
                    map_cycle_fsm_client_state = _map_cycle_client_idle;
                    return;
                }
                
                GameManager::resetGameLogic();
                map_cycle_fsm_client_state = _map_cycle_client_load_map;
                return;
            }
            break;

        case _map_cycle_client_load_map : {
                int percent_complete;
                char str_buf[128];

                if ( GameManager::gameMapLoad( &percent_complete ) == false ) {
                    map_cycle_fsm_client_state = _map_cycle_client_wait_for_respawn_ack;

                    sprintf( str_buf, "Loading Game Map ... (%d%%)", percent_complete);
                    progressView->update( str_buf );

                    progressView->scrollAndUpdate( "Waiting to respawn ..." );
                } else {
                    sprintf( str_buf, "Loading Game Map ... (%d%%)", percent_complete);
                    progressView->update( str_buf );
                }

                return;
            }

        case _map_cycle_client_wait_for_respawn_ack : {
                if( map_cycle_fsm_client_respawn_ack_flag == true ) {
                    progressView->toggleGameView();
                    map_cycle_fsm_client_respawn_ack_flag = false;
                    map_cycle_fsm_client_state = _map_cycle_client_idle;
                }

                return;
            }
            break;

    } // ** switch
}


void GameControlRulesDaemon::mapCycleFsmServer()
{
    switch( map_cycle_fsm_server_state ) {
        case _map_cycle_server_state_idle:
            break;

        case _map_cycle_server_state_display_endgame_views: {
                ChatInterface::setMessageScopeServer();
                ChatInterface::sendCurrentMessage("Round is over");
                                                                
                SystemViewControl view_control;

                ServerConnectDaemon::lockConnectProcess();

                view_control.set("RankView", _view_control_flag_visible_on | _view_control_flag_close_all );

                if ( GameControlRulesDaemon::execution_mode == _execution_mode_loop_back_server ) {
                    Desktop::setVisibilityAllWindows(false);
                    Desktop::setVisibility("GameView", true);
                    Desktop::setVisibility("RankView", true );
                }

                SERVER->sendMessage(&view_control, sizeof(SystemViewControl));

                map_cycle_fsm_server_endgame_timer.changePeriod( _MAP_CYCLE_ENDGAME_WAIT_PERIOD );
                map_cycle_fsm_server_endgame_timer.reset();

                map_cycle_fsm_server_state = _map_cycle_server_state_cycle_next_map;
            }
            break;

        case _map_cycle_server_state_cycle_next_map : {
                if ( map_cycle_fsm_server_endgame_timer.count() &&
                        (ServerConnectDaemon::isConnecting() == false)
                   ) {
                    GameManager::shutdownParticleSystems();

                    if(nextmap != "") {
                        gameconfig->map = nextmap;
                        nextmap = "";
                    } else {
                        gameconfig->map 
                            = MapsManager::getNextMap(gameconfig->map);
                    }
                    
                    ConsoleInterface::postMessage("loading map '%s'.",
                            gameconfig->map.c_str());

                    GameControlCycleMap cycle_map_mesg;
                    cycle_map_mesg.set( gameconfig->map.c_str() );

                    SERVER->sendMessage( &cycle_map_mesg, sizeof( GameControlCycleMap ));

                    if ( GameControlRulesDaemon::execution_mode == _execution_mode_dedicated_server ) {
                        ObjectiveInterface::resetLogic();

                        GameManager::dedicatedLoadGameMap(
                                gameconfig->map.c_str());

                        GameManager::resetGameLogic();

                        map_cycle_fsm_server_map_load_timer.changePeriod(_MAP_CYCLE_MAP_LOAD_WAIT_PERIOD);
                        map_cycle_fsm_server_map_load_timer.reset();
                        map_cycle_fsm_server_state = _map_cycle_server_state_wait_for_client_map_load;
                    } else {
                        progressView->open();

                        progressView->scrollAndUpdate( "Loading Game Map ..." );
                        ObjectiveInterface::resetLogic();

                        try {
                            GameManager::startGameMapLoad
                                (gameconfig->map.c_str(), 16);
                        } catch(std::exception& e) {
                            progressView->scrollAndUpdate(
                                    "Error while loading map:");
                            progressView->scrollAndUpdate(e.what());
                            map_cycle_fsm_server_state = _map_cycle_server_state_idle;
                            return;
                        }
                        
                        GameManager::resetGameLogic();
                        map_cycle_fsm_server_state = _map_cycle_server_state_load_map;
                        return;
                    }

                }
            }
            break;

        case _map_cycle_server_state_load_map : {
                int percent_complete;
                char str_buf[128];

                if ( GameManager::gameMapLoad( &percent_complete ) == false ) {
                    map_cycle_fsm_server_state = _map_cycle_server_state_respawn_players;
                }

                sprintf( str_buf, "Loading Game Map ... (%d%%)", percent_complete);
                progressView->update( str_buf );
            }
            break;

        case _map_cycle_server_state_wait_for_client_map_load : {
                if ( map_cycle_fsm_server_map_load_timer.count() ) {
                    ConsoleInterface::postMessage("game started.");
                    map_cycle_fsm_server_state = _map_cycle_server_state_respawn_players;
                }
            }
            break;

        case _map_cycle_server_state_respawn_players : {
                SystemResetGameLogic reset_game_logic_mesg;

                GameManager::resetGameLogic();
                SERVER->sendMessage( &reset_game_logic_mesg, sizeof(SystemResetGameLogic));

                GameManager::respawnAllPlayers();

                PlayerInterface::unlockPlayerStats();
                GameControlRulesDaemon::game_state = _game_state_in_progress;

                progressView->toggleGameView();

                GameControlCycleRespawnAck respawn_ack_mesg;
                SERVER->sendMessage( &respawn_ack_mesg, sizeof(GameControlCycleRespawnAck));

                map_cycle_fsm_server_state = _map_cycle_server_state_idle;

                ServerConnectDaemon::unlockConnectProcess();
            }
            break;

    } // ** switch
}

void GameControlRulesDaemon::onTimelimitGameCompleted()
{
    PlayerInterface::lockPlayerStats();

    map_cycle_fsm_server_state = _map_cycle_server_state_display_endgame_views;
    GameControlRulesDaemon::game_state = _game_state_completed;
}

void GameControlRulesDaemon::onFraglimitGameCompleted()
{
    PlayerInterface::lockPlayerStats();

    map_cycle_fsm_server_state = _map_cycle_server_state_display_endgame_views;
    GameControlRulesDaemon::game_state = _game_state_completed;
}

void GameControlRulesDaemon::onObjectiveGameCompleted()
{
    PlayerInterface::lockPlayerStats();

    map_cycle_fsm_server_state = _map_cycle_server_state_display_endgame_views;
    GameControlRulesDaemon::game_state = _game_state_completed;
}

void GameControlRulesDaemon::forceMapChange(std::string _nextmap)
{
    nextmap = _nextmap;

    PlayerInterface::lockPlayerStats();

    map_cycle_fsm_server_state = _map_cycle_server_state_display_endgame_views;
    GameControlRulesDaemon::game_state = _game_state_completed;
}

void GameControlRulesDaemon::checkGameRules()
{
    PlayerState *player_state;

    if ( (GameControlRulesDaemon::game_state == _game_state_in_progress) &&
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

        case _gametype_objective:
                float ratio 
                    = (float) gameconfig->objectiveoccupationpercentage / 100.0;
                if (PlayerInterface::testRuleObjectiveRatio(
                            ratio, &player_state)) {
                    onObjectiveGameCompleted( );
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

void GameControlRulesDaemon::netMessageCycleMap(const NetMessage* message)
{
    GameControlCycleMap *cycle_map_mesg;

    cycle_map_mesg = (GameControlCycleMap *) message;
    snprintf(map_cycle_fsm_client_map_name, 256, cycle_map_mesg->map_name);
    map_cycle_fsm_client_state = _map_cycle_client_start_map_load;
}

void GameControlRulesDaemon::netMessageCycleRespawnAck(const NetMessage* )
{
    map_cycle_fsm_client_respawn_ack_flag = true;
}

void GameControlRulesDaemon::processNetMessage(const NetMessage* message)
{
    switch(message->message_id) {
        case _net_message_id_game_control_cycle_map :
            netMessageCycleMap(message);
            break;

        case _net_message_id_game_control_cycle_respawn_ack :
            netMessageCycleRespawnAck(message);
            break;

        default:
            LOGGER.warning(
                    "Received GameControlRulesMessage with unknown id (%d-%d)",
                    message->message_class, message->message_id);
            break;
    }
}

void GameControlRulesDaemon::updateGameControlFlow()
{
    if ( NetworkState::status == _network_state_server ) {
        mapCycleFsmServer();
    } else {
        mapCycleFsmClient();
    }

    checkGameRules();
}

void GameControlRulesDaemon::mapLoadFailureResponse(int result_code, const char *map_name)
{
    char str_buf[128];

    if( result_code == _mapload_result_no_map_file ) {
        sprintf( str_buf, "MAP %s NOT FOUND!", map_name );
        progressView->scrollAndUpdate( str_buf);
    } else if( result_code == _mapload_result_no_wad_file ) {
            progressView->scrollAndUpdate( "MAP TILE SET NOT FOUND!" );
    }
}
