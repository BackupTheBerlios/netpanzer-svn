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

#include <list>

#include <SDL.h>
#include <SDL_thread.h>
#include <SDL_net.h>

#include "GameServer.hpp"
#include "GameServerList.hpp"
#include "IRCChatMessage.hpp"

class IRCLobby;
class IRCLobbyView;

class NotifyIRCChangeName {
public:
    virtual void changeIRCName(std::string &newname) = 0;
    friend class IRCLobby;
};


//---------------------------------------------------------------------------
class IRCLobby
{
public:
    IRCLobby(const std::string& servername,
             const std::string& nickname,
             const std::string& channel);
    ~IRCLobby();

    void sendChatMessage(const std::string& user, const std::string& line);
    void sendIRCMessageLine(const std::string& line);
    void refreshServerList();
    void refreshUserList();

    bool isConnected() const
    { return irc_server_socket != 0; }
    void changeNickName(const std::string &nick);
    void restartThread();   // restart thread & reconnect irc
    void stopThread();

    GameServerList* game_servers;
    NotifyIRCChangeName* change_name;

private:
    void startMessagesThread();

    void processMessages();
    static int messagesThreadEntry(void* t);

    void sendServerInfo(const std::string& dest);
    // read any messages that need to be processed
    void processMessage();
    void sendIRCLine(const std::string& line);
    void sendIRCMessageLine(const std::string& line, const std::string& to);
    void readIRCLine(char *buf, size_t buf_len);
    void connectToServer();
    void sendLoginInfo();
    void setNickName(const std::string &nick);
    void sendNickName();
    void sendPingMessage();
    void addChatMessage(const std::string& user, const std::string& message);

    SDL_mutex *game_servers_mutex;   

    friend class IRCLobbyView;
    TCPsocket irc_server_socket;

    std::string channel_name;
    std::string nickname;
    std::string serveraddress;
    std::string server_host;
    std::list<IRCChatMessage> chat_messages;
    unsigned int expected_ping;
    
    SDL_Thread *running_thread;
};

#endif