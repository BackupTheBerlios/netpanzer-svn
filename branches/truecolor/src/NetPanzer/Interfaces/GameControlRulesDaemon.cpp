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

#include "Interfaces/Console.hpp"
#include "Particles/ParticleInterface.hpp"
#include "Resources/ResourceManager.hpp"

#include "Scripts/ScriptManager.hpp"

enum
{
    _fsm_state_idle,

    _fsm_state_server_display_endgame_views,
    _fsm_state_server_waiting_to_switch_map,

    _fsm_state_prepare_map_load,
    _fsm_state_load_map,
    _fsm_state_load_tileset,
    _fsm_state_init_particles,

    _fsm_state_server_waiting_clients_load_map,
    _fsm_state_server_respawn_players,

    _fsm_state_connecting_to_server,
    _fsm_state_connecting_to_server_work,
    _fsm_state_waiting_connection,
    _fsm_state_waiting_link_ack,
    _fsm_state_waiting_connect_start,
    _fsm_state_waiting_connect_result,
    _fsm_state_waiting_game_setup,
    _fsm_state_send_gamesetup_ack,
    _fsm_state_waiting_resetgamelogic_or_connectid,
    _fsm_state_waiting_complete_connect,
    _fsm_state_waiting_respawn_ack,

    _fsm_state_game_in_progress

};

enum
{
    _execution_mode_loop_back_server,
    _execution_mode_dedicated_server
};

static int fsm_state = _fsm_state_idle;

int GameControlRulesDaemon::execution_mode = _execution_mode_loop_back_server;
std::string GameControlRulesDaemon::nextmap = "";
NTimer GameControlRulesDaemon::respawntimer;

#define _MAP_CYCLE_ENDGAME_WAIT_PERIOD  (20) // seconds
#define _MAP_CYCLE_MAP_LOAD_WAIT_PERIOD (7) // seconds

Timer GameControlRulesDaemon::map_cycle_fsm_server_endgame_timer;
Timer GameControlRulesDaemon::map_cycle_fsm_server_map_load_timer;

bool GameControlRulesDaemon::map_cycle_fsm_client_respawn_ack_flag = false;

static std::string connecting_server_name;

//-----------------------------------------------------------------
void GameControlRulesDaemon::setStateIdle()
{
    fsm_state = _fsm_state_idle;
}
//-----------------------------------------------------------------
void GameControlRulesDaemon::setStateServerLoadingMap()
{
    map_cycle_fsm_server_endgame_timer.changePeriod( 0.0f );
    fsm_state = _fsm_state_server_waiting_to_switch_map;
}
//-----------------------------------------------------------------
void GameControlRulesDaemon::setStateClientConnectToServer(const std::string& server_name)
{
    connecting_server_name = server_name;
    fsm_state = _fsm_state_connecting_to_server;
}
//-----------------------------------------------------------------
void GameControlRulesDaemon::setDedicatedServer()
{
    GameControlRulesDaemon::execution_mode = _execution_mode_dedicated_server;
}
unsigned char GameControlRulesDaemon::getGameState()
{
    return fsm_state;
}

//-----------------------------------------------------------------
void GameControlRulesDaemon::updateGameControlFlow()
{
    switch( fsm_state )
    {
        case _fsm_state_idle:
            return;

        case _fsm_state_connecting_to_server:
            ScriptManager::runFile("user_commands_load","scripts/usercommands.lua");
            LoadingView::show();
            LoadingView::append("Starting network connection...");
            fsm_state = _fsm_state_connecting_to_server_work;
            break;

        case _fsm_state_connecting_to_server_work:
            NetworkState::setNetworkStatus( _network_state_client );
            MessageRouter::initialize(false);
            NetworkClient::joinServer(connecting_server_name);
            fsm_state = _fsm_state_waiting_connection;
            break;

        case _fsm_state_waiting_connection:
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
                    fsm_state = _fsm_state_waiting_link_ack;
                }
                else
                {
                    LoadingView::append("Error in wait_connection");
                    // TODO fail here if message is not expected
                }
            }
        }
            break;

        case _fsm_state_waiting_link_ack:
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
                            LoadingView::append("Join successfull");
                            fsm_state = _fsm_state_waiting_connect_start;
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

        case _fsm_state_waiting_connect_start:
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
                    fsm_state = _fsm_state_waiting_connect_result;
                }
                else
                {
                    LoadingView::append("Wrong connection packet received when waiting for start");
                    // TODO fail here if message is not expected
                }
            }
        }
            break;

        case _fsm_state_waiting_connect_result:
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
                        fsm_state = _fsm_state_waiting_game_setup;
                    }
                    else
                    {
                         LoadingView::append("Connection result failed");
                        // TODO fail here
                    }
                }
                else
                {
                    LoadingView::append("Wrong connection packet when waiting for connection result");
                    // TODO fail here if message is not expected
                }
            }
        }
            break;

        case _fsm_state_waiting_game_setup:
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
                    fsm_state = _fsm_state_prepare_map_load;
                }
                else
                {
                    LoadingView::append("Wrong connection packet when waiting server settings");
                    // TODO fail here if message is not expected
                }
            }
        }
            break;

        case _fsm_state_server_display_endgame_views:
            {
                ChatInterface::serversay("Round is over");

                SystemViewControl view_control;

                ServerConnectDaemon::lockConnectProcess();

                view_control.set("RankView", _view_control_flag_visible_on | _view_control_flag_close_all );

                if ( execution_mode == _execution_mode_loop_back_server ) {
                    Desktop::setVisibility("GameView", true);
                    Desktop::setVisibility("RankView", true );
                }

                NetworkServer::broadcastMessage(&view_control, sizeof(SystemViewControl));

                map_cycle_fsm_server_endgame_timer.changePeriod( _MAP_CYCLE_ENDGAME_WAIT_PERIOD );

                fsm_state = _fsm_state_server_waiting_to_switch_map;
            }
            break;

        case _fsm_state_server_waiting_to_switch_map:
            MessageRouter::routePackets();
            Physics::sim();

            if ( execution_mode != _execution_mode_dedicated_server )
            {
                ParticleSystem2D::simAll();
                Particle2D::simAll();
            }

            if (   map_cycle_fsm_server_endgame_timer.count()
                && ServerConnectDaemon::isConnecting() == false )
            {
                ScriptManager::runFile("server_commands_load","scripts/servercommands.lua");
                ScriptManager::runFile("user_commands_load","scripts/usercommands.lua");

                if ( nextmap != "" )
                {
                    gameconfig->map = nextmap;
                    nextmap = "";
                }
                else
                {
                    gameconfig->map = MapsManager::getNextMap(gameconfig->map);
                }

                fsm_state = _fsm_state_prepare_map_load;
            }
            break;

        case _fsm_state_prepare_map_load:
            GameManager::shutdownParticleSystems();
            ObjectiveInterface::resetLogic();

            if ( execution_mode == _execution_mode_dedicated_server )
            {
                ConsoleInterface::postMessage(Color::white, false, 0,
                        "loading map '%s'.", gameconfig->map.c_str());
            }
            else
            {
                LoadingView::show();

                char buf[256];
                snprintf(buf, sizeof(buf), "Next Map '%s'.",
                                           gameconfig->map.c_str());
                LoadingView::append( buf);
                LoadingView::append( "Loading Game Map ..." );
            }

            if ( NetworkState::getNetworkStatus() == _network_state_server )
            {
                GameControlCycleMap cycle_map_mesg;
                cycle_map_mesg.set( gameconfig->map.c_str() );

                NetworkServer::broadcastMessage(&cycle_map_mesg,
                                                sizeof(cycle_map_mesg));

            }
            
            fsm_state = _fsm_state_load_map;
            break;

        case _fsm_state_load_map:
            try
            {
                GameManager::loadGameMap(gameconfig->map.c_str());
            }
            catch(std::exception& e)
            { // XXX Handle correctly for both client and server.
                LoadingView::append("Error while loading map:");
                LoadingView::append(e.what());
                fsm_state = _fsm_state_idle;
                return;
            }
            
            if ( execution_mode == _execution_mode_dedicated_server )
            {
                map_cycle_fsm_server_map_load_timer.changePeriod(_MAP_CYCLE_MAP_LOAD_WAIT_PERIOD);
                map_cycle_fsm_server_map_load_timer.reset();

                ParticleInterface::initParticleSystems();
                Particle2D::setCreateParticles(false);

                Console::mapSwitch(gameconfig->map);
                *Console::server << "Server Settings:\n"
                    << "Map: " << gameconfig->map << "\n"
                    << "MaxPlayers: " << gameconfig->maxplayers << "\n"
                    << "MaxUnits: " << gameconfig->maxunits << "\n"
                    << "Gametype: " << gameconfig->getGameTypeString() << "\n"
                    << "ObjectivePercentage: " <<
                        gameconfig->objectiveoccupationpercentage << "\n"
                    << "TimeLimit: " << gameconfig->timelimit << "\n"
                    << "FragLimit: " << gameconfig->fraglimit << "\n"
                    << "RespawnType: " << gameconfig->getRespawnTypeString() << "\n"
                    << "Mapcycle: " << gameconfig->mapcycle << "\n"
                    << "Powerups: " << (gameconfig->powerups ? "yes" : "no") << "\n"
                    << "AllowAllies: " << (gameconfig->allowallies ? "yes" : "no") << "\n"
                    << "CloudCoverage: " << gameconfig->cloudcoverage << " (Windspeed "
                                         << gameconfig->windspeed << ")" << std::endl;

                fsm_state = _fsm_state_server_waiting_clients_load_map;
            }
            else
            {

                if ( ! TileSet::tileImagesLoaded() )
                {
                    LoadingView::append("Loading TileSet...");
                    fsm_state = _fsm_state_load_tileset;
                }
                else
                {
                    LoadingView::append("Initializing particles...");
                    fsm_state = _fsm_state_init_particles;
                }
            }
            break;

        case _fsm_state_load_tileset:
            TileSet::loadImages();
            LoadingView::append("Initializing particles...");
            fsm_state = _fsm_state_init_particles;
            break;

        case _fsm_state_init_particles:
            ParticleInterface::initParticleSystems();
            ParticleInterface::addCloudParticle(gameconfig->cloudcoverage);
            Physics::wind.setVelocity(gameconfig->windspeed, 107);

            if ( NetworkState::getNetworkStatus() == _network_state_server )
            {
                LoadingView::append("Data loaded, respawning players...");
                fsm_state = _fsm_state_server_respawn_players;
            }
            else
            {
                LoadingView::append("Data loaded, sending ack...");
//                global_engine_state->game_manager->resetGameLogic();
                fsm_state = _fsm_state_send_gamesetup_ack;
            }
            break;

        case _fsm_state_server_waiting_clients_load_map :
        {
            MessageRouter::routePackets();
            if ( map_cycle_fsm_server_map_load_timer.count() )
            {
                ConsoleInterface::postMessage(Color::white, false, 0, "Game started.");
                fsm_state = _fsm_state_server_respawn_players;
            }
        }
            break;

        case _fsm_state_send_gamesetup_ack:
        {
            ConnectMesgClientGameSetupAck ack;
            NetworkClient::sendMessage( &ack, sizeof(ack));
            fsm_state = _fsm_state_waiting_resetgamelogic_or_connectid;
        }
            break;


        case _fsm_state_waiting_resetgamelogic_or_connectid:
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
                            fsm_state = _fsm_state_waiting_complete_connect;
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
                            fsm_state = _fsm_state_waiting_respawn_ack;
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

        case _fsm_state_waiting_respawn_ack:
            MessageRouter::routePackets();
            if( map_cycle_fsm_client_respawn_ack_flag == true )
            {
                LoadingView::loadFinish();
                map_cycle_fsm_client_respawn_ack_flag = false;
                fsm_state = _fsm_state_game_in_progress;
            }
            break;

        case _fsm_state_waiting_complete_connect:
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
                        fsm_state = _fsm_state_game_in_progress;
                    }

                    MessageRouter::routePacket(np);
                }
            }
            break;

        case _fsm_state_server_respawn_players :
        {
            MessageRouter::routePackets();
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

            if ( execution_mode == _execution_mode_loop_back_server )
            {
                LoadingView::loadFinish();
            }

            GameControlCycleRespawnAck respawn_ack_mesg;
            NetworkServer::broadcastMessage( &respawn_ack_mesg, sizeof(GameControlCycleRespawnAck));

            ServerConnectDaemon::unlockConnectProcess();
            fsm_state = _fsm_state_game_in_progress;
        }
            break;

        case _fsm_state_game_in_progress:
            MessageRouter::routePackets();
            UnitInterface::updateUnitStatus();

            ProjectileInterface::updateStatus();
            ObjectiveInterface::updateObjectiveStatus();
            PowerUpInterface::updateState();
            PathScheduler::run();

            Physics::sim();

            if ( execution_mode != _execution_mode_dedicated_server )
            {
                ParticleSystem2D::simAll();
                Particle2D::simAll();
            }

            if ( NetworkState::getNetworkStatus() == _network_state_server )
            {
                BotManager::simBots();
                checkGameRules();
            }
            break;

    } // ** switch
}

void GameControlRulesDaemon::onTimelimitGameCompleted()
{
    PlayerInterface::lockPlayerStats();

    fsm_state = _fsm_state_server_display_endgame_views;
}

void GameControlRulesDaemon::onFraglimitGameCompleted()
{
    PlayerInterface::lockPlayerStats();

    fsm_state = _fsm_state_server_display_endgame_views;
}

void GameControlRulesDaemon::onObjectiveGameCompleted()
{
    PlayerInterface::lockPlayerStats();

    fsm_state = _fsm_state_server_display_endgame_views;
}

void GameControlRulesDaemon::forceMapChange(std::string _nextmap)
{
    nextmap = _nextmap;

    PlayerInterface::lockPlayerStats();

    fsm_state = _fsm_state_server_display_endgame_views;
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

    fsm_state = _fsm_state_prepare_map_load;
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
