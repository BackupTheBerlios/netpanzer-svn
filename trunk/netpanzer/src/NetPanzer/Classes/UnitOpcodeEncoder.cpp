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
#include "OpcodeDebugger.hpp"
#include "UnitNetMessage.hpp"

void UnitOpcodeEncoder::initialize( int send_method )
{
    UnitOpcodeEncoder::send_method = send_method;
    opcode_message.reset();
}

void UnitOpcodeEncoder::encodeOpcode( UnitOpcode *opcode )
{
    opcode_message.add(opcode);

    if (opcode_message.isFull()) {
        sendOpcodeMessage();
    }
}

void UnitOpcodeEncoder::setDecodeMessage( UnitOpcodeMessage *message )
{
    memcpy( &decode_message, message, sizeof( UnitOpcodeMessage ) );
    current_decode_opcode = 0;
}

bool UnitOpcodeEncoder::decodeMessage( UnitOpcodeStruct *opcode )
{
    if (!decode_message.extract(current_decode_opcode, opcode))
        return false;

    current_decode_opcode++;

    NetworkState::incOpcodesReceived();

    return true;
}

void UnitOpcodeEncoder::sendOpcodeMessage( void )
{
    if (!opcode_message.isEmpty()) {
        if( send_method == _opcode_encoder_send_method_guarantee ) {
            SERVER->sendMessage( &opcode_message, opcode_message.realSize(), 0 );
        } else {
            SERVER->sendMessage( &opcode_message, opcode_message.realSize(), _network_send_no_guarantee );
        }

        opcode_message.reset();
    }

    NetworkState::incOpcodesSent();
}
