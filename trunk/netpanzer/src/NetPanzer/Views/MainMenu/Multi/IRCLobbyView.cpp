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
#include <SDL_net.h>

#include "Log.hpp"
#include "IRCLobbyView.hpp"
#include "IPAddressView.hpp"
#include "Desktop.hpp"
#include "GameViewGlobals.hpp"
#include "GameConfig.hpp"

#include "Client.hpp"



IRCLobby *IRCLobbyView::lobby_connection=NULL;

cInputFieldString IRCLobbyView::szChat;


static void buttonRefresh(void)
{
    IRCLobbyView::lobby_connection->refreshServerList();
}

static void buttonChat(void)
{
    IRCLobbyView::lobby_connection->sendChatMessage(
        IRCLobbyView::szChat.getString());
    IRCLobbyView::szChat.setString("");
}


// IRCLobbyView
//---------------------------------------------------------------------------
IRCLobbyView::IRCLobbyView() : View()
{
    lobby_view_height=160;
    mouse_down_server=NULL;
    total_displayed_servers=0;
    setSearchName("IRCLobbyView");
    setTitle("Lobby");
    setSubTitle("");

    setAllowResize(false);
    setAllowMove(false);
    setVisible(false);

    moveTo(iXY(bodyTextRect.min.x, bodyTextRect.min.y + 200));

    iXY  area_size = iXY(400, lobby_view_height);
    resizeClientArea(area_size);

    int chat_y=lobby_view_height-(Surface::getFontHeight()*2);
    addButtonCenterText(iXY(320, chat_y ), 80,  "Chat", "", buttonChat);
    addButtonCenterText(iXY(320, chat_y-(Surface::getFontHeight()*2) ), 80,  "Refresh", "", buttonRefresh);
    szChat.init("  ", 34);
    addInputField(iXY(4, chat_y), &szChat, "", true);
} 


// doDraw
//---------------------------------------------------------------------------
void IRCLobbyView::doDraw(Surface &viewArea, Surface &clientArea)
{
    assert(this!=NULL);
    clientArea.fill(Color::black);
    int y=0;
    int bucket_upto=0;
    int disp_server_upto=0;

    if(lobby_connection==NULL) { return; }

    LinkListDoubleTemplate<GameServer> *bucket=NULL;
    GameServer *server=NULL;

//~~~ todo: scrollbar for large list of servers
    SDL_mutexP(lobby_connection->game_servers_mutex);
    int server_list_end_y=lobby_view_height-(Surface::getFontHeight()*6);
    int chat_list_end_y=server_list_end_y+(Surface::getFontHeight()*4);
    for(; y<server_list_end_y; y+=Surface::getFontHeight()) {
        while(server==NULL && bucket_upto<GameServerList::max_buckets) {
            bucket=lobby_connection->game_servers->getBucket(bucket_upto++);
            server=bucket->getFront();
        }
        if(server==NULL) { break; }

        char players_str[256];
        sprintf(players_str,"%i/%i",server->players,server->max_players);
        clientArea.bltString(iXY(0,y),server->user, Color::white);
        clientArea.bltString(iXY(140,y),players_str, Color::white);
        clientArea.bltString(iXY(200,y),server->map, Color::white);
        displayed_servers[disp_server_upto++]=server;
        server=server->next;
    }
    SDL_mutexV(lobby_connection->game_servers_mutex);
    total_displayed_servers=disp_server_upto;

    IRCChatMessage *cm=lobby_connection->chat_messages.getFront();
    y=server_list_end_y;
    for(; cm!=NULL && y<chat_list_end_y; y+=Surface::getFontHeight()) {
        char mess_str[256];
        snprintf(mess_str,sizeof(mess_str),"%s:%s",
            (const char *)cm->user,(const char *)cm->mess);
        clientArea.bltString(iXY(4,y),mess_str, Color::white);
        cm=cm->next;
    }
    
    View::doDraw(viewArea, clientArea);
}




int IRCLobbyView::lMouseUp(const iXY &down_pos,const iXY &up_pos)
{
    assert(this!=NULL);
    int idx_down=down_pos.y/Surface::getFontHeight();
    int idx=up_pos.y/Surface::getFontHeight();
    if(idx>=0 && idx_down==idx && idx<total_displayed_servers) {
        GameServer *server=displayed_servers[idx];
        assert(server!=NULL);

        // connect to this game
        IPAddressView::szServer.setString(server->host);
    }
    return View::lMouseUp(down_pos,up_pos);
}



// connect to remote irc
//---------------------------------------------------------------------------
void IRCLobbyView::startIRC()
{
    IPaddress addr;
    stopIRC();
    if(SDLNet_ResolveHost(&addr,"irc.freenode.net",6667)==0) {
        lobby_connection=new IRCLobby(&addr);
        lobby_connection->startMessagesThread();
    }
    else { LOG(("cannot find irc host\n")); }
}

void IRCLobbyView::stopIRC()
{
    if(lobby_connection!=NULL) {
        lobby_connection->stopThread();
        delete lobby_connection;
        lobby_connection=NULL;
        LOG(("stopped irc"));
    }
}

