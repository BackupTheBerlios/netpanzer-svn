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
#include "GameManager.hpp"

#include <stdio.h>
#include <fcntl.h>

#include "UILib/UIDraw.hpp"
#include "UILib/Sound.hpp"
#include "UILib/SDL/SDLSound.hpp"
#include "UILib/SDL/SDLDraw.hpp"
#include "UILib/DummySound.hpp"
#include "NetworkServerUnix.hpp"
#include "NetworkClientUnix.hpp"

// ** PObject netPanzer Network Includes
#include "Server.hpp"
#include "Client.hpp"
#include "ClientMessageRouter.hpp"
#include "ClientConnectDaemon.hpp"
#include "ServerConnectDaemon.hpp"
#include "ServerMessageRouter.hpp"
#include "NetworkState.hpp"
#include "SystemNetMessage.hpp"
#include "ConnectNetMessage.hpp"

#include "Log.hpp"
#include "MouseInterface.hpp"
#include "KeyboardInterface.hpp"
#include "ScreenSurface.hpp"
#include "GameConfig.hpp"
#include "TileInterface.hpp"
#include "TileEngine.hpp"
#include "MapInterface.hpp"
#include "MiniMapInterface.hpp"
#include "PlayerInterface.hpp"
#include "UnitInterface.hpp"
#include "UnitBlackBoard.hpp"
#include "ProjectileInterface.hpp"
#include "PathScheduler.hpp"
#include "WorldViewInterface.hpp"
#include "WorldInputCmdProcessor.hpp"
#include "SpriteSorter.hpp"
#include "Vehicle.hpp"
#include "UnitProfileInterface.hpp"
#include "ObjectiveInterface.hpp"
#include "ConsoleInterface.hpp"
#include "UnitGlobals.hpp"
#include "SelectionBoxSprite.hpp"
#include "Gdatstct.hpp"
#include "MapsManager.hpp"
#include "PowerUpInterface.hpp"
#include "ChatInterface.hpp"
#include "Exception.hpp"
#include "FileSystem.hpp"

// ** GVS Includes
#include "cMouse.hpp"
#include "Palette.hpp"
#include "Desktop.hpp"
#include "GameView.hpp"
#include "MainMenuView.hpp"
#include "JoinView.hpp"
#include "HostView.hpp"
#include "GetSessionView.hpp"
#include "GetSessionHostView.hpp"
#include "ChatView.hpp"
#include "WinnerMesgView.hpp"
#include "RankView.hpp"
#include "VehicleSelectionView.hpp"
#include "MiniMapView.hpp"
#include "OptionsTemplateView.hpp"
#include "OrderingView.hpp"
#include "SkirmishView.hpp"
#include "HelpView.hpp"
#include "SoundView.hpp"
#include "ControlsView.hpp"
#include "InterfaceView.hpp"
#include "VisualsView.hpp"
#include "LobbyView.hpp"
#include "ProgressView.hpp"
#include "CodeStatsView.hpp"
#include "ViewGlobals.hpp"
#include "LibView.hpp"
#include "HelpScrollView.hpp"
#include "ResignView.hpp"
#include "AreYouSureResignView.hpp"
#include "AreYouSureExitView.hpp"
#include "UnitSelectionView.hpp"
#include "FlagSelectionView.hpp"
#include "UnitColorView.hpp"
#include "HostOptionsView.hpp"
#include "MapSelectionView.hpp"
#include "PlayerNameView.hpp"
#include "GameInfoView.hpp"
#include "GameToolbarView.hpp"

//winsock hack
#include "IPAddressView.hpp"

//** Physics/Particle Stuff
#include "Particle2D.hpp"
#include "ParticleSystem2D.hpp"
#include "ParticleInterface.hpp"
#include "Physics.hpp"
#include "TimerInterface.hpp"
#include "Math.hpp"

#include "Bot.hpp"

#define _MAX_INITIALIZE_PROCS (10)
#define _MAX_DEDICATED_INITIALIZE_PROCS (8)


time_t GameManager::game_start_time = 0;
time_t GameManager::game_elapsed_time_offset = 0;

bool GameManager::display_frame_rate_flag = false;
bool GameManager::display_network_info_flag;

std::string GameManager::map_path;

static Surface hostLoadSurface;

// ******************************************************************

void GameManager::drawTextCenteredOnScreen(const char *string, PIX color)
{
    screen->lock();
    
    screen->fill(0);
    screen->bltStringCenter(string, color);
    
    screen->unlock();
    screen->copyToVideoFlip();
}

// ******************************************************************

void GameManager::loadPalette(char *palette_path)
{
    Palette::init(palette_path);
    Screen->setPalette(Palette::color);
}

// ******************************************************************

void GameManager::setVideoMode()
{
    iXY mode_res;
    iXY old_res = screen ? screen->getPix() : iXY(0,0) ;
    bool fullscreen = gameconfig->fullscreen;

    int mode;
    for(mode=gameconfig->screenresolution; mode>=0; mode--) {
        switch(mode) {
        case 0: mode_res = iXY(640,480); break;
        case 1: mode_res = iXY(800,600); break;
        case 2: mode_res = iXY(1024,768); break;
        case 3: mode_res = iXY(1280, 1024); break;
        }

        if(Screen->isDisplayModeAvailable(mode_res.x, mode_res.y, 8, fullscreen)) {
            gameconfig->screenresolution = mode;
            break;
        }
    }
    if(mode<0)
        throw Exception("couldn't find a usable video mode");

    Screen->setVideoMode(mode_res.x, mode_res.y, 8, fullscreen);

    WorldViewInterface::setCameraSize( mode_res.x, mode_res.y );
    delete screen;
    screen = new ScreenSurface(Screen, mode_res.x, mode_res.y, 8);
    gameView.setSize(mode_res);

    Desktop::checkResolution(old_res, mode_res);
    Desktop::checkViewPositions(mode_res);
    //ConsoleInterface::setToSurfaceSize( mode_res );

    // reset palette
    Screen->setPalette(Palette::color);
}

// ******************************************************************
void GameManager::initializeGameLogic()
{
    PlayerInterface::initialize( gameconfig->maxplayers,
                                 gameconfig->initialunits );
    UnitBlackBoard::initializeBlackBoard();
    UnitInterface::initialize( gameconfig->GetUnitsPerPlayer() );
    PathScheduler::initialize();
    PowerUpInterface::resetLogic();
}

// ******************************************************************
void GameManager::reinitializeGameLogic()
{
    shutdownGameLogic();
    initializeGameLogic();
}

// ******************************************************************
bool GameManager::resetGameLogic()
{
    PlayerInterface::reset();
    UnitInterface::reset();
    UnitBlackBoard::initializeBlackBoard();
    PathScheduler::initialize();
    PowerUpInterface::resetLogic();
    ProjectileInterface::resetLogic();
    startGameTimer();
    return true;
}

// ******************************************************************
void GameManager::shutdownGameLogic()
{
    PlayerInterface::cleanUp();
    UnitInterface::cleanUp();
    PathScheduler::cleanUp();
}

void GameManager::shutdownParticleSystems()
{
    ParticleSystem2D::removeAll();
    Particle2D::removeAll();
}

// ******************************************************************
bool GameManager::startGameMapLoad(const char *map_file_path, unsigned long partitions, int *result_code )
{
    int check_return_code;
    check_return_code = MapsManager::checkMapValidity( map_file_path );

    if( check_return_code == _mapfile_not_found ) {
        *result_code = _mapload_result_no_map_file;
        return false;
    } else
        if( check_return_code == _wadfile_not_found ) {
            *result_code = _mapload_result_no_wad_file;
            return false;
        } else {
            *result_code = _mapload_result_success;
        }

    map_path = "maps/";
    map_path.append(map_file_path);

    if ( MapInterface::startMapLoad( map_path.c_str(), true, partitions ) == false ) {
        finishGameMapLoad();
        return false;
    }

    return true;
}

// ******************************************************************

bool GameManager::gameMapLoad( int *percent_complete )
{
    if( MapInterface::loadMap( percent_complete ) == false ) {
        finishGameMapLoad();
        return false;
    }

    return true;
}

// ******************************************************************

void GameManager::finishGameMapLoad()
{
    std::string temp_path = map_path;
    temp_path.append(".opt");
    ObjectiveInterface::loadObjectiveList( temp_path.c_str() );

    miniMapView.init();
    ParticleInterface::initParticleSystems();

    ParticleInterface::addCloudParticle(gameconfig->cloudcoverage);
    Physics::wind.setVelocity(gameconfig->windspeed, 107);
}

// ******************************************************************

void GameManager::dedicatedLoadGameMap(const char *map_name )
{
    map_path = "maps/";
    map_path.append(map_name);

    MapInterface::startMapLoad( map_path.c_str(), false, 0 );

    map_path.append(".opt");
    ObjectiveInterface::loadObjectiveList( map_path.c_str() );

    ParticleInterface::initParticleSystems();
    Particle2D::setCreateParticles(false);

    ParticleInterface::addCloudParticle(gameconfig->cloudcoverage);
    Physics::wind.setVelocity(gameconfig->windspeed, 107);
}


// ******************************************************************

void GameManager::spawnPlayer( const PlayerID &player )
{
    iXY spawn_point;

    sound->stopTankIdle();

    // ** Get a new spawn point and spawn the player **
    MapInterface::getFreeSpawnPoint( &spawn_point );
    PlayerInterface::spawnPlayer( player, spawn_point );

    //** Change the location of the view camera to the spawn point **
    iXY world_loc;
    MapInterface::mapXYtoPointXY( spawn_point, &world_loc );
    if ( PlayerInterface::getLocalPlayerIndex() == player.getIndex() ) {
        WorldViewInterface::setCameraPosition( world_loc );
    } else {
        SystemSetPlayerView set_view;

        set_view.camera_loc = world_loc;
        SERVER->sendMessage( player, &set_view, sizeof( SystemSetPlayerView ), 0);
    }

    sound->playTankIdle();
}

void GameManager::respawnAllPlayers()
{
    PlayerState    *player_state;
    unsigned short max_players;
    unsigned short player_index;

    max_players = PlayerInterface::getMaxPlayers();

    for( player_index = 0; player_index < max_players; player_index++ ) {
        player_state = PlayerInterface::getPlayerState( player_index );
        if ( player_state->getStatus() == _player_state_active ) {
            spawnPlayer( player_state->getPlayerID() );
        }
    }
}

// ******************************************************************
void GameManager::spawnPlayer( PlayerState *player_state )
{
    spawnPlayer( player_state->getPlayerID() );
}

// ******************************************************************

void GameManager::netMessageSetView( NetMessage *message )
{
    SystemSetPlayerView *set_view;

    set_view = (SystemSetPlayerView *) message;

    WorldViewInterface::setCameraPosition( set_view->camera_loc );
}

// ******************************************************************

void GameManager::netMessageViewControl( NetMessage *message )
{
    SystemViewControl *view_control;

    view_control =  (SystemViewControl *) message;

    if ( view_control->action_flags & _view_control_flag_close_all ) {
        Desktop::setVisibilityAllWindows(false);
        Desktop::setVisibility("GameView", true);
    }

    if ( view_control->action_flags & _view_control_flag_visible_on ) {
        Desktop::setVisibility( view_control->view_name, true );
    }

    if ( view_control->action_flags & _view_control_flag_visible_off ) {
        Desktop::setVisibility( view_control->view_name, false );
    }
}

// ******************************************************************
void GameManager::netMessageConnectAlert( NetMessage *message )
{
    SystemConnectAlert *connect_alert = 0;
    PlayerState *player_state = 0;

    connect_alert = (SystemConnectAlert *) message;

    player_state = PlayerInterface::getPlayerState( connect_alert->player_id );

    switch( connect_alert->alert_enum ) {
    case _connect_alert_mesg_connect : {
            ConsoleInterface::postMessage( "%s has joined the game.", player_state->getName() );
        }
        break;

    case _connect_alert_mesg_disconnect : {
            ConsoleInterface::postMessage( "%s has left the game.", player_state->getName() );
        }
        break;

    case _connect_alert_mesg_client_drop : {
            ConsoleInterface::postMessage( "Connection to %s has been unexpectedly broken.", player_state->getName() );
        }
        break;

    default :
        assert(0);
    } // ** switch
}

// ******************************************************************

void GameManager::netMessageResetGameLogic( NetMessage *message )
{
    resetGameLogic();
}

// ******************************************************************

void GameManager::getServerGameSetup( NetMessage *message )
{
    ConnectMesgServerGameSettings *game_setup;

    game_setup = (ConnectMesgServerGameSettings *) message;

    game_setup->max_players = gameconfig->maxplayers;
    game_setup->max_units = gameconfig->maxunits;
    MapsManager::getCurrentMap( game_setup->map_name );
    game_setup->cloud_coverage = gameconfig->cloudcoverage;
    game_setup->wind_speed = gameconfig->windspeed;
    game_setup->game_type = gameconfig->gametype;
    game_setup->map_cycle_state = gameconfig->mapcycling;
    game_setup->powerup_state = gameconfig->powerups;
    game_setup->frag_limit = gameconfig->fraglimit;
    game_setup->time_limit = gameconfig->timelimit;
    game_setup->elapsed_time = getGameTime();
}

// ******************************************************************

void GameManager::netMessagePingRequest( NetMessage *message )
{
    PlayerID player_id;
    SystemPingRequest *ping_request;

    ping_request = (SystemPingRequest *) message;

    player_id = PlayerInterface::getPlayerID( ping_request->client_player_index );

    SystemPingAcknowledge ping_ack;

    if ( (PlayerInterface::getPlayerStatus( player_id ) == _player_state_active) &&
            (ping_request->client_player_index != 0)
       ) {
        SERVER->sendMessage( player_id, &ping_ack, sizeof(SystemPingAcknowledge), 0 );
    }
}

// ******************************************************************

void GameManager::netMessagePingAcknowledge( NetMessage *message )
{
    NetworkState::ping_time = (now() - NetworkState::ping_time_stamp) * 1000;
}

// ******************************************************************

bool GameManager::startClientGameSetup( NetMessage *message, int *result_code )
{
    ConnectMesgServerGameSettings *game_setup;

    game_setup = (ConnectMesgServerGameSettings *) message;

    gameconfig->maxplayers = game_setup->max_players;
    gameconfig->maxunits = game_setup->max_units;
    gameconfig->cloudcoverage = game_setup->cloud_coverage;
    gameconfig->windspeed = (int) game_setup->wind_speed;
    gameconfig->mapcycling = game_setup->map_cycle_state;
    gameconfig->powerups = game_setup->powerup_state;
    gameconfig->gametype = game_setup->game_type;
    gameconfig->fraglimit = game_setup->frag_limit;
    gameconfig->timelimit = game_setup->time_limit;

    startGameTimer();
    game_elapsed_time_offset = game_setup->elapsed_time;

    startGameMapLoad( game_setup->map_name, 20, result_code );
    return true;
}

// ******************************************************************

bool GameManager::clientGameSetup( int *percent_complete )
{
    if( gameMapLoad( percent_complete ) == false ) {
        reinitializeGameLogic();
        return false;
    }

    return true;
}

// ******************************************************************
void GameManager::processSystemMessage( NetMessage *message )
{
    switch( message->message_id ) {
    case _net_message_id_system_set_view :
        netMessageSetView( message );
        break;

    case _net_message_id_system_view_control :
        netMessageViewControl( message );
        break;

    case _net_message_id_system_reset_game_logic :
        netMessageResetGameLogic( message );
        break;

    case _net_message_id_system_ping_request :
        netMessagePingRequest( message );
        break;

    case _net_message_id_system_ping_ack :
        netMessagePingAcknowledge( message );
        break;

    case _net_message_id_system_connect_alert :
        netMessageConnectAlert( message );
        break;


    } // ** switch
}

// ******************************************************************
void GameManager::requestNetworkPing()
{
    SystemPingRequest ping_request;

    ping_request.client_player_index = PlayerInterface::getLocalPlayerIndex();

    NetworkState::ping_time_stamp = now();
    CLIENT->sendMessage( &ping_request, sizeof(SystemPingRequest), 0  );

}

void GameManager::setNetPanzerGameOptions()
{

    MiniMapInterface::setProperties( gameconfig->getPlayerRadarUnitColor(),
                                     gameconfig->getSelectedRadarUnitColor(),
                                     gameconfig->getAlliedRadarUnitColor(),
                                     gameconfig->getPlayerOutpostRadarColor(),
                                     gameconfig->getAlliedOutpostRadarColor(),
                                     gameconfig->getEnemyOutpostRadarColor() );
}

// ******************************************************************
void GameManager::exitNetPanzer()
{
    quitNetPanzerGame();

    SDL_Event event;
    event.type = SDL_QUIT;
    SDL_PushEvent(&event);
}


void GameManager::quitNetPanzerGame()
{
    if ( NetworkState::status == _network_state_client ) {
        ClientConnectDaemon::shutdownConnectDaemon();
        CLIENT->closeSession();
    } else {
        ServerConnectDaemon::shutdownConnectDaemon();
        SERVER->closeSession();
    }
}

void GameManager::startGameTimer()
{
    game_elapsed_time_offset = 0;
    time( &game_start_time );
}

time_t GameManager::getGameTime()
{
    time_t current_time;

    time( &current_time );

    return( (current_time - game_start_time) + game_elapsed_time_offset );
}

