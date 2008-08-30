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

#include "2D/Color.hpp"

#include "Interfaces/ChatInterface.hpp"
#include "Interfaces/ConsoleInterface.hpp"
#include "Interfaces/PlayerInterface.hpp"
#include "Classes/Network/NetworkState.hpp"
#include "Classes/Network/NetworkServer.hpp"
#include "Classes/Network/NetworkClient.hpp"
#include "Classes/Network/ChatNetMessage.hpp"
#include "Util/Log.hpp"


ChatMesgRequest ChatInterface::current_chat_mesg;
void (* ChatInterface::addChatString)( const char *message_text ) = 0;

void ChatInterface::chatMessageRequest(const NetMessage* message)
{
    bool post_on_server = false;
    ChatMesg chat_mesg;
    const ChatMesgRequest* chat_request = (const ChatMesgRequest*) message;

    if(chat_request->message_scope != _chat_mesg_scope_server
            && chat_request->getSourcePlayerIndex() 
            >= PlayerInterface::getMaxPlayers())
    {
        LOGGER.warning("Invalid chatMessageRequest");
        return;
    }

    chat_mesg.setSourcePlayerIndex(chat_request->getSourcePlayerIndex());
    chat_mesg.message_scope = chat_request->message_scope;
    snprintf(chat_mesg.message_text, sizeof(chat_mesg.message_text), "%s",
             chat_request->message_text);

    if( chat_request->message_scope == _chat_mesg_scope_all ) {
        SERVER->sendMessage(&chat_mesg, sizeof(ChatMesg));
        post_on_server = true;
    } else if( chat_request->message_scope == _chat_mesg_scope_alliance ) {
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
                        SERVER->sendMessage(player_id, &chat_mesg,
                                sizeof(ChatMesg));
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
                    &chat_mesg, sizeof(ChatMesg));
        }
    } else if( chat_request->message_scope == _chat_mesg_scope_server ) {
        SERVER->sendMessage(&chat_mesg, sizeof(ChatMesg));
        ConsoleInterface::postMessage(Color::unitAqua, "Server: %s",
                chat_mesg.message_text );
        return;
    }

    if (post_on_server == true) {
        PlayerState *player_state;

        player_state = PlayerInterface::getPlayerState(chat_mesg.getSourcePlayerIndex() );

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
        ConsoleInterface::postMessage(color, "%s: %s",
                player_state->getName().c_str(), chat_mesg.message_text );
    }
}

void ChatInterface::chatMessage(const NetMessage* message)
{
    unsigned short local_player_index;
    const ChatMesg *chat_mesg = (const ChatMesg*) message;

    if(chat_mesg->message_scope != _chat_mesg_scope_server 
            && chat_mesg->getSourcePlayerIndex() 
            >= PlayerInterface::getMaxPlayers()) {
        LOGGER.warning("malformed chatmessage packet.");
        return;
    }

    if( chat_mesg->message_scope == _chat_mesg_scope_server ) {
        ConsoleInterface::postMessage(Color::unitAqua, "Server: %s", chat_mesg->message_text );
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

    ConsoleInterface::postMessage(color, "%s: %s",
            player_state->getName().c_str(), chat_mesg->message_text );
}

void ChatInterface::processChatMessages(const NetMessage* message)
{
    switch(message->message_id) {
        case _net_message_id_chat_mesg_req:
            chatMessageRequest(message);
            break;

        case _net_message_id_chat_mesg:
            chatMessage(message);
            break;

        default:
            LOGGER.warning("Received unknown chat message (id %d-%d)",
                    message->message_class, message->message_id);
    }
}


void ChatInterface::setNewMessageCallBack( void (* addStringCallBack )( const char *message_text ) )
{
    addChatString = addStringCallBack;
}

void ChatInterface::setMessageScopeAll()
{
    current_chat_mesg.message_scope =  _chat_mesg_scope_all;
}

void ChatInterface::setMessageScopeAllies()
{
    current_chat_mesg.message_scope = _chat_mesg_scope_alliance;
}

void ChatInterface::setMessageScopeEnemies()
{
    current_chat_mesg.message_scope = _chat_mesg_scope_enemies;
}

void ChatInterface::setMessageScopeServer()
{
    current_chat_mesg.message_scope = _chat_mesg_scope_server;
}

void ChatInterface::sendCurrentMessage( const char *message_text )
{
    current_chat_mesg.setSourcePlayerIndex(PlayerInterface::getLocalPlayerIndex());
    strncpy( current_chat_mesg.message_text, message_text, 149 );
    current_chat_mesg.message_text[ 149 ] = 0;

    if ( NetworkState::status == _network_state_client ) {
        CLIENT->sendMessage(&current_chat_mesg, sizeof(ChatMesgRequest));
    } else {
        processChatMessages(&current_chat_mesg);
    }

    current_chat_mesg.reset();
}

