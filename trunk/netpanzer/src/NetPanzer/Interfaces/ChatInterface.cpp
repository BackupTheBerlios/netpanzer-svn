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

#include "ChatInterface.hpp"
#include "2D/Color.hpp"
#include "PlayerInterface.hpp"
#include "NetworkState.hpp"
#include "Server.hpp"
#include "Client.hpp"

#include "ConsoleInterface.hpp"

SystemChatMesgRequest ChatInterface::current_chat_mesg;
void (* ChatInterface::addChatString)( const char *message_text ) = 0;


void ChatInterface::chatMessageRequest( NetMessage *message )
{
    bool post_on_server = false;
    SystemChatMesg chat_mesg;
    SystemChatMesgRequest *chat_request = (SystemChatMesgRequest *) message;

    chat_mesg.setSourcePlayerIndex(chat_request->getSourcePlayerIndex());
    chat_mesg.message_scope = chat_request->message_scope;
    strcpy( chat_mesg.message_text, chat_request->message_text );

    if( chat_request->message_scope == _chat_mesg_scope_all ) {
        SERVER->sendMessage( &chat_mesg, sizeof(SystemChatMesg), 0 );
        post_on_server = true;
    } else
        if( chat_request->message_scope == _chat_mesg_scope_alliance ) {
            unsigned long max_players;
            unsigned short local_player_index;
            PlayerID player_id;

            local_player_index = PlayerInterface::getLocalPlayerIndex();

            max_players = PlayerInterface::getMaxPlayers();
            for( unsigned long i = 0; i < max_players; i++ ) {
                player_id = PlayerInterface::getPlayerID( i );

                if ( (PlayerInterface::getPlayer(i)->getStatus() == _player_state_active) ) {
                    if( PlayerInterface::isAllied( chat_request->getSourcePlayerIndex(), i ) == true ) {
                        if ( (local_player_index != i) ) {
                            SERVER->sendMessage( player_id, &chat_mesg, sizeof(SystemChatMesg), 0 );
                        } else {
                            post_on_server = true;
                        }
                    }
                }
            }

            if( chat_request->getSourcePlayerIndex() == PlayerInterface::getLocalPlayerIndex() ) {
                post_on_server = true;
            } else {
                SERVER->sendMessage(
                    PlayerInterface::getPlayerID(chat_request->getSourcePlayerIndex()),
                    &chat_mesg, sizeof(SystemChatMesg), 0);
            }
        } else
            if( chat_request->message_scope == _chat_mesg_scope_server ) {
                SERVER->sendMessage( &chat_mesg, sizeof(SystemChatMesg), 0 );
                ConsoleInterface::postMessage("Server :: %s",
                        chat_mesg.message_text );
                return;
            }

    if ( post_on_server == true ) {

        PlayerState *player_state;

        player_state = PlayerInterface::getPlayerState( chat_mesg.getSourcePlayerIndex() );

        if( (addChatString != 0) ) {
            char mesg_str[256];
            sprintf( mesg_str, " ---- %s ----", player_state->getName().c_str() );

            addChatString( mesg_str );
            addChatString( chat_mesg.message_text );
        }

        PIX color = Color::white;

        switch ( chat_request->message_scope ) {
        case _chat_mesg_scope_all :
            color = Color::white;
            break;

        case _chat_mesg_scope_alliance :
            color = Color::orange;
            break;

        case _chat_mesg_scope_server :
            color = Color::unitAqua;
            break;

        } // ** switch

        // TODO add unitcolor
        ConsoleInterface::postMessage("%s :: %s",
                player_state->getName().c_str(), chat_mesg.message_text );
    }
}

void ChatInterface::chatMessage( NetMessage *message )
{
    unsigned short local_player_index;
    SystemChatMesg *chat_mesg;

    chat_mesg = (SystemChatMesg * ) message;

    if( chat_mesg->message_scope == _chat_mesg_scope_server ) {
        ConsoleInterface::postMessage("Server :: %s", chat_mesg->message_text );
        return;
    }

    local_player_index = PlayerInterface::getLocalPlayerIndex();

    PlayerState *player_state;

    player_state = PlayerInterface::getPlayerState( chat_mesg->getSourcePlayerIndex() );

    if ( (addChatString != 0) ) {
        char mesg_str[144];
        sprintf( mesg_str, " ---- %s ----", player_state->getName().c_str() );

        addChatString( mesg_str );
        addChatString( chat_mesg->message_text );
    } // ** if

    PIX color = Color::white;

    switch ( chat_mesg->message_scope ) {
    case _chat_mesg_scope_all :
        color = Color::white;
        break;

    case _chat_mesg_scope_alliance :
        color = Color::orange;
        break;

    case _chat_mesg_scope_server :
        color = Color::unitAqua;
        break;

    } // ** switch

    ConsoleInterface::postMessage("%s :: %s",
            player_state->getName().c_str(), chat_mesg->message_text );
}

void ChatInterface::processChatMessages( NetMessage *message )
{
    switch( message->message_id ) {
    case _net_message_id_system_chat_mesg_req :
        chatMessageRequest( message );
        break;

    case _net_message_id_system_chat_mesg :
        chatMessage( message );
        break;

    } // ** switch
}


void ChatInterface::setNewMessageCallBack( void (* addStringCallBack )( const char *message_text ) )
{
    addChatString = addStringCallBack;
}

void ChatInterface::setMessageScopeAll( void )
{
    current_chat_mesg.setMessageScope( _chat_mesg_scope_all );
}

void ChatInterface::setMessageScopeAllies( void )
{
    current_chat_mesg.setMessageScope( _chat_mesg_scope_alliance );
}

void ChatInterface::setMessageScopeEnemies( void )
{
    current_chat_mesg.setMessageScope( _chat_mesg_scope_enemies );
}

void ChatInterface::setMessageScopeServer( void )
{
    current_chat_mesg.setMessageScope( _chat_mesg_scope_server );
}

void ChatInterface::sendCurrentMessage( const char *message_text )
{
    current_chat_mesg.setSourcePlayerIndex(PlayerInterface::getLocalPlayerIndex());
    strncpy( current_chat_mesg.message_text, message_text, 149 );
    current_chat_mesg.message_text[ 149 ] = 0;

    if ( NetworkState::status == _network_state_client ) {
        CLIENT->sendMessage( &current_chat_mesg, sizeof(SystemChatMesgRequest), 0 );
    } else {
        processChatMessages( &current_chat_mesg );
    }

    current_chat_mesg.reset();
}
