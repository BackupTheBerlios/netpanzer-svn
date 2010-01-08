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

#include <cstring>
#include "2D/Color.hpp"

#include "Interfaces/ChatInterface.hpp"
#include "Interfaces/ConsoleInterface.hpp"
#include "Interfaces/PlayerInterface.hpp"

#include "Classes/Network/NetworkState.hpp"
#include "Classes/Network/NetworkServer.hpp"
#include "Classes/Network/NetworkClient.hpp"

#include "Util/Log.hpp"
#include "GameConfig.hpp"
#include "Util/NTimer.hpp"

#include "Classes/Network/NetMessage.hpp"
#include "Classes/Network/NetPacket.hpp"

#include "Scripts/ScriptManager.hpp"

enum { _net_message_id_chat_mesg_req,
       _net_message_id_chat_mesg
};

enum { _chat_mesg_scope_player_set,
       _chat_mesg_scope_alliance,
       _chat_mesg_scope_enemies,
       _chat_mesg_scope_all,
       _chat_mesg_scope_server
};

#define CHATREQUEST_HEADER_LEN (sizeof(NetMessage) + sizeof(Uint8))
#define CHATMESG_HEADER_LEN (sizeof(NetMessage) + sizeof(Uint8) + sizeof(Uint16))
#define MAX_CHAT_MSG_LEN (_MAX_NET_PACKET_SIZE - CHATMESG_HEADER_LEN)

class ChatMesgRequest : public NetMessage
{
public:
    Uint8 message_scope;
    char message_text[MAX_CHAT_MSG_LEN];

    ChatMesgRequest()
    {
        reset();
    }

    void reset()
    {
        message_class = _net_message_class_chat;
        message_id = _net_message_id_chat_mesg_req;
        message_scope = _chat_mesg_scope_all;
    }

    int getTextLen() const
    {
        return getSize() - CHATREQUEST_HEADER_LEN;
    }
} __attribute__((packed));


class ChatMesg: public NetMessage
{
public:
    Uint8  message_scope;
private:
    Uint16 source_player_index;
public:
    char message_text[MAX_CHAT_MSG_LEN];

    ChatMesg()
    {
        message_class = _net_message_class_chat;
        message_id = _net_message_id_chat_mesg;
        memset(message_text, 0, sizeof(message_text));
    }

    int getTextLen() const
    {
        return getSize() - CHATMESG_HEADER_LEN;
    }

    Uint16 getSourcePlayerIndex() const
    {
        return SDL_SwapLE16(source_player_index);
    }
    void setSourcePlayerIndex(Uint16 playerIndex)
    {
        source_player_index = SDL_SwapLE16(playerIndex);
    }
} __attribute__((packed));

static void chatMessageRequest(const NetPacket* packet)
{
    const ChatMesgRequest* chat_request = (const ChatMesgRequest*) packet->getNetMessage();

    if (   chat_request->message_scope == _chat_mesg_scope_server
        && NetworkState::getNetworkStatus() == _network_state_server
        && packet->fromClient )
    {
        LOGGER.warning("Chat server message cheat from player %d", packet->fromPlayer);
        return;
    }

    bool post_on_server = false;
    ChatMesg chat_mesg;
    char text[MAX_CHAT_MSG_LEN+1];
    int text_len = chat_request->getTextLen();

    chat_mesg.setSourcePlayerIndex(packet->fromPlayer);
    chat_mesg.message_scope = chat_request->message_scope;
    memcpy(chat_mesg.message_text, chat_request->message_text, text_len);
    memcpy(text, chat_request->message_text, text_len);
    text[text_len] = 0;

    if( chat_request->message_scope == _chat_mesg_scope_all )
    {
        if ( text[0] != '/' || ! ScriptManager::runServerCommand(&text[1], packet->fromPlayer) )
        {
            NetworkServer::broadcastMessage(&chat_mesg, CHATMESG_HEADER_LEN + text_len);
            post_on_server = true;
        }
    }
    else if( chat_request->message_scope == _chat_mesg_scope_alliance )
    {
        unsigned long max_players;
        unsigned short local_player_index;
        PlayerState * player = 0;

        local_player_index = PlayerInterface::getLocalPlayerIndex();

        max_players = PlayerInterface::getMaxPlayers();
        for( unsigned long i = 0; i < max_players; i++ )
        {
            player = PlayerInterface::getPlayer( (unsigned short)i );

            if ( player->getStatus() == _player_state_active )
            {
                if( PlayerInterface::isAllied( packet->fromPlayer, i ) == true )
                {
                    if ( local_player_index != i )
                    {
                        NetworkServer::sendMessage((unsigned short)i, &chat_mesg,
                                                CHATMESG_HEADER_LEN + text_len);
                    }
                    else
                    {
                        post_on_server = true;
                    }
                }
            }
        }

        if( packet->fromPlayer == PlayerInterface::getLocalPlayerIndex() )
        {
            post_on_server = true;
        }
        else
        {
            NetworkServer::sendMessage( packet->fromPlayer,
                                 &chat_mesg, CHATMESG_HEADER_LEN + text_len);
        }
    }
    else if( chat_request->message_scope == _chat_mesg_scope_server )
    {
        NetworkServer::broadcastMessage(&chat_mesg, CHATMESG_HEADER_LEN + text_len);
        ConsoleInterface::postMessage(Color::unitAqua, false, 0, "Server: %s", text);
        return;
    }

    if (post_on_server == true)
    {
        PlayerState *player_state;

        player_state = PlayerInterface::getPlayer(chat_mesg.getSourcePlayerIndex() );

        IntColor color = Color::white;

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
        ConsoleInterface::postMessage(color, true, player_state->getFlag(), "%s: %s",
                player_state->getName().c_str(), text);
    }
}

static void chatMessage(const NetPacket* packet)
{
    unsigned short local_player_index;
    const ChatMesg *chat_mesg = (const ChatMesg*) packet->getNetMessage();

    if(chat_mesg->message_scope != _chat_mesg_scope_server 
            && chat_mesg->getSourcePlayerIndex() 
            >= PlayerInterface::getMaxPlayers()) {
        LOGGER.warning("malformed chatmessage packet.");
        return;
    }

    unsigned char text[MAX_CHAT_MSG_LEN+1];
    int text_len = chat_mesg->getTextLen();
    memcpy(text, chat_mesg->message_text, text_len);
    text[text_len] = 0;

    if( chat_mesg->message_scope == _chat_mesg_scope_server ) {
        ConsoleInterface::postMessage(Color::unitAqua, false, 0, "Server: %s", text );
        return;
    }

    local_player_index = PlayerInterface::getLocalPlayerIndex();

    PlayerState *player_state;

    player_state = PlayerInterface::getPlayer( chat_mesg->getSourcePlayerIndex() );

    IntColor color = Color::white;

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

    ConsoleInterface::postMessage(color, true, player_state->getFlag(), "%s: %s",
                                        player_state->getName().c_str(), text);
}

void ChatInterface::processChatMessages(const NetPacket* packet)
{
    switch(packet->getNetMessage()->message_id) {
        case _net_message_id_chat_mesg_req:
            chatMessageRequest(packet);
            break;

        case _net_message_id_chat_mesg:
            chatMessage(packet);
            break;

        default:
            LOGGER.warning("Received unknown chat message (id %d-%d)",
                            packet->getNetMessage()->message_class,
                            packet->getNetMessage()->message_id);
    }
}

void
ChatInterface::sendQuickMessage(unsigned int n)
{
    static NTimer sendtimer(2000);
    if ( ! sendtimer.isTimeOut() )
    {
        return; // can't send now
    }

    sendtimer.reset();

    char * msg = 0;
    switch ( n )
    {
        case 0:
            msg = (char *)gameconfig->quickchat_0.c_str();
            break;
        case 1:
            msg = (char *)gameconfig->quickchat_1.c_str();
            break;
        case 2:
            msg = (char *)gameconfig->quickchat_2.c_str();
            break;
        case 3:
            msg = (char *)gameconfig->quickchat_3.c_str();
            break;
        case 4:
            msg = (char *)gameconfig->quickchat_4.c_str();
            break;
        case 5:
            msg = (char *)gameconfig->quickchat_5.c_str();
            break;
        case 6:
            msg = (char *)gameconfig->quickchat_6.c_str();
            break;
        case 7:
            msg = (char *)gameconfig->quickchat_7.c_str();
            break;
        case 8:
            msg = (char *)gameconfig->quickchat_8.c_str();
            break;
        case 9:
            msg = (char *)gameconfig->quickchat_9.c_str();
            break;
        default:
            ; // nothing
    }

    if ( msg )
    {
        say(msg);
    }
}

void
ChatInterface::say(const char *message)
{
    int text_len = strlen(message);
    ChatMesgRequest cmsg;
    memcpy(cmsg.message_text, message, text_len );
    cmsg.message_scope = _chat_mesg_scope_all;
    NetworkClient::sendMessage(&cmsg, CHATREQUEST_HEADER_LEN+text_len);
}

void
ChatInterface::teamsay(const char* message)
{
    int text_len = strlen(message);
    ChatMesgRequest cmsg;
    memcpy( cmsg.message_text, message, text_len );
    cmsg.message_scope = _chat_mesg_scope_alliance;
    NetworkClient::sendMessage(&cmsg, CHATREQUEST_HEADER_LEN+text_len);
}

void
ChatInterface::serversay(const char* message)
{
    int text_len = strlen(message);
    ChatMesgRequest cmsg;
    strncpy( cmsg.message_text, message, text_len );
    cmsg.message_scope = _chat_mesg_scope_server;
//    NetworkServer::broadcastMessage(&cmsg, sizeof(cmsg));
    NetworkClient::sendMessage(&cmsg, CHATREQUEST_HEADER_LEN+text_len);
}

void
ChatInterface::serversayTo(const Uint16 player, const char* message)
{
    if ( player == PlayerInterface::getLocalPlayerIndex() )
    {
        ConsoleInterface::postMessage(Color::unitAqua, false, 0, "Server: %s", message );
    }
    else
    {
        int text_len = strlen(message);
        ChatMesg cmsg;
        cmsg.setSourcePlayerIndex(PlayerInterface::getLocalPlayerIndex());
        strncpy( cmsg.message_text, message, text_len );
        cmsg.message_scope = _chat_mesg_scope_server;
        NetworkServer::sendMessage(player, &cmsg, CHATMESG_HEADER_LEN+text_len);
    }
}

