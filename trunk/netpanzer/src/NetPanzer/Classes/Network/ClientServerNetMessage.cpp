/*
Copyright (C) 2004 Hollis Blanchard
 
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

#include <stdint.h>
#include "ClientServerNetMessage.hpp"

TransportClientAccept::TransportClientAccept()
{
    message_class = _net_message_class_client_server;
    message_id = _net_message_id_transport_client_accept;
}

uint32_t TransportClientAccept::getClientTransportID(void)
{
    return ltoh32(client_transport_id);
}

void TransportClientAccept::setClientTransportID(uint32_t transport_id)
{
    client_transport_id = htol32(transport_id);
}


ClientMesgConnectAck::ClientMesgConnectAck()
{
    message_class = _net_message_class_client_server;
    message_id = _net_message_id_client_connect_ack;
}

uint32_t ClientMesgConnectAck::getClientTransportID(void)
{
    return ltoh32(client_transport_id);
}

void ClientMesgConnectAck::setClientTransportID(uint32_t transport_id)
{
    client_transport_id = htol32(transport_id);
}


ServerMesgKeepAlive::ServerMesgKeepAlive()
{
    message_class = _net_message_class_client_server;
    message_id = _net_message_id_server_keep_alive;
}

uint16_t ServerMesgKeepAlive::getClientID(void)
{
    return ltoh16(client_id);
}

void ServerMesgKeepAlive::setClientID(uint16_t clientID)
{
    client_id = htol16(clientID);
}


ServerMesgPingRequest::ServerMesgPingRequest()
{
    message_class = _net_message_class_client_server;
    message_id = _net_message_id_server_ping_request;
}

uint16_t ServerMesgPingRequest::getClientID(void)
{
    return ltoh16(client_id);
}

void ServerMesgPingRequest::setClientID(uint16_t clientID)
{
    client_id = htol16(clientID);
}
