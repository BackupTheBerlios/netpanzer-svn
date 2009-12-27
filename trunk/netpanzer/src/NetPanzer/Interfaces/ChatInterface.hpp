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
#ifndef _CHATINTERFACE_HPP
#define _CHATINTERFACE_HPP

#include "Classes/Network/ChatNetMessage.hpp"
#include "Core/CoreTypes.hpp"
class NetPacket;

class ChatInterface
{
protected:
    static ChatMesgRequest current_chat_mesg;
    static void (* addChatString)( const char *message_text );

    static void chatMessageRequest(const NetPacket* packet);
    static void chatMessage(const NetPacket* packet);

public:
    static void processChatMessages(const NetPacket* packet);

    static void say(const char * message);
    static void teamsay(const char * message);
    static void serversay(const char * message);
    static void serversayTo(const Uint16 player, const char * message);

    // ** ChatView Interface Methods
    static void setNewMessageCallBack( void (* addStringCallBack)( const char *message_text ) );
    static void setMessageScopeAll();
    static void setMessageScopeAllies();
    static void setMessageScopeEnemies();
    static void setMessageScopeServer();
    static void sendCurrentMessage(const char *message_text);
    static void sendQuickMessage(unsigned int n);
};

#endif // ** _CHATINTERFACE_HPP
