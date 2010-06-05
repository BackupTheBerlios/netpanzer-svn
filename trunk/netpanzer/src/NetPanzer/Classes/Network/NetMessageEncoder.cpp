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

#include "Classes/Network/NetMessageEncoder.hpp"

#include <string.h>

#include "Classes/Network/NetworkState.hpp"
#include "Classes/Network/NetworkServer.hpp"
#include "NetworkClient.hpp"

NetMessageEncoder::NetMessageEncoder(bool sendAsClient)
{
    this->sendAsClient = sendAsClient;
    usePlayerID = false;
    resetEncoder();
}

NetMessageEncoder::NetMessageEncoder(PlayerID player_index)
{
    usePlayerID = true;
    sendAsClient = false;
    player = player_index;
    resetEncoder();
}

NetMessageEncoder::~NetMessageEncoder()
{
}

void NetMessageEncoder::resetEncoder()
{
    encode_message.message_class = _net_message_class_multi ;
    encode_message.message_id = 0;
    encode_message.message_count = 0;
    memset(encode_message.data, 0, _MULTI_PACKET_LIMIT);
    encode_message_index = 0;
}

void NetMessageEncoder::encodeMessage(NetMessage *message, size_t size)
{
    if(encode_message_index + size > _MULTI_PACKET_LIMIT
            || encode_message.message_count == 255) {
        sendEncodedMessage();

        resetEncoder();
    }

    message->setSize(size);
    memcpy(encode_message.data + encode_message_index, message, size);

    encode_message_index += size;
    encode_message.message_count++;
}

void NetMessageEncoder::sendEncodedMessage()
{
    if (encode_message.message_count > 0) {
        size_t size = encode_message_index + encode_message.getHeaderSize();
        if(usePlayerID) {
            SERVER->sendMessage(player, &encode_message, size);
        } else if(sendAsClient) {
            CLIENT->sendMessage(&encode_message, size);
        } else if(NetworkState::status == _network_state_server) {
            SERVER->broadcastMessage(&encode_message, size);
        } else {
            CLIENT->sendMessage(&encode_message, size);
        }

        resetEncoder();
    }
}

