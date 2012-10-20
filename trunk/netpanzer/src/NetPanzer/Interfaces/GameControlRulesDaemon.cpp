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

#include "Interfaces/GameControlRulesDaemon.hpp"
#include "Interfaces/GameManager.hpp"
#include "Interfaces/GameConfig.hpp"
#include "Interfaces/PlayerInterface.hpp"
#include "Interfaces/MapsManager.hpp"
#include "Interfaces/ConsoleInterface.hpp"
#include "Interfaces/ChatInterface.hpp"
#include "Interfaces/TeamManager.hpp"
#include "Interfaces/WorldViewInterface.hpp"
#include "Interfaces/StrManager.hpp"

#include "Objectives/ObjectiveInterface.hpp"

#include "Classes/Network/NetworkState.hpp"
#include "Classes/Network/SystemNetMessage.hpp"
#include "Classes/Network/GameControlNetMessage.hpp"
#include "Classes/Network/NetworkServer.hpp"
#include "Classes/Network/ServerConnectDaemon.hpp"

#include "Units/UnitProfileInterface.hpp"

#include "Views/Components/Desktop.hpp"
#include "Views/Game/LoadingView.hpp"

#include "Util/Log.hpp"

enum { _map_cycle_server_state_idle,
       _map_cycle_server_state_display_endgame_views,
       _map_cycle_server_state_cycle_next_map,
       _map_cycle_server_state_load_map,
       _map_cycle_server_state_wait_for_client_map_load,
       _map_cycle_server_state_load_unit_profiles,
       _map_cycle_server_state_sync_profiles,
       _map_cycle_server_state_respawn_players,
       _map_cycle_server_restart_team,
       _map_cycle_server_prepare_team
     };

enum { _map_cycle_client_idle,
       _map_cycle_client_start_map_load,
       _map_cycle_client_load_map,
       _map_cycle_client_wait_for_respawn_ack,
       _map_cycle_client_prepare_team,
       _map_cycle_client_team_start
     };

enum { _execution_mode_loop_back_server,
       _execution_mode_dedicated_server
     };

int GameControlRulesDaemon::execution_mode = _execution_mode_loop_back_server;
unsigned char GameControlRulesDaemon::game_state  = _game_state_idle;
std::string GameControlRulesDaemon::nextmap = "";

#define _MAP_CYCLE_ENDGAME_WAIT_PERIOD  (20) // seconds
#define _MAP_CYCLE_MAP_LOAD_WAIT_PERIOD (7) // seconds

int GameControlRulesDaemon::map_cycle_fsm_server_state = _map_cycle_server_state_idle;
Timer GameControlRulesDaemon::map_cycle_fsm_server_endgame_timer;
Timer GameControlRulesDaemon::map_cycle_fsm_server_map_load_timer;
Timer GameControlRulesDaemon::cooldown;

int GameControlRulesDaemon::map_cycle_fsm_client_state = _map_cycle_client_idle;
bool GameControlRulesDaemon::map_cycle_fsm_client_respawn_ack_flag = false;
char GameControlRulesDaemon::map_cycle_fsm_client_map_name[256];

static unsigned int sync_profile_index = 0;

//-----------------------------------------------------------------
void GameControlRulesDaemon::setStateServerInProgress()
{
    GameControlRulesDaemon::game_state = _game_state_in_progress;
}
//-----------------------------------------------------------------
void GameControlRulesDaemon::setStateServerIdle()
{
    GameControlRulesDaemon::game_state = _game_state_idle;
    map_cycle_fsm_client_state = _map_cycle_client_idle;
}

void GameControlRulesDaemon::setStateServerprepareteam()
{
    GameControlRulesDaemon::game_state = _game_state_prepare_team;
    map_cycle_fsm_server_state = _map_cycle_server_prepare_team;
    cooldown.changePeriod(300);
    cooldown.reset();
}
//-----------------------------------------------------------------
void GameControlRulesDaemon::setDedicatedServer()
{
    GameControlRulesDaemon::execution_mode = _execution_mode_dedicated_server;
}

//-----------------------------------------------------------------
void GameControlRulesDaemon::mapCycleFsmClient()
{
    switch( map_cycle_fsm_client_state )
    {
    case _map_cycle_client_idle :
        return;

    case _map_cycle_client_start_map_load :
    {
        LoadingView::show();

        GameManager::shutdownParticleSystems();
        ObjectiveInterface::resetLogic();

        GameConfig::game_map->assign( map_cycle_fsm_client_map_name );

        char buf[256];
        snprintf(buf, sizeof(buf), _("Next Map '%s'."),
                 GameConfig::game_map->c_str());
        LoadingView::append( buf);
        LoadingView::append( _("Loading Game Map...") );

        try
        {
            GameManager::startGameMapLoad(GameConfig::game_map->c_str(), 16);
        }
        catch(std::exception& e)
        {
            LoadingView::append(_("Error while loading map:"));
            LoadingView::append(e.what());
            map_cycle_fsm_client_state = _map_cycle_client_idle;
            return;
        }

        GameManager::resetGameLogic();
        map_cycle_fsm_client_state = _map_cycle_client_load_map;
        return;
    }
    break;

    case _map_cycle_client_load_map :
    {
        int percent_complete;
        char str_buf[128];

        if ( GameManager::gameMapLoad( &percent_complete ) == false )
        {
            map_cycle_fsm_client_state = _map_cycle_client_wait_for_respawn_ack;

            sprintf( str_buf, "%s (%d%%)", _("Loading Game Map..."), percent_complete);
            LoadingView::update( str_buf );

            LoadingView::append( _("Waiting to respawn...") );
        }
        else
        {
            sprintf( str_buf, "%s (%d%%)", _("Loading Game Map..."), percent_complete);
            LoadingView::update( str_buf );
        }

        return;
    }

    case _map_cycle_client_wait_for_respawn_ack :
    {
        if( map_cycle_fsm_client_respawn_ack_flag == true )
        {
            LoadingView::loadFinish();
            map_cycle_fsm_client_respawn_ack_flag = false;
            map_cycle_fsm_client_state = _map_cycle_client_idle;
            if (GameConfig::game_teammode)
            {
                setStateServerprepareteam();
                TeamManager::reset();
                cooldown.reset();
                map_cycle_fsm_client_state = _map_cycle_client_prepare_team;
            }
        }
        return;
    }
    break;

    case _map_cycle_client_prepare_team :
    {
        if (!Desktop::getVisible("PrepareTeam")&& !Desktop::getVisible("GFlagSelectionView"))
        {
            Desktop::setVisibility("PrepareTeam", true);
            TeamManager::reset();
        }
        if (Desktop::getVisible("PrepareTeam"))
            WorldViewInterface::MoveCamera();
        return;
    }
    break;

    case _map_cycle_client_team_start :
    {
        Desktop::setVisibility("PrepareTeam", false);
        map_cycle_fsm_client_state = _map_cycle_client_idle;
        setStateServerInProgress();
        GameManager::startGameTimer();
        return;
    }
    break;

    } // ** switch
}


void GameControlRulesDaemon::mapCycleFsmServer()
{
    switch( map_cycle_fsm_server_state )
    {
    case _map_cycle_server_state_idle:
        break;

    case _map_cycle_server_state_display_endgame_views:
    {
        ChatInterface::serversay("Round is over");

        SystemViewControl view_control;

        ServerConnectDaemon::lockConnectProcess();

        view_control.set("EndRoundView", _view_control_flag_visible_on | _view_control_flag_close_all );

        if ( GameControlRulesDaemon::execution_mode == _execution_mode_loop_back_server )
        {
            Desktop::setVisibility("GameView", true);
            Desktop::setVisibility("EndRoundView", true );
        }

        SERVER->broadcastMessage(&view_control, sizeof(SystemViewControl));

        map_cycle_fsm_server_endgame_timer.changePeriod( _MAP_CYCLE_ENDGAME_WAIT_PERIOD );
        map_cycle_fsm_server_endgame_timer.reset();

        map_cycle_fsm_server_state = _map_cycle_server_state_cycle_next_map;
    }
    break;

    case _map_cycle_server_state_cycle_next_map :
    {
        if ( map_cycle_fsm_server_endgame_timer.count() &&
                (ServerConnectDaemon::isConnecting() == false)
           )
        {
            GameManager::shutdownParticleSystems();

            if(nextmap != "")
            {
                GameConfig::game_map->assign( nextmap );
                nextmap = "";
            }
            else
            {
                GameConfig::game_map->assign( MapsManager::getNextMap( *GameConfig::game_map ) );
            }

            ConsoleInterface::postMessage(Color::white, false, 0, "loading map '%s'.",
                                          GameConfig::game_map->c_str());

            GameControlCycleMap cycle_map_mesg;
            cycle_map_mesg.set( GameConfig::game_map->c_str() );

            SERVER->broadcastMessage( &cycle_map_mesg, sizeof( GameControlCycleMap ));

            if ( GameControlRulesDaemon::execution_mode == _execution_mode_dedicated_server )
            {
                ObjectiveInterface::resetLogic();

                GameManager::dedicatedLoadGameMap(
                    GameConfig::game_map->c_str());

                GameManager::resetGameLogic();

                map_cycle_fsm_server_map_load_timer.changePeriod(_MAP_CYCLE_MAP_LOAD_WAIT_PERIOD);
                map_cycle_fsm_server_map_load_timer.reset();
                map_cycle_fsm_server_state = _map_cycle_server_state_wait_for_client_map_load;
            }
            else
            {
                LoadingView::show();

                LoadingView::append( "Loading Game Map..." );
                ObjectiveInterface::resetLogic();

                try
                {
                    GameManager::startGameMapLoad
                    (GameConfig::game_map->c_str(), 16);
                }
                catch(std::exception& e)
                {
                    LoadingView::append("Error while loading map:");
                    LoadingView::append(e.what());
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

    case _map_cycle_server_state_load_map :
    {
        int percent_complete;
        char str_buf[128];

        if ( GameManager::gameMapLoad( &percent_complete ) == false )
        {
            map_cycle_fsm_server_state = _map_cycle_server_state_load_unit_profiles;
        }

        if ( GameControlRulesDaemon::execution_mode != _execution_mode_dedicated_server )
        {
            sprintf( str_buf, "%s (%d%%)", _("Loading Game Map..."), percent_complete);
            LoadingView::update( str_buf );
        }
    }
    break;

    case _map_cycle_server_state_wait_for_client_map_load :
    {
        if ( map_cycle_fsm_server_map_load_timer.count() )
        {
            ConsoleInterface::postMessage(Color::white, false, 0, "game started.");
            map_cycle_fsm_server_state = _map_cycle_server_state_load_unit_profiles;
        }
    }
    break;

    case _map_cycle_server_state_load_unit_profiles:
    {
        UnitProfileInterface::loadUnitProfiles();
        Uint8 data[ _MAX_NET_PACKET_SIZE ];
        NetMessage *pmsg = (NetMessage*)&data;
        int len = UnitProfileInterface::fillProfileResetMessage(pmsg);
        SERVER->broadcastMessage(pmsg, len);
        sync_profile_index = 0;
        map_cycle_fsm_server_state = _map_cycle_server_state_sync_profiles;
    }
    break;

    case _map_cycle_server_state_sync_profiles :
    {
        if ( sync_profile_index <= UnitProfileInterface::getNumUnitTypes() )
        {
            Uint8 data[ _MAX_NET_PACKET_SIZE ];
            NetMessage *pmsg = (NetMessage*)&data;
            int len = UnitProfileInterface::fillProfileSyncMessage(pmsg, sync_profile_index);
            SERVER->broadcastMessage(pmsg, len);
            sync_profile_index++;
        }
        if ( sync_profile_index >= UnitProfileInterface::getNumUnitTypes() )
        {
            map_cycle_fsm_server_state = _map_cycle_server_state_respawn_players;
        }
    }
    break;

    case _map_cycle_server_state_respawn_players :
    {
        SystemResetGameLogic reset_game_logic_mesg;

        GameManager::resetGameLogic();
        SERVER->broadcastMessage( &reset_game_logic_mesg, sizeof(SystemResetGameLogic));

        PlayerInterface::unlockPlayerStats();

        if ( GameControlRulesDaemon::execution_mode != _execution_mode_dedicated_server )
        {
            LoadingView::loadFinish();
        }
        GameControlCycleRespawnAck respawn_ack_mesg;
        SERVER->broadcastMessage( &respawn_ack_mesg, sizeof(GameControlCycleRespawnAck));

        if (GameConfig::game_teammode)
        {
            map_cycle_fsm_server_state = _map_cycle_server_restart_team;
        }
        else
        {
            map_cycle_fsm_server_state = _map_cycle_server_state_idle;
        }
        ServerConnectDaemon::unlockConnectProcess();
    }
    break;

    case _map_cycle_server_restart_team :
    {
        map_cycle_fsm_server_state = _map_cycle_server_prepare_team;
        TeamManager::reset();
        setStateServerprepareteam();
        cooldown.reset();
    }
    break;

    case _map_cycle_server_prepare_team :
    {
       if (cooldown.count() || TeamManager::CheckisPlayerReady())
        {
            TeamManager::BalancedTeam();
            GameControlCycleTeamStart team_start_mesg;
            SERVER->broadcastMessage(&team_start_mesg, sizeof(GameControlCycleTeamStart));
            TeamManager::SpawnTeams();
            map_cycle_fsm_server_state = _map_cycle_server_state_idle;
            GameManager::startGameTimer();
            setStateServerInProgress();
        }
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

void GameControlRulesDaemon::forceEndRound()
{
    PlayerInterface::lockPlayerStats();

    map_cycle_fsm_server_state = _map_cycle_server_state_display_endgame_views;
    GameControlRulesDaemon::game_state = _game_state_completed;
}


void GameControlRulesDaemon::checkGameRules()
{
    PlayerState *player_state;


    if ( (GameControlRulesDaemon::game_state == _game_state_in_progress) &&
            (NetworkState::status == _network_state_server)
       )
    {
        unsigned char game_type;
        game_type = GameConfig::game_gametype;

        switch( game_type )
        {
        case  _gametype_timelimit:
        {
            int game_minutes = GameManager::getGameTime() / 60;
            if( game_minutes >= GameConfig::game_timelimit )
            {
                onTimelimitGameCompleted();
            }
            break;
        }
        case _gametype_fraglimit:
            if (GameConfig::game_teammode)
            {
                if (TeamManager::testRuleScoreLimit( GameConfig::game_fraglimit ))
                    onFraglimitGameCompleted();
                int game_minutes = GameManager::getGameTime() / 60;
                if( game_minutes >= GameConfig::game_timelimit )
                    onTimelimitGameCompleted();
            }
            else
            {
                if ( PlayerInterface::testRuleScoreLimit( GameConfig::game_fraglimit, &player_state ) == true )
                    onFraglimitGameCompleted();
            }
            break;

        case _gametype_objective:
        {
            float ratio = (float) GameConfig::game_occupationpercentage / 100.0;


            if (GameConfig::game_teammode)
            {
                if (TeamManager::testRuleObjectiveRatio( ratio ))
                    onObjectiveGameCompleted();
                int game_minutes = GameManager::getGameTime() / 60;
                if( game_minutes >= GameConfig::game_timelimit )
                    onTimelimitGameCompleted();
            }
            else
            {
                if (PlayerInterface::testRuleObjectiveRatio( ratio, &player_state))
                {
                    onObjectiveGameCompleted( );
                }
            }
            break;
        }
        default:
            // nothing
            ;
        }

        // ** Check for Player Respawns **
        bool respawn_rule_complete = false;
        while( respawn_rule_complete == false )
        {
            if ( PlayerInterface::testRulePlayerRespawn( &respawn_rule_complete, &player_state ) )
            {
                PlayerInterface::resetPlayerUnitConfig( player_state->getID() );
                GameManager::spawnPlayer( player_state->getID() );
            }
        }
    }
}

void GameControlRulesDaemon::netMessageCycleMap(const NetMessage* message)
{
    GameControlCycleMap *cycle_map_mesg;

    cycle_map_mesg = (GameControlCycleMap *) message;

    snprintf(map_cycle_fsm_client_map_name, 256, "%s", cycle_map_mesg->map_name);
    map_cycle_fsm_client_state = _map_cycle_client_start_map_load;

}

void GameControlRulesDaemon::netMessageCycleRespawnAck(const NetMessage* )
{
    map_cycle_fsm_client_respawn_ack_flag = true;
}

void GameControlRulesDaemon::netMessageCyclePrepareTeam(const NetMessage* message)
{
    GameControlCyclePrepareTeam *prepare_team_mesg;
    prepare_team_mesg = (GameControlCyclePrepareTeam *) message;

    map_cycle_fsm_client_state = _map_cycle_client_prepare_team;
    GameControlRulesDaemon::game_state = _game_state_prepare_team;
    cooldown.changePeriod(prepare_team_mesg->Cooldown);
}

void GameControlRulesDaemon::netMessageCycleTeamStart(const NetMessage* )
{
    map_cycle_fsm_client_state = _map_cycle_client_team_start;
    GameControlRulesDaemon::game_state = _game_state_idle;
}

void GameControlRulesDaemon::processNetMessage(const NetMessage* message)
{
    switch(message->message_id)
    {
    case _net_message_id_game_control_cycle_map :
        netMessageCycleMap(message);
        break;

    case _net_message_id_game_control_cycle_respawn_ack :
        netMessageCycleRespawnAck(message);
        break;

    case _net_message_id_game_control_cycle_prepare_team :
        netMessageCyclePrepareTeam(message);
        break;

    case _net_message_id_game_control_cycle_team_start :
        netMessageCycleTeamStart(message);
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
    if ( NetworkState::status == _network_state_server )
    {
        mapCycleFsmServer();
    }
    else
    {
        mapCycleFsmClient();
    }

    checkGameRules();
}

void GameControlRulesDaemon::mapLoadFailureResponse(int result_code, const char *map_name)
{
    char str_buf[128];

    if( result_code == _mapload_result_no_map_file )
    {
        sprintf( str_buf, _("MAP %s NOT FOUND!"), map_name );
        LoadingView::append( str_buf);
    }
    else if( result_code == _mapload_result_no_wad_file )
    {
        LoadingView::append( _("MAP TILE SET NOT FOUND!") );
    }
}



