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
#ifndef __IRCLobbyView_h__
#define __IRCLobbyView_h__

#include "IRCLobby.hpp"
#include "View.hpp"
#include "Surface.hpp"
#include "GameServer.hpp"

//---------------------------------------------------------------------------
class IRCLobbyView : public View
{
 int lobby_view_height;
 const GameServer *mouse_down_server;
 const GameServer *displayed_servers[64];
 int total_displayed_servers;
public:
 static std::string lobby_server;
 static cInputFieldString szChat;
 static IRCLobby *lobby_connection;
 IRCLobbyView();

 static void startIRC();
 static void stopIRC();

 virtual void doDraw(Surface &windowArea, Surface &clientArea); 
 virtual int lMouseUp(const iXY &up_pos,const iXY &down_pos);
};

#endif

