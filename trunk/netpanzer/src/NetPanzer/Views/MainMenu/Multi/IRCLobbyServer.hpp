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
#ifndef __IRCLobbyServer_h__
#define __IRCLobbyServer_h__

#include <string>

#include "IRCLobby.hpp"
#include "PlayerInterface.hpp"

//---------------------------------------------------------------------------
class IRCLobbyServer : public IRCLobby
{
public:
    IRCLobbyServer(const std::string& servername,
             const std::string& nickname,
             const std::string& channel);

    virtual ~IRCLobbyServer();

    // send server info to someone
    virtual void sendServerInfo(const std::string& dest);
    virtual void notifyStartup();
    virtual void addChatMessage(const std::string& user, const std::string& message);
};


#endif

