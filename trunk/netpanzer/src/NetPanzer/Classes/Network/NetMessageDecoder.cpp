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

#include <string.h>
#include "NetMessageDecoder.hpp"
#include "Util/Log.hpp"

NetMessageDecoder::NetMessageDecoder()
{
    memset(&decode_message, 0, sizeof(decode_message));
}

NetMessageDecoder::~NetMessageDecoder()
{
}

void
NetMessageDecoder::setDecodeMessage(const NetMessage* message)
{
    if(message->getSize() > sizeof(decode_message)) {
        LOGGER.warning("Multimessage with wrong size!");
        memset(&decode_message, 0, sizeof(decode_message));
        return;
    }
    memcpy(&decode_message, message, message->getSize());
    decode_message_index = 0;
    decode_current_count = 0;
}

bool
NetMessageDecoder::decodeMessage(NetMessage **message)
{
    if(decode_message.getHeaderSize() + decode_message_index
            > decode_message.getSize()) {
        LOGGER.warning("Malformed Multimessage!");
        return false;
    }
    if(decode_current_count >= decode_message.message_count
            || decode_message.getHeaderSize() + decode_message_index
                    >= decode_message.getSize())
        return false;

    *message = (NetMessage *) (decode_message.data + decode_message_index);
    decode_message_index += (*message)->getSize();
    decode_current_count++;

    return true;
}

