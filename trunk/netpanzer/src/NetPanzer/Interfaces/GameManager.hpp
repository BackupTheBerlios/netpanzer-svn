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
#ifndef _GAMEMANAGER_HPP
#define _GAMEMANAGER_HPP

#include <time.h>
#include <stdint.h>
#include <string>

#include "PlayerState.hpp"
#include "NetPacket.hpp"
#include "ClientConnectDaemon.hpp"
#include "ServerConnectDaemon.hpp"
#include "GameControlRulesDaemon.hpp"
#include "iXY.hpp"

enum { _mapload_result_success,
       _mapload_result_no_map_file,
       _mapload_result_no_wad_file
     };

class GameManager
{
private:
    static std::string map_path;

protected:
    static time_t game_start_time;
    static time_t game_elapsed_time_offset;

    static bool display_frame_rate_flag;
    static bool display_network_info_flag;

protected:
    static void finishGameMapLoad();

    // ** Network Message Handlers
    static void netMessageSetView( NetMessage *message );
    static void netMessageViewControl( NetMessage *message );
    static void netMessageClientGameSetup( NetMessage *message );
    static void netMessagePingRequest( NetMessage *message );
    static void netMessagePingAcknowledge( NetMessage *message );
    static void netMessageConnectAlert( NetMessage *message );
    static void netMessageResetGameLogic( NetMessage *message );

public:
    static void dedicatedLoadGameMap(const char *map_file_path );
    static bool startGameMapLoad(const char *map_file_path, unsigned long partitions, int *result_code );
    static bool gameMapLoad( int *percent_complete );

    // ** Game Rules Methods
    static void spawnPlayer( const PlayerID &player );
    static void spawnPlayer( PlayerState *player_state );
    static void respawnAllPlayers();

    static void initializeGameLogic();
    static void reinitializeGameLogic();
    static bool resetGameLogic();
    static void shutdownGameLogic();

    static void shutdownParticleSystems();

    static bool startClientGameSetup( NetMessage *message, int *result_code );
    static bool clientGameSetup( int *percent_complete );
    static void getServerGameSetup( NetMessage *message );


public:
    static void exitNetPanzer();

    static void quitNetPanzerGame();

    static void setNetPanzerGameOptions();

    static void requestNetworkPing();

    static void processSystemMessage( NetMessage *message );

    static void setVideoMode();

    static void loadPalette( char *palette_path );

    static void drawTextCenteredOnScreen(const char *string, unsigned char color);

    static void   startGameTimer();
    static time_t getGameTime();
};

#endif
