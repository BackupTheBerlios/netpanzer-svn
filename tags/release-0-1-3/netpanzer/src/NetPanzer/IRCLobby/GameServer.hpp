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
#ifndef __GameServer_h__
#define __GameServer_h__

#include <ctime>
#include <string>

//---------------------------------------------------------------------------
class GameServer
{
public:
    GameServer(const std::string& newhost, int newport,
            const std::string& newuser, 
            const std::string& newrealuser, 
            const std::string& newmap,
            int newplayercount, int newmaxplayers)
        : host(newhost), port(newport), irc_user(newuser), 
            real_user(newrealuser), map(newmap),
            playercount(newplayercount), max_players(newmaxplayers)
    {
        std::time(&last_update);
    }

    GameServer::GameServer(const GameServer& other)
        : host(other.host), port(other.port), irc_user(other.irc_user), 
          real_user(other.real_user), map(other.map),
          last_update(other.last_update), playercount(other.playercount),
          max_players(other.max_players)
    { }
    
    std::string host;
    int port;
    std::string irc_user;
    std::string real_user;
    std::string map;
    std::time_t last_update;
    int playercount;
    int max_players;
};

#endif
