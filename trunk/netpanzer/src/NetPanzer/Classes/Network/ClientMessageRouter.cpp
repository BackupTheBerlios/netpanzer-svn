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
#include "ClientMessageRouter.hpp"

#include "Client.hpp"
#include "NetworkClient.hpp"
#include "ClientConnectDaemon.hpp"

#include "GameManager.hpp"

#include "UnitNetMessage.hpp"
#include "PlayerNetMessage.hpp"
#include "SystemNetMessage.hpp"
#include "ConnectNetMessage.hpp"
#include "ObjectiveNetMessage.hpp"

#include "ObjectiveInterface.hpp"
#include "UnitInterface.hpp"
#include "PlayerInterface.hpp"
#include "ChatInterface.hpp"
#include "GameControlRulesDaemon.hpp"
#include "PowerUpInterface.hpp"

#include "NetworkState.hpp"

NetMessage * ClientMessageRouter::temp_message;
NetMessageDecoder ClientMessageRouter::message_decoder;

void ClientMessageRouter::initialize( void )
{
    temp_message = (NetMessage *) malloc( sizeof( NetMessageStruct ) );
}

void ClientMessageRouter::cleanUp( void )
{
    free( temp_message );
}

void ClientMessageRouter::routeMessages( void )
{
    ClientConnectDaemon::connectProcess( );

    if( NetworkState::ping_timer.count() ) {
        //GameManager::requestNetworkPing();
    }

    while( CLIENT->getMessage( temp_message ) == true ) {
        if ( temp_message->message_class == _net_message_class_multi ) {
            NetMessage *message;
            message_decoder.setDecodeMessage( (MultiMessage *) temp_message );

            while(message_decoder.decodeMessage(&message)) {
                switch ( message->message_class ) {
                case _net_message_class_unit : {
                        UnitInterface::processNetMessage( message );
                    }
                    break;

                case _net_message_class_player : {
                        PlayerInterface::processNetMessage( message );
                    }
                    break;

                case _net_message_class_system :
                    GameManager::processSystemMessage( message );
                    ChatInterface::processChatMessages( message );
                    break;

                case _net_message_class_connect :
                    ClientConnectDaemon::processNetMessage( message );
                    break;

                case _net_message_class_objective :
                    ObjectiveInterface::processNetMessages( message );
                    break;

                case _net_message_class_game_control :
                    GameControlRulesDaemon::processNetMessage( message );
                    break;

                case _net_message_class_powerup :
                    PowerUpInterface::processNetMessages( message );
                    break;

                } // ** switch

            } // ** while

        } // ** if


        switch ( temp_message->message_class ) {
        case _net_message_class_unit : {
                UnitInterface::processNetMessage( temp_message );
            }
            break;

        case _net_message_class_player : {
                PlayerInterface::processNetMessage( temp_message );
            }
            break;

        case _net_message_class_system :
            GameManager::processSystemMessage( temp_message );
            ChatInterface::processChatMessages( temp_message );
            break;

        case _net_message_class_connect :
            ClientConnectDaemon::processNetMessage( temp_message );
            break;

        case _net_message_class_objective :
            ObjectiveInterface::processNetMessages( temp_message );
            break;

        case _net_message_class_game_control :
            GameControlRulesDaemon::processNetMessage( temp_message );
            break;

        case _net_message_class_powerup :
            PowerUpInterface::processNetMessages( temp_message );
            break;

        } // ** switch

    } // ** while
}



