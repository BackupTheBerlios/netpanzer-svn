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
#include "UnitOpcodeEncoder.hpp"

#include "Server.hpp"
#include "NetworkState.hpp"

#include "ConsoleInterface.hpp"

void UnitOpcodeEncoder::initialize( int send_method )
{
    UnitOpcodeEncoder::send_method = send_method;
    resetEncoder();
}

void UnitOpcodeEncoder::resetEncoder( void )
{
    opcode_message.code_size = 0;
    opcode_message.opcode_count = 0;
    memset( opcode_message.data, 0, _OPCODE_MESSAGE_LIMIT );
    opcode_message.message_class = _net_message_class_unit;
    opcode_message.message_id = _net_message_id_opcode_mesg;
}

void UnitOpcodeEncoder::encodeOpcode( UnitOpcode *opcode )
{
    unsigned char *current_code_ptr;

    if ( ( opcode_message.code_size + sizeof( UnitOpcodeStruct ) ) >
            _OPCODE_MESSAGE_LIMIT
       ) {
        if( send_method == _opcode_encoder_send_method_guarantee ) {
            SERVER->sendMessage( &opcode_message, opcode_message.realSize(), 0 );
        } else {
            SERVER->sendMessage( &opcode_message, opcode_message.realSize(), _network_send_no_guarantee );
        }

        resetEncoder( );
    }

    current_code_ptr =  ( (unsigned char *) &opcode_message.data) + opcode_message.code_size;
    memmove( current_code_ptr, opcode, sizeof( UnitOpcodeStruct ) );

    opcode_message.code_size += sizeof( UnitOpcodeStruct );
    opcode_message.opcode_count++;

    NetworkState::incOpcodesSent();

}

void UnitOpcodeEncoder::setDecodeMessage( UnitOpcodeMessage *message )
{
    memmove( &decode_message, message, sizeof( UnitOpcodeMessage ) );
    current_decode_opcode = 0;
}

bool UnitOpcodeEncoder::decodeMessage( UnitOpcodeStruct *opcode )
{
    unsigned char *current_code_ptr;

    if ( current_decode_opcode == decode_message.opcode_count )
        return ( false );

    current_code_ptr = ( (unsigned char *) &decode_message.data)
                       + sizeof(UnitOpcodeStruct)*current_decode_opcode;

    memmove( opcode, current_code_ptr, sizeof(UnitOpcodeStruct) );

    current_decode_opcode++;

    NetworkState::incOpcodesReceived();
    return( true );
}

void UnitOpcodeEncoder::sendOpcodeMessage( void )
{
    if ( opcode_message.opcode_count > 0 ) {
        if( send_method == _opcode_encoder_send_method_guarantee ) {
            SERVER->sendMessage( &opcode_message, opcode_message.realSize(), 0 );
        } else {
            SERVER->sendMessage( &opcode_message, opcode_message.realSize(), _network_send_no_guarantee );
        }

        resetEncoder( );
    }

}
