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
#include "ServerMessageRouter.hpp"

#include "Server.hpp"
#include "NetworkServer.hpp"
#include "ServerConnectDaemon.hpp"
#include "UnitInterface.hpp"
#include "PlayerInterface.hpp"
#include "ObjectiveInterface.hpp"
#include "GameManager.hpp"
#include "ChatInterface.hpp"

#include "TerminalNetMesg.hpp"
#include "SystemNetMessage.hpp"
#include "ConnectNetMessage.hpp"
#include "PlayerNetMessage.hpp"
#include "Util/Log.hpp"


NetPacket ServerMessageRouter::temp_packet;
NetMessageDecoder ServerMessageRouter::message_decoder;

void ServerMessageRouter::initialize()
{
}

void ServerMessageRouter::cleanUp()
{
}

void ServerMessageRouter::processTerminalPacket(const NetPacket* packet)
{
    const NetMessage* message = packet->getNetMessage();
    switch(message->message_id) {
        case _net_message_id_term_unit_cmd:
            UnitInterface::processNetPacket(packet);
            break;

        case _net_message_id_term_unit_gen: {
            const TerminalOutpostUnitGenRequest* terminal_command
                = (const TerminalOutpostUnitGenRequest*) message;
            ObjectiveInterface::sendMessage( &(terminal_command->unit_gen_request) );
            break;
        }

        case _net_message_id_term_output_loc: {
            const TerminalOutpostOutputLocRequest* terminal_command
                = (const TerminalOutpostOutputLocRequest*) message;
            ObjectiveInterface::sendMessage( &(terminal_command->output_loc_request));
            break;
        }
        default:
            LOGGER.warning("unnown Terminal Message (id %d, player %u)",
                    message->message_id, packet->fromID);
    }
}

void ServerMessageRouter::routePacket(const NetPacket* packet)
{
    const NetMessage* message = packet->getNetMessage();
    switch (message->message_class) {
        case _net_message_class_terminal:
            processTerminalPacket(packet);
            break;

        case _net_message_class_system:
            GameManager::processSystemMessage(message);
            break;
            
        case _net_message_class_chat:
            ChatInterface::processChatMessages(message);
            break;

        case _net_message_class_connect:
            ServerConnectDaemon::processNetPacket(packet);
            break;

        case _net_message_class_player:
            PlayerInterface::processNetMessage(message);
            break;

        case _net_message_class_client_server:
            break;

        default:
            LOGGER.warning("Packet contained unknown message class: %d",
                    message->message_class);
    }
}

void ServerMessageRouter::routeMessages()
{
    ServerConnectDaemon::connectProcess();

    while(SERVER->getPacket(&temp_packet) == true) {
        const NetMessage* message = temp_packet.getNetMessage();
        if (message->message_class == _net_message_class_multi) {
            message_decoder.setDecodeMessage((const MultiMessage *) message);

            NetPacket packet;
            packet.toID = temp_packet.toID;
            packet.fromID = temp_packet.fromID;
            NetMessage* mmessage;
            while(message_decoder.decodeMessage(&mmessage)) {
                memcpy(packet.data, mmessage, mmessage->getSize());
                routePacket(&packet);
            }
        } else {
            routePacket(&temp_packet);
        }
    }
}
