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

#include "2D/Color.hpp"

#include "Interfaces/ChatInterface.hpp"
#include "Interfaces/ConsoleInterface.hpp"
#include "Interfaces/PlayerInterface.hpp"
#include "Classes/Network/NetworkState.hpp"
#include "Classes/Network/NetworkServer.hpp"
#include "Classes/Network/NetworkClient.hpp"
//#include "Classes/Network/ChatNetMessage.hpp"
#include "Util/Log.hpp"

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
    char player_set[32];

private:
    Uint16 source_player_index;
public:
    char message_text[150];

// XPROTO
//public:
//    Uint8 message_scope;
//    char message_text[MAX_CHAT_MSG_LEN];

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

    Uint16 getSourcePlayerIndex() const
	{
		return SDL_SwapLE16(source_player_index);
	}

    void setSourcePlayerIndex(Uint16 playerIndex)
	{
		source_player_index = SDL_SwapLE16(playerIndex);
	}
} __attribute__((packed));


class ChatMesg: public NetMessage
{
public:
    Uint8  message_scope;
private:
    Uint16 source_player_index;
public:
//    char message_text[MAX_CHAT_MSG_LEN]; // XPROTO
    char message_text[150];

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

static void chatMessageRequest(const NetMessage* message) {
	bool post_on_server = false;
	ChatMesg chat_mesg;
	const ChatMesgRequest* chat_request = (const ChatMesgRequest*) message;

	if (chat_request->message_scope != _chat_mesg_scope_server
			&& chat_request->getSourcePlayerIndex()
					>= PlayerInterface::getMaxPlayers()) {
		LOGGER.warning("Invalid chatMessageRequest");
		return;
	}

	chat_mesg.setSourcePlayerIndex(chat_request->getSourcePlayerIndex());
	chat_mesg.message_scope = chat_request->message_scope;
	snprintf(chat_mesg.message_text, sizeof(chat_mesg.message_text), "%s",
			chat_request->message_text);

	if (chat_request->message_scope == _chat_mesg_scope_all) {
		SERVER->broadcastMessage(&chat_mesg, sizeof(ChatMesg));
		post_on_server = true;
	} else if (chat_request->message_scope == _chat_mesg_scope_alliance) {
		unsigned long max_players;
		unsigned short local_player_index;
		PlayerState * player = 0;

		local_player_index = PlayerInterface::getLocalPlayerIndex();

		max_players = PlayerInterface::getMaxPlayers();
		for (unsigned long i = 0; i < max_players; i++) {
			player = PlayerInterface::getPlayer((unsigned short) i);

			if (player->getStatus() == _player_state_active) {
				if (PlayerInterface::isAllied(
						chat_request->getSourcePlayerIndex(), i) == true) {
					if (local_player_index != i) {
						SERVER->sendMessage((unsigned short) i, &chat_mesg,
								sizeof(ChatMesg));
					} else {
						post_on_server = true;
					}
				}
			}
		}

		if (chat_request->getSourcePlayerIndex()
				== PlayerInterface::getLocalPlayerIndex()) {
			post_on_server = true;
		} else {
			SERVER->sendMessage(chat_request->getSourcePlayerIndex(),
					&chat_mesg, sizeof(ChatMesg));
		}
	} else if (chat_request->message_scope == _chat_mesg_scope_server) {
		SERVER->broadcastMessage(&chat_mesg, sizeof(ChatMesg));
		ConsoleInterface::postMessage(Color::unitAqua, false, 0, "Server: %s",
				chat_mesg.message_text);
		return;
	}

	if (post_on_server == true) {
		PlayerState *player_state;

		player_state = PlayerInterface::getPlayer(
				chat_mesg.getSourcePlayerIndex());

		PIX color = Color::white;

		switch (chat_request->message_scope) {
		case _chat_mesg_scope_all:
			color = Color::white;
			break;

		case _chat_mesg_scope_alliance:
			color = Color::orange;
			break;

		case _chat_mesg_scope_server:
			color = Color::unitAqua;
			break;

		} // ** switch

		// TODO add unitcolor
		ConsoleInterface::postMessage(color, true, player_state->getFlag(),
				"%s: %s", player_state->getName().c_str(),
				chat_mesg.message_text);
	}
}

static void chatMessage(const NetMessage* message) {
	unsigned short local_player_index;
	const ChatMesg *chat_mesg = (const ChatMesg*) message;

	if (chat_mesg->message_scope != _chat_mesg_scope_server
			&& chat_mesg->getSourcePlayerIndex()
					>= PlayerInterface::getMaxPlayers()) {
		LOGGER.warning("malformed chatmessage packet.");
		return;
	}

	if (chat_mesg->message_scope == _chat_mesg_scope_server) {
		ConsoleInterface::postMessage(Color::unitAqua, false, 0, "Server: %s",
				chat_mesg->message_text);
		return;
	}

	local_player_index = PlayerInterface::getLocalPlayerIndex();

	PlayerState *player_state;

	player_state
			= PlayerInterface::getPlayer(chat_mesg->getSourcePlayerIndex());

	PIX color = Color::white;

	switch (chat_mesg->message_scope) {
	case _chat_mesg_scope_all:
		color = Color::white;
		break;

	case _chat_mesg_scope_alliance:
		color = Color::orange;
		break;

	case _chat_mesg_scope_server:
		color = Color::unitAqua;
		break;

	} // ** switch

	ConsoleInterface::postMessage(color, true, player_state->getFlag(),
			"%s: %s", player_state->getName().c_str(), chat_mesg->message_text);
}

void ChatInterface::processChatMessages(const NetMessage* message) {
	switch (message->message_id) {
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

void ChatInterface::say(const char *message)
{
	unsigned int text_len = strlen(message);
	ChatMesgRequest cmsg;

	if ( text_len >= sizeof(cmsg.message_text) )
	{
		text_len = sizeof(cmsg.message_text)-1;
	}
	memcpy(cmsg.message_text, message, text_len);
	cmsg.message_text[text_len] = 0;

	cmsg.message_scope = _chat_mesg_scope_all;

	cmsg.setSourcePlayerIndex( PlayerInterface::getLocalPlayerIndex() );

	if (NetworkState::status == _network_state_client)
	{
		CLIENT->sendMessage(&cmsg, sizeof(cmsg));
	}
	else
	{
		processChatMessages(&cmsg);
	}

	// XPROTO
//	NetworkClient::sendMessage(&cmsg, CHATREQUEST_HEADER_LEN + text_len);
}

void ChatInterface::teamsay(const char* message)
{
	unsigned int text_len = strlen(message);
	ChatMesgRequest cmsg;

	if ( text_len >= sizeof(cmsg.message_text) )
	{
		text_len = sizeof(cmsg.message_text)-1;
	}
	memcpy(cmsg.message_text, message, text_len);
	cmsg.message_text[text_len] = 0;

	cmsg.message_scope = _chat_mesg_scope_alliance;

	cmsg.setSourcePlayerIndex( PlayerInterface::getLocalPlayerIndex() );

	if (NetworkState::status == _network_state_client)
	{
		CLIENT->sendMessage(&cmsg, sizeof(cmsg));
	}
	else
	{
		processChatMessages(&cmsg);
	}

	// XPROTO
//	NetworkClient::sendMessage(&cmsg, CHATREQUEST_HEADER_LEN + text_len);
}

void ChatInterface::serversay(const char* message)
{
	unsigned int text_len = strlen(message);
	ChatMesgRequest cmsg;

	if ( text_len >= sizeof(cmsg.message_text) )
	{
		text_len = sizeof(cmsg.message_text)-1;
	}
	memcpy(cmsg.message_text, message, text_len);
	cmsg.message_text[text_len] = 0;

	cmsg.message_scope = _chat_mesg_scope_server;

	cmsg.setSourcePlayerIndex( PlayerInterface::getLocalPlayerIndex() );

	if (NetworkState::status == _network_state_client)
	{
		CLIENT->sendMessage(&cmsg, sizeof(cmsg));
	}
	else
	{
		processChatMessages(&cmsg);
	}

	// XPROTO
	// unused    NetworkServer::broadcastMessage(&cmsg, sizeof(cmsg));
//	NetworkClient::sendMessage(&cmsg, CHATREQUEST_HEADER_LEN + text_len);
}

void ChatInterface::serversayTo(const int player, const char* message)
{
	if (player == PlayerInterface::getLocalPlayerIndex())
	{
		ConsoleInterface::postMessage(Color::unitAqua, false, 0, "Server: %s",
				message);
	}
	else
	{
		unsigned int text_len = strlen(message);
		ChatMesg cmsg;
		cmsg.setSourcePlayerIndex(PlayerInterface::getLocalPlayerIndex());
		if ( text_len >= sizeof(cmsg.message_text) )
		{
			text_len = sizeof(cmsg.message_text)-1;
		}
		memcpy(cmsg.message_text, message, text_len);
		cmsg.message_text[text_len] = 0;

		cmsg.message_scope = _chat_mesg_scope_server;

		SERVER->sendMessage(player, &cmsg, sizeof(cmsg));

//		NetworkServer::sendMessage(player, &cmsg, CHATMESG_HEADER_LEN
//				+ text_len);
	}
}

