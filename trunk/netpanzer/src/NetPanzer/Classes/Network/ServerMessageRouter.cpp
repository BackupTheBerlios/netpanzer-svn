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


NetMessage * ServerMessageRouter::temp_message;
NetMessageDecoder ServerMessageRouter::message_decoder;

void ServerMessageRouter::initialize( void )
{
    temp_message = (NetMessage *) malloc( sizeof( NetMessageStruct ) );
}

void ServerMessageRouter::cleanUp( void )
{
    free( temp_message );
}

void ServerMessageRouter::classTerminalMessages( NetMessage *message )
{
    switch( message->message_id ) {
    case _net_message_id_term_unit_cmd : {
            TerminalUnitCmdRequest *terminal_command;
            terminal_command = (TerminalUnitCmdRequest *) message;

            UnitInterface::sendMessage( &(terminal_command->comm_request) );
        }
        break;

    case _net_message_id_term_unit_gen : {
            TerminalOutpostUnitGenRequest *terminal_command;
            terminal_command = (TerminalOutpostUnitGenRequest *) message;

            ObjectiveInterface::sendMessage( &(terminal_command->unit_gen_request) );
        }
        break;

    case _net_message_id_term_output_loc: {
            TerminalOutpostOutputLocRequest * terminal_command;
            terminal_command = (TerminalOutpostOutputLocRequest *) message;
            ObjectiveInterface::sendMessage( &(terminal_command->output_loc_request));
        }
        break;

    } // ** switch

}


void ServerMessageRouter::routeMessages()
{
    ServerConnectDaemon::connectProcess();

    while( SERVER->getMessage( temp_message ) == true ) {
        if ( temp_message->message_class == _net_message_class_multi ) {
            NetMessage *message;
            message_decoder.setDecodeMessage( (MultiMessage *) temp_message );

            while( message_decoder.decodeMessage( &message ) ) {
                switch ( message->message_class ) {
                case _net_message_class_terminal :
                    classTerminalMessages( message );
                    break;

                case _net_message_class_system :
                    GameManager::processSystemMessage( message );
                    ChatInterface::processChatMessages( message );
                    break;

                case _net_message_class_connect :
                    ServerConnectDaemon::processNetMessage( message );
                    break;

                case _net_message_class_player :
                    PlayerInterface::processNetMessage( message );
                    break;
                }
            }
        }

        switch ( temp_message->message_class ) {
        case _net_message_class_terminal :
            classTerminalMessages( temp_message );
            break;

        case _net_message_class_system :
            GameManager::processSystemMessage( temp_message );
            ChatInterface::processChatMessages( temp_message );
            break;

        case _net_message_class_connect :
            ServerConnectDaemon::processNetMessage( temp_message );
            break;

        case _net_message_class_player :
            PlayerInterface::processNetMessage( temp_message );
            break;
        }
    }
}
