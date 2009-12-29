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

#include "Core/GlobalGameState.hpp"
#include "Core/GlobalEngineState.hpp"
#include "Core/NetworkGlobals.hpp"

#include "Util/NTimer.hpp"
#include "Interfaces/GameControlRulesDaemon.hpp"
#include "GameManager.hpp"
#include "BaseGameManager.hpp"

#include "Interfaces/GameManager.hpp"
#include "Interfaces/GameConfig.hpp"
#include "Interfaces/PlayerInterface.hpp"
#include "Objectives/ObjectiveInterface.hpp"
#include "Interfaces/MapsManager.hpp"
#include "Interfaces/ConsoleInterface.hpp"
#include "Interfaces/ChatInterface.hpp"

#include "Weapons/ProjectileInterface.hpp"
#include "PowerUps/PowerUpInterface.hpp"
#include "Interfaces/PathScheduler.hpp"
#include "Particles/Physics.hpp"
#include "Particles/ParticleSystem2D.hpp"
#include "Bot/BotManager.hpp"

#include "Classes/Network/NetworkState.hpp"
#include "Classes/Network/SystemNetMessage.hpp"
#include "Classes/Network/GameControlNetMessage.hpp"
#include "Classes/Network/NetworkServer.hpp"
#include "Classes/Network/ServerConnectDaemon.hpp"
#include "Classes/Network/ConnectNetMessage.hpp"
#include "Classes/Network/ClientServerNetMessage.hpp"
#include "Classes/Network/PlayerNetMessage.hpp"

#include "Classes/Network/NetworkClient.hpp"
#include "Network/MessageRouter.hpp"

#include "Views/Components/Desktop.hpp"
#include "Views/Game/LoadingView.hpp"

#include "Util/Log.hpp"

enum { _map_cycle_server_state_idle,
       _map_cycle_server_state_display_endgame_views,
       _map_cycle_server_state_cycle_next_map,
       _map_cycle_server_state_load_map,
       _map_cycle_server_state_wait_for_client_map_load,
       _map_cycle_server_state_respawn_players,
       _map_cycle_server_state_in_progress
     };

enum { _map_cycle_client_idle,
       _map_cycle_client_connecting_to_server,
       _map_cycle_client_wait_connection,
       _map_cycle_client_wait_link_ack,
       _map_cycle_client_wait_connect_start,
       _map_cycle_client_wait_connect_result,
       _map_cycle_client_wait_game_setup,
       _map_cycle_client_start_map_load,
       _map_cycle_client_load_map,
       _map_cycle_client_wait_for_resetgamelogic_or_connectid,
       _map_cycle_client_wait_complete_connect,
       _map_cycle_client_wait_for_respawn_ack,
       _map_cycle_client_state_in_progress
   };

enum { _execution_mode_loop_back_server,
       _execution_mode_dedicated_server
     };

int GameControlRulesDaemon::execution_mode = _execution_mode_loop_back_server;
std::string GameControlRulesDaemon::nextmap = "";
NTimer GameControlRulesDaemon::respawntimer;

#define _MAP_CYCLE_ENDGAME_WAIT_PERIOD  (20) // seconds
#define _MAP_CYCLE_MAP_LOAD_WAIT_PERIOD (7) // seconds

int GameControlRulesDaemon::map_cycle_fsm_server_state = _map_cycle_server_state_idle;
Timer GameControlRulesDaemon::map_cycle_fsm_server_endgame_timer;
Timer GameControlRulesDaemon::map_cycle_fsm_server_map_load_timer;

int GameControlRulesDaemon::map_cycle_fsm_client_state = _map_cycle_client_idle;
bool GameControlRulesDaemon::map_cycle_fsm_client_respawn_ack_flag = false;

static std::string connecting_server_name;

//-----------------------------------------------------------------
void GameControlRulesDaemon::setStateServerLoadingMap()
{
    map_cycle_fsm_server_endgame_timer.changePeriod( 0.0f );
    GameControlRulesDaemon::map_cycle_fsm_server_state = _map_cycle_server_state_cycle_next_map;
}
//-----------------------------------------------------------------
void GameControlRulesDaemon::setStateClientLoadingMap()
{
    GameControlRulesDaemon::map_cycle_fsm_client_state = _map_cycle_client_start_map_load;
}
void GameControlRulesDaemon::setStateClientConnectToServer(const std::string& server_name)
{
    connecting_server_name = server_name;
    GameControlRulesDaemon::map_cycle_fsm_client_state = _map_cycle_client_connecting_to_server;
}
//-----------------------------------------------------------------
void GameControlRulesDaemon::setStateServerIdle()
{
    GameControlRulesDaemon::map_cycle_fsm_server_state = _map_cycle_server_state_idle;
}
//-----------------------------------------------------------------
void GameControlRulesDaemon::setDedicatedServer()
{
    GameControlRulesDaemon::execution_mode = _execution_mode_dedicated_server;
}

//-----------------------------------------------------------------
void GameControlRulesDaemon::mapCycleFsmClient()
{
    bool loop_finished = false;
    while ( ! loop_finished )
    {
        loop_finished = true;
    switch( map_cycle_fsm_client_state )
    {
        case _map_cycle_client_idle :
            return;

        case _map_cycle_client_connecting_to_server:
            LoadingView::show();
            LoadingView::append("Starting network connection...");
            NetworkState::setNetworkStatus( _network_state_client );
            MessageRouter::initialize(false);
            NetworkClient::joinServer(connecting_server_name);
            map_cycle_fsm_client_state = _map_cycle_client_wait_connection;
            break;

        case _map_cycle_client_wait_connection:
            {
                NetPacket np;
                if ( MessageRouter::getNextPacket(np) )
                {
                    const NetMessage * msg = np.getNetMessage();
                    if (  msg->message_class == _net_message_class_client_server
                        && msg->message_id == _net_message_id_client_connect_ack
                       )
                    {
                        LoadingView::append("Sending join request...");
                        ClientConnectJoinRequest join_request;
                        join_request.setProtocolVersion(NETPANZER_PROTOCOL_VERSION);
                        NetworkClient::sendMessage(&join_request, sizeof(join_request));
                        map_cycle_fsm_client_state = _map_cycle_client_wait_link_ack;
                    }
                    else
                    {
                        LoadingView::append("Error in wait_connection");
                        // TODO fail here if message is not expected
                    }
                }
            }
            break;

        case _map_cycle_client_wait_link_ack:
            {
                NetPacket np;
                if ( MessageRouter::getNextPacket(np) )
                {
                    const ClientConnectJoinRequestAck * msg = (ClientConnectJoinRequestAck*)np.getNetMessage();
                    if (  msg->message_class == _net_message_class_connect
                        && msg->message_id == _net_message_id_connect_join_game_request_ack
                       )
                    {
                        switch ( msg->getResultCode() )
                        {
                            case _join_request_result_success:
                                LoadingView::append("Link to Server Established");
                                loop_finished = false;
                                map_cycle_fsm_client_state = _map_cycle_client_wait_connect_start;
                                break;

                            case _join_request_result_invalid_protocol:
                                // TODO fail here
                                break;

                            case _join_request_result_server_busy:
                                // TODO fail here
                                break;

                            default:
                                // TODO fail here if message is not expected
                                ;
                        }
                    }
                    else
                    {
                        LoadingView::append("Error in wait_link_ack");
                        // TODO fail here if message is not expected
                    }
                }
            }
            break;

        case _map_cycle_client_wait_connect_start:
            {
                NetPacket np;
                if ( MessageRouter::getNextPacket(np) )
                {
                    const NetMessage * msg = np.getNetMessage();
                    if (  msg->message_class == _net_message_class_connect
                        && msg->message_id == _net_message_id_client_start_connect
                       )
                    {
                        LoadingView::append("Starting connection ...");
                        ClientConnectRequest connect_request;
                        NetworkClient::sendMessage(&connect_request, sizeof(connect_request));
                        loop_finished = false;
                        map_cycle_fsm_client_state = _map_cycle_client_wait_connect_result;
                    }
                    else
                    {
                        // TODO fail here if message is not expected
                    }
                }
            }
            break;

        case _map_cycle_client_wait_connect_result:
            {
                NetPacket np;
                if ( MessageRouter::getNextPacket(np) )
                {
                    const ClientConnectResult * msg = (ClientConnectResult*)np.getNetMessage();
                    if (  msg->message_class == _net_message_class_connect
                        && msg->message_id == _net_message_id_client_connect_result
                       )
                    {
                         if ( msg->result_code == _connect_result_success )
                        {
                            LoadingView::append("Sending client settings...");
                            ConnectClientSettings client_setting;

                            client_setting.set(gameconfig->playername.c_str(),
                                               gameconfig->getUnitColor(),
                                               gameconfig->playerflag.c_str());

                            NetworkClient::sendMessage(&client_setting, sizeof(client_setting));
                            loop_finished = false;
                            map_cycle_fsm_client_state = _map_cycle_client_wait_game_setup;
                        }
                        else
                        {
                            // TODO fail here
                        }
                    }
                    else
                    {
                        // TODO fail here if message is not expected
                    }
                }
            }
            break;

        case _map_cycle_client_wait_game_setup:
            {
                NetPacket np;
                if ( MessageRouter::getNextPacket(np) )
                {
                    const ConnectMesgServerGameSettings * msg = (ConnectMesgServerGameSettings*)np.getNetMessage();
                    if (  msg->message_class == _net_message_class_connect
                        && msg->message_id == _net_message_id_connect_server_game_setup
                       )
                    {
                         LoadingView::append("Received server settings...");
                        gameconfig->maxplayers = msg->getMaxPlayers();
                        gameconfig->maxunits = msg->getMaxUnits();
                        gameconfig->cloudcoverage = msg->getCloudCoverage();
                        gameconfig->windspeed = (int)msg->getWindSpeed();
                        gameconfig->powerups = msg->powerup_state;
                        gameconfig->gametype = msg->getGameType();
                        gameconfig->fraglimit = msg->getFragLimit();
                        gameconfig->timelimit = msg->getTimeLimit();

                        global_engine_state->game_manager->setElapsetTimeOffset(msg->getElapsedTime());
                        gameconfig->map = msg->map_name;

                        ConnectMesgClientGameSetupPing ping;
                        NetworkClient::sendMessage(&ping, sizeof(ping));
                        loop_finished = false;
                        map_cycle_fsm_client_state = _map_cycle_client_start_map_load;
                    }
                    else
                    {
                        // TODO fail here if message is not expected
                    }
                }
            }
            break;

        case _map_cycle_client_start_map_load : {
                LoadingView::show();

                GameManager::shutdownParticleSystems();
                ObjectiveInterface::resetLogic();

                char buf[256];
                snprintf(buf, sizeof(buf), "Next Map '%s'.",
                        gameconfig->map.c_str());
                LoadingView::append( buf);
                LoadingView::append( "Loading Game Map ..." );

                try {
                    GameManager::loadGameMap(gameconfig->map.c_str());
                } catch(std::exception& e) {
                    LoadingView::append("Error while loading map:");
                    LoadingView::append(e.what());
                    map_cycle_fsm_client_state = _map_cycle_client_idle;
                    return;
                }
                
                global_engine_state->game_manager->resetGameLogic();
                loop_finished = false;
                map_cycle_fsm_client_state = _map_cycle_client_load_map;
            }
            break;

        case _map_cycle_client_load_map:
        {
//                int percent_complete;
//                char str_buf[128];
            LoadingView::append("Map loaded, sending ack...");

//                if ( GameManager::gameMapLoad( &percent_complete ) == false ) {
//
//                    sprintf( str_buf, "Loading Game Map ... (%d%%)", percent_complete);
//                    LoadingView::update( str_buf );
//
//                    LoadingView::append( "Waiting to respawn ..." );
//                } else {
//                    sprintf( str_buf, "Loading Game Map ... (%d%%)", percent_complete);
//                    LoadingView::update( str_buf );
//                }
            ConnectMesgClientGameSetupAck ack;
            NetworkClient::sendMessage( &ack, sizeof(ack));
            loop_finished = false;
            map_cycle_fsm_client_state = _map_cycle_client_wait_for_resetgamelogic_or_connectid;
            break;
        }

        case _map_cycle_client_wait_for_resetgamelogic_or_connectid:
            {
                NetPacket np;
                if ( MessageRouter::getNextPacket(np) )
                {
                    switch ( np.getNetMessage()->message_class )
                    {
                        case _net_message_class_player:
                            if ( np.getNetMessage()->message_id == _net_message_id_player_connect_id )
                            {
                                global_engine_state->game_manager->reinitializeGameLogic();
                                LoadingView::append("Received my id...");
                                MessageRouter::routePacket(np);
                                loop_finished = false;
                                map_cycle_fsm_client_state = _map_cycle_client_wait_complete_connect;
                            }
                            else
                            {
                                // TODO fail here
                            }
                            break;

                        case _net_message_class_system:
                            if ( np.getNetMessage()->message_id == _net_message_id_system_reset_game_logic )
                            {
                                LoadingView::append("Resetting game logic...");
                                global_engine_state->game_manager->resetGameLogic();
                                loop_finished = false;
                                map_cycle_fsm_client_state = _map_cycle_client_wait_for_respawn_ack;
                            }
                            else
                            {
                                // TODO fail here
                            }
                            break;
                        default:
                            char buf[1024];
                            snprintf(buf,sizeof(buf), "Received unknown packet: %d:%d",
                                        np.getNetMessage()->message_class,
                                        np.getNetMessage()->message_id);
                            LoadingView::append(buf);
                            // TODO fail here if message is not expected
                            ;
                    }

                }
            }
            break;

        case _map_cycle_client_wait_for_respawn_ack:
            if( map_cycle_fsm_client_respawn_ack_flag == true )
            {
                LoadingView::loadFinish();
                map_cycle_fsm_client_respawn_ack_flag = false;
                map_cycle_fsm_client_state = _map_cycle_client_state_in_progress;
            }
            break;

        case _map_cycle_client_wait_complete_connect:
            {
                NetPacket np;
                bool in_loop = true;
                while ( in_loop &&  MessageRouter::getNextPacket(np) )
                {
                    if ( np.getNetMessage()->message_class == _net_message_class_connect
                        && np.getNetMessage()->message_id == _net_message_id_client_connect_process_state_mesg
                        && ((ConnectProcessStateMessage*)np.getNetMessage())->getMessageEnum() == _connect_state_sync_complete
                       )
                    {
                        map_cycle_fsm_client_state = _map_cycle_client_state_in_progress;
                    }

                    MessageRouter::routePacket(np);
                }
            }
            break;
        case _map_cycle_client_state_in_progress:
            UnitInterface::updateUnitStatus();

            ProjectileInterface::updateStatus();
            ObjectiveInterface::updateObjectiveStatus();
            PowerUpInterface::updateState();
            PathScheduler::run();

            Physics::sim();

            ParticleSystem2D::simAll();
            Particle2D::simAll();
            break;
    } // ** switch

    }
}


void GameControlRulesDaemon::mapCycleFsmServer()
{
    switch ( map_cycle_fsm_server_state )
    {
        case _map_cycle_server_state_idle:
            break;

        case _map_cycle_server_state_display_endgame_views:
            {
                ChatInterface::serversay("Round is over");
                                                                
                SystemViewControl view_control;

                ServerConnectDaemon::lockConnectProcess();

                view_control.set("RankView", _view_control_flag_visible_on | _view_control_flag_close_all );

                if ( GameControlRulesDaemon::execution_mode == _execution_mode_loop_back_server ) {
                    Desktop::setVisibility("GameView", true);
                    Desktop::setVisibility("RankView", true );
                }

                NetworkServer::broadcastMessage(&view_control, sizeof(SystemViewControl));

                map_cycle_fsm_server_endgame_timer.changePeriod( _MAP_CYCLE_ENDGAME_WAIT_PERIOD );

                map_cycle_fsm_server_state = _map_cycle_server_state_cycle_next_map;
            }
            break;

        case _map_cycle_server_state_cycle_next_map:
            {
                if ( map_cycle_fsm_server_endgame_timer.count() &&
                        (ServerConnectDaemon::isConnecting() == false)
                   )
                {
                    GameManager::shutdownParticleSystems();

                    if(nextmap != "") {
                        gameconfig->map = nextmap;
                        nextmap = "";
                    } else {
                        gameconfig->map 
                            = MapsManager::getNextMap(gameconfig->map);
                    }
                    
                    ConsoleInterface::postMessage(Color::white, false, 0, "loading map '%s'.",
                            gameconfig->map.c_str());

                    GameControlCycleMap cycle_map_mesg;
                    cycle_map_mesg.set( gameconfig->map.c_str() );

                    NetworkServer::broadcastMessage( &cycle_map_mesg, sizeof( GameControlCycleMap ));

                    ObjectiveInterface::resetLogic();

                    if ( GameControlRulesDaemon::execution_mode == _execution_mode_dedicated_server )
                    {
                        GameManager::dedicatedLoadGameMap(gameconfig->map.c_str());

                        map_cycle_fsm_server_map_load_timer.changePeriod(_MAP_CYCLE_MAP_LOAD_WAIT_PERIOD);
                        map_cycle_fsm_server_map_load_timer.reset();
                        map_cycle_fsm_server_state = _map_cycle_server_state_wait_for_client_map_load;
                    } else {
                        LoadingView::show();

                        LoadingView::append( "Loading Game Map ..." );

                        try {
                            GameManager::loadGameMap(gameconfig->map.c_str());
                        } catch(std::exception& e) {
                            LoadingView::append(
                                    "Error while loading map:");
                            LoadingView::append(e.what());
                            map_cycle_fsm_server_state = _map_cycle_server_state_idle;
                            return;
                        }
                        
                        map_cycle_fsm_server_state = _map_cycle_server_state_load_map;
                    }
                }
            }
            break;

        case _map_cycle_server_state_load_map : {
//                int percent_complete;
//                char str_buf[128];

//                if ( GameManager::gameMapLoad( &percent_complete ) == false ) {
                    map_cycle_fsm_server_state = _map_cycle_server_state_respawn_players;
//                }
//
//                sprintf( str_buf, "Loading Game Map ... (%d%%)", percent_complete);
//                LoadingView::update( str_buf );
            }
            break;

        case _map_cycle_server_state_wait_for_client_map_load :
            {
                if ( map_cycle_fsm_server_map_load_timer.count() )
                {
                    ConsoleInterface::postMessage(Color::white, false, 0, "Game started.");
                    map_cycle_fsm_server_state = _map_cycle_server_state_respawn_players;
                }
            }
            break;

        case _map_cycle_server_state_respawn_players :
            {
                if ( ! UnitInterface::getUnits() )
                {
                    ConsoleInterface::postMessage(Color::white, false, 0, "Reinitializing game logic.");
                    global_engine_state->game_manager->reinitializeGameLogic();
                    if ( GameControlRulesDaemon::execution_mode == _execution_mode_loop_back_server )
                    {
                        PlayerState * player_state = PlayerInterface::allocateLoopBackPlayer();
                        player_state->setName(gameconfig->playername.c_str());
                        ResourceManager::loadDefaultFlags();
                        Uint8 flagdata[20*14]; // XXX shouldn't use fixed values
                        ResourceManager::getFlagData(gameconfig->playerflag.c_str(), (Uint8 *)&flagdata);
                        player_state->setFlag(ResourceManager::registerFlagFromData(flagdata));
                    }
                }
                else
                {
                    ConsoleInterface::postMessage(Color::white, false, 0, "Resetting game logic.");
                    SystemResetGameLogic reset_game_logic_mesg;
                    NetworkServer::broadcastMessage( &reset_game_logic_mesg, sizeof(SystemResetGameLogic));
                    global_engine_state->game_manager->resetGameLogic();
                }

                GameManager::respawnAllPlayers();

                PlayerInterface::unlockPlayerStats();

                LoadingView::loadFinish();

                GameControlCycleRespawnAck respawn_ack_mesg;
                NetworkServer::broadcastMessage( &respawn_ack_mesg, sizeof(GameControlCycleRespawnAck));

                map_cycle_fsm_server_state = _map_cycle_server_state_in_progress;

                ServerConnectDaemon::unlockConnectProcess();
            }
            break;

        case _map_cycle_server_state_in_progress:
            UnitInterface::updateUnitStatus();

            ProjectileInterface::updateStatus();
            ObjectiveInterface::updateObjectiveStatus();
            PowerUpInterface::updateState();
            PathScheduler::run();

            Physics::sim();

            ParticleSystem2D::simAll();
            Particle2D::simAll();

            BotManager::simBots();

            checkGameRules();
            break;

    } // ** switch
}

void GameControlRulesDaemon::onTimelimitGameCompleted()
{
    PlayerInterface::lockPlayerStats();

    map_cycle_fsm_server_state = _map_cycle_server_state_display_endgame_views;
}

void GameControlRulesDaemon::onFraglimitGameCompleted()
{
    PlayerInterface::lockPlayerStats();

    map_cycle_fsm_server_state = _map_cycle_server_state_display_endgame_views;
}

void GameControlRulesDaemon::onObjectiveGameCompleted()
{
    PlayerInterface::lockPlayerStats();

    map_cycle_fsm_server_state = _map_cycle_server_state_display_endgame_views;
}

void GameControlRulesDaemon::forceMapChange(std::string _nextmap)
{
    nextmap = _nextmap;

    PlayerInterface::lockPlayerStats();

    map_cycle_fsm_server_state = _map_cycle_server_state_display_endgame_views;
}

void GameControlRulesDaemon::checkGameRules()
{
    PlayerState *player_state;

    unsigned char game_type;
    game_type = gameconfig->gametype;

    switch( game_type )
    {
        case  _gametype_timelimit:
        {
            int game_minutes = global_engine_state->game_manager->getGameTime() / 60;
            if( game_minutes >= gameconfig->timelimit )
            {
                onTimelimitGameCompleted();
            }
            break;
        }
        case _gametype_fraglimit:
            if ( PlayerInterface::testRuleScoreLimit( gameconfig->fraglimit, &player_state ) == true )
            {
                onFraglimitGameCompleted();
            }
            break;

        case _gametype_objective:
        {
            float ratio = (float) gameconfig->objectiveoccupationpercentage / 100.0;
            if (PlayerInterface::testRuleObjectiveRatio( ratio, &player_state))
            {
                onObjectiveGameCompleted( );
            }
            break;
        }
        default:
            // nothing
            ;
    }

    checkRespawn();
}

void
GameControlRulesDaemon::checkRespawn()
{
    bool doRespawn = false;

    switch ( gameconfig->respawnmode )
    {
        case 1: // round mode
        {
            unsigned short players_alive = 0;
            for ( unsigned short player = 0; player < PlayerInterface::getMaxPlayers(); player++ )
            {
                if ( PlayerInterface::getPlayer(player)->getStatus() == _player_state_active
                     && UnitInterface::getPlayerUnitCount( player ) > 0 )
                {
                    players_alive++;
                }
            }
            
            if ( players_alive < 2 )
            {
                doRespawn = true;
            }
            break;
        }
        case 2: // timer mode
            respawntimer.setTimeOut(int(gameconfig->respawntime) * 1000);
            if ( respawntimer.isTimeOut() )
            {
                doRespawn = true;
                respawntimer.reset();
            }
            break;
        case 0: // normal mode
        default:
            doRespawn = true;
    }

    if ( doRespawn )
    {
        for ( unsigned short player = 0; player < PlayerInterface::getMaxPlayers(); player++ )
        {
            if ( PlayerInterface::getPlayer(player)->getStatus() == _player_state_active
                 && UnitInterface::getPlayerUnitCount( player ) == 0 )
            {
                GameManager::spawnPlayer(player);
            }
        }
        respawntimer.reset();
    }

}

void GameControlRulesDaemon::netMessageCycleMap(const NetMessage* message)
{
    GameControlCycleMap *cycle_map_mesg;
    cycle_map_mesg = (GameControlCycleMap *) message;
    gameconfig->map = cycle_map_mesg->map_name;

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
}

void GameControlRulesDaemon::mapLoadFailureResponse(int result_code, const char *map_name)
{
    char str_buf[128];

    if( result_code == _mapload_result_no_map_file ) {
        sprintf( str_buf, "MAP %s NOT FOUND!", map_name );
        LoadingView::append( str_buf);
    } else if( result_code == _mapload_result_no_wad_file ) {
            LoadingView::append( "MAP TILE SET NOT FOUND!" );
    }
}
