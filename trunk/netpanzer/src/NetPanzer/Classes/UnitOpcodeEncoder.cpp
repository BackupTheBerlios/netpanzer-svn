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
#include "NetworkServer.hpp"
#include "NetworkState.hpp"
#include "ConsoleInterface.hpp"
#include "OpcodeDebugger.hpp"
#include "UnitNetMessage.hpp"
#include "Util/Log.hpp"

UnitOpcodeEncoder::UnitOpcodeEncoder()
{
    reset();
}

UnitOpcodeEncoder::~UnitOpcodeEncoder()
{
}

void
UnitOpcodeEncoder::reset()
{
    opcode_index = 0;
}

void
UnitOpcodeEncoder::encode(const UnitOpcode *opcode)
{
    if(opcode_index + UnitOpcode::getSize() > sizeof(opcode_message.data)) {
        send();
    }
   
    memcpy(opcode_message.data + opcode_index, opcode, UnitOpcode::getSize());
    opcode_index += UnitOpcode::getSize();
}

void
UnitOpcodeEncoder::send()
{
    if (!opcode_index)
        return;

    size_t size = opcode_message.getHeaderSize() + opcode_index;
    SERVER->sendMessage(&opcode_message, size);

    reset();
    NetworkState::incOpcodesSent();
}

