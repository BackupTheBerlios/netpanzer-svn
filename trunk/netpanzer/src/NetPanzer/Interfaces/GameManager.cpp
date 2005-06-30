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

#include "System/Sound.hpp"
#include "System/SDLSound.hpp"
#include "System/SDLVideo.hpp"
#include "System/DummySound.hpp"
#include "NetworkServerUnix.hpp"
#include "NetworkClientUnix.hpp"
#include "ConsoleInterface.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "ClientMessageRouter.hpp"
#include "ClientConnectDaemon.hpp"
#include "ServerConnectDaemon.hpp"
#include "ServerMessageRouter.hpp"
#include "NetworkState.hpp"
#include "SystemNetMessage.hpp"
#include "ConnectNetMessage.hpp"
#include "Util/Log.hpp"
#include "Util/Exception.hpp"
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
#include "Console.hpp"
#include "UnitGlobals.hpp"
#include "SelectionBoxSprite.hpp"
#include "MapsManager.hpp"
#include "PowerUpInterface.hpp"
#include "ChatInterface.hpp"
#include "Util/Exception.hpp"
#include "Util/FileSystem.hpp"

#include "cMouse.hpp"
#include "2D/Palette.hpp"
#include "Desktop.hpp"
#include "GameView.hpp"
#include "MainMenuView.hpp"
#include "JoinView.hpp"
#include "HostView.hpp"
#include "GetSessionView.hpp"
#include "GetSessionHostView.hpp"
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
#include "BaseGameManager.hpp"
#include "PlayerGameManager.hpp"
#include "IPAddressView.hpp"

#include "Particle2D.hpp"
#include "ParticleSystem2D.hpp"
#include "ParticleInterface.hpp"
#include "Physics.hpp"
#include "Util/TimerInterface.hpp"
#include "Util/Math.hpp"

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

void GameManager::loadPalette(const std::string& palette_name)
{
    Palette::init(palette_name);
    Screen->setPalette(Palette::color);
}

// ******************************************************************

void GameManager::setVideoMode()
{
    // try to find sensible defaults for SDL flags...
    if(gameconfig->hardwareSurface.isDefaultValue()) {
        gameconfig->hardwareSurface = false;
    }
    if(gameconfig->hardwareDoubleBuffer.isDefaultValue()) {
        gameconfig->hardwareDoubleBuffer = false;
    }

    // construct flags
    iXY mode_res;
    iXY old_res = screen ? screen->getPix() : iXY(0,0) ;
    Uint32 flags = gameconfig->fullscreen ? SDL_FULLSCREEN : 0;
    flags |= gameconfig->hardwareSurface ? SDL_HWSURFACE : 0;
    flags |= gameconfig->hardwareDoubleBuffer ? SDL_DOUBLEBUF : 0;

    int mode;
    for(mode=gameconfig->screenresolution; mode>=0; mode--) {
        switch(mode) {
            case 0: mode_res = iXY(640,480); break;
            case 1: mode_res = iXY(800,600); break;
            case 2: mode_res = iXY(1024,768); break;
        }

        if(Screen->isDisplayModeAvailable(mode_res.x, mode_res.y, 8, flags)) {
            gameconfig->screenresolution = mode;
            break;
        }
    }
    if(mode<0)
        throw Exception("couldn't find a usable video mode");

    Screen->setVideoMode(mode_res.x, mode_res.y, 8, flags);

    WorldViewInterface::setCameraSize( mode_res.x, mode_res.y);
    delete screen;
    screen = new ScreenSurface(Screen, mode_res.x, mode_res.y);
    gameView.setSize(mode_res);

    Desktop::checkResolution(old_res, mode_res);
    Desktop::checkViewPositions(mode_res);

    if(old_res == iXY(0,0)) {
        // users can get annoyed when they see a black screen for half a minute
        // so we display something here... (we're just hoping that palette1 is
        // not black)
        Palette::color[255].red = 255;
        Palette::color[255].green = 255;
        Palette::color[255].blue = 255;
        Screen->setPalette(Palette::color);
        drawTextCenteredOnScreen("Please wait... generating palettes", 255);
    }
    
    // reset palette
    Screen->setPalette(Palette::color);
}

// ******************************************************************
void GameManager::initializeGameLogic()
{
    PlayerInterface::initialize(gameconfig->maxplayers);
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
    //ObjectiveInterface::cleanUp();
}

void GameManager::shutdownParticleSystems()
{
    ParticleSystem2D::removeAll();
    Particle2D::removeAll();
}

// ******************************************************************
void GameManager::startGameMapLoad(const char *map_file_path,
                                   unsigned long partitions)
{
    map_path = "maps/";
    map_path.append(map_file_path);

    if (!MapInterface::startMapLoad( map_path.c_str(), true, partitions))
        throw Exception("map format error.");
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

    if(screen)
        miniMapView.init();
    ParticleInterface::initParticleSystems();

    ParticleInterface::addCloudParticle(gameconfig->cloudcoverage);
    Physics::wind.setVelocity(gameconfig->windspeed, 107);
}

// ******************************************************************

void GameManager::dedicatedLoadGameMap(const char *map_name )
{
    Console::mapSwitch(map_name);
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
    
    map_path = "maps/";
    map_path.append(map_name);

    MapInterface::startMapLoad( map_path.c_str(), false, 0 );

    map_path.append(".opt");
    ObjectiveInterface::loadObjectiveList( map_path.c_str() );

    ParticleInterface::initParticleSystems();
    Particle2D::setCreateParticles(false);
}

// ******************************************************************
void GameManager::spawnPlayer( const PlayerID &player )
{
    sound->stopTankIdle();

    // ** Get a new spawn point and spawn the player **
    iXY spawn_point = MapInterface::getFreeSpawnPoint();
    PlayerInterface::spawnPlayer( player, spawn_point );

    //** Change the location of the view camera to the spawn point **
    iXY world_loc;
    MapInterface::mapXYtoPointXY( spawn_point, &world_loc );
    if ( PlayerInterface::getLocalPlayerIndex() == player.getIndex() ) {
        WorldViewInterface::setCameraPosition( world_loc );
    } else {
        SystemSetPlayerView set_view(world_loc.x, world_loc.y);
        SERVER->sendMessage(player, &set_view, sizeof(SystemSetPlayerView));
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

void GameManager::netMessageSetView(const NetMessage* message)
{
    const SystemSetPlayerView* set_view
        = (const SystemSetPlayerView*) message;

    WorldViewInterface::setCameraPosition(
            iXY(set_view->getCameraLocX(), set_view->getCameraLocY()) );
}

// ******************************************************************

void GameManager::netMessageViewControl(const NetMessage* message)
{
    const SystemViewControl *view_control
        = (const SystemViewControl*) message;

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
void GameManager::netMessageConnectAlert(const NetMessage* message)
{
    const SystemConnectAlert *connect_alert 
        = (const SystemConnectAlert*) message;
    PlayerState *player_state = 0;

    if(connect_alert->getPlayerID() >= PlayerInterface::getMaxPlayers()) {
        LOGGER.warning("Malformed connect alert message.");
        return;
    }

    player_state = PlayerInterface::getPlayerState( connect_alert->getPlayerID() );

    switch (connect_alert->alert_enum) {
        case _connect_alert_mesg_connect:
            ConsoleInterface::postMessage("%s has joined the game.",
                    player_state->getName().c_str());
            break;

        case _connect_alert_mesg_disconnect: 
            ConsoleInterface::postMessage("%s has left the game.",
                    player_state->getName().c_str());
            break;

        case _connect_alert_mesg_client_drop:
            ConsoleInterface::postMessage(
                    "Connection to %s has been unexpectedly broken.",
                    player_state->getName().c_str());
            break;

        default:
            LOGGER.warning("invalid connection_alert enum found.");
    }
}

// ******************************************************************

void GameManager::netMessageResetGameLogic(const NetMessage* )
{
    resetGameLogic();
}

// ******************************************************************

ConnectMesgServerGameSettings* GameManager::getServerGameSetup()
{
    ConnectMesgServerGameSettings* game_setup 
        = new ConnectMesgServerGameSettings();

    game_setup->setMaxPlayers(gameconfig->maxplayers);
    game_setup->setMaxUnits(gameconfig->maxunits);
    snprintf(game_setup->map_name, 32, gameconfig->map.c_str()); 
    game_setup->setCloudCoverage(gameconfig->cloudcoverage);
    game_setup->setWindSpeed(gameconfig->windspeed);
    game_setup->setGameType(gameconfig->gametype);
    game_setup->powerup_state = gameconfig->powerups;
    game_setup->setFragLimit(gameconfig->fraglimit);
    game_setup->setTimeLimit(gameconfig->timelimit);
    game_setup->setElapsedTime(getGameTime());

    return game_setup;
}

// ******************************************************************

void GameManager::netMessagePingRequest(const NetMessage* message)
{
    PlayerID player_id;
    const SystemPingRequest *ping_request
        = (const SystemPingRequest*) message;

    if(ping_request->getClientPlayerIndex() >= PlayerInterface::getMaxPlayers()) {
        LOGGER.warning("Invalid pingRequest message");
        return;
    }

    player_id = PlayerInterface::getPlayerID( ping_request->getClientPlayerIndex() );

    SystemPingAcknowledge ping_ack;

    if ( (PlayerInterface::getPlayer(player_id.getIndex())->getStatus() == _player_state_active) &&
            (ping_request->getClientPlayerIndex() != 0)
       ) {
        SERVER->sendMessage(player_id, &ping_ack, sizeof(SystemPingAcknowledge));
    }
}

// ******************************************************************

void GameManager::netMessagePingAcknowledge(const NetMessage*)
{
    //NetworkState::ping_time = (now() - NetworkState::ping_time_stamp) * 1000;
}

// ******************************************************************

bool GameManager::startClientGameSetup(const NetMessage* message, int *result_code)
{
    const ConnectMesgServerGameSettings* game_setup
        = (const ConnectMesgServerGameSettings*) message;

    gameconfig->maxplayers = game_setup->getMaxPlayers();
    gameconfig->maxunits = game_setup->getMaxUnits();
    gameconfig->cloudcoverage = game_setup->getCloudCoverage();
    gameconfig->windspeed = (int) game_setup->getWindSpeed();
    gameconfig->powerups = game_setup->powerup_state;
    gameconfig->gametype = game_setup->getGameType();
    gameconfig->fraglimit = game_setup->getFragLimit();
    gameconfig->timelimit = game_setup->getTimeLimit();

    startGameTimer();
    game_elapsed_time_offset = game_setup->getElapsedTime();

    try {
        startGameMapLoad( game_setup->map_name, 20);
    } catch(std::exception& e) {
        LOGGER.warning("Error while loading map '%s': %s",
                game_setup->map_name, e.what());
        *result_code = _mapload_result_no_map_file;
        return false;
    }

    *result_code = _mapload_result_success;
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
void GameManager::processSystemMessage(const NetMessage* message)
{
    switch(message->message_id) {
        case _net_message_id_system_set_view:
            netMessageSetView( message );
            break;

        case _net_message_id_system_view_control:
            netMessageViewControl( message );
            break;

        case _net_message_id_system_reset_game_logic:
            netMessageResetGameLogic( message );
            break;

        case _net_message_id_system_ping_request:
            netMessagePingRequest( message );
            break;

        case _net_message_id_system_ping_ack:
            netMessagePingAcknowledge( message );
            break;

        case _net_message_id_system_connect_alert:
            netMessageConnectAlert( message );
            break;

        default:
            LOGGER.warning("Found unknown SystemNetworkPacket (id %d)",
                    message->message_id);
    }
}

// ******************************************************************
void GameManager::requestNetworkPing()
{
    SystemPingRequest ping_request(PlayerInterface::getLocalPlayerIndex());

    NetworkState::ping_time_stamp = now();
    CLIENT->sendMessage( &ping_request, sizeof(SystemPingRequest));
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

    gamemanager->stopMainLoop();
}

void GameManager::quitNetPanzerGame()
{
    if ( NetworkState::status == _network_state_client ) {
        ClientConnectDaemon::shutdownConnectDaemon();
        CLIENT->partServer();
    } else {
        ServerConnectDaemon::shutdownConnectDaemon();
        SERVER->closeSession();

        // hacky...
        PlayerGameManager* playerGameManager 
            = dynamic_cast<PlayerGameManager*>(gamemanager);
        if(playerGameManager)
            playerGameManager->quitGame();
    }

    ParticleSystem2D::removeAll();
    Particle2D::removeAll();
    sound->stopTankIdle();
    UnitInterface::reset();
    PlayerInterface::reset();
    ObjectiveInterface::resetLogic();
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

