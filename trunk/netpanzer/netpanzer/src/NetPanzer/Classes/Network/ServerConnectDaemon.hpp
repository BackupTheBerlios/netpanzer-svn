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

#include "Classes/PlayerState.hpp"
#include "ArrayUtil/QueueTemplate.hpp"
#include "Classes/Network/NetPacket.hpp"
#include "Network/ClientSocket.hpp"
#include "Util/Timer.hpp"
#include "UnitSync.hpp"

class ServerConnectDaemon
{
protected:
	enum ConnectionState {
		connect_state_idle,
	 	connect_state_wait_for_connect_request,
	   	connect_state_wait_for_client_settings,
		connect_state_wait_for_client_game_setup_ack,
		connect_state_player_state_sync,
		connect_state_unit_sync
     };

    static ConnectionState      connection_state;
    static bool                 connection_lock_state;
    static ClientSocket         *connect_client;
    static UnitSync             *connect_unit_sync;
    static Timer		time_out_timer;
    static int	                time_out_counter;
    static Timer                sendunitpercent_timer;
    
    typedef std::list<ClientSocket *> ConnectQueue;
    typedef ConnectQueue::iterator ConnectQueueIterator;
    static ConnectQueue connect_queue;
    
    static void connectFsm(const NetMessage* message);
    static void connectProcess(const NetMessage* message);

    static void updateQueuedClients();

    static void netPacketClientDisconnect(const NetPacket* packet);
    static void netPacketClientJoinRequest(const NetPacket* packet);

    static void sendConnectionAlert( ClientSocket * client );

    static void resetConnectFsm();

    // ** FSM States
    static bool connectStateIdle();
    static bool connectStateWaitForConnectRequest(const NetMessage* message);
    static bool connectStateWaitForClientSettings(const NetMessage* message);
    static bool connectStateWaitForClientGameSetupAck(const NetMessage* message);
    static bool connectStatePlayerStateSync();
    static bool connectStateUnitSync();

public:
    static void initialize( unsigned long max_players );

    static void startConnectDaemon( unsigned long max_players );

    static void shutdownConnectDaemon();

    static bool inConnectQueue( ClientSocket *client );

    static void connectProcess();

    static void processNetPacket(const NetPacket* message);

    static void lockConnectProcess();
    static void unlockConnectProcess();
    static bool getConnectLockStatus();

    static bool isConnecting();
    static void removeClientFromQueue(ClientSocket *client);
};

#endif // ** _SERVERCONNECTDAEMON_HPP
