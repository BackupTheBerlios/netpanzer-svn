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

#include <sstream>

#include "Log.hpp"
#include "IRCLobbyView.hpp"
#include "IPAddressView.hpp"
#include "Desktop.hpp"
#include "GameViewGlobals.hpp"
#include "GameConfig.hpp"

#include "Client.hpp"

IRCLobbyView::IRCLobbyView() 
    : View()
{
    lobby_connection=0;
    lobby_view_height=160;
    total_displayed_servers=0;
    setSearchName("IRCLobbyView");
    setTitle("Lobby");
    setSubTitle("");

    setAllowResize(false);
    setAllowMove(false);
    setVisible(false);

    moveTo(iXY(bodyTextRect.max.x-400, bodyTextRect.min.y + 200));

    iXY  area_size = iXY(400, lobby_view_height);
    resizeClientArea(area_size);

    int chat_y=lobby_view_height-(Surface::getFontHeight()*2);
    //addButtonCenterText(iXY(320, chat_y ), 80,  "Chat", "", buttonChat);
    addButtonCenterText(iXY(320, chat_y), 80,  "Refresh", "", buttonRefresh);
    szChat.init("  ", 34);
    cInputField* input = addInputField(iXY(4, chat_y), &szChat, "", true);
    input->setReturnAction(chatReturnPressed);

    // XXX ugly ugly ugly
    if(!lobby_view)
        lobby_view = this;
} 


// doDraw
//---------------------------------------------------------------------------
void IRCLobbyView::doDraw(Surface &viewArea, Surface &clientArea)
{
    assert(this!=0);
    clientArea.fill(Color::black);
    startIRC();  // start irc if needed
    
    int y=0;
    int disp_server_upto=0;
    int server_list_end_y=lobby_view_height-(Surface::getFontHeight()*6);
    int chat_list_end_y=server_list_end_y+(Surface::getFontHeight()*4);      

// XXX todo: scrollbar for large list of servers
    if(lobby_connection == 0 || !lobby_connection->isConnected()) {
        clientArea.bltString(iXY(0,0),"Not connected to lobby", Color::white);
        View::doDraw(viewArea, clientArea);
        return;
    }
    else {
        SDL_mutexP(lobby_connection->game_servers_mutex);
        
        GameServerList::iterator i;
        GameServerList* serverlist = lobby_connection->game_servers;
        disp_server_upto=0;
        for(i=serverlist->begin(); i!=serverlist->end(); i++) {
            const GameServer* server = &(*i);

            std::stringstream playerstr;
            playerstr << server->playercount << "/" << server->max_players;
            
            clientArea.bltString(iXY(0,y),server->user.c_str(), Color::white);
            clientArea.bltString(iXY(140,y),playerstr.str().c_str(), Color::white);
            clientArea.bltString(iXY(200,y),server->map.c_str(), Color::white);
            
            displayed_servers[disp_server_upto++]=server;
            y += Surface::getFontHeight();
            if(y >= server_list_end_y)
                break;
        }
        SDL_mutexV(lobby_connection->game_servers_mutex);
        total_displayed_servers=disp_server_upto;
    }

    std::list<IRCChatMessage>::reverse_iterator m;
    y = chat_list_end_y - Surface::getFontHeight();
    for(m = lobby_connection->chat_messages.rbegin();
        m != lobby_connection->chat_messages.rend(); m++) {

        std::stringstream temp;
        temp << m->getUser() << ": " << m->getMessage();
        clientArea.bltString(iXY(4,y), temp.str().c_str(), Color::white);
        
        y-=Surface::getFontHeight();
        if(y < server_list_end_y)
            break;
    }
    
    View::doDraw(viewArea, clientArea);
}

int IRCLobbyView::lMouseUp(const iXY &down_pos,const iXY &up_pos)
{
    assert(this!=0);
    int idx_down=down_pos.y/Surface::getFontHeight();
    int idx=up_pos.y/Surface::getFontHeight();
    if(idx>=0 && idx_down==idx && idx<total_displayed_servers) {
        const GameServer *server=displayed_servers[idx];
        assert(server!=0);

        // connect to this game
        std::stringstream server_host;
        server_host << server->host.c_str() <<":"<<server->port;
        IPAddressView::szServer.setString(server_host.str().c_str());
    }
    return View::lMouseUp(down_pos,up_pos);
}

void IRCLobbyView::buttonRefresh()
{
    if(!lobby_view->lobby_connection)
        return;
    
    lobby_view->lobby_connection->refreshServerList();
    lobby_view->lobby_connection->refreshUserList();
}

void IRCLobbyView::chatReturnPressed(cInputField* )
{
    lobby_view->lobby_connection->sendChatMessage(
        gameconfig->playername,
        lobby_view->szChat.getString());
    lobby_view->szChat.setString("");
}

// connect to remote irc
//---------------------------------------------------------------------------
void IRCLobbyView::startIRC()
{
    if(lobby_connection) {
        // already started
        return; 
    }
    if((const std::string&) gameconfig->lobbyserver == "")
        return;

    try {
        stopIRC();
        lobby_connection=new IRCLobby(gameconfig->lobbyserver,
                gameconfig->playername, "#netpanzerlob");
    } catch(std::exception& e) {
        LOG(("Couldn't connect to irc lobby: %s", e.what()));
        error_message = e.what();
    }
}

void IRCLobbyView::stopIRC()
{
    if(lobby_connection) {
        delete lobby_connection;
    }
    lobby_connection=0;
}

IRCLobbyView* lobby_view = 0;
