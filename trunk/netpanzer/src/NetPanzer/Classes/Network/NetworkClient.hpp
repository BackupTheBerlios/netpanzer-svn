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
#ifndef _NETWORK_CLIENT_HPP
#define _NETWORK_CLIENT_HPP

#include "codewiz.hpp"
#include "NetworkInterface.hpp"
#include "NetworkReturnCodes.hpp"

#include "Timer.hpp"
#include "PlayerID.hpp"
#include "ArrayTemplate.hpp"

enum { _connection_loop_back,
	   _connection_network
	 };

enum { _connection_status_no_connection,
       _connection_status_connected
     };

class ConnectionAddress
 {
  public:
   char string_rep[256];
 };


class SessionInfo
 {
  public:
   char name[64];
   char map[64];
   char game_type[64];
   int  current_players;
   int  max_players;
 };

typedef ArrayTemplate< SessionInfo > SessionList;


#define _CLIENT_KEEP_ALIVE_SEND_INTERVAL   (2)  // in seconds
#define _SERVER_KEEP_ALIVE_THRESHOLD     (120)  // in seconds
#define _SERVER_PING_INTERVAL              (5)  // in seconds

class NetworkClient : public NetworkInterface
{
protected:
 	NetPacket net_packet;

  	SessionList session_list;
   
   	Timer   ping_timer;
   
	bool keep_alive_state;
	Timer   keep_alive_emit_timer;
	Timer   keep_alive_timer;
	void updateKeepAliveState( void );

	DWORD client_transport_id;

	unsigned short connection_type;
	unsigned short connection_status;
   
	void netMessageClientKeepAlive( NetMessage *message );
	void netMessageClientSetKeepAliveState( NetMessage *message ); 
	void netMessageClientPingAck( NetMessage *message ); 
	void netMessageClientConnectAck( NetMessage *message ); 

	void processNetMessage( NetMessage *message );
public:
 	NetworkClient ();
  	virtual ~NetworkClient ();
   
   	void activateKeepAlive( void ); 
	void deactivateKeepAlive( void ); 

	virtual int openSession() = 0;

	virtual int startEnumeration( ConnectionAddress address ) = 0;
	virtual int startEnumeration( void ) = 0;
	virtual int stopEnumeration( void ) = 0;
	virtual int getSessionList( SessionList &list ) = 0;

	virtual int joinSession( void ) = 0;
	virtual int joinSession( int session_index ) = 0;
	virtual int joinSession( const char session_name ) = 0;
	virtual int setJoinSession( const char *session_name ) = 0;
      
	virtual int closeSession( void ) = 0;
	
	virtual int sendMessage( NetMessage *message, unsigned long size, int flags
			) = 0;
 
   virtual int getMessage( NetMessage *message ) = 0;
};

#endif // ** _NETWORK_CLIENT_HPP
