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
#ifndef __IRCLobby_h__
#define __IRCLobby_h__

#include <SDL.h>
#include <SDL_thread.h>
#include <SDL_net.h>

#include "View.hpp"
#include "Surface.hpp"
#include "GameServer.hpp"
#include "GameServerList.hpp"
#include "IRCChatMessage.hpp"

class IRCLobbyView;

//---------------------------------------------------------------------------
class IRCLobby
{
    friend class IRCLobbyView;

    static const char server_running_mess[];
    static const char ask_server_running_mess[];
    IPaddress irc_server;
    TCPsocket irc_server_sock;
    char *channel_name;
    SDL_Thread *running_thread;
    int quit_thread;
    LinkListDoubleTemplate<IRCChatMessage> chat_messages;
    GameServerList *game_servers;

    int sendServerInfo(const char *dest);
    // read any messages that need to be processed
    int processMessage();
    int sendIRCLine(char *line);
    int sendIRCMessageLine(char *line,const char *to);
    int readIRCLine(char *buf,int buf_len);
    int connectToServer();
    void addChatMessage(const char *m,const char *u);


public:
    SDL_mutex *game_servers_mutex;

    IRCLobby(IPaddress *addr);
    ~IRCLobby();

    int sendChatMessage(const char *line);
    int sendIRCMessageLine(char *line);
    void stopThread();
    int refreshServerList();
    int startMessagesThread();
    int processMessages();
};

#endif
