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
#ifndef _CLIENT_SERVER_NETMESSAGE_HPP
#define _CLIENT_SERVER_NETMESSAGE_HPP

#include <stdint.h>

#include "NetPacket.hpp"
#include "PlayerID.hpp"

enum { _net_message_id_transport_client_accept,
       _net_message_id_client_connect_ack,
       _net_message_id_client_server_id,
       _net_message_id_client_keep_alive,
       _net_message_id_client_set_keepalive_state,
       _net_message_id_server_client_disconnect,
       _net_message_id_server_keep_alive,
       _net_message_id_server_ping_request,
       _net_message_id_client_ping_ack
     };

#ifdef MSVC
#pragma pack(1)
#endif

class TransportClientAccept : public NetMessage
{
private:
    uint32_t client_transport_id;
    
public:
    TransportClientAccept()
    {
        message_class = _net_message_class_client_server;
        message_id = _net_message_id_transport_client_accept;
    }

    uint32_t getClientTransportID() const
    {
        return ltoh32(client_transport_id);
    }
    void setClientTransportID(uint32_t transport_id)
    {
        client_transport_id = htol32(transport_id);
    }
}
__attribute__((packed));

class ClientMesgConnectAck : public NetMessage
{
private:
    uint32_t client_transport_id;
    
public:
    ClientMesgConnectAck()
    {
        message_class = _net_message_class_client_server;
        message_id = _net_message_id_client_connect_ack;
    }
    
    uint32_t getClientTransportID() const
    {
        return ltoh32(client_transport_id);
    }
    void setClientTransportID(uint32_t transport_id)
    {
        client_transport_id = htol32(transport_id);
    }
}
__attribute__((packed));

class ClientMesgKeepAlive : public NetMessage
{
public :
    ClientMesgKeepAlive()
    {
        message_class = _net_message_class_client_server;
        message_id = _net_message_id_client_keep_alive;
    }
} __attribute__((packed));

class ClientMesgSetKeepAlive : public NetMessage
{
public :
    bool keep_alive_state;

    ClientMesgSetKeepAlive()
    {
        message_class = _net_message_class_client_server;
        message_id = _net_message_id_client_set_keepalive_state;
    }
} __attribute__((packed));


class ServerMesgKeepAlive: public NetMessage
{
private:
    uint16_t client_id;

public:
    ServerMesgKeepAlive()
    {
        message_class = _net_message_class_client_server;
        message_id = _net_message_id_server_keep_alive;
    }
    
    uint16_t getClientID() const
    {
        return ltoh16(client_id);
    }
    void setClientID(uint16_t clientID)
    {
        client_id = htol16(clientID);
    }
} __attribute__((packed));

class ServerMesgPingRequest: public NetMessage
{
private:
    uint16_t client_id;

public:
    ServerMesgPingRequest()
    {
        message_class = _net_message_class_client_server;
        message_id = _net_message_id_server_ping_request;
    }
    
    uint16_t getClientID() const
    {
        return ltoh16(client_id);
    }
    void setClientID(uint16_t clientID)
    {
        client_id = htol16(clientID);
    }
} __attribute__((packed));

class ClientMesgPingAck: public NetMessage
{
public:
    ClientMesgPingAck()
    {
        message_class = _net_message_class_client_server;
        message_id = _net_message_id_client_ping_ack;
    }
} __attribute__((packed));

#ifdef MSVC
#pragma pack()
#endif

#endif // ** _CLIENT_SERVER_NETMESSAGE_HPP
