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
#ifndef _GAME_CONTROL_RULES_DAEMON_HPP
#define _GAME_CONTROL_RULES_DAEMON_HPP

#include "Timer.hpp"
#include "NetPacket.hpp"

class GameControlRulesDaemon
{
    static int execution_mode;
    static unsigned char game_state;

protected:
    static int map_cycle_fsm_server_state;
    static Timer map_cycle_fsm_server_endgame_timer;
    static Timer map_cycle_fsm_server_map_load_timer;
    static void mapCycleFsmServer();

    static int map_cycle_fsm_client_state;
    static char map_cycle_fsm_client_map_name[256];
    static bool map_cycle_fsm_client_respawn_ack_flag;
    static void mapCycleFsmClient();

    static void onTimelimitGameCompleted();
    static void onFraglimitGameCompleted();
    static void onObjectiveGameCompleted();

    static void checkGameRules();

    static void mapLoadFailureResponse( int result_code, char *map_name );

    static void netMessageCycleMap(NetMessage *message);
    static void netMessageCycleRespawnAck(NetMessage *message);

public:
    static void setStateServerInProgress();
    static void processNetMessage(NetMessage *message);
    static void forceMapCycle();
    static void updateGameControlFlow();
};

#endif // ** _GAME_CONTROL_RULES_DAEMON_HPP
