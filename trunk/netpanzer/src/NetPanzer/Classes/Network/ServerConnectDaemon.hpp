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
#ifndef _SERVERCONNECTDAEMON_HPP
#define _SERVERCONNECTDAEMON_HPP

#include <list>

#include "PlayerState.hpp"
#include "ArrayUtil/QueueTemplate.hpp"
#include "NetPacket.hpp"
#include "Util/Timer.hpp"
#include "UnitSync.hpp"

enum { _connect_status_waiting,
       _connect_status_abort,
       _connect_status_connecting
     };

class ConnectQueueElement
{
public:
    PlayerID       new_player_id;
    unsigned short connect_status;
};

class ServerConnectDaemon
{
protected:
	enum ConnectionState {
		connect_state_idle,
	 	connect_state_wait_for_connect_request,
	  	connect_state_attempt_player_alloc,
	   	connect_state_wait_for_client_settings,
		connect_state_wait_for_client_game_setup_ack,
		connect_state_player_state_sync,
		connect_state_unit_sync,
     };

    static ConnectionState      connection_state;
    static bool                 connection_lock_state;
    static PlayerID             connect_player_id;
    static PlayerState*         connect_player_state;
    static UnitSync*            connect_unit_sync;
    static Timer		time_out_timer;
    static int	                time_out_counter;
    static std::list<ConnectQueueElement> connect_queue;

    static bool inConnectQueue( PlayerID &new_player_id );

    static void connectFsm(const NetMessage* message);
    static void connectProcess(const NetMessage* message);
    static bool disconnectClient(PlayerID player_id);

    static void updateQueuedClients();

    static void netMessageClientDisconnect(const NetMessage* message);
    static void netPacketClientJoinRequest(const NetPacket* packet);

    static void sendConnectionAlert(PlayerID &player_id, int alert_enum);

    static void resetConnectFsm();

    // ** FSM States
    static bool connectStateIdle();
    static bool connectStateWaitForConnectRequest(const NetMessage* message);
    static bool connectStateAttemptPlayerAlloc();
    static bool connectStateWaitForClientSettings(const NetMessage* message);
    static bool connectStateWaitForClientGameSetupAck(const NetMessage* message);
    static bool connectStatePlayerStateSync();
    static bool connectStateUnitSync();

public:
    static void initialize( unsigned long max_players );

    static void startConnectDaemon( unsigned long max_players );

    static void shutdownConnectDaemon();

    static void startDisconnectionProcess( PlayerID player_id );
    static void startClientDropProcess( PlayerID player_id );

    static void connectProcess();

    static void processNetPacket(const NetPacket* message);

    static void lockConnectProcess();
    static void unlockConnectProcess();
    static bool getConnectLockStatus();

    static bool isConnecting();
};

#endif // ** _SERVERCONNECTDAEMON_HPP
