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
#ifndef _NETMESSAGEENCODER_HPP
#define _NETMESSAGEENCODER_HPP

#include "NetPacket.hpp"

class NetMessageEncoder
{
protected:
    MultiMessage encode_message;
    MultiMessage decode_message;
    unsigned long encode_message_index;
    unsigned long decode_message_index;
    unsigned char decode_current_count;

public:

    void initalize( void );
    void resetEncoder( void );
    void encodeMessage( NetMessage *message, unsigned short size );

    bool NetMessageEncoder::encodeMessage( NetMessage *message,
                                           unsigned short size,
                                           MultiMessage **encoded_message );

    void getEncodeMessage( MultiMessage **message );

    void setDecodeMessage( MultiMessage *message );
    bool decodeMessage( NetMessage **message );

    void sendEncodedMessage( void );
};


extern NetMessageEncoder PUBLIC_MESSAGE_ENCODER;


#endif // ** _NETMESSAGEENCODER_HPP
