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

#include "Interfaces/GameManager.hpp"
#include "Core/GlobalEngineState.hpp"

#include <stdio.h>
#include <fcntl.h>

#include "System/Sound.hpp"
#include "System/SDLSound.hpp"
#include "System/SDLVideo.hpp"
#include "System/DummySound.hpp"

#include "Interfaces/BaseGameManager.hpp"
#include "Interfaces/ChatInterface.hpp"
#include "Interfaces/Console.hpp"
#include "Interfaces/ConsoleInterface.hpp"
#include "Interfaces/MouseInterface.hpp"
#include "Interfaces/KeyboardInterface.hpp"
#include "Interfaces/GameConfig.hpp"
#include "Interfaces/MapInterface.hpp"
#include "Interfaces/MapsManager.hpp"
#include "Objectives/ObjectiveInterface.hpp"
#include "Interfaces/PathScheduler.hpp"
#include "Interfaces/PlayerGameManager.hpp"
#include "Interfaces/PlayerInterface.hpp"
#include "PowerUps/PowerUpInterface.hpp"
#include "Weapons/ProjectileInterface.hpp"
#include "Units/UnitInterface.hpp"
#include "Units/UnitProfileInterface.hpp"
#include "Interfaces/WorldViewInterface.hpp"

#include "Classes/ScreenSurface.hpp"
#include "Classes/WorldInputCmdProcessor.hpp"
#include "Classes/SpriteSorter.hpp"
#include "Classes/Network/ClientConnectDaemon.hpp"
#include "Classes/Network/ServerConnectDaemon.hpp"
#include "Classes/Network/NetworkState.hpp"
#include "Classes/Network/NetworkServer.hpp"
#include "Classes/Network/NetworkClient.hpp"
#include "Classes/Network/SystemNetMessage.hpp"
#include "Classes/Network/ConnectNetMessage.hpp"
#include "Units/Unit.hpp"

#include "2D/Palette.hpp"
#include "Views/Components/Desktop.hpp"
#include "Views/Components/ViewGlobals.hpp"
#include "Views/MainMenu/MainMenuView.hpp"
#include "Views/MainMenu/OptionsTemplateView.hpp"
#include "Views/MainMenu/HelpView.hpp"
#include "Views/MainMenu/Multi/JoinView.hpp"
#include "Views/MainMenu/Multi/HostView.hpp"
#include "Views/MainMenu/Multi/GetSessionView.hpp"
#include "Views/MainMenu/Multi/FlagSelectionView.hpp"
#include "Views/MainMenu/Multi/HostOptionsView.hpp"
#include "Views/MainMenu/Multi/MapSelectionView.hpp"
#include "Views/MainMenu/Multi/PlayerNameView.hpp"
#include "Views/MainMenu/Multi/IPAddressView.hpp"
#include "Views/MainMenu/Options/SoundView.hpp"
#include "Views/MainMenu/Options/ControlsView.hpp"
#include "Views/MainMenu/Options/InterfaceView.hpp"
#include "Views/MainMenu/Options/VisualsView.hpp"
#include "Views/Game/RankView.hpp"
#include "Views/Game/VehicleSelectionView.hpp"
#include "Views/Game/CodeStatsView.hpp"
#include "Views/Game/LibView.hpp"
#include "Views/Game/HelpScrollView.hpp"
#include "Views/Game/AreYouSureResignView.hpp"
#include "Views/Game/AreYouSureExitView.hpp"
#include "Views/Game/GameView.hpp"
#include "Views/Game/MiniMapView.hpp"

#include "Particles/Particle2D.hpp"
#include "Particles/ParticleSystem2D.hpp"
#include "Particles/ParticleInterface.hpp"
#include "Particles/Physics.hpp"

#include "Util/TimerInterface.hpp"
#include "Util/Math.hpp"
#include "Util/Log.hpp"
#include "Util/Exception.hpp"
#include "Util/FileSystem.hpp"

#include "Bot/Bot.hpp"
#include "Bot/BotManager.hpp"

#include "Classes/SpawnList.hpp"
#include "Resources/ResourceManager.hpp"
#include "Resources/ResourceManagerMessages.hpp"

#define _MAX_INITIALIZE_PROCS (10)
#define _MAX_DEDICATED_INITIALIZE_PROCS (8)

// ******************************************************************

void GameManager::drawTextCenteredOnScreen(const char *string, IntColor color)
{
        Surface text;
        text.renderText(string, color, 0);
        screen->lock();
    
        screen->fill(0);
        int x = (screen->getWidth() / 2) - (text.getWidth() / 2);
        int y = (screen->getHeight() / 2) - (text.getHeight() / 2);
        text.blt(*screen,x,y);
    
        screen->unlock();
        screen->copyToVideoFlip();
}

// ******************************************************************

void GameManager::setVideoMode()
{

    // construct flags
    iXY mode_res;
    iXY old_res = screen ? iXY(screen->getWidth(), screen->getHeight()): iXY(0,0);
    Uint32 flags = gameconfig->fullscreen ? SDL_FULLSCREEN : 0;
    flags |= gameconfig->hardwareSurface ? SDL_HWSURFACE : 0;
    flags |= gameconfig->hardwareDoubleBuffer ? SDL_DOUBLEBUF : 0;

    int bpp = 32;
    int mode;
    for(mode=gameconfig->screenresolution; mode>=0; mode--) {
        switch(mode) {
            case 0: mode_res = iXY(640,480); break;
            case 1: mode_res = iXY(800,600); break;
            case 2: mode_res = iXY(1024,768); break;
            case 3: mode_res = iXY(1280,1024); break;
        }

        if(Screen->isDisplayModeAvailable(mode_res.x, mode_res.y, bpp, flags))
        {
            gameconfig->screenresolution = mode;
            break;
        }
    }

    if(mode<0)
        throw Exception("couldn't find a usable video mode");

    Screen->setVideoMode(mode_res.x, mode_res.y, bpp, flags);

    WorldViewInterface::setCameraSize( mode_res.x, mode_res.y);
    delete screen;
    screen = new ScreenSurface(Screen, mode_res.x, mode_res.y);

    Desktop::checkResolution(old_res, mode_res);
    Desktop::checkViewPositions(mode_res);

    if(old_res == iXY(0,0))
    {
        drawTextCenteredOnScreen("Loading...", 0xffffff); // white
    }
    
    Palette::setColors();
}

void GameManager::shutdownParticleSystems()
{
    ParticleSystem2D::removeAll();
    Particle2D::removeAll();
}

// ******************************************************************
void GameManager::loadGameMap(const char *map_file_path)
{
    std::string map_path("maps/");
    map_path.append(map_file_path);

    if ( !MapInterface::load( map_path.c_str()) )
        throw Exception("map format error.");

    map_path.append(".opt");
    ObjectiveInterface::loadObjectiveList( map_path.c_str() );

}

// ******************************************************************
void GameManager::spawnPlayer( const Uint16 player )
{
    global_engine_state->sound_manager->stopTankIdle();

    // ** Get a new spawn point and spawn the player **
    iXY spawn_point = MapInterface::getFreeSpawnPoint();

    PlayerInterface::spawnPlayer( player, spawn_point );

    global_engine_state->sound_manager->playTankIdle();
}

void GameManager::spawnPlayerAt( const Uint16 player, const iXY& location )
{
    if (   location.x >=0 && location.y >=0
        && location.x < MapInterface::getWidth()
        && location.y < MapInterface::getHeight() )
    {
        global_engine_state->sound_manager->stopTankIdle();
        PlayerInterface::spawnPlayer( player, location );
        global_engine_state->sound_manager->playTankIdle();
    }
}

void GameManager::respawnAllPlayers()
{
    PlayerState    *player_state;
    unsigned short max_players;
    unsigned short player_index;

    max_players = PlayerInterface::getMaxPlayers();

    for( player_index = 0; player_index < max_players; player_index++ )
    {
        player_state = PlayerInterface::getPlayer( player_index );
        if ( player_state->getStatus() == _player_state_active )
        {
            spawnPlayer( player_index );
        }
    }
}

void GameManager::disconnectPlayerCleanUp(const Uint16 player)
{
    destroyPlayerUnits(player);
    disownPlayerObjectives(player);

    PlayerState* player_st = PlayerInterface::getPlayer(player);

    ResourceManagerReleaseFlagMessage releasemsg;
    releasemsg.setFlagID(player_st->getFlag());
    ResourceManager::releaseFlag(player_st->getFlag());
    NetworkServer::broadcastMessage(&releasemsg, sizeof(releasemsg));

    PlayerInterface::disconnectPlayerCleanup( player );

    SystemConnectAlert msg;
    msg.set( player, _connect_alert_mesg_disconnect);
    NetworkServer::broadcastMessage(&msg, sizeof(msg));
    NetworkClient::sendMessage(&msg, sizeof(msg));
}

void GameManager::kickPlayer(const Uint16 player)
{
    if ( player < PlayerInterface::getMaxPlayers() )
    {
        if ( BotManager::isBot(player) )
        {
            BotManager::removeBot(player);
        }
        else
        {
            NetworkServer::removePlayerSocket(player);
        }
        disconnectPlayerCleanUp(player);
    }
}

void GameManager::destroyPlayerUnits(const Uint16 player)
{
    UnitInterface::destroyPlayerUnits(player);
}

void GameManager::disownPlayerObjectives(const Uint16 player)
{
    if ( player < PlayerInterface::getMaxPlayers() )
    {
        ObjectiveInterface::disownPlayerObjectives(player);
    }
}

Uint16 GameManager::addBot()
{
    return BotManager::addBot();
}

void GameManager::removeAllBots()
{
    for (Uint16 player=0; player<PlayerInterface::getMaxPlayers(); ++player)
    {
        if ( BotManager::isBot(player) )
        {
            BotManager::removeBot(player);
            disconnectPlayerCleanUp(player);
        }
    }
}

bool GameManager::changeMap(const char* map_name)
{
    if ( !MapsManager::existsMap(map_name) )
    {
        return false;
    }

    GameControlRulesDaemon::forceMapChange(map_name);
    return true;
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

    player_state = PlayerInterface::getPlayer( connect_alert->getPlayerID() );

    switch (connect_alert->alert_enum) {
        case _connect_alert_mesg_connect:
            ConsoleInterface::postMessage(Color::cyan, true, player_state->getFlag(),
                                          "%s has joined the game.",
                    player_state->getName().c_str());

            break;

        case _connect_alert_mesg_disconnect: 
            ConsoleInterface::postMessage(Color::cyan, true, player_state->getFlag(),
                                          "%s has left the game.",
                    player_state->getName().c_str());
            break;

        case _connect_alert_mesg_client_drop:
            ConsoleInterface::postMessage(Color::cyan, true, player_state->getFlag(),
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
    global_engine_state->game_manager->resetGameLogic();
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
    game_setup->setElapsedTime(global_engine_state->game_manager->getGameTime());

    return game_setup;
}

// ******************************************************************

void GameManager::netMessagePingRequest(const NetMessage* message)
{
    const SystemPingRequest *ping_request
        = (const SystemPingRequest*) message;

    if(ping_request->getClientPlayerIndex() >= PlayerInterface::getMaxPlayers())
    {
        LOGGER.warning("Invalid pingRequest message");
        return;
    }

    PlayerState * player;
    player = PlayerInterface::getPlayer( ping_request->getClientPlayerIndex() );

    SystemPingAcknowledge ping_ack;

    if ( player->getStatus() == _player_state_active
         && ping_request->getClientPlayerIndex() != 0
       )
    {
        NetworkServer::sendMessage(player->getID(), &ping_ack, sizeof(SystemPingAcknowledge));
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

    global_engine_state->game_manager->setElapsetTimeOffset(game_setup->getElapsedTime());
    gameconfig->map = game_setup->map_name;

//    try {
//        loadGameMap(game_setup->map_name);
//    } catch(std::exception& e) {
//        LOGGER.warning("XError while loading map '%s': %s",
//                game_setup->map_name, e.what());
//        *result_code = _mapload_result_no_map_file;
//        return false;
//    }

    *result_code = _mapload_result_success;
    return true;
}

// ******************************************************************

void GameManager::clientGameSetup( )
{
    global_engine_state->game_manager->reinitializeGameLogic();
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
    NetworkClient::sendMessage( &ping_request, sizeof(SystemPingRequest));
}

void GameManager::setNetPanzerGameOptions()
{
    // nothing
}

// ******************************************************************
void GameManager::exitNetPanzer()
{
    quitNetPanzerGame();

    global_engine_state->game_manager->stopMainLoop();
}

void GameManager::quitNetPanzerGame()
{
    if ( NetworkState::status == _network_state_client ) {
        ClientConnectDaemon::shutdownConnectDaemon();
        NetworkClient::partServer();
    } else {
        ServerConnectDaemon::shutdownConnectDaemon();
        NetworkServer::closeSession();

        // hacky...
        PlayerGameManager* playerGameManager 
            = dynamic_cast<PlayerGameManager*>(global_engine_state->game_manager);
        if(playerGameManager)
            playerGameManager->quitGame();
    }

    ParticleSystem2D::removeAll();
    Particle2D::removeAll();
    global_engine_state->sound_manager->stopTankIdle();
//    UnitInterface::reset();
    PlayerInterface::reset();
    ObjectiveInterface::resetLogic();
    GameControlRulesDaemon::setStateIdle();
}
