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

#include <assert.h>
#include "Log.hpp"
#include "NetworkGlobals.hpp"
#include "NetworkState.hpp"
#include "Exception.hpp"
#include "NetworkServerUnix.hpp"

NetworkServerUnix::NetworkServerUnix()
  : NetworkServer(), serversocket(0)
{
}

NetworkServerUnix::~NetworkServerUnix()
{
	delete serversocket;
}

void NetworkServerUnix::shutdownClientTransport(const PlayerID &client_id)
{
	assert(serversocket != 0);
	serversocket->removeClient(client_id.getNetworkID());
}

void NetworkServerUnix::openSession()
{
}

void NetworkServerUnix::hostSession()
{
	delete serversocket;
	serversocket = new ServerSocket(_NETPANZER_DEFAULT_PORT_TCP, 	
									_NETPANZER_DEFAULT_PORT_UDP);
}

void NetworkServerUnix::closeSession()
{
  	delete serversocket;
	serversocket = 0;
}

void NetworkServerUnix::sendMessage(ServerClientListData *client_data_ptr,
									const PlayerID& player_id,
									NetMessage* message, int flags)
{
	if( flags & _network_send_no_guarantee )
	{
		if(client_data_ptr==0) {
			client_data_ptr = client_list.getClientData(player_id);
			assert(client_data_ptr != 0);
		}
		message->sequence = client_data_ptr->no_guarantee_sequence_counter;
		client_data_ptr->no_guarantee_sequence_counter++;

		serversocket->sendMessage(
				player_id.getNetworkID(),
				(char*) message, message->size, false);
	}
	else
	{
		serversocket->sendMessage(
				player_id.getNetworkID(),
				(char *) message, message->size, true);
	}

	NetworkState::incPacketsSent(message->size); 
}

int NetworkServerUnix::sendMessage(NetMessage *message, size_t size,
								   int flags)
{
	message->size = size;
 
	LOG( ( "SEND >> Class: %d ID: %d", message->message_class,
									   message->message_id ) );
  
	ServerClientListData *iterator = 0;
	ServerClientListData *client_data_ptr = 0;

	client_list.resetIterator( &iterator );

	client_data_ptr = client_list.incIteratorPtr( &iterator );
  
	while( client_data_ptr != 0 )
	{
		try {
			sendMessage(client_data_ptr, client_data_ptr->client_id,
						message, flags);
		} catch(Exception e) {
			LOG( ("Error while sending network packet.") );
			//return -1;
		}

		client_data_ptr = client_list.incIteratorPtr(&iterator);
	}
  
   	return 0;
}

int NetworkServerUnix::sendMessage( NetMessage *message, size_t size,
									const PlayerID &player_id, int flags )
{
	message->size = size;

	LOG( ( "SEND >> Class: %d ID: %d", message->message_class,
									   message->message_id ) );

	try {
		sendMessage(0, player_id, message, flags);
	} catch(Exception e) {
		LOG( ("Error while sending network packet.") );
		//return -1;
	}
  
	return 0;
}

int NetworkServerUnix::getMessage(NetMessage *message)
{
   	updateKeepAliveState();
      
	if (loop_back_recv_queue.isReady() )
	{
		loop_back_recv_queue.dequeue( &net_packet );
		memmove( (void *) message, net_packet.data, net_packet.packet_size );

		return true;
	}
	else
	{
		if ( receive_queue.isReady() )
		{
			receive_queue.dequeue( &net_packet );
			
			memmove(  (void *) message, net_packet.data, net_packet.packet_size );
			NetworkState::incPacketsReceived( net_packet.packet_size );
	    
			LOG( ( "RECV >> Class: %d ID: %d", message->message_class,
											   message->message_id ) );
        
			if ( message->message_class == _net_message_class_client_server )
			{ processNetMessage( message ); }

			return true;
		}
	} // ** else 
    
	return false;
}

void NetworkServerUnix::checkIncoming()
{
	if(serversocket)
		serversocket->read();
}
