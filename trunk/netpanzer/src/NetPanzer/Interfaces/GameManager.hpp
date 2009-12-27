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
#include <string>

#include "Classes/PlayerState.hpp"
#include "Classes/Network/NetPacket.hpp"
#include "Classes/Network/ClientConnectDaemon.hpp"
#include "Classes/Network/ServerConnectDaemon.hpp"
#include "Interfaces/GameControlRulesDaemon.hpp"
#include "Types/iXY.hpp"

enum { _mapload_result_success,
       _mapload_result_no_map_file,
       _mapload_result_no_wad_file
     };

class ConnectMesgServerGameSettings;

class GameManager
{
public:
    // ** Game Rules Methods
    static void spawnPlayer( const Uint16 player );
    static void spawnPlayerAt( const Uint16 player, const iXY& position );
    static void respawnAllPlayers();

    static void kickPlayer( const Uint16 player );
    static void destroyPlayerUnits( const Uint16 player );
    static void disownPlayerObjectives( const Uint16 player );

    static Uint16 addBot();
    static void removeAllBots();

    static void exitNetPanzer();
    static void quitNetPanzerGame();

    static bool changeMap(const char * map_name);

    static void disconnectPlayerCleanUp( const Uint16 player );

    static void dedicatedLoadGameMap(const char *map_file_path );
    static void loadGameMap(const char *map_file_path);

    static void shutdownParticleSystems();

    static bool startClientGameSetup(const NetMessage* message, int *result_code);
    static void clientGameSetup();
    static ConnectMesgServerGameSettings* getServerGameSetup();

    static void setNetPanzerGameOptions();

    static void requestNetworkPing();

    static void processSystemMessage(const NetMessage* message);

    static void setVideoMode();

    static void drawTextCenteredOnScreen(const char *string, IntColor color);

protected:
    // ** Network Message Handlers
    static void netMessageSetView(const NetMessage* message);
    static void netMessageViewControl(const  NetMessage* message);
    static void netMessageClientGameSetup(const NetMessage* message);
    static void netMessagePingRequest(const NetMessage* message);
    static void netMessagePingAcknowledge(const NetMessage* message);
    static void netMessageConnectAlert(const NetMessage* message);
    static void netMessageResetGameLogic(const NetMessage* message);
};

#endif
