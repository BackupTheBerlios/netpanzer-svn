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

#include "Util/Log.hpp"
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
    change_name=0;
    skipChatLines=0;
    lobby_view_height=184;
    total_displayed_servers=0;
    setSearchName("IRCLobbyView");
    setTitle("Lobby");
    setSubTitle("");

    setAllowResize(false);
    setAllowMove(false);
    setVisible(false);
    topViewableItem=0;

    moveTo(iXY(bodyTextRect.max.x-440, bodyTextRect.min.y + 170));

    iXY  area_size = iXY(440, lobby_view_height);
    resizeClientArea(area_size);

    int chat_y=lobby_view_height-(Surface::getFontHeight()*2);
    addButtonCenterText(iXY(getClientRect().getSizeX()-80, chat_y), 80,  "Refresh", "", buttonRefresh);
    szChat.init("  ", 255,39);
    cInputField* input = addInputField(iXY(4, chat_y), &szChat, "", true);
    input->setReturnAction(chatReturnPressed);

    server_list_end_y=(Surface::getFontHeight()*6);
    chat_list_end_y=getClientRect().getSizeY()-(Surface::getFontHeight()*2);
    server_list_end_x=(getClientRect().getSizeX()-12);

    iXY size(12, 12);
    iXY pos(getClientRect().getSizeX() - size.x, 0);
    serverUpButton.setLabel("-");
    serverUpButton.setBounds(iRect(pos, pos + size));
    add(&serverUpButton);

    pos.y= server_list_end_y-size.y;
    serverDownButton.setLabel("+");
    serverDownButton.setBounds(iRect(pos, pos + size));
    add(&serverDownButton);

    pos.y+=size.y;
    chatUpButton.setLabel("-");
    chatUpButton.setBounds(iRect(pos, pos + size));
    add(&chatUpButton);

    pos.y = chat_list_end_y-size.y;
    chatDownButton.setLabel("+");
    chatDownButton.setBounds(iRect(pos, pos + size));
    add(&chatDownButton);

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
    total_displayed_servers=0;

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
        int upto=0;
        for(i=serverlist->begin(); i!=serverlist->end(); i++) {
            if(upto>=topViewableItem) { break; }
            upto++;
        }
        for(; i!=serverlist->end(); i++) {
            const GameServer* server = &(*i);

            std::stringstream playerstr;
            playerstr << server->playercount << "/" << server->max_players;
         
            clientArea.bltString(iXY(0,y),server->real_user.c_str(), Color::white);
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
    int skipped=0;
    for(m = lobby_connection->chat_messages.rbegin();
        skipped < skipChatLines && m != lobby_connection->chat_messages.rend(); m++,skipped++) {
    }
    for(; m != lobby_connection->chat_messages.rend(); m++) {

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
    int fontHeight=Surface::getFontHeight();
    int down_y=(down_pos.y-2);
    int up_y=(up_pos.y-2);
    if(up_pos.y<0 || down_pos.y<0) { return View::lMouseUp(down_pos,up_pos); }
    int idx_down=down_y/fontHeight;
    int idx=up_y/fontHeight;

    if(up_pos.x<server_list_end_x && down_pos.x<server_list_end_x) {
        if(idx_down==idx && idx<total_displayed_servers) {
            const GameServer *server=displayed_servers[idx];
            assert(server!=0);

            // connect to this game
            std::stringstream server_host;
            server_host << server->host.c_str() <<":"<<server->port;
            IPAddressView::szServer.setString(server_host.str().c_str());
        }
    }
    return View::lMouseUp(down_pos,up_pos);
}


void IRCLobbyView::actionPerformed(mMouseEvent me)
{
    if (me.getID() == mMouseEvent::MOUSE_EVENT_CLICKED) {
        if (me.getSource(serverUpButton)) {
            if (--topViewableItem < 0) {
                topViewableItem = 0;
            }
        } else if (me.getSource(serverDownButton)) {
            int max_size=lobby_connection->game_servers->size()-1;
            ++topViewableItem;
            if (topViewableItem >= max_size) {
                topViewableItem = max_size;
            }
        } else if (me.getSource(chatUpButton)) {
            if(++skipChatLines>=((int)lobby_connection->chat_messages.size())) {
                skipChatLines--;
            }
        } else if (me.getSource(chatDownButton)) {
            if (--skipChatLines <= 0) {
                skipChatLines = 0;
            }
        }
    }
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
        lobby_connection->change_name=change_name;
    } catch(std::exception& e) {
        LOG(("Couldn't connect to irc lobby: %s", e.what()));
        error_message = e.what();
    }
}

void IRCLobbyView::stopIRC()
{
    if(lobby_connection) {
        lobby_connection->stopThread();
        delete lobby_connection;
    }
    lobby_connection=0;
}

void IRCLobbyView::restartIRC()
{
        // the crude method for nickname change...
        stopIRC();
        SDL_Delay(500);
        startIRC();
}

IRCLobbyView* lobby_view = 0;
