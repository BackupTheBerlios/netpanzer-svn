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

#include "PlayerState.hpp"
#include "ArrayUtil/QueueTemplate.hpp"
#include "NetPacket.hpp"
#include "ArrayUtil/Timer.hpp"

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

class ServerConnectQueue : public QueueTemplate< ConnectQueueElement >
{
public:

    void resetIterator( unsigned long *iterator )
    {
        *iterator = front;
    }

    ConnectQueueElement incIterator( unsigned long *iterator, bool *completed )
    {
        if ( *iterator == rear ) {
            *completed = true;
            return( array[ *iterator ] );
        };

        *iterator = ( *iterator + 1 ) % size;

        *completed = false;
        return( array[ *iterator ] );
    }

};

class ServerConnectDaemon
{
protected:
    static unsigned char      connection_state;
    static bool            connection_lock_state;
    static PlayerID           connect_player_id;
    static PlayerState        *connect_player_state;
    static Timer		         time_out_timer;
    static int	             time_out_counter;
    static ServerConnectQueue connect_queue;


    static bool inConnectQueue( PlayerID &new_player_id );

    static void connectFsm( NetMessage *message );
    static void connectProcess( NetMessage *message );
    static bool disconnectClient( PlayerID player_id );

    static void updateQueuedClients( void );

    static void netMessageClientDisconnect( NetMessage *message );
    static void netMessageClientJoinRequest( NetMessage *message );

    static void sendConnectionAlert( PlayerID &player_id, int alert_enum );

    static void resetConnectFsm( void );

    // ** FSM States
    static bool connectStateIdle( void );
    static bool connectStateWaitForConnectRequest( NetMessage *message );
    static bool connectStateAttemptPlayerAlloc( void );
    static bool connectStateWaitForClientSettings( NetMessage *message );
    static bool connectStateWaitForClientGameSetupAck( NetMessage *message );
    static bool connectStatePlayerStateSync( void );
    static bool connectStateUnitSync( void );

public:

    static void initialize( unsigned long max_players );

    static void startConnectDaemon( unsigned long max_players );

    static void shutdownConnectDaemon( void );

    static void startConnectionProcess( PlayerID new_player_id );

    static void startDisconnectionProcess( PlayerID player_id );
    static void startClientDropProcess( PlayerID player_id );

    static void connectProcess( void );

    static void processNetMessage( NetMessage *message );

    static void lockConnectProcess( void );
    static void unlockConnectProcess( void );
    static bool getConnectLockStatus( void );

    static bool isConnecting( void );
};

#endif // ** _SERVERCONNECTDAEMON_HPP
