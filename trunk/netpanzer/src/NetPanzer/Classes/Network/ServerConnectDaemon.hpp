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

class ServerConnectDaemon
{
protected:
    static int          connection_state;
    static bool         connection_lock_state;
    static ClientSocket *connect_client;
    static Timer	time_out_timer;
    static int	        time_out_counter;
    static Timer        sendunitpercent_timer;
    static int          sync_count;
    static int          sync_end;
    static int          sync_total;
    static int          sync_done;
    
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
    static void connectStateIdle();
    static void connectStateWaitForConnectRequest(const NetMessage* message);
    static void connectStateWaitForClientSettings(const NetMessage* message);
    static void connectStateWaitForClientGameSetupAck(const NetMessage* message);
    static void connectStateUnitProfileSync( const NetMessage* message);
    static void connectStatePlayerStateSync();
    static void connectStateSyncFlags();
    static void connectStateUnitSync();

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
