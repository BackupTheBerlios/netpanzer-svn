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

#include <string>

#include "GameConfig.hpp"
#include "IRCLobbyServer.hpp"
#include "Util/Log.hpp"

IRCLobbyServer::IRCLobbyServer(const std::string& servername,
         const std::string& nickname,
         const std::string& channel) : IRCLobby(servername,nickname,channel)
{ }
IRCLobbyServer::~IRCLobbyServer() { }

// send server info to someone
void IRCLobbyServer::sendServerInfo(const std::string& dest)
{
    std::stringstream buffer;

    buffer << PlayerInterface::countPlayers()
           << "/" << PlayerInterface::getMaxPlayers()
           << " port:" << gameconfig->serverport
           << " map:" << gameconfig->map;
    sendServerRunningMess(dest,buffer.str());
}

void IRCLobbyServer::notifyStartup()
{
    if(gameconfig->hostorjoin== _game_session_host) {
        // tell everyone the server just started
        sendServerInfo(getChannelName());
    }
    else if(gameconfig->hostorjoin== _game_session_join) {
        refreshServerList();
    }
}

void IRCLobbyServer::addChatMessage(const std::string& user, const std::string& message)
{
    if(gameconfig->hostorjoin== _game_session_host) {
        LOG(("%s:%s",user.c_str(),message.c_str()));
    }
    IRCLobby::addChatMessage(user,message);
}




