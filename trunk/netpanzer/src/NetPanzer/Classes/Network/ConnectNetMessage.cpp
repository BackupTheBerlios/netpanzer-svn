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

#include <string.h>
#include <stdint.h>

#include "ConnectNetMessage.hpp"
#include "Util/Endian.hpp"

ClientConnectJoinRequest::ClientConnectJoinRequest()
{
    message_class = _net_message_class_connect;
    message_id = _net_message_id_connect_join_game_request;
    memset(code_word, 0, sizeof(code_word));
}

uint32_t ClientConnectJoinRequest::getTransportID(void)
{
    return ltoh32(client_transport_id);
}

void ClientConnectJoinRequest::setTransportID(uint32_t transport_id)
{
    client_transport_id = htol32(transport_id);
}

uint32_t ClientConnectJoinRequest::getProtocolVersion(void)
{
    return ltoh32(protocol_version);
}

void ClientConnectJoinRequest::setProtocolVersion(uint32_t version)
{
    protocol_version = htol32(version);
}

ClientConnectJoinRequestAck::ClientConnectJoinRequestAck()
{
    message_class = _net_message_class_connect;
    message_id = _net_message_id_connect_join_game_request_ack;
}

int ClientConnectJoinRequestAck::getResultCode(void)
{
    return ltoh32(result_code);
}

void ClientConnectJoinRequestAck::setResultCode(int result)
{
    result_code = htol32(result);
}

uint32_t ClientConnectJoinRequestAck::getServerProtocolVersion(void)
{
    return ltoh32(server_protocol_version);
}

void ClientConnectJoinRequestAck::setServerProtocolVersion(uint32_t protocol_version)
{
    server_protocol_version = htol32(protocol_version);
}


ConnectProcessUpdate::ConnectProcessUpdate()
{
    message_class = _net_message_class_connect;
    message_id = _net_message_id_client_connect_process_update;
}

uint32_t ConnectProcessUpdate::getQueuePosition(void)
{
    return ltoh32(queue_position);
}

void ConnectProcessUpdate::setQueuePosition(uint32_t position)
{
    queue_position = htol32(position);
}


ConnectProcessStateMessage::ConnectProcessStateMessage()
{
    message_class = _net_message_class_connect;
    message_id = _net_message_id_client_connect_process_state_mesg;
}

uint32_t ConnectProcessStateMessage::getMessageEnum(void)
{
    return ltoh32(message_enum);
}

void ConnectProcessStateMessage::setMessageEnum(uint32_t message)
{
    message_enum = htol32(message);
}

int32_t ConnectProcessStateMessage::getPercentComplete(void)
{
    return ltoh32(percent_complete);
}

void ConnectProcessStateMessage::setPercentComplete(int32_t percent)
{
    percent_complete = htol32(percent);
}


ConnectClientSettings::ConnectClientSettings()
{
    message_class = _net_message_class_connect;
    message_id = _net_message_id_connect_client_settings;
    memset(&player_name, 0, sizeof(player_name));
}

int16_t ConnectClientSettings::getPlayerFlag(void)
{
    return ltoh16(player_flag);
}

void ConnectClientSettings::set(const char *player_name, unsigned char unit_color, short player_flag )
{
    strncpy(ConnectClientSettings::player_name, player_name, 64);
    ConnectClientSettings::player_name[63] = '\0';
    ConnectClientSettings::unit_color = unit_color;
    ConnectClientSettings::player_flag = player_flag;
}
